#ifndef __Power_Limit__
#define __Power_Limit__

#include "main.h"
#include "Chassis.h"

extern float Power_Pre_Sum;

void Power_Limit(Chassis_Control_StructTypeDef *Chassis_Struct,Motor_Data_StructTypeDef *Motor_Data,float P_max,float *Torque_Wheel);
float Power_Predict_Calculate(Motor_Data_StructTypeDef *Motor_Data, float Wheel_Torque);
float Power_Proportional_scaling(float *Power_Pre,float P_max);
void Torque_Inverse_Solution(float * Inverse_Torque,float P_max_Single,Motor_Data_StructTypeDef *Motor_Data);
void Torque_decision(Chassis_Control_StructTypeDef *Chassis_Struct,uint8_t i,float *Torque_Wheel);

#endif

