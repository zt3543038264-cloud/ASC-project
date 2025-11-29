#include "stm32f10x.h"
#include <string.h>
#include <stdlib.h>

#include "Delay.h"
#include "Timer.h"
#include "PWM.h"
#include "Key.h"
#include "OLED.h"
#include "Motor.h"
#include "MotorEncoder.h"
#include "ReflectiveInfraredSensor.h"
#include "Serial.h"

uint8_t keynum;
uint8_t mode = 0;
uint8_t x1,x2,x3;
float basis1 = 0, basis2 = 0;
float kp=0.27,ki=0.09,kd=0.18;
float Error01 = 0, Error11 = 0, Error21 = 0;
float Error02 = 0, Error12 = 0, Error22 = 0;
float Target1 = 0, Target2 = 0, Out1 = 0, Out2 = 0;
float Actual1 = 0, Actual2 = 0;
int16_t place = 0, preplace;
uint8_t edit = 0;
int main(void)
{
	Key_Init();
	OLED_Init();
	Motor_Init();
	MotorEncoder_Init();
	ReflectiveInfraredSensor_Init();
	Serial_Init();
	Timer_Init();
	PWM_Init();
	while(1)
	{
		keynum = Key_GetNum();
		if (keynum == 1)
		{mode += 1;
		mode %= 2;}
		if(mode == 0)
		{Motor1_SetPWM(0);
		Motor2_SetPWM(0);
		if(keynum == 2)
			edit = 1 - edit;
		if(edit)
		{OLED_Printf(24,0,OLED_8X16,"E");
		OLED_Update();}
		else
		{OLED_Printf(24,0,OLED_8X16," ");
		OLED_Update();}
		if(keynum == 3&& !edit)
		{preplace = place;
			place += 16;
		if(place > 16)
		place = 0;}
		if(keynum == 4&& !edit)
		{preplace = place;
			place -= 16;
		if(place < 0)
		place = 16;}
		
		if(keynum == 3&& edit)
		{
			if(place == 0)
				Target1++;
			if(place == 16)
				Target2++;
		}
		if(keynum == 4&& edit)
		{
			if(place == 0)
				Target1--;
			if(place == 16)
				Target2--;
		}
		OLED_Printf(0,0,OLED_8X16,"NO");
		OLED_Printf(40,preplace, OLED_8X16, " ");
		OLED_Printf(40,place, OLED_8X16, ">");
		OLED_Update();}
		if(mode == 1)
		{
		OLED_Printf(40, 0, OLED_8X16, " ");
		OLED_Printf(40, 16, OLED_8X16, " ");
			OLED_Printf(0,0,OLED_8X16,"GO");
			x1 = ReflectiveInfraredSensor_Get1();
			x2 = ReflectiveInfraredSensor_Get2();
			x3 = ReflectiveInfraredSensor_Get3();
			if((x1 == 1&&x2 == 1&&x3 == 1)||(x1 == 0&&x2 == 1&&x3 == 0))
		 	{basis1 =-60;basis2 =60;}
			if(x1 == 1&&x2 == 1&&x3 == 0)
			{basis1 =-40;basis2 =30;}
			if(x1 == 0&&x2 == 1&&x3 == 1)
			{basis1 =-30;basis2 =40;}
			if(x1 == 1&&x2 == 0&&x3 == 0)
			{basis1 =-30;basis2 =-30;}
			if(x1 == 0&&x2 == 0&&x3 == 1)
			{basis1 =30;basis2 =30;}
			Serial_Printf("%f,%f\r\n", Actual1, Actual2);
		}
		OLED_Printf(48, 0, OLED_8X16, "Tar1:    ");
		OLED_Printf(48, 16, OLED_8X16, "Tar2:     ");
		OLED_Printf(48, 0, OLED_8X16, "Tar1:%.0f", Target1);
		OLED_Printf(48, 16, OLED_8X16, "Tar2:%.0f", Target2);
		OLED_Update();
	}
}
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		if(mode == 1)
		{Actual1 = MotorEncoder_GetSpeed1();
		Actual2 = MotorEncoder_GetSpeed2();
		Target1 = basis1;
		Target2 = basis2;
			Error21 = Error11;
			Error11 = Error01;
			Error01 = Target1 - Actual1;
			
			Error22 = Error12;
			Error12 = Error02;
			Error02 = Target2 - Actual2;
			
			Out1 += kp * (Error01 - Error11) + ki * Error01
					+ kd * (Error01 - 2 * Error11 + Error21);
			Out2 += kp * (Error02 - Error12) + ki * Error02
					+ kd * (Error02 - 2 * Error12 + Error22);
			if (Out1 > 100) {Out1 = 100;}
			if (Out1 < -100) {Out1 = -100;}
			if (Out2 > 100) {Out2 = 100;}
			if (Out2 < -100) {Out2 = -100;}
			
			Motor1_SetPWM(Out1);
			Motor2_SetPWM(Out2);
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

