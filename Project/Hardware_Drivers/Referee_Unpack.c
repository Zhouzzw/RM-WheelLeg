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

#define Chassis_Attributes  0

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

Keyboard_Mouse_StructTypedef            Keyboard_Mouse_Struct;      //图传链路键鼠数据
Customer_Controller_StructTypedef       Customer_Controller_Struct; //图传链路自定义控制器数据
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
    
    Robo_PowerHeatData.buffer_energy = 60.0f;   //在裁判系统断开连接时可操控
}


/**
 * @brief 裁判系统数据解包
 *
 * @param 串口收到的数据包
 */
void Referee_Data_Unpack(uint8_t *frame)
{
	uint16_t Judge_Length;
	uint16_t Cmd_ID = 0;
	if(frame == NULL)   return;
	memcpy(&Referee_Package_Header, frame, sizeof(Referee_Package_Header_StructTypedef));
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
                    
                    case ImageLink_CMD_ID:     memcpy(&Keyboard_Mouse_Struct,      frame + DATA_Offset, sizeof(Keyboard_Mouse_StructTypedef));  
                                                    ImageLink_Rx_CallBack(&Keyboard_Mouse_Struct);                                                  break;
                    case Customer_Controller_CMD_ID:memcpy(&Customer_Controller_Struct, frame + DATA_Offset, sizeof(Customer_Controller_StructTypedef));  break;
					default: break;
				}
			}
		}
	}
	// 首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,从而判断一个数据包是否有多帧数据
	if (*(frame + sizeof(Referee_Package_Header_StructTypedef) + LEN_CMDID + Referee_Package_Header.Data_Length + LEN_TAIL) == 0xA5)
	{ 
        // 如果一个数据包出现了多帧数据,则再次调用解析函数,直到所有数据包解析完毕
		Referee_Data_Unpack(frame + sizeof(Referee_Package_Header_StructTypedef) + LEN_CMDID +Referee_Package_Header.Data_Length + LEN_TAIL);
	}
}

