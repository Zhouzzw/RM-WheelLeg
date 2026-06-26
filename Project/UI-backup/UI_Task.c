#include "UI_Task.h"
#include "RoboControl.h"
#include "chassis.h"
#include "Referee_Unpack.h"

int UI_SELF_ID;


UI_State_StructTypedef UI_State;

void UI_Init()
{
    ui_init_default_Dynamic();
    ui_init_default_Static();
}

void UI_Update()
{
    ui_update_default_Dynamic();
}

void UI_Task(void const * argument)
{
	while(Robo_State.robot_id == 0) osDelay(1000);
	UI_SELF_ID = Robo_State.robot_id;
	
	UI_Init();
    
	while(1)
	{
        /*===| 如果要求重刷UI，则重新初始化 |===*/
        if(RoboControl_Struct.Refresh_UI_Flag)
        {
            UI_Init();
        }
        else
        {
            /*===| 获取UI显示数据 |===*/
            UI_State.Pitch       = RoboControl_Struct.Robo_Target_Pitch;                      
            UI_State.Yaw         = -RoboControl_Struct.Robo_Target_Yaw;
            UI_State.YawErr      = RoboControl_Struct.Yaw_Err;   
            UI_State.Shoot_State = RoboControl_Struct.Shoot_State;
            UI_State.Fric1_Speed = Motor_Data_Struct[Shoot_Fric_First_Left_ID].Speed_RPM     * Fric_RPM_to_m_s;
            UI_State.Fric2_Speed = Motor_Data_Struct[Shoot_Fric_First_Right_ID].Speed_RPM    * Fric_RPM_to_m_s;
            UI_State.Fric3_Speed = Motor_Data_Struct[Shoot_Fric_First_Middle_ID].Speed_RPM   * Fric_RPM_to_m_s;       
            UI_State.Fric4_Speed = Motor_Data_Struct[Shoot_Fric_Second_Left_ID].Speed_RPM    * Fric_RPM_to_m_s;
            UI_State.Fric5_Speed = Motor_Data_Struct[Shoot_Fric_Second_Right_ID].Speed_RPM   * Fric_RPM_to_m_s;
            UI_State.Fric6_Speed = Motor_Data_Struct[Shoot_Fric_Second_Middle_ID].Speed_RPM  * Fric_RPM_to_m_s;   
            UI_State.Bullet      = Robo_BulletRemain.projectile_allowance_42mm;
            UI_State.SuperCap_IF_Enable = RoboControl_Struct.SuperCap_State;
            UI_State.SuperCap_K = (RoboControl_Struct.SuperCap_V - 6.0f) / 20.0f; //超电安全电压:8~25.5V
            if(UI_State.SuperCap_K < 0) UI_State.SuperCap_K = 0;
            UI_State.Chassis_Motor1_Speed = Motor_Data_Struct[Chassis_Motor1_ID].Speed_RPM * Chassis_RPM_to_m_s;
            UI_State.Chassis_Motor2_Speed = Motor_Data_Struct[Chassis_Motor2_ID].Speed_RPM * Chassis_RPM_to_m_s;
            UI_State.Chassis_Motor3_Speed = Motor_Data_Struct[Chassis_Motor4_ID].Speed_RPM * Chassis_RPM_to_m_s;
            UI_State.Chassis_Motor4_Speed = Motor_Data_Struct[Chassis_Motor3_ID].Speed_RPM * Chassis_RPM_to_m_s;
            UI_State.Chassis_State = RoboControl_Struct.Chassis_State;
            UI_State.Chassis_Speed_Level = RoboControl_Struct.Chassis_Speed_Level;
            
            
            /*===| Pitch |===*/
            ui_default_Dynamic_Pitch->start_angle = 270 + UI_State.Pitch;
            ui_default_Dynamic_Pitch->end_angle   = 271 + UI_State.Pitch;
            
            /*===| Armor|===*/
            if(UI_State.YawErr < 0) UI_State.YawErr += 360;
            ui_default_Dynamic_Armor->start_angle = -20  + UI_State.YawErr;
            ui_default_Dynamic_Armor->end_angle = 20 + UI_State.YawErr;
            if(ui_default_Dynamic_Armor->start_angle > 360) ui_default_Dynamic_Armor->start_angle -= 360;
            else if(ui_default_Dynamic_Armor->start_angle < 0) ui_default_Dynamic_Armor->start_angle += 360;
            if(ui_default_Dynamic_Armor->end_angle > 360) ui_default_Dynamic_Armor->end_angle -= 360;
            else if(ui_default_Dynamic_Armor->end_angle < 0) ui_default_Dynamic_Armor->end_angle += 360;
            
            /*===| Fric |===*/
            ui_default_Dynamic_Fric_L->end_y = 638 + 100 * UI_State.Fric1_Speed / 20.0f;
            ui_default_Dynamic_Fric_M->end_y = 638 + 100 * UI_State.Fric3_Speed / 20.0f;
            ui_default_Dynamic_Fric_R->end_y = 638 + 100 * UI_State.Fric2_Speed / 20.0f;
        
            /*===| Chassis_Speed |===*/
            ui_default_Dynamic_Chassis_1->number = UI_State.Chassis_Motor1_Speed * 100;
            ui_default_Dynamic_Chassis_2->number = UI_State.Chassis_Motor2_Speed * 100;
            ui_default_Dynamic_Chassis_3->number = UI_State.Chassis_Motor3_Speed * 100;
            ui_default_Dynamic_Chassis_4->number = UI_State.Chassis_Motor4_Speed * 100;
            
            /*===| SuperCap |===*/
            ui_default_Dynamic_SuperCap->start_angle = 129 - UI_State.SuperCap_K * 79;
            if(UI_State.SuperCap_K > 0.3f)     ui_default_Dynamic_SuperCap->color = 6;
            else                                    ui_default_Dynamic_SuperCap->color = 5;
            
            if(UI_State.SuperCap_IF_Enable == 0)    ui_default_Dynamic_SuperCap->color = 4;
            
            /*===| Speed_Level |===*/
            ui_default_Dynamic_Speed->start_x = 1572;
            ui_default_Dynamic_Speed->end_x = 1572 + (UI_State.Chassis_Speed_Level / 5.0f) * 138;


            /*===| 更新UI |===*/     
            UI_Update();
        }
        
    }	
}
