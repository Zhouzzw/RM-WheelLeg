//============================================================================================================================================================================
//                                              Super_Driver[超电驱动]
//      包含了一些超电驱动相关的封装好的函数
//      队内超电定义控制CANID为0x30,无反馈
//      通过双板通信得到超电开关
//      在底盘communicate_task以100Hz循环发送控制信息[开关，功率限制，缓冲区能量]
//      未获取裁判系统数据时默认关断超电
//
//============================================================================================================================================================================
#include "SuperCap_Driver.h"
#include "RoboControl.h"
#include "Referee_Unpack.h"

/*===| 超电控制数据结构体 |===*/
SuperCap_Control_Data_StructTypedef SuperCap_Control_Data_Struct;
/*===| 超电反馈数据结构体 |===*/
SuperCap_FeedBack_Data_StructTypedef SuperCap_FeedBack_Data;

/**
 * @brief 循环发送超电控制帧
 */
void SuperCap_Send_Control_Data(void)
{
    /*===| 如果没有裁判系统数据，则强制关断输出 |===*/
    if(Robo_State.chassis_power_limit == 0) SuperCap_Control_Data_Struct.Enable_Flag = 0; 
    else SuperCap_Control_Data_Struct.Enable_Flag = RoboControl_Struct.SuperCap_State; 
    
    SuperCap_Control_Data_Struct.Max_Referee_P = Robo_State.chassis_power_limit;
    SuperCap_Control_Data_Struct.Power_Buff_Energy = Robo_PowerHeatData.buffer_energy;
    
    CAN_Send_Data(&SuperCap_CAN, SuperCap_Control_CAN_ID, (uint8_t *)&SuperCap_Control_Data_Struct);
}

/**
 * @brief 解析超电反馈帧
 */
void SuperCap_Unpack_Data(uint8_t *Data)
{
    memcpy((uint8_t *)&SuperCap_FeedBack_Data, Data, 8);
    RoboControl_Struct.SuperCap_V = SuperCap_FeedBack_Data.SuperCap_V_x100 / 100.0f;
    RoboControl_Struct.Referee_P  = SuperCap_FeedBack_Data.Referee_P_x10 / 10.0f;
    RoboControl_Struct.Chassis_P  = SuperCap_FeedBack_Data.Chassis_P_x10 / 10.0f;
}

