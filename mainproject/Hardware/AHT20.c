#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "Delay.h"
#include "OLED.h"

#define MyI2C_SAck(x)	MyI2C_SendByte(x);MyI2C_ReceiveAck()
#define AHT20_Address	0x70

void AHT20_Init(void)
{
	MyI2C_Init();
	Delay_ms(40);
}

void AHT20_Reset(void)
{
	MyI2C_Start();
    MyI2C_SendByte(0x70);      
    MyI2C_ReceiveAck();
    MyI2C_SendByte(0xBE);      
    MyI2C_ReceiveAck();
    MyI2C_SendByte(0x08);       
    MyI2C_ReceiveAck();
    MyI2C_SendByte(0x00);     
    MyI2C_ReceiveAck();
    MyI2C_Stop();
    Delay_ms(200);       
}

void AHT20_GetData(uint32_t *RH,uint32_t *T,uint16_t *Num)
{
	uint8_t data[6];
	MyI2C_Start();
	MyI2C_SAck(0x70);
	MyI2C_SAck(0xAC);
	MyI2C_SAck(0x33);
	MyI2C_SAck(0x00);
	MyI2C_Stop();
	Delay_ms(80);
	
	MyI2C_Start();
	MyI2C_SAck(0x71);
	uint8_t Status;
	Status = MyI2C_ReceiveByte();
	if((Status&0x80)==0&&(Status&0x08)==0x08)
	{
		MyI2C_SendAck(0);
		OLED_ShowNum(1,13,((Status&0x80)>>7)*10+((Status&0x08)>>3),2);
	}
	else{
		*Num+=1;
		MyI2C_SendAck(1);
		MyI2C_Stop();
		OLED_ShowNum(1,13,((Status&0x80)>>7)*10+((Status&0x08)>>3),2);
		AHT20_Reset();
		return;
	}
	data[1] = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	data[2] = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	data[3] = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	*RH = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
	
	data[4] = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	data[5] = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	*T = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
	
	*RH = (*RH * 100.0) / (1 << 20);   // 湿度百分比
	*T = (*T * 200.0) / (1 << 20) - 50; // 温度摄氏度
}
