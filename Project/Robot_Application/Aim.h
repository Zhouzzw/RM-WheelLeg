#ifndef __Aim__
#define __Aim__

#include "main.h"


/*===| 发送给电脑的数据 |===*/
typedef __PACKED_STRUCT
{
  uint8_t cmd_ID;//命令码
  uint32_t time_stamp;//时间戳

  float yaw;    //°
  float pitch;  //°
  float roll;   //°

  float yaw_vel;    //m/s
  float pitch_vel;  //m/s
  float roll_vel;   //m/s

  float bullet_speed; //m/s
  float controller_delay; //s
  uint8_t manual_reset_count;
  uint8_t detect_color;//0 red 1 blue
  uint16_t Check_SUM;
	
  float operator_yaw_offset;
  float operator_pitch_offset;
	
} Aim_Send_StructTypedef;

/*===| 收到电脑的自瞄数据 |===*/
typedef __PACKED_STRUCT
{
    uint8_t cmd_ID; //命令码
    uint32_t time_stamp;
    uint8_t appear;
    uint8_t Shoot_Frequency;
    
    float pitch; //最佳控制yaw pitch
    float yaw;

    float target_yaw; //最佳发射时机yaw pitch
    float target_pitch;

    float enable_yaw_diff;
    float enable_pitch_diff; //计算当前yaw pitch 与本包次发送target_yaw target_pitch的差绝对值小于enable（角度）开火
    
    float v_yaw;
    float v_pitch;
    uint8_t detect_color; // 0 red 1 blue
} Aim_Receive_StructTypedef;

typedef struct
{
	uint8_t Aim_If_Allow_Shoot;
	
}Aim_DOuble_up_to_down_StructTypedef;
                                 
extern Aim_Receive_StructTypedef Aim_Receive_Struct;
extern Aim_Send_StructTypedef Aim_Send_Struct;
extern Aim_DOuble_up_to_down_StructTypedef Aim_DOuble_up_to_down_Struct;        

void Get_Average_Bullet_Speed(void);
void Aim_Get_Data_CallBack(uint8_t *Data);
void Aim_If_Allow_Shoot(void);

#endif
