#ifndef __AHT20_H
#define __AHT20_H

void AHT20_Init(void);
void AHT20_GetData(uint32_t *RH,uint32_t *T,uint16_t *Num);

#endif
