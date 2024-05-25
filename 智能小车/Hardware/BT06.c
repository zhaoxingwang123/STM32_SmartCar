#include "BT06.h"

TypeUsart3 MyUsart3;

void USART3_Init(uint32_t baud){
    NVIC_InitTypeDef NVIC_InitStructure;   //NVIC结构体初始化
    GPIO_InitTypeDef GPIO_InitStructure;    //GPIO结构体初始化
    USART_InitTypeDef USART_InitStructure;  //USART结构体初始化
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //串口时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //使能GPIOA时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB , &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB , &GPIO_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);        //使能串口外设接收中断
    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);        //使能串口外设空闲中断
    
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b ;
    USART_InitStructure.USART_StopBits = USART_StopBits_1 ;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3 , &USART_InitStructure);
    
    USART_Cmd(USART3 , ENABLE);
}


void usatr3_send_data(uint8_t c){                           //发送一个字节的函数
    while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));     //获取发送完成标志位
    USART_SendData(USART3,c);
}

//发送数据
void uatr3_send_data(char * data,uint8_t len){
    while(len--)
    {
        usatr3_send_data(*data);
        data++;
    }
}

void BT06_Ctrl(void){
    if(MyUsart3.flag){           //接收到一次数据了
        MyUsart3.flag=0; //清空标志位
            
        if(strcmp((const char*)MyUsart3.buf,"ONA")==0){
            printf("go forward!\r\n"); 
            Car_Run(70,0);
        }else if(strcmp((const char*)MyUsart3.buf,"ONB")==0){
            printf("go back!\r\n"); 
            Car_Back(70,0);
        }else if(strcmp((const char*)MyUsart3.buf,"ONC")==0){
            printf("go left!\r\n"); 
            Car_Left(70,0);
        }else if(strcmp((const char*)MyUsart3.buf,"OND")==0){
            printf("go right!\r\n"); 
            Car_Right(70,0);
        }else if(strcmp((const char*)MyUsart3.buf,"ONF")==0){
            printf("Stop!\r\n"); 
            Car_Brake(0);
        }else if(strcmp((const char*)MyUsart3.buf,"ONE")==0){
            printf("Stop!\r\n"); 
            Car_Brake(0);
        }
    }
}


void USART3_IRQHandler(void){
    if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET){
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
        MyUsart3.buf[MyUsart3.len++] = USART_ReceiveData(USART3);
    }
    if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET){
        MyUsart3.buf[MyUsart3.len] = '\0';
        MyUsart3.len = 0;
        MyUsart3.flag = 1;
        USART_ReceiveData(USART3);
    }
    BT06_Ctrl();
}

