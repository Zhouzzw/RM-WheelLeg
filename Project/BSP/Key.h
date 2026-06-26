#ifndef __Key__
#define __Key__

#include "main.h"

typedef enum
{
    Release = 0,
    ShortPush = 1,
    LongPush = 2,
} Key_State;

typedef struct
{
    uint8_t Pin_Now;
    uint8_t Pin_Last;
    
    int Push_Time;
    int Release_Time;
    
    Key_State State;
} Key_Data_StructTypeDef;

typedef enum
{
    K_L = 0,
    K_R = 1,
    K_P = 2,
} Key_EnumTypedef;

extern Key_Data_StructTypeDef Key_Data[3];

void Key_Init(void);

void Key_Scan_Task(void);

void Limit(uint8_t* Limit, uint8_t Max);

Key_State Get_Key_State(uint8_t Key_EnumTypedef);

#endif
