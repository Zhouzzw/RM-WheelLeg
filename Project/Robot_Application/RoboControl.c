    //====================================================================================
//                           RoboControl[机器状态控制]
//      根据操控意愿把遥控器数据处理成机器人
//      整体[RoboEnable,Vx,Vy,Wz,Yaw,Pitch等],各个模块[Gimbal,Shoot,Chassis等]的状态,
//      各个模块再根据状态执行相应的功能
//      其中包括状态设置，冲突处理，错误检测与处理等    
//
//      机器人状态结构体：RoboControl_Struct
//====================================================================================

#include "RoboControl.h"
#include "INS.h"
#include "Buzzer.h"
#include "Remote_Control.h"

/*===| 机器人整体状态数据结构体 |===*/
RoboControl_StructTypeDef RoboControl_Struct;

/*===| 计算Wz的PID结构体 |===*/
PID_Struct_TypeDef RoboWz_PID_Struct;

void Robo_Task(void *argument)
{
    /*===| Yaw参数初始化 |===*/
    RoboControl_Struct.Robo_Target_Yaw = 0;
    
    /*===| Pitch参数初始化 |===*/
    RoboControl_Struct.Robo_Target_Pitch = 0;
    
    /*===| Wz_PID初始化 |===*/
    PID_Init(&RoboWz_PID_Struct, 0.05, 0, 0.01, 0, 0, Robo_Wz_MaxSpeed);
    
    /*===| 默认模块状态 |===*/
    RoboControl_Struct.Chassis_State = Chassis_STATIC;
    RoboControl_Struct.Shoot_State = Shoot_State_Off;
    RoboControl_Struct.Gimbal_State = Gimbal_State_Gyro;
    RoboControl_Struct.Chassis_Speed_Level = 3;
    RoboControl_Struct.SuperCap_State = 1;
    
    for(;;)
    {
        /*===| 收到一次遥控器数据才处理一次，同步接收和处理 |===*/
        if(Remote_Control_Struct.If_Remote_Connect == 1 && Remote_Control_Struct.If_Remote_Data_New == 1)
        {        
            /*===| 右拨杆拨到最下面开启键盘控制，否则是遥控器控制 |===*/
            if(Remote_Control_Struct.S2 != SW_Down)
            {
                RemoteControl();
            }
            else if(Remote_Control_Struct.S2 == SW_Down && Remote_Control_Last_Struct.S2 != SW_Down)
            {
                RoboControl_Struct.Gimbal_State = Gimbal_State_Gyro;
                RoboControl_Struct.Chassis_State = Chassis_STATIC;
                RoboControl_Struct.Shoot_State = Shoot_State_Off;
            }
            else if(Remote_Control_Struct.S2 != SW_Down && Remote_Control_Last_Struct.S2 == SW_Down)
            {
                RoboControl_Struct.Gimbal_State = Gimbal_State_Gyro;
                RoboControl_Struct.Chassis_State = Chassis_STATIC;
                RoboControl_Struct.Shoot_State = Shoot_State_Off;
            }
            else if(Remote_Control_Struct.S2 == SW_Down && Remote_Control_Last_Struct.S2 == SW_Down)
            {
                /*===| 左拨杆拨中选择使用遥控器的键鼠数据，拨到上选择图传链路的键鼠数据 |===*/  
                if(Remote_Control_Struct.S1 == SW_Up) 
                {
                    Remote_Control_Struct.IF_USE_ImageLink = 1;  
                    /*===| 图传链路键盘控制 |===*/  
                }
                else if(Remote_Control_Struct.S1 == SW_Mid) 
                {
                    Remote_Control_Struct.IF_USE_ImageLink = 0;
                    /*===| 遥控器链路键盘控制 |===*/  
                    KeyControl();
                }
            }
            
            /*===| Pitch控制参数限幅 |===*/        
            Limit_float(&RoboControl_Struct.Robo_Target_Pitch, Gimbal_Pitch_Limit_Up, Gimbal_Pitch_Limit_Down);
            
            /*===| 控制参数低通滤波 |===*/         
            Control_Filter();
            
            /*===| 根据底盘运动状态得到Wz |===*/   
            Get_Chassis_Wz();
            
            /*===| 左拨杆拨到最下面关机 |===*/     
            if(Remote_Control_Struct.S1 == SW_Down) Robo_Stop();
            
            /*===| 急停状态结束重启设备 |===*/     
            else if(Remote_Control_Struct.S1 != SW_Down && Remote_Control_Last_Struct.S1 == SW_Down) Robo_Restart();     
        }
        
        /*===| 如果没有遥控器数据，但有图传链路数据，则自动切换到图传链路 |===*/  
        if(((Remote_Control_Struct.If_Remote_Connect == 0 && Remote_Control_Struct.If_ImageLink_Connect == 1) || Remote_Control_Struct.IF_USE_ImageLink == 1) && Remote_Control_Struct.If_ImageLink_Data_New == 1)
        {
            Remote_Control_Struct.IF_USE_ImageLink = 1;
        
            /*===| 键盘控制 |===*/  
            KeyControl();
            
            /*===| Pitch控制参数限幅 |===*/        
            Limit_float(&RoboControl_Struct.Robo_Target_Pitch, Gimbal_Pitch_Limit_Up, Gimbal_Pitch_Limit_Down);
                                                    
            /*===| 控制参数低通滤波 |===*/         
            Control_Filter();
                                                    
            /*===| 根据底盘运动状态得到Wz |===*/   
            Get_Chassis_Wz();
        }
                                
        /*===| 此次遥控器数据已被使用 |===*/
        Remote_Control_Struct.If_Remote_Data_New = 0;
        /*===| 此次图传链路数据已被使用 |===*/
        Remote_Control_Struct.If_ImageLink_Data_New = 0;

        
        /*===| 遥控器断连处理 |===*/
        Remote_Control_Struct.Remote_GetData_Ticker++;
        Remote_Control_Struct.ImageLink_GetData_Ticker++;
        
        if(Remote_Control_Struct.Remote_GetData_Ticker >= 500)
        {
            Remote_Control_Struct.If_Remote_Connect = 0;
            Remote_Control_Struct.Remote_GetData_Ticker = 200;
        }
        if(Remote_Control_Struct.ImageLink_GetData_Ticker >= 200)
        {
            Remote_Control_Struct.If_ImageLink_Connect = 0;
            Remote_Control_Struct.ImageLink_GetData_Ticker = 500;
            
            memset(&Remote_Control_Last_Struct.Mouse_Speed_X, 0, 30);
        }
        
        if(Remote_Control_Struct.If_ImageLink_Connect == 0 && Remote_Control_Struct.If_Remote_Connect == 0)
        {
            Robo_Stop();
        }


        osDelay(1);
    }
}


/**
 * @brief 根据底盘运动状态得到Wz
 */
void Get_Chassis_Wz(void)
{
    /*===| 底盘关闭 |===*/
    if(RoboControl_Struct.Chassis_State == Chassis_OFF)
    {
        RoboControl_Struct.Robo_Target_Wz = 0;
    }
    /*===| 底盘静止 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_STATIC)
    {
        RoboControl_Struct.Robo_Target_Wz = 0;
    }
    /*===| 底盘跟随模式 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_FOLLOW)
    {
        PID_Position_Calculate(&RoboWz_PID_Struct, 0, RoboControl_Struct.Yaw_Err);
        RoboControl_Struct.Robo_Target_Wz = -RoboWz_PID_Struct.Output;
    }
    /*===| 底盘侧向跟随模式 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_F_SIDE)
    {
        PID_Position_Calculate(&RoboWz_PID_Struct, 2.0f * (RoboControl_Struct.Chassis_Follow_45_Direction_Flag - 0.5f) * 45, RoboControl_Struct.Yaw_Err);
        RoboControl_Struct.Robo_Target_Wz = -RoboWz_PID_Struct.Output;
    }
    /*===| 底盘小陀螺 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_SPIN)
    {
        /*===| 小陀螺得到Wz,每次切换正反转 |===*/
        if(RoboControl_Struct.SPIN_Direction_Flag)
        {
            RoboControl_Struct.Robo_Target_Wz = Robo_Wz_MaxSpeed;
        }                                                         
        else 
        {
            RoboControl_Struct.Robo_Target_Wz = -Robo_Wz_MaxSpeed;
        }
            
        Motor_Data_Struct[Gimbal_Yaw_ID].Round = 0;
    }
    /*===| 冲刺模式 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_DASH)
    {
        /*===| 如果底盘没有回正，则等待底盘回到正方向 |===*/
        if(RoboControl_Struct.Yaw_Err > 5.0f || RoboControl_Struct.Yaw_Err < -5.0f) 
        {
            RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
            PID_Position_Calculate(&RoboWz_PID_Struct, 0, RoboControl_Struct.Yaw_Err);
            RoboControl_Struct.Robo_Target_Wz = -RoboWz_PID_Struct.Output;
        }
        else
        {
            RoboControl_Struct.Chassis_State = Chassis_DASH;
            RoboControl_Struct.Robo_Target_Wz = 0;
        }
    }  
    
    /*===| 根据档位限制旋转速度，遥控器控制状态不限制 |===*/
    if(Remote_Control_Struct.S2 == SW_Down)
    {
        RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz * (1.0f + RoboControl_Struct.Chassis_Speed_Level) / 6.0f;
    }
    else
    {
        RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz * (1.0f + 4) / 6.0f;
    }
}


/**
 * @brief 输入参数低通滤波
 */
void Control_Filter(void)
{
    RoboControl_Struct.Robo_Target_Vx    = 0.1f * RoboControl_Struct.Robo_Target_Vx    + (1 - 0.1f)  * RoboControl_Struct.Robo_Target_Vx_Last   ;
    RoboControl_Struct.Robo_Target_Vy    = 0.1f * RoboControl_Struct.Robo_Target_Vy    + (1 - 0.1f)  * RoboControl_Struct.Robo_Target_Vy_Last   ;
    RoboControl_Struct.Robo_Target_Wz    = 0.8f * RoboControl_Struct.Robo_Target_Wz    + (1 - 0.8f)  * RoboControl_Struct.Robo_Target_Wz_Last   ;
    RoboControl_Struct.Robo_Target_Yaw   = 0.6f * RoboControl_Struct.Robo_Target_Yaw   + (1 - 0.6f)  * RoboControl_Struct.Robo_Target_Yaw_Last  ;
    RoboControl_Struct.Robo_Target_Pitch = 0.7f * RoboControl_Struct.Robo_Target_Pitch + (1 - 0.7f)  * RoboControl_Struct.Robo_Target_Pitch_Last;

    RoboControl_Struct.Robo_Target_Vx_Last    =  RoboControl_Struct.Robo_Target_Vx   ;
    RoboControl_Struct.Robo_Target_Vy_Last    =  RoboControl_Struct.Robo_Target_Vy   ;              
    RoboControl_Struct.Robo_Target_Wz_Last    =  RoboControl_Struct.Robo_Target_Wz   ;              
    RoboControl_Struct.Robo_Target_Yaw_Last   =  RoboControl_Struct.Robo_Target_Yaw  ;              
    RoboControl_Struct.Robo_Target_Pitch_Last =  RoboControl_Struct.Robo_Target_Pitch; 
}

/**
 * @brief 遥控器控制
 */
void RemoteControl(void)
{
    /*===| 底盘和云台运动控制 |===*/
        static uint32_t Remote_DWT_Count;
        float Dt = DWT_GetDeltaT(&Remote_DWT_Count);
    
        if(Remote_Control_Struct.RC_Side < 0.8f)
        {
            RoboControl_Struct.Robo_Target_Vx = Robo_Vx_MaxSpeed * RoboControl_Struct.Chassis_Speed_Level / 5.0f * Remote_Control_Struct.RC_Left_X;
            RoboControl_Struct.Robo_Target_Vy = Robo_Vy_MaxSpeed * RoboControl_Struct.Chassis_Speed_Level / 5.0f * Remote_Control_Struct.RC_Left_Y;
            RoboControl_Struct.Robo_Target_Yaw    -= Dt * 300.0f * Remote_Control_Struct.RC_Right_X;
            RoboControl_Struct.Robo_Target_Pitch  += Dt * 80.0f  * Remote_Control_Struct.RC_Right_Y;  
        }
    /*===| 侧滑轮往上进入自定义功能选项，取消摇杆控制移动，通过摇杆选择触发一次定义的八个功能 |===*/ 
        if(Remote_Control_Struct.RC_Side > 0.8f)
        {
            /*===| 自定义功能-左摇杆向右 |===*/
            if      (Remote_Control_Struct.RC_Left_X > 0.8f && Remote_Control_Last_Struct.RC_Left_X < 0.8f)
            {
                /*===| 底盘跟随模式 |===*/
                RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
            }
            /*===| 自定义功能-左摇杆向左 |===*/
            else if (Remote_Control_Struct.RC_Left_X < -0.8f && Remote_Control_Last_Struct.RC_Left_X > -0.8f)
            {
                /*===| 底盘静止模式 |===*/
                RoboControl_Struct.Chassis_State = Chassis_STATIC;
            }
            /*===| 自定义功能-左摇杆向上 |===*/
            if      (Remote_Control_Struct.RC_Left_Y > 0.8f && Remote_Control_Last_Struct.RC_Left_Y < 0.8f)
            {
                /*===| 改后驱冲刺 |===*/
                RoboControl_Struct.Chassis_State = Chassis_DASH;
            }
            /*===| 自定义功能-左摇杆向下 |===*/
            else if (Remote_Control_Struct.RC_Left_Y < -0.8f && Remote_Control_Last_Struct.RC_Left_Y > -0.8f)
            {

            }
            /*===| 自定义功能-右摇杆向右 |===*/
            if      (Remote_Control_Struct.RC_Right_X > 0.8f && Remote_Control_Last_Struct.RC_Right_X < 0.8f)
            {
                /*===| 底盘斜对敌-右 |===*/
                RoboControl_Struct.Chassis_State = Chassis_F_SIDE;
                RoboControl_Struct.Chassis_Follow_45_Direction_Flag = 1;
            }
            /*===| 自定义功能-右摇杆向左 |===*/
            else if (Remote_Control_Struct.RC_Right_X < -0.8f && Remote_Control_Last_Struct.RC_Right_X > -0.8f)
            {
                /*===| 底盘斜对敌-左 |===*/
                RoboControl_Struct.Chassis_State = Chassis_F_SIDE;
                RoboControl_Struct.Chassis_Follow_45_Direction_Flag = 0;
            }
            /*===| 自定义功能-右摇杆向上 |===*/
            if      (Remote_Control_Struct.RC_Right_Y > 0.8f && Remote_Control_Last_Struct.RC_Right_Y < 0.8f)
            {
                /*===| 开超电 |===*/
                RoboControl_Struct.SuperCap_State = 1;
                Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_ON);
            }
            /*===| 自定义功能-右摇杆向下 |===*/
            else if (Remote_Control_Struct.RC_Right_Y < -0.8f && Remote_Control_Last_Struct.RC_Right_Y > -0.8f)
            {
                /*===| 关超电 |===*/
                RoboControl_Struct.SuperCap_State = 0;
                Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_OFF);
            }
        }

        /*===| 左拨杆拨到中间开启小陀螺 |===*/
        if(Remote_Control_Struct.S1 == SW_Up) 
        {
            /*===| 每次开启小陀螺反向 |===*/
            if (Remote_Control_Last_Struct.S1 != SW_Up)
            RoboControl_Struct.SPIN_Direction_Flag = !RoboControl_Struct.SPIN_Direction_Flag;
            
            RoboControl_Struct.Chassis_State = Chassis_SPIN;
        }
        else
        {
            if(RoboControl_Struct.Chassis_State == Chassis_SPIN)
            {
                RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
            }
        }

        /*===| 发射状态控制 |===*/
        /*===| 右摇杆控制发射状态 上[关闭摩擦轮] 中[打开摩擦轮]|===*/
        if      (Remote_Control_Struct.S2 == SW_Mid) RoboControl_Struct.Shoot_State = Shoot_State_Off;
        else if (Remote_Control_Struct.S2 == SW_Up) RoboControl_Struct.Shoot_State = Shoot_State_Ready;

        if(RoboControl_Struct.Shoot_State != Shoot_State_Off)
        { 
            /*===| 侧滑轮往下拨一次单发 |===*/
            if( Remote_Control_Struct.RC_Side < -0.2f && Remote_Control_Last_Struct.RC_Side > -0.2f)
            {
                Shoot_Single();
            }
            /*===| 侧滑轮往下拨到底连发 |===*/
            else if(Remote_Control_Struct.RC_Side < -0.90f)
            {
                RoboControl_Struct.Shoot_State = Shoot_State_Continue;     
            }    
            /*===| 侧滑轮松开则回到准备发射状态 |===*/
            else if(Remote_Control_Struct.RC_Side == 0)
            {
                RoboControl_Struct.Shoot_State = Shoot_State_Ready;
            }
        }
}


/**
 * @brief 键盘控制
 */
void KeyControl(void)
{
    static uint32_t Key_DWT_Count;
    float Dt = DWT_GetDeltaT(&Key_DWT_Count);
    
    /*===| 键盘控制Pitch错误值过滤 |===*/
    if(fabs(Remote_Control_Struct.Mouse_Speed_Y - Remote_Control_Last_Struct.Mouse_Speed_Y) > 3.6f)
    {
        Remote_Control_Struct.Mouse_Speed_Y = 0;
    }  

    /*===| 底盘和云台运动控制 |===*/
    RoboControl_Struct.Robo_Target_Vx = Robo_Vx_MaxSpeed * RoboControl_Struct.Chassis_Speed_Level / 5.0f * (Remote_Control_Struct.Keyboard_D - Remote_Control_Struct.Keyboard_A);
    RoboControl_Struct.Robo_Target_Vy = Robo_Vy_MaxSpeed * RoboControl_Struct.Chassis_Speed_Level / 5.0f * (Remote_Control_Struct.Keyboard_W - Remote_Control_Struct.Keyboard_S);
    if(RoboControl_Struct.Gimbal_State == Gimbal_State_Aimming)
    {
        RoboControl_Struct.Robo_Target_Yaw    -= Dt * 30.0f * Remote_Control_Struct.Mouse_Speed_X;
        RoboControl_Struct.Robo_Target_Pitch  -= Dt * 30.0f * Remote_Control_Struct.Mouse_Speed_Y;
    }
    else
    {
        RoboControl_Struct.Robo_Target_Yaw    -= Dt * 200.0f * Remote_Control_Struct.Mouse_Speed_X;
        RoboControl_Struct.Robo_Target_Pitch  -= Dt * 160.0f * Remote_Control_Struct.Mouse_Speed_Y;
    }    
    
    /*===| 按住Ctrl进入瞄准模式 |===*/
    if(KeyCtrl_Single_Press)
    {
        RoboControl_Struct.Gimbal_State = Gimbal_State_Aimming;
        RoboControl_Struct.Chassis_State = Chassis_STATIC;
    }
    else if(KeyCtrl_Single_Release)
    {
        RoboControl_Struct.Gimbal_State = Gimbal_State_Gyro;
        RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
    }
    
    /*===| 按住Shift改后驱冲刺 |===*/
    if(KeyShift_Press) 
    {
        RoboControl_Struct.Chassis_State = Chassis_DASH;
    }  
    else if(KeyShift_Single_Release) 
    {
        RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
    }
    /*===| 按X改底盘跟随模式 |===*/
    if(KeyX_Single_Press) 
    {
        if(RoboControl_Struct.Chassis_State != Chassis_STATIC) RoboControl_Struct.Chassis_State = Chassis_STATIC;
        else RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
    }
    /*===| 按住右键斜对敌 |===*/
    if(MouseR_Single_Press)
    {
        RoboControl_Struct.Chassis_State = Chassis_F_SIDE;
        RoboControl_Struct.Chassis_Follow_45_Direction_Flag = !RoboControl_Struct.Chassis_Follow_45_Direction_Flag;
    }
    if(MouseR_Single_Release)
    {
        RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
    }
     /*===| 按C开启小陀螺 |===*/
    if(KeyC_Single_Press) 
    {
        if(RoboControl_Struct.Chassis_State != Chassis_SPIN)
        {
            /*===| 每次开启小陀螺反向 |===*/
            RoboControl_Struct.SPIN_Direction_Flag = !RoboControl_Struct.SPIN_Direction_Flag;
            RoboControl_Struct.Chassis_State = Chassis_SPIN;
        }
        else
        {
            RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
        } 
    }

    /*===| 按Q/E向左/右转90度 |===*/
    if(KeyQ_Single_Press) RoboControl_Struct.Robo_Target_Yaw += 150;
    if(KeyE_Single_Press) RoboControl_Struct.Robo_Target_Yaw -= 150;
    
    /*===| 按R开关摩擦轮 |===*/
    if(KeyR_Single_Press)
    {
        RoboControl_Struct.Shoot_State = (Shoot_State_EnumTypedef)!(uint8_t)RoboControl_Struct.Shoot_State;
    }

    if(RoboControl_Struct.Shoot_State != Shoot_State_Off)
    {
        /*===| 左键单发 |===*/
        if(MouseL_Single_Press)
        {
            Shoot_Single();
        }
    }
    
    /*===| 按V开关超电 |===*/
    if(KeyV_Single_Press)
    {
        RoboControl_Struct.SuperCap_State = !RoboControl_Struct.SuperCap_State;
        if(RoboControl_Struct.SuperCap_State) Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_ON);
        else Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_OFF);
    }   
     
    /*===| 按F切换速度档位 |===*/
    if(KeyF_Single_Press)
    {
        RoboControl_Struct.Chassis_Speed_Level++;
        if(RoboControl_Struct.Chassis_Speed_Level == 6) RoboControl_Struct.Chassis_Speed_Level = 1;
    } 
    
    /*===| 按B重刷UI |===*/
    if(KeyB_Press)
    {
        RoboControl_Struct.Refresh_UI_Flag = 1;
    }
    else
    {
        RoboControl_Struct.Refresh_UI_Flag = 0;
    }
    
    /*===| 按G重启 |===*/
    if(KeyG_Single_Press)
    {
        Robo_Restart();
    } 
}


/**
 * @brief 关闭机器人
 */
void Robo_Stop(void)
{
    RoboControl_Struct.Robo_Enable = 0;

    RoboControl_Struct.Robo_Target_Vx = 0;
    RoboControl_Struct.Robo_Target_Vy = 0;
    RoboControl_Struct.Robo_Target_Wz = 0;
    RoboControl_Struct.Shoot_State = Shoot_State_Off;
    
    RoboControl_Struct.Chassis_State = Chassis_OFF;
    
    vTaskSuspendAll();

    HAL_Delay(5);
    Motor_DM_CMD_Disable(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID);
    HAL_Delay(5);

    xTaskResumeAll(); 
}


/**
 * @brief 重启机器人
 */
void Robo_Restart(void)
{
    vTaskSuspendAll();
                        
    HAL_Delay(5);
    Motor_DM_CMD_ClearErr(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID);
    HAL_Delay(5);
    Motor_DM_CMD_Enable(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID);
    HAL_Delay(5);

    /*===| 重启后Pitch回到0，Yaw更新到当前位置 |===*/
    RoboControl_Struct.Robo_Target_Yaw = INS_Data.YawTotalAngle;
    RoboControl_Struct.Robo_Target_Pitch = 0;
    
    /*===| 默认模块状态 |===*/
    RoboControl_Struct.Chassis_State = Chassis_STATIC;
    RoboControl_Struct.Shoot_State = Shoot_State_Off;
    RoboControl_Struct.Gimbal_State = Gimbal_State_Gyro;
    
    RoboControl_Struct.Robo_Enable = 1;

    xTaskResumeAll();
}

