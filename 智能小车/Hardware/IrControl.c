#include "IrControl.h"

uint32_t IR_Receivecode;        //保存接受码
uint8_t  IR_Receiveflag;        //指示接受标志

void IrControl_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); //选择GPIO管脚用作外部中断线路
    EXTI_ClearITPendingBit(EXTI_Line8);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
}



uint8_t IRremote_Counttime(void){
    uint8_t t = 0;
    while(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_8) == 1){
        t++;
        Delay_us(20);
        if(t>=250) return t;
    }
    return t;
}


void ControlCar_Ircontrol(void) {
    uint8_t buf[2];
    uint8_t code;
    if(IR_Receiveflag == 1){
        IR_Receiveflag = 0;                //标志位清零
        printf("红外接收码 %0.8X\r\n",IR_Receivecode);
        code = IR_Receivecode>>8;
        IR_Receivecode = 0;
        buf[0] = code/16;                   //取高四位
        buf[1] = code%16;                   //取低四位
        
        printf("buf[0]:%d\r\n",buf[0]);
        printf("buf[1]:%d\r\n",buf[1]);
    }
    if(buf[0] == 1 && buf[1] == 8){
        Car_Run(70,10);  // 前进
    }
    else if(buf[0] == 4 && buf[1] == 10){
        Car_Back(70,10); // 后退
    }
    else if(buf[0] == 1 && buf[1] == 0){
        Car_Spin_Left(70,10); //左转
    }
    else if(buf[0] == 5 && buf[1] == 10){
        Car_Spin_Right(70,10); // 右转
    }
    else if(buf[0] == 3 && buf[1] == 8){
        Car_Brake(0); // 停止
    }
    else{
        Car_Brake(0); // 停止
    }
   
}


void EXTI9_5_IRQHandler(void){
    uint8_t tim = 0 , ok = 0 ,num = 0,data;
    while(1){
        if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_8) == 1){
            tim = IRremote_Counttime();
            if(tim >= 250) break;
            if(tim > 200 && tim < 250){     //高电平时间为 4000 - 5000内也就是4ms - 5ms
                ok = 1;                     //起始信号
            }else if(tim > 60 && tim < 90){ //若高电平持续时间为1200~1800us内则为数据位为1：
                data = 1;                   //数据1
            }else if(tim > 10 && tim < 50){ //若高电平持续时间为200~1000us内则为数据位为0：  560us在 200-1000us之间
                data = 0;                   //数据0
            }
            if(ok == 1){
                IR_Receivecode<<=1;
                IR_Receivecode+=data;
                if(num >= 32){
                    printf("%0.8X\r\n",IR_Receivecode);
                    IR_Receiveflag = 1;
                    printf("FFFFFFF\n");
                    break;
                }
            }
            num++;
        }
        ControlCar_Ircontrol();
    }
    EXTI_ClearITPendingBit(EXTI_Line8);
}
