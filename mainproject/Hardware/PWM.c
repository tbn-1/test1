#include "stm32f10x.h"                  // Device header

void PWM_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	

	GPIO_InitTypeDef GPIO_InitTypeStructure;
	//时钟源是片上外设，需要使用复用推挽输出，参考GPIO结构图。
	GPIO_InitTypeStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitTypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;	//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitTypeStructure;
	TIM_OCStructInit(&TIM_OCInitTypeStructure);	//赋初始值，防止多参赋值出错。
	TIM_OCInitTypeStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitTypeStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitTypeStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeStructure.TIM_Pulse = 0;	//CCR
	TIM_OC3Init(TIM2,&TIM_OCInitTypeStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_SetCompare3(uint16_t Compare){
	TIM_SetCompare3(TIM2,Compare);
}
