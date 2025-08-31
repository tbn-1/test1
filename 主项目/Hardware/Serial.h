#ifndef __SERIAL_H
#define __SERISL_H

#include <stdio.h>

extern uint8_t Serial_RxFlag;
extern uint8_t Serial_RxData;
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X,uint32_t Y);
void Serial_SendNumber(uint16_t Number,uint8_t Length);
void Serial_Printf(char *format,...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);


#endif
