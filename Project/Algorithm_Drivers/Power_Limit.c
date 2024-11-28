//===========================================================================
//                      Power_Limit[功率控制]
//		根据底盘驱动轮电机的目标力矩和转速正解算出电机功率
//		再根据最大功率逆解算出最大的力矩再加速
//      开源来源:西浦 麦克斯韦妖 开源
//		Author:WUST_MURRAY
//		作者给后人的建议:功率分配还可以再优化一下
//===========================================================================

#include "Power_Limit.h"
#include "SuperCap_Driver.h"
#include "Referee_Unpack.h"
#include "PID.h"

float Motor_Power_Pre[4];
float Motor_Power_Zoom[4];
float Power_K;
double Inverse_Torque[2];
float Torque_Current[8];
float Power_Pre_Sum;
double a;
double b;
double c;


/**
 * @brief 功率限制模型
 *
 * @param 底盘数据结构体
 * @param 电机数据结构体
 * @param 底盘最大功率
 */
void Power_Limit(Chassis_Control_StructTypeDef *Chassis_Struct,Motor_Data_StructTypeDef *Motor_Data,float P_max)
{
	/*===| 根据目标力矩和当前转速预测下一时刻的功率 |===*/
	for(uint8_t i = 0 ; i < 4 ; i++)
	{
		Motor_Power_Pre[i] = Power_Predict_Calculate(&Motor_Data[i],&Chassis_Struct->MotorID_PID_Struct[i]);
	}
	
	/*===| 计算缩放系数[P_max / Power_Pre_Sum] |===*/
	Power_K = Power_Proportional_scaling(Motor_Power_Pre,P_max);
	for(uint8_t i = 0  ;i < 4 ; i++)
	{
		if(Motor_Power_Pre[i]>0)
		{
			Motor_Power_Zoom[i] = Power_K * Motor_Power_Pre[i];
		}
	}
	
	/*===| 根据缩放后的功率，逆结算出输出的力矩 |===*/
	for(uint8_t i = 0  ;i < 4 ; i++)
	{
		Torque_Inverse_Solution(&Torque_Current[2*i],Motor_Power_Zoom[i],&Motor_Data[i]);
	}
	
	for(uint8_t i = 0 ; i<4 ; i++)
	{
		Torque_decision(Chassis_Struct,i);
	}
}	

/**
 * @brief 功率预测模型
 *
 * @param 电机数据结构体
 * @param 电机PID结构体
 */
float Power_Predict_Calculate(Motor_Data_StructTypeDef *Motor_Data,PID_Struct_TypeDef* Motor_PID_Struct)
{
	float Power_Per;
	Power_Per = (Motor_PID_Struct->Output * Current2Torque) * Motor_Data->SpeedRPM / 9.55f + K1 * Motor_PID_Struct->Output * Motor_PID_Struct->Output + K2 * Motor_Data->SpeedRPM * Motor_Data->SpeedRPM + Static_Power;
	
	return Power_Per;
}


/**
 * @brief 功率缩放计算
 *
 * @param 预测后四个电机的功率
 * @param 最大底盘总功率
 */
float Power_Proportional_scaling(float *Power_Pre,float P_max)
{
	float Proportional_Constant;
	
	Power_Pre_Sum = Power_Pre[0] + Power_Pre[1] + Power_Pre[2] + Power_Pre[3];
	Proportional_Constant = P_max / Power_Pre_Sum;
	
	if(Proportional_Constant > 1.0f || Proportional_Constant < 0.0f) Proportional_Constant = 1.0f;
	return Proportional_Constant;
}




/**
 * @brief 逆结算出功率缩放后的力矩
 *
 * @param 力矩结构体[Pos,Neg]
 * @param 单电机功率
 * @param 电机PID结构体
 * @param 电机数据结构体
 */
//void Torque_Inverse_Solution(float * Torque_Current,float P_max_Single,Motor_Data_StructTypeDef *Motor_Data)
//{
//	a = K1;
//	b = Torque2Current * Motor_Data->SpeedRPM / 9.55f;
//	c = K2 * (Motor_Data->SpeedRPM) * (Motor_Data->SpeedRPM) + Static_Power - P_max_Single;
//	
//	Inverse_Torque[0] = (-b + sqrt(b * b - 4.0f * a *c)) / (2.0f * a);
//	Inverse_Torque[1] = (-b - sqrt(b * b - 4.0f * a *c)) / (2.0f * a);
//	
//	if(Inverse_Torque[0] >= 0) 
//	{
//		Torque_Current[0] = Inverse_Torque[0];
//		Torque_Current[1] = Inverse_Torque[1];
//	}
//	else 
//	{
//		Torque_Current[0] = Inverse_Torque[1];
//		Torque_Current[1] = Inverse_Torque[0];
//	}
//	
//	Limit_float(&Torque_Current[0],16000,-16000);
//	Limit_float(&Torque_Current[1],16000,-16000);
//}

void Torque_Inverse_Solution(float * Torque_Current,float P_max_Single,Motor_Data_StructTypeDef *Motor_Data)
{
	a = K1;
	b = Current2Torque * Motor_Data->SpeedRPM / 9.55f;
	c = K2 * (Motor_Data->SpeedRPM) * (Motor_Data->SpeedRPM) + Static_Power - P_max_Single;
	
	Inverse_Torque[0] = (-b + sqrt(b * b - 4.0f * a *c)) / (2.0f * a);
	Inverse_Torque[1] = (-b - sqrt(b * b - 4.0f * a *c)) / (2.0f * a);
	
	Torque_Current[0] = Inverse_Torque[0];
	Torque_Current[1] = Inverse_Torque[1];
	
	Limit_float(&Torque_Current[0],16000,-16000) ;
	Limit_float(&Torque_Current[1],16000,-16000) ;
}

/**
 * @brief 力矩决策
 */
void Torque_decision(Chassis_Control_StructTypeDef *Chassis_Struct,uint8_t Order)
{
	if(Chassis_Struct->MotorID_PID_Struct[Order].Output >0)
	{
		Chassis_Struct->MotorID_PID_Struct[Order].Output = Torque_Current[2*Order];
	}
	else 
	{
		Chassis_Struct->MotorID_PID_Struct[Order].Output = Torque_Current[2*Order+1];
	}
}

