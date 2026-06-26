#ifndef __Motor_Unitree_Driver__
#define __Motor_Unitree_Driver__

#include "main.h"
  
typedef __PACKED_STRUCT
{
    int16_t HEAD;
    int8_t ID:4;
    int8_t STATUS:3;
    int8_t None:1;
    int16_t Touque;
    int16_t Speed;
    int32_t Position;
    int16_t Kp;
    int16_t Kd;
    uint16_t CRC16;
} Unitree_6010_Control_StructTypedef;

typedef struct
{ 
    int16_t HEAD;
    int8_t ID:4;
    int8_t STATUS:3;
    int8_t None1:1;			
    int16_t Touque;
    int16_t Speed;
    int32_t Position;
    int8_t Tempture;
    uint8_t Err_Flag:3;
    int16_t Force:12;
    int8_t None2:1;
    uint16_t CRC16;
} Unitree_6010_Receive_StructTypedef;

typedef struct
{
	float Temperature;
	float Position;
	float Speed_rad_Second;
	float Torque_Current;
}MF9025_Receive_Data_StructTypedef;

void Unitree_6010_Receive_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_Struct);

void Unitree_6010_Send_Control_Data(UART_HandleTypeDef *huart, uint8_t ID, float Position, float Speed, float Torque, float Kp, float Kd);

extern  MF9025_Receive_Data_StructTypedef MF9025_Data_Struct[2];

void Unitree_6010_Receive_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data);
void MF9025_Send_Data(FDCAN_HandleTypeDef* hfdcan,uint16_t ID,float motor1_torque,float motor2_torque,float motor3_torque,float motor4_torque);
void MF9025_Receive_Data(MF9025_Receive_Data_StructTypedef *MF_Motor_Data_Struct,uint8_t *data);
void Unitree_6010_Send_Control_Data(UART_HandleTypeDef *huart, uint8_t ID, float Position, float Speed, float Torque, float Kp, float Kd);


#endif
