#include "Timer.h"

uint8_t gCount = 0;

void Timer2_Init(void){
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;            //一次中断1ms
    TIM_TimeBaseStructure.TIM_Prescaler =72 - 1;            //一个计数代表1us
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
//    TIM_ClearFlag(TIM2 , TIM_FLAG_Update);          //清楚更新中断，免得一打开中断立即产生中断
    TIM_ITConfig(TIM2 , TIM_IT_Update | TIM_IT_Trigger , ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void){
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        gCount++;
    }
}

