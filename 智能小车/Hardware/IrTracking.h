#ifndef __IRTRACKING_H
#define __IRTRACKING_H

#include "stm32f10x.h"                  // Device header
#include "car.h"

void IrTracking_Init(void);
uint8_t Left_IrTracking_Get(void);
uint8_t Right_IrTracking_Get(void);
void Car_Traction(void);

#endif
