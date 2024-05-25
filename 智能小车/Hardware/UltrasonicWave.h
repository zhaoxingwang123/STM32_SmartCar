#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Delay.h"
#include "Serial.h"
#include "Buzzer.h"
#include "car.h"

void UltrasonicWave_Init(void);
uint32_t Get_EchoTime(void);
int UltrasonicWave_StartMeasure(void);
void UltrasonicWave_Ctrl(void);

#endif
