#ifndef __Motor_Driver__
#define __Motor_Driver__

#include "main.h"

/*===| 电机反馈数据结构体定义 |===*/
typedef struct
{
    int16_t Encoder;        //编码器值
    int16_t Encoder_Last;   //上一次的编码器值
    int Round;          //总圈数
    float Total_Angle;      //总角度值
	int16_t SpeedRPM;       //转速[RPM]
	int16_t Torque;         //力矩[电流]
    int8_t Temperature;     //电机温度
} Motor_Data_StructTypeDef;

/*===| 电机数据结构体 |===*/
extern Motor_Data_StructTypeDef Motor_Data_Struct[10];


void Motor_Init(void);

void Motor_DJI_SendCurrent(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, int16_t ID1_Currnet, int16_t ID2_Currnet, int16_t ID3_Currnet, int16_t ID4_Currnet);

void Motor_DJI_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *M2006_FeedbackData_StructTypeDef);

/*===| 达妙相关驱动 |===*/
#define P_MIN   -12.5
#define P_MAX   12.5 
#define V_MIN   -30  
#define V_MAX   30   
#define KP_MIN  0    
#define KP_MAX  500  
#define KD_MIN  0    
#define KD_MAX  5    
#define T_MIN   -10
#define T_MAX   10  


void Motor_DM_CMD_Enable(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);

void Motor_DM_CMD_Disable(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);

void Motor_DM_CMD_SetZero(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);

void Motor_DM_CMD_ClearErr(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID);
    
void Motor_DM_CMD_MIT(FDCAN_HandleTypeDef* hfdcan,uint16_t CAN_ID, float _pos, float _vel,float _KP, float _KD, float _torq);

void Motor_DM_CMD_Position(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float Position, float Speed);

void Motor_DM_CMD_Speed(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float Speed);

void Motor_DM_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_StructTypeDef);


float DM_uint_to_float(int x_int, float x_min, float x_max, int bits);
int DM_float_to_uint(float x, float x_min, float x_max, int bits);
  
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

void Unitree_6010_Send_Control_Data(UART_HandleTypeDef *huart, uint8_t ID, float Position, float Speed, float Torque, float Kp, float Kd);

#endif
