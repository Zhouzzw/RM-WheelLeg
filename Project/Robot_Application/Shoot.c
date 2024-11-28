//======================================================================
//                      Shoot[发射机构控制]
//      根据RoboControl中的Shoot_State驱动相应电机实现单发，连发等功能
//======================================================================

#include "Shoot.h"
#include "RoboControl.h"

/*===| 发送控制数据结构体 |===*/
Shoot_Control_StructTypeDef Shoot_Control_Struct;

void Shoot_Task(void *argument)
{
    /*===| 发射机构PID参数初始化 |===*/
    PID_Init(&Shoot_Control_Struct.Fric_Left_Speed_PID_Struct,      Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);
    PID_Init(&Shoot_Control_Struct.Fric_Right_Speed_PID_Struct,     Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);  
    PID_Init(&Shoot_Control_Struct.Fric_Middle_Speed_PID_Struct,    Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);    
    PID_Init(&Shoot_Control_Struct.Trigger_Angle_PID_Struct,        Shoot_Trigger_Angle_PID_Kp,  Shoot_Trigger_Angle_PID_Ki, Shoot_Trigger_Angle_PID_Kd, 0, Shoot_Trigger_Angle_PID_I_Output_Max,   Shoot_Trigger_Angle_PID_Output_Max);
    PID_Init(&Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct,        Shoot_Trigger_Single_Speed_PID_Kp,  Shoot_Trigger_Single_Speed_PID_Ki, Shoot_Trigger_Single_Speed_PID_Kd, 0, Shoot_Trigger_Single_Speed_PID_I_Output_Max,   Shoot_Trigger_Single_Speed_PID_Output_Max);
    PID_Init(&Shoot_Control_Struct.Trigger_Continue_Speed_PID_Struct,        Shoot_Trigger_Continue_Speed_PID_Kp,  Shoot_Trigger_Continue_Speed_PID_Ki, Shoot_Trigger_Continue_Speed_PID_Kd, 0, Shoot_Trigger_Continue_Speed_PID_I_Output_Max,   Shoot_Trigger_Continue_Speed_PID_Output_Max);
    
    for(;;)
    {     
        /*===| 得到发射状态 |===*/
        Shoot_Control_Struct.Shoot_State = RoboControl_Struct.Shoot_State;
        
        /*===| 根据发射状态控制摩擦轮和拨弹轮 |===*/
            /*===| 发射机构关闭 |===*/
            if      (Shoot_Control_Struct.Shoot_State == Shoot_State_Off)
            {
                /*===| 摩擦轮关闭，拨弹轮速度0 |===*/
                Shoot_Control_Struct.Fric_Left_Target_Speed = 0;
                Shoot_Control_Struct.Fric_Right_Target_Speed = 0;
                Shoot_Control_Struct.Fric_Middle_Target_Speed = 0;
                
                Shoot_Control_Struct.Trigger_Target_Speed = 0;
                
                Motor_Data_Struct[Shoot_Trigger_ID].Round %= (int)(Motor_3508_Reduction);
                Shoot_Control_Struct.Trigger_Target_Angle = Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Motor_3508_Reduction;
            }
            /*===| 发射机构开启，准备单发或者连发 |===*/
            else if (Shoot_Control_Struct.Shoot_State == Shoot_State_Ready)
            {
                /*===| 摩擦轮开启，拨弹轮角度环控制 |===*/
                Shoot_Control_Struct.Fric_Left_Target_Speed   = Shoot_Fric_Left_Speed  ;
                Shoot_Control_Struct.Fric_Right_Target_Speed  = Shoot_Fric_Right_Speed ;
                Shoot_Control_Struct.Fric_Middle_Target_Speed = Shoot_Fric_Middle_Speed;
                
                Shoot_Control_Struct.Trigger_Target_Speed = 0;
            }
            /*===| 连发模式 |===*/
            else if (Shoot_Control_Struct.Shoot_State == Shoot_State_Continue)
            {
                /*===| 摩擦轮开启，拨弹轮速度环控制 |===*/
                Shoot_Control_Struct.Fric_Left_Target_Speed    = Shoot_Fric_Left_Speed;
                Shoot_Control_Struct.Fric_Right_Target_Speed   = Shoot_Fric_Right_Speed;
                Shoot_Control_Struct.Fric_Middle_Target_Speed  = Shoot_Fric_Middle_Speed;
                
                Shoot_Control_Struct.Trigger_Target_Speed = Shoot_Trigger_Speed_Max;
                
                /*===| 重置拨弹轮角度环，防止单发位置错误 |===*/
                Motor_Data_Struct[Shoot_Trigger_ID].Round %= (int)(Motor_3508_Reduction);
                Shoot_Control_Struct.Trigger_Target_Angle = Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Motor_3508_Reduction;
            }
            
            /*===| 摩擦轮斜坡加减速，防止瞬时功率过大导致Shoot断电 |===*/
            if(Shoot_Control_Struct.Fric_Left_Set_Speed < Shoot_Control_Struct.Fric_Left_Target_Speed) Shoot_Control_Struct.Fric_Left_Set_Speed += 0.02f;
            else if(Shoot_Control_Struct.Fric_Left_Set_Speed > Shoot_Control_Struct.Fric_Left_Target_Speed) Shoot_Control_Struct.Fric_Left_Set_Speed -= 0.02f;
            if(Shoot_Control_Struct.Fric_Right_Set_Speed < Shoot_Control_Struct.Fric_Right_Target_Speed) Shoot_Control_Struct.Fric_Right_Set_Speed += 0.02f;
            else if(Shoot_Control_Struct.Fric_Right_Set_Speed > Shoot_Control_Struct.Fric_Right_Target_Speed) Shoot_Control_Struct.Fric_Right_Set_Speed -= 0.02f;
            if(Shoot_Control_Struct.Fric_Middle_Set_Speed < Shoot_Control_Struct.Fric_Middle_Target_Speed) Shoot_Control_Struct.Fric_Middle_Set_Speed += 0.02f;
            else if(Shoot_Control_Struct.Fric_Middle_Set_Speed > Shoot_Control_Struct.Fric_Middle_Target_Speed) Shoot_Control_Struct.Fric_Middle_Set_Speed -= 0.02f;
            
            Limit_float(&Shoot_Control_Struct.Fric_Left_Set_Speed, Shoot_Control_Struct.Fric_Left_Target_Speed, 0);
            Limit_float(&Shoot_Control_Struct.Fric_Right_Set_Speed, Shoot_Control_Struct.Fric_Right_Target_Speed, 0);
            Limit_float(&Shoot_Control_Struct.Fric_Middle_Set_Speed, Shoot_Control_Struct.Fric_Middle_Target_Speed, 0);
        
        /*===| PID计算 |===*/
            /*===| 摩擦轮 |===*/
            PID_Position_Calculate(&Shoot_Control_Struct.Fric_Left_Speed_PID_Struct,    Shoot_Control_Struct.Fric_Left_Set_Speed,  Motor_Data_Struct[Shoot_Fric_Left_ID].SpeedRPM * Fric_RPM_to_m_s);
            PID_Position_Calculate(&Shoot_Control_Struct.Fric_Right_Speed_PID_Struct,  -Shoot_Control_Struct.Fric_Right_Set_Speed,  Motor_Data_Struct[Shoot_Fric_Right_ID].SpeedRPM * Fric_RPM_to_m_s);
            PID_Position_Calculate(&Shoot_Control_Struct.Fric_Middle_Speed_PID_Struct,  Shoot_Control_Struct.Fric_Middle_Set_Speed,  Motor_Data_Struct[Shoot_Fric_Middle_ID].SpeedRPM * Fric_RPM_to_m_s);
        
            /*===| 拨弹轮 |===*/
                /*===| 发送机构关闭无PID输出 |===*/
                if(Shoot_Control_Struct.Shoot_State == Shoot_State_Off)
                {
                    Shoot_Control_Struct.Trigger_Send_Current = 0;
                }
                /*===| 单发模式角度环控制 |===*/
                else if(Shoot_Control_Struct.Shoot_State == Shoot_State_Ready)
                {
                    PID_Position_Calculate(&Shoot_Control_Struct.Trigger_Angle_PID_Struct,   Shoot_Control_Struct.Trigger_Target_Angle ,            Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Motor_3508_Reduction);
                    PID_Position_Calculate(&Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct,   Shoot_Control_Struct.Trigger_Angle_PID_Struct.Output, Motor_Data_Struct[Shoot_Trigger_ID].SpeedRPM / Motor_3508_Reduction);
                    Shoot_Control_Struct.Trigger_Send_Current = Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct.Output;
                }
                /*===| 连发模式速度环控制 |===*/
                else if(Shoot_Control_Struct.Shoot_State == Shoot_State_Continue)
                {
                    PID_Position_Calculate(&Shoot_Control_Struct.Trigger_Continue_Speed_PID_Struct,   Shoot_Control_Struct.Trigger_Target_Speed,  Motor_Data_Struct[Shoot_Trigger_ID].SpeedRPM / Motor_3508_Reduction);
                    Shoot_Control_Struct.Trigger_Send_Current = Shoot_Control_Struct.Trigger_Continue_Speed_PID_Struct.Output;
                }
                   
                /*===| 卡弹检测和处理 |===*/
                Stuck_Handle();                
        

        /*===| 发送电流参数 |===*/
            if(RoboControl_Struct.Robo_Enable) 
            {
                Motor_DJI_SendCurrent(&Shoot_Fric_CAN, Shoot_Fric_Send_CAN_ID,  
                                    Shoot_Control_Struct.Fric_Middle_Speed_PID_Struct.Output,
                                    Shoot_Control_Struct.Fric_Left_Speed_PID_Struct.Output,
                                    Shoot_Control_Struct.Fric_Right_Speed_PID_Struct.Output, 
                                    Shoot_Control_Struct.Trigger_Send_Current);
            }
            else
            {
                Motor_DJI_SendCurrent(&Shoot_Fric_CAN, Shoot_Fric_Send_CAN_ID,  0, 0, 0, 0);
            }
                                                                  
        osDelay(1);
    }
}


/**
 * @brief 单发[调用一次射击一发]
 */
void Shoot_Single(void)
{
    Shoot_Control_Struct.Trigger_Target_Angle += 360.0f / 5.0f;
}    

/**
 * @brief 退一发[调用一次退一发]
 */
void Shoot_Getout(void)
{
    Shoot_Control_Struct.Trigger_Target_Angle -= 360.0f / 5.0f;
}


/**
 * @brief 卡弹检测和处理
 */
void Stuck_Handle(void)
{
    /*===| 如果有电流无转速，则代表卡弹 |===*/
    if(fabs(Shoot_Control_Struct.Trigger_Send_Current) > 4000 && fabs((float)Motor_Data_Struct[Shoot_Trigger_ID].SpeedRPM) < 10) 
    {
        Shoot_Control_Struct.Trigger_Block_Time++; 
    }  
    else 
    {
        Shoot_Control_Struct.Trigger_Block_Time = 0;
    }

    /*===| 达到堵转时间阈值 |===*/
    if(Shoot_Control_Struct.Trigger_Block_Time >= 300) 
    {
        Shoot_Control_Struct.Trigger_Block_Flag = 1; 
        Shoot_Control_Struct.Trigger_Release_Time = 0;
        Shoot_Control_Struct.Trigger_Block_Time = 400;
    }           

    /*===| 卡弹处理 |===*/
    if(Shoot_Control_Struct.Trigger_Block_Flag)
    {   
        if(Shoot_Control_Struct.Trigger_Release_Time < 200)
        {
            /*===| 倒转100ms |===*/
            Shoot_Control_Struct.Trigger_Target_Speed = -0.2f * Shoot_Trigger_Speed_Max;
            PID_Position_Calculate(&Shoot_Control_Struct.Trigger_Continue_Speed_PID_Struct,   Shoot_Control_Struct.Trigger_Target_Speed,  Motor_Data_Struct[Shoot_Trigger_ID].SpeedRPM / Motor_3508_Reduction);
            Shoot_Control_Struct.Trigger_Send_Current = Shoot_Control_Struct.Trigger_Continue_Speed_PID_Struct.Output;
            
            Shoot_Control_Struct.Trigger_Release_Time++;
        }
        else
        {
            Shoot_Control_Struct.Trigger_Block_Flag = 0;
            Shoot_Control_Struct.Trigger_Block_Time = 0;
            Shoot_Control_Struct.Trigger_Release_Time = 0;
            Shoot_Getout();
        }
    }
}

