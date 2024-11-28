//===========================================================================
//                      Chassis[底盘控制]
//      根据云台发下来的Vx,Vy,Wz,YawErr进行运动解算，实现底盘的各种运动状态
//      以及根据裁判系统反馈数据和[超电状态]实现功率控制
//===========================================================================

#include "Chassis.h"
#include "RoboControl.h"
#include "Power_Limit.h"

/*===| 底盘控制数据结构体 |===*/
Chassis_Control_StructTypeDef Chassis_Control_Struct;

/**
 * @brief 底盘任务
 */
void Chassis_Task(void *argument)
{
    /*===| 底盘PID参数初始化 |===*/
    PID_Init(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor1_ID], Chassis_PID_Kp, Chassis_PID_Ki, Chassis_PID_Kd, 0, Chassis_PID_I_Output_Max, Chassis_PID_Output_Max);
    PID_Init(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor2_ID], Chassis_PID_Kp, Chassis_PID_Ki, Chassis_PID_Kd, 0, Chassis_PID_I_Output_Max, Chassis_PID_Output_Max);
    PID_Init(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor3_ID], Chassis_PID_Kp, Chassis_PID_Ki, Chassis_PID_Kd, 0, Chassis_PID_I_Output_Max, Chassis_PID_Output_Max);
    PID_Init(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor4_ID], Chassis_PID_Kp, Chassis_PID_Ki, Chassis_PID_Kd, 0, Chassis_PID_I_Output_Max, Chassis_PID_Output_Max);
    
    PID_Init(&Chassis_Control_Struct.Energy_Buffer_PID, Energy_PID_Kp, Energy_PID_Ki, Energy_PID_Kd, 0, Energy_PID_I_Output_Max, Energy_PID_Output_Max);

    
    for(;;)
    {
        /*===| 得到Yaw偏移弧度，用于小陀螺时移动控制 |===*/
        Chassis_Control_Struct.Yaw_ErrAngle =  RoboControl_Struct.Yaw_Err * PI / 180.0f;
        
        /*===| 得到底盘各个方向的速度且缓启急停 |===*/
        Chassis_Speed_Addto(RoboControl_Struct.Robo_Target_Vx, &Chassis_Control_Struct.Chassis_Vx_Set);
        Chassis_Speed_Addto(RoboControl_Struct.Robo_Target_Vy, &Chassis_Control_Struct.Chassis_Vy_Set);
        
        /*===| 冲刺模式关闭横向和旋转运动，前进速度增加 |===*/
        if(RoboControl_Struct.Chassis_State == Chassis_DASH)
        {
            Chassis_Control_Struct.Chassis_Vx = 0;
            Chassis_Control_Struct.Chassis_Vy = Chassis_Dash_Speed_Multiple * Chassis_Control_Struct.Chassis_Vy_Set * cos(Chassis_Control_Struct.Yaw_ErrAngle) + Chassis_Control_Struct.Chassis_Vx_Set * sin(Chassis_Control_Struct.Yaw_ErrAngle);
            Chassis_Control_Struct.Chassis_Wz = 0;
        }
        else
        {
            Chassis_Control_Struct.Chassis_Vx = Chassis_Control_Struct.Chassis_Vx_Set * cos(Chassis_Control_Struct.Yaw_ErrAngle) + Chassis_Control_Struct.Chassis_Vy_Set * sin(Chassis_Control_Struct.Yaw_ErrAngle);
            Chassis_Control_Struct.Chassis_Vy = Chassis_Control_Struct.Chassis_Vy_Set * cos(Chassis_Control_Struct.Yaw_ErrAngle) - Chassis_Control_Struct.Chassis_Vx_Set * sin(Chassis_Control_Struct.Yaw_ErrAngle);
            Chassis_Control_Struct.Chassis_Wz = RoboControl_Struct.Robo_Target_Wz;
        }
        
        /*===| 里程计积分 |===*/
        float Self_Vx = (Chassis_RPM_to_m_s * 0.25f) * (+ Motor_Data_Struct[Chassis_Motor1_ID].SpeedRPM + Motor_Data_Struct[Chassis_Motor2_ID].SpeedRPM - Motor_Data_Struct[Chassis_Motor3_ID].SpeedRPM - Motor_Data_Struct[Chassis_Motor4_ID].SpeedRPM);
        float Self_Vy = (Chassis_RPM_to_m_s * 0.25f) * (+ Motor_Data_Struct[Chassis_Motor1_ID].SpeedRPM - Motor_Data_Struct[Chassis_Motor2_ID].SpeedRPM - Motor_Data_Struct[Chassis_Motor3_ID].SpeedRPM + Motor_Data_Struct[Chassis_Motor4_ID].SpeedRPM);
        float Self_Wz = (Chassis_RPM_to_m_s * 0.25f) * (+ Motor_Data_Struct[Chassis_Motor1_ID].SpeedRPM + Motor_Data_Struct[Chassis_Motor2_ID].SpeedRPM + Motor_Data_Struct[Chassis_Motor3_ID].SpeedRPM + Motor_Data_Struct[Chassis_Motor4_ID].SpeedRPM) / (Chassis_A_Distance + Chassis_B_Distance);
        float Dt = DWT_GetDeltaT(&Chassis_Control_Struct.Odometer_DWT_Count);
        
        Chassis_Control_Struct.Odometer_X += Self_Vx * Dt;
        Chassis_Control_Struct.Odometer_Y += Self_Vy * Dt;
        Chassis_Control_Struct.Odometer_Angle += Self_Wz * Dt * (180.0f / PI);
            
        /*===| 运动学解算[麦轮] |===*/
        Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor1_ID] = ( Chassis_Control_Struct.Chassis_Vy + Chassis_Control_Struct.Chassis_Vx) + (Chassis_A_Distance + Chassis_B_Distance) * Chassis_Control_Struct.Chassis_Wz;
        Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor2_ID] = (-Chassis_Control_Struct.Chassis_Vy + Chassis_Control_Struct.Chassis_Vx) + (Chassis_A_Distance + Chassis_B_Distance) * Chassis_Control_Struct.Chassis_Wz;
        Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor3_ID] = (-Chassis_Control_Struct.Chassis_Vy - Chassis_Control_Struct.Chassis_Vx) + (Chassis_A_Distance + Chassis_B_Distance) * Chassis_Control_Struct.Chassis_Wz;
        Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor4_ID] = ( Chassis_Control_Struct.Chassis_Vy - Chassis_Control_Struct.Chassis_Vx) + (Chassis_A_Distance + Chassis_B_Distance) * Chassis_Control_Struct.Chassis_Wz;
        
        /*===| PID计算得到每个电机的电流大小 |===*/
        PID_Position_Calculate(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor1_ID], Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor1_ID], Motor_Data_Struct[Chassis_Motor1_ID].SpeedRPM * Chassis_RPM_to_m_s);
        PID_Position_Calculate(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor2_ID], Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor2_ID], Motor_Data_Struct[Chassis_Motor2_ID].SpeedRPM * Chassis_RPM_to_m_s);
        PID_Position_Calculate(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor3_ID], Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor3_ID], Motor_Data_Struct[Chassis_Motor3_ID].SpeedRPM * Chassis_RPM_to_m_s);
        PID_Position_Calculate(&Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor4_ID], Chassis_Control_Struct.MotorID_SpeedSet[Chassis_Motor4_ID], Motor_Data_Struct[Chassis_Motor4_ID].SpeedRPM * Chassis_RPM_to_m_s);
        
//        /*===| 底盘功率限制 |===*/
//        Chassis_Power_Limit();
        /*===| 缓冲区能量环+电机功率预测模型实现功率控制 |===*/
        Chassis_Control_Struct.Chassis_Power_MAX = Robo_State.chassis_power_limit;
        PID_Position_Calculate(&Chassis_Control_Struct.Energy_Buffer_PID, 40.0f, Robo_PowerHeatData.buffer_energy);
        Chassis_Control_Struct.Chassis_Power_MAX = Robo_State.chassis_power_limit - Chassis_Control_Struct.Energy_Buffer_PID.Output;
        if(RoboControl_Struct.SuperCap_V >= 8 && RoboControl_Struct.SuperCap_V <= 10)Chassis_Control_Struct.Chassis_Power_MAX *= ( RoboControl_Struct.SuperCap_V-8) / 2.0f; 
        Power_Limit(&Chassis_Control_Struct,Motor_Data_Struct,Chassis_Control_Struct.Chassis_Power_MAX);
        
        /*===| 发送电流参数 |===*/
        if(RoboControl_Struct.Robo_Enable) 
        {
            Motor_DJI_SendCurrent(&Chassis_Motor_CAN, Chassis_Motor_Send_CAN_ID,    
                                  Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor1_ID].Output,
                                  Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor2_ID].Output,
                                  Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor4_ID].Output,
                                  Chassis_Control_Struct.MotorID_PID_Struct[Chassis_Motor3_ID].Output);
        }
        else 
        {
            Motor_DJI_SendCurrent(&Chassis_Motor_CAN, Chassis_Motor_Send_CAN_ID, 0, 0, 0, 0);   
        }                                                                  
        
        osDelay(1);
    }
}

/**
 * @brief 速度渐变
 */
void Chassis_Speed_Addto(float Set, float* Chassis_Speed)
{
    if      (Set > *Chassis_Speed && *Chassis_Speed >= 0) *Chassis_Speed += Chassis_Acc / 1000.0f;
    else if (Set < *Chassis_Speed && *Chassis_Speed <= 0) *Chassis_Speed -= Chassis_Acc / 1000.0f;
    else if (Set < *Chassis_Speed && *Chassis_Speed > 0) *Chassis_Speed -= Chassis_Dec / 1000.0f;
    else if (Set > *Chassis_Speed && *Chassis_Speed < 0) *Chassis_Speed += Chassis_Dec / 1000.0f;
}


///**
// * @brief 功率限制
// */
//#include "Referee_Unpack.h"
//#define Chassis_PowerBuffer_Energe_Max 60.0f   //缓冲区最大能量60J 有超电的车这里改成50J，因为超电能量环期望为55J
//#define PID_Total_Out_Max 40000.0f             //底盘最大输出PID

//void Chassis_Power_Limit(void)
//{    
//    /*===| 得到缓冲区能量百分比 |===*/
//    Chassis_Control_Struct.PowerBuffer_K = Robo_PowerHeatData.buffer_energy / Chassis_PowerBuffer_Energe_Max;
//    if(Chassis_Control_Struct.PowerBuffer_K > 1) 
//        Chassis_Control_Struct.PowerBuffer_K = 1;
////    else if(Chassis_Control_Struct.PowerBuffer_K > 0.5f && Chassis_Control_Struct.PowerBuffer_K <= 1)
////        Chassis_Control_Struct.PowerBuffer_K = Chassis_Control_Struct.PowerBuffer_K;
////    else if(Chassis_Control_Struct.PowerBuffer_K > 0.25f && Chassis_Control_Struct.PowerBuffer_K <= 0.5f)
////        Chassis_Control_Struct.PowerBuffer_K = 2 * Chassis_Control_Struct.PowerBuffer_K - 0.5f;
//    else 
////        Chassis_Control_Struct.PowerBuffer_K = 0;
//    Chassis_Control_Struct.PowerBuffer_K = Chassis_Control_Struct.PowerBuffer_K * Chassis_Control_Struct.PowerBuffer_K;
//    /*===| 缓冲区能量不足时减缓小陀螺转速，保证全向移动稳定 |===*/
//    Chassis_Control_Struct.Chassis_Wz *= Chassis_Control_Struct.PowerBuffer_K;
//    
//    /*===| 冲刺模式减少前轮输出 |===*/
//    if(RoboControl_Struct.Chassis_State == Chassis_DASH) 
//    {
//        Chassis_Control_Struct.MotorID_PID_Struct[0].Output *= 0.3f;
//        Chassis_Control_Struct.MotorID_PID_Struct[1].Output *= 0.3f;
//    }
//    
//    /*===| 得到限制后的最大PID输出值 |===*/
//    float PID_Total_Out_Limit = Chassis_Control_Struct.PowerBuffer_K * PID_Total_Out_Max;
//    
//    /*===| 限制PID输出 |===*/
//    float PID_Total_Out =   fabs(Chassis_Control_Struct.MotorID_PID_Struct[0].Output) + 
//                            fabs(Chassis_Control_Struct.MotorID_PID_Struct[1].Output) + 
//                            fabs(Chassis_Control_Struct.MotorID_PID_Struct[2].Output) + 
//                            fabs(Chassis_Control_Struct.MotorID_PID_Struct[3].Output);
//    
//    float PID_Limit_K = PID_Total_Out_Limit / PID_Total_Out;
//    Limit_float(&PID_Limit_K, 1.0f, 0);
//    
//    Chassis_Control_Struct.MotorID_PID_Struct[0].Output *= PID_Limit_K;
//    Chassis_Control_Struct.MotorID_PID_Struct[1].Output *= PID_Limit_K;
//    Chassis_Control_Struct.MotorID_PID_Struct[2].Output *= PID_Limit_K;
//    Chassis_Control_Struct.MotorID_PID_Struct[3].Output *= PID_Limit_K;
//}

