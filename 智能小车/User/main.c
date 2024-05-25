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
        EXTI->IMR &= ~(EXTI_Line8);     //暂时关闭中断
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



