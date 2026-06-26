#ifndef __Remote_Control__
#define __Remote_Control__

#include "main.h"
#include "Referee_Unpack.h"

/*===| 遥控器相关定义 |===*/
#define Remote_CH_VALUE_OFFSET      ((uint16_t)1024)
#define Remote_Mode_C 0
#define Remote_Mode_N 1
#define Remote_Mode_S 2
/*===| 遥控器数据结构体 |===*/
typedef __PACKED_STRUCT
{
    uint8_t If_Remote_Connect;
    uint8_t If_Remote_Data_New;
    uint16_t Remote_GetData_Ticker;

    float Right_X;
    float Right_Y;
    float Left_X;
    float Left_Y;
    float Wheel;
    uint8_t Mode;
    uint8_t Pause;
    uint8_t Custom_L;
    uint8_t Custom_R;
    uint8_t Trigger;
    
    float Mouse_Vx;
    float Mouse_Vy;
    float Mouse_Vz;
    uint8_t Mouse_L;
    uint8_t Mouse_M;
    uint8_t Mouse_R;
    
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
} Remote_StructTypeDef;

typedef __packed struct
{
    uint8_t sof_1;
    uint8_t sof_2;
    uint64_t ch_0:11;
    uint64_t ch_1:11;
    uint64_t ch_2:11;
    uint64_t ch_3:11;
    uint64_t mode_sw:2;
    uint64_t pause:1;
    uint64_t fn_1:1;
    uint64_t fn_2:1;
    uint64_t wheel:11;
    uint64_t trigger:1;

    int16_t mouse_x;
    int16_t mouse_y;
    int16_t mouse_z;
    uint8_t mouse_left:2;
    uint8_t mouse_right:2;
    uint8_t mouse_middle:2;
    uint8_t key1;
    uint8_t key2;
    uint16_t crc16;
}remote_rawdata_t;

extern Remote_StructTypeDef Remote;
extern Remote_StructTypeDef Remote_Last;

void Remote_Rx_CallBack(uint8_t *Remote_Control_RxDataBuff);

#define Remote_Press_Pause      (Remote.Pause           == 1)
#define Remote_Press_Custom_L   (Remote.Custom_L        == 1)
#define Remote_Press_Custom_R   (Remote.Custom_R        == 1)
#define Remote_Press_Trigger    (Remote.Trigger         == 1)
#define Remote_Press_W          (Remote.Keyboard_W      == 1)
#define Remote_Press_A          (Remote.Keyboard_A      == 1)
#define Remote_Press_S          (Remote.Keyboard_S      == 1)
#define Remote_Press_D          (Remote.Keyboard_D      == 1)
#define Remote_Press_Q          (Remote.Keyboard_Q      == 1)
#define Remote_Press_E          (Remote.Keyboard_E      == 1)
#define Remote_Press_Shift      (Remote.Keyboard_Shift  == 1)
#define Remote_Press_Ctrl       (Remote.Keyboard_Ctrl   == 1)
#define Remote_Press_R          (Remote.Keyboard_R      == 1)
#define Remote_Press_F          (Remote.Keyboard_F      == 1)
#define Remote_Press_G          (Remote.Keyboard_G      == 1)
#define Remote_Press_Z          (Remote.Keyboard_Z      == 1)
#define Remote_Press_X          (Remote.Keyboard_X      == 1)
#define Remote_Press_C          (Remote.Keyboard_C      == 1)
#define Remote_Press_V          (Remote.Keyboard_V      == 1)
#define Remote_Press_B          (Remote.Keyboard_B      == 1)
#define Remote_Press_Mouse_L     (Remote.Mouse_L         == 1)
#define Remote_Press_Mouse_M     (Remote.Mouse_M         == 1)
#define Remote_Press_Mouse_R     (Remote.Mouse_R         == 1)

#define Remote_Release_Pause      (Remote.Pause           == 0)
#define Remote_Release_Custom_L   (Remote.Custom_L        == 0)
#define Remote_Release_Custom_R   (Remote.Custom_R        == 0)
#define Remote_Release_Trigger    (Remote.Trigger         == 0)
#define Remote_Release_W          (Remote.Keyboard_W      == 0)
#define Remote_Release_A          (Remote.Keyboard_A      == 0)
#define Remote_Release_S          (Remote.Keyboard_S      == 0)
#define Remote_Release_D          (Remote.Keyboard_D      == 0)
#define Remote_Release_Q          (Remote.Keyboard_Q      == 0)
#define Remote_Release_E          (Remote.Keyboard_E      == 0)
#define Remote_Release_Shift      (Remote.Keyboard_Shift  == 0)
#define Remote_Release_Ctrl       (Remote.Keyboard_Ctrl   == 0)
#define Remote_Release_R          (Remote.Keyboard_R      == 0)
#define Remote_Release_F          (Remote.Keyboard_F      == 0)
#define Remote_Release_G          (Remote.Keyboard_G      == 0)
#define Remote_Release_Z          (Remote.Keyboard_Z      == 0)
#define Remote_Release_X          (Remote.Keyboard_X      == 0)
#define Remote_Release_C          (Remote.Keyboard_C      == 0)
#define Remote_Release_V          (Remote.Keyboard_V      == 0)
#define Remote_Release_B          (Remote.Keyboard_B      == 0)
#define Remote_Release_Mouse_L     (Remote.Mouse_L         == 0)
#define Remote_Release_Mouse_M     (Remote.Mouse_M         == 0)
#define Remote_Release_Mouse_R     (Remote.Mouse_R         == 0)

#define Remote_PressSingle_Pause      (Remote.Pause           == 1 && Remote_Last.Pause          == 0)
#define Remote_PressSingle_Custom_L   (Remote.Custom_L        == 1 && Remote_Last.Custom_L       == 0)
#define Remote_PressSingle_Custom_R   (Remote.Custom_R        == 1 && Remote_Last.Custom_R       == 0)
#define Remote_PressSingle_Trigger    (Remote.Trigger         == 1 && Remote_Last.Trigger        == 0)
#define Remote_PressSingle_W          (Remote.Keyboard_W      == 1 && Remote_Last.Keyboard_W     == 0)
#define Remote_PressSingle_A          (Remote.Keyboard_A      == 1 && Remote_Last.Keyboard_A     == 0)
#define Remote_PressSingle_S          (Remote.Keyboard_S      == 1 && Remote_Last.Keyboard_S     == 0)
#define Remote_PressSingle_D          (Remote.Keyboard_D      == 1 && Remote_Last.Keyboard_D     == 0)
#define Remote_PressSingle_Q          (Remote.Keyboard_Q      == 1 && Remote_Last.Keyboard_Q     == 0)
#define Remote_PressSingle_E          (Remote.Keyboard_E      == 1 && Remote_Last.Keyboard_E     == 0)
#define Remote_PressSingle_Shift      (Remote.Keyboard_Shift  == 1 && Remote_Last.Keyboard_Shift == 0)
#define Remote_PressSingle_Ctrl       (Remote.Keyboard_Ctrl   == 1 && Remote_Last.Keyboard_Ctrl  == 0)
#define Remote_PressSingle_R          (Remote.Keyboard_R      == 1 && Remote_Last.Keyboard_R     == 0)
#define Remote_PressSingle_F          (Remote.Keyboard_F      == 1 && Remote_Last.Keyboard_F     == 0)
#define Remote_PressSingle_G          (Remote.Keyboard_G      == 1 && Remote_Last.Keyboard_G     == 0)
#define Remote_PressSingle_Z          (Remote.Keyboard_Z      == 1 && Remote_Last.Keyboard_Z     == 0)
#define Remote_PressSingle_X          (Remote.Keyboard_X      == 1 && Remote_Last.Keyboard_X     == 0)
#define Remote_PressSingle_C          (Remote.Keyboard_C      == 1 && Remote_Last.Keyboard_C     == 0)
#define Remote_PressSingle_V          (Remote.Keyboard_V      == 1 && Remote_Last.Keyboard_V     == 0)
#define Remote_PressSingle_B          (Remote.Keyboard_B      == 1 && Remote_Last.Keyboard_B     == 0)
#define Remote_PressSingle_Mouse_L        (Remote.Mouse_L         == 1 && Remote_Last.Mouse_L        == 0)
#define Remote_PressSingle_Mouse_M        (Remote.Mouse_M         == 1 && Remote_Last.Mouse_M        == 0)
#define Remote_PressSingle_Mouse_R        (Remote.Mouse_R         == 1 && Remote_Last.Mouse_R        == 0)

#define Remote_ReleaseSingle_Pause      (Remote.Pause           == 0 && Remote_Last.Pause          == 1)
#define Remote_ReleaseSingle_Custom_L   (Remote.Custom_L        == 0 && Remote_Last.Custom_L       == 1)
#define Remote_ReleaseSingle_Custom_R   (Remote.Custom_R        == 0 && Remote_Last.Custom_R       == 1)
#define Remote_ReleaseSingle_Trigger    (Remote.Trigger         == 0 && Remote_Last.Trigger        == 1)
#define Remote_ReleaseSingle_W          (Remote.Keyboard_W      == 0 && Remote_Last.Keyboard_W     == 1)
#define Remote_ReleaseSingle_A          (Remote.Keyboard_A      == 0 && Remote_Last.Keyboard_A     == 1)
#define Remote_ReleaseSingle_S          (Remote.Keyboard_S      == 0 && Remote_Last.Keyboard_S     == 1)
#define Remote_ReleaseSingle_D          (Remote.Keyboard_D      == 0 && Remote_Last.Keyboard_D     == 1)
#define Remote_ReleaseSingle_Q          (Remote.Keyboard_Q      == 0 && Remote_Last.Keyboard_Q     == 1)
#define Remote_ReleaseSingle_E          (Remote.Keyboard_E      == 0 && Remote_Last.Keyboard_E     == 1)
#define Remote_ReleaseSingle_Shift      (Remote.Keyboard_Shift  == 0 && Remote_Last.Keyboard_Shift == 1)
#define Remote_ReleaseSingle_Ctrl       (Remote.Keyboard_Ctrl   == 0 && Remote_Last.Keyboard_Ctrl  == 1)
#define Remote_ReleaseSingle_R          (Remote.Keyboard_R      == 0 && Remote_Last.Keyboard_R     == 1)
#define Remote_ReleaseSingle_F          (Remote.Keyboard_F      == 0 && Remote_Last.Keyboard_F     == 1)
#define Remote_ReleaseSingle_G          (Remote.Keyboard_G      == 0 && Remote_Last.Keyboard_G     == 1)
#define Remote_ReleaseSingle_Z          (Remote.Keyboard_Z      == 0 && Remote_Last.Keyboard_Z     == 1)
#define Remote_ReleaseSingle_X          (Remote.Keyboard_X      == 0 && Remote_Last.Keyboard_X     == 1)
#define Remote_ReleaseSingle_C          (Remote.Keyboard_C      == 0 && Remote_Last.Keyboard_C     == 1)
#define Remote_ReleaseSingle_V          (Remote.Keyboard_V      == 0 && Remote_Last.Keyboard_V     == 1)
#define Remote_ReleaseSingle_B          (Remote.Keyboard_B      == 0 && Remote_Last.Keyboard_B     == 1)
#define Remote_ReleaseSingle_Mouse_L    (Remote.Mouse_L         == 0 && Remote_Last.Mouse_L        == 1)
#define Remote_ReleaseSingle_Mouse_M    (Remote.Mouse_M         == 0 && Remote_Last.Mouse_M        == 1)
#define Remote_ReleaseSingle_Mouse_R    (Remote.Mouse_R         == 0 && Remote_Last.Mouse_R        == 1)

#endif
