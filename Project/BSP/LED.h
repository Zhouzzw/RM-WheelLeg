#ifndef __LED__
#define __LED__

#include "main.h"

typedef enum{
	LED_R = 1,
	LED_G = 2,
	LED_B = 3,
}LED_Color;

void LED_Task(void);
void LED_Init(void);
void LED_ON(LED_Color Color, float Light);
void LED_OFF(LED_Color Color);
void LED_Floow_Step(void);
void HSVtoRGB(uint16_t h, uint16_t s, uint16_t v);

#endif
