#include "stm32f10x.h"                  // Device header

void ReflectiveInfraredSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
uint8_t ReflectiveInfraredSensor_Get1(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
}
uint8_t ReflectiveInfraredSensor_Get2(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);
}
uint8_t ReflectiveInfraredSensor_Get3(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
}
