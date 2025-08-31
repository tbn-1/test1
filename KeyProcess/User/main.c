#include "stm32f10x.h"                  // Device header

#include "Key.h"
#include "Timer.h"
#include "LED.h"
#include "OLED.h"

extern uint8_t Key_flag;

int main(void) 
{
	LED_Init();
	OLED_Init();
	Timer_Init();
    Key_Init();


    while (1) 
	{
		
		uint8_t value = Key_Read();
		OLED_ShowNum(1,1,value,3);
        if(value==1)
		{
			LED1_Turn();
		}
		else if(value==2)
		{
			LED2_Turn();
		}
		else
		{
			LED1_OFF();
			LED2_OFF();
		}
    }
}
