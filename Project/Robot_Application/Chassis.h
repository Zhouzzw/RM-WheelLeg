#ifndef __Chassis__
#define __Chassis__

#include "main.h"

/*===| 底盘电机PID参数 |===*/
#define Chassis_PID_Kp              5000.0f
#define Chassis_PID_Ki              500.0f
#define Chassis_PID_Kd              0.0f
#define Chassis_PID_I_Output_Max    1500.0f
#define Chassis_PID_Output_Max      20000.0f

#define Energy_PID_Kp								10.0f
#define Energy_PID_Ki								0.0f
#define Energy_PID_Kd								0.0f
#define Energy_PID_I_Output_Max						0.0f
#define Energy_PID_Output_Max						200.0f

/*===| 底盘运动状态枚举定义 |===*/
typedef enum 
{
    Chassis_OFF = 0,
    Chassis_STATIC = 1,
    Chassis_FOLLOW = 2,
    Chassis_F_SIDE = 3,
    Chassis_SPIN = 4,
    Chassis_DASH = 5
} Chassis_State_EnumTypedef;


/*===| 底盘数据结构体定义 |===*/
typedef struct
{
    /*===| 底盘运动状态 |===*/
    Chassis_State_EnumTypedef Chassis_State;
    
    float Chassis_Power_MAX;
    
    /*===| 底盘设置速度 |===*/
    float Chassis_Vx_Set;                   //底盘目标设定左右速度 m/s
    float Chassis_Vy_Set;                   //底盘目标设定前后速度 m/s
    float Chassis_Wz;                       //底盘目标设定旋转角速度   rad/s
    
    float Chassis_Vx;                       //底盘实际设定左右速度 m/s
    float Chassis_Vy;                       //底盘实际设定前后速度 m/s
    
    /*===| 里程计 |===*/
    float Odometer_X;
    float Odometer_Y;
    float Odometer_Angle;
    uint32_t Odometer_DWT_Count;
    
    /*===| Yaw与机械中值偏差角度 |===*/
    float Yaw_ErrAngle;
    
    /*===| 各个轮解算后的速度 |===*/
    float MotorID_SpeedSet[4];
    
    /*===| PID结构体 |===*/
    PID_Struct_TypeDef Energy_Buffer_PID;
    PID_Struct_TypeDef MotorID_PID_Struct[4];  
    
    /*===| 缓冲区能量百分比 |===*/
    float PowerBuffer_K;
} Chassis_Control_StructTypeDef;

/*===| 底盘控制数据结构体 |===*/
extern Chassis_Control_StructTypeDef Chassis_Control_Struct;

void Chassis_Speed_Addto(float Set, float* Chassis_Speed);

#endif
