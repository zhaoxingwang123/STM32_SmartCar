#include "IrObstacle.h"

void IrObstacle_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

uint8_t Left_IrObstacle_Get(void){
    return GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_11);
}

uint8_t Right_IrObstacle_Get(void){
    return GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_12);
}

void Obstacle_Avoidance(void){
    if(Left_IrObstacle_Get() == 1 && Right_IrObstacle_Get() == 1){            //无障碍，前进
        Car_Run(70 , 400);
    }else if(Left_IrObstacle_Get() == 1 && Right_IrObstacle_Get() == 0){      //右侧有障碍，左转
        Car_Left(70 , 400);
    }else if(Left_IrObstacle_Get() == 0 && Right_IrObstacle_Get() == 1){      //左侧有障碍，右转
        Car_Right(70 , 400);
    }else if(Left_IrObstacle_Get() == 0 && Right_IrObstacle_Get() == 0){      //两边都有障碍，先刹车后退，在右旋
        Car_Brake(400);
        Car_Back(70 , 400);
        Car_Spin_Right(70 , 900);
    }
}

