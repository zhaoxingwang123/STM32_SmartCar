#include "Buzzer.h"

void Buzzer_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Buzzer_ON(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void Buzzer_OFF(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

void Buzzer_Turn(void)
{
    if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0) == 0)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);
    }
}
