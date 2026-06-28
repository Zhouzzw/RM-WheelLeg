#include "LED.h"

//10ms
void LED_Task(void)
{
    LED_Floow_Step(); 
}


void LED_Init(void)
{
	HAL_TIM_PWM_Start(LED_TIM, LED_R_TIM_CHANNAL);
	HAL_TIM_PWM_Start(LED_TIM, LED_G_TIM_CHANNAL);
	HAL_TIM_PWM_Start(LED_TIM, LED_B_TIM_CHANNAL);
	__HAL_TIM_SetCompare(LED_TIM,LED_R_TIM_CHANNAL, 0);
	__HAL_TIM_SetCompare(LED_TIM,LED_G_TIM_CHANNAL, 0);
	__HAL_TIM_SetCompare(LED_TIM,LED_B_TIM_CHANNAL, 0);
}

void LED_ON(LED_Color Color, float Light)
{
	switch((uint8_t)Color)
	{
		case 1: __HAL_TIM_SetCompare(LED_TIM,LED_R_TIM_CHANNAL, Light * LED_TIM_ARR);break;
		case 2: __HAL_TIM_SetCompare(LED_TIM,LED_G_TIM_CHANNAL, Light * LED_TIM_ARR);break;
		case 3: __HAL_TIM_SetCompare(LED_TIM,LED_B_TIM_CHANNAL, Light * LED_TIM_ARR);break;
	}
}

void LED_OFF(LED_Color Color)
{
	switch((uint8_t)Color)
	{
		case 1: __HAL_TIM_SetCompare(LED_TIM,LED_R_TIM_CHANNAL, 0);break;
		case 2: __HAL_TIM_SetCompare(LED_TIM,LED_G_TIM_CHANNAL, 0);break;
		case 3: __HAL_TIM_SetCompare(LED_TIM,LED_B_TIM_CHANNAL, 0);break;
	}
}

/**
 * @brief HSV转RGB
 *
 * @param 色环角度
 * @param 明度
 * @param 饱和度
 */
uint8_t R_Color,G_Color,B_Color;
void HSVtoRGB(uint16_t h, uint16_t s, uint16_t v)      
{
    if(h >= 360) h = 360;

    if(s >= 100) s = 100;

    if(v >= 100) v = 100;

    int i;
    R_Color = 0;
    B_Color = 0;
    G_Color = 0;
    i = h / 60;
    int difs = h % 60; 
    float RGB_max = v * 2.55f;
    float RGB_min = RGB_max * (100 - s) / 100.0f;
    float RGB_Adj = (RGB_max - RGB_min) * difs / 60.0f; 
    // switch(i)
    // {
    // case 0:
    //     R_Color = RGB_max;
    //     G_Color = RGB_min + RGB_Adj;
    //     B_Color = RGB_min;
    //     break;

    // case 1:
    //     R_Color = RGB_max - RGB_Adj;
    //     G_Color = RGB_max;
    //     B_Color = RGB_min;
    //     break;

    // case 2:
    //     R_Color = RGB_min;
    //     G_Color = RGB_max;
    //     B_Color = RGB_min + RGB_Adj;
    //     break;

    // case 3:
    //     R_Color = RGB_min;
    //     G_Color = RGB_max - RGB_Adj;
    //     B_Color = RGB_max;
    //     break;

    // case 4:
    //     R_Color = RGB_min + RGB_Adj;
    //     G_Color = RGB_min;
    //     B_Color = RGB_max;
    //     break;

    // default:
    //     R_Color = RGB_max;
    //     G_Color = RGB_min;
    //     B_Color = RGB_max - RGB_Adj;
    //     break;
    // }
}


void LED_Floow_Step(void)
{
    static uint16_t H_BASE = 0;
    HSVtoRGB(H_BASE,100,100);
	H_BASE = (H_BASE + 4) % 360;

    LED_ON(LED_R, R_Color / 256.0f);
    LED_ON(LED_G, G_Color / 256.0f);
    LED_ON(LED_B, B_Color / 256.0f);
}


