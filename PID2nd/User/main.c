#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "motor.h"
#include "pwm.h"
#include "Encoder.h"
#include "serial.h"
#include <stdlib.h>
#include <string.h>
uint8_t keynum;
uint8_t mode = 0;

float Target = 0,Actual,Out;
float kp1 = 0.6, ki1 = 0.3, kd1 = 0.3;
float kp2, ki2, kd2;
float Error0, Error1, Error2;
float location1, location2;
float l_out;
float le0, le1, le2;

int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	Encoder_Init();
	Serial_Init();
	Timer_Init();
	
	while(1)
	{
		keynum = Key_GetNum();
		if (keynum == 1)
		{
			mode++;
			mode %= 2;
		}
		if (mode == 0)
		{
			if(Serial_RxFlag == 1)
			{
				Target = (float)atof(Serial_RxPacket + 5);
				Serial_RxFlag = 0;
			}
			OLED_Printf(0,0,OLED_8X16,"Test1");
		}
		else
		{
			OLED_Printf(0,0,OLED_8X16,"Test2");
		}
		OLED_Update();
		Serial_Printf("%f,%f,%f\r\n", Target, Actual, Out);
	}
	
}
/* 定时器中断函数，可以复制到使用它的地方 */
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		if(mode == 0){
			Actual = Encoder_Get1();
			Error2 = Error1;
			Error1 = Error0;
			Error0 = Target - Actual;
			Out += kp1 * (Error0 - Error1) + ki1 * Error0
					+ kd1 * (Error0 - 2 * Error1 + Error2);
			if (Out > 100) {Out = 100;}
			if (Out < -100) {Out = -100;}
			
			Motor1_SetPWM(Out);
		}
		else
		{
			location1 = (int16_t)(TIM3->CNT);
			location2  = (int16_t)(TIM4->CNT);
			le2 = le1;
			le1 = le0;
			le0 = location1 - location2;
			l_out = kp2 * (le0 - le1) + ki2 * le0 + kd2 *(le0 + le2 - 2 * le1);
			if (l_out > 100) {l_out = 100;}
			if (l_out < -100) {l_out = -100;}
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

