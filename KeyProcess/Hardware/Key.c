#include "stm32f10x.h"                  // Device header


// 引脚定义
#define Key_Port GPIOB
#define Key_Pin GPIO_Pin_1

#define DoubleTime (uint16_t)300
#define LongTime (uint16_t)800
#define MinTime (uint16_t)10	//定时器最小切分（ms）


uint8_t Key_state=0,//0=初始/释放，1=消抖，2=按下确认
		Key_DU=0,//瞬时物理电平​（0表示按下，1表示释放）
		Key_double_timerEN=0,//是否已开始等待第二次按键按下
		Key_flag=0;//0无事件；1单击；2双击；3长按
uint16_t 	key_time=0,//按下状态的持续时间
			Key_double_time=0;//检测到第一次单击后启动计时，用于判断双击

// 按键初始化函数
void Key_Init(void) 
{
    // 开启按键GPIO端口的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置KEY1引脚（低电平有效）
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = Key_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  TIM3中断服务函数,实现单击、双击、长按的检测
  */
void TIM3_IRQHandler(void) 
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) 
	{
        Key_DU=GPIO_ReadInputDataBit(Key_Port,Key_Pin);
		switch(Key_state)
		{
			case 0:
			{
				if(Key_DU==0)
				{
					Key_state=1;
					key_time=0;
				}
				break;
			}
			case 1:
			{
				if(Key_DU==0)
				{
					Key_state=2;
				}
				else
				{
					Key_state=0;
				}
				break;
			}
			case 2:
			{
				if(Key_DU==1&&key_time<LongTime)
				{
					if(Key_double_timerEN==0)
					{
						Key_double_timerEN=1;
						Key_double_time=0;
					}
					else
					{
						Key_flag=2;
						Key_double_timerEN=0;
					}
					Key_state=0;
				}
				else if(Key_DU==1&&key_time>=LongTime)
				{
					Key_state=0;
					Key_flag=3;
				}
				else
				{
					key_time+=MinTime;
				}
				break;
			}
		}
		if(Key_double_timerEN==1)
		{
			Key_double_time+=MinTime;
			if(Key_double_time>DoubleTime)
			{
				Key_flag=1;
				Key_double_timerEN=0;
			}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}


uint8_t Key_Read(void)
{
	uint8_t value=Key_flag;
//	Key_flag=0;
	return value;
}
