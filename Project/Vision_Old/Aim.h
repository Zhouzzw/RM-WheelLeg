#ifndef __Aim__
#define __Aim__

#include "main.h"
#include "stdbool.h"

#define MAX_AIM_ERROR_PITCH 0.2f //(rad) 最大瞄准pitch误差
#define MAX_AIM_ERROR_YAW 0.15f //(rad) 最大瞄准yaw误差
#define SCAN_DYAW 0.2f     // (rad) 扫描偏航角增量

#define DIAMETER 42 // (mm) 弹丸直径

// 弹丸相关参数
#if (DIAMETER == 17)
#define V0     23            // (m/s) 小弹丸初速度
#define FRICTION_COEFF 0.033f // 17mm小单丸受到的空气阻力系数

#elif (DIAMETER == 42)
//#define V0 15.5                 // (m/s) 大弹丸初速度
#define FRICTION_COEFF 0.019f // 42mm大弹丸受到的空气阻力系数//get_friction_coeff(0.53, 1.293, 0.00001385);

#else
//当弹丸直径填写错误的时候跳出错误提示
#error "The DIAMETER is not 17 or 42, plase write 17 or 42! ! ! !"
#endif


#define BIAS_TIME 10 // (ms) USB通信偏差时间
#define FIRE_CONTINUE_TIME 500 //(ms) 连续开火时间

// 机器人结构相关参数
#define S_STATIC 0.080f // (m) 枪口前推的距离 根据机器人的实际情况做出改变，具体值请从图纸上查阅
#define Z_STATIC 0.1400f // (m) yaw轴电机到枪口水平面的垂直距离 根据机器人的实际情况做出改变，具体值请从图纸上查阅

// 旋转矩阵用相关参数
#define ROTATE_YAW 0   // (rad) yaw旋转参数
#define ROTATE_PITCH 0 // (rad) pitch旋转参数
#define ROTATE_ROLL 0  // (rad) roll旋转参数

typedef struct 
{
	float Vision_Pitch;
	float Vision_Yaw;
	
	uint8_t Enemy_Appear_State;
	uint8_t Enemy_Appear_State_Last;

}PcControl_StructTypeDef;

typedef struct
{
  float yaw;         // 射击偏航角
  float pitch;       // 射击俯仰角
  float x;           // 落点x坐标
  float y;           // 落点y坐标
  float z;           // 落点z坐标
} trajectory_data_t; // 弹道参数

typedef struct
{
  float yaw;         // 陀螺仪测得的偏航角
  float pitch;       // 陀螺仪测得的俯仰角
  float roll;        // 陀螺仪测得的滚转角
} gyro_angle_rad_t; // 陀螺仪参数

typedef struct
{
  float x;
  float y;
  float z;
  float vx;
  float vy;
  float vz;
} target_data_t; // 锁定目标的参数

typedef struct
{
	float control_pitch;
	float control_yaw;
	uint8_t IF_Vision_Control;
	
}Vision_Control_t;


// 开放调用的变量
extern PcControl_StructTypeDef PC_Control_Struct;


static bool JudgeFireFlag(trajectory_data_t *AimTra, gyro_angle_rad_t *gyro_angle_rad);

static void AimPositionCalculate(trajectory_data_t *AimTra, gyro_angle_rad_t GAR_A,
                                 float s_static, float z_static,
                                 float v0, float k, int BiasTime);

static void CurrentPositionCalculate(trajectory_data_t *CurTra, gyro_angle_rad_t GAR_C, float v0, float k, float distance);

static float get_distance(float dx, float dy, float dz);

static void GyroAngleRadRotate(gyro_angle_rad_t *gyro_angle_rad, float yaw, float pitch, float roll);

void AutoAimConsole(float *yaw, float *pitch);

bool GetFireFlag(void);

const bool *GetFireFlagPoint(void);

void fire_ctrl(float allow_fire_ang_max, float allow_fire_ang_min, uint8_t target);

void expected_preview_calc(void);


extern Vision_Control_t Vision_Control;
                                 
                                 
#endif
