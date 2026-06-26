#ifndef __Motor_DJI_Driver__
#define __Motor_DJI_Driver__

#include "main.h"

void Motor_DJI_SendCurrent(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, int16_t ID1_Currnet, int16_t ID2_Currnet, int16_t ID3_Currnet, int16_t ID4_Currnet);

void Motor_DJI_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *DJI_Motor_Data_Struct);

#endif