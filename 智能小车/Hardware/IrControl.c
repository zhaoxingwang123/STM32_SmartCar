#include "IrControl.h"

uint32_t IR_Receivecode;        //���������
uint8_t  IR_Receiveflag;        //ָʾ���ܱ�־

void IrControl_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); //ѡ��GPIO�ܽ������ⲿ�ж���·
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
        IR_Receiveflag = 0;                //��־λ����
        printf("��������� %0.8X\r\n",IR_Receivecode);
        code = IR_Receivecode>>8;
        IR_Receivecode = 0;
        buf[0] = code/16;                   //ȡ����λ
        buf[1] = code%16;                   //ȡ����λ
        
        printf("buf[0]:%d\r\n",buf[0]);
        printf("buf[1]:%d\r\n",buf[1]);
    }
    if(buf[0] == 1 && buf[1] == 8){
        Car_Run(70,10);  // ǰ��
    }
    else if(buf[0] == 4 && buf[1] == 10){
        Car_Back(70,10); // ����
    }
    else if(buf[0] == 1 && buf[1] == 0){
        Car_Spin_Left(70,10); //��ת
    }
    else if(buf[0] == 5 && buf[1] == 10){
        Car_Spin_Right(70,10); // ��ת
    }
    else if(buf[0] == 3 && buf[1] == 8){
        Car_Brake(0); // ֹͣ
    }
    else{
        Car_Brake(0); // ֹͣ
    }
   
}


void EXTI9_5_IRQHandler(void){
    uint8_t tim = 0 , ok = 0 ,num = 0,data;
    while(1){
        if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_8) == 1){
            tim = IRremote_Counttime();
            if(tim >= 250) break;
            if(tim > 200 && tim < 250){     //�ߵ�ƽʱ��Ϊ 4000 - 5000��Ҳ����4ms - 5ms
                ok = 1;                     //��ʼ�ź�
            }else if(tim > 60 && tim < 90){ //���ߵ�ƽ����ʱ��Ϊ1200~1800us����Ϊ����λΪ1��
                data = 1;                   //����1
            }else if(tim > 10 && tim < 50){ //���ߵ�ƽ����ʱ��Ϊ200~1000us����Ϊ����λΪ0��  560us�� 200-1000us֮��
                data = 0;                   //����0
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
