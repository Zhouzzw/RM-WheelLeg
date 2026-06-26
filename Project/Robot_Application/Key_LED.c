//========================================================
//                      Key_LED[按键扫描与LED驱动]
//========================================================
#include "Key_LED.h"
#include "Key.h"
#include "LED.h"


void Key_LED_Task(void const * argument)
{
    Key_Init();
    LED_Init();
    
    for(;;)
    {
        Key_Scan_Task();
        LED_Task();
        
        osDelay(10);
    }
}