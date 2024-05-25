#include "IrTracking.h"

void IrTracking_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Left_IrTracking_Get(void)
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}

uint8_t Right_IrTracking_Get(void)
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
}

void Car_Traction(void){
    if(Left_IrTracking_Get() == 0 && Right_IrTracking_Get() == 0){
        Car_Run(70,50);                                                        
    }else if(Left_IrTracking_Get() == 1 && Right_IrTracking_Get() == 0){        
        Car_Left(70,50);                                                       
    }else if(Left_IrTracking_Get() == 0 && Right_IrTracking_Get() == 1){        
        Car_Right(70,50);                                                      
    }else if(Left_IrTracking_Get() == 1 && Right_IrTracking_Get() == 1){        
        Car_Brake(400);                                                           
    }
}

