//============================================================================================================================================================================
//                                              Motor_Driver[电机驱动]
//      包含了一些电机驱动相关的封装好的函数
//
//      大疆电机发送数据 Motor_DJI_SendCurrent(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, int16_t ID1_Currnet, int16_t ID2_Currnet, int16_t ID3_Currnet, int16_t ID4_Currnet)
//      大疆电机解析反馈数据 Motor_DJI_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_StructTypeDef)
//      达妙电机相关指令 Motor_DM_CMD_Enable/Disable/ClearErr/SetZero/MIT/Position等
//============================================================================================================================================================================
#include "Motor_DJI_Driver.h"

/**
 * @brief 大疆电机发送电流
 *
 * @param hfdcan
 * @param 发送报文ID
 * @param ID1电流大小
 * @param ID2电流大小
 * @param ID3电流大小
 * @param ID4电流大小
 */
void Motor_DJI_SendCurrent(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, int16_t ID1_Currnet, int16_t ID2_Currnet, int16_t ID3_Currnet, int16_t ID4_Currnet)
{
    uint8_t Motor_Tx_Data[8];
	Motor_Tx_Data[0] = ID1_Currnet >> 8;
	Motor_Tx_Data[1] = ID1_Currnet & 0xFF;
    Motor_Tx_Data[2] = ID2_Currnet >> 8;
	Motor_Tx_Data[3] = ID2_Currnet & 0xFF;
    Motor_Tx_Data[4] = ID3_Currnet >> 8;
	Motor_Tx_Data[5] = ID3_Currnet & 0xFF;
    Motor_Tx_Data[6] = ID4_Currnet >> 8;
	Motor_Tx_Data[7] = ID4_Currnet & 0xFF;
    
	CAN_Send_Data(hfdcan, CAN_ID, Motor_Tx_Data);
}

/**
 * @brief 存储大疆电机状态参数并计算得到总角度
 *
 * @param 电调发送的数组
 * @param 要存储到的电机数据结构体
 */
void Motor_DJI_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_Struct)
{
    /*===| 协议解包 |===*/
    Motor_Data_Struct->Encoder = (int16_t)(Data[0] << 8 | Data[1]);
    Motor_Data_Struct->Speed_RPM = (int16_t)(Data[2] << 8 | Data[3]);
    Motor_Data_Struct->Torque = (int16_t)(Data[4] << 8 |Data[5]);
    Motor_Data_Struct->Temperature = (int16_t)(Data[6]);
    
    Motor_Data_Struct->Angle = (Motor_Data_Struct->Encoder - 4096.0f) * 180.0f / 4096.0f;
    
    /*===| 得到总角度值 |===*/
    if      (Motor_Data_Struct->Encoder - Motor_Data_Struct->Encoder_Last >  4096)    Motor_Data_Struct->Round--;
    else if (Motor_Data_Struct->Encoder - Motor_Data_Struct->Encoder_Last < -4096)    Motor_Data_Struct->Round++;
    Motor_Data_Struct->Total_Angle = 360.0f * (Motor_Data_Struct->Round + Motor_Data_Struct->Encoder / 8192.0f);
    
    /*===| 记录编码器值 |===*/
    Motor_Data_Struct->Encoder_Last = Motor_Data_Struct->Encoder;
    Motor_Data_Struct->Angle_Last = Motor_Data_Struct->Angle;
	
	Motor_Data_Struct->Ticker = 0;
    Motor_Data_Struct->If_Online = 1; 
}