#include "Vision_Data_Center.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "usbd_conf.h"
#include "cmsis_os.h"
//#include "referee.h"
#include "CRC8_CRC16.h"
#include "Referee_Unpack.h"

ReceivedPacketVision_s ReceivedPacketVision;
Received_Navigation_StructTypeDef Received_Navigation;
ReceivedPacketScanStatus_s ReceivedScanStatus = {.stop_gimbal_scan = 0,.chassis_spin_vel = 0};
SendPacketVision_s SendPacketVision;

#define APP_TX_DATA_SIZE 2048
#define VISION_SEND_HEDER 0x5A
#define VISION_RECEIVE_HEADER 0xA5
#define NAVIGATION_RECEIVE_HEADER 0XA4
#define PRESET_SELF_COLOR 1    // 0-red 1-blue


// Variable Declarations
static uint8_t usb_tx_buf[APP_TX_DATA_SIZE];
static uint8_t usb_rx_buf[APP_RX_DATA_SIZE];

ReceivedPacketVision_s* GetReceivedPacketVisionPoint()
{
	return &ReceivedPacketVision;
}

Received_Navigation_StructTypeDef *GetReceivedPacketNavigation()
{
	return &Received_Navigation;
}

SendPacketVision_s *GetSendPacketVisionPoint()
{
	return &SendPacketVision;
}

ReceivedPacketScanStatus_s *GetReceivedPacketScanStatus()
{
	return &ReceivedScanStatus;
}

/**
 * @brief      用USB发送数据
 * @param[in]  len 发送数据的长度
 */
void usb_send_data(uint16_t len)
{
	USB_Send(usb_tx_buf, len);
//    HAL_UART_Transmit(&huart4, usb_tx_buf, len, 1000);
}

/**
 * @brief      为视觉 部分发送数据
 * @param      t 数据发送周期
 * @retval     None
 */
void usb_send_vision(uint8_t t)
{

    SendPacketVision.header = VISION_SEND_HEDER;
    uint8_t self_color;
    // 获取自身颜色
    if(Robo_State.robot_id > 0 && Robo_State.robot_id <12) self_color = 1;
    else if(Robo_State.robot_id > 100 && Robo_State.robot_id <112) self_color = 2;
    else   self_color = 0;      
    
    if (self_color == 1)                   // 处理裁判系统队伍判断异常的情况
    {
        SendPacketVision.detect_color = 1;
    }
    else if(self_color == 2)
    {
        SendPacketVision.detect_color = 0;
    }
    else SendPacketVision.detect_color = !PRESET_SELF_COLOR;
    
    append_CRC16_check_sum((uint8_t *)&SendPacketVision, sizeof(SendPacketVision));
    memcpy(usb_tx_buf, &SendPacketVision, sizeof(SendPacketVision_s));
    usb_send_data(sizeof(SendPacketVision_s));
}

/**
 * @brief      接收视觉数据
 * @param[in]  none
 * @retval     crc_ok
 */
void usb_receive_vision(uint8_t *usb_rx_buf)
{
	if(usb_rx_buf[0] == VISION_RECEIVE_HEADER)
	{
		if (verify_CRC16_check_sum((uint8_t *)usb_rx_buf, sizeof(ReceivedPacketVision_s)))
		{
			memcpy(&ReceivedPacketVision, usb_rx_buf, sizeof(ReceivedPacketVision_s));
		}
	}
}


