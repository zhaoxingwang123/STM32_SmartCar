#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void);            //按键初始化
uint8_t Key_GetNum(void);       //按键检测

#endif
