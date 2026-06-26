//=====================================================================================
//                    PID[PID计算]
// PID_Init(PID_Struct_TypeDef *PID_Struct, float Kp, float Ki, float Kd, float Kf, float I_Output_Max, float Output_Max)
// PID_Position_Calculate(PID_Struct_TypeDef *PID_Struct, float Target, float Feedback)
//=====================================================================================

#include "PID.h"
#include "DWT.h"

/**
 * @brief PID前馈位置式计算
 *
 * @param PID数据结构体
 */
void PID_Position_Calculate(PID_Struct_TypeDef *PID_Struct, float Target, float Feedback)
{ 
    /*===| 赋值 |===*/
	 PID_Struct->Target = PID_Struct->Targer_LowPass_K * Target + (1 - PID_Struct->Targer_LowPass_K) * PID_Struct->Target_Last;
    PID_Struct->Target = Target;
    PID_Struct->Feedback = Feedback;
    
	/*===| 计算误差 |===*/
	PID_Struct->Error = Target - Feedback;
    
    /*===| 计算各部分输出 |===*/
    PID_Struct->K_Output =  PID_Struct->Kp * PID_Struct->Error;
    PID_Struct->I_Output += PID_Struct->Ki * PID_Struct->Error * DWT_GetDeltaT(&PID_Struct->DWT_Count) * 1000.0f;   //Ki->每1的误差每1msI累积量
    PID_Struct->D_Output =  PID_Struct->Kd * (PID_Struct->Error - PID_Struct->Error_Last);
    PID_Struct->F_Output =  PID_Struct->Kf * PID_Struct->Target;
    
	/*===| 积分输出限幅 |===*/
	if(PID_Struct->I_Output >  PID_Struct->I_Output_Max && PID_Struct->I_Output_Max != 0)	PID_Struct->I_Output =  PID_Struct->I_Output_Max;
	if(PID_Struct->I_Output < -PID_Struct->I_Output_Max && PID_Struct->I_Output_Max != 0)	PID_Struct->I_Output = -PID_Struct->I_Output_Max;
    
	/*===| 计算总输出 |===*/
	PID_Struct->Output = PID_Struct->K_Output + PID_Struct->I_Output + PID_Struct->D_Output + PID_Struct->F_Output;
    
	
    /*===| 输出限幅 |===*/
	if(PID_Struct->Output >  PID_Struct->Output_Max) PID_Struct->Output =  PID_Struct->Output_Max;
	if(PID_Struct->Output < -PID_Struct->Output_Max) PID_Struct->Output = -PID_Struct->Output_Max;
	
    /*===| 记录误差和输出 |===*/
	PID_Struct->Error_Last = PID_Struct->Error;
	PID_Struct->Target_Last = PID_Struct->Target;
}

/**
 * @brief PID增量式计算
 *
 * @param PID数据结构体
 */
void PID_Incress_Calculate(PID_Struct_TypeDef *PID_Struct, float Target, float Feedback)
{ 
	/*===| 计算误差 |===*/
    PID_Struct->Error_Last_Last = PID_Struct->Error_Last;
    PID_Struct->Error_Last = PID_Struct->Error;
	PID_Struct->Error = Target - Feedback;
    
    /*===| 计算各部分输出 |===*/
    PID_Struct->Output += PID_Struct->Kp * (PID_Struct->Error - PID_Struct->Error_Last)
                        + PID_Struct->Ki * PID_Struct->Error
                        + PID_Struct->Kd * (PID_Struct->Error - 2 * PID_Struct->Error_Last + PID_Struct->Error_Last_Last);
    
    /*===| 输出限幅 |===*/
	if(PID_Struct->Output >  PID_Struct->Output_Max)	PID_Struct->Output =  PID_Struct->Output_Max;
	if(PID_Struct->Output < -PID_Struct->Output_Max)	PID_Struct->Output = -PID_Struct->Output_Max;
}

/**
 * @brief PID参数初始化
 *
 * @param PID结构体
 * @param Kp, Ki, Kd, Kf, I_Output_Max, Output_Max
 */
void PID_Init(PID_Struct_TypeDef *PID_Struct, float Kp, float Ki, float Kd, float Kf, float I_Output_Max, float Output_Max)
{
    PID_Struct->Kp = Kp;
    PID_Struct->Ki = Ki;
    PID_Struct->Kd = Kd;
    PID_Struct->Kf = Kf;
    PID_Struct->I_Output_Max = I_Output_Max;
    PID_Struct->Output_Max = Output_Max;
	PID_Struct->Targer_LowPass_K = 1;
}

