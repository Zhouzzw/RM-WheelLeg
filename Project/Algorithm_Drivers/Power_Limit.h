#ifndef __Power_Limit__
#define __Power_Limit__

#include "main.h"
#include "Chassis.h"

extern float Power_Pre_Sum;

#define Current2Torque 					   1.90702994e-5f	// (20/16384)*(0.3)*(187/3591)
#define Torque2Current 					   5.24375615e+4f

#define K1   				1.23e-07f
#define K2   				1.453e-07f
#define Static_Power    	4.081f

float Power_Predict_Calculate(Motor_Data_StructTypeDef *Motor_Data,PID_Struct_TypeDef* Motor_PID_Struct);
float Power_Proportional_scaling(float *Power_Pre,float P_max);
void Torque_Inverse_Solution(float * Torque,float P_max_Single,Motor_Data_StructTypeDef *Motor_Data);
void Power_Limit(Chassis_Control_StructTypeDef *Chassis_Struct,Motor_Data_StructTypeDef *Motor_Data,float P_max);
void Torque_decision(Chassis_Control_StructTypeDef *Chassis_Struct,uint8_t Order);

#endif

