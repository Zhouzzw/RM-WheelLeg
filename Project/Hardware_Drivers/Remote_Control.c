//=====================================================================================
//                    Remote_Control[遥控器数据解包]
//      解析遥控器的数据，得到遥控器各个通道状态和键鼠信息，供RoboControl使用
//=====================================================================================

#include "Remote_Control.h"
#include "RoboControl.h"

/*===| 遥控器数据结构体 |===*/
remote_rawdata_t remote_rawdata;
Remote_StructTypeDef Remote;
Remote_StructTypeDef Remote_Last;

/**
 * @brief 遥控串口接收中断回调函数
 *
 * @param 串口收到的数组
 */
void Remote_Rx_CallBack(uint8_t *DataBuff)
{
    if(verify_CRC16_check_sum(DataBuff, 21) == TRUE)
    {
        Remote.If_Remote_Connect = 1;
        Remote.Remote_GetData_Ticker = 0;
        
        memcpy(&remote_rawdata, DataBuff, sizeof(remote_rawdata));

        memcpy(&Remote_Last, &Remote, sizeof(Remote_StructTypeDef));
    
        Remote.Right_X  = (remote_rawdata.ch_0 - Remote_CH_VALUE_OFFSET) / 660.0f;
        Remote.Right_Y  = (remote_rawdata.ch_1 - Remote_CH_VALUE_OFFSET) / 660.0f;
        Remote.Left_Y   = (remote_rawdata.ch_2 - Remote_CH_VALUE_OFFSET) / 660.0f;
        Remote.Left_X   = (remote_rawdata.ch_3 - Remote_CH_VALUE_OFFSET) / 660.0f;
        Remote.Mode     = remote_rawdata.mode_sw;
        Remote.Pause    = remote_rawdata.pause;
        Remote.Custom_L = remote_rawdata.fn_1;
        Remote.Custom_R = remote_rawdata.fn_2;
        Remote.Wheel    = (remote_rawdata.wheel - Remote_CH_VALUE_OFFSET) / 660.0f;
        Remote.Trigger  = remote_rawdata.trigger;
        
        Remote.Mouse_Vx = (remote_rawdata.mouse_x) / 32.0f;
        Remote.Mouse_Vy = (remote_rawdata.mouse_y) / 32.0f;
        Remote.Mouse_Vz = (remote_rawdata.mouse_z) / 32.0f;
        
        Remote.Mouse_L = remote_rawdata.mouse_left;
        Remote.Mouse_M = remote_rawdata.mouse_middle;
        Remote.Mouse_R = remote_rawdata.mouse_right;
        
        Remote.Keyboard_W       = !!(remote_rawdata.key1 & 0x01);
        Remote.Keyboard_S       = !!(remote_rawdata.key1 & 0x02);
        Remote.Keyboard_A       = !!(remote_rawdata.key1 & 0x04);
        Remote.Keyboard_D       = !!(remote_rawdata.key1 & 0x08);
        Remote.Keyboard_Shift   = !!(remote_rawdata.key1 & 0x10);
        Remote.Keyboard_Ctrl    = !!(remote_rawdata.key1 & 0x20);
        Remote.Keyboard_Q       = !!(remote_rawdata.key1 & 0x40);
        Remote.Keyboard_E       = !!(remote_rawdata.key1 & 0x80);
        Remote.Keyboard_R       = !!(remote_rawdata.key2 & 0x01);
        Remote.Keyboard_F       = !!(remote_rawdata.key2 & 0x02);
        Remote.Keyboard_G       = !!(remote_rawdata.key2 & 0x04);
        Remote.Keyboard_Z       = !!(remote_rawdata.key2 & 0x08);
        Remote.Keyboard_X       = !!(remote_rawdata.key2 & 0x10);
        Remote.Keyboard_C       = !!(remote_rawdata.key2 & 0x20);
        Remote.Keyboard_V       = !!(remote_rawdata.key2 & 0x40);
        Remote.Keyboard_B       = !!(remote_rawdata.key2 & 0x80);

        Remote.If_Remote_Data_New = 1;
    }
}

