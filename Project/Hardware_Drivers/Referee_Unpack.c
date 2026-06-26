//=====================================================================================
//                    Referee_Unpack[裁判系统数据解包]
//        解析裁判系统的数据，得到状态数据
//
//        Game_State;              //比赛阶段
//        Game_Event;              //比赛事件
//        Robo_State;              //机器人状态
//        Robo_HP;                 //全场机器人血量
//        Robo_Position;           //机器人位置
//        Robo_Buff;               //机器人增益
//        Robo_HurtData;           //受伤数据                         
//        Robo_PowerHeatData;      //功率和热量情况
//        Robo_ShootData;          //发射机构数据
//        Robo_BulletRemain;       //剩余弹量
//=====================================================================================

#include "Referee_Unpack.h"
#include "CRC8_CRC16.h"
#include "Remote_Control.h"

Referee_Package_Header_StructTypedef    Referee_Package_Header;     //包头
Referee_PackData_StructTypedef          Referee_PackData;           //包体
    
Game_State_StructTypedef                Game_State;                 //比赛阶段
Game_Event_StructTypedef                Game_Event;                 //比赛事件
    
Robo_State_StructTypedef                Robo_State;                 //机器人状态
Robo_HP_StructTypedef                   Robo_HP;                    //全场机器人血量
Robo_Position_StructTypedef             Robo_Position;              //机器人位置
Robo_Buff_StructTypedef                 Robo_Buff;                  //机器人增益
Robo_HurtData_StructTypedef             Robo_HurtData;              //受伤数据
                                                                    
Robo_PowerHeatData_StructTypedef        Robo_PowerHeatData;         //功率和热量情况
Robo_ShootData_StructTypedef            Robo_ShootData;             //发射机构数据
Robo_BulletRemain_StructTypedef         Robo_BulletRemain;          //剩余弹量

Keyboard_Mouse_StructTypedef            Keyboard_Mouse_Struct;      //键鼠数据
Customer_Controller_StructTypedef       Customer_Controller_Struct; //自定义控制器数据

RF_Position_StructTypedef               RF_Position_Struct;         //对方机器人位置
RF_HP_StructTypedef                     RF_HP_Struct;               //对方机器人血量
RF_Bullet_StructTypedef                 RF_Bullet_Struct;           //对方机器人允许发弹量
RF_Coin_RFID_StructTypedef              RF_Coin_RFID_Struct;        //对方金币与占领状态
RF_Key_StructTypedef                    RF_Key_Struct;              //干扰波秘钥
//如有其他数据需求则需要根据裁判系统通信手册更新

/**
 * @brief 裁判系统数据初始化
 */
void Referee_Data_Init(void)
{
    memset(&Referee_Package_Header      , 0, sizeof(Referee_Package_Header_StructTypedef ));
    memset(&Referee_PackData            , 0, sizeof(Referee_PackData_StructTypedef       ));
    memset(&Game_State                  , 0, sizeof(Game_State_StructTypedef             ));
    memset(&Game_Event                  , 0, sizeof(Game_Event_StructTypedef             ));
    memset(&Robo_State                  , 0, sizeof(Robo_State_StructTypedef             ));
    memset(&Robo_HP                     , 0, sizeof(Robo_HP_StructTypedef                ));
    memset(&Robo_Position               , 0, sizeof(Robo_Position_StructTypedef          ));
    memset(&Robo_Buff                   , 0, sizeof(Robo_Buff_StructTypedef              ));
    memset(&Robo_HurtData               , 0, sizeof(Robo_HurtData_StructTypedef          ));
    memset(&Robo_PowerHeatData          , 0, sizeof(Robo_PowerHeatData_StructTypedef     ));
    memset(&Robo_ShootData              , 0, sizeof(Robo_ShootData_StructTypedef         ));
    memset(&Robo_BulletRemain           , 0, sizeof(Robo_BulletRemain_StructTypedef      ));
    memset(&Keyboard_Mouse_Struct       , 0, sizeof(Keyboard_Mouse_StructTypedef         ));
    memset(&Customer_Controller_Struct  , 0, sizeof(Customer_Controller_StructTypedef    ));
    memset(&RF_Position_Struct          , 0, sizeof(RF_Position_StructTypedef            ));
    memset(&RF_HP_Struct                , 0, sizeof(RF_HP_StructTypedef                  ));
    memset(&RF_Bullet_Struct            , 0, sizeof(RF_Bullet_StructTypedef              ));
    memset(&RF_Coin_RFID_Struct         , 0, sizeof(RF_Coin_RFID_StructTypedef           ));
    memset(&RF_Key_Struct               , 0, sizeof(RF_Key_StructTypedef                 ));
    
    Robo_PowerHeatData.buffer_energy = 55.0f;   //在裁判系统断开连接时可操控
    Robo_State.chassis_power_limit = 60.0f;
    Robo_State.shooter_barrel_heat_limit = 1000;
}


/**
 * @brief 裁判系统数据解包
 *
 * @param 电管串口收到的数据包
 */
void Referee_Data_Unpack(uint8_t *frame)
{
    uint16_t Judge_Length;
    uint16_t Cmd_ID = 0;

    if(frame[0] == 0xA5)
    {
        if(verify_CRC8_check_sum(frame, LEN_HEADER) == TRUE)
        {
            Judge_Length = frame[DATA_LENGTH] + LEN_HEADER + LEN_CMDID + LEN_TAIL;
            if(verify_CRC16_check_sum(frame, Judge_Length) == TRUE) 
            {
                Cmd_ID = frame[6] << 8 | frame[5];
                switch (Cmd_ID)
                {
                    case GAME_STATE_CMD_ID:         memcpy(&Game_State,                 frame + DATA_Offset, sizeof(Game_State_StructTypedef));           break;
                    case FIELD_EVENTS_CMD_ID:       memcpy(&Game_Event,                 frame + DATA_Offset, sizeof(Game_Event));                         break;
                    case GAME_ROBOT_HP_CMD_ID:      memcpy(&Robo_HP,                    frame + DATA_Offset, sizeof(Robo_HP_StructTypedef));              break;
                    case ROBOT_STATE_CMD_ID:        memcpy(&Robo_State,                 frame + DATA_Offset, sizeof(Robo_State_StructTypedef));           break;
                    case POWER_HEAT_DATA_CMD_ID:    memcpy(&Robo_PowerHeatData,         frame + DATA_Offset, sizeof(Robo_PowerHeatData));                 break;
                    case ROBOT_POS_CMD_ID:          memcpy(&Robo_Position,              frame + DATA_Offset, sizeof(Robo_Position));                      break;
                    case BUFF_MUSK_CMD_ID:          memcpy(&Robo_Buff,                  frame + DATA_Offset, sizeof(Robo_Buff));                          break;
                    case ROBOT_HURT_CMD_ID:         memcpy(&Robo_HurtData,              frame + DATA_Offset, sizeof(Robo_HurtData));                      break;
                    case SHOOT_DATA_CMD_ID:         memcpy(&Robo_ShootData,             frame + DATA_Offset, sizeof(Robo_ShootData));                     break;
                    case BULLET_REMAINING_CMD_ID:   memcpy(&Robo_BulletRemain,          frame + DATA_Offset, sizeof(Robo_BulletRemain_StructTypedef));    break;
                    case RF_Position_CMD_ID:        memcpy(&RF_Position_Struct,         frame + DATA_Offset, sizeof(RF_Position_StructTypedef));          break;
                    case RF_HP_CMD_ID:              memcpy(&RF_HP_Struct,               frame + DATA_Offset, sizeof(RF_HP_StructTypedef));                break;
                    case RF_Bullet_CMD_ID:          memcpy(&RF_Bullet_Struct,           frame + DATA_Offset, sizeof(RF_Bullet_StructTypedef));            break;
                    case RF_Coin_RFID_CMD_ID:       memcpy(&RF_Coin_RFID_Struct,        frame + DATA_Offset, sizeof(RF_Coin_RFID_StructTypedef));         break;
                    case RF_Key_CMD_ID:             memcpy(&RF_Key_Struct,              frame + DATA_Offset, sizeof(RF_Key_StructTypedef));               break;
                    default: break;
                }
            }
        }
    }
    // 首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,从而判断一个数据包是否有多帧数据
    if (*(frame + Judge_Length) == 0xA5)
    { 
        // 如果一个数据包出现了多帧数据,则再次调用解析函数,直到所有数据包解析完毕
        Referee_Data_Unpack(frame + Judge_Length);
    }
}

/**
 * @brief 图传数据解包
 *
 * @param 图传串口收到的数据包
 */
void ImageLink_Data_Unpack(uint8_t *frame)
{
    uint16_t Judge_Length;
    uint16_t Cmd_ID = 0;

    if(frame[0] == 0xA5)
    {
        if(verify_CRC8_check_sum(frame, LEN_HEADER) == TRUE)
        {
            Judge_Length = frame[DATA_LENGTH] + LEN_HEADER + LEN_CMDID + LEN_TAIL;
            if(verify_CRC16_check_sum(frame, Judge_Length) == TRUE) 
            {
                Cmd_ID = frame[6] << 8 | frame[5];
                switch (Cmd_ID)
                {
                    case Customer_Controller_CMD_ID:memcpy(&Customer_Controller_Struct, frame + DATA_Offset, sizeof(Customer_Controller_StructTypedef));  break;
                    default: break;
                }
            }
        }
    }
    else if(frame[0] == 0xA9 && frame[1] == 0x53)
    {
        Judge_Length = 21;
        Remote_Rx_CallBack(frame);
    }
    // 首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,从而判断一个数据包是否有多帧数据
    if (*(frame + Judge_Length) == 0xA5 || *(frame + Judge_Length) == 0xA9)
    { 
    // 如果一个数据包出现了多帧数据,则再次调用解析函数,直到所有数据包解析完毕
            ImageLink_Data_Unpack(frame + Judge_Length);
    }
}

/**
 * @brief 自定义控制器发送自定义控制器数据
 *
 * @param 数据数组，长度
 */
Controler_Struct_TypeDef Tx_Data;
void Custom_Controler_Send(UART_HandleTypeDef *huart ,uint8_t *Data, uint16_t Data_Length)
{
        /*=== | 添加帧头 |===*/
    Tx_Data.Header.SOF = 0xA5;                              
    Tx_Data.Header.Data_Length = Data_Length; 
    Tx_Data.Header.seq = Tx_Data.Header.seq + 1; 
        
        /*=== | 命令码ID |===*/
    Tx_Data.Cmd_ID = Customer_Controller_CMD_ID;
        
        /*=== | 数据拷贝 |===*/
    memcpy(Tx_Data.Data, Data, Data_Length);
        
        /*=== | 添加CRC校验和 |===*/
        append_CRC8_check_sum((uint8_t *)(&Tx_Data.Header), LEN_HEADER);
    append_CRC16_check_sum((uint8_t *)(&Tx_Data), DATA_FRAME_LENGTH);
    
    HAL_UART_Transmit(huart, (uint8_t *)(&Tx_Data), sizeof(Tx_Data), 1000);
}
