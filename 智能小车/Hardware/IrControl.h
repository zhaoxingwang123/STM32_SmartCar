#ifndef __IRCONTROL_H
#define __IRCONTROL_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "car.h"

extern uint32_t IR_Receivecode;
extern uint8_t  IR_Receiveflag;


void IrControl_Init(void);
uint8_t IRremote_Counttime(void);
void ControlCar_Ircontrol(void);

#endif

