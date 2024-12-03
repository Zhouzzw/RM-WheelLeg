#ifndef __Remote_Control__
#define __Remote_Control__

#include "main.h"
#include "Referee_Unpack.h"

/** 《遥控器信息图》
 *	-1.0f ┌───┐ 1.0f
 *        │Sid│
 *        └───┘
 * 			┌──────────────────────────────────────────┐
 * 			│ ┌───┐1  1.0f                1.0f  ┌───┐1 │
 * 			│ │S 1│3   Λ                    Λ   │S 2│3 │
 * 			│ └───┘2   │                    │   └───┘2 │
 * 			│          │                    │          │
 * 			│ CH2<─────┼─────>1.0fCH0<─────┼─────>1.0f │
 * 			│          │                    │          │
 * 			│          │                    │          │
 * 			│          V                    V          │
 * 			│         CH3                  CH1         │
 * 			└──────────────────────────────────────────┘
 */

/*===| 遥控器相关定义 |===*/
#define SW_Up 1
#define SW_Mid 3
#define SW_Down 2

#define KeyShift_Single_Press  Remote_Control_Struct.Keyboard_Shift == 1 && Remote_Control_Last_Struct.Keyboard_Shift == 0
#define KeyCtrl_Single_Press   Remote_Control_Struct.Keyboard_Ctrl  == 1 && Remote_Control_Last_Struct.Keyboard_Ctrl  == 0
#define KeyQ_Single_Press      Remote_Control_Struct.Keyboard_Q == 1    && Remote_Control_Last_Struct.Keyboard_Q == 0
#define KeyE_Single_Press      Remote_Control_Struct.Keyboard_E == 1    && Remote_Control_Last_Struct.Keyboard_E == 0
#define KeyR_Single_Press      Remote_Control_Struct.Keyboard_R == 1    && Remote_Control_Last_Struct.Keyboard_R == 0
#define KeyF_Single_Press      Remote_Control_Struct.Keyboard_F == 1    && Remote_Control_Last_Struct.Keyboard_F == 0
#define KeyG_Single_Press      Remote_Control_Struct.Keyboard_G == 1    && Remote_Control_Last_Struct.Keyboard_G == 0
#define KeyZ_Single_Press      Remote_Control_Struct.Keyboard_Z == 1    && Remote_Control_Last_Struct.Keyboard_Z == 0
#define KeyX_Single_Press      Remote_Control_Struct.Keyboard_X == 1    && Remote_Control_Last_Struct.Keyboard_X == 0
#define KeyC_Single_Press      Remote_Control_Struct.Keyboard_C == 1    && Remote_Control_Last_Struct.Keyboard_C == 0
#define KeyV_Single_Press      Remote_Control_Struct.Keyboard_V == 1    && Remote_Control_Last_Struct.Keyboard_V == 0
#define KeyB_Single_Press      Remote_Control_Struct.Keyboard_B == 1    && Remote_Control_Last_Struct.Keyboard_B == 0
#define MouseL_Single_Press    Remote_Control_Struct.Mouse_Press_L == 1 && Remote_Control_Last_Struct.Mouse_Press_L == 0
#define MouseR_Single_Press    Remote_Control_Struct.Mouse_Press_R == 1 && Remote_Control_Last_Struct.Mouse_Press_R == 0

#define KeyShift_Single_Release  Remote_Control_Struct.Keyboard_Shift == 0 && Remote_Control_Last_Struct.Keyboard_Shift == 1
#define KeyCtrl_Single_Release   Remote_Control_Struct.Keyboard_Ctrl  == 0 && Remote_Control_Last_Struct.Keyboard_Ctrl  == 1
#define KeyQ_Single_Release      Remote_Control_Struct.Keyboard_Q == 0    && Remote_Control_Last_Struct.Keyboard_Q == 1
#define KeyE_Single_Release      Remote_Control_Struct.Keyboard_E == 0    && Remote_Control_Last_Struct.Keyboard_E == 1
#define KeyR_Single_Release      Remote_Control_Struct.Keyboard_R == 0    && Remote_Control_Last_Struct.Keyboard_R == 1
#define KeyF_Single_Release      Remote_Control_Struct.Keyboard_F == 0    && Remote_Control_Last_Struct.Keyboard_F == 1
#define KeyG_Single_Release      Remote_Control_Struct.Keyboard_G == 0    && Remote_Control_Last_Struct.Keyboard_G == 1
#define KeyZ_Single_Release      Remote_Control_Struct.Keyboard_Z == 0    && Remote_Control_Last_Struct.Keyboard_Z == 1
#define KeyX_Single_Release      Remote_Control_Struct.Keyboard_X == 0    && Remote_Control_Last_Struct.Keyboard_X == 1
#define KeyC_Single_Release      Remote_Control_Struct.Keyboard_C == 0    && Remote_Control_Last_Struct.Keyboard_C == 1
#define KeyV_Single_Release      Remote_Control_Struct.Keyboard_V == 0    && Remote_Control_Last_Struct.Keyboard_V == 1
#define KeyB_Single_Release      Remote_Control_Struct.Keyboard_B == 0    && Remote_Control_Last_Struct.Keyboard_B == 1
#define MouseL_Single_Release    Remote_Control_Struct.Mouse_Press_L == 0 && Remote_Control_Last_Struct.Mouse_Press_L == 1
#define MouseR_Single_Release    Remote_Control_Struct.Mouse_Press_R == 0 && Remote_Control_Last_Struct.Mouse_Press_R == 1

#define KeyShift_Press  Remote_Control_Struct.Keyboard_Shift == 1
#define KeyCtrl_Press   Remote_Control_Struct.Keyboard_Ctrl  == 1
#define KeyQ_Press      Remote_Control_Struct.Keyboard_Q == 1    
#define KeyE_Press      Remote_Control_Struct.Keyboard_E == 1    
#define KeyR_Press      Remote_Control_Struct.Keyboard_R == 1    
#define KeyF_Press      Remote_Control_Struct.Keyboard_F == 1    
#define KeyG_Press      Remote_Control_Struct.Keyboard_G == 1    
#define KeyZ_Press      Remote_Control_Struct.Keyboard_Z == 1    
#define KeyX_Press      Remote_Control_Struct.Keyboard_X == 1    
#define KeyC_Press      Remote_Control_Struct.Keyboard_C == 1    
#define KeyV_Press      Remote_Control_Struct.Keyboard_V == 1    
#define KeyB_Press      Remote_Control_Struct.Keyboard_B == 1  
#define MouseL_Press    Remote_Control_Struct.Mouse_Press_L == 1 
#define MouseR_Press    Remote_Control_Struct.Mouse_Press_R == 1 

#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)

/*===| 遥控器数据结构体 |===*/
typedef __PACKED_STRUCT
{
    uint8_t If_Remote_Connect;
    uint8_t If_Remote_Data_New;
    uint16_t Remote_GetData_Ticker;
    
    uint8_t IF_USE_ImageLink;     //是否使用图传链路数据
    uint8_t If_ImageLink_Connect;
    uint8_t If_ImageLink_Data_New;
    uint16_t ImageLink_GetData_Ticker;
    
	float RC_Right_X;
	float RC_Right_Y;
	float RC_Left_X;
	float RC_Left_Y;
	float RC_Side;
	uint8_t S1;
	uint8_t S2;
    
    float Mouse_Speed_X;
    float Mouse_Speed_Y;
    float Mouse_Speed_Z;
    uint8_t Mouse_Press_L;
    uint8_t Mouse_Press_R;
    
    uint8_t Keyboard_W;
    uint8_t Keyboard_A;
    uint8_t Keyboard_S;
    uint8_t Keyboard_D;
    uint8_t Keyboard_Q;
    uint8_t Keyboard_E;
    uint8_t Keyboard_Shift;
    uint8_t Keyboard_Ctrl;
    uint8_t Keyboard_R;
    uint8_t Keyboard_F;
    uint8_t Keyboard_G;
    uint8_t Keyboard_Z;
    uint8_t Keyboard_X;
    uint8_t Keyboard_C;
    uint8_t Keyboard_V;
    uint8_t Keyboard_B;
} Remote_Control_StructTypeDef;

extern Remote_Control_StructTypeDef Remote_Control_Struct;
extern Remote_Control_StructTypeDef Remote_Control_Last_Struct;

void Remote_Control_GetData(uint8_t *DataBuff);

void Remote_Control_Rx_CallBack(uint8_t *Remote_Control_RxDataBuff);

void ImageLink_Rx_CallBack(Keyboard_Mouse_StructTypedef *Keyboard_Mouse_Struct);

void ImageLink_GetData(Keyboard_Mouse_StructTypedef *Keyboard_Mouse_Struct);

#endif
