//======================================================================
//                      Shoot[发射机构控制]
//      根据RoboControl中的Shoot_State驱动相应电机实现单发，连发等功能
//======================================================================

#include "Shoot.h"
#include "RoboControl.h"
#include "Motor_DJI_Driver.h"
#include "Motor_DAMIAO_Driver.h"
#include "Motor_Unitree_Driver.h"
#include "Referee_Unpack.h"
#include "Communicate.h"
#include "Aim.h"

/*===| 发送控制数据结构体 |===*/

Shoot_Control_StructTypeDef Shoot_Control_Struct;

float Shoot_Fric_First_Left_Speed    = 20.0;
float Shoot_Fric_First_Right_Speed   = 20.0;
float Shoot_Fric_First_Middle_Speed  = 20.0;
float Shoot_Fric_Second_Left_Speed   = 20.0;
float Shoot_Fric_Second_Right_Speed  = 20.0;
float Shoot_Fric_Second_Middle_Speed = 20.0;

void Shoot_Task(void *argument)
{
    /*===| 发射机构PID参数初始化 |===*/
    PID_Init(&Shoot_Control_Struct.Fric_First_Left_Speed_PID_Struct,      Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);
    PID_Init(&Shoot_Control_Struct.Fric_First_Right_Speed_PID_Struct,     Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);  
    PID_Init(&Shoot_Control_Struct.Fric_First_Middle_Speed_PID_Struct,    Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);    
    PID_Init(&Shoot_Control_Struct.Fric_Second_Left_Speed_PID_Struct,      Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);
    PID_Init(&Shoot_Control_Struct.Fric_Second_Right_Speed_PID_Struct,     Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);  
    PID_Init(&Shoot_Control_Struct.Fric_Second_Middle_Speed_PID_Struct,    Shoot_Fric_Speed_PID_Kp,     Shoot_Fric_Speed_PID_Ki,    Shoot_Fric_Speed_PID_Kd,    0, Shoot_Fric_Speed_PID_I_Output_Max,      Shoot_Fric_Speed_PID_Output_Max);    
    PID_Init(&Shoot_Control_Struct.Trigger_Angle_PID_Struct,        Shoot_Trigger_Angle_PID_Kp,  Shoot_Trigger_Angle_PID_Ki, Shoot_Trigger_Angle_PID_Kd, 0, Shoot_Trigger_Angle_PID_I_Output_Max,   Shoot_Trigger_Angle_PID_Output_Max);
    PID_Init(&Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct,        Shoot_Trigger_Single_Speed_PID_Kp,  Shoot_Trigger_Single_Speed_PID_Ki, Shoot_Trigger_Single_Speed_PID_Kd, 0, Shoot_Trigger_Single_Speed_PID_I_Output_Max,   Shoot_Trigger_Single_Speed_PID_Output_Max);
   
	Shoot_Control_Struct.Shoot_Frequency = 10;
	
    for(;;)
    {     
		float Dt = DWT_GetDeltaT(&Shoot_Control_Struct.DWT_Count);
        Shoot_Control_Struct.Cool_Down -= Dt;
        Limit_float(&Shoot_Control_Struct.Cool_Down, 100, 0);
        /*===| 得到发射状态 |===*/
        Shoot_Control_Struct.Shoot_State = RoboControl_Struct.Shoot_State;
                           
        /*===| 摩擦轮堵弹检测与处理 |===*/
        //Fric_Stuck_Handle();    
        
        /*===| 根据发射状态控制摩擦轮和拨弹轮 |===*/
            /*===| 发射机构关闭 |===*/
            if      (Shoot_Control_Struct.Shoot_State == Shoot_State_Off)
            {
                /*===| 摩擦轮关闭，拨弹轮速度0 |===*/
                Shoot_Control_Struct.Fric_First_Left_Target_Speed   = 0;
                Shoot_Control_Struct.Fric_First_Right_Target_Speed  = 0;
                Shoot_Control_Struct.Fric_First_Middle_Target_Speed = 0;
                Shoot_Control_Struct.Fric_Second_Left_Target_Speed  = 0;
                Shoot_Control_Struct.Fric_Second_Right_Target_Speed = 0;
                Shoot_Control_Struct.Fric_Second_Middle_Target_Speed= 0;

                /*===| 重置拨弹轮角度环，防止单发位置错误 |===*/
                Motor_Data_Struct[Shoot_Trigger_ID].Round %= (int)(Triger_Reduction);
                Shoot_Control_Struct.Trigger_Target_Angle = Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Triger_Reduction;
            }
            /*===| 发射机构开启，准备单发或者连发 |===*/
            else if (Shoot_Control_Struct.Shoot_State == Shoot_State_Ready)
            {
                /*===| 摩擦轮开启，拨弹轮角度环控制 |===*/
//                Shoot_Control_Struct.Fric_First_Left_Target_Speed   = Shoot_Fric_First_Left_Speed  ;
//                Shoot_Control_Struct.Fric_First_Right_Target_Speed  = Shoot_Fric_First_Right_Speed ;
//                Shoot_Control_Struct.Fric_First_Middle_Target_Speed = Shoot_Fric_First_Middle_Speed;
//                Shoot_Control_Struct.Fric_Second_Left_Target_Speed  = Shoot_Fric_Second_Left_Speed  ;
//                Shoot_Control_Struct.Fric_Second_Right_Target_Speed = Shoot_Fric_Second_Right_Speed ;
//                Shoot_Control_Struct.Fric_Second_Middle_Target_Speed= Shoot_Fric_Second_Middle_Speed;
                
                
            }
            /*===| 发射机构开启，摩擦轮拨弹轮倒转 |===*/
            else if (Shoot_Control_Struct.Shoot_State == Shoot_State_Unload)
            {
                /*===| 摩擦轮开启，拨弹轮角度环控制 |===*/
//                Shoot_Control_Struct.Fric_First_Left_Target_Speed   = -Shoot_Fric_First_Left_Speed  ;
//                Shoot_Control_Struct.Fric_First_Right_Target_Speed  = -Shoot_Fric_First_Right_Speed ;
//                Shoot_Control_Struct.Fric_First_Middle_Target_Speed = -Shoot_Fric_First_Middle_Speed;
//                Shoot_Control_Struct.Fric_Second_Left_Target_Speed  = -Shoot_Fric_Second_Left_Speed  ;
//                Shoot_Control_Struct.Fric_Second_Right_Target_Speed = -Shoot_Fric_Second_Right_Speed ;
//                Shoot_Control_Struct.Fric_Second_Middle_Target_Speed= -Shoot_Fric_Second_Middle_Speed;
                
                
            }
            /*===| 连发模式 |===*/
            else if (Shoot_Control_Struct.Shoot_State == Shoot_State_Continue)
            {
                /*===| 摩擦轮开启，拨弹轮速度环控制 |===*/
//                Shoot_Control_Struct.Fric_First_Left_Target_Speed   = Shoot_Fric_First_Left_Speed  ;
//                Shoot_Control_Struct.Fric_First_Right_Target_Speed  = Shoot_Fric_First_Right_Speed ;
//                Shoot_Control_Struct.Fric_First_Middle_Target_Speed = Shoot_Fric_First_Middle_Speed;
//                Shoot_Control_Struct.Fric_Second_Left_Target_Speed  = Shoot_Fric_Second_Left_Speed  ;
//                Shoot_Control_Struct.Fric_Second_Right_Target_Speed = Shoot_Fric_Second_Right_Speed ;
//                Shoot_Control_Struct.Fric_Second_Middle_Target_Speed= Shoot_Fric_Second_Middle_Speed;
				 Shoot_Single(1);
            }
            
            /*===| 摩擦轮斜坡加减速，防止瞬时功率过大导致Shoot断电 |===*/
            if      (Shoot_Control_Struct.Fric_First_Left_Set_Speed     <= Shoot_Control_Struct.Fric_First_Left_Target_Speed)    Shoot_Control_Struct.Fric_First_Left_Set_Speed +=    Fric_Acc;
            else if (Shoot_Control_Struct.Fric_First_Left_Set_Speed     >= Shoot_Control_Struct.Fric_First_Left_Target_Speed)    Shoot_Control_Struct.Fric_First_Left_Set_Speed -=    Fric_Acc;
            if      (Shoot_Control_Struct.Fric_First_Right_Set_Speed    <= Shoot_Control_Struct.Fric_First_Right_Target_Speed)   Shoot_Control_Struct.Fric_First_Right_Set_Speed +=   Fric_Acc;
            else if (Shoot_Control_Struct.Fric_First_Right_Set_Speed    >= Shoot_Control_Struct.Fric_First_Right_Target_Speed)   Shoot_Control_Struct.Fric_First_Right_Set_Speed -=   Fric_Acc;
            if      (Shoot_Control_Struct.Fric_First_Middle_Set_Speed   <= Shoot_Control_Struct.Fric_First_Middle_Target_Speed)  Shoot_Control_Struct.Fric_First_Middle_Set_Speed +=  Fric_Acc;
            else if (Shoot_Control_Struct.Fric_First_Middle_Set_Speed   >= Shoot_Control_Struct.Fric_First_Middle_Target_Speed)  Shoot_Control_Struct.Fric_First_Middle_Set_Speed -=  Fric_Acc;
            if      (Shoot_Control_Struct.Fric_Second_Left_Set_Speed    <= Shoot_Control_Struct.Fric_Second_Left_Target_Speed)   Shoot_Control_Struct.Fric_Second_Left_Set_Speed +=   Fric_Acc;
            else if (Shoot_Control_Struct.Fric_Second_Left_Set_Speed    >= Shoot_Control_Struct.Fric_Second_Left_Target_Speed)   Shoot_Control_Struct.Fric_Second_Left_Set_Speed -=   Fric_Acc;
            if      (Shoot_Control_Struct.Fric_Second_Right_Set_Speed   <= Shoot_Control_Struct.Fric_Second_Right_Target_Speed)  Shoot_Control_Struct.Fric_Second_Right_Set_Speed +=  Fric_Acc;
            else if (Shoot_Control_Struct.Fric_Second_Right_Set_Speed   >= Shoot_Control_Struct.Fric_Second_Right_Target_Speed)  Shoot_Control_Struct.Fric_Second_Right_Set_Speed -=  Fric_Acc;
            if      (Shoot_Control_Struct.Fric_Second_Middle_Set_Speed  <= Shoot_Control_Struct.Fric_Second_Middle_Target_Speed) Shoot_Control_Struct.Fric_Second_Middle_Set_Speed += Fric_Acc;
            else if (Shoot_Control_Struct.Fric_Second_Middle_Set_Speed  >= Shoot_Control_Struct.Fric_Second_Middle_Target_Speed) Shoot_Control_Struct.Fric_Second_Middle_Set_Speed -= Fric_Acc;
            
            Limit_float(&Shoot_Control_Struct.Fric_First_Left_Set_Speed,    Shoot_Control_Struct.Fric_First_Left_Target_Speed,      -Shoot_Control_Struct.Fric_First_Left_Target_Speed   );
            Limit_float(&Shoot_Control_Struct.Fric_First_Right_Set_Speed,   Shoot_Control_Struct.Fric_First_Right_Target_Speed,     -Shoot_Control_Struct.Fric_First_Right_Target_Speed  );
            Limit_float(&Shoot_Control_Struct.Fric_First_Middle_Set_Speed,  Shoot_Control_Struct.Fric_First_Middle_Target_Speed,    -Shoot_Control_Struct.Fric_First_Middle_Target_Speed );
            Limit_float(&Shoot_Control_Struct.Fric_Second_Left_Set_Speed,   Shoot_Control_Struct.Fric_Second_Left_Target_Speed,     -Shoot_Control_Struct.Fric_Second_Left_Target_Speed  );
            Limit_float(&Shoot_Control_Struct.Fric_Second_Right_Set_Speed,  Shoot_Control_Struct.Fric_Second_Right_Target_Speed,    -Shoot_Control_Struct.Fric_Second_Right_Target_Speed );
            Limit_float(&Shoot_Control_Struct.Fric_Second_Middle_Set_Speed, Shoot_Control_Struct.Fric_Second_Middle_Target_Speed,   -Shoot_Control_Struct.Fric_Second_Middle_Target_Speed);
        
        /*===| PID计算 |===*/
            /*===| 摩擦轮 |===*/
//            PID_Position_Calculate(&Shoot_Control_Struct.Fric_First_Left_Speed_PID_Struct,    Shoot_Control_Struct.Fric_First_Left_Set_Speed,     Motor_Data_Struct[Shoot_Fric_First_Left_ID].Speed_RPM * Fric_RPM_to_m_s);
//            PID_Position_Calculate(&Shoot_Control_Struct.Fric_First_Right_Speed_PID_Struct,  -Shoot_Control_Struct.Fric_First_Right_Set_Speed,    Motor_Data_Struct[Shoot_Fric_First_Right_ID].Speed_RPM * Fric_RPM_to_m_s);

            /*===| 拨弹轮 |===*/
                /*===| 发送机构关闭无PID输出 |===*/
                if(Shoot_Control_Struct.Shoot_State == Shoot_State_Off)
                {
                    Shoot_Control_Struct.Trigger_Send_Current = 0;
                }
                /*===| 角度环控制 |===*/
                else 
                {
                    PID_Position_Calculate(&Shoot_Control_Struct.Trigger_Angle_PID_Struct,   Shoot_Control_Struct.Trigger_Target_Angle ,            Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Triger_Reduction);
                    PID_Position_Calculate(&Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct,   Shoot_Control_Struct.Trigger_Angle_PID_Struct.Output, Motor_Data_Struct[Shoot_Trigger_ID].Speed_RPM / Triger_Reduction);
                    Shoot_Control_Struct.Trigger_Send_Current = Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct.Output;
                }
                
                
            /*===| 拨弹轮堵弹检测与处理 |===*/
            Trigger_Stuck_Handle();
        
            /*===| 发送电流参数 |===*/
            if(RoboControl_Struct.Robo_Enable) 
            {
//                Motor_DJI_SendCurrent(&Shoot_Fric_First_CAN, Shoot_Fric_First_Send_CAN_ID,  
//                                    Shoot_Control_Struct.Fric_First_Left_Speed_PID_Struct.Output,
//                                    Shoot_Control_Struct.Fric_First_Right_Speed_PID_Struct.Output,
//                                    0, 
//                                    Shoot_Control_Struct.Trigger_Send_Current);
//				 Motor_DJI_SendCurrent(&Shoot_Trigger_CAN, 
//										Shoot_Trigger_Send_CAN_ID,
//										Shoot_Control_Struct.Trigger_Send_Current,  
//										0,
//										0, 
//										0);
            }
            else
            {
//                Motor_DJI_SendCurrent(&Shoot_Trigger_CAN, Shoot_Trigger_Send_CAN_ID,  0, 0, 0, 0);
                //Motor_DJI_SendCurrent(&Shoot_Fric_Second_CAN, Shoot_Fric_Second_Send_CAN_ID, 0, 0, 0, 0); 
            }
                                                                  
        osDelay(1);
    }
}


/**
 * @brief 单发[调用一次射击一发]
 */
void Shoot_Single(float Num)
{
    if(
        (fabs(Shoot_Control_Struct.Trigger_Target_Angle - Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Triger_Reduction) <= 40.0f)&& 
        Shoot_Control_Struct.Heat_Limit == 0 && 
        Shoot_Control_Struct.Cool_Down == 0
    ) 
    {
        Shoot_Control_Struct.Trigger_Target_Angle += Num * 360.0f / 9.0f;
        if(Shoot_Control_Struct.Shoot_Frequency <= 0) Shoot_Control_Struct.Shoot_Frequency = 1;
        Shoot_Control_Struct.Cool_Down += Num/Shoot_Control_Struct.Shoot_Frequency;
    }
} 

/**
 * @brief 退n发
 */
void Shoot_Getout(float n)
{
    Shoot_Control_Struct.Trigger_Target_Angle += 360.0f / 9.0f * n;
}

/**
 * @brief 热量限制
 */
void Heat_Limit(void)
{
    if(RoboControl_Struct.Shoot_State != Shoot_State_Off && Robo_PowerHeatData.shooter_17mm_barrel_heat >= (Robo_State.shooter_barrel_heat_limit - 40)) 
    {
        RoboControl_Struct.Shoot_State = Shoot_State_Ready;
        Shoot_Control_Struct.Heat_Limit = 1;
    }
    else
    {
        Shoot_Control_Struct.Heat_Limit = 0;
    }

} 
/**
 * @brief 卡弹检测和处理
 */
void Trigger_Stuck_Handle(void)
{
    /*===| 如果拨弹轮有电流无转速，则代表卡弹 |===*/
    if(fabs(Shoot_Control_Struct.Trigger_Send_Current) > 8000 && fabs((float)Motor_Data_Struct[Shoot_Trigger_ID].Speed_RPM) < 100)
    {
        if(Shoot_Control_Struct.Trigger_Block_Flag == 0)
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
        Shoot_Control_Struct.Trigger_Block_Time = 0;
    }           

    /*===| 卡弹处理 |===*/
    if(Shoot_Control_Struct.Trigger_Block_Flag)
    {   
        if(Shoot_Control_Struct.Trigger_Release_Time == 1)
        {
            /*===| 退0.3弹位 |===*/ 
            /*===| 重置拨弹轮角度环，防止单发位置错误 |===*/
            Motor_Data_Struct[Shoot_Trigger_ID].Round %= (int)(Triger_Reduction);
            Shoot_Control_Struct.Trigger_Target_Angle = Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Triger_Reduction;
            Shoot_Getout(0.3f);
        }
        
        if(Shoot_Control_Struct.Trigger_Release_Time < 500)
        {
            Shoot_Control_Struct.Trigger_Release_Time++;
        }
        else
        {   
            Shoot_Control_Struct.Trigger_Block_Flag = 0;
            Shoot_Control_Struct.Trigger_Release_Time = 0;
        }
    }
}

void Fric_Stuck_Handle(void)
{
    /*===| 如果摩擦轮有电流无转速，则代表卡弹 |===*/
    if((fabs(Shoot_Control_Struct.Fric_First_Left_Speed_PID_Struct.Output) > 10000 && fabs((float)Motor_Data_Struct[Shoot_Fric_First_Left_ID].Speed_RPM) < 100) 
        && (fabs(Shoot_Control_Struct.Fric_First_Right_Speed_PID_Struct.Output) > 10000 && fabs((float)Motor_Data_Struct[Shoot_Fric_First_Right_ID].Speed_RPM) < 100) )
    {
        if(Shoot_Control_Struct.Fric_Block_Flag == 0)
        Shoot_Control_Struct.Fric_Block_Time++; 
    }  
    else 
    {
        Shoot_Control_Struct.Fric_Block_Time = 0;
    }

    /*===| 达到堵转时间阈值 |===*/
    if(Shoot_Control_Struct.Fric_Block_Time >= 500) 
    {
        Shoot_Control_Struct.Fric_Block_Flag = 1; 
        Shoot_Control_Struct.Fric_Release_Time = 0;
        Shoot_Control_Struct.Fric_Block_Time = 0;
    }           

    /*===| 卡弹处理 |===*/
    if(Shoot_Control_Struct.Fric_Block_Flag)
    {   
        if(Shoot_Control_Struct.Fric_Release_Time == 1)
        {
            /*===| 退0.3弹位 |===*/ 
            /*===| 重置拨弹轮角度环，防止单发位置错误 |===*/
            Motor_Data_Struct[Shoot_Trigger_ID].Round %= (int)(Triger_Reduction);
            Shoot_Control_Struct.Trigger_Target_Angle = Motor_Data_Struct[Shoot_Trigger_ID].Total_Angle / Triger_Reduction;
            Shoot_Getout(0.3f);
        }
        
        if(Shoot_Control_Struct.Fric_Release_Time < 1000)
        {
            /*===| 摩擦轮倒转1000ms |===*/
            RoboControl_Struct.Shoot_State = Shoot_State_Unload;
            
            Shoot_Control_Struct.Fric_Release_Time++;
        }
        else
        {   
            Shoot_Control_Struct.Fric_Block_Flag = 0;
            Shoot_Control_Struct.Fric_Block_Time = 0;
            Shoot_Control_Struct.Fric_Release_Time = 0;
            RoboControl_Struct.Shoot_State = Shoot_State_Ready;
        }
    }
}


