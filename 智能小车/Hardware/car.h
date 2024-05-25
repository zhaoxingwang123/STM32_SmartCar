#ifndef __CAR_H
#define __CAR_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

#include "car.h"

void Car_Init(void);
void Car_SetSpeed(uint8_t left1_speed,uint8_t left2_speed,uint8_t right1_speed,uint8_t right2_speed);
void Car_Run(int8_t speed , uint16_t time);         //ǰ��
void Car_Brake(uint16_t time);                      //ɲ��
void Car_Left(int8_t speed , uint16_t time);        //��ת
void Car_Spin_Left(int8_t speed , uint16_t time);   //����
void Car_Right(int8_t speed , uint16_t time);       //��ת
void Car_Spin_Right(int8_t speed , uint16_t time);  //����
void Car_Back(int8_t speed , uint16_t time);        //����


#endif

