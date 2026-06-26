#include "UI_Task.h"
#include "RoboControl.h"
#include "chassis.h"
#include "Referee_Unpack.h"
#include "INS.h"
#include "Aim.h"
#include "Gimbal.h"

float CAM_Angle_X = 64.4;
float CAM_Angle_Y = 40;
float CAM_X_ADD = 13;
float CAM_Y_ADD = -45;

int UI_SELF_ID;

UI_State_StructTypedef UI_State;

void UI_Init()
{
    ui_init_g_Dynamic();
    ui_init_g_Static();
}

void UI_Update()
{
    ui_update_g_Dynamic();
}

void UI_Task(void const * argument)
{
	while(Robo_State.robot_id == 0) osDelay(1000);
	UI_SELF_ID = Robo_State.robot_id;
	
	UI_Init();
    
	while(1)
	{
        UI_SELF_ID = Robo_State.robot_id;
        
        /*===| 如果要求重刷UI，则重新初始化 |===*/
        if(RoboControl_Struct.Refresh_UI_Flag)
        {
            UI_Init();
        }
        else
        {
            /*===| 获取UI显示数据 |===*/
            UI_State.Pitch       = INS_Data_Gimbal.Pitch;                      
            UI_State.Yaw         = -RoboControl_Struct.Robo_Target_Gimbal_Yaw;
            UI_State.YawErr      = RoboControl_Struct.Yaw_Err;   
			
			UI_State.Left_Leg_Angle  = Chassis_Control_Struct.Left_Leg_Angle;
			UI_State.Right_Leg_Angle = -Chassis_Control_Struct.Right_Leg_Angle;
			
			UI_State.Fric1_Speed	 = fabs(Motor_Data_Struct[Shoot_Fric_First_Left_ID].Speed_RPM *4.0f);		 
			UI_State.Fric2_Speed	 = fabs(Motor_Data_Struct[Shoot_Fric_First_Right_ID].Speed_RPM*4.0f);
			
            UI_State.Shoot_State = RoboControl_Struct.Shoot_State;
			UI_State.Gimbal_State = RoboControl_Struct.Gimbal_State;
			
            UI_State.Bullet      = Robo_BulletRemain.projectile_allowance_42mm;
            UI_State.SuperCap_IF_Enable = RoboControl_Struct.SuperCap_State;
            UI_State.SuperCap_K = (RoboControl_Struct.SuperCap_V - 6.0f) / 20.0f; //超电安全电压:8~25.5V
            if(UI_State.SuperCap_K < 0) UI_State.SuperCap_K = 0;
            UI_State.Chassis_State = RoboControl_Struct.Chassis_State;
            UI_State.Chassis_Speed_Level = RoboControl_Struct.Chassis_Speed_Level;
			
			UI_State.Leg_Length = RoboControl_Struct.Leg_Length * 100;
			UI_State.Volecity_Fdb = Chassis_Control_Struct.Volecity_Fdb * 10;

            /*===| Pitch |===*/
            ui_g_Dynamic_Pitch->start_angle = 270 + UI_State.Pitch;
            ui_g_Dynamic_Pitch->end_angle   = 271 + UI_State.Pitch;
            ui_g_Dynamic_Pitch_Num->number = UI_State.Pitch;
            
            /*===| Armor|===*/
            ui_g_Dynamic_Armor->start_x = 1633 + 150 * cos((UI_State.YawErr - 10 + 90) * 2.0f*PI/360.0f);
            ui_g_Dynamic_Armor->start_y = 703  + 150 * sin((UI_State.YawErr - 10 + 90) * 2.0f*PI/360.0f);
            ui_g_Dynamic_Armor->end_x   = 1633 + 150 * cos((UI_State.YawErr + 10 + 90) * 2.0f*PI/360.0f);
            ui_g_Dynamic_Armor->end_y   = 703  + 150 * sin((UI_State.YawErr + 10 + 90) * 2.0f*PI/360.0f);
			
			/*===| Fric |===*/
            ui_g_Dynamic_Fric_L->start_angle  = 0 ;
            ui_g_Dynamic_Fric_R->start_angle  = 0 ;
            ui_g_Dynamic_Fric_L->end_angle    = 1 + 330 * (UI_State.Fric1_Speed / 19.0f);
            ui_g_Dynamic_Fric_R->end_angle    = 1 + 330 * (UI_State.Fric2_Speed / 19.0f);
            
            /*===| SuperCap |===*/
            ui_g_Dynamic_SuperCap->start_angle = 129 - UI_State.SuperCap_K * 79;
            if(UI_State.SuperCap_K > 0.3f)     ui_g_Dynamic_SuperCap->color = 6;
            else                                    ui_g_Dynamic_SuperCap->color = 5;
            
            if(UI_State.SuperCap_IF_Enable == 0)    ui_g_Dynamic_SuperCap->color = 4;
            
            /*===| Speed_Level |===*/
            ui_g_Dynamic_Speed->number = UI_State.Chassis_Speed_Level;
			
			/*===| Volecity |===*/
//            ui_g_Dynamic_Speed_Num->number = UI_State.Volecity_Fdb;
            
            /*===| Leg_Length |===*/
            ui_g_Dynamic_LegLength_Num->number = UI_State.Leg_Length;
			
			/*===| Shoot |===*/
			if(UI_State.Shoot_State == Shoot_State_Off)
			{
				String_Printf(ui_g_Dynamic_Shoot_State->string, "Shoot_Off     ");
			}
			else if(UI_State.Shoot_State == Shoot_State_Ready)
			{
				String_Printf(ui_g_Dynamic_Shoot_State->string, "Shoot_Ready   ");
			}
			else if(UI_State.Shoot_State == Shoot_State_Continue)
			{
				String_Printf(ui_g_Dynamic_Shoot_State->string, "Shoot_Continue");
			}
			
            /*===| Aim |===*/
            if(UI_State.Gimbal_State == Gimbal_State_Aim)
            {
                ui_g_Dynamic_Aim->color = 5; 
//				String_Printf(ui_g_Dynamic_Aim_State_NF->string, "ON");
            }
            else
            {
                ui_g_Dynamic_Aim->color = 1;
//				String_Printf(ui_g_Dynamic_Aim_State_NF->string, "OFF");				
            }
			/*===| Chassis |===*/
			if(Chassis_Control_Struct.Chassis_State == Chassis_OFF)
            {
				String_Printf(ui_g_Dynamic_Chassis_State->string, "Chassis_OFF   ");
            }
            else if(Chassis_Control_Struct.Chassis_State == Chassis_STATIC)
            {
				String_Printf(ui_g_Dynamic_Chassis_State->string, "Chassis_STATIC");				
            }
            else if(Chassis_Control_Struct.Chassis_State == Chassis_FOLLOW)
            {
				String_Printf(ui_g_Dynamic_Chassis_State->string, "Chassis_FOLLOW");				
            }
			else if(Chassis_Control_Struct.Chassis_State == Chassis_SPIN)
            {
				String_Printf(ui_g_Dynamic_Chassis_State->string, "Chassis_SPIN  ");				
            }
			else if(Chassis_Control_Struct.Chassis_State == Chassis_Bench)
            {
				String_Printf(ui_g_Dynamic_Chassis_State->string, "Chassis_Bench ");				
            }
			
			
			/*===| Leg_Attitude |===*/
			//up 	startpoint : joint;	endpoint : knee
			//down	startpoint : foot;	endpoint : knee
			//		sinA = 0.84; cosA = 0.55;
			//		OK = 99;	 KF = 131;	 OF = 156;(可改为腿长，但余弦不为定值计算量增大)
			
			/*===| Left_Leg_Attitude |===*/
			ui_g_Dynamic_Left_up_Leg->start_x = 1624;
			ui_g_Dynamic_Left_up_Leg->start_y = 807;
			
			ui_g_Dynamic_Left_up_Leg->end_x = 1624 + 99 * (0.84 * cos(UI_State.Left_Leg_Angle * 2.0f*PI/360.0f) - 0.55 * sin(UI_State.Left_Leg_Angle * 2.0f*PI/360.0f));
			ui_g_Dynamic_Left_up_Leg->end_y = 807 + 99 * (0.55 * cos(UI_State.Left_Leg_Angle * 2.0f*PI/360.0f) - 0.84 * sin(UI_State.Left_Leg_Angle * 2.0f*PI/360.0f));
			
			ui_g_Dynamic_Left_down_Leg->start_x = 1624 - 156 * sin(UI_State.Left_Leg_Angle * 2.0f*PI/360.0f);
			ui_g_Dynamic_Left_down_Leg->start_y = 807 - 156 * cos(UI_State.Left_Leg_Angle * 2.0f*PI/360.0f);
			
			ui_g_Dynamic_Left_down_Leg->end_x = ui_g_Dynamic_Left_up_Leg->end_x;
			ui_g_Dynamic_Left_down_Leg->end_y = ui_g_Dynamic_Left_up_Leg->end_y;
			
//			ui_g_Dynamic_Left_down_Leg->end_x = 1624 + 99 * (0.84 * cos(UI_State.Joint_Angle * 2.0f*PI/360.0f) - 0.55 * sin(UI_State.Joint_Angle * 2.0f*PI/360.0f));
//			ui_g_Dynamic_Left_down_Leg->end_y = 807 + 99 * (0.55 * cos(UI_State.Joint_Angle * 2.0f*PI/360.0f) - 0.84 * sin(UI_State.Joint_Angle * 2.0f*PI/360.0f));
            
			/*===| Right_Leg_Attitude |===*/
			ui_g_Dynamic_Right_up_Leg->start_x = 1630;
			ui_g_Dynamic_Right_up_Leg->start_y = 807;
			
			ui_g_Dynamic_Right_up_Leg->end_x = 1630 + 99 * (0.84 * cos(UI_State.Right_Leg_Angle * 2.0f*PI/360.0f) - 0.55 * sin(UI_State.Right_Leg_Angle * 2.0f*PI/360.0f));
			ui_g_Dynamic_Right_up_Leg->end_y = 807 + 99 * (0.55 * cos(UI_State.Right_Leg_Angle * 2.0f*PI/360.0f) - 0.84 * sin(UI_State.Right_Leg_Angle * 2.0f*PI/360.0f));
				
			ui_g_Dynamic_Right_down_Leg->start_x = 1630 - 156 * sin(UI_State.Right_Leg_Angle * 2.0f*PI/360.0f);
			ui_g_Dynamic_Right_down_Leg->start_y = 807 - 156 * cos(UI_State.Right_Leg_Angle * 2.0f*PI/360.0f);
			
			ui_g_Dynamic_Right_down_Leg->end_x = ui_g_Dynamic_Right_up_Leg->end_x;
			ui_g_Dynamic_Right_down_Leg->end_y = ui_g_Dynamic_Right_up_Leg->end_y;
			
//			ui_g_Dynamic_Right_down_Leg->end_x = 1708 + 99 * (0.84 * cos(UI_State.Joint_Angle * 2.0f*PI/360.0f) - 0.55 * sin(UI_State.Joint_Angle * 2.0f*PI/360.0f));
//			ui_g_Dynamic_Right_down_Leg->end_y = 741 + 99 * (0.55 * cos(UI_State.Joint_Angle * 2.0f*PI/360.0f) - 0.84 * sin(UI_State.Joint_Angle * 2.0f*PI/360.0f));

			/*===| Left_Leg_Attitude |===*/
			/*===| L档 |===*/
			if(Chassis_Control_Struct.Leg_length_Level == 1)
			{
				ui_g_Dynamic_LegLength_State->start_x = 703;
				ui_g_Dynamic_LegLength_State->start_y = 311;
				ui_g_Dynamic_LegLength_State->end_x = 753;
				ui_g_Dynamic_LegLength_State->end_y = 361;
			}
			/*===| M1档 |===*/
			else if(Chassis_Control_Struct.Leg_length_Level == 2)
			{
				ui_g_Dynamic_LegLength_State->start_x = 703;
				ui_g_Dynamic_LegLength_State->start_y = 373;
				ui_g_Dynamic_LegLength_State->end_x = 780;
				ui_g_Dynamic_LegLength_State->end_y = 439;

			}
			/*===| M2档 |===*/
			else if(Chassis_Control_Struct.Leg_length_Level == 3)
			{
				ui_g_Dynamic_LegLength_State->start_x = 802;
				ui_g_Dynamic_LegLength_State->start_y = 373;
				ui_g_Dynamic_LegLength_State->end_x = 879;
				ui_g_Dynamic_LegLength_State->end_y = 439;
			}
			/*===| H档 |===*/
			else if(Chassis_Control_Struct.Leg_length_Level == 4)
			{
				ui_g_Dynamic_LegLength_State->start_x = 703;
				ui_g_Dynamic_LegLength_State->start_y = 453;
				ui_g_Dynamic_LegLength_State->end_x = 753;
				ui_g_Dynamic_LegLength_State->end_y = 501;
			}

//            if(Aim_Receive_Struct.fire)
//            {
//                float Pos_Pitch = atanf((ReceivedPacketVision.z + 0.18) / Caculate_Distence(ReceivedPacketVision.x, ReceivedPacketVision.y));
//                
//                ui_g_Dynamic_Aim->start_x = 1920/2 - 1920/2 *(-Gimbal_Control_Struct.Vision_Yaw_Err) / CAM_Angle_X - 30 + CAM_X_ADD;
//                ui_g_Dynamic_Aim->start_y = 1080/2 + 1080/2 *(Pos_Pitch - Gimbal_Control_Struct.Pitch_Feedback) / CAM_Angle_Y + 30 + CAM_Y_ADD;
//                ui_g_Dynamic_Aim->end_x   = 1920/2 - 1920/2 *(-Gimbal_Control_Struct.Vision_Yaw_Err) / CAM_Angle_X + 30 + CAM_X_ADD;
//                ui_g_Dynamic_Aim->end_y   = 1080/2 + 1080/2 *(Pos_Pitch - Gimbal_Control_Struct.Pitch_Feedback) / CAM_Angle_Y - 30 + CAM_Y_ADD; 
//            }
//            else    
//            {
//                ui_g_Dynamic_Aim->start_x = 911;
//                ui_g_Dynamic_Aim->start_y = 686;
//                ui_g_Dynamic_Aim->end_x = 1011;
//                ui_g_Dynamic_Aim->end_y = 786;
//            } 

             /*===| 自瞄偏置 |===*/
//            String_Printf(ui_g_Dynamic_Yaw_OFFnum->string, "%02.1f", Aim_Send_Struct.operator_yaw_offset);
//            String_Printf(ui_g_Dynamic_PITCH_OFFnum->string, "%02.1f", Aim_Send_Struct.operator_pitch_offset);

			
            /*===| 更新UI |===*/     
            UI_Update();
        }
        
    }	
}

void String_Printf(char *String_Arr, const char *fmt,...)
{
    static va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)String_Arr, fmt, ap);
    va_end(ap);
}
