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


uint8_t keynum;


float Target,Actual,Out;
float kp = 0.4, ki = 0.2, kd = 0.2;
float Error0, Error1, Error2;

int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	Encoder_Init();
	Serial_Init();
	Timer_Init();
	
	OLED_Printf(0,0,OLED_8X16,"Location Control");
	while(1)
	{
		keynum = Key_GetNum();
		if(keynum == 1)
		{Target += 10;}
		OLED_Printf(0,16,OLED_8X16,"kp=%4.2f",kp);
		OLED_Printf(0,32,OLED_8X16,"ki=%4.2f",ki);
		OLED_Printf(0,48,OLED_8X16,"kd=%4.2f",kd);
		
		OLED_Printf(64,16,OLED_8X16,"Tar=%+04.0f",Target);
		OLED_Printf(64,32,OLED_8X16,"Act=%+04.0f",Actual);
		OLED_Printf(64,48,OLED_8X16,"Out=%+04.0f",Out);
		
		OLED_Update();
		
		Serial_Printf("%f,%f,%f\n",Target,Actual,Out);
	}
	
}
/* 定时器中断函数，可以复制到使用它的地方 */
void TIM1_UP_IRQHandler(void)
{
	static uint16_t count;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		count++;
		if(count >= 40)
		{
			count = 0;
			
			Actual += Encoder_Get1();
			Error2 = Error1;
			Error1 = Error0;
			Error0 = Target - Actual;
			Out += kp * (Error0 - Error1) + ki * Error0 + kd * (Error0 - 2 * Error1 + Error2);
			
			if(Out>100)
				Out = 100;
			if(Out < -100)
				Out = -100;
			
			Motor1_SetPWM(Out);
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
