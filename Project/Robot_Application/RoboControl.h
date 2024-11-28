#ifndef __RoboControl__
#define __RoboControl__

#include "main.h"
#include "Chassis.h"
#include "Shoot.h"
#include "Gimbal.h"
#include "Remote_Control.h"


/*===| 机器人整体状态数据结构体定义 |===*/
typedef struct
{
    uint8_t Robo_Enable;                    //是否启动机器人
    
    float Robo_Target_Vx;                   //相对云台的前后速度
    float Robo_Target_Vy;                   //相对云台的左右速度
    float Robo_Target_Wz;                   //相对云台的旋转速度
    float Robo_Target_Yaw;                  //云台目标Yaw值
    float Robo_Target_Pitch;                //云台目标Pitch值
    
    float Robo_Target_Vx_Last;              //上一次相对云台的前后速度
    float Robo_Target_Vy_Last;              //上一次相对云台的左右速度
    float Robo_Target_Wz_Last;              //上一次相对云台的旋转速度
    float Robo_Target_Yaw_Last;             //上一次云台目标Yaw值
    float Robo_Target_Pitch_Last;           //上一次云台目标Pitch值

    float Yaw_Err;                          //云台相对底盘的角度差值[-180 ~ +180]
    
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
    
    uint8_t Refresh_UI_Flag;                //刷新UI标志位    
} RoboControl_StructTypeDef;

/*===| 机器人整体状态数据结构体 |===*/
extern RoboControl_StructTypeDef RoboControl_Struct;


/*===| 输入参数滤波 |===*/
void Control_Filter(void);

/*===| 根据底盘运动状态得到Wz |===*/
void Get_Chassis_Wz(void);

/*===| 遥控器控制 |===*/
void RemoteControl(void);

/*===| 键盘控制 |===*/
void KeyControl(void);

/*===| 停止机器人 |===*/
void Robo_Stop(void);

/*===| 重启机器人 |===*/
void Robo_Restart(void);


#endif
