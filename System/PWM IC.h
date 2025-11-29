#include "stm32f10x.h"                  // Device header

#ifndef __PWMIC_H
#define __PWMIC_H

void IC_Init(void);
uint32_t IC_GetFrequency(void);
uint32_t IC_GetDuty(void);

#endif
