#ifndef __Laser__
#define __Laser__

#include "main.h"

typedef __PACKED_STRUCT
{
    uint8_t Head;
    uint8_t ID;
    uint8_t Type;
    uint8_t CMD;
    uint8_t Reserved;
    uint16_t Length;
    uint16_t CRC16;
} Laser_Send_StructTypedef;

typedef __PACKED_STRUCT
{
    
} Laser_Receive_StructTypedef;

void Laser_Start(UART_HandleTypeDef *huart);  
void Laser_GetData_Call_Back(uint8_t *Data);
extern int16_t Laser_mm;

#endif
