//=====================================================================================
//                    Remote_Control[遥控器数据解包]
//      解析RM遥控器的数据，得到遥控器各个通道状态和键鼠信息，供RoboControl使用
//
//      遥控器以及键鼠数据  Remote_Control_Struct; Remote_Control_Last_Struct
//=====================================================================================

#include "Remote_Control.h"
#include "RoboControl.h"


/*===| 遥控器数据结构体 |===*/
Remote_Control_StructTypeDef Remote_Control_Struct;
Remote_Control_StructTypeDef Remote_Control_Last_Struct;

/**
 * @brief 遥控串口接收中断回调函数
 *
 * @param 串口收到的数组
 */
void Remote_Control_Rx_CallBack(uint8_t *Remote_Control_RxDataBuff)
{
    Remote_Control_Struct.If_Remote_Connect = 1;
    Remote_Control_Struct.Remote_GetData_Ticker = 0;
    
    Remote_Control_GetData(Remote_Control_RxDataBuff);
}

/**
 * @brief 图传链路接收键鼠数据中断回调函数
 *
 * @param 串口收到的数组
 */
void ImageLink_Rx_CallBack(Keyboard_Mouse_StructTypedef *Keyboard_Mouse_Struct)
{
    Remote_Control_Struct.If_ImageLink_Connect = 1;
    Remote_Control_Struct.ImageLink_GetData_Ticker = 0;
    
    ImageLink_GetData(Keyboard_Mouse_Struct);
}


/**
  * @brief 遥控器协议解析
  * @param SBUS原始数据
  * @param 遥控器解析后的数据结构体
  */
void Remote_Control_GetData(uint8_t *DataBuff)
{
    memcpy(&Remote_Control_Last_Struct, &Remote_Control_Struct, 31);
    
    Remote_Control_Struct.RC_Right_X   = (((DataBuff[0] | (DataBuff[1] << 8)) & 0x07ff) - RC_CH_VALUE_OFFSET) / 660.0f;        
    Remote_Control_Struct.RC_Right_Y   = ((((DataBuff[1] >> 3) | (DataBuff[2] << 5)) & 0x07ff) - RC_CH_VALUE_OFFSET) / 660.0f;
    Remote_Control_Struct.RC_Left_X    = ((((DataBuff[2] >> 6) | (DataBuff[3] << 2) | (DataBuff[4] << 10)) & 0x07ff) - RC_CH_VALUE_OFFSET) / 660.0f;
    Remote_Control_Struct.RC_Left_Y    = ((((DataBuff[4] >> 1) | (DataBuff[5] << 7)) & 0x07ff) - RC_CH_VALUE_OFFSET) / 660.0f;
    Remote_Control_Struct.RC_Side      = -((DataBuff[16] | (DataBuff[17] << 8)) - RC_CH_VALUE_OFFSET) / 660.0f;
    
    Remote_Control_Struct.S1 = ((DataBuff[5] >> 4) & 0x000C) >> 2;
    Remote_Control_Struct.S2 = ((DataBuff[5] >> 4) & 0x0003);

    int16_t Mouse_X = ((int16_t)DataBuff[6]) | ((int16_t)DataBuff[7] << 8);
    int16_t Mouse_Y = ((int16_t)DataBuff[8]) | ((int16_t)DataBuff[9] << 8);
    int16_t Mouse_Z = ((int16_t)DataBuff[10]) | ((int16_t)DataBuff[11] << 8); 
    
    if(Remote_Control_Struct.IF_USE_ImageLink == 0)
    {
        memcpy(&Remote_Control_Last_Struct.Mouse_Speed_X, &Remote_Control_Struct.Mouse_Speed_X, 30);
        
        Remote_Control_Struct.Mouse_Speed_X = Mouse_X / 32.0f;
        Remote_Control_Struct.Mouse_Speed_Y = Mouse_Y / 32.0f;
        Remote_Control_Struct.Mouse_Speed_Z = Mouse_Z / 32.0f;
        
        Remote_Control_Struct.Mouse_Press_L = DataBuff[12];
        Remote_Control_Struct.Mouse_Press_R = DataBuff[13];
        
        Remote_Control_Struct.Keyboard_W       = !!(DataBuff[14] & 0x01);
        Remote_Control_Struct.Keyboard_S       = !!(DataBuff[14] & 0x02);
        Remote_Control_Struct.Keyboard_A       = !!(DataBuff[14] & 0x04);
        Remote_Control_Struct.Keyboard_D       = !!(DataBuff[14] & 0x08);
        Remote_Control_Struct.Keyboard_Shift   = !!(DataBuff[14] & 0x10);
        Remote_Control_Struct.Keyboard_Ctrl    = !!(DataBuff[14] & 0x20);
        Remote_Control_Struct.Keyboard_Q       = !!(DataBuff[14] & 0x40);
        Remote_Control_Struct.Keyboard_E       = !!(DataBuff[14] & 0x80);
        Remote_Control_Struct.Keyboard_R       = !!(DataBuff[15] & 0x01);
        Remote_Control_Struct.Keyboard_F       = !!(DataBuff[15] & 0x02);
        Remote_Control_Struct.Keyboard_G       = !!(DataBuff[15] & 0x04);
        Remote_Control_Struct.Keyboard_Z       = !!(DataBuff[15] & 0x08);
        Remote_Control_Struct.Keyboard_X       = !!(DataBuff[15] & 0x10);
        Remote_Control_Struct.Keyboard_C       = !!(DataBuff[15] & 0x20);
        Remote_Control_Struct.Keyboard_V       = !!(DataBuff[15] & 0x40);
        Remote_Control_Struct.Keyboard_B       = !!(DataBuff[15] & 0x80);
        
        if(Remote_Control_Struct.RC_Right_X > -0.2f && Remote_Control_Struct.RC_Right_X  < 0.2f ) Remote_Control_Struct.RC_Right_X  = 0;
        if(Remote_Control_Struct.RC_Right_Y > -0.2f && Remote_Control_Struct.RC_Right_Y  < 0.2f ) Remote_Control_Struct.RC_Right_Y  = 0;
        if(Remote_Control_Struct.RC_Left_X  > -0.2f && Remote_Control_Struct.RC_Left_X   < 0.2f ) Remote_Control_Struct.RC_Left_X   = 0;
        if(Remote_Control_Struct.RC_Left_Y  > -0.2f && Remote_Control_Struct.RC_Left_Y   < 0.2f ) Remote_Control_Struct.RC_Left_Y   = 0;
        if(Remote_Control_Struct.RC_Side    > -0.2f && Remote_Control_Struct.RC_Side     < 0.2f ) Remote_Control_Struct.RC_Side     = 0;
    }                                                                                                                         

    Remote_Control_Struct.If_Remote_Data_New = 1;
} 

/**
  * @brief 图传链路协议解析
  * @param 图传链路原始数据
  * @param 遥控器解析后的数据结构体
  */
void ImageLink_GetData(Keyboard_Mouse_StructTypedef *Keyboard_Mouse_Struct)
{
    if(Remote_Control_Struct.IF_USE_ImageLink == 1)
    {
        memcpy(&Remote_Control_Last_Struct.Mouse_Speed_X, &Remote_Control_Struct.Mouse_Speed_X, 30);

        Remote_Control_Struct.Mouse_Speed_X = Keyboard_Mouse_Struct->Mouse_Speed_X / 32.0f;
        Remote_Control_Struct.Mouse_Speed_Y = Keyboard_Mouse_Struct->Mouse_Speed_Y / 32.0f;
        Remote_Control_Struct.Mouse_Speed_Z = Keyboard_Mouse_Struct->Mouse_Speed_Z / 32.0f;
        
        Remote_Control_Struct.Mouse_Press_L    = Keyboard_Mouse_Struct->Mouse_Left;
        Remote_Control_Struct.Mouse_Press_R    = Keyboard_Mouse_Struct->Mouse_Right;
        
        Remote_Control_Struct.Keyboard_W       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0001);
        Remote_Control_Struct.Keyboard_S       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0002);
        Remote_Control_Struct.Keyboard_A       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0004);
        Remote_Control_Struct.Keyboard_D       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0008);
        Remote_Control_Struct.Keyboard_Shift   = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0010);
        Remote_Control_Struct.Keyboard_Ctrl    = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0020);
        Remote_Control_Struct.Keyboard_Q       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0040);
        Remote_Control_Struct.Keyboard_E       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0080);
        Remote_Control_Struct.Keyboard_R       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0100);
        Remote_Control_Struct.Keyboard_F       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0200);
        Remote_Control_Struct.Keyboard_G       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0400);
        Remote_Control_Struct.Keyboard_Z       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x0800);
        Remote_Control_Struct.Keyboard_X       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x1000);
        Remote_Control_Struct.Keyboard_C       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x2000);
        Remote_Control_Struct.Keyboard_V       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x4000);
        Remote_Control_Struct.Keyboard_B       = !!(Keyboard_Mouse_Struct->KeyBoard & 0x8000);
    }
    
    Remote_Control_Struct.If_ImageLink_Data_New = 1;
} 
