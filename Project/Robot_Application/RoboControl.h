#ifndef __RoboControl__
#define __RoboControl__

#include "main.h"
#include "Chassis.h"
#include "Shoot.h"
#include "Gimbal.h"

#define RoboWz_PID_Kp					0.15f
#define RoboWz_PID_Ki					0.0f
#define RoboWz_PID_Kd					0.0f
#define RoboWz_PID_I_Output_MAX			0.0f
#define RoboWz_PID_Output_MAX			5.0f

typedef enum
{
    none = 0,
	Customer,
	Joystick,
	KeyboardMouse,
} Robo_Controler_EnumTypedef;

/*===| 机器人整体状态数据结构体定义 |===*/
typedef struct
{
    uint8_t Robo_Enable;                    //是否启动机器人
	float Smooth_Start_K;                   //平滑启动比例
	Robo_Controler_EnumTypedef Controler;   //控制方式
	uint32_t Remote_DWT;					//遥控器计时器
	uint8_t If_SettingZero;
	
    float Robo_Target_Vx;                   //相对云台的前后速度
    float Robo_Target_Vy;                   //相对云台的左右速度
    float Robo_Target_Wz;                   //相对云台的旋转速度
    float Robo_Target_Gimbal_Yaw;           //云台目标Yaw值
    float Robo_Target_Gimbal_Pitch;         //云台目标Pitch值
    
    float Robo_Target_Vx_Last;              //上一次相对云台的前后速度
    float Robo_Target_Vy_Last;              //上一次相对云台的左右速度
    float Robo_Target_Wz_Last;              //上一次相对云台的旋转速度
    float Robo_Target_Gimbal_Yaw_Last;      //上一次云台目标Yaw值
    float Robo_Target_Gimbal_Pitch_Last;    //上一次云台目标Pitch值

    float Yaw_Err;                          //云台相对底盘的角度差值[-180 ~ +180]
	float Target_Direction;
	float Target_Direction_Err;
	
	Gimbal_State_EnumTypedef Gimbal_State;  //云台状态
    
    Shoot_State_EnumTypedef Shoot_State;    //发射机构状态	
	
	uint8_t Chassis_Speed_Level;            //底盘速度等级1~5
	Chassis_State_EnumTypedef Chassis_State;//底盘运动状态
    uint8_t SPIN_Direction_Flag;            //小陀螺旋转方向标志位
    uint8_t Chassis_Follow_45_Direction_Flag;//底盘跟随云台偏移方向标志位
	
	uint8_t SuperCap_State;                 //超电放电开关
    float SuperCap_V;                       //超电电压
    float Referee_P;                        //裁判系统Chassis功率
    float Chassis_P;                        //底盘功率
	
	float Robo_Target_Chassis_Yaw;			  //相对云台的目标Yaw值
	float Robo_Target_Chassis_Yaw_Last;		  //上一次相对云台目标Yaw值
	
	float Leg_Length;
	float Leg_Angle;
		
    uint8_t Power_Limit_Flag;				//功率限制标志位
    
    float Yaw_Err_Init;
		
    uint8_t Refresh_UI_Flag;                //刷新UI标志位 
	
	uint8_t Chassis_Static_Flag;
	
	float Yaw;
	float Pitch;
		
	float Robo_Target_Yaw;
    float Robo_Target_Pitch1; 
    float Robo_Target_Pitch2;
    float Robo_Target_J4;
    float Robo_Target_J5;
    float Robo_Target_J6;
    float Robo_Target_J7;
		
	float Robo_Target_Clap;
		
} RoboControl_StructTypeDef;

/*===| 视觉状态参数枚举 |===*/
typedef enum
{
	PC_State_Off = 0,
    PC_State_Aimming ,
	PC_State_Navigation,
} PC_State_EnumTypedef;

typedef struct 
{
	float Vision_Pitch;
	float Vision_Yaw;
	float Navigation_Vx;
	float Navigation_Vy;
	
	uint8_t Enemy_Appear_State;
	uint8_t Enemy_Appear_State_Last;
	
	PC_State_EnumTypedef PC_State;
	
}PcControl_StructTypeDef;

/*===| 机器人整体状态数据结构体 |===*/
extern RoboControl_StructTypeDef RoboControl_Struct;
extern PcControl_StructTypeDef	 PC_Control_Struct;

/*===| 输入参数滤波 |===*/
void Control_Filter(void);

/*===| 根据底盘运动状态得到Wz |===*/
void Get_Chassis_Wz(void);

/*===| 遥控器控制 |===*/
void RemoteControl_Float(void);
void RemoteControl_Bool(void);

/*===| 键盘控制 |===*/
void KeyControl_Float(void);
void KeyControl_Bool(void);

/*===| 停止机器人 |===*/
void Robo_Stop(void);

/*===| 重启机器人 |===*/
void Robo_Restart(void);

/*===| 视觉测试 |===*/
void PC_Test();

void INS_Shortage(uint8_t *Robo_Data,RoboControl_StructTypeDef *RoboControl_Struct);
void Vision_Shortage(uint8_t *Vision_Data,PcControl_StructTypeDef *PC_Control_Struct);
void Navigation_Shortage(uint8_t *Vision_Data,PcControl_StructTypeDef *PC_Control_Struct);

#endif