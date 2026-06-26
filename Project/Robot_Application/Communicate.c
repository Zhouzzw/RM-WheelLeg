//========================================================
//                      Commnuicate[双板通信]
//      双板通信的发送和解包
//      当前有；云台Vx,Vy,Wz,YawErr，以及一些状态标志位
//========================================================

#include "Communicate.h"
#include "RoboControl.h"
#include "INS.h"
#include "Shoot.h"
#include "SuperCap_Driver.h"
#include "Vofa.h"
#include "Laser.h"
#include "Aim.h"
#include "Remote_Control.h"

/*===| 通信错误看门狗---如果10ms内没有收到双板通信数据，则重置底盘状态 |===*/
extern uint8_t IF_Chassis_Online;

Double_Board_Up_to_Down_TypedefStruct Double_Board_Up_to_Down;
Remote_Pack1_TypedefStruct Remote_Pack1;
Remote_Pack2_TypedefStruct Remote_Pack2;

Double_Board_Down_to_UP_Referee_TypedefStruct Double_Board_Down_to_UP_Referee;
Double_Board_Down_to_UP_TypedefStruct Double_Board_Down_to_UP;


/**
 * @brief 通信任务
 */
void Communicate_Task(void *argument)
{
    for(;;)
    {
		SuperCap_Send_Control_Data();
		
		Chassis_Send_Referee_Unpack();
		Chassis_Send_Func_Data();
		
		osDelay(5);
    }  
}

void Chassis_Receive_attitude_Pack(uint8_t *Data)
{
    IF_Chassis_Online = 1;
    
    memcpy(&Double_Board_Up_to_Down, Data, sizeof(Double_Board_Up_to_Down_TypedefStruct));

    RoboControl_Struct.Yaw_Err                         = Double_Board_Up_to_Down.Yaw_Errx100               /  100.0f   ;
    INS_Data_Gimbal.YawTotalAngle                      = Double_Board_Up_to_Down.Gimbal_Yaw_TotalAnglex100 /  100.0f   ;                                                                                            
    Aim_DOuble_up_to_down_Struct.Aim_If_Allow_Shoot    = Double_Board_Up_to_Down.Aim_If_Allow_Shoot                    ;
    RoboControl_Struct.Shoot_State                     = Double_Board_Up_to_Down.Shoot_State;
    RoboControl_Struct.Gimbal_State                    = Double_Board_Up_to_Down.Gimbal_State;
	
	Motor_Data_Struct[Shoot_Fric_First_Left_ID].Speed_RPM				= Double_Board_Up_to_Down.Fric1_Speed ;
	Motor_Data_Struct[Shoot_Fric_First_Right_ID].Speed_RPM 				= Double_Board_Up_to_Down.Fric2_Speed ;
    
    INS_Data_Gimbal.Pitch                              = Double_Board_Up_to_Down.Gimbal_Pitch;

	Aim_Send_Struct.operator_yaw_offset			   = Double_Board_Up_to_Down.operator_yaw_offsetx10   / 10.0f;
	Aim_Send_Struct.operator_pitch_offset		   = Double_Board_Up_to_Down.operator_pitch_offsetx10 / 10.0f;
}

void Chassis_Receive_Remote_Pack1(uint8_t *Data)
{
	Remote.Remote_GetData_Ticker = 0;
	IF_Chassis_Online = 1;
	Remote.If_Remote_Data_New = 1;
	
	memcpy(&Remote_Pack1, Data, sizeof(Remote_Pack1_TypedefStruct));
	
	Remote_Last.Right_X				=Remote.Right_X				;
	Remote_Last.Right_Y				=Remote.Right_Y				;
	Remote_Last.Left_X				=Remote.Left_X				;
	Remote_Last.Left_Y				=Remote.Left_Y				;
	Remote_Last.Wheel				=Remote.Wheel				;
	Remote_Last.If_Remote_Connect	=Remote.If_Remote_Connect	;
	Remote_Last.Mode				=Remote.Mode				;
	Remote_Last.Pause				=Remote.Pause				;
	Remote_Last.Custom_L			=Remote.Custom_L			;
	Remote_Last.Custom_R			=Remote.Custom_R			;
	Remote_Last.Trigger				=Remote.Trigger				;
	
	Remote.Right_X				= Remote_Pack1.Right_X	/127.0f		;					
	Remote.Right_Y				= Remote_Pack1.Right_Y	/127.0f		;			
	Remote.Left_X				= Remote_Pack1.Left_X	/127.0f		;			
	Remote.Left_Y				= Remote_Pack1.Left_Y	/127.0f		;					
	Remote.Wheel				= Remote_Pack1.Wheel	/127.0f		;				
	Remote.If_Remote_Connect	= Remote_Pack1.If_Remote_Connect	;
	Remote.Mode					= Remote_Pack1.Mode					;
	Remote.Pause				= Remote_Pack1.Pause				;
	Remote.Custom_L				= Remote_Pack1.Custom_L				;
	Remote.Custom_R				= Remote_Pack1.Custom_R				;
	Remote.Trigger				= Remote_Pack1.Trigger				;
}

void Chassis_Receive_Remote_Pack2(uint8_t *Data)
{
	Remote.Remote_GetData_Ticker = 0;
	IF_Chassis_Online = 1;

	memcpy(&Remote_Pack2, Data, sizeof(Remote_Pack2_TypedefStruct));

	Remote_Last.Mouse_Vx                =    Remote.Mouse_Vx        ;
	Remote_Last.Mouse_Vy                =    Remote.Mouse_Vy        ;
	Remote_Last.Mouse_Vz                =    Remote.Mouse_Vz        ;
	Remote_Last.Mouse_L                 =    Remote.Mouse_L         ;
	Remote_Last.Mouse_M                 =    Remote.Mouse_M         ;
	Remote_Last.Mouse_R                 =    Remote.Mouse_R         ;
	Remote_Last.Keyboard_W              =    Remote.Keyboard_W      ;
	Remote_Last.Keyboard_A              =    Remote.Keyboard_A      ;
	Remote_Last.Keyboard_S              =    Remote.Keyboard_S      ;
	Remote_Last.Keyboard_D              =    Remote.Keyboard_D      ;
	Remote_Last.Keyboard_Q              =    Remote.Keyboard_Q      ;
	Remote_Last.Keyboard_E              =    Remote.Keyboard_E      ;
	Remote_Last.Keyboard_Shift          =    Remote.Keyboard_Shift  ;
	Remote_Last.Keyboard_Ctrl           =    Remote.Keyboard_Ctrl   ;
	Remote_Last.Keyboard_R              =    Remote.Keyboard_R      ;
	Remote_Last.Keyboard_F              =    Remote.Keyboard_F      ;
	Remote_Last.Keyboard_G              =    Remote.Keyboard_G      ;
	Remote_Last.Keyboard_Z              =    Remote.Keyboard_Z      ;
	Remote_Last.Keyboard_X              =    Remote.Keyboard_X      ;
	Remote_Last.Keyboard_C              =    Remote.Keyboard_C      ;
	Remote_Last.Keyboard_V              =    Remote.Keyboard_V      ;
	Remote_Last.Keyboard_B              =    Remote.Keyboard_B      ;
	
	Remote.Mouse_Vx				= Remote_Pack2.Mouse_Vx	/127.0f		;
	Remote.Mouse_Vy				= Remote_Pack2.Mouse_Vy	/127.0f		;
	Remote.Mouse_Vz				= Remote_Pack2.Mouse_Vz	/127.0f		;
	Remote.Mouse_L				= Remote_Pack2.Mouse_L				;
	Remote.Mouse_M				= Remote_Pack2.Mouse_M				;
	Remote.Mouse_R				= Remote_Pack2.Mouse_R				;
	Remote.Keyboard_W			= Remote_Pack2.Keyboard_W			;
	Remote.Keyboard_A			= Remote_Pack2.Keyboard_A			;
	Remote.Keyboard_S			= Remote_Pack2.Keyboard_S			;
	Remote.Keyboard_D			= Remote_Pack2.Keyboard_D			;
	Remote.Keyboard_Q			= Remote_Pack2.Keyboard_Q			;
	Remote.Keyboard_E			= Remote_Pack2.Keyboard_E			;
	Remote.Keyboard_Shift		= Remote_Pack2.Keyboard_Shift		;
	Remote.Keyboard_Ctrl		= Remote_Pack2.Keyboard_Ctrl		;
	Remote.Keyboard_R			= Remote_Pack2.Keyboard_R			;
	Remote.Keyboard_F			= Remote_Pack2.Keyboard_F			;
	Remote.Keyboard_G			= Remote_Pack2.Keyboard_G			;
	Remote.Keyboard_Z			= Remote_Pack2.Keyboard_Z			;
	Remote.Keyboard_X			= Remote_Pack2.Keyboard_X			;
	Remote.Keyboard_C			= Remote_Pack2.Keyboard_C			;
	Remote.Keyboard_V			= Remote_Pack2.Keyboard_V			;
	Remote.Keyboard_B			= Remote_Pack2.Keyboard_B			;
}

void Chassis_Send_Referee_Unpack(void)
{
    Double_Board_Down_to_UP_Referee.Shoot_Qmax     = Robo_State.shooter_barrel_heat_limit;
    Double_Board_Down_to_UP_Referee.Shoot_Qnow     = Robo_PowerHeatData.shooter_17mm_barrel_heat;
    Double_Board_Down_to_UP_Referee.robot_id  	   = Robo_State.robot_id;
	Double_Board_Down_to_UP_Referee.initial_speedx100 = Robo_ShootData.initial_speed * 100;
	CAN_Send_Data(&hfdcan3, 0x53, (uint8_t *)&Double_Board_Down_to_UP_Referee);
}

void Chassis_Send_Func_Data(void)
{
	Double_Board_Down_to_UP.Recover_from_ground_Flag = Chassis_Control_Struct.Recover_from_ground_Flag;
    Double_Board_Down_to_UP.Chassis_Odometer_Vxx100  = Chassis_Control_Struct.Volecity_Fdb*arm_cos_f32(RoboControl_Struct.Yaw_Err * Angle2Radain) * 100.0f;
    Double_Board_Down_to_UP.Chassis_Odometer_Vyx100  = Chassis_Control_Struct.Volecity_Fdb*arm_sin_f32(RoboControl_Struct.Yaw_Err * Angle2Radain) * 100.0f;
	Double_Board_Down_to_UP.Cooling_value 			 = Robo_State.shooter_barrel_cooling_value;
	Double_Board_Down_to_UP.null2 = 0;
	CAN_Send_Data(&hfdcan3, 0x54, (uint8_t *)&Double_Board_Down_to_UP);
}
