#ifndef __IROBSTACLE_H
#define __IROBSTACLE_H

#include "stm32f10x.h"                  // Device header
#include "car.h"

void IrObstacle_Init(void);
uint8_t Left_IrObstacle_Get(void);
uint8_t Right_IrObstacle_Get(void);
void Obstacle_Avoidance(void);     // ∫ÏÕ‚±‹’œ◊”≥Ã–Ú

#endif

