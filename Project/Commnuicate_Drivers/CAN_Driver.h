#ifndef __CAN_Driver__
#define __CAN_Driver__

#include "main.h"

void CAN_Init(void);

void CAN_Send_Data(FDCAN_HandleTypeDef *hfdcan, uint16_t ID, uint8_t *TX_Data);

void CAN_Filter_Init(FDCAN_HandleTypeDef *hfdcan);

#endif
