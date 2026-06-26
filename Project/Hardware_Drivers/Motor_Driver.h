#ifndef __Motor_Driver__
#define __Motor_Driver__

#include "main.h"

#define Motor_Num 13

/*===| 电机数据结构体定义 |===*/
typedef struct
{
    int16_t Encoder;        //编码器值
    int16_t Encoder_Last;   //上一个编码器值
    int32_t Round;          //圈数
    float Angle;            //绝对角度   
    float Angle_Last;       //上一个绝对角度     
    float Total_Angle;      //总角度值
    float Total_Angle_Last; //总角度值
    float Total_Angle_Speed_RPM; //总角度值速度
    uint32_t Total_Angle_DWT_Count;
	float Speed_RPM;        //转速[RPM]
	float Torque;           //力矩[电流]
    int8_t Temperature;     //电机温度
	uint16_t Ticker;        //收到数据计时
    uint8_t If_Online;      //是否在线  
	uint8_t Error_ID;
} Motor_Data_StructTypeDef;

/*===| 电机数据结构体 |===*/
extern Motor_Data_StructTypeDef Motor_Data_Struct[14];

void Get_TotalAngle_Speed(Motor_Data_StructTypeDef *Motor_Data_Struct);

#endif