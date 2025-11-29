#include "stm32f10x.h"                  // Device header

#ifndef __REFLECTIVEINFRAREDSENSOR_H
#define __REFLECTIVEINFRAREDSENSOR_H

void ReflectiveInfraredSensor_Init(void);
uint8_t ReflectiveInfraredSensor_Get1(void);
uint8_t ReflectiveInfraredSensor_Get2(void);
uint8_t ReflectiveInfraredSensor_Get3(void);

#endif
