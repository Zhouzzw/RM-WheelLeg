//========================================================
//                      Aim[自瞄]
//      自瞄的通信与处理
//========================================================

#include "Aim.h"
#include "solve_trajectory.h"
#include "INS.h"
#include "Vision_Data_Center.h"
#include "RoboControl.h"
#include "user_lib.h"
#include "Gimbal.h"
#include "Referee_Unpack.h"

#define Bullet_Speed_Average_Count_Number 5
float V0 = 14.5f;
float Bullet_Speed[Bullet_Speed_Average_Count_Number];
float Bullet_Speed_Last;

void Aim_Task(void const * argument)
{
    Vision_Control.IF_Vision_Control = 1;
    
    for(uint8_t i = 0; i < Bullet_Speed_Average_Count_Number; i++)
    {
        Bullet_Speed[i] = V0; 
    }
    
    for(;;)
    {
        if(Robo_ShootData.initial_speed != Bullet_Speed_Last && Robo_ShootData.initial_speed > 12.0f && Robo_ShootData.initial_speed < 16.0f)
        {
            for(uint8_t i = Bullet_Speed_Average_Count_Number - 1; i > 0; i--)
            { 
                Bullet_Speed[i] = Bullet_Speed[i-1];
            }
            Bullet_Speed[0] = Robo_ShootData.initial_speed;
            
            float Speed_Sum = 0;
            for(uint8_t i = 0; i < Bullet_Speed_Average_Count_Number; i++)
            { 
                Speed_Sum += Bullet_Speed[i];
            }
            
            V0 = Speed_Sum / (float)Bullet_Speed_Average_Count_Number;
        }
        Bullet_Speed_Last = Robo_ShootData.initial_speed;
        
        
        //向机器人控制模块传值
        PC_Control_Struct.Vision_Pitch = Vision_Control.control_pitch*(180.0/3.1415926);
        PC_Control_Struct.Vision_Yaw = Vision_Control.control_yaw*(180.0/3.1415926);
        PC_Control_Struct.Enemy_Appear_State = ReceivedPacketVision.tracking;
        if(Vision_Control.IF_Vision_Control == 1 )
        {
            AutoAimConsole(&Vision_Control.control_yaw,&Vision_Control.control_pitch);
            usb_send_vision(1);
        }
        
        osDelay(1);
    }
}



// 定义用到的变量
static trajectory_data_t aim_traj; // 瞄准的弹道数据(aim_trajectory)
static trajectory_data_t cur_traj; // 当前的弹道数据(current_trajectory)

Vision_Control_t Vision_Control;

static bool fire_flag = 0; // 开火标志位

static uint32_t start_fire_time = 0; // 开始开火的时间

PcControl_StructTypeDef PC_Control_Struct;



/**
 * @brief      yaw角循环限幅
 * @param[in]  yaw (rad)
 * @return     限幅后的yaw
 */
float yaw_format(float yaw)
{
    return loop_float_constrain(yaw, -PI, PI);
}

/**
 * @brief      pitch角循环限幅
 * @param[in]  pitch (rad)
 * @return     限幅后的pitch
 */
float pitch_format(float pitch)
{
    return loop_float_constrain(pitch, -PI / 2, PI / 2);
}

/**
 * @brief      roll角循环限幅
 * @param[in]  roll (rad)
 * @return     限幅后的roll
 */
float roll_format(float roll)
{
    return loop_float_constrain(roll, -PI, PI);
}

static float get_friction_coeff(float S, float vs)
{ // 获取空气阻力系数球体阻力系数C，空气密度p，球体迎风面S，速度vs
    // return (C*p*S*pow(vs,2))/2;
    return (0.47 * 1.169 * S * pow(vs, 2)) / 2;
}


/**
 * @brief 获取距离
 * @param x:m x方向上的距离
 * @param y:m y方向上的距离
 * @param z:m z方向上的距离
 * @return distance:m 水平距离
 */
static float get_distance(float dx, float dy, float dz)
{
    return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
}



/**
 * @brief 计算是否符合开火条件
 * @param AimTra:in 瞄准的弹道数据
 * @param gyro_angle_rad:in 当前的陀螺仪数据
 */
static bool JudgeFireFlag(trajectory_data_t *AimTra, gyro_angle_rad_t *gyro_angle_rad)
{
    // 当到达范围内时更新开始开火时间
    // 在之后的 FIRE_CONTINUE_TIME ms 内保持开火位为true
    float error_pitch, error_yaw;
    error_pitch = fabs(AimTra->pitch - gyro_angle_rad->pitch);
    error_yaw = fabs(AimTra->yaw - gyro_angle_rad->yaw);
    bool fire = false;
    uint32_t now_time = HAL_GetTick();
    if ((error_pitch < MAX_AIM_ERROR_PITCH) &&
        (error_yaw < MAX_AIM_ERROR_YAW))
    {
        fire = true;
        start_fire_time = now_time;
    }
    else if (now_time - start_fire_time < FIRE_CONTINUE_TIME)
    {
        fire = true;
    }
    else
    {
        fire = false;
    }

    return fire;
}


/**
 * @brief       陀螺仪数据旋转矩阵（和miniPC上的旋转矩阵参数对齐）
 * @param[out]  gyro_angle_rad 陀螺仪数据
 * @param[in]   yaw yaw旋转角度(rad)
 * @param[in]   pitch pitch旋转角度(rad)
 * @param[in]   roll roll旋转角度(rad)
 */
static void GyroAngleRadRotate(gyro_angle_rad_t *gyro_angle_rad, float yaw, float pitch, float roll)
{
    gyro_angle_rad->yaw = yaw_format(gyro_angle_rad->yaw + yaw);
    gyro_angle_rad->pitch = pitch_format(gyro_angle_rad->pitch + pitch);
    gyro_angle_rad->roll = roll_format(gyro_angle_rad->roll + roll);
}



/**
 * @brief       计算瞄准点的弹道，返回瞄准的xyz坐标及yaw和pitch角度
 * @param[out]  AimTra 瞄准的弹道数据
 * @param[in]   GAR 经过处理的陀螺仪数据（旋转与miniPC对齐），通过get_INS_angle_point()函数获取欧拉角, 0:yaw, 1:pitch, 2:roll 单位 rad
 * @param[in]   s_static 枪口前推的距离
 * @param[in]   z_static yaw轴电机到枪口水平面的垂直距离
 * @param[in]   v0 弹丸初速度(m/s)
 * @param[in]   k 弹丸空气阻力系数
 * @param[in]   BiasTime 固定时间延迟偏置//还有点不了解这个参数具体是干嘛的
 */
static void AimPositionCalculate(trajectory_data_t *AimTra, gyro_angle_rad_t GAR_A,
                                 float s_static, float z_static,
                                 float v0, float k, int BiasTime)
{
    // 获取接收到的视觉数据
    const ReceivedPacketVision_s *ReceivedData = GetReceivedPacketVisionPoint();

    // 初始化弹道计算相关参数
    GimbalControlInit(GAR_A.pitch, GAR_A.yaw,
                      ReceivedData->yaw, ReceivedData->v_yaw,
                      ReceivedData->r1, ReceivedData->r2,
                      ReceivedData->dz, // 这个变量是z2但是现在还不知道要用什么变量
                      ReceivedData->armors_num,
                      v0, k);
    // 计算瞄准位置
    GimbalControlTransform(ReceivedData->x, ReceivedData->y, ReceivedData->z,
                           ReceivedData->vx, ReceivedData->vy, ReceivedData->vz,
                           s_static, z_static,
                           BiasTime,
                           &AimTra->pitch, &AimTra->yaw,
                           &AimTra->x, &AimTra->y, &AimTra->z);
}


/**
 * @brief       计算当前的弹道，返回当前的xyz坐标及yaw和pitch角度
 * @param[out]  CurTra 当前的弹道数据
 * @param[in]   GAR 经过处理的陀螺仪数据（旋转与miniPC对齐），通过get_INS_angle_point()函数获取欧拉角, 0:yaw, 1:pitch, 2:roll 单位 rad
 * @param[in]   v0 弹丸初速度(m/s)
 * @param[in]   k 弹丸空气阻力系数
 * @param[in]   distance 与装甲板之间的水平距离
 */
static void CurrentPositionCalculate(trajectory_data_t *CurTra, gyro_angle_rad_t GAR_C, float v0, float k, float distance)
{
    // 获取接收到的视觉数据
    const ReceivedPacketVision_s *ReceivedData = GetReceivedPacketVisionPoint();

    // 初始化弹道计算相关参数
    GimbalControlInit(GAR_C.pitch, GAR_C.yaw,
                      ReceivedData->yaw, ReceivedData->v_yaw,
                      ReceivedData->r1, ReceivedData->r2,
                      ReceivedData->dz, // 这个变量是z2但是现在还不知道要用什么变量
                      ReceivedData->armors_num,
                      v0, k);

    CurTra->x = distance * cos(GAR_C.yaw);                            // 当前落点的x坐标
    CurTra->y = distance * sin(GAR_C.yaw);                            // 当前落点的y坐标
    CurTra->z = -GimbalControlBulletModel(distance, v0, GAR_C.pitch); // 计算当前落点的z坐标
    // CurTra->z = GimbalControlBulletModel(distance, v0, -GAR_C.pitch);//计算当前落点的z坐标
    // 上面有点奇怪，在pitch角度上加了负号就对了，是一个瞄高打低的状态，但是不知道为什么，以后再深究吧
    CurTra->pitch = GAR_C.pitch; // 当前落点的pitch角
    CurTra->yaw = GAR_C.yaw;     // 当前落点的yaw角
}


/**
 * @brief       自瞄控制器
 * @param[out]  yaw yaw角增量
 * @param[out]  pitch pitch角增量
 */
void AutoAimConsole(float *yaw, float *pitch)
{
    const ReceivedPacketVision_s *ReceivedData = GetReceivedPacketVisionPoint();
    const ReceivedPacketScanStatus_s *ReceivedPacketScanStatus = GetReceivedPacketScanStatus();
    SendPacketVision_s *SendData = GetSendPacketVisionPoint();

    gyro_angle_rad_t gyro_angle_rad;
    gyro_angle_rad.yaw = Gimbal_Control_Struct.Yaw_Feedback * PI/180.0f;
    gyro_angle_rad.pitch = -Gimbal_Control_Struct.Pitch_Feedback * PI/180.0f;
    gyro_angle_rad.roll = 0;

    // 填写云台当前欧拉角数据
    SendData->reset_tracker = 0; // TODO: 由自瞄模式赋值，是否重置追踪器
    SendData->roll = gyro_angle_rad.roll;
    SendData->pitch = gyro_angle_rad.pitch;
    SendData->yaw = gyro_angle_rad.yaw;

    // 云台运行逻辑
    if (ReceivedData->tracking) // 处在目标跟踪状态
    {
        GyroAngleRadRotate(&gyro_angle_rad, ROTATE_YAW, ROTATE_PITCH, ROTATE_ROLL);

        // 计算瞄准的弹道
        AimPositionCalculate(&aim_traj, gyro_angle_rad,
                             S_STATIC, Z_STATIC,
                             V0, FRICTION_COEFF,
                             BIAS_TIME);

        float distance = get_distance(aim_traj.x, aim_traj.y, 0);

        // 计算当前的弹道
        CurrentPositionCalculate(&cur_traj, gyro_angle_rad, V0, FRICTION_COEFF, distance); // 计算在目标xy坐标下的落点

        // 求解需要旋转的角度（(目标位置-当前位置)）
        float dyaw = yaw_format(aim_traj.yaw - gyro_angle_rad.yaw);
        // float dpitch = pitch_format(atan2(aim_traj.z - cur_traj.z, distance));
        float dpitch = pitch_format(-(aim_traj.pitch - gyro_angle_rad.pitch)); // pitch轴旋转方向相反

        // 填写发送的yaw和pitch角度
        *yaw = aim_traj.yaw;
        *pitch = aim_traj.pitch;

        // 判断是否符合开火条件
        fire_flag = JudgeFireFlag(&aim_traj, &gyro_angle_rad);
    }
    else // 处在丢失目标的状态
    {    /*造成目标丢失的情况比较多，都有不同的云台运行逻辑
           但都不适合开火，并且为了方便调试，落点直接计算3米处的*/

        // 计算当前的弹道
//        CurrentPositionCalculate(&cur_traj, gyro_angle_rad, V0, FRICTION_COEFF, 3); // 目标丢失时计算在3米处的落点

        fire_flag = 0; // 不符合开火条件

//        if (false) // usb掉线
//        {
//            *yaw = gyro_angle_rad.yaw;            // 保持当前位置
//            *pitch = -(0 - gyro_angle_rad.pitch); // pitch回0
//        }
//        else if (!ReceivedPacketScanStatus->stop_gimbal_scan) // 处在扫描状态
//        {
//            *yaw = gyro_angle_rad.yaw + SCAN_DYAW; // 扫描
//            *pitch = -(0 - gyro_angle_rad.pitch);  // pitch回0
//        }
//        else // 其他状态
//        {
//            *yaw = gyro_angle_rad.yaw;            // 保持当前位置
//            *pitch = -(0 - gyro_angle_rad.pitch); // pitch回0
//        }
    }

//    SendData->fire = fire_flag; // 发送是否开火


    // 现在反而有点不知道aim point的作用了
    SendData->aim_x = cur_traj.x;            // 主要用于上位机可视化与自瞄调试，不影响实际功能
    SendData->aim_y = cur_traj.y;            // 主要用于上位机可视化与自瞄调试，不影响实际功能
    SendData->aim_z = cur_traj.z - Z_STATIC; // 主要用于上位机可视化与自瞄调试，不影响实际功能
}

/**
 * @brief   获取开火标志符(1可以开火，0不可以开火)
 * @param   none
 * @return  开火标志符
 */
bool GetFireFlag(void)
{
    return fire_flag;
}


