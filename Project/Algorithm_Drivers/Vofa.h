#ifndef __VOFA__
#define __VOFA__

#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"

void USART1_Init(void);
void VOFA_Rx_CallBack(uint8_t *VOFA_RxDataBuff);
#endif
