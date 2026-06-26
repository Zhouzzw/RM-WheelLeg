//============================================================================================================================================================================
//                                              Motor_Driver[电机驱动]
//      包含了一些电机驱动相关的封装好的函数
//============================================================================================================================================================================

#include "Motor_Driver.h"

/*===| 电机数据结构体 |===*/

Motor_Data_StructTypeDef Motor_Data_Struct[14] = {0};

void Get_TotalAngle_Speed(Motor_Data_StructTypeDef *Motor_Data_Struct)
{
    float Dt = DWT_GetDeltaT(&Motor_Data_Struct->Total_Angle_DWT_Count);
    Motor_Data_Struct->Total_Angle_Speed_RPM = (Motor_Data_Struct->Total_Angle - Motor_Data_Struct->Total_Angle_Last) / Dt * 60.0f / 360.0f;
    Motor_Data_Struct->Total_Angle_Last = Motor_Data_Struct->Total_Angle;
}


