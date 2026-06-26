#include "Motor_Unitree_Driver.h"


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

void Unitree_6010_Receive_Data(uint8_t *Data, Motor_Data_StructTypeDef *Motor_Data_Struct)
{
    /*===| 协议解包 |===*/
    Unitree_6010_Receive_StructTypedef Unitree_6010_Receive_Struct;
    memcpy(&Unitree_6010_Receive_Struct, Data, sizeof(Unitree_6010_Receive_StructTypedef));
    
    /*===| 转移数据 |===*/
    Motor_Data_Struct->Total_Angle = Unitree_6010_Receive_Struct.Position / 32768.0f * 360.0f;
    Motor_Data_Struct->Speed_RPM    = Unitree_6010_Receive_Struct.Speed / 256.0f * 60;
    Motor_Data_Struct->Torque      = Unitree_6010_Receive_Struct.Touque / 256.0f;
    Motor_Data_Struct->Temperature = Unitree_6010_Receive_Struct.Tempture;
}


uint8_t MF9025_Data_Send[8];
void MF9025_Send_Data(FDCAN_HandleTypeDef* hfdcan,uint16_t ID,float motor1_torque,float motor2_torque,float motor3_torque,float motor4_torque)
{
	int16_t send_buf[4] = {0};
	
	send_buf[0] = motor1_torque * 62.5;
	send_buf[1] = motor2_torque * 62.5;
	send_buf[2] = motor3_torque * 62.5;
	send_buf[3] = motor4_torque * 62.5;
	
	MF9025_Data_Send[0] = send_buf[0]&0xFF;
	MF9025_Data_Send[1] = send_buf[0] >> 8;
	MF9025_Data_Send[2] = send_buf[1]&0xFF;
	MF9025_Data_Send[3] = send_buf[1] >> 8;	
	MF9025_Data_Send[4] = send_buf[2]&0xFF;
	MF9025_Data_Send[5] = send_buf[2] >> 8;
	MF9025_Data_Send[6] = send_buf[3]&0xFF;
	MF9025_Data_Send[7] = send_buf[3] >> 8;
	
	CAN_Send_Data(hfdcan ,ID ,MF9025_Data_Send);
}


void MF9025_Receive_Data(MF9025_Receive_Data_StructTypedef *MF_Motor_Data_Struct,uint8_t *data)
{
	MF_Motor_Data_Struct->Temperature = data[1];
	MF_Motor_Data_Struct->Torque_Current = 	(((data)[3] << 8 ) | (data)[2] )*0.021973997f;
	MF_Motor_Data_Struct->Speed_rad_Second = (((data)[5] << 8 )| (data)[4]) * 0.017453f;
	MF_Motor_Data_Struct->Position = (((data)[7] << 8 )| (data)[6]) * 0.021973997f;
}

