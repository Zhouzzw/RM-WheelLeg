#ifndef __USB_DRIVER__
#define __USB_DRIVER__

#include "main.h"



void USB_Init(void);

void USB_Send(uint8_t *Data, uint8_t Length);
void USB_ReceiveCallback(uint8_t *buf, uint32_t len);

void USB_Printf(const char *fmt,...);

#endif
