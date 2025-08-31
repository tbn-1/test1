#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AHT20.h"
#include "Motor.h"
#include "LED.h"
#include "Key.h"
#include <string.h>
#include "Serial.h"

uint8_t keynum,MODE=0,Speed;
uint16_t Num=0;
uint32_t RH=12,T=10;

void Change_Mode(void)
{
	if(MODE==0){MODE=1;}
	else{MODE=0;}
}

int main (void)
{
	OLED_Init();                  
	AHT20_Init();
	Motor_Init();
	LED_Init();
	Key_Init();
	Serial_Init();
	
	OLED_ShowString(1,1,"State:0000::");
	OLED_ShowString(2,1,"Mode:");
	OLED_ShowString(2,8,"Speed:");
	OLED_ShowString(3,1,"RH:0000%");
	OLED_ShowString(4,1," T:0000^C");
	
	
    while (1)
    {
		if(Key_GetNum()){Change_Mode();}
		OLED_ShowNum(2,6,MODE,1);
		AHT20_GetData(&RH,&T,&Num);
		OLED_ShowNum(1,7,Num,4);
		OLED_ShowNum(3,4,RH,4);
		OLED_ShowNum(4,4,T,4);
		if(MODE ==1)
		{
			LED_ON();
			
			if(RH>65||T>35)
			{
				Motor_SetSpeed(100);
				OLED_ShowNum(2,14,100,3);
			}
			else if(RH>30||T>20)
			{
				Motor_SetSpeed(50);
				OLED_ShowNum(2,14,50,3);
			}
			else
			{
				Motor_SetSpeed(20);
				OLED_ShowNum(2,14,20,3);
			}
		}
		else
		{
			LED_OFF();
			if(Serial_GetRxFlag()==1)
			{
				Speed=Serial_GetRxData();
				Motor_SetSpeed(Speed);
				OLED_ShowNum(2,14,Speed,3);
			}
		}
    }
}
