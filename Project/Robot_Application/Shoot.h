#ifndef __Shoot__
#define __Shoot__

#include "main.h"
#include "CAN_Driver.h"
#include "PID.h"

/*===| 发射电机PID参数 |===*/
#define Shoot_Fric_Speed_PID_Kp                 5000.0f
#define Shoot_Fric_Speed_PID_Ki                 10.0f
#define Shoot_Fric_Speed_PID_Kd                 0.0f
#define Shoot_Fric_Speed_PID_I_Output_Max       200.0f
#define Shoot_Fric_Speed_PID_Output_Max         13000.0f

#define Shoot_Trigger_Angle_PID_Kp               5.0f
#define Shoot_Trigger_Angle_PID_Ki               0.0f
#define Shoot_Trigger_Angle_PID_Kd               0.0f
#define Shoot_Trigger_Angle_PID_I_Output_Max     0.0f
#define Shoot_Trigger_Angle_PID_Output_Max       100.0f

#define Shoot_Trigger_Single_Speed_PID_Kp               1000.0f
#define Shoot_Trigger_Single_Speed_PID_Ki               5.0f
#define Shoot_Trigger_Single_Speed_PID_Kd               0.0f
#define Shoot_Trigger_Single_Speed_PID_I_Output_Max     3000.0f
#define Shoot_Trigger_Single_Speed_PID_Output_Max       15000.0f

#define Shoot_Trigger_Continue_Speed_PID_Kp               500.0f
#define Shoot_Trigger_Continue_Speed_PID_Ki               5.0f
#define Shoot_Trigger_Continue_Speed_PID_Kd               0.0f
#define Shoot_Trigger_Continue_Speed_PID_I_Output_Max     3000.0f
#define Shoot_Trigger_Continue_Speed_PID_Output_Max       15000.0f

/*===| 发射状态参数枚举 |===*/
typedef enum
{
    Shoot_State_Off = 0,
    Shoot_State_Ready = 1,
    Shoot_State_Continue = 2,
    Shoot_State_Unload = 3,
} Shoot_State_EnumTypedef;

/*===| 发射数据结构体定义 |===*/
typedef struct
{
    Shoot_State_EnumTypedef Shoot_State;
	uint8_t Heat_Limit;
    uint8_t Shoot_Frequency;
    float Cool_Down;
	
	uint32_t DWT_Count;
    
    /*===| 目标转速等 |===*/
    float Fric_First_Left_Target_Speed;
    float Fric_First_Right_Target_Speed;
    float Fric_First_Middle_Target_Speed;
    float Fric_First_Left_Set_Speed;
    float Fric_First_Right_Set_Speed;
    float Fric_First_Middle_Set_Speed;
    float Fric_Second_Left_Target_Speed;
    float Fric_Second_Right_Target_Speed;
    float Fric_Second_Middle_Target_Speed;
    float Fric_Second_Left_Set_Speed;
    float Fric_Second_Right_Set_Speed;
    float Fric_Second_Middle_Set_Speed;
    
    float Trigger_Target_Angle;
    
    
    /*===| PID结构体 |===*/
    PID_Struct_TypeDef Fric_First_Left_Speed_PID_Struct;          //Fric_PID结构体
    PID_Struct_TypeDef Fric_First_Right_Speed_PID_Struct;         //Fric_PID结构体
    PID_Struct_TypeDef Fric_First_Middle_Speed_PID_Struct;        //Fric_PID结构体
    PID_Struct_TypeDef Fric_Second_Left_Speed_PID_Struct;          //Fric_PID结构体
    PID_Struct_TypeDef Fric_Second_Right_Speed_PID_Struct;         //Fric_PID结构体
    PID_Struct_TypeDef Fric_Second_Middle_Speed_PID_Struct;        //Fric_PID结构体
    
    PID_Struct_TypeDef Trigger_Angle_PID_Struct;            //Trigger_Angle_PID结构体
    PID_Struct_TypeDef Trigger_Single_Speed_PID_Struct;     //单发状态Trigger_Speed_PID结构体
    
    
    float Trigger_Send_Current;
    
    uint8_t  Trigger_Block_Flag;      //卡弹标志位
    uint16_t Trigger_Block_Time;     //卡弹判断时间
    uint16_t Trigger_Release_Time;   //处理时间
    uint8_t  Fric_Block_Flag;      //卡弹标志位
    uint16_t Fric_Block_Time;     //卡弹判断时间
    uint16_t Fric_Release_Time;   //处理时间
    
} Shoot_Control_StructTypeDef;


/*===| 发送控制数据结构体 |===*/
extern Shoot_Control_StructTypeDef Shoot_Control_Struct;

void Shoot_Single(float Num);
void Shoot_Getout(float n);

void Trigger_Stuck_Handle(void);
void Fric_Stuck_Handle(void);

#endif
