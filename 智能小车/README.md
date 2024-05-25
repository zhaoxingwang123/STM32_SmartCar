# 基于STM32C8T6的智能小车（循迹，红外避障，超声波避障，蓝牙遥控，红外遥控）

注释：由于硬件学的不太好，所以是买的淘宝上的基础的小车零件，以及自己买了一些其他的控制模块，并且PCB原理图不是自己画的，秉承着能看懂原理图就行的心态，大部分代码都是自己一行一行敲的。（如有错误，欢迎指出）

## 一.小车基本行走功能

小车行走使用的基本的TIM4的PWM输出功能，来控制电机的驱动，通过设置寄存的CRR的比较值，来实现PWM的占空比，从而控制速度，并使用L298N电机驱动模块来连接两个电机与四路的PWM波的输出。GPIO口分别用的GPIOB的GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 | GPIO_Pin_9

L298N可以驱动两路直流电机，下面是驱动一路直流电机的状态表

| ENA  | IN1  | IN2  | 直流电机状态 |
| :--: | :--: | :--: | :----------: |
|  0   |  X   |  X   |     停止     |
|  1   |  0   |  0   |     制动     |
|  1   |  0   |  1   |     正转     |
|  1   |  1   |  0   |     反转     |
|  1   |  1   |  1   |     制动     |

## 二.循迹功能

循迹功能使用RPR220循迹传感器模块，循迹模块的基本原理就是当接收到光反射时IO口的状态为0，当接受不到光反射（离得太远或者是黑线时，因为黑线吸收光线的能力较强）IO的状态为1，根据这个原理，我只实现了基础一点的循迹功能，就是检测一边检测到黑线就调整，两边都没有就走，两边都是黑线直接停下。

使用的GPIO口为GPIOB的GPIO_Pin_12 | GPIO_Pin_13；

## 三、红外避障

红外避障功能使用两路的红外避障传感器模块，红外避障模块的基本原理为发射一个其具有一对红外线发射与接收管，发射管发射出一定频率的红外线，当检测方向遇到障碍物（反射面）时，红外线反射回来被接收管接收，经过比较器电路处理之后，绿色指示灯会亮起，同时信号输出接回输出数字信号（一个低电平信号）。所以，当左边读取到低电平时向右转，当右边读取到低电平时向左转，两边都有直接后退并转向。

使用的GPIO口为GPIOA的GPIO_Pin_11 | GPIO_Pin_12

## 四.超声波避障

超声波避障使用的是HC-SR04的超声波测距传感器模块，超声波测距的原理是通过发送和收超声波，利用时间差和声音传播速度， 计算出模块到前方障碍物的距离。STM32给超声波的Trig管脚一个10微秒的高电平，此时将触发超声波工作。超声波发射端会发送8个40KHz的方波，方波发射后遇障碍物返回到超声波接收端。 测试距离=(高电平时间*声速(340M/S))/2 或者 us/58 =厘米 或者 us/148=英寸。控制思路为将Echo作为外部中断，当Echo发出上升沿，变成高电平时开启定时器TIM2的中断，然后测出时间持续高电平的时间，关闭TIM2的中断，根据us/58得出距离。并根据测出的距离判断要不要避让。

使用的GPIO口为GPIOB14作为Echo，GPIOB15作为Trig.

## 五.蓝牙遥控

蓝牙模块我使用的是BT06，虽然他不能作为主机主动发起连接，只能作为从机被动接受连接，但是作为驱动小车，只需要让他做从机就好（主要还是HC-05有点复杂）,手机蓝牙我使用的是厂家给的apk，但是其实只要是任意一款可以发送A-Z调试的蓝牙调试软件都可以。但蓝牙连接成功后，串口会等待接受命令，将接收到的命令与你想要进行的方式条件比对即可。

蓝牙使用的GPIO口为GPIOB10作为TXD,GPIOB11作为RXD

## 六.红外遥控

红外遥控是需要根据接收到的高电平的时长来判断接收到的是什么电平。一个逻辑1传输需要2.25ms ( 560us脉冲+1680us低电平)，一个逻辑0的传输需要1.125ms ( 560us脉冲+560us低电平)。遥控接收头在收到脉冲的时候为低电平，在没有脉冲的时候为高电平，这样，我们在接收头端收到的信号为:逻辑1应该是560us低+1680us高，逻辑0应该是560us低+560us高。NEC遥控指令的数据格式为:同步码头、地址码、地址反码、控制码、控制反码。同步码由一个9ms的低电平和一个4.5ms的高电平组成，地址码、地址反码、控制码、控制反码均是8位数据格式。按照低位在前，高位在后的顺序发送。采用反码是为了增加传输的可靠性(可用于校验)     

红外遥控使用的GPIO口为GPIOA8并将其作为外部中断。

## 七.其他辅助模块与主函数

辅助模块主要用于判断当前是哪个功能独立按键模块，用于测试超声波测距的蜂鸣器模块以及用于调试的串口模块，由于调试的串口模块主要的作用时将printf输出到串口调试助手中（改写printf实际上是改写fputc,想要定义自己的printf需要使用变参函数），由于我对怎么改写printf以及c语言的变参数函数不是特别熟练，所以我是借鉴的别人的代码。

主函数代码：

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "car.h"
#include "LEDSEG.h"
#include "IrObstacle.h"
#include "IrTracking.h"
#include "IrControl.h"
#include "Serial.h"
#include "BT06.h"
#include "Buzzer.h"
#include "Timer.h"
#include "UltrasonicWave.h"

unsigned char tmp = 1;             //模式

int main(void){
    Car_Init();
    Key_Init();
    LEDSEG_Init();
    IrObstacle_Init();
    IrTracking_Init();
    IrControl_Init();
    Serial_Init();
    USART3_Init(9600);
    Buzzer_Init();
    Timer2_Init();
    UltrasonicWave_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    while(1){
        EXTI->IMR &= ~(EXTI_Line8);     //暂时关闭中断，不影响其他功能
        EXTI->IMR &= ~(EXTI_Line14);    //暂时关闭中断
        USART_ITConfig(USART3 , USART_IT_RXNE ,DISABLE);    //暂时关闭中断
        if(Key_GetNum() == 1){
            tmp++;
        }
        if(tmp == 3){
            EXTI->IMR |= EXTI_Line8;        //暂时开启该中断
        }else if(tmp == 4){
            USART_ITConfig(USART3 , USART_IT_RXNE ,ENABLE);     //暂时开启该中断
        }else if(tmp == 5){
            EXTI->IMR |= EXTI_Line14;    //暂时开启该中断
        }
        if(tmp > 5) tmp = 1;
        switch(tmp){
            case 1:
                Digital_Display(1);
                Car_Traction();             //红外循迹
                break;
            case 2:
                Digital_Display(2);
                //Obstacle_Avoidance();       //红外避障
                break;
            case 3:
                Digital_Display(3);         //红外遥控
                break;
            case 4:
                Digital_Display(4);
                break;
            case 5:
                Digital_Display(5);
                UltrasonicWave_Ctrl();
                break;
            default:
                break;
        }
    }
    return 0;
}
```

