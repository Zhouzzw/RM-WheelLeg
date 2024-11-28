#ifndef UI_TASK_H
#define UI_TASK_H

#include "main.h"
#include "ui_interface.h"
#include "ui.h"
#include "shoot.h"
#include "Chassis.h"

typedef struct 
{
    int16_t Yaw;
    int16_t YawErr;
    int8_t Pitch;
    
    Shoot_State_EnumTypedef Shoot_State;
    uint8_t Fric1_Speed;
    uint8_t Fric2_Speed;
    uint8_t Fric3_Speed;
    int16_t Bullet;
    
    Chassis_State_EnumTypedef Chassis_State;
    float Chassis_Motor1_Speed;
    float Chassis_Motor2_Speed;
    float Chassis_Motor3_Speed;
    float Chassis_Motor4_Speed;
    
    uint8_t SuperCap_IF_Enable;
    float SuperCap_K;
    
    uint8_t Chassis_Speed_Level;
} UI_State_StructTypedef;

extern UI_State_StructTypedef UI_State;

void UI_Task(void const * argument);


#endif
