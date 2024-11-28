//======================================================================
//                      Gimbal[云台控制]
//      驱动相应电机使云台陀螺仪达到RoboControl的Pitch，Yaw目标角度
//======================================================================

#include "Gimbal.h"
#include "RoboControl.h"
#include "INS.h"

/*===| 云台控制数据结构体 |===*/
Gimbal_Control_StructTypeDef Gimbal_Control_Struct;


void Gimbal_Task(void *argument)
{
    /*===| 云台PID参数初始化 |===*/
    PID_Init(&Gimbal_Control_Struct.Yaw_Angle_PID_Struct,   Gimbal_Yaw_Angle_PID_Kp,    Gimbal_Yaw_Angle_PID_Ki,    Gimbal_Yaw_Angle_PID_Kd,    0, Gimbal_Yaw_Angle_PID_I_Output_Max,   Gimbal_Yaw_Angle_PID_Output_Max);
    PID_Init(&Gimbal_Control_Struct.Yaw_Speed_PID_Struct,   Gimbal_Yaw_Speed_PID_Kp,    Gimbal_Yaw_Speed_PID_Ki,    Gimbal_Yaw_Speed_PID_Kd,    0, Gimbal_Yaw_Speed_PID_I_Output_Max,   Gimbal_Yaw_Speed_PID_Output_Max);
    PID_Init(&Gimbal_Control_Struct.Pitch_Angle_PID_Struct,   Gimbal_Pitch_Angle_PID_Kp,    Gimbal_Pitch_Angle_PID_Ki,    Gimbal_Pitch_Angle_PID_Kd,    0, Gimbal_Pitch_Angle_PID_I_Output_Max,   Gimbal_Pitch_Angle_PID_Output_Max);
    
    
    for(;;)
    {
        /*===| 得到云台控制参数 |===*/
        Gimbal_Control_Struct.Gimbal_State = RoboControl_Struct.Gimbal_State;
        Gimbal_Control_Struct.Yaw_Target   = RoboControl_Struct.Robo_Target_Yaw;
        Gimbal_Control_Struct.Pitch_Target = RoboControl_Struct.Robo_Target_Pitch;             
        
        /*===| 得到当前Pitch和Yaw数据 |===*/
//        Gimbal_Control_Struct.Pitch_Feedback  =  INS_Data.Roll; 
        Gimbal_Control_Struct.Pitch_Feedback  =  -Motor_Data_Struct[Gimbal_Pitch_ID].Total_Angle * 180.0f/PI;
        Gimbal_Control_Struct.Yaw_Feedback = INS_Data.YawTotalAngle;
        
        /*===| 得到Yaw与底盘正方向的偏移角度[-180 ~ +180度] |===*/
        Gimbal_Control_Struct.Yaw_Encoder_Err = Motor_Data_Struct[Gimbal_Yaw_ID].Encoder - Gimbal_Median_Yaw;
        if(Gimbal_Control_Struct.Yaw_Encoder_Err > 4096)                                                        Gimbal_Control_Struct.Yaw_Encoder_Err = 8192 - Gimbal_Control_Struct.Yaw_Encoder_Err;
        else if(Gimbal_Control_Struct.Yaw_Encoder_Err <= 4096 && Gimbal_Control_Struct.Yaw_Encoder_Err >= 0 )   Gimbal_Control_Struct.Yaw_Encoder_Err = -Gimbal_Control_Struct.Yaw_Encoder_Err;
        else if(Gimbal_Control_Struct.Yaw_Encoder_Err >= -4096 && Gimbal_Control_Struct.Yaw_Encoder_Err < 0 )   Gimbal_Control_Struct.Yaw_Encoder_Err = -Gimbal_Control_Struct.Yaw_Encoder_Err;
        else if(Gimbal_Control_Struct.Yaw_Encoder_Err < -4096)                                                  Gimbal_Control_Struct.Yaw_Encoder_Err = -8192 - Gimbal_Control_Struct.Yaw_Encoder_Err;
        
        Gimbal_Control_Struct.Yaw_Err =  (float)Gimbal_Control_Struct.Yaw_Encoder_Err * 360.0f / 8192.0f;
        RoboControl_Struct.Yaw_Err = Gimbal_Control_Struct.Yaw_Err; 
                
        /*===| 串级PID串级计算得到Yaw电机的电流大小 |===*/
        PID_Position_Calculate(&Gimbal_Control_Struct.Yaw_Angle_PID_Struct, Gimbal_Control_Struct.Yaw_Target,                  Gimbal_Control_Struct.Yaw_Feedback);
        PID_Position_Calculate(&Gimbal_Control_Struct.Yaw_Speed_PID_Struct, Gimbal_Control_Struct.Yaw_Angle_PID_Struct.Output, Motor_Data_Struct[Gimbal_Yaw_ID].SpeedRPM);
       
       /*===| PID计算得到Pitch电机的速度大小 |===*/
        PID_Position_Calculate(&Gimbal_Control_Struct.Pitch_Angle_PID_Struct, Gimbal_Control_Struct.Pitch_Target, Gimbal_Control_Struct.Pitch_Feedback);
        
        
        /*===| 发送电流参数 |===*/
        if(RoboControl_Struct.Robo_Enable) 
        {
            Motor_DJI_SendCurrent(&Gimbal_Yaw_CAN, Gimbal_Yaw_Send_CAN_ID, Gimbal_Control_Struct.Yaw_Speed_PID_Struct.Output, 0, 0, 0);
            Motor_DM_CMD_Position(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID, -Gimbal_Control_Struct.Pitch_Target * PI/180.0f, -Gimbal_Control_Struct.Pitch_Angle_PID_Struct.Output);
        }
        else 
        {
//            Motor_DM_CMD_SetZero(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID);
            Motor_DJI_SendCurrent(&Gimbal_Yaw_CAN, Gimbal_Yaw_Send_CAN_ID, 0, 0, 0, 0);
        }
        
        osDelay(1);
    }
}
