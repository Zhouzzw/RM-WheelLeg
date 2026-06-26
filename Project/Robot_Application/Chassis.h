#ifndef __Chassis__
#define __Chassis__

#include "main.h"
#include "kalman_filter.h"

#define Energy_PID_Kp								10.0f
#define Energy_PID_Ki								0.0f
#define Energy_PID_Kd								0.0f
#define Energy_PID_I_Output_Max						0.0f
#define Energy_PID_Output_Max						100.0f


/*===| 底盘运动状态枚举定义 |===*/
typedef enum 
{
    Chassis_OFF = 0,
    Chassis_STATIC = 1,
    Chassis_FOLLOW = 2,
    Chassis_F_SIDE = 3,
    Chassis_SPIN = 4,
    Chassis_DASH = 5,
	Chassis_Bench= 6,
} Chassis_State_EnumTypedef;

/*===| 倾倒检测结构体 |===*/
typedef struct 
{
    uint8_t Warning_Flag;           // 倾倒预警标志（1：需要降低腿长）
    float Current_Pitch_Angle;      // 当前俯仰角度
    float Current_Pitch_Rate;       // 当前俯仰角速度
    float Current_Theta_Error;      // 当前腿部角度误差
    uint32_t Warning_Counter;       // 预警计数器
} Simple_TipOver_TypeDef;

/*=== 氮气弹簧 腿长-力 映射表点 ===*/
typedef struct 
{
    float leg_len;   // 腿长 (m)
    float force;     // 该腿长下的弹簧推力 (N)，方向为使腿伸长
} SpringMapPoint_t;

/*=== 氮气弹簧模型结构体===*/
typedef struct 
{
    SpringMapPoint_t *points;      // 映射表指针
    uint8_t num_points;            // 有效点数
    float max_len;                 // 最大腿长（弹簧完全伸展）
    float min_len;                 // 最小腿长（弹簧完全压缩）
    uint8_t enable;                // 补偿使能标志
    float safety_force_limit;      // 补偿力安全限幅 (N)
} GasSpringModel_TypeDef;


/*===| 底盘数据结构体定义 |===*/
typedef struct
{
    /*===| 底盘运动状态 |===*/
    Chassis_State_EnumTypedef Chassis_State;
    
	float Power_MAX;
	
    /*===| 机器状态标志位 |===*/
	uint8_t Power_Limit_Flag;
	uint8_t right_off_flag;	//右腿离地标志位
	uint8_t left_off_flag;	//左腿离地标志位
    uint8_t Off_ground_Flag;//离地检测标志位 触地为0 离地为1
	uint8_t Balance_Flag;	//平衡标志位 倒地为0 平衡为1
	uint8_t Up_Slope_Mode_Flag;
	
	//缓起立相关标志位
	uint8_t Smooth_restand_Flag;//缓起立功能执行标志位
	uint8_t Allow_to_Stand_Flag;//允许起立进入常规运控标志位
	uint16_t Allow_to_Stand_Tick;
	
	/*===| 机器底盘正方向切换标志位 |===*/
	uint8_t Chassis_Forward_Change_Flag;
	
	/*===| 机器人跳跃功能相关变量 |===*/
	uint8_t Jump_Flag;
	uint8_t Jump_State;
	uint16_t Jump_state_change_Tick;
	
	/*===| 机器人倒地自起功能相关变量 |===*/
	uint8_t Recover_from_ground_Flag;
	uint8_t Recover_Mode;
	uint8_t Recover_State;
	uint16_t Recover_state_change_Tick;
	
	/*===| 机器人上台阶功能相关变量 |===*/
	uint8_t UpStep_Dect_Flag;//上台阶磕碰检测标志位
	uint8_t UpStep_Flag;
	uint8_t UpStep_State;
	uint16_t UpStep_state_change_Tick;
	
	/*===| 氮气弹簧动态补偿相关变量 |===*/
	GasSpringModel_TypeDef GasSpring_L;
	GasSpringModel_TypeDef GasSpring_R;
	
	/*===| 功率控制相关定义 |===*/
	float Chassis_Power_MAX;
    
	/*===| 一堆指针 |===*/
    float state_lqr[6];
	float v_state[2];            //vx vy
	
    /*===| 底盘设置速度 |===*/
		float Chassis_Vx_Target;		//底盘x方向目标速度
		float Chassis_Vy_Target;		//底盘y方向目标速度
		float Chassis_Vy_Target_last;
		float Chassis_Wz_Target;        //底盘旋转角速度   m/s
		float Chassis_Vx_Set;			//底盘x方向上的当前设定值
		float Chassis_Vy_Set;			//底盘y方向上的当前设定值
		float Chassis_Wz_Set;           //底盘旋转角速度   m/s
		float Volecity_R;
		float Volecity_L;
		float Volecity_Fdb;
		float Volecity_Fdb_last;
		
	/*===| 云台设置速度 |===*/
		float Gimbal_Vx_Target;
		float Gimbal_Vy_Target;
		float Gimbal_Wz_Target;
		float Gimbal_Vx_Set;            //底盘实际设定左右速度 m/s
		float Gimbal_Vy_Set;            //底盘实际设定前后速度 m/s
		float Gimbal_Wz_Set;            //底盘目标设定旋转角速度   rad/s

    /*===| 角度反馈值 |===*/
		float Yaw_ErrEncoder;
		float Yaw_ErrAngle;
		float Roll_Feedback;
		float Yaw_Feedback;
		float Pitch_Angle;
		float Pitch_Angle_Last;
		float Pitch_Speed_fdb;
		float Pitch_Speed_fdb_last;
    
	/*===| 位移 |===*/
        float Left_displacement_now;
        float Right_displacement_now;
        float Left_displacement_last;
        float Right_displacement_last;
        float Left_displacement;
        float Right_displacement;
        
        float Left_displacement_Init;
        float Right_displacement_Init;

        float displacement;
		float now_displacement;
		float add_displacement;
		float Target_displacement;
		
	/*===| 腿部变量 |===*/
		//左腿长度
		float Leg_Length_feedback;
		float Left_Leg_Length;
		float Left_Leg_Length_last;
		float Left_Leg_Length_dot;
		float Left_Leg_Length_dot_last;
		float Left_Leg_Length_dot_dot;
		//右腿长度
		float Right_Leg_Length;
		float Right_Leg_Length_last;
		float Right_Leg_Length_dot;
		float Right_Leg_Length_dot_last;
		float Right_Leg_Length_dot_dot;
		
		uint8_t Leg_length_Level;
		
		//左腿角度
		float Left_Leg_Angle;
		float Left_Leg_Angle_last;
		float Left_Leg_Angle_dot;
		float Left_Leg_Angle_dot_last;
		float Left_Leg_TotalAngle;
		float Left_Leg_TotalAngle_Last;
		int16_t Left_Leg_Angle_Round;
		//右腿角度
		float Right_Leg_Angle;
		float Right_Leg_Angle_last;
		float Right_Leg_Angle_dot;
		float Right_Leg_Angle_dot_last;
		float Right_Leg_TotalAngle;
		float Right_Leg_TotalAngle_Last;
		int16_t Right_Leg_Angle_Round;
		
		
		float Angle[4];
		float Ave_Leg_Length;
		
		uint8_t Jump_Level;
	
    /*===| 雅可比矩阵 |===*/
	float Jl[2][2];
    float Jr[2][2];
	
	/*===| 雅可比力矩阵 |===*/
	float Jt_l[2][2];
	float Jt_r[2][2];
	
	/*===| 10个状态量 |===*/
	float State[10];
	float State_last[10];
	
	/*===| 电机最后输出力矩 |===*/
	float Torque_wheel_left;
	float TorqueCurrent_wheel_left;
	float Torque_wheel_right;
	float TorqueCurrent_wheel_right;
	float Torque_Joint_left;
	float Torque_Joint_right;
	float JointTorque_Set[4];
//	float JointSpeed[4];
	
	/*===| 一些PID的输出 |===*/
	float F_L , F_R;
	
	/*===| 陀螺仪数据 |===*/
	float MotionAccel_n_X;		//绝对系加速度
	float MotionAccel_n_Y;
	float MotionAccel_n_Z;
	
	float MotionAccel_b_X;		//机体坐标加速度
	float MotionAccel_b_Y;
	float MotionAccel_b_Z;
	
	/*===| 倾倒检测结构体 |===*/
	Simple_TipOver_TypeDef TipOver_Simple;
	
    /*===| PID结构体 |===*/
	PID_Struct_TypeDef Left_leg_Length_PID;
	PID_Struct_TypeDef Right_leg_Length_PID;
	PID_Struct_TypeDef Left_leg_Length_Speed_PID;
	PID_Struct_TypeDef Right_leg_Length_Speed_PID;
	
	PID_Struct_TypeDef Fold_left_Leg_Position_PID;
	PID_Struct_TypeDef Fold_right_Leg_Position_PID;
	PID_Struct_TypeDef Fold_left_Leg_Speed_PID;
	PID_Struct_TypeDef Fold_right_Leg_Speed_PID;
	
	PID_Struct_TypeDef Limit_Left_Wheel_RPM_PID;
	PID_Struct_TypeDef Limit_Right_Wheel_RPM_PID;
	
	PID_Struct_TypeDef Left_leg_Angle_PID;
	PID_Struct_TypeDef Right_leg_Angle_PID;
	PID_Struct_TypeDef Left_leg_AngleSpeed_PID;
	PID_Struct_TypeDef Right_leg_AngleSpeed_PID;
	
    PID_Struct_TypeDef Roll_Leg_PID;
	PID_Struct_TypeDef Roll_Leg_Speed_PID;
	PID_Struct_TypeDef Turn_Angle_PID;
	PID_Struct_TypeDef Turn_Speed_PID;
	PID_Struct_TypeDef Tp_Comp_PID;
		
} Chassis_Control_StructTypeDef;


/*===| 防打滑控制结构体 |===*/
typedef struct 
{
    float Speed_Est;       // 估计速度（滤波后）
    float Speed_Meas;      // 测量速度（编码器）
    float Speed_Last;      // 上一时刻速度
    float P;               // 状态协方差
    float Q;               // 过程噪声（0.001~0.1）
    float R;               // 测量噪声（0.01~0.5）
    float K;               // 卡尔曼增益
    float SlipThreshold;   // 打滑阈值（0.3~1.0）
    uint8_t SlipFlag;      // 打滑标志
    float Accel;           // 加速度（用于检测）
} WheelKF_TypeDef;

typedef struct 
{
    WheelKF_TypeDef LeftWheel;
    WheelKF_TypeDef RightWheel;
    float RobotSpeed_Est;  // 机器人整体估计速度
} AntiSlip_TypeDef;


/*===| 控制数据结构体 |===*/
extern Chassis_Control_StructTypeDef Chassis_Control_Struct;
extern AntiSlip_TypeDef AntiSlip;

void Calculated_displacement(void);
void Motor_DM_One_Control_Four(FDCAN_HandleTypeDef* hfdcan, uint16_t CAN_ID, int16_t ID1_current,int16_t ID2_current,int16_t ID3_current,int16_t ID4_current);
void Output_Current(void);
void Get_Control_Data(void);
void Chassis_Get_Speed(void);
void Chassis_Init(void);
void Accelerated_Speed(float Target_Speed, float* Speed_Set);
void calculateValues_Q_l(Chassis_Control_StructTypeDef *leg_yak2);
void calculateValues_Q_r(Chassis_Control_StructTypeDef *leg_yak2);
void calucateTp(Chassis_Control_StructTypeDef *leg_yak9);
void Fl_Calculate(void);
void Fr_Calculate(void);

void State_Filter(void);
void add_six_state(Chassis_Control_StructTypeDef *leg_yak4);
void calucateK(float (*Poly_Coefficient)[4]);
float LQR_K_calc(float *coe,float len);
void calucateLQR_L(float *lqr_l);
void calucateLQR_R(float *lqr_r);
void Off_ground_Detection(void);
void Jump_Control(void);
void Recover_from_Ground(void);
void Simple_TipOver_Init(void);
void Simple_TipOver_Detection(void);
void Up_Step_Dection(void);
void Up_Step_Func(void);
void GasSpring_Init(GasSpringModel_TypeDef *model, const SpringMapPoint_t *table, uint8_t num, float min_len, float max_len);
float GasSpring_GetForce(const GasSpringModel_TypeDef *model, float leg_len);
void Smooth_Restand_Func(void);

void xvEstimateKF_Init(KalmanFilter_t *L_EstimateKF,KalmanFilter_t *R_EstimateKF);
void xvEstimateKF_Update(KalmanFilter_t *EstimateKF ,float acc,float vel);

#endif