#ifndef VISION_DATA_H
#define VISION_DATA_H

#include "main.h"


typedef __PACKED_STRUCT
{
    uint8_t header;
    uint8_t tracking;
    uint8_t id;         // 0-outpost 6-guard 7-base
    uint8_t armors_num; // 2-balance 3-outpost 4-normal
    uint8_t reserved;
    float x;
    float y;
    float z;
    float yaw;
    float vx;
    float vy;
    float vz;
    float v_yaw;
    float r1;
    float r2;
    float dz;
    uint16_t checksum;
} __attribute__((packed)) ReceivedPacketVision_s;

extern ReceivedPacketVision_s ReceivedPacketVision;

typedef struct
{
    uint8_t header;
    uint8_t stop_gimbal_scan;
    float chassis_spin_vel;
    uint16_t checksum;
} __attribute__((packed)) ReceivedPacketScanStatus_s;

typedef struct
{
    uint8_t header;
    uint8_t detect_color; // 0-red 1-blue
    uint8_t reset_tracker;
//    uint8_t fire : 1;
    uint8_t reserved;
    float roll;
    float pitch;
    float yaw;
    float aim_x;
    float aim_y;
    float aim_z;
    uint16_t checksum;
} __attribute__((packed)) SendPacketVision_s;

typedef struct
{
	uint8_t header;
	
	float linear_x ;
	float linear_y ;
	float linear_z ;
	float angular_x;
	float angular_y;
	float angular_z;
	
	uint16_t checksum;
} __attribute__((packed)) Received_Navigation_StructTypeDef;

ReceivedPacketVision_s* GetReceivedPacketVisionPoint();

Received_Navigation_StructTypeDef *GetReceivedPacketNavigation();

SendPacketVision_s *GetSendPacketVisionPoint();

ReceivedPacketScanStatus_s *GetReceivedPacketScanStatus();

void usb_send_data(uint16_t len);

void usb_send_vision(uint8_t t);

void usb_receive_vision(uint8_t *usb_rx_buf);


#endif
