#ifndef __BT06_H
#define __BT06_H

#include "stm32f10x.h"                  // Device header
#include "string.h"
#include "Serial.h"
#include "car.h"

typedef struct{
    uint8_t buf[100];
    uint8_t flag;       //���յ����ݵı�־λ
    uint8_t len;
}TypeUsart3;
extern TypeUsart3 MyUsart3;

void USART3_Init(u32 baud);                   //���ڳ�ʼ������
void usatr3_send_data(u8 c);                  //����һ���ֽڵĺ���
void uatr3_send_data(char * data,u8 len);     //����3����һ���ַ����ĺ���
void BT06_Ctrl(void);

#endif

