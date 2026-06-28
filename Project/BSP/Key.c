#include "Key.h"

Key_Data_StructTypeDef Key_Data[3];

void Key_Scan_Task(void) //10ms
{
    Key_Data[0].Pin_Now = HAL_GPIO_ReadPin(K_L_GPIO_Port, K_L_Pin);
    Key_Data[1].Pin_Now = HAL_GPIO_ReadPin(K_R_GPIO_Port, K_R_Pin);
    Key_Data[2].Pin_Now = HAL_GPIO_ReadPin(K_P_GPIO_Port, K_P_Pin);
    
    for(uint8_t i = 0; i < 3; i++)
    {
        // if(Key_Data[i].Pin_Now == 0 && Key_Data[i].Pin_Last == 1)
        // {
        //     //按下瞬间
        //     Key_Data[i].Push_Time = 0;
        //     Key_Data[i].Release_Time = 0;
        // }
        // if(Key_Data[i].Pin_Now == 0 && Key_Data[i].Pin_Last == 0)
        // {
        //     //持续按下
        //     Key_Data[i].Push_Time++;
        //     Limit_int(&Key_Data[i].Push_Time, 110, 0);
        // }
        // if(Key_Data[i].Pin_Now == 1 && Key_Data[i].Pin_Last == 0)
        // {
        //     //松开瞬间
        //     if(Key_Data[i].Push_Time >= 100) Key_Data[i].State = LongPush;
        //     else Key_Data[i].State = ShortPush;
        // }
        // if(Key_Data[i].Pin_Now == 1 && Key_Data[i].Pin_Last == 1)
        // {
        //     //持续松开
        //     Key_Data[i].Release_Time++;
        //     Limit_int(&Key_Data[i].Release_Time, 100, 0);
        //     if(Key_Data[i].Release_Time >= 50)
        //         Key_Data[i].State = Release;
        // }
		
		Key_Data[i].Pin_Last = Key_Data[i].Pin_Now;
    }
}

void Key_Init(void)
{
    for(uint8_t i = 0; i < 3; i++)
    {
        Key_Data[i].Pin_Now = 1;
        Key_Data[i].Pin_Last = 1;     
        Key_Data[i].State = Release;
    }  
}


Key_State Get_Key_State(Key_EnumTypedef Key_ID)
{
    Key_State temp = Key_Data[Key_ID].State;
    Key_Data[Key_ID].State = Release;
    return temp;
}
