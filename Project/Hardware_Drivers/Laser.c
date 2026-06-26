#include "Laser.h"

Laser_Send_StructTypedef Laser_Send_Struct = {0};
int16_t Laser_mm;

void Laser_Start(UART_HandleTypeDef *huart)
{
    Laser_Send_Struct.Head = 0xA5;
    Laser_Send_Struct.ID = 0x03;
    Laser_Send_Struct.Type = 0x20;
    Laser_Send_Struct.CMD = 0x01;
    Laser_Send_Struct.CRC16 = 0x00;
    HAL_UART_Transmit(huart, (uint8_t *)&Laser_Send_Struct, sizeof(Laser_Send_StructTypedef), 1000);
}

void Laser_GetData_Call_Back(uint8_t *Data)
{
    Laser_mm = *(int16_t *)&Data[13];
}