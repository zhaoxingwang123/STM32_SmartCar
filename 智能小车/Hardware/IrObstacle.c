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
    if(Left_IrObstacle_Get() == 1 && Right_IrObstacle_Get() == 1){            //���ϰ���ǰ��
        Car_Run(70 , 400);
    }else if(Left_IrObstacle_Get() == 1 && Right_IrObstacle_Get() == 0){      //�Ҳ����ϰ�����ת
        Car_Left(70 , 400);
    }else if(Left_IrObstacle_Get() == 0 && Right_IrObstacle_Get() == 1){      //������ϰ�����ת
        Car_Right(70 , 400);
    }else if(Left_IrObstacle_Get() == 0 && Right_IrObstacle_Get() == 0){      //���߶����ϰ�����ɲ�����ˣ�������
        Car_Brake(400);
        Car_Back(70 , 400);
        Car_Spin_Right(70 , 900);
    }
}

