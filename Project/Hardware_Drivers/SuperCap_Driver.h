#ifndef __SuperCap_Driver__
#define __SuperCap_Driver__

#include "main.h"


#define CAP_CAN_RX_ID 0x30

/*===| 电机控制结构体定义 |===*/
typedef __PACKED_STRUCT
{
    uint8_t Enable_Flag;
    uint8_t Power_Buff_Energy;
    uint8_t Max_Referee_P;
} SuperCap_Control_Data_StructTypedef;

/*===| 电机反馈结构体定义 |===*/
typedef __PACKED_STRUCT
{
    uint8_t State;                       //超电开关状态
    uint16_t SuperCap_V_x100;            //电容组电压x100
    uint16_t Referee_P_x10;              //裁判系统功率x10
    uint16_t Chassis_P_x10;              //底盘功率x10
    uint8_t None; 
} SuperCap_FeedBack_Data_StructTypedef;


void SuperCap_Control(uint8_t IF_Enable);

void SuperCap_Send_Control_Data(void);

void SuperCap_Unpack_Data(uint8_t *Data);
  

#endif
