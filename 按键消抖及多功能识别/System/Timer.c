#include "stm32f10x.h"

#define MinTime (uint8_t)10	//定时器最小切分（ms）

void Timer_Init(void) 
{

    // 1. 开启TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 2. 配置定时器TIM3
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 10 * MinTime - 1; // 自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1; // 时钟预分频数
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // 3. 开启TIM3更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 4. 配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 使能定时器
    TIM_Cmd(TIM3, ENABLE);
}
