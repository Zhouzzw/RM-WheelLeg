#ifndef __Referee_Unpack__
#define __Referee_Unpack__

#include "main.h"

#define HEADER_SOF 0xA5
#define REF_PROTOCOL_FRAME_MAX_SIZE         128

#define REF_PROTOCOL_HEADER_SIZE            sizeof(frame_header_struct_t)
#define REF_PROTOCOL_CMD_SIZE               2
#define REF_PROTOCOL_CRC16_SIZE             2
#define REF_HEADER_CRC_LEN                  (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE)
#define REF_HEADER_CRC_CMDID_LEN            (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE + sizeof(uint16_t))
#define REF_HEADER_CMDID_LEN                (REF_PROTOCOL_HEADER_SIZE + sizeof(uint16_t))

#pragma PACK(push, 1)

/*===| 机器人ID定义 |===*/
typedef enum
{
        Hero_Red       = 1,
        Engineer_Red   = 2,
        Infantry3_Red  = 3,
        Infantry4_Red  = 4,
        Infantry5_Red  = 5,
        Plane_Red      = 6,
        
        Hero_Blue      = 101,
        Engineer_Blue  = 102,
        Infantry3_Blue = 103,
        Infantry4_Blue = 104,
        Infantry5_Blue = 105,
        Plane_Blue     = 106,
} judge_robot_ID;


/*===| 裁判系统命令码ID |===*/
typedef enum
{
    GAME_STATE_CMD_ID                 = 0x0001,
    GAME_RESULT_CMD_ID                = 0x0002,
    GAME_ROBOT_HP_CMD_ID              = 0x0003,
    FIELD_EVENTS_CMD_ID               = 0x0101,
    SUPPLY_PROJECTILE_ACTION_CMD_ID   = 0x0102,
    SUPPLY_PROJECTILE_BOOKING_CMD_ID  = 0x0103,
    REFEREE_WARNING_CMD_ID            = 0x0104,
    ROBOT_STATE_CMD_ID                = 0x0201,
    POWER_HEAT_DATA_CMD_ID            = 0x0202,
    ROBOT_POS_CMD_ID                  = 0x0203,
    BUFF_MUSK_CMD_ID                  = 0x0204,
    AERIAL_ROBOT_ENERGY_CMD_ID        = 0x0205,
    ROBOT_HURT_CMD_ID                 = 0x0206,
    SHOOT_DATA_CMD_ID                 = 0x0207,
    BULLET_REMAINING_CMD_ID           = 0x0208,
    STUDENT_INTERACTIVE_DATA_CMD_ID   = 0x0301,
    
    Customer_Controller_CMD_ID        = 0x0302,
    ImageLink_CMD_ID                  = 0x0304,
    
    RF_Position_CMD_ID                = 0x0A01,
    RF_HP_CMD_ID                      = 0x0A02,
    RF_Bullet_CMD_ID                  = 0x0A03,
    RF_Coin_RFID_CMD_ID               = 0x0A04,
    RF_Gain_CMD_ID                    = 0x0A05,
    RF_Key_CMD_ID                     = 0x0A06,
} Referee_CMD_ID_EnumTypedef;

/*===| 包头 |===*/
typedef __PACKED_STRUCT
{
  uint8_t SOF;
  uint16_t Data_Length;
  uint8_t seq;
  uint8_t CRC8;
} Referee_Package_Header_StructTypedef;

//*===| 帧头偏移 |===*/
typedef enum
{
        SOF = 0,                 // 起始位
        DATA_LENGTH = 1, // 帧内数据长度,根据这个来获取数据长度
        SEQ = 3,                 // 包序号
        CRC8 = 4                 // CRC8
} FrameHeaderOffset_e;

/*===| 通信协议长度 |===*/
typedef enum
{
        LEN_HEADER = 5,                 // 帧头长
        LEN_CMDID = 2,                        // 命令码长度
        LEN_TAIL = 2,                        // 帧尾CRC16
        
        /*===| 例:data从Rx_Buff[7]开始 |===*/
        FRAME_HEADER_Offset = 0,//帧头偏移位
        CMD_ID_Offset = 5,                //命令码偏移位
        DATA_Offset = 7,                //数据偏移位                
        
} JudgeFrameLength_Enum;

/*===| 裁判系统数据解包 |===*/
typedef __PACKED_STRUCT
{
  Referee_Package_Header_StructTypedef *p_header;
  uint16_t       data_len;
  uint8_t        protocol_packet[REF_PROTOCOL_FRAME_MAX_SIZE];
  uint16_t       index;
} Referee_PackData_StructTypedef;


/*===| 比赛进程定义 |===*/
typedef enum
{
    PROGRESS_UNSTART        = 0,
    PROGRESS_PREPARE        = 1,
    PROGRESS_SELFCHECK      = 2,
    PROGRESS_5sCOUNTDOWN    = 3,
    PROGRESS_BATTLE         = 4,
    PROGRESS_CALCULATING    = 5,
} Game_Stage_EnumTypedef;

/*===| 比赛状态 |===*/
typedef __PACKED_STRUCT
{ 
    uint8_t game_type : 4; 
    uint8_t Game_Progress : 4; 
    uint16_t Stage_Remain_Time; 
    uint64_t SyncTimeStamp; 
} Game_State_StructTypedef;

/*===| 机器人血量 |===*/
typedef __PACKED_STRUCT
{
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_base_HP;
} Robo_HP_StructTypedef;

/*===| 场地事件[101] |===*/
typedef __PACKED_STRUCT //
{
    uint32_t event_type;
} Game_Event_StructTypedef;

/*===| 当前机器人状态[201] |===*/
typedef __PACKED_STRUCT
{
    uint8_t robot_id;                             	  //机器人ID
        uint8_t robot_level;                          //机器人等级
        uint16_t current_HP;                          //当前血量    
        uint16_t maximum_HP;                          //最大血量
        uint16_t shooter_barrel_cooling_value;        //枪口冷却速率
        uint16_t shooter_barrel_heat_limit;           //热量限制最大值
        uint16_t chassis_power_limit;                 //底盘功率限制
        uint8_t power_management_gimbal_output : 1;   //云台供电
        uint8_t power_management_chassis_output : 1;  //底盘供电
        uint8_t power_management_shooter_output : 1;  //发射机构供电
} Robo_State_StructTypedef;

/*===| 功率和热量数据[202] |===*/
typedef __PACKED_STRUCT 
{
    uint16_t chassis_voltage;                          //底盘输出电压 mV(已失效)
    uint16_t chassis_current;                          //底盘输出电流 mA(已失效)
    float chassis_power;                               //底盘输出功率 (已失效)
    uint16_t buffer_energy;                            //底盘缓冲功率剩余
    uint16_t shooter_17mm_barrel_heat;                 //17mm 发射机构的枪口热量 
    uint16_t shooter_42mm_barrel_heat;                 //42mm枪口目前热量
} Robo_PowerHeatData_StructTypedef;

/*===| 机器人位置 |===*/
typedef __PACKED_STRUCT //0x0203
{
        float x;                                                                        //本机器人位置 x 坐标，单位：m
        float y;                                                                        //本机器人位置 y 坐标，单位：m
        float angle;                                                                //本机器人测速模块朝向，单位：度。正北为 0 度
} Robo_Position_StructTypedef;

/*===| 增益数据 |===*/
typedef __PACKED_STRUCT //0x0204
{
    uint8_t recovery_buff;                                            //机器人回血增益（百分比，值为 10 意为每秒回复 10%最大血量）
        uint8_t cooling_buff;                                                //机器人枪口冷却倍率（直接值，值为 5 意味着 5 倍冷却）
        uint8_t defence_buff;                                                //机器人防御增益（百分比，值为 50 意为 50%防御增益）
        uint16_t attack_buff;                                                //机器人攻击增益（百分比，值为 50 意为 50%攻击增益）
} Robo_Buff_StructTypedef;

/*===| 扣血数据 |===*/
typedef __PACKED_STRUCT //0x0206
{
  uint8_t armor_id : 4;                         //扣血方向
        uint8_t HP_deduction_reason : 4;            //扣血原因
} Robo_HurtData_StructTypedef;

/*===| 发射弹丸数据 |===*/
typedef __PACKED_STRUCT //0x0207
{
    uint8_t bullet_type;                                //弹丸类型
    uint8_t shooter_number;                                //发射机构ID
    uint8_t launching_frequency;                //弹丸射频（单位：Hz）
    float initial_speed;                                //弹丸初速度（单位：m/s）
} Robo_ShootData_StructTypedef;

/*===| 剩余发弹量 |===*/
typedef __PACKED_STRUCT //0x208
{
    uint16_t projectile_allowance_17mm;                        //17mm 弹丸允许发弹量
    uint16_t projectile_allowance_42mm;                        //42mm 弹丸允许发弹量
    uint16_t remaining_gold_coin;                                //剩余金币数量
    uint16_t projectile_allowance_fortress;     //堡垒增益点提供的储备17mm弹丸允许发弹量
} Robo_BulletRemain_StructTypedef;

/*===| 图传链路-键鼠数据 |===*/
typedef __PACKED_STRUCT
{ 
    int16_t Mouse_Speed_X; 
    int16_t Mouse_Speed_Y; 
    int16_t Mouse_Speed_Z; 
    uint8_t Mouse_Left; 
    uint8_t Mouse_Right; 
    uint16_t KeyBoard; 
    uint16_t Reserved; 
} Keyboard_Mouse_StructTypedef; 

/*===| 图传链路-自定义控制器数据 |===*/
typedef __PACKED_STRUCT
{
    uint8_t Customer_Controller_Data[30];
} Customer_Controller_StructTypedef; 

/*===| 雷达无线链路-对方机器人位置 |===*/
typedef __PACKED_STRUCT
{ 
    int16_t Robo_1_X_cm; 
    int16_t Robo_1_Y_cm; 
    int16_t Robo_2_X_cm; 
    int16_t Robo_2_Y_cm; 
    int16_t Robo_3_X_cm; 
    int16_t Robo_3_Y_cm; 
    int16_t Robo_4_X_cm; 
    int16_t Robo_4_Y_cm; 
    int16_t Robo_6_X_cm; 
    int16_t Robo_6_Y_cm; 
    int16_t Robo_5_X_cm; 
    int16_t Robo_5_Y_cm; 
} RF_Position_StructTypedef; 

/*===| 雷达无线链路-对方机器人血量 |===*/
typedef __PACKED_STRUCT
{ 
    int16_t Robo_1_HP; 
    int16_t Robo_2_HP; 
    int16_t Robo_3_HP; 
    int16_t Robo_4_HP; 
    int16_t reserve; 
    int16_t Robo_7_HP; 
} RF_HP_StructTypedef; 

/*===| 雷达无线链路-对方机器人允许发弹量 |===*/
typedef __PACKED_STRUCT
{ 
    int16_t Robo_1_Bullet; 
    int16_t Robo_3_Bullet; 
    int16_t Robo_4_Bullet; 
    int16_t Robo_6_Bullet; 
    int16_t Robo_7_Bullet; 
} RF_Bullet_StructTypedef; 

/*===| 雷达无线链路-对方金币与占领状态 |===*/
typedef __PACKED_STRUCT
{ 
    int16_t Remain_Coin; 
    int16_t Total_Coin; 
    int32_t RFID_State; 
} RF_Coin_RFID_StructTypedef; 

/*===| 雷达无线链路-干扰波秘钥 |===*/
typedef __PACKED_STRUCT
{ 
    uint8_t Key[6];
} RF_Key_StructTypedef; 

/*===| 自定义控制器发送数据包 |===*/
#define CONTROLER_DATA_LENGTH                30
#define DATA_FRAME_LENGTH (LEN_HEADER + LEN_CMDID + CONTROLER_DATA_LENGTH + LEN_TAIL) 
typedef __PACKED_STRUCT
{
        Referee_Package_Header_StructTypedef Header;
    uint16_t Cmd_ID;                       // ???
    uint8_t Data[30];                      // ??????????
    uint16_t Frame_Tail;                   // ??CRC16??
} Controler_Struct_TypeDef; 


extern Game_State_StructTypedef                Game_State;              //比赛阶段
extern Game_Event_StructTypedef                Game_Event;              //比赛事件

extern Robo_State_StructTypedef                Robo_State;              //机器人状态
extern Robo_HP_StructTypedef                   Robo_HP;                 //全场机器人血量
extern Robo_Position_StructTypedef             Robo_Position;           //机器人位置
extern Robo_Buff_StructTypedef                 Robo_Buff;               //机器人增益
extern Robo_HurtData_StructTypedef             Robo_HurtData;           //受伤数据
                                                              
extern Robo_PowerHeatData_StructTypedef        Robo_PowerHeatData;      //功率和热量情况
extern Robo_ShootData_StructTypedef            Robo_ShootData;          //发射机构数据
extern Robo_BulletRemain_StructTypedef         Robo_BulletRemain;       //剩余弹量4

extern Keyboard_Mouse_StructTypedef            Keyboard_Mouse_Struct;      //图传链路键鼠数据
extern Customer_Controller_StructTypedef       Customer_Controller_Struct; //图传链路自定义控制器数据

extern RF_Position_StructTypedef               RF_Position_Struct;         //对方机器人位置
extern RF_HP_StructTypedef                     RF_HP_Struct;               //对方机器人血量
extern RF_Bullet_StructTypedef                 RF_Bullet_Struct;           //对方机器人允许发弹量
extern RF_Coin_RFID_StructTypedef              RF_Coin_RFID_Struct;        //对方金币与占领状态
extern RF_Key_StructTypedef                    RF_Key_Struct;              //干扰波秘钥

void Referee_Data_Init(void);

void Referee_Data_Unpack(uint8_t *frame);
void ImageLink_Data_Unpack(uint8_t *frame);
void Custom_Controler_Send(UART_HandleTypeDef *huart ,uint8_t *Data, uint16_t Data_Length);

#endif

