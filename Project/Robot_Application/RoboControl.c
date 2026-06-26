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
#include "Motor_DJI_Driver.h"
#include "Motor_DAMIAO_Driver.h"
#include "Motor_Unitree_Driver.h"
#include "Aim.h"
#include "Function.h"
#include "SuperCap_Driver.h"

/*===| 机器人整体状态数据结构体 |===*/
RoboControl_StructTypeDef RoboControl_Struct;
PcControl_StructTypeDef PC_Control_Struct;

/*===| 计算Wz的PID结构体 |===*/
PID_Struct_TypeDef RoboWz_PID_Struct;

uint16_t Board_Commnuicate_Error_Ticker;
uint8_t IF_Chassis_Online;

uint8_t count=0;

extern float Steer_Total_angle_Target;
extern float Steer_Total_angle_Feedback;

float Wz_PID_output_max = 5.0f;
float K_limit_wz_max = 0.0f;

void Robo_Task(void *argument)
{
	/*===| Yaw参数初始化 |===*/
    RoboControl_Struct.Robo_Target_Gimbal_Yaw = 0;
    
    /*===| Pitch参数初始化 |===*/
    RoboControl_Struct.Robo_Target_Gimbal_Pitch = 0;
    
    /*===| Wz_PID初始化 |===*/
	RoboControl_Struct.Target_Direction = 0.0f;
	PID_Init(&RoboWz_PID_Struct,RoboWz_PID_Kp,RoboWz_PID_Ki,RoboWz_PID_Kd,0.0f,RoboWz_PID_I_Output_MAX,RoboWz_PID_Output_MAX);
	
    /*===| 默认模块状态 |===*/
    RoboControl_Struct.Chassis_State = Chassis_OFF;
    RoboControl_Struct.Shoot_State = Shoot_State_Off;
    RoboControl_Struct.Gimbal_State = Gimbal_State_Normal;
    RoboControl_Struct.Chassis_Speed_Level = 1;
    RoboControl_Struct.SuperCap_State = 1;//平衡步兵默认上电开超电
    Referee_Data_Init();
    for(;;)
    {
        /*===| 如果遥控器连接 |===*/
        if(Remote.If_Remote_Connect)
        {
            /*===| 选择控制模式：摇杆|键鼠|自定义控制器 |===*/
            if     (Remote.Mode == Remote_Mode_C) RoboControl_Struct.Controler = Joystick;
            else if(Remote.Mode == Remote_Mode_N && RoboControl_Struct.Controler != Customer) RoboControl_Struct.Controler = Joystick;
            else if(Remote.Mode == Remote_Mode_S && RoboControl_Struct.Controler != Customer) RoboControl_Struct.Controler = KeyboardMouse;   
            
            /*===| 判断是否停机 |===*/     
            if(Remote.Mode == Remote_Mode_C && RoboControl_Struct.Robo_Enable == 1) Robo_Stop();   
            else if(Remote.Mode != Remote_Mode_C && RoboControl_Struct.Robo_Enable == 0 && RoboControl_Struct.Controler == Joystick) Robo_Restart(); 
        }
        else
        {
            Robo_Stop(); 
        }
        
        /*===| 遥控方式 |===*/
        /*===| 遥控器摇杆控制 |===*/
        if(RoboControl_Struct.Controler == Joystick)
        {
            RemoteControl_Float();
            if(Remote.If_Remote_Data_New == 1)
            {
//                if(Remote_ReleaseSingle_Pause) RoboControl_Struct.Controler = Customer;
                RemoteControl_Bool();
            }
        }
        /*===| 遥控器键鼠控制 |===*/
        else if(RoboControl_Struct.Controler == KeyboardMouse)
        {
            KeyControl_Float();
            if(Remote.If_Remote_Data_New == 1)
            {
//                if(Remote_ReleaseSingle_B) RoboControl_Struct.Controler = Customer; 
                KeyControl_Bool();
            }
        }
       
        Remote.If_Remote_Data_New = 0;      
        
        /*===| 控制参数限幅 |===*/              
		Limit_float(&RoboControl_Struct.Robo_Target_Gimbal_Pitch, 27, -20);
		Limit_float(&RoboControl_Struct.Leg_Length,0.38f,0.15f);
		Limit_float(&RoboControl_Struct.Leg_Angle ,70.0f,-70.0f);
				
        /*===| 根据底盘运动状态得到Wz |===*/  
		Get_Chassis_Wz();		
        
        /*===| 控制参数低通滤波 |===*/         
        Control_Filter();
        
        /*===| 平滑启停 |===*/    
        if(RoboControl_Struct.Robo_Enable)
            RoboControl_Struct.Smooth_Start_K += 0.00033333f;
        else 
            RoboControl_Struct.Smooth_Start_K -= 0.001f;
        Limit_float(&RoboControl_Struct.Smooth_Start_K, 1, 0);
				
		/*===| 遥控器断连处理 |===*/
		Board_Commnuicate_Error_Ticker++;
		if(Board_Commnuicate_Error_Ticker >= 500)
        {
            Board_Commnuicate_Error_Ticker = 500;
			IF_Chassis_Online = 0;
        }	

        if(Remote.If_Remote_Connect == 0 && IF_Chassis_Online == 0)
        {
            Robo_Stop();
            memset(&Remote_Last.Mouse_Vx, 0, 30);
        }
		
		//电管底盘失能 机器人也失能 防止重使能关节乱动
		if(Robo_State.power_management_chassis_output == 0)
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
		PID_Init(&RoboWz_PID_Struct,RoboWz_PID_Kp,RoboWz_PID_Ki,RoboWz_PID_Kd,0.0f,RoboWz_PID_I_Output_MAX,RoboWz_PID_Output_MAX);
        RoboControl_Struct.Robo_Target_Wz = 0;
    }
    /*===| 底盘静止 WASD控制底盘转向朝向 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_STATIC)
    {
		K_limit_wz_max = (5.0f - 2.8f) / (0.36f - 0.25f);
		
		if(Chassis_Control_Struct.Ave_Leg_Length > 0.25f) 
		{
			Wz_PID_output_max = 5.0f - K_limit_wz_max * (Chassis_Control_Struct.Ave_Leg_Length - 0.25f);// 快速线性递减
		} 
		else
		{
			Wz_PID_output_max = 5.0f;
		} 
		PID_Init(&RoboWz_PID_Struct,RoboWz_PID_Kp,RoboWz_PID_Ki,RoboWz_PID_Kd,0.0f,RoboWz_PID_I_Output_MAX,Wz_PID_output_max);
		
		PID_Position_Calculate(&RoboWz_PID_Struct, -RoboControl_Struct.Target_Direction, RoboControl_Struct.Target_Direction_Err);
        RoboControl_Struct.Robo_Target_Wz = RoboWz_PID_Struct.Output;
    }
    /*===| 底盘跟随模式 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_FOLLOW)
    {
		K_limit_wz_max = (5.0f - 2.8f) / (0.36f - 0.25f);
		
		if(Chassis_Control_Struct.Ave_Leg_Length > 0.25f) 
		{
			Wz_PID_output_max = 5.0f - K_limit_wz_max * (Chassis_Control_Struct.Ave_Leg_Length - 0.25f);// 快速线性递减
		} 
		else
		{
			Wz_PID_output_max = 5.0f;
		} 
		PID_Init(&RoboWz_PID_Struct,RoboWz_PID_Kp,RoboWz_PID_Ki,RoboWz_PID_Kd,0.0f,RoboWz_PID_I_Output_MAX,Wz_PID_output_max);
		
        PID_Position_Calculate(&RoboWz_PID_Struct, RoboControl_Struct.Target_Direction, RoboControl_Struct.Target_Direction_Err);
        RoboControl_Struct.Robo_Target_Wz = RoboWz_PID_Struct.Output;
		INS_Data_Self.Yaw_Round = 0;
		Steer_Total_angle_Target = 0;
    }
    /*===| 底盘小陀螺 |===*/
    else if(RoboControl_Struct.Chassis_State == Chassis_SPIN)
    {
		PID_Init(&RoboWz_PID_Struct,RoboWz_PID_Kp,RoboWz_PID_Ki,RoboWz_PID_Kd,0.0f,RoboWz_PID_I_Output_MAX,RoboWz_PID_Output_MAX);
        /*===| 小陀螺得到Wz,每次切换正反转 |===*/
        if(RoboControl_Struct.SPIN_Direction_Flag)
        {
            RoboControl_Struct.Robo_Target_Wz = Robo_Wz_MaxSpeed;
        }                                                         
        else 
        {
            RoboControl_Struct.Robo_Target_Wz = -Robo_Wz_MaxSpeed;
        }
        /*===| 根据档位限制旋转速度，遥控器控制状态不限制 |===*/
			switch(RoboControl_Struct.Chassis_Speed_Level)
			{
				case 1:
					RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz*(6.0f/12.0f);
					break; 
				case 2:
					RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz*(7.50f/12.0f);
					break;
				case 3:
					RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz*(8.50f/12.0f);
					break;
				case 4:
					RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz*(10.00f/12.0f);
					break; 
				case 5:
					RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz;
					break;
			}
    }
}


/**
 * @brief 输入参数低通滤波
 */
void Control_Filter(void)
{
    RoboControl_Struct.Robo_Target_Vx    = 0.4f * RoboControl_Struct.Robo_Target_Vx    + (1 - 0.4f)  * RoboControl_Struct.Robo_Target_Vx_Last   ;
    RoboControl_Struct.Robo_Target_Vy    = 0.4f * RoboControl_Struct.Robo_Target_Vy    + (1 - 0.4f)  * RoboControl_Struct.Robo_Target_Vy_Last   ;
    RoboControl_Struct.Robo_Target_Wz    = 0.9f * RoboControl_Struct.Robo_Target_Wz    + (1 - 0.9f)  * RoboControl_Struct.Robo_Target_Wz_Last   ;
    RoboControl_Struct.Robo_Target_Gimbal_Yaw   = 0.8f * RoboControl_Struct.Robo_Target_Gimbal_Yaw   + (1 - 0.8f)  * RoboControl_Struct.Robo_Target_Gimbal_Yaw_Last  ;
    RoboControl_Struct.Robo_Target_Gimbal_Pitch = 0.8f * RoboControl_Struct.Robo_Target_Gimbal_Pitch + (1 - 0.8f)  * RoboControl_Struct.Robo_Target_Gimbal_Pitch_Last;

    RoboControl_Struct.Robo_Target_Vx_Last    =  RoboControl_Struct.Robo_Target_Vx   ;
    RoboControl_Struct.Robo_Target_Vy_Last    =  RoboControl_Struct.Robo_Target_Vy   ;              
    RoboControl_Struct.Robo_Target_Wz_Last    =  RoboControl_Struct.Robo_Target_Wz   ;              
    RoboControl_Struct.Robo_Target_Gimbal_Yaw_Last   =  RoboControl_Struct.Robo_Target_Gimbal_Yaw  ;              
    RoboControl_Struct.Robo_Target_Gimbal_Pitch_Last =  RoboControl_Struct.Robo_Target_Gimbal_Pitch; 
}

/**
 * @brief 遥控器控制
 */
void RemoteControl_Float(void)
{
   /*===| 底盘和云台运动控制 |===*/
   static uint32_t Remote_DWT_Count;
   float Dt = DWT_GetDeltaT(&Remote_DWT_Count);
    
   if(Remote_Mode_N)
   {
		/*===| 底盘运动控制 |===*/
		switch(RoboControl_Struct.Chassis_Speed_Level)
		{
			case 1:
				RoboControl_Struct.Robo_Target_Vy = 1.4f * (Remote.Left_Y);
				break; 
			case 2:
				RoboControl_Struct.Robo_Target_Vy = 1.7f * (Remote.Left_Y);
				break;
			case 3:
				RoboControl_Struct.Robo_Target_Vy = 2.0f  * (Remote.Left_Y);
				break;                              
			case 4:                                 
				RoboControl_Struct.Robo_Target_Vy = 2.15f * (Remote.Left_Y);
				break;                              
			case 5:                                 
				RoboControl_Struct.Robo_Target_Vy = 2.30f * (Remote.Left_Y);
				break;
		}
  
	    RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz;
	   
	   /*===| 底盘静止状态下 以云台为正方向 左摇杆直接控制底盘全向移动 先转向再前进 与云台控制解耦 |===*/
	   if(RoboControl_Struct.Chassis_State == Chassis_STATIC || RoboControl_Struct.Chassis_State == Chassis_SPIN)
	   {
		   switch(RoboControl_Struct.Chassis_Speed_Level)
			{
				case 1:
					RoboControl_Struct.Robo_Target_Vx = 1.4f * (Remote.Left_X);
					break;                              
				case 2:                                 
					RoboControl_Struct.Robo_Target_Vx = 1.7f * (Remote.Left_X);
					break;                              
				case 3:                                 
					RoboControl_Struct.Robo_Target_Vx = 2.0f * (Remote.Left_X);
					break;                              
				case 4:                                 
					RoboControl_Struct.Robo_Target_Vx = 2.15f * (Remote.Left_X);
					break;                              
				case 5:                                 
					RoboControl_Struct.Robo_Target_Vx = 2.30f * (Remote.Left_X);
					break;
			}  
			
			Limit_float(&RoboControl_Struct.Robo_Target_Vx, 1.8f, -1.8f); 
	   }
	}
}

void RemoteControl_Bool(void)
{
	static uint32_t Remote_DWT_Count;
    float Dt = DWT_GetDeltaT(&Remote_DWT_Count);
	
		/*===| 滑键滑至C模式关闭所有设备 |===*/     
        if(Remote.Mode == Remote_Mode_C && RoboControl_Struct.Robo_Enable == 1) Robo_Stop();
        
        /*===| 重启设备 |===*/     
        else if(Remote.Mode == Remote_Mode_N && RoboControl_Struct.Robo_Enable == 0) Robo_Restart();
		
		if((RoboControl_Struct.Robo_Enable == 1) && Remote.Wheel > 0.7f)
		{
			RoboControl_Struct.Leg_Length += 0.16f * Remote.Right_Y * Dt;//控制腿长
			RoboControl_Struct.Leg_Angle  += 10.0f * Remote.Right_X * Dt;//板凳状态下控制摆角
			 
			/*===| 自定义功能-左摇杆向右 |===*/
            if(Remote.Left_X > 0.8f && Remote_Last.Left_X < 0.8f)
            {
				/*===| 底盘跟随模式 |===*/
                RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
            }
            /*===| 自定义功能-左摇杆向左 |===*/
            else if (Remote.Left_X < -0.8f && Remote_Last.Left_X > -0.8f)
            {
				/*===| 底盘静止模式 |===*/
                RoboControl_Struct.Chassis_State = Chassis_STATIC;
            }
            /*===| 自定义功能-左摇杆向上 |===*/
            if      (Remote.Left_Y > 0.8f && Remote_Last.Left_Y < 0.8f)
            {

            }
            /*===| 自定义功能-左摇杆向下 |===*/
            else if (Remote.Left_Y < -0.8f && Remote_Last.Left_Y > -0.8f)
            {

            }
            /*===| 自定义功能- 右摇杆向右 |===*/
            if      (Remote.Right_X > 0.8f && Remote_Last.Right_X < 0.8f)
            {
				/*===| 速度等级+1 |===*/
                RoboControl_Struct.Chassis_Speed_Level += 1;
                if(RoboControl_Struct.Chassis_Speed_Level > 5) RoboControl_Struct.Chassis_Speed_Level = 5;
				Chassis_Control_Struct.Jump_Level += 1;
				if(Chassis_Control_Struct.Jump_Level > 3) Chassis_Control_Struct.Jump_Level = 3;
            }
            /*===| 自定义功能- 右摇杆向左 |===*/
            else if (Remote.Right_X < -0.8f && Remote_Last.Right_X > -0.8f)
            {
				/*===| 速度等级-1 |===*/
                RoboControl_Struct.Chassis_Speed_Level -= 1;
                if(RoboControl_Struct.Chassis_Speed_Level < 1) RoboControl_Struct.Chassis_Speed_Level = 1;
				Chassis_Control_Struct.Jump_Level -= 1;
				if(Chassis_Control_Struct.Jump_Level < 1) Chassis_Control_Struct.Jump_Level = 1;
            }
            /*===| 自定义功能-右摇杆向上 |===*/
            if      (Remote.Right_Y > 0.8f && Remote_Last.Right_Y < 0.8f)
            {
                /*===| 开超电 |===*/
                RoboControl_Struct.SuperCap_State = 1;
                Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_ON);
            }
            /*===| 自定义功能-右摇杆向下 |===*/
            else if (Remote.Right_Y < -0.8f && Remote_Last.Right_Y > -0.8f)
            {
                /*===| 关超电 |===*/
                RoboControl_Struct.SuperCap_State = 0;
                Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_OFF);
            }
		}
		 
		else if(Remote.Wheel < -0.7f)
		{
			 /*===| 自定义功能-左摇杆向右 |===*/
            if(Remote.Left_X > 0.8f && Remote_Last.Left_X < 0.8f)
            {

            }
            /*===| 自定义功能-左摇杆向左 |===*/
            else if (Remote.Left_X < -0.8f && Remote_Last.Left_X > -0.8f)
            {
				//手动清零部分功能标志位 避免疯车
				Chassis_Control_Struct.Recover_from_ground_Flag = 0;
				Chassis_Control_Struct.Jump_Flag = 0;
				Chassis_Control_Struct.UpStep_Dect_Flag = 0;
				Chassis_Control_Struct.UpStep_Flag = 0;
            }
            /*===| 自定义功能-左摇杆向上 |===*/
            if      (Remote.Left_Y > 0.8f && Remote_Last.Left_Y < 0.8f)
            {
//				RoboControl_Struct.If_SettingZero = 1;//设零点
            }
            /*===| 自定义功能-左摇杆向下 |===*/
            else if (Remote.Left_Y < -0.8f && Remote_Last.Left_Y > -0.8f)
            {
				Chassis_Control_Struct.Recover_from_ground_Flag = 1;//翻倒自起功能
            }
            /*===| 自定义功能-滑轮下滑 右摇杆向右 |===*/
            if      (Remote.Right_X > 0.8f && Remote_Last.Right_X < 0.8f)
            {
				Chassis_Control_Struct.Jump_Flag = 1;//机器人跳跃功能
            }
            /*===| 自定义功能-滑轮下滑 右摇杆向左 |===*/
            else if (Remote.Right_X < -0.8f && Remote_Last.Right_X > -0.8f)
            {
				Chassis_Control_Struct.UpStep_Dect_Flag = 1;//机器人上台阶磕碰检测标志位
				Buzzer_Set_SoundEffect(Buzzer_SoundEffect_Aim_ON);
            }
            /*===| 自定义功能-右摇杆向上 |===*/
            if      (Remote.Right_Y > 0.8f && Remote_Last.Right_Y < 0.8f)
            {
				
            }
            /*===| 自定义功能-右摇杆向下 |===*/
            else if (Remote.Right_Y < -0.8f && Remote_Last.Right_Y > -0.8f)
            {
                Chassis_Control_Struct.Up_Slope_Mode_Flag = 0;
            }
		}
		 
        /*===| 左按键开启小陀螺 |===*/
        if(Remote_PressSingle_Custom_L) 
        {			
            if(RoboControl_Struct.Chassis_State != Chassis_SPIN)
            {
                RoboControl_Struct.Chassis_State = Chassis_SPIN;
            }
            
            /*===| 再次按左键 关闭小陀螺 并让下次小陀螺时反转 |===*/
            else if(RoboControl_Struct.Chassis_State == Chassis_SPIN)
            {
                RoboControl_Struct.Chassis_State = Chassis_STATIC;        //底盘跟随
				if(fabsf(RoboControl_Struct.Yaw_Err)<90.0f)
				{
					Chassis_Control_Struct.Chassis_Forward_Change_Flag = 0;
				}
				else
				{
					Chassis_Control_Struct.Chassis_Forward_Change_Flag = 1;
				}
                RoboControl_Struct.SPIN_Direction_Flag = !RoboControl_Struct.SPIN_Direction_Flag;
            }     
			
			INS_Data_Self.Yaw_Round = 0;
			Steer_Total_angle_Target = 0;
        }
				
        /*===| 右键全功能按键*/
        if (RoboControl_Struct.Robo_Enable == 1 && Remote_PressSingle_Custom_R)
        {
//            Chassis_Control_Struct.UpStep_Flag = 1;
			Chassis_Control_Struct.Jump_Flag = 1;//机器人跳跃功能
        }    
		else if(RoboControl_Struct.Robo_Enable == 0 && Remote_PressSingle_Custom_R)
		{
			Chassis_Control_Struct.Smooth_restand_Flag = 1;
		}
} 


/**
 * @brief 键盘控制
 */
void KeyControl_Float(void)
{
    static uint32_t Key_DWT_Count;
    float Dt = DWT_GetDeltaT(&Key_DWT_Count);
    
    /*===| 键盘控制Pitch错误值过滤 |===*/
    if(fabs(Remote.Mouse_Vy - Remote_Last.Mouse_Vy) > 3.6f)
    {
        Remote.Mouse_Vy = 0;
    }  
	
	if(Remote.Mouse_Vz>0.5f)//鼠标滚轮控制跳跃等级
	{
		Chassis_Control_Struct.Jump_Level = 1;
	}
	else if(Remote.Mouse_Vz<-0.5f)
	{
		Chassis_Control_Struct.Jump_Level = 2;
	}

	switch(RoboControl_Struct.Chassis_Speed_Level)
		{
			case 1:
				RoboControl_Struct.Robo_Target_Vy = 1.4f * (Remote.Keyboard_W - Remote.Keyboard_S);
				break;                              
			case 2:                                 
				RoboControl_Struct.Robo_Target_Vy = 1.7f * (Remote.Keyboard_W - Remote.Keyboard_S);
				break;                              
			case 3:                                 
				RoboControl_Struct.Robo_Target_Vy = 2.0f * (Remote.Keyboard_W - Remote.Keyboard_S);
				break;                              
			case 4:                                 
				RoboControl_Struct.Robo_Target_Vy = 2.05f * (Remote.Keyboard_W - Remote.Keyboard_S);
				break;                              
			case 5:                                 
				RoboControl_Struct.Robo_Target_Vy = 2.10f * (Remote.Keyboard_W - Remote.Keyboard_S);
				break;
		}
	
	RoboControl_Struct.Robo_Target_Wz = RoboControl_Struct.Robo_Target_Wz;
		
	/*===| 底盘静止状态下 以云台为正方向 左摇杆直接控制底盘全向移动 先转向再前进 与云台控制解耦 |===*/
    if(RoboControl_Struct.Chassis_State == Chassis_STATIC || RoboControl_Struct.Chassis_State == Chassis_SPIN)
    {
	    switch(RoboControl_Struct.Chassis_Speed_Level)
		{
			case 1:
				RoboControl_Struct.Robo_Target_Vx = 1.4f * (Remote.Keyboard_D - Remote.Keyboard_A);
				break;                              
			case 2:                                 
				RoboControl_Struct.Robo_Target_Vx = 1.7f * (Remote.Keyboard_D - Remote.Keyboard_A);
				break;                              
			case 3:                                 
				RoboControl_Struct.Robo_Target_Vx = 2.0f * (Remote.Keyboard_D - Remote.Keyboard_A);
				break;                              
			case 4:                                 
				RoboControl_Struct.Robo_Target_Vx = 2.05f * (Remote.Keyboard_D - Remote.Keyboard_A);
				break;                              
			case 5:                                 
				RoboControl_Struct.Robo_Target_Vx = 2.10f * (Remote.Keyboard_D - Remote.Keyboard_A);
				break;
		}
		
		Limit_float(&RoboControl_Struct.Robo_Target_Vx, 1.8f, -1.8f); 
    }
}

void KeyControl_Bool(void) 
{	
	static uint32_t Key_DWT_Count;
	float Dt = DWT_GetDeltaT(&Key_DWT_Count);
	
    /*===| 按住C进入上台阶检测模式 |===*/
		if(Remote_Press_C)
		{
			Chassis_Control_Struct.UpStep_Dect_Flag = 1;//机器人上台阶磕碰检测标志位
			Buzzer_Set_SoundEffect(Buzzer_SoundEffect_Aim_ON);
		}
		else if(Remote_ReleaseSingle_C)
		{
			Chassis_Control_Struct.UpStep_Dect_Flag = 0;
		}
    
		/*===| 按Q降腿长 按E升腿长 |===*/
		if(!Remote_Press_Ctrl && Remote_PressSingle_E)
		{
			Chassis_Control_Struct.Leg_length_Level ++ ;
			if(Chassis_Control_Struct.Leg_length_Level > 4)
			{
				Chassis_Control_Struct.Leg_length_Level = 4;
			}
			switch(Chassis_Control_Struct.Leg_length_Level)
			{
				case 1:
					RoboControl_Struct.Leg_Length = 0.18f;
					break; 
				case 2:
					RoboControl_Struct.Leg_Length = 0.22f; 
					break;
				case 3:
					RoboControl_Struct.Leg_Length = 0.28f;
					break;
				case 4:
					RoboControl_Struct.Leg_Length = 0.35f;
					break;
			}
		}
		if(!Remote_Press_Ctrl && Remote_PressSingle_Q)
		{
			Chassis_Control_Struct.Leg_length_Level -- ;
			if(Chassis_Control_Struct.Leg_length_Level < 1)
			{
				Chassis_Control_Struct.Leg_length_Level = 1;
			}
			switch(Chassis_Control_Struct.Leg_length_Level)
			{
				case 1:
					RoboControl_Struct.Leg_Length = 0.18f;
					break; 
				case 2:
					RoboControl_Struct.Leg_Length = 0.22f; 
					break;
				case 3:
					RoboControl_Struct.Leg_Length = 0.28f;
					break;
				case 4:
					RoboControl_Struct.Leg_Length = 0.35f;
					break;
			}
		}
			
		/*===| Ctrl组合键 |===*/
		if(Remote_Press_Ctrl) 
		{
			if(Remote_PressSingle_R)
			{
				Chassis_Control_Struct.Recover_from_ground_Flag = 1;//翻倒自起功能
			}
			if(Remote_PressSingle_E)
			{
				Chassis_Control_Struct.Smooth_restand_Flag = 1;//缓起立自救功能
			}
			if(Remote_PressSingle_Q)
			{
				Chassis_Control_Struct.UpStep_Flag = 1;
			}				
			if(Remote_PressSingle_Z)
			{
				//手动清零部分功能标志位 避免疯车
				Chassis_Control_Struct.Recover_from_ground_Flag = 0;
				Chassis_Control_Struct.Recover_State    = 0;
				Chassis_Control_Struct.Jump_Flag        = 0;
				Chassis_Control_Struct.Jump_State       = 0;
				Chassis_Control_Struct.UpStep_Dect_Flag = 0;
				Chassis_Control_Struct.UpStep_Flag   	= 0;
				Chassis_Control_Struct.UpStep_State  	= 0;
			}
		}  
		
		/*===| 按鼠标中键跳跃 |===*/
		if(Remote_PressSingle_Mouse_M)
		{
			Chassis_Control_Struct.Jump_Flag = 1;//机器人跳跃功能
		}
		
		/*===| 按住Shift开启小陀螺 松手即停 每次松手后自动改变下次小陀螺的方向 |===*/
		if(Remote_Press_Shift) 
		{
			if(RoboControl_Struct.Chassis_State != Chassis_SPIN)
            {
                RoboControl_Struct.Chassis_State = Chassis_SPIN;
            }  
		}
		else if(Remote_ReleaseSingle_Shift) 
		{
			RoboControl_Struct.SPIN_Direction_Flag = !RoboControl_Struct.SPIN_Direction_Flag;
			RoboControl_Struct.Chassis_State = Chassis_STATIC;
			if(fabsf(RoboControl_Struct.Yaw_Err)<90.0f)
			{
				Chassis_Control_Struct.Chassis_Forward_Change_Flag = 0;
			}
			else
			{
				Chassis_Control_Struct.Chassis_Forward_Change_Flag = 1;
			}
		}
		
		/*===| 按V开关超电 |===*/
		if(Remote_PressSingle_V)
		{
			RoboControl_Struct.SuperCap_State = !RoboControl_Struct.SuperCap_State;
			if(RoboControl_Struct.SuperCap_State) Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_ON);
			else Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SuperCap_OFF);
		}   
		
		/*===| 按F切换速度档位 |===*/
		if(Remote_PressSingle_F)
		{
			RoboControl_Struct.Chassis_Speed_Level++;
			if(RoboControl_Struct.Chassis_Speed_Level == 6) RoboControl_Struct.Chassis_Speed_Level = 1;
		}
		
		/*===| 长按B重刷UI |===*/
		if(Remote_Press_B)
		{
			RoboControl_Struct.Refresh_UI_Flag = 1;
		}
		else
		{
			RoboControl_Struct.Refresh_UI_Flag = 0;
		}
    
		/*===| 按Z切换底盘静止状态 |===*/
		if(!Remote_Press_Ctrl && Remote_PressSingle_Z)
		{
			if(RoboControl_Struct.Chassis_State != Chassis_STATIC) RoboControl_Struct.Chassis_State = Chassis_STATIC;
			else RoboControl_Struct.Chassis_State = Chassis_FOLLOW;
		} 
		
		/*===| 按X失能 |===*/
		if(Remote_PressSingle_X) 
		{
			Robo_Stop();
		}
		
		/*===| 按G重启 |===*/
		if(Remote_PressSingle_G || Chassis_Control_Struct.Smooth_restand_Flag == 1)
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

	RoboControl_Struct.Chassis_State  = Chassis_OFF;
	RoboControl_Struct.Shoot_State    = Shoot_State_Off;
    RoboControl_Struct.Robo_Target_Vx = 0;
    RoboControl_Struct.Robo_Target_Vy = 0;
    RoboControl_Struct.Robo_Target_Wz = 0;
    RoboControl_Struct.SuperCap_State = 1;//默认超电一直开启
    
	//清零功能标志位 避免疯车
	Chassis_Control_Struct.Recover_from_ground_Flag = 0;
	Chassis_Control_Struct.Recover_State    = 0;
	Chassis_Control_Struct.Jump_Flag        = 0;
	Chassis_Control_Struct.Jump_State       = 0;
	Chassis_Control_Struct.UpStep_Dect_Flag = 0;
	Chassis_Control_Struct.UpStep_Flag   	= 0;
	Chassis_Control_Struct.UpStep_State  	= 0;
	
	Chassis_Control_Struct.Leg_length_Level = 1;
	Chassis_Control_Struct.Jump_Level = 1;
	
//	Chassis_Control_Struct.Smooth_restand_Flag = 0;
	Chassis_Control_Struct.Allow_to_Stand_Flag = 0;
	
    vTaskSuspendAll();
	
    xTaskResumeAll(); 
}


/**
 * @brief 重启机器人
 */
void Robo_Restart(void)
{
    vTaskSuspendAll();

	RoboControl_Struct.Robo_Enable = 1;
	if(Chassis_Control_Struct.Smooth_restand_Flag == 1)
	{
		Chassis_Control_Struct.Recover_from_ground_Flag = 0;
	}
	else
	{
		Chassis_Control_Struct.Recover_from_ground_Flag = 1;
	}
	
	/*===| 默认模块状态 |===*/
	RoboControl_Struct.Chassis_State  = Chassis_STATIC;
    RoboControl_Struct.SuperCap_State = 1;//默认超电一直开启
	
	RoboControl_Struct.Leg_Length = 0.15f;
	Chassis_Control_Struct.Leg_length_Level = 1;
	Chassis_Control_Struct.Jump_Level = 1;
	if(fabsf(RoboControl_Struct.Yaw_Err)<90.0f)
	{
		Chassis_Control_Struct.Chassis_Forward_Change_Flag = 0;
	}
	else
	{
		Chassis_Control_Struct.Chassis_Forward_Change_Flag = 1;
	}

	INS_Data_Self.Yaw_Round  = 0;
	Steer_Total_angle_Target = 0;
		
    Chassis_Control_Struct.Target_displacement = Chassis_Control_Struct.displacement;
    Chassis_Control_Struct.add_displacement    = 0.0f;
    Chassis_Control_Struct.now_displacement    = Chassis_Control_Struct.displacement;
    Chassis_Control_Struct.Chassis_Vy_Target_last = 0.0f; 
	
//	Chassis_Control_Struct.Smooth_restand_Flag = 1;//使能缓起立功能执行标志位

    xTaskResumeAll();
}

