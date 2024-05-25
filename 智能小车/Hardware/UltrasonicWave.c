#include "UltrasonicWave.h"

float UltrasonicWave_Distance;

void UltrasonicWave_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
}

uint32_t Get_EchoTime(void){
    uint32_t t = 0;
    t = gCount * 1000;          //得到ms
    t += TIM_GetCounter(TIM2);  //得到us
    TIM2->CNT = 0;              //将计数器值清零
    Delay_ms(50);
    return t;
}

void EXTI15_10_IRQHandler(void){
    Delay_us(10);           
    uint32_t t;
    if(EXTI_GetITStatus(EXTI_Line14) != RESET){
        TIM_SetCounter(TIM2,0);
        gCount = 0;
        TIM_Cmd(TIM2, ENABLE);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));
        TIM_Cmd(TIM2, DISABLE);
        t = Get_EchoTime();
        UltrasonicWave_Distance= ((float)t/58.0);       //cm
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}

//开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
int UltrasonicWave_StartMeasure(void){
    GPIO_SetBits(GPIOB,GPIO_Pin_15);   //送>10US的高电平
    Delay_us(20);
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);
    printf("distance %f:cm\r\n",UltrasonicWave_Distance);
    return UltrasonicWave_Distance;
}

void UltrasonicWave_Ctrl(void){
    int distance;
    distance = UltrasonicWave_StartMeasure();
    printf("测到的距离值为：%d\r\n",distance);
    if(distance<40){
        Buzzer_ON();
        Car_Back(70 , 500);
        Car_Right(70 , 500);
        distance =  UltrasonicWave_StartMeasure();
    }else{
        Car_Run(70 , 1);
        Buzzer_OFF();
    }
}

