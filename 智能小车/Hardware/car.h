#ifndef __CAR_H
#define __CAR_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

#include "car.h"

void Car_Init(void);
void Car_SetSpeed(uint8_t left1_speed,uint8_t left2_speed,uint8_t right1_speed,uint8_t right2_speed);
void Car_Run(int8_t speed , uint16_t time);         //前进
void Car_Brake(uint16_t time);                      //刹车
void Car_Left(int8_t speed , uint16_t time);        //左转
void Car_Spin_Left(int8_t speed , uint16_t time);   //左旋
void Car_Right(int8_t speed , uint16_t time);       //右转
void Car_Spin_Right(int8_t speed , uint16_t time);  //右旋
void Car_Back(int8_t speed , uint16_t time);        //后退


#endif

