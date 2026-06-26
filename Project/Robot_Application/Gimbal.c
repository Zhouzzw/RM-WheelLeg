//======================================================================
//                      Gimbal[云台控制]
//      驱动相应电机使云台陀螺仪达到RoboControl的Pitch，Yaw目标角度
//======================================================================

#include "Gimbal.h"
#include "RoboControl.h"
#include "INS.h"
#include "Motor_DJI_Driver.h"
#include "Motor_DAMIAO_Driver.h"
#include "Motor_Unitree_Driver.h"
#include "Chassis.h"
#include "Aim.h"
#include "Remote_Control.h"

/*===| 云台控制数据结构体 |===*/
Gimbal_Control_StructTypeDef Gimbal_Control_Struct;


//CAN离线看门狗
extern uint16_t CAN1_Err_Ticker;
extern uint16_t CAN2_Err_Ticker;
extern uint16_t CAN3_Err_Ticker;

extern float Fn_r;
extern float Fn_l;

extern float Z_w_dd_L;
extern float Z_w_dd_R; 

extern float V_L,V_R;
extern float V_ave;

extern float LQR_K_L[12];
float K1,K2,K3,K4,K5,K6;

extern float Filter_Left_Leg_Angle_dot;
extern float Filter_Right_Leg_Angle_dot;
extern float Filter_Pitch_Speed_fdb;
extern float Filter_Volecity_Fdb;
extern float Filter_MotionAccel_n_Z;
extern float K_slope;

extern INS_t INS;

extern float spin_time;
extern float Spin_T;
extern float Spin_move_Direction;
extern float Move_time;
extern float Move_Flag;
extern float Chassis_Yaw;

extern float Soft_landing_Jump_F;

extern float Left_Wheel_RPM;
extern float Right_Wheel_RPM;

void Gimbal_Task(void *argument)
{
    /*===| 云台PID参数初始化 |===*/
//    PID_Init(&Gimbal_Control_Struct.Yaw_Angle_PID_Struct,   Gimbal_Yaw_Angle_PID_Kp,    Gimbal_Yaw_Angle_PID_Ki,    Gimbal_Yaw_Angle_PID_Kd,    0, Gimbal_Yaw_Angle_PID_I_Output_Max,   Gimbal_Yaw_Angle_PID_Output_Max);
//    PID_Init(&Gimbal_Control_Struct.Yaw_Speed_PID_Struct,   Gimbal_Yaw_Speed_PID_Kp,    Gimbal_Yaw_Speed_PID_Ki,    Gimbal_Yaw_Speed_PID_Kd,    0, Gimbal_Yaw_Speed_PID_I_Output_Max,   Gimbal_Yaw_Speed_PID_Output_Max);
//    PID_Init(&Gimbal_Control_Struct.Pitch_Angle_PID_Struct,   Gimbal_Pitch_Angle_PID_Kp,    Gimbal_Pitch_Angle_PID_Ki,    Gimbal_Pitch_Angle_PID_Kd,    0, Gimbal_Pitch_Angle_PID_I_Output_Max,   Gimbal_Pitch_Angle_PID_Output_Max);
//    PID_Init(&Gimbal_Control_Struct.Pitch_Speed_PID_Struct,   Gimbal_Pitch_Speed_PID_Kp,    Gimbal_Pitch_Speed_PID_Ki,    Gimbal_Pitch_Speed_PID_Kd,    0, Gimbal_Pitch_Speed_PID_I_Output_Max,   Gimbal_Pitch_Speed_PID_Output_Max);

    for(;;)
    {
        /*===| 得到当前Pitch和Yaw数据 |===*/
//        Gimbal_Control_Struct.Pitch_Feedback  =  -Motor_Data_Struct[Gimbal_Pitch_ID].Angle;
//        Gimbal_Control_Struct.Pitch_Feedback  =  INS_Data_Gimbal.Pitch;
//        Gimbal_Control_Struct.Yaw_Feedback    = INS_Data_Gimbal.YawTotalAngle;
//        
//        /*===| 得到云台控制参数 ===*/
//        Gimbal_Control_Struct.Gimbal_State = RoboControl_Struct.Gimbal_State;
        
        /*===| 得到自瞄位置与当前位置的偏移角度[-180 ~ +180度] |===*/
//        Gimbal_Control_Struct.Vision_Yaw_Err = INS_Data_Gimbal.Yaw - Aim_Receive_Struct.yaw;
//        if(Gimbal_Control_Struct.Vision_Yaw_Err > 180.0f) Gimbal_Control_Struct.Vision_Yaw_Err = Gimbal_Control_Struct.Vision_Yaw_Err - 360;
//        else if(Gimbal_Control_Struct.Vision_Yaw_Err < -180.0f) Gimbal_Control_Struct.Vision_Yaw_Err = 360 + Gimbal_Control_Struct.Vision_Yaw_Err;
        
//        if(Gimbal_Control_Struct.Gimbal_State == Gimbal_State_Aim && Aim_Receive_Struct.appear)
//        {
//            RoboControl_Struct.Robo_Target_Gimbal_Yaw   = Gimbal_Control_Struct.Yaw_Feedback - Gimbal_Control_Struct.Vision_Yaw_Err;
//            RoboControl_Struct.Robo_Target_Gimbal_Pitch = Aim_Receive_Struct.pitch;   
//        }
        
             
        
//		Gimbal_Control_Struct.Pitch_Target = RoboControl_Struct.Robo_Target_Gimbal_Pitch;  
//				
//        Gimbal_Control_Struct.Yaw_Target = RoboControl_Struct.Robo_Target_Chassis_Yaw;  

        
        /*===| 得到Yaw与底盘正方向的偏移角度[-180 ~ +180度] |===*/
//        Gimbal_Control_Struct.Yaw_Err = Motor_Data_Struct[Gimbal_Yaw_ID].Angle - Gimbal_Median_Yaw;
//        if(Gimbal_Control_Struct.Yaw_Err >= 180) Gimbal_Control_Struct.Yaw_Err = 360 - Gimbal_Control_Struct.Yaw_Err;
//        if(Gimbal_Control_Struct.Yaw_Err <= -180) Gimbal_Control_Struct.Yaw_Err = 360 + Gimbal_Control_Struct.Yaw_Err;
//        RoboControl_Struct.Yaw_Err = Gimbal_Control_Struct.Yaw_Err;
		
                
        /*===| 串级PID串级计算得到Yaw电机的电流大小 |===*/
//        PID_Position_Calculate(&Gimbal_Control_Struct.Yaw_Angle_PID_Struct, Gimbal_Control_Struct.Yaw_Target,                  Gimbal_Control_Struct.Yaw_Feedback);
//        PID_Position_Calculate(&Gimbal_Control_Struct.Yaw_Speed_PID_Struct, Gimbal_Control_Struct.Yaw_Angle_PID_Struct.Output, -Motor_Data_Struct[Gimbal_Yaw_ID].Speed_RPM);
//         
//        /*===| 串级PID串级计算得到Pitch电机的电流大小 |===*/
//        PID_Position_Calculate(&Gimbal_Control_Struct.Pitch_Angle_PID_Struct, Gimbal_Control_Struct.Pitch_Target,                  Gimbal_Control_Struct.Pitch_Feedback);
//        PID_Position_Calculate(&Gimbal_Control_Struct.Pitch_Speed_PID_Struct, Gimbal_Control_Struct.Pitch_Angle_PID_Struct.Output, -Motor_Data_Struct[Gimbal_Pitch_ID].Speed_RPM);
          
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*===| 遥控器断连处理 |===*/
        Remote.Remote_GetData_Ticker++;

        if(Remote.Remote_GetData_Ticker >= 500)
        {
            Remote.If_Remote_Connect = 0;
            Remote.Remote_GetData_Ticker = 500;
        }

        if(Remote.If_Remote_Connect == 0)
        {
            Robo_Stop();
        }
		
		
		/*===| CAN错误处理 |===*/
        CAN1_Err_Ticker++;
        CAN2_Err_Ticker++;
        CAN3_Err_Ticker++;
        if(CAN1_Err_Ticker > 100)
        {
            MX_FDCAN1_Init();
            HAL_FDCAN_Start(&hfdcan1);
            CAN_Filter_Init(&hfdcan1);
            CAN1_Err_Ticker = 0;
        }
        if(CAN2_Err_Ticker > 100)
        {
            MX_FDCAN2_Init();
            HAL_FDCAN_Start(&hfdcan2);
            CAN_Filter_Init(&hfdcan2);
            CAN2_Err_Ticker = 0;
        }
        if(CAN3_Err_Ticker > 100)
        {
            MX_FDCAN3_Init();
            HAL_FDCAN_Start(&hfdcan3);
            CAN_Filter_Init(&hfdcan3);
            CAN3_Err_Ticker = 0;
        }

		/*===| 轮电机离线检测 |===*/
		Motor_Data_Struct[5].Ticker++; 
		if(Motor_Data_Struct[5].Ticker >= 100) Motor_Data_Struct[5].If_Online = 0; 
		if(Motor_Data_Struct[5].Ticker >  110) Motor_Data_Struct[5].Ticker = 100;
		
        Motor_Data_Struct[6].Ticker++; 
		if(Motor_Data_Struct[6].Ticker >= 100) Motor_Data_Struct[6].If_Online = 0; 
		if(Motor_Data_Struct[6].Ticker >  110) Motor_Data_Struct[6].Ticker = 100;
		
		/*===| 关节电机离线检测 |===*/
		Motor_Data_Struct[1].Ticker++; 
		if(Motor_Data_Struct[1].Ticker >= 100) Motor_Data_Struct[1].If_Online = 0; 
		if(Motor_Data_Struct[1].Ticker >  110) Motor_Data_Struct[1].Ticker = 100;
		
        Motor_Data_Struct[2].Ticker++; 
		if(Motor_Data_Struct[2].Ticker >= 100) Motor_Data_Struct[2].If_Online = 0; 
		if(Motor_Data_Struct[2].Ticker >  110) Motor_Data_Struct[2].Ticker = 100;
		
        Motor_Data_Struct[3].Ticker++; 
		if(Motor_Data_Struct[3].Ticker >= 100) Motor_Data_Struct[3].If_Online = 0; 
		if(Motor_Data_Struct[3].Ticker >  110) Motor_Data_Struct[3].Ticker = 100;
		
        Motor_Data_Struct[4].Ticker++; 
		if(Motor_Data_Struct[4].Ticker >= 100) Motor_Data_Struct[4].If_Online = 0; 
		if(Motor_Data_Struct[4].Ticker >  110) Motor_Data_Struct[4].Ticker = 100;
        
		/*===| 关节电机离线断连处理 |===*/
        if(Motor_Data_Struct[1].Ticker == 110)      
        {
            Motor_DM_CMD_ClearErr(&Chassis_JointMotor_CAN, Chassis_JointMotor1_Send_CAN_ID);
            osDelay(1);
            Motor_DM_CMD_Enable(&Chassis_JointMotor_CAN,Chassis_JointMotor1_Send_CAN_ID);
			osDelay(1);
        }
		if(Motor_Data_Struct[2].Ticker == 110)      
        {
            Motor_DM_CMD_ClearErr(&Chassis_JointMotor_CAN, Chassis_JointMotor2_Send_CAN_ID);
            osDelay(1);
            Motor_DM_CMD_Enable(&Chassis_JointMotor_CAN,Chassis_JointMotor2_Send_CAN_ID);
			osDelay(1);
        }
		if(Motor_Data_Struct[3].Ticker == 110)      
        {
            Motor_DM_CMD_ClearErr(&Chassis_JointMotor_CAN, Chassis_JointMotor3_Send_CAN_ID);
            osDelay(1);
            Motor_DM_CMD_Enable(&Chassis_JointMotor_CAN,Chassis_JointMotor3_Send_CAN_ID);
			osDelay(1);
        }
		if(Motor_Data_Struct[4].Ticker == 110)      
        {
            Motor_DM_CMD_ClearErr(&Chassis_JointMotor_CAN, Chassis_JointMotor4_Send_CAN_ID);
            osDelay(1);
            Motor_DM_CMD_Enable(&Chassis_JointMotor_CAN,Chassis_JointMotor4_Send_CAN_ID);
			osDelay(1);
        }
		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		K1=-LQR_K_L[6]*Chassis_Control_Struct.State[1];
		K2=-LQR_K_L[7]*Chassis_Control_Struct.State[2];
		K3=-LQR_K_L[8]*Chassis_Control_Struct.State[3];
		K4=-LQR_K_L[9]*Chassis_Control_Struct.State[4];
		K5=-LQR_K_L[10]*Chassis_Control_Struct.State[5];
		K6=-LQR_K_L[11]*Chassis_Control_Struct.State[6];
		
//		UART4_Printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",K1,K2,K3,K4,K5,K6,Chassis_Control_Struct.Torque_Joint_left,Chassis_Control_Struct.Torque_wheel_left,Chassis_Control_Struct.Chassis_Vy_Target,Chassis_Control_Struct.Volecity_Fdb,Chassis_Control_Struct.Target_displacement,Chassis_Control_Struct.displacement);
		
//		UART4_Printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",Chassis_Control_Struct.Left_Leg_Angle_dot,Filter_Left_Leg_Angle_dot,Chassis_Control_Struct.Pitch_Speed_fdb,Filter_Pitch_Speed_fdb,Chassis_Control_Struct.Volecity_Fdb,Filter_Volecity_Fdb,Chassis_Control_Struct.Left_Leg_Angle_dot_last,Chassis_Control_Struct.Pitch_Speed_fdb_last,Chassis_Control_Struct.Volecity_Fdb_last,Chassis_Control_Struct.Right_Leg_Angle_dot,Filter_Right_Leg_Angle_dot);

//		UART4_Printf("%d,%d,%d,%d\n",Motor_Data_Struct[1].Error_ID,Motor_Data_Struct[2].Error_ID,Motor_Data_Struct[3].Error_ID,Motor_Data_Struct[4].Error_ID);

//		UART4_Printf("%f,%f,%f,%d\n",K_slope,Chassis_Control_Struct.F_L,Chassis_Control_Struct.F_R,Chassis_Control_Struct.Jump_State);
//		UART4_Printf("%f,%f,%f,%f\n",Chassis_Control_Struct.Chassis_Vy_Target,Chassis_Control_Struct.Volecity_Fdb,Chassis_Control_Struct.Target_displacement,Chassis_Control_Struct.displacement);
//		UART4_Printf("%f,%f,%f,%f\n",Chassis_Control_Struct.State[1]*Radain2Angle,Chassis_Control_Struct.State[2],Chassis_Control_Struct.State[7]*Radain2Angle,Chassis_Control_Struct.State[8]);
//		UART4_Printf("%f,%f,%f\n",INS_Data_Self.Roll,INS_Data_Self.Gyro[X],INS_Data_Self.Gyro[Y]);
//		UART4_Printf("%f,%f,%f,%f,%d\n",RoboControl_Struct.SuperCap_V,Chassis_Control_Struct.Chassis_Vy_Target,Chassis_Control_Struct.Volecity_Fdb,Chassis_Control_Struct.Chassis_Wz_Target,Robo_PowerHeatData.buffer_energy);
//		UART4_Printf("%f,%f\n",Chassis_Control_Struct.Volecity_Fdb,Chassis_Control_Struct.Chassis_Vy_Target);
//		UART4_Printf("%f,%f,%f,%d\n",INS_Data_Self.Pitch,INS_Data_Self.Roll,INS_Data_Self.Yaw,Chassis_Control_Struct.Recover_State);
		
//		UART4_Printf("%f,%f,%f,%f,%f,%f,%f\n",spin_time,Spin_T,Spin_move_Direction,Move_time,Move_Flag,Chassis_Yaw,Chassis_Control_Struct.Chassis_Vy_Target);
		
//		UART4_Printf("%f,%f,%f,%d\n",Fn_l,Fn_r,Chassis_Control_Struct.MotionAccel_n_Z,Chassis_Control_Struct.Off_ground_Flag);
		
//		UART4_Printf("%f,%f,%f,%f,%f,%f,%f\n",RoboControl_Struct.Leg_Length,Chassis_Control_Struct.Left_Leg_Length,Chassis_Control_Struct.Left_leg_Length_PID.Output,Chassis_Control_Struct.Left_Leg_Length_dot,Chassis_Control_Struct.Right_Leg_Length,Chassis_Control_Struct.Right_leg_Length_PID.Output,Chassis_Control_Struct.Right_Leg_Length_dot);

//		UART4_Printf("%f,%f,%f,%f\n",Chassis_Control_Struct.Left_Leg_Angle_dot*Angle2Radain,Chassis_Control_Struct.Right_Leg_Angle_dot*Angle2Radain,Chassis_Control_Struct.Pitch_Speed_fdb,Chassis_Control_Struct.Volecity_Fdb);
	
//		UART4_Printf("%f,%f,%f,%f\n",Chassis_Control_Struct.Torque_Joint_left,Chassis_Control_Struct.Torque_Joint_right,Chassis_Control_Struct.Torque_wheel_left,Chassis_Control_Struct.Torque_wheel_right);
//		USB_Printf("%f,%f,%f,%f\n",Chassis_Control_Struct.Volecity_L,V_L,Chassis_Control_Struct.Volecity_R,V_R);
//		USB_Printf("%f,%f\n",Chassis_Control_Struct.Torque_wheel_left,Chassis_Control_Struct.Torque_wheel_right);
//		USB_Printf("%f,%f\n",Chassis_Control_Struct.Torque_wheel_left,Chassis_Control_Struct.Torque_wheel_right);
//		USB_Printf("%f,%f\n",Chassis_Control_Struct.Torque_Joint_left,Chassis_Control_Struct.Torque_Joint_right);
//		USB_Printf("%f,%f\n",Chassis_Control_Struct.F_L,Chassis_Control_Struct.F_R);
//		USB_Printf("%f,%f\n",(Chassis_Control_Struct.F_L+Chassis_Control_Struct.Torque_Joint_left),(Chassis_Control_Struct.F_R+Chassis_Control_Struct.Torque_Joint_right));
		
//		USB_Printf("%f\n",Chassis_Control_Struct.Volecity_Fdb);//轮电机计算出的速度 m/s
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
		
        /*===| 发送电流参数 |===*/
        if(RoboControl_Struct.Robo_Enable && INS_Data_Gimbal.If_INS_Init) 
        {
//            Motor_DM_CMD_SetZero(&Gimbal_Yaw_CAN, Gimbal_Yaw_Send_CAN_ID);
//            Motor_DM_CMD_SetZero(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID);
//            Motor_DM_CMD_MIT(&Gimbal_Yaw_CAN, Gimbal_Yaw_Send_CAN_ID, 0, 0, 0, 0, -Gimbal_Control_Struct.Yaw_Speed_PID_Struct.Output);
//            Motor_DM_CMD_MIT(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID, 0, 0, 0, 0, Gimbal_Control_Struct.Pitch_Speed_PID_Struct.Output);
            //Motor_DM1to4_SendCurrent(&Gimbal_Yaw_CAN, 0x3FE, -Gimbal_Control_Struct.Pitch_Speed_PID_Struct.Output, -Gimbal_Control_Struct.Yaw_Speed_PID_Struct.Output, 0, 0);
        }
        else 
        {
//            Motor_DM_CMD_Disable(&Gimbal_Pitch_CAN, Gimbal_Pitch_Send_CAN_ID);
//            Motor_DM_CMD_Disable(&Gimbal_Yaw_CAN, Gimbal_Yaw_Send_CAN_ID);
           // Motor_DM1to4_SendCurrent(&Gimbal_Yaw_CAN, 0x3FE, 0, 0, 0, 0);
        }
				//UART4_Printf("%f\n", -Motor_Data_Struct[Gimbal_Yaw_ID].Speed_RPM);

//		static uint32_t DWT_Count;
//		float Dt = DWT_GetDeltaT(&DWT_Count);

//		static uint8_t tick = 0;
//		tick ++;
//		if(tick == 5)
//		{
//			Chassis_Control_Struct.Left_Leg_Length_dot = (Chassis_Control_Struct.Left_Leg_Length - Chassis_Control_Struct.Left_Leg_Length_last)/Dt;
//			Chassis_Control_Struct.Left_Leg_Angle_dot = (Chassis_Control_Struct.Left_Leg_Angle - Chassis_Control_Struct.Left_Leg_Angle_last) /Dt;
//			Chassis_Control_Struct.Right_Leg_Length_dot = (Chassis_Control_Struct.Right_Leg_Length - Chassis_Control_Struct.Right_Leg_Length_last)/Dt;
//			Chassis_Control_Struct.Right_Leg_Angle_dot = (Chassis_Control_Struct.Right_Leg_Angle - Chassis_Control_Struct.Right_Leg_Angle_last) /Dt;
//			Chassis_Control_Struct.Pitch_Speed_fdb = (Chassis_Control_Struct.Pitch_Angle - Chassis_Control_Struct.Pitch_Angle_Last)/Dt;//pitch一阶导
//			
//			Chassis_Control_Struct.Pitch_Angle_Last = Chassis_Control_Struct.Pitch_Angle;
//			Chassis_Control_Struct.Left_Leg_Length_last = Chassis_Control_Struct.Left_Leg_Length;
//			Chassis_Control_Struct.Left_Leg_Angle_last = Chassis_Control_Struct.Left_Leg_Angle;
//			Chassis_Control_Struct.Right_Leg_Length_last = Chassis_Control_Struct.Right_Leg_Length;
//			Chassis_Control_Struct.Right_Leg_Angle_last = Chassis_Control_Struct.Right_Leg_Angle;

//			tick = 0;
//		}
			
        osDelay(1);
    }
}
