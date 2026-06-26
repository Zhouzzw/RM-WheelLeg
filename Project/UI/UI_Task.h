#ifndef UI_TASK_H
#define UI_TASK_H

#include "main.h"
#include "ui_interface.h"
#include "ui.h"
#include "shoot.h"
#include "Chassis.h"
#include "Gimbal.h"

typedef struct 
{
    int16_t Yaw;
    int16_t YawErr;
    int8_t Pitch;
    int16_t Bullet;
	
	uint8_t Fric1_Speed;
    uint8_t Fric2_Speed;
	
    Shoot_State_EnumTypedef Shoot_State;
	Gimbal_State_EnumTypedef Gimbal_State;
    Chassis_State_EnumTypedef Chassis_State;
    
    uint8_t SuperCap_IF_Enable;
    float SuperCap_K;
	float Left_Leg_Angle;
	float Right_Leg_Angle;
    
    uint8_t Chassis_Speed_Level;
	uint8_t Leg_Length;
	int8_t Volecity_Fdb;
	
} UI_State_StructTypedef;

extern UI_State_StructTypedef UI_State;

void UI_Task(void const * argument);
void String_Printf(char *String_Arr, const char *fmt,...);



#endif
