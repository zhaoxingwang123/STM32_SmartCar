#include "LEDSEG.h"

uint8_t LedShowData[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uint16_t GPIO_PIN_x[] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7};

void LEDSEG_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA , GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

void Digital_Display(uint8_t val){
    uint8_t i;
    uint16_t display_val = LedShowData[val];
    for(i = 0;i < 8;i++)
    {
        GPIO_WriteBit(GPIOA,GPIO_PIN_x[i],(BitAction)(display_val>>i & 0x01));
    }
//    uint16_t display_val = GPIO_ReadOutputData(GPIOA);
//    display_val &= 0xFF00;
//    display_val |= LedShowData[val];
//    GPIO_Write(GPIOA , display_val);
}

