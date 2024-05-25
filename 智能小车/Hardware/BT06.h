#ifndef __BT06_H
#define __BT06_H

#include "stm32f10x.h"                  // Device header
#include "string.h"
#include "Serial.h"
#include "car.h"

typedef struct{
    uint8_t buf[100];
    uint8_t flag;       //接收到数据的标志位
    uint8_t len;
}TypeUsart3;
extern TypeUsart3 MyUsart3;

void USART3_Init(u32 baud);                   //串口初始化函数
void usatr3_send_data(u8 c);                  //发送一个字节的函数
void uatr3_send_data(char * data,u8 len);     //串口3发送一个字符串的函数
void BT06_Ctrl(void);

#endif

