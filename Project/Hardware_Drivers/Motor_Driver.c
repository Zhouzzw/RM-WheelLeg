//============================================================================================================================================================================
//                                              Motor_Driver[电机驱动]
//      包含了一些电机驱动相关的封装好的函数
//
//      大疆电机发送数据 Motor_DJI_SendCurrent(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, int16_t ID1_Currnet, int16_t ID2_Currnet, int16_t ID3_Currnet, int16_t ID4_Currnet)
//      大疆电机解析反馈数据 Motor_DJI_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_StructTypeDef)
//      达妙电机相关指令 Motor_DM_CMD_Enable/Disable/ClearErr/SetZero/MIT/Position等
//============================================================================================================================================================================

#include "Motor_Driver.h"
#include "CRC8_CRC16.h"

/*===| 电机数据结构体 |===*/
Motor_Data_StructTypeDef Motor_Data_Struct[10];


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
void Motor_DJI_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_StructTypeDef)
{
    /*===| 记录原始值 |===*/
	Motor_Data_StructTypeDef->Encoder       = 	(int)(Data[0] * 256 + Data[1]);
	Motor_Data_StructTypeDef->SpeedRPM      = 	(int)(Data[2] * 256 + Data[3]);
	Motor_Data_StructTypeDef->Torque        = 	(int)(Data[4] * 256 + Data[5]);
    Motor_Data_StructTypeDef->Temperature   = 	(int)(Data[6]);
    
    /*===| 得到总角度值 |===*/
    if      (Motor_Data_StructTypeDef->Encoder - Motor_Data_StructTypeDef->Encoder_Last >  4096)    Motor_Data_StructTypeDef->Round--;
    else if (Motor_Data_StructTypeDef->Encoder - Motor_Data_StructTypeDef->Encoder_Last < -4096)    Motor_Data_StructTypeDef->Round++;
    Motor_Data_StructTypeDef->Total_Angle = 360.0f * (Motor_Data_StructTypeDef->Round + Motor_Data_StructTypeDef->Encoder / 8092.0f);
    
    /*===| 记录编码器值 |===*/
    Motor_Data_StructTypeDef->Encoder_Last = Motor_Data_StructTypeDef->Encoder;
}
 

/**
* @brief 达妙电机使能
 */
void Motor_DM_CMD_Enable(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID)
{
    uint8_t DM_Enable_Data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC};
    CAN_Send_Data(hfdcan, CAN_ID, DM_Enable_Data);
}

/**
* @brief 达妙电机失能
 */
void Motor_DM_CMD_Disable(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID)
{
    uint8_t DM_Disable_Data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD};
    CAN_Send_Data(hfdcan, CAN_ID, DM_Disable_Data);
}

/**
* @brief 达妙电机设置零点
 */
void Motor_DM_CMD_SetZero(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID)
{
    uint8_t DM_SetZero_Data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE};
    CAN_Send_Data(hfdcan, CAN_ID, DM_SetZero_Data);
}

/**
* @brief 达妙电机清除错误
 */
void Motor_DM_CMD_ClearErr(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID)
{
    uint8_t DM_ClearErr_Data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB};
    CAN_Send_Data(hfdcan, CAN_ID, DM_ClearErr_Data);
}

/**
 * @brief 达妙电机MIT控制
 *
 * @param 位置，速度，Kp, Ki，力矩
 */
uint8_t DM_Send_Data[8];
void Motor_DM_CMD_MIT(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float _pos, float _vel,float _KP, float _KD, float _torq)
{
    uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;
    
    pos_tmp = DM_float_to_uint(_pos, P_MIN, P_MAX, 16);
    vel_tmp = DM_float_to_uint(_vel, V_MIN, V_MAX, 12);
    kp_tmp = DM_float_to_uint(_KP, KP_MIN, KP_MAX, 12);
    kd_tmp = DM_float_to_uint(_KD, KD_MIN, KD_MAX, 12);
    tor_tmp = DM_float_to_uint(_torq, T_MIN, T_MAX, 12);

    DM_Send_Data[0] = (pos_tmp >> 8);
    DM_Send_Data[1] = pos_tmp;
    DM_Send_Data[2] = (vel_tmp >> 4);
    DM_Send_Data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
    DM_Send_Data[4] = kp_tmp;
    DM_Send_Data[5] = (kd_tmp >> 4);
    DM_Send_Data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
    DM_Send_Data[7] = tor_tmp;
    
    CAN_Send_Data(hfdcan, CAN_ID, DM_Send_Data);
}

/**
 * @brief 达妙电机位置控制
 *
 * @param 位置，速度
 */
void Motor_DM_CMD_Position(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float Position, float Speed)
{
    float Position_Speed_Buf[2] = {Position, Speed};
    
    CAN_Send_Data(hfdcan, CAN_ID, (uint8_t *)Position_Speed_Buf);
}

/**
 * @brief 达妙电机速度控制
 *
 * @param 速度
 */
void Motor_DM_CMD_Speed(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, float Speed)
{
    float Speed_Buf[1] = {Speed};
    
    CAN_Send_Data(hfdcan, CAN_ID, (uint8_t *)Speed_Buf);
}

float DM_uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

int DM_float_to_uint(float x, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}

/**
* @brief 达妙电机反馈数据
 *
 * @param 电调发送的数组
 * @param 要存储到的电机数据结构体
 */
void Motor_DM_Storage_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_StructTypeDef)
{
    /*===| 记录原始值 |===*/
    int p_int=(Data[1]<<8)|Data[2];
    int v_int=(Data[3]<<4)|(Data[4]>>4);
    int t_int=((Data[4]&0xF)<<8)|Data[5];
    Motor_Data_StructTypeDef->Total_Angle = DM_uint_to_float(p_int, P_MIN, P_MAX, 16);// (-12.5,12.5)
    Motor_Data_StructTypeDef->SpeedRPM = DM_uint_to_float(v_int, V_MIN, V_MAX, 12);// (-45.0,45.0)
    Motor_Data_StructTypeDef->Torque = DM_uint_to_float(t_int, T_MIN, T_MAX, 12); //(-18.0,18.0)
}

#define SATURATE(_IN, _MIN, _MAX) {\
 if (_IN < _MIN)\
 _IN = _MIN;\
 else if (_IN > _MAX)\
 _IN = _MAX;\
 } 

void Unitree_6010_Send_Control_Data(UART_HandleTypeDef *huart, uint8_t ID, float Position, float Speed, float Torque, float Kp, float Kd)
{
    SATURATE(Kp,  0.0f,   25.599f);
    SATURATE(Kd,  0.0f,   25.599f);
    SATURATE(Torque, -127.99f,  127.99f);
    SATURATE(Speed, -804.00f,  804.00f);
    SATURATE(Position, -411774.0f,  411774.0f);

    
    Unitree_6010_Control_StructTypedef Data = {0};
    Data.HEAD = 0xEEFE;
    Data.STATUS = 1;
    Data.None = 0;
    Data.Touque = Torque*256;
    Data.Speed = Speed / 6.2832f * 256;
    Data.Position = Position / 6.2832f * 32768;
    Data.ID = ID;
    Data.Kp = Kp / 25.6f * 32768;
    Data.Kd = Kd / 25.6f * 32768;
    
    Data.CRC16 = crc_ccitt(0, (uint8_t *)&Data, sizeof(Unitree_6010_Control_StructTypedef) - 2);

    HAL_UART_Transmit(huart, (uint8_t *)&Data, sizeof(Unitree_6010_Control_StructTypedef), 1000);
}


