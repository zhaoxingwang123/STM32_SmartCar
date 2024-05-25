#include "Key.h"

void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void){
    uint8_t keynum = 0;
    if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_15) == 0){
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0);
        Delay_ms(20);
        keynum = 1;
    }
    return keynum;
}


