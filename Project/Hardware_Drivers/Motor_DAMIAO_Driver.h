#ifndef __Motor_DAMIAO_Driver__
#define __Motor_DAMIAO_Driver__

#include "main.h"

/*===| 达妙相关驱动 |===*/
#define P_MIN   -3.1415926f
#define P_MAX   3.1415926f
#define V_MIN   -45  
#define V_MAX   45   
#define KP_MIN  0    
#define KP_MAX  500  
#define KD_MIN  0    
#define KD_MAX  5    
#define T_MIN   -54
#define T_MAX   54  

typedef struct
{
    float Total_Angle;    //总角度值
	int16_t Encoder;      //编码器值
    int16_t Encoder_Last; //上一次的编码器值
    int Round;            //圈数
	float Speed;       	  //转速[RPM]
	float Torque;         //力矩[电流]
	int8_t Temperature;   //电机温度 
		
} MotorDM_Data_StructTypeDef;

void Motor_DM_CMD_Enable(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);

void Motor_DM_CMD_Disable(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);

void Motor_DM_CMD_SetZero(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);

void Motor_DM_CMD_ClearErr(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);
    
void Motor_DM_CMD_MIT(FDCAN_HandleTypeDef* hfdcan,uint16_t CAN_ID, float _pos, float _vel,float _KP, float _KD, float _torq);

void Motor_DM_CMD_Position(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float Position, float Speed);

void Motor_DM_CMD_Speed(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float Speed);

void Motor_DM_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_Struct);

void Motor_DM1to4_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_Struct);
void Motor_DM1to4_SendCurrent(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, int16_t ID1_Currnet, int16_t ID2_Currnet, int16_t ID3_Currnet, int16_t ID4_Currnet);
void Motor_DM1to4_ClearErr(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID);

extern MotorDM_Data_StructTypeDef MotorDM_Data_Struct[4];

float DM_uint_to_float(int x_int, float x_min, float x_max, int bits);
int DM_float_to_uint(float x, float x_min, float x_max, int bits);

#endif
