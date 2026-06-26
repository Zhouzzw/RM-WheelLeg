#ifndef __Communicate__
#define __Communicate__

#include "main.h"

typedef __packed struct
{
//    uint8_t Robo_Enable:2;
//    uint8_t Gimbal_State:2;
//    uint8_t Shoot_State:2;
//    uint8_t Null:2;
//    int16_t Pitch_Targetx100;
//    
//    float Yaw_Target;
//    float Volecity_Fdb;
        
    uint16_t Shoot_Qmax;
    uint16_t Shoot_Qnow;    
    
    uint16_t robot_id;
    uint16_t initial_speedx100;
    
} Double_Board_Down_to_UP_Referee_TypedefStruct;

typedef __packed struct
{
    uint8_t Recover_from_ground_Flag;
    
    int16_t Chassis_Odometer_Vxx100;
    int16_t Chassis_Odometer_Vyx100;
    
    uint8_t Cooling_value;
    uint16_t null2;
    
} Double_Board_Down_to_UP_TypedefStruct;

typedef __packed struct
{
	//4字节
    int16_t Yaw_Errx100;
    int16_t Gimbal_Yaw_TotalAnglex100;
    
	//12位
    uint8_t Aim_If_Allow_Shoot:1;
    uint8_t Shoot_State:3;	//off、ready、continue
    uint8_t Gimbal_State:2;	//normal、aim
	uint8_t Fric1_Speed:3;
	uint8_t Fric2_Speed:3; 
	
	//20位
	int8_t Gimbal_Pitch:6;
	int8_t operator_yaw_offsetx10:7;
	int8_t operator_pitch_offsetx10:7;
	
}Double_Board_Up_to_Down_TypedefStruct;

//typedef __packed struct
//{
//    int16_t Yaw_Errx100;
//    int16_t Gimbal_Yaw_TotalAnglex100;
//    
//    uint8_t Aim_If_Allow_Shoot:1;
//    uint8_t Shoot_Frequency:5;
//    uint8_t Shoot_State:4;
//    uint8_t Gimbal_State:4;
//    
//    int8_t Gimbal_Pitch:8;
//    uint8_t Fric1_Speed:5;
//    uint8_t Fric2_Speed:5;
//	
//}Double_Board_Up_to_Down_TypedefStruct;

typedef __packed struct
{
	int8_t Right_X;
	int8_t Right_Y;
	int8_t Left_X;
	int8_t Left_Y;
	int8_t Wheel;
	uint8_t If_Remote_Connect:1;
	uint8_t Mode:2;
	uint8_t Pause:1;
	uint8_t Custom_L:1;
	uint8_t Custom_R:1;
	uint8_t Trigger:1;
	uint8_t null1:1;
	uint8_t null2;
} Remote_Pack1_TypedefStruct;

typedef __packed struct
{
	int8_t  Mouse_Vx;
    int8_t  Mouse_Vy;
    int8_t  Mouse_Vz;
    uint8_t Mouse_L:1;
    uint8_t Mouse_M:1;
    uint8_t Mouse_R:1;
    uint8_t null1:5;
    uint8_t Keyboard_W:1;
    uint8_t Keyboard_A:1;
    uint8_t Keyboard_S:1;
    uint8_t Keyboard_D:1;
    uint8_t Keyboard_Q:1;
    uint8_t Keyboard_E:1;
	uint8_t Keyboard_Shift:1;
	uint8_t Keyboard_Ctrl:1;
    uint8_t Keyboard_R:1;
    uint8_t Keyboard_F:1;
    uint8_t Keyboard_G:1;
    uint8_t Keyboard_Z:1;
    uint8_t Keyboard_X:1;
    uint8_t Keyboard_C:1;
    uint8_t Keyboard_V:1;
    uint8_t Keyboard_B:1;
	uint16_t null2;
} Remote_Pack2_TypedefStruct;

void Chassis_Receive_attitude_Pack(uint8_t *Data);
void Chassis_Receive_Remote_Pack1(uint8_t *Data);
void Chassis_Receive_Remote_Pack2(uint8_t *Data);

void Chassis_Send_Referee_Unpack(void);
void Chassis_Send_Func_Data(void);

#endif
