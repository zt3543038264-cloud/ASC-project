#include "stm32f10x.h"                  // Device header
#ifndef __MOTORENCODER_H
#define __MOTORENCODER_H

void MotorEncoder_Init(void);
int16_t MotorEncoder_GetSpeed1(void);
int16_t MotorEncoder_GetSpeed2(void);
int16_t MotorEncoder_GetPosition1(void);
int16_t MotorEncoder_GetPosition2(void);

#endif
