//===========================================================================
//                      Chassis[底盘控制]
//      根据云台发下来的Vx,Vy,Wz,YawErr进行运动解算，实现底盘的各种运动状态
//      以及根据裁判系统反馈数据和[超电状态]实现功率控制
//                     腿长 0.15 ~ 0.35 m
//===========================================================================

#include "Chassis.h"
#include "RoboControl.h"
#include "Power_Limit.h"
#include "CAN_Driver.h"
#include "Motor_DJI_Driver.h"
#include "PID.h"
#include "USART_Driver.h"
#include "Motor_Unitree_Driver.h"
#include "Motor_DAMIAO_Driver.h"
#include "Referee_Unpack.h"
#include "Vofa.h"
#include "INS.h"
#include "Function.h"
#include "Buzzer.h"
#include "arm_math.h"  // CMSIS-DSP 库
#include "user_lib.h"
#include "Remote_Control.h"
#include "kalman_filter.h"
#include <math.h>   
#include <stdbool.h>

/*===| 底盘控制数据结构体 |===*/
Chassis_Control_StructTypeDef Chassis_Control_Struct;
PID_Struct_TypeDef Energy_Buffer_PID;
PID_Struct_TypeDef Trans_VelocityWz_PID;
INS_t INS;
	
uint32_t Tick;
float Chassis_dt=0.0f;
float TP_wl_wr_jl_jr[6] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; //关节电机0 1 2 3//轮电机4 5

float Joint_Motor_Angle[4]={0};
float Joint_Motor_Total_Angle[4]={0};
float Joint_Motor_Round[4]={0};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Target_Roll_angle=0.0f;

float Target_Yaw_angle=0.0f;
float Chassis_Wz=0.0f;

float flag_a=1.0f,flag_b=1.0f;
float flag_c=1.0f,flag_d=1.0f;

float Joint_Comp_L=0.0f;
float Joint_Comp_R=0.0f;

float Left_angle;
float Right_angle;

float Composite_V;
float Steer_Angle_target;
float Steer_Total_angle_Target;
float Steer_Total_angle_Feedback;

float K_limit_V;
float V_limit;
float K_limit_Wz;
float Wz_limit;

float theta_L_dot_last;
float theta_R_dot_last;
float theta_L_dot_dot;
float theta_R_dot_dot;

float Z_w_dd_L;
float Z_w_dd_R;

float Z_w_dd_L_last;
float Z_w_dd_R_last;

float Filter_MotionAccel_n_Z;
float MotionAccel_n_Z_last;

float K_Lowpass_fliter = 0.05f;
float Filter_Left_Leg_Angle_dot;
float Filter_Right_Leg_Angle_dot;
float Filter_Pitch_Speed_fdb;
float Filter_Volecity_Fdb;

float K_slope;//跳跃斜坡函数系数
float Jump_Leg_length;//跳跃蹬伸腿长度等级

bool is_moving ;//运动状态 用于期望位移
bool was_moving;//运动状态 用于期望位移

float Spin_x_V;//小陀螺移动X方向速度
float Spin_y_V;//小陀螺移动Y方向速度
float Spin_Forward_V;//小陀螺移动运动合成方向速度
float offset_Comp = -45.0f;//小陀螺移动 偏移相位补偿 单位°

float Left_Wheel_RPM;
float Right_Wheel_RPM;

float Super_Cap_Pmax;
float Chassis_Power_MAX;
float Inverse_V_Limit;
float Inverse_Wz_Limit;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KalmanFilter_t L_vaEstimateKF;	   // 卡尔曼滤波器结构体-左腿 
KalmanFilter_t R_vaEstimateKF;	   // 卡尔曼滤波器结构体-右腿
float L_vaEstimateKF_F[4] = {1.0f, 0.002f, 0.0f, 1.0f};	   // 状态转移矩阵，控制周期为0.002s
float L_vaEstimateKF_P[4] = {1.0f, 0.0f, 0.0f, 1.0f};      // 后验估计协方差初始值
const float L_vaEstimateKF_H[4] = {1.0f, 0.0f, 0.0f, 1.0f};// 设置矩阵H为常量

float R_vaEstimateKF_F[4] = {1.0f, 0.002f, 0.0f, 1.0f};	   // 状态转移矩阵，控制周期为0.002s
float R_vaEstimateKF_P[4] = {1.0f, 0.0f, 0.0f, 1.0f};      // 后验估计协方差初始值
const float R_vaEstimateKF_H[4] = {1.0f, 0.0f, 0.0f, 1.0f};// 设置矩阵H为常量
float L_vaEstimateKF_K[4];
float R_vaEstimateKF_K[4];

float L_vaEstimateKF_Q[4] = {0.05f, 0.0f, 0.0f, 0.01f};     // Q矩阵初始值
float L_vaEstimateKF_R[4] = {1000.0f,  0.0f,   0.0f, 500000.0f}; 	  
								 
float R_vaEstimateKF_Q[4] = {0.05f, 0.0f, 0.0f, 0.01f};     // Q矩阵初始值
float R_vaEstimateKF_R[4] = {1000.0f,  0.0f, 0.0f, 500000.0f};  	
						 
float vel_acc[2]; 
	
float V_L;							 
float V_R;	
float V_ave;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Gravity_Comp = 90.0f;

float Fn_r=0.0f;
float Fn_l=0.0f;
float Detect_Force = 50.0f;//离地检测支持力阈值 90N - 50N = 40N //较稳定阈值50.0f

float Soft_landing_Comp_F = 120.0f;//离地检测落地缓冲力(恒力):向下缓冲力:100N - 50N = 50N
float Soft_landing_Jump_F = 0.0f;

float Spring_comp_left;
float Spring_comp_right;
	
static const SpringMapPoint_t spring_table_L[] = 
{
    {0.14f, 100.0f},
    {0.16f, 105.0f},
    {0.18f, 110.0f},
    {0.20f, 120.0f},
    {0.22f, 130.0f},
    {0.24f, 140.0f},
	{0.26f, 150.0f},
	{0.28f, 152.0f},
	{0.30f, 155.0f},
	{0.32f, 158.0f},
	{0.34f, 160.0f},
	{0.36f, 170.0f},
	{0.38f, 180.0f}
};

static const SpringMapPoint_t spring_table_R[] = 
{
    {0.14f, 90.0f},
    {0.16f, 95.0f},
    {0.18f, 100.0f},
    {0.20f, 105.0f},
    {0.22f, 110.0f},
    {0.24f, 120.0f},
	{0.26f, 130.0f},
	{0.28f, 140.0f},
	{0.30f, 150.0f},
	{0.32f, 150.0f},
	{0.34f, 153.0f},
	{0.36f, 156.0f},
	{0.38f, 159.0f}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float LQR_K_L[12]={ 
 -6.999522, -0.279220, -0.000103, -0.003176, 0.706350, 0.346893,
    24.403412, 1.003978, 0.008156, 0.243378, 0.119447, -0.044184
};

float LQR_K_R[12]={ 
 -6.999522, -0.279220, -0.000103, -0.003176, 0.706350, 0.346893,
    24.403412, 1.003978, 0.008156, 0.243378, 0.119447, -0.044184
};



//三次多项式拟合系数
//Q=diag([20000 1500  250  100  100000  1000]);
//R=diag([120 4]);
const float Poly_Coefficient[12][4]={
{-204.71377921,	289.63818168,	-157.59015500,	-4.96782823},
	{-7.82854075,	19.15680744,	-17.41989049,	-0.23003324},
	{-7.53928256,	12.07032519,	-6.74694158,	-0.05542880},
	{-13.69695924,	22.38839045,	-12.69903703,	-0.34124181},
	{202.15935394,	-119.52253547,	-28.73024512,	30.05658725},
	{8.53035961,	0.97505447,	-8.73026755,	4.45533442},
	{1451.56173722,	-1146.30857312,	122.25511452,	96.79758089},
	{170.43065142,	-127.63122938,	1.84610137,	19.79612454},
	{55.36361918,	-32.73259440,	-7.86810167,	8.23133542},
	{36.92907744,	7.73400354,	-41.51630122,	19.92978630},
	{825.88702496,	-1322.23787701,	739.09041989,	6.07192070},
	{120.04567253,	-175.60627295,	92.90386168,	-0.44963915}
};	
//Q=diag([16000 1000  250  100  70000  1000]);//稳定飞坡
//R=diag([120  4]);
//{-194.60677435,	273.62859437,	-147.13062554,	-3.88197971},
//	{-8.60141767,	17.84999113,	-15.67765586,	-0.09330932},
//	{-9.25917120,	13.83683591,	-7.33730191,	-0.00327915},
//	{-16.54951929,	24.83798532,	-13.26699568,	-0.22182222},
//	{159.74455554,	-88.95772191,	-29.41926148,	25.71144540},
//	{10.63399606,	-0.78762825,	-8.24796200,	4.32059163},
//	{1227.24811650,	-961.37647469,	84.81891267,	88.59964683},
//	{142.13449851,	-106.90966198,	1.45028909,	16.42577893},
//	{52.28867983,	-29.11824959,	-9.62971376,	8.41604605},
//	{37.22580179,	8.53006576,	-41.46724631,	19.18267082},
//	{848.61705773,	-1268.16695917,	672.47482771,	0.30053912},
//	{136.72164908,	-193.73852805,	99.19459646,	-1.35944249}
//};
//Q=diag([4000  1500  600  5000  100000  500]);//综合运动稳定 联盟赛上场版
//R=diag([150  8]);
//{-197.43219224,	266.63298381,	-160.92203597,	-5.10368469},
//	{-4.33645434,	8.01836224,	-17.38731784,	-0.39073755},
//	{-14.66363299,	19.27229539,	-9.39865149,	-0.18822697},
//	{-46.13978083,	61.19889470,	-30.73325206,	-0.78456643},
//	{63.46162821,	-3.85043971,	-50.34311831,	27.96090924},
//	{-6.27551753,	12.92321649,	-10.31015121,	3.90127193},
//	{903.57187225,	-773.92952524,	124.42142192,	70.81869022},
//	{80.69253169,	-68.67562484,	9.34974703,	14.06504254},
//	{21.28567720,	-1.29147674,	-16.88559521,	9.37837406},
//	{53.49112800,	12.37690907,	-61.51698790,	32.15005610},
//	{819.72200429,	-1077.35406413,	525.40059078,	10.52220736},
//	{109.55452979,	-136.92144593,	63.94320377,	-1.13543702}
//};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////						
/**
 * @brief 底盘任务
 */
void Chassis_Task(void *argument)
{
	osDelay(100);
	/*===| 底盘初始化  |===*/
	Chassis_Init();
	
	/*===| 卡尔曼滤波初始化  |===*/
	xvEstimateKF_Init(&L_vaEstimateKF,&R_vaEstimateKF);
	
	//氮气弹簧动态补偿初始化
	GasSpring_Init(&Chassis_Control_Struct.GasSpring_L, spring_table_L, (sizeof(spring_table_L)/sizeof(SpringMapPoint_t)), 0.14f, 0.38f);
	GasSpring_Init(&Chassis_Control_Struct.GasSpring_R, spring_table_R, (sizeof(spring_table_R)/sizeof(SpringMapPoint_t)), 0.14f, 0.38f);
	
	/*===| 倾倒检测初始化  |===*/
//	Simple_TipOver_Init();
	
    for(;;)
    {
		Chassis_dt = DWT_GetDeltaT(&Tick);

		//获取控制参数和反馈参数
		Get_Control_Data();
		
		//算位移和速度
		Calculated_displacement();
		
		//卡尔曼滤波获取滤波后数据	
		xvEstimateKF_Update(&L_vaEstimateKF,-INS.MotionAccel_n[1],Chassis_Control_Struct.Volecity_L);
		xvEstimateKF_Update(&R_vaEstimateKF,-INS.MotionAccel_n[1],Chassis_Control_Struct.Volecity_R);
		V_L = L_vaEstimateKF.FilteredValue[0];//得到卡尔曼滤波后左轮的速度
		V_R = R_vaEstimateKF.FilteredValue[0];//得到卡尔曼滤波后右轮的速度
		V_ave = (V_L + V_R)/2.0f;
		Chassis_Control_Struct.Volecity_Fdb = V_ave;
		
		//根据关节电机角度算虚拟腿长、虚拟角度、虚拟角速度、雅可比矩阵
		calculateValues_Q_l(&Chassis_Control_Struct);
		calculateValues_Q_r(&Chassis_Control_Struct);
		
		//状态量滤波
		State_Filter();
		
		//LQR控制器状态量
		add_six_state(&Chassis_Control_Struct);
		
		//向前摆角度增大 后摆角度减小 范围0~360°
		Left_angle  = fmodf(Chassis_Control_Struct.Left_Leg_TotalAngle ,360.0f);
		Right_angle = fmodf(Chassis_Control_Struct.Right_Leg_TotalAngle,360.0f);
		
		//缓起立操作
		if(Chassis_Control_Struct.Smooth_restand_Flag == 1)
		{
			Smooth_Restand_Func();
		}
		
		//LQR计算运动控制参数
		if(Chassis_Control_Struct.Allow_to_Stand_Flag == 1)
		{
			calucateK(Poly_Coefficient);
			calucateLQR_L(LQR_K_L);
			calucateLQR_R(LQR_K_R);
		}

		//Roll轴补偿
		PID_Position_Calculate(&Chassis_Control_Struct.Roll_Leg_PID ,Target_Roll_angle, INS_Data_Self.Roll);//右边翘起增大
//		PID_Position_Calculate(&Chassis_Control_Struct.Roll_Leg_Speed_PID ,Chassis_Control_Struct.Roll_Leg_PID.Output, INS_Data_Self.Gyro[Y]);//右边翘起增大

		//防劈叉PID
		PID_Position_Calculate(&Chassis_Control_Struct.Tp_Comp_PID ,0.0f, (Chassis_Control_Struct.Left_Leg_Angle+Chassis_Control_Struct.Right_Leg_Angle));
		
		//转向PID
		if(Chassis_Control_Struct.Chassis_State != Chassis_OFF)
		{
			PID_Position_Calculate(&Chassis_Control_Struct.Turn_Speed_PID ,Chassis_Control_Struct.Chassis_Wz_Target, Chassis_Wz);
		}
		
		//限制轮电机转速 防轮疯转功率超限 补偿PID
//		Left_Wheel_RPM  = -Motor_Data_Struct[Chassis_DriverMotor1_ID].Speed_RPM / Motor_3508_Reduction;
//		Right_Wheel_RPM =  Motor_Data_Struct[Chassis_DriverMotor2_ID].Speed_RPM / Motor_3508_Reduction;
//		if(Left_Wheel_RPM > 400.0f)//速度2.5 m/s 时 对应轮转速大概为400 //前进转速为正
//			PID_Position_Calculate(&Chassis_Control_Struct.Limit_Left_Wheel_RPM_PID ,400.0f, Left_Wheel_RPM);
//		else if(Left_Wheel_RPM < -400.0f)
//			PID_Position_Calculate(&Chassis_Control_Struct.Limit_Left_Wheel_RPM_PID ,-400.0f, Left_Wheel_RPM);
//		
//		if(Right_Wheel_RPM > 400.0f)
//			PID_Position_Calculate(&Chassis_Control_Struct.Limit_Right_Wheel_RPM_PID,400.0f, Right_Wheel_RPM);
//		else if(Right_Wheel_RPM < -400.0f)
//			PID_Position_Calculate(&Chassis_Control_Struct.Limit_Right_Wheel_RPM_PID,-400.0f, Right_Wheel_RPM);
		
		//离地检测(跳跃 上台阶功能执行时不检测)
		//起立后才执行离地检测 倒地状态下不执行否则可能无法起立
		Chassis_Control_Struct.Balance_Flag = 1;
		
		if(Chassis_Control_Struct.Allow_to_Stand_Flag == 1)
		{
			if(Chassis_Control_Struct.Jump_Flag == 0 && Chassis_Control_Struct.UpStep_Dect_Flag == 0 && Chassis_Control_Struct.UpStep_Flag == 0)
			{
				Off_ground_Detection();
			}
		}
		
		//倾倒检测
//		Simple_TipOver_Detection();					//功能待测试！！！
		
		//跳跃功能控制 & 侧向惯性力矩前馈总和计算
		Jump_Control();
		
		//上台阶功能
		Up_Step_Dection();
		Up_Step_Func();
		
		//翻倒自起功能
		Recover_from_Ground();

		//算总力矩电流
		calucateTp(&Chassis_Control_Struct);
		Output_Current();
			
		if(RoboControl_Struct.If_SettingZero)
		{
			Motor_DM_CMD_SetZero(&Chassis_JointMotor_CAN,Chassis_JointMotor1_Send_CAN_ID);
			Motor_DM_CMD_SetZero(&Chassis_JointMotor_CAN,Chassis_JointMotor2_Send_CAN_ID);
			osDelay(1);
			Motor_DM_CMD_SetZero(&Chassis_JointMotor_CAN,Chassis_JointMotor3_Send_CAN_ID);
			Motor_DM_CMD_SetZero(&Chassis_JointMotor_CAN,Chassis_JointMotor4_Send_CAN_ID);
			osDelay(100);	//延时100ms防止设完零点进入运动控制疯车
			Buzzer_Set_SoundEffect(Buzzer_SoundEffect_SetZero_Done);
			
			Chassis_Control_Struct.Left_Leg_Angle_Round  = 0;
			Chassis_Control_Struct.Right_Leg_Angle_Round = 0;
			RoboControl_Struct.If_SettingZero = 0;
		}
		else
		{
			//电管底盘失能 关节电机也失能
			if( (Chassis_Control_Struct.Chassis_State == Chassis_OFF) || (Robo_State.power_management_chassis_output == 0) )
			{
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor1_Send_CAN_ID, 0, 0, 0, 0, 0);
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor2_Send_CAN_ID, 0, 0, 0, 0, 0);
					osDelay(1);
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor3_Send_CAN_ID, 0, 0, 0, 0, 0);
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor4_Send_CAN_ID, 0, 0, 0, 0, 0);
					Motor_DJI_SendCurrent(&Chassis_DriverMotor_CAN,Chassis_DriverMotor_Send_CAN_ID,0,0,0,0);
			}		
			else
			{	
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor2_Send_CAN_ID, 0, 0, 0, 0, Chassis_Control_Struct.JointTorque_Set[0]);	
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor1_Send_CAN_ID, 0, 0, 0, 0, Chassis_Control_Struct.JointTorque_Set[1]);
					osDelay(1);
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor4_Send_CAN_ID, 0, 0, 0, 0, Chassis_Control_Struct.JointTorque_Set[2]);
					Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN, Chassis_JointMotor3_Send_CAN_ID, 0, 0, 0, 0, Chassis_Control_Struct.JointTorque_Set[3]);
					Motor_DJI_SendCurrent(&Chassis_DriverMotor_CAN,Chassis_DriverMotor_Send_CAN_ID,0,0,Chassis_Control_Struct.TorqueCurrent_wheel_left,Chassis_Control_Struct.TorqueCurrent_wheel_right);
			}
		}
		osDelay(1);
   }
}



/**
 * @brief 计算位移和速度
 */
void Calculated_displacement(void)
{
	Chassis_Control_Struct.Left_displacement_last  = Chassis_Control_Struct.Left_displacement_now;
    Chassis_Control_Struct.Right_displacement_last = Chassis_Control_Struct.Right_displacement_now;
    Chassis_Control_Struct.Left_displacement_now  = Motor_Data_Struct[Chassis_DriverMotor1_ID].Total_Angle / Motor_3508_Reduction * Chassis_Wheel_Radius * PI / 180.0f;
    Chassis_Control_Struct.Right_displacement_now = Motor_Data_Struct[Chassis_DriverMotor2_ID].Total_Angle / Motor_3508_Reduction * Chassis_Wheel_Radius * PI / 180.0f;
    
    Chassis_Control_Struct.Left_displacement = -(Chassis_Control_Struct.Left_displacement_now - Chassis_Control_Struct.Left_displacement_Init);
    Chassis_Control_Struct.Right_displacement=  Chassis_Control_Struct.Right_displacement_now - Chassis_Control_Struct.Right_displacement_Init;
	Chassis_Control_Struct.displacement = (Chassis_Control_Struct.Left_displacement + Chassis_Control_Struct.Right_displacement)/2.0f;
	
	Chassis_Control_Struct.Volecity_Fdb_last = Chassis_Control_Struct.Volecity_Fdb;
	Chassis_Control_Struct.Volecity_L = -Motor_Data_Struct[Chassis_DriverMotor1_ID].Speed_RPM * Chassis_RPM_to_m_s;
	Chassis_Control_Struct.Volecity_R =  Motor_Data_Struct[Chassis_DriverMotor2_ID].Speed_RPM * Chassis_RPM_to_m_s;
	Chassis_Control_Struct.Volecity_Fdb = (Chassis_Control_Struct.Volecity_L + Chassis_Control_Struct.Volecity_R)/2.0f;
}


/**
 * @brief 底盘初始化
 */
void Chassis_Init(void)
{
	//腿摆角位置环PID
	PID_Init(&Chassis_Control_Struct.Left_leg_Angle_PID,  100.0f, 0.0f, 2000.0f, 0.0f,0.0f,60.0f);
	PID_Init(&Chassis_Control_Struct.Right_leg_Angle_PID, 100.0f, 0.0f, 2550.0f, 0.0f,0.0f,60.0f);
	
	//腿摆角速度环PID
	PID_Init(&Chassis_Control_Struct.Left_leg_AngleSpeed_PID,  0.3f,0.005f,0.0f,0.0f,10.0f,30.0f);
	PID_Init(&Chassis_Control_Struct.Right_leg_AngleSpeed_PID, 0.3f,0.005f,0.0f,0.0f,10.0f,30.0f);
	
	//腿长位置环PID
	PID_Init(&Chassis_Control_Struct.Left_leg_Length_PID, 20.0f, 0.0f, 0.0f, 0.0f,0.0f,0.40f);
	PID_Init(&Chassis_Control_Struct.Right_leg_Length_PID,20.0f, 0.0f, 0.0f, 0.0f,0.0f,0.40f);
	//腿长速度环PID
	PID_Init(&Chassis_Control_Struct.Left_leg_Length_Speed_PID, 200.0f, 0.0f, 0.0f, 200.0f,20.0f,50.0f);//Ki=10.0f
	PID_Init(&Chassis_Control_Struct.Right_leg_Length_Speed_PID,200.0f, 0.0f, 0.0f, 200.0f,20.0f,50.0f);
	//跳跃收腿双环PID
	PID_Init(&Chassis_Control_Struct.Fold_left_Leg_Position_PID, 40.0f, 0.0f, 0.0f, 0.0f,0.0f,1.20f);
	PID_Init(&Chassis_Control_Struct.Fold_right_Leg_Position_PID,40.0f, 0.0f, 0.0f, 0.0f,0.0f,1.20f);
	PID_Init(&Chassis_Control_Struct.Fold_left_Leg_Speed_PID, 400.0f, 0.0f, 0.0f, 200.0f,20.0f,80.0f);
	PID_Init(&Chassis_Control_Struct.Fold_right_Leg_Speed_PID,400.0f, 0.0f, 0.0f, 200.0f,20.0f,80.0f);
	
	//Roll轴横滚角补偿PID
	PID_Init(&Chassis_Control_Struct.Roll_Leg_PID, 60.0f, 0.0f, 250.0f, 0.0f, 0.0f, 80.0f);//kd=5.0f
//	PID_Init(&Chassis_Control_Struct.Roll_Leg_PID, 30.0f, 0.0f, 10.0f, 0.0f, 0.0f, 120.0f);//Roll轴角速度°/s
//	PID_Init(&Chassis_Control_Struct.Roll_Leg_Speed_PID, 2.5f, 0.0f, 10.0f, 0.0f, 0.0f, 90.0f);
	
	//转向PID
	PID_Init(&Chassis_Control_Struct.Turn_Angle_PID, 0.05f, 0.0f, 0.0f, 0.0f,0.0f,40.0f);
	PID_Init(&Chassis_Control_Struct.Turn_Speed_PID, 3.0f,  0.0f, 0.1f, 0.0f,0.0f,3.0f);
	
	//防劈叉PID
	PID_Init(&Chassis_Control_Struct.Tp_Comp_PID, 4.0f, 0.0f, 0.1f, 0.0f,0.0f,10.0f);
	
	//限制轮电机转速 防轮疯转功率超限 补偿PID (此处输出值为最终发给电机的电流值)
	PID_Init(&Chassis_Control_Struct.Limit_Left_Wheel_RPM_PID,  300.0f, 0.0f, 0.0f, 0.0f,0.0f,16384.0f);
	PID_Init(&Chassis_Control_Struct.Limit_Right_Wheel_RPM_PID, 300.0f, 0.0f, 0.0f, 0.0f,0.0f,16384.0f);
	
	//轮电机位置初始化
	Chassis_Control_Struct.Left_displacement_Init  = Motor_Data_Struct[Chassis_DriverMotor1_ID].Total_Angle / Motor_3508_Reduction * Chassis_Wheel_Radius * PI / 180.0f;
    Chassis_Control_Struct.Right_displacement_Init = Motor_Data_Struct[Chassis_DriverMotor2_ID].Total_Angle / Motor_3508_Reduction * Chassis_Wheel_Radius * PI / 180.0f;
		
	//先发一帧
	osDelay(1);
	Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN,Chassis_JointMotor1_Send_CAN_ID,0,0,0,0,0);
	osDelay(1);
	Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN,Chassis_JointMotor2_Send_CAN_ID,0,0,0,0,0);
	osDelay(1);
	Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN,Chassis_JointMotor3_Send_CAN_ID,0,0,0,0,0);
	osDelay(1);
	Motor_DM_CMD_MIT(&Chassis_JointMotor_CAN,Chassis_JointMotor4_Send_CAN_ID,0,0,0,0,0);

	Chassis_Control_Struct.Chassis_State = Chassis_OFF;
	RoboControl_Struct.Leg_Length=0.15f;
    RoboControl_Struct.Leg_Angle =0.0f;
	
	Chassis_Control_Struct.Jump_Level = 1;
}


void Output_Current(void)
{	
	if(Chassis_Control_Struct.Chassis_State != Chassis_OFF)
	{
		Chassis_Control_Struct.JointTorque_Set[1] = TP_wl_wr_jl_jr[0] * K_gear2chain;//对应1号电机
		Chassis_Control_Struct.JointTorque_Set[0] = TP_wl_wr_jl_jr[1] * K_gear2chain;//对应2号电机
		Chassis_Control_Struct.JointTorque_Set[3] = TP_wl_wr_jl_jr[2] * K_gear2chain;//对应3号电机
		Chassis_Control_Struct.JointTorque_Set[2] = TP_wl_wr_jl_jr[3] * K_gear2chain;//对应4号电机
	}
	else
	{
		Chassis_Control_Struct.JointTorque_Set[1] = 0;
		Chassis_Control_Struct.JointTorque_Set[0] = 0;
		Chassis_Control_Struct.JointTorque_Set[3] = 0;
		Chassis_Control_Struct.JointTorque_Set[2] = 0;	
	}
	
	if(Chassis_Control_Struct.Chassis_State != Chassis_OFF)
	{
//		if(fabsf(Left_Wheel_RPM) > 400.0f && Chassis_Control_Struct.Chassis_State != Chassis_SPIN)
//			Chassis_Control_Struct.TorqueCurrent_wheel_left  = TP_wl_wr_jl_jr[4]*Torque2Current - Chassis_Control_Struct.Limit_Left_Wheel_RPM_PID.Output;
//		else
//			Chassis_Control_Struct.TorqueCurrent_wheel_left  = TP_wl_wr_jl_jr[4]*Torque2Current;
//		if(fabsf(Right_Wheel_RPM) > 400.0f && Chassis_Control_Struct.Chassis_State != Chassis_SPIN)
//			Chassis_Control_Struct.TorqueCurrent_wheel_right = TP_wl_wr_jl_jr[5]*Torque2Current  + Chassis_Control_Struct.Limit_Right_Wheel_RPM_PID.Output;
//		else
//			Chassis_Control_Struct.TorqueCurrent_wheel_right = TP_wl_wr_jl_jr[5]*Torque2Current ;
		
		Chassis_Control_Struct.TorqueCurrent_wheel_left  = TP_wl_wr_jl_jr[4]*Torque2Current;
		Chassis_Control_Struct.TorqueCurrent_wheel_right = TP_wl_wr_jl_jr[5]*Torque2Current ;
	}
	else
	{
		Chassis_Control_Struct.TorqueCurrent_wheel_left  = 0;
		Chassis_Control_Struct.TorqueCurrent_wheel_right = 0;
	}
}


void Get_Control_Data(void)
{
	Chassis_Control_Struct.Chassis_State = RoboControl_Struct.Chassis_State;
	
	Chassis_Control_Struct.Ave_Leg_Length = (Chassis_Control_Struct.Left_Leg_Length + Chassis_Control_Struct.Right_Leg_Length)/2.0f;
	
	/*===| 底盘运动遥控数据 |===*/
	Chassis_Wz = INS_Data_Self.Yaw_Speed;
	Target_Yaw_angle = RoboControl_Struct.Robo_Target_Chassis_Yaw;
	
	/*===| 底盘调头切换正方向 |===*/
	if(Chassis_Control_Struct.Chassis_Forward_Change_Flag == 1)
	{
		RoboControl_Struct.Target_Direction = 90.0f;
		RoboControl_Struct.Target_Direction_Err = Caculate_Included_Angle(90.0f,RoboControl_Struct.Yaw_Err);
		
		RoboControl_Struct.Robo_Target_Vy = -RoboControl_Struct.Robo_Target_Vy;
	}
	else if(Chassis_Control_Struct.Chassis_Forward_Change_Flag == 2)
	{
		RoboControl_Struct.Target_Direction = 90.0f;
		RoboControl_Struct.Target_Direction_Err = RoboControl_Struct.Yaw_Err;
	}
	else
	{
		RoboControl_Struct.Target_Direction = 0.0f;
		RoboControl_Struct.Target_Direction_Err = RoboControl_Struct.Yaw_Err;
	}
		
	// 底盘静止单舵转向解算
	if(RoboControl_Struct.Chassis_State == Chassis_STATIC)
	{
		if(Chassis_Control_Struct.Chassis_Forward_Change_Flag == 0)
		{
			RoboControl_Struct.Target_Direction_Err = RoboControl_Struct.Yaw_Err;
		}
		else if(Chassis_Control_Struct.Chassis_Forward_Change_Flag == 1)
		{
			RoboControl_Struct.Target_Direction_Err = Caculate_Included_Angle(90.0f,RoboControl_Struct.Yaw_Err);
		}

		Composite_V = sqrtf((RoboControl_Struct.Robo_Target_Vx*RoboControl_Struct.Robo_Target_Vx)+(RoboControl_Struct.Robo_Target_Vy*RoboControl_Struct.Robo_Target_Vy));
		Limit_float(&Composite_V, 2.2f, 0.0f);
		if(RoboControl_Struct.Controler == Joystick)
		{
			Steer_Angle_target = atan2f(Remote.Left_X, Remote.Left_Y) * Radain2Angle ;//+ RoboControl_Struct.Yaw_Err;//右上为+45°
		}
		else if(RoboControl_Struct.Controler == KeyboardMouse)
		{
			Steer_Angle_target = atan2f((Remote.Keyboard_D - Remote.Keyboard_A), (Remote.Keyboard_W - Remote.Keyboard_S)) * Radain2Angle ;
		}
		
		// 判断是否超出 [-90, 90] 的半圆范围
		if (fabsf(Steer_Angle_target) > 90.0f) 
		{
			Composite_V = -Composite_V; // 超出范围，速度取反
		}
		
		// 将角度强行“折叠”回 [-90, 90] 区间 
		if(Steer_Angle_target > 90.0f) 
		{
			Steer_Angle_target -= 180.0f;
		}
		if(Steer_Angle_target < -90.0f) 
		{
			Steer_Angle_target += 180.0f;
		}
		
		if(Chassis_Control_Struct.Chassis_Forward_Change_Flag == 0)
		{
			RoboControl_Struct.Target_Direction = Caculate_Included_Angle(Steer_Total_angle_Target, Steer_Angle_target);
			//先转向再前进
			if(fabsf(Caculate_Included_Angle(-RoboControl_Struct.Target_Direction, RoboControl_Struct.Target_Direction_Err)) < 5.0f)
			{
				RoboControl_Struct.Robo_Target_Vy = Composite_V;
			}
		}
		else if(Chassis_Control_Struct.Chassis_Forward_Change_Flag == 1)
		{
			RoboControl_Struct.Target_Direction = -90.0f + Caculate_Included_Angle(Steer_Total_angle_Target, Steer_Angle_target);
			//先转向再前进
			if(fabsf(Caculate_Included_Angle(-RoboControl_Struct.Target_Direction, RoboControl_Struct.Target_Direction_Err)) < 5.0f)
			{
				RoboControl_Struct.Robo_Target_Vy = -Composite_V;
			}
		}
	}
	
	Acc_Slow(RoboControl_Struct.Robo_Target_Vy, &Chassis_Control_Struct.Chassis_Vy_Set, Chassis_Acc, Chassis_Dec, Chassis_dt);//底盘速度缓起急停
//	Chassis_Control_Struct.Chassis_Vy_Set = RoboControl_Struct.Robo_Target_Vy;
	
	if(Chassis_Control_Struct.Chassis_State == Chassis_SPIN)//限制小陀螺的进入状态 速度不可过快 防止翻车
	{
		if(fabsf(Chassis_Control_Struct.Volecity_Fdb) < 2.2f)
			Chassis_Control_Struct.Chassis_Wz_Set = RoboControl_Struct.Robo_Target_Wz;
	}
	else
	{
		Chassis_Control_Struct.Chassis_Wz_Set = RoboControl_Struct.Robo_Target_Wz;
	}
	
	Chassis_Control_Struct.Chassis_Wz_Target = Chassis_Control_Struct.Chassis_Wz_Set;
	Chassis_Control_Struct.Chassis_Vy_Target_last = Chassis_Control_Struct.Chassis_Vy_Target;
	Chassis_Control_Struct.Chassis_Vy_Target = Chassis_Control_Struct.Chassis_Vy_Set;
	
	if(RoboControl_Struct.SuperCap_State == 1) //控目标
	{
		if(RoboControl_Struct.SuperCap_V >= 8 && RoboControl_Struct.SuperCap_V <= 14)
			Chassis_Control_Struct.Chassis_Vy_Target*= (RoboControl_Struct.SuperCap_V-8) / 6.0f; 
		
		else if(RoboControl_Struct.SuperCap_V < 8) 
			Chassis_Control_Struct.Chassis_Vy_Target = 0;
	}	
	
	// 小陀螺平移解算
	if(Chassis_Control_Struct.Chassis_State == Chassis_SPIN)
	{			
		if(RoboControl_Struct.SPIN_Direction_Flag == 0 && Chassis_Control_Struct.Chassis_Forward_Change_Flag == 0)
		{
			Spin_y_V = RoboControl_Struct.Robo_Target_Vy*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp) * Angle2Radain);
			Spin_x_V = RoboControl_Struct.Robo_Target_Vx*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp+90.0f) * Angle2Radain);
		}
		else if(RoboControl_Struct.SPIN_Direction_Flag == 0 && Chassis_Control_Struct.Chassis_Forward_Change_Flag == 1)
		{
			Spin_y_V = -RoboControl_Struct.Robo_Target_Vy*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp) * Angle2Radain);
			Spin_x_V = RoboControl_Struct.Robo_Target_Vx*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp+90.0f) * Angle2Radain);
		}
		else if(RoboControl_Struct.SPIN_Direction_Flag == 1 && Chassis_Control_Struct.Chassis_Forward_Change_Flag == 0)
		{
			Spin_y_V = -RoboControl_Struct.Robo_Target_Vy*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp+90.0f) * Angle2Radain);
			Spin_x_V = RoboControl_Struct.Robo_Target_Vx*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp) * Angle2Radain);
		}
		else if(RoboControl_Struct.SPIN_Direction_Flag == 1 && Chassis_Control_Struct.Chassis_Forward_Change_Flag == 1)
		{
			Spin_y_V = RoboControl_Struct.Robo_Target_Vy*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp+90.0f) * Angle2Radain);
			Spin_x_V = RoboControl_Struct.Robo_Target_Vx*arm_sin_f32((RoboControl_Struct.Yaw_Err+offset_Comp) * Angle2Radain);
		}
	
		Chassis_Control_Struct.Chassis_Vy_Target = Spin_y_V + Spin_x_V ;
	}
	
	//最大速度限幅(考虑轮电机性能 速度过高无法达到 控制无法收敛)
	if(Chassis_Control_Struct.Chassis_State != Chassis_OFF)//根据腿长限制最高速度 防止机体控制发散
	{
		K_limit_V = (2.3f - 1.3f) / (0.36f - 0.22f);
		
		if(Chassis_Control_Struct.Ave_Leg_Length > 0.22f) 
		{
			if(Chassis_Control_Struct.UpStep_Dect_Flag == 1)
			{
				V_limit = 2.3f;//高腿情况下解除安全限速 使其能在短距离内能加速磕台阶
			}
			else
			{
				V_limit = 2.3f - K_limit_V * (Chassis_Control_Struct.Ave_Leg_Length - 0.22f);// 快速线性递减
			}
		} 
		else
		{
			V_limit = 2.3f;// 短腿保持高速
		} 
		
		if(Chassis_Control_Struct.Chassis_State == Chassis_SPIN)//根据腿长限制小陀螺最高速度 防止机体控制发散
		{
			K_limit_Wz = (Robo_Wz_MaxSpeed - 5.0f) / (0.36f-0.23f);
			
			if(Chassis_Control_Struct.Ave_Leg_Length > 0.23f)
			{
				Wz_limit = Robo_Wz_MaxSpeed - K_limit_Wz * (Chassis_Control_Struct.Ave_Leg_Length - 0.23f);
			} 
			else 
			{
				Wz_limit = Robo_Wz_MaxSpeed;// 短腿保持高转速
			} 
		}
		else
		{
			Wz_limit = RoboWz_PID_Output_MAX;
		}
		
		/*===| 功率控制 控目标 |===*/
		if(RoboControl_Struct.SuperCap_State == 1)
		{
			Super_Cap_Pmax = 15.0f * RoboControl_Struct.SuperCap_V;//超电此时能输出的最大功率 额定电流 15A * SuperCap_V
			Limit_float(&Super_Cap_Pmax,360.0f,0.0f);
			 
//			限目标思路：根据 (超电实时能提供最大功率 + 裁判系统当前的功率上限) 反解出当前能输出的最大目标速度与转速 基本与功率成线性关系
//			Predict_Chassis_Power_V  = 62.41f*fabsf(Chassis_Control_Struct.Volecity_Fdb) + (-3.40f);
//			Predict_Chassis_Power_Wz =  9.56f*fabsf(Chassis_Wz) + (1.03f);
			
			Chassis_Power_MAX = Super_Cap_Pmax + Robo_State.chassis_power_limit;
		
			Inverse_V_Limit  = (Chassis_Power_MAX - (-3.40f)) / 62.41f;
			Inverse_Wz_Limit = (Chassis_Power_MAX - (1.03f))  / 9.56f;
			
			if(Inverse_V_Limit < V_limit)
			{
				V_limit  = Inverse_V_Limit;
			}
			if(Inverse_Wz_Limit < Wz_limit)
			{
				Wz_limit = Inverse_Wz_Limit;
			}
		}
		Limit_float(&Chassis_Control_Struct.Chassis_Vy_Target,V_limit ,-V_limit);
		Limit_float(&Chassis_Control_Struct.Chassis_Wz_Target,Wz_limit,-Wz_limit);
	}

	//位移
	// 判断当前和上一次的速度是否“有效”（非零）
	// 判断当前和上一次的速度是否“有效”（非零）
	is_moving  = fabsf(Chassis_Control_Struct.Chassis_Vy_Target)      > 0.05f;
	was_moving = fabsf(Chassis_Control_Struct.Chassis_Vy_Target_last) > 0.05f;

	// 状态一：刹车瞬间 (从运动变为静止)
	if (was_moving && !is_moving) 
	{
		// 【关键修复】直接把“当前实际位移”作为目标，死锁住，实现真正的“稳在原地”
//		Chassis_Control_Struct.Target_displacement = Chassis_Control_Struct.displacement;
		// 【关键修复】清零积分器，为下次起步做准备
		Chassis_Control_Struct.add_displacement = 0.0f; 
	}
	// 状态二：起步瞬间 (从静止变为运动)
	else if (!was_moving && is_moving) 
	{
		// 记录起点的实际位移
		Chassis_Control_Struct.now_displacement = Chassis_Control_Struct.Target_displacement;
		// 【关键修复】清零积分器，防止历史脏数据导致瞬间跳跃
		Chassis_Control_Struct.add_displacement = 0.0f;
	}
	// 状态三：持续运动中 (不管之前是什么状态，只要现在有速度就执行积分)
	if (is_moving) 
	{
		// 正常积分
		Chassis_Control_Struct.add_displacement += Chassis_Control_Struct.Chassis_Vy_Target * Chassis_dt;
		// 计算目标位移 = 起点位移 + 积分出来的位移增量
		Chassis_Control_Struct.Target_displacement = Chassis_Control_Struct.now_displacement + Chassis_Control_Struct.add_displacement;
	}
	// 状态四：持续静止 (什么都不做，Target_displacement保持在上次刹车时锁死的值)

	
	//陀螺仪
	Chassis_Control_Struct.Roll_Feedback = INS_Data_Self.Roll * Angle2Radain;
	Chassis_Control_Struct.Yaw_Feedback  = INS_Data_Self.YawTotalAngle * Angle2Radain;
	Chassis_Control_Struct.Pitch_Angle_Last = Chassis_Control_Struct.Pitch_Angle;		//反馈上一时刻的pitch
	Chassis_Control_Struct.Pitch_Angle = INS_Data_Self.Pitch * Angle2Radain;			//反馈当前时刻的pitch
	Chassis_Control_Struct.Pitch_Speed_fdb_last = Chassis_Control_Struct.Pitch_Speed_fdb;
	Chassis_Control_Struct.Pitch_Speed_fdb = (Chassis_Control_Struct.Pitch_Angle - Chassis_Control_Struct.Pitch_Angle_Last)/Chassis_dt;//pitch一阶导
	
	//状态量更新
	Chassis_Control_Struct.state_lqr[0] = (Chassis_Control_Struct.displacement - Chassis_Control_Struct.Target_displacement);
	Chassis_Control_Struct.state_lqr[1] = (Chassis_Control_Struct.Volecity_Fdb - Chassis_Control_Struct.Chassis_Vy_Target);		//目标速度-当前速度
	Chassis_Control_Struct.state_lqr[4] = Chassis_Control_Struct.Pitch_Angle-2.5f*Angle2Radain; //pitch角
	Chassis_Control_Struct.state_lqr[5] = Chassis_Control_Struct.Pitch_Speed_fdb;				//pitch一阶导
	
	//关节电机角度
	Joint_Motor_Angle[0]=(-Motor_Data_Struct[1].Angle)*K_gear2chain;
	Joint_Motor_Angle[1]=( Motor_Data_Struct[2].Angle)*K_gear2chain;
	Joint_Motor_Angle[2]=( Motor_Data_Struct[3].Angle)*K_gear2chain;
	Joint_Motor_Angle[3]=(-Motor_Data_Struct[4].Angle)*K_gear2chain;
	
	Joint_Motor_Total_Angle[0]=(-Motor_Data_Struct[1].Total_Angle)*K_gear2chain;
	Joint_Motor_Total_Angle[1]=( Motor_Data_Struct[2].Total_Angle)*K_gear2chain;
	Joint_Motor_Total_Angle[2]=( Motor_Data_Struct[3].Total_Angle)*K_gear2chain;
	Joint_Motor_Total_Angle[3]=(-Motor_Data_Struct[4].Total_Angle)*K_gear2chain;
}


void calculateValues_Q_l(Chassis_Control_StructTypeDef *leg_yak2)
{
//哈工程正运动学解算
	float Xb,Yb;
	float Xd,Yd;
	float Xc,Yc;
	
	float A0,B0,C0,lBD;
	float PHI0,PHI1,PHI2,PHI3,PHI4; //弧度制
	float l5=0;
	float phi1,phi2,phi3,phi4;		//角度制
	float L0;
	float phi0;
	
	phi1=180.0-Joint_Motor_Total_Angle[0];
	phi4=Joint_Motor_Total_Angle[1];

	PHI1=phi1*Angle2Radain;
	PHI4=phi4*Angle2Radain;
	
	Xb=L1u*arm_cos_f32(PHI1);
	Yb=L1u*arm_sin_f32(PHI1);
	Xd=l5+L1u*arm_cos_f32(PHI4);
	Yd=L1u*arm_sin_f32(PHI4);
	
	lBD=sqrtf((Xd-Xb)*(Xd-Xb)+(Yd-Yb)*(Yd-Yb));
	A0=2*L1d*(Xd-Xb);
	B0=2*L1d*(Yd-Yb);
	C0=L1d*L1d+lBD*lBD-L1d*L1d;
	
	PHI2=2*atan2f((B0+sqrtf(A0*A0+B0*B0-C0*C0)),(A0+C0));
	PHI3=atan2f(Yb-Yd+L1d*arm_sin_f32(PHI2),Xb-Xd+L1d*arm_cos_f32(PHI2));
	
	Xc=Xb+L1d*arm_cos_f32(PHI2);
	Yc=Yb+L1d*arm_sin_f32(PHI2);

	L0=sqrtf((Xc-l5/2)*(Xc-l5/2)+Yc*Yc);
	PHI0=atan2f(Yc,(Xc-l5/2));
	phi0=PHI0*Radain2Angle;
	
	leg_yak2->Left_Leg_Length_last = leg_yak2->Left_Leg_Length;
	leg_yak2->Left_Leg_Length = L0;
	leg_yak2->Left_Leg_Length_dot_last = leg_yak2->Left_Leg_Length_dot;
	leg_yak2->Left_Leg_Length_dot = (leg_yak2->Left_Leg_Length - leg_yak2->Left_Leg_Length_last) / Chassis_dt;
	leg_yak2->Left_Leg_Length_dot_dot = (leg_yak2->Left_Leg_Length_dot - leg_yak2->Left_Leg_Length_dot_last) / Chassis_dt;
	
	
//	leg_yak2->Left_Leg_Angle_last = leg_yak2->Left_Leg_Angle;
	leg_yak2->Left_Leg_Angle = -Caculate_Included_Angle(90.0f,phi0);
	
	if(leg_yak2->Left_Leg_Angle - leg_yak2->Left_Leg_Angle_last > 180) leg_yak2->Left_Leg_Angle_Round--;
	else if(leg_yak2->Left_Leg_Angle - leg_yak2->Left_Leg_Angle_last < -180) leg_yak2->Left_Leg_Angle_Round++;
	
	leg_yak2->Left_Leg_TotalAngle_Last = leg_yak2->Left_Leg_TotalAngle;
	leg_yak2->Left_Leg_TotalAngle = 360.0f*leg_yak2->Left_Leg_Angle_Round + leg_yak2->Left_Leg_Angle;
	leg_yak2->Left_Leg_Angle_dot_last = leg_yak2->Left_Leg_Angle_dot;
	leg_yak2->Left_Leg_Angle_dot = (leg_yak2->Left_Leg_TotalAngle - leg_yak2->Left_Leg_TotalAngle_Last) / Chassis_dt;
	
/////////////////////////////////////////////
//哈工程VMC
	float d_phi1_L, d_phi4_L;
	float dxc_L   , dyc_L;

	float J[2][2] ={0};
	float Jt[2][2]={0};
	float JT[2][2]={0};
	
	d_phi1_L=Motor_Data_Struct[1].Speed_RPM *2.0f*PI/60.0f *K_gear2chain;//角速度
	d_phi4_L=Motor_Data_Struct[2].Speed_RPM *2.0f*PI/60.0f *K_gear2chain;//角速度

	/*===| 计算雅各比矩阵J |===*/
	J[0][0]= (L1u*arm_sin_f32(PHI1-PHI2)*arm_sin_f32(PHI3)) / (arm_sin_f32(PHI2-PHI3));
	J[0][1]= (L1u*arm_sin_f32(PHI3-PHI4)*arm_sin_f32(PHI2)) / (arm_sin_f32(PHI2-PHI3));
	J[1][0]=-(L1u*arm_sin_f32(PHI1-PHI2)*arm_cos_f32(PHI3)) / (arm_sin_f32(PHI2-PHI3));
	J[1][1]=-(L1u*arm_sin_f32(PHI3-PHI4)*arm_cos_f32(PHI2)) / (arm_sin_f32(PHI2-PHI3));
	/*===| 计算矩阵Jᵀ*R*M |===*/
	leg_yak2->Jt_l[0][0]=(L1u*arm_sin_f32(PHI0-PHI3)*arm_sin_f32(PHI1-PHI2)) / (arm_sin_f32(PHI3-PHI2));
	leg_yak2->Jt_l[0][1]=(L1u*arm_cos_f32(PHI0-PHI3)*arm_sin_f32(PHI1-PHI2)) / (L0*arm_sin_f32(PHI3-PHI2));
	leg_yak2->Jt_l[1][0]=(L1u*arm_sin_f32(PHI0-PHI2)*arm_sin_f32(PHI3-PHI4)) / (arm_sin_f32(PHI3-PHI2));
	leg_yak2->Jt_l[1][1]=(L1u*arm_cos_f32(PHI0-PHI2)*arm_sin_f32(PHI3-PHI4)) / (L0*arm_sin_f32(PHI3-PHI2));
	
	/*===| 速度映射部分 |===*/
	dxc_L=J[0][0]*d_phi1_L + J[0][1]*d_phi4_L;
	dyc_L=J[1][0]*d_phi1_L + J[1][1]*d_phi4_L;
}

void calculateValues_Q_r(Chassis_Control_StructTypeDef *leg_yak2)
{
    //哈工程正运动学解算
	float Xb,Yb;
	float Xd,Yd;
	float Xc,Yc;
	
	float A0,B0,C0,lBD;
	float PHI0,PHI1,PHI2,PHI3,PHI4; //弧度制
	float l5=0;
	float phi1,phi4,phi2,phi3;
	float L0;
	float phi0;
	
	phi1=180.0-Joint_Motor_Total_Angle[3];
	phi4=Joint_Motor_Total_Angle[2];

	PHI1=phi1*Angle2Radain;
	PHI4=phi4*Angle2Radain;
	
	Xb=L1u*arm_cos_f32(PHI1);
	Yb=L1u*arm_sin_f32(PHI1);
	Xd=l5+L1u*arm_cos_f32(PHI4);
	Yd=L1u*arm_sin_f32(PHI4);
	
	lBD=sqrtf((Xd-Xb)*(Xd-Xb)+(Yd-Yb)*(Yd-Yb));
	A0=2*L1d*(Xd-Xb);
	B0=2*L1d*(Yd-Yb);
	C0=L1d*L1d+lBD*lBD-L1d*L1d;
	
	PHI2=2*atan2f((B0+sqrtf(A0*A0+B0*B0-C0*C0)),(A0+C0));
	PHI3=atan2f(Yb-Yd+L1d*arm_sin_f32(PHI2),Xb-Xd+L1d*arm_cos_f32(PHI2));
	
	Xc=Xb+L1d*arm_cos_f32(PHI2);
	Yc=Yb+L1d*arm_sin_f32(PHI2);

	L0=sqrtf((Xc-l5/2)*(Xc-l5/2)+Yc*Yc);
	PHI0=atan2f(Yc,(Xc-l5/2));
	phi0=PHI0*Radain2Angle;
	
	leg_yak2->Right_Leg_Length_last = leg_yak2->Right_Leg_Length;
	leg_yak2->Right_Leg_Length = L0;
	leg_yak2->Right_Leg_Length_dot_last = leg_yak2->Right_Leg_Length_dot;
	leg_yak2->Right_Leg_Length_dot = (leg_yak2->Right_Leg_Length - leg_yak2->Right_Leg_Length_last) / Chassis_dt;
	leg_yak2->Right_Leg_Length_dot_dot = (leg_yak2->Right_Leg_Length_dot - leg_yak2->Right_Leg_Length_dot_last) / Chassis_dt;
	
	
//	leg_yak2->Right_Leg_Angle_last = leg_yak2->Right_Leg_Angle;
	leg_yak2->Right_Leg_Angle = -Caculate_Included_Angle(90.0f,phi0);
	
	if(leg_yak2->Right_Leg_Angle - leg_yak2->Right_Leg_Angle_last > 180) leg_yak2->Right_Leg_Angle_Round--;
	else if(leg_yak2->Right_Leg_Angle - leg_yak2->Right_Leg_Angle_last < -180) leg_yak2->Right_Leg_Angle_Round++;
	
	leg_yak2->Right_Leg_TotalAngle_Last = leg_yak2->Right_Leg_TotalAngle;
	leg_yak2->Right_Leg_TotalAngle = 360.0f*leg_yak2->Right_Leg_Angle_Round + leg_yak2->Right_Leg_Angle;
	leg_yak2->Right_Leg_Angle_dot_last = leg_yak2->Right_Leg_Angle_dot;
	leg_yak2->Right_Leg_Angle_dot = (leg_yak2->Right_Leg_TotalAngle - leg_yak2->Right_Leg_TotalAngle_Last) / Chassis_dt;
	
/////////////////////////////////////////////
//哈工程VMC
	float d_phi1_R, d_phi4_R;
	float dxc_R   , dyc_R;

	float J[2][2] ={0};
	float Jt[2][2]={0};
	float JT[2][2]={0};
	
	d_phi1_R=Motor_Data_Struct[3].Speed_RPM *2.0f*PI/60.0f *K_gear2chain;
	d_phi4_R=Motor_Data_Struct[4].Speed_RPM *2.0f*PI/60.0f *K_gear2chain;

	/*===| 计算雅各比矩阵J |===*/
	J[0][0]= (L1u*arm_sin_f32(PHI1-PHI2)*arm_sin_f32(PHI3)) / (arm_sin_f32(PHI2-PHI3));
	J[0][1]= (L1u*arm_sin_f32(PHI3-PHI4)*arm_sin_f32(PHI2)) / (arm_sin_f32(PHI2-PHI3));
	J[1][0]=-(L1u*arm_sin_f32(PHI1-PHI2)*arm_cos_f32(PHI3)) / (arm_sin_f32(PHI2-PHI3));
	J[1][1]=-(L1u*arm_sin_f32(PHI3-PHI4)*arm_cos_f32(PHI2)) / (arm_sin_f32(PHI2-PHI3));
	/*===| 计算矩阵Jᵀ*R*M |===*/
	leg_yak2->Jt_r[0][0]=(L1u*arm_sin_f32(PHI0-PHI3)*arm_sin_f32(PHI1-PHI2)) / (arm_sin_f32(PHI3-PHI2));
	leg_yak2->Jt_r[0][1]=(L1u*arm_cos_f32(PHI0-PHI3)*arm_sin_f32(PHI1-PHI2)) / (L0*arm_sin_f32(PHI3-PHI2));
	leg_yak2->Jt_r[1][0]=(L1u*arm_sin_f32(PHI0-PHI2)*arm_sin_f32(PHI3-PHI4)) / (arm_sin_f32(PHI3-PHI2));
	leg_yak2->Jt_r[1][1]=(L1u*arm_cos_f32(PHI0-PHI2)*arm_sin_f32(PHI3-PHI4)) / (L0*arm_sin_f32(PHI3-PHI2));
	
	/*===| 速度映射部分 |===*/
	dxc_R=J[0][0]*d_phi1_R + J[0][1]*d_phi4_R;
	dyc_R=J[1][0]*d_phi1_R + J[1][1]*d_phi4_R;
}



void Off_ground_Detection(void)
{
	float M_wheel = 0.6f;//600g 0.6kg
	float Z_m_dd  = Chassis_Control_Struct.MotionAccel_n_Z;
	
	float Tp_l       = Chassis_Control_Struct.Torque_Joint_left;
	float theta_l    = Chassis_Control_Struct.State[1];
	float theta_l_d  = Chassis_Control_Struct.State[2];
	float theta_l_dd = theta_L_dot_dot;
	float L0_l       = Chassis_Control_Struct.Left_Leg_Length;
	float L0_l_d     = Chassis_Control_Struct.Left_Leg_Length_dot;
	float L0_l_dd    = Chassis_Control_Struct.Left_Leg_Length_dot_dot;
	float P_l        = (Gravity_Comp + Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output)*arm_cos_f32(theta_l) + Tp_l*arm_sin_f32(theta_l)/L0_l;
	
	float Tp_r       = Chassis_Control_Struct.Torque_Joint_right;
	float theta_r    = Chassis_Control_Struct.State[7];
	float theta_r_d  = Chassis_Control_Struct.State[8];
	float theta_r_dd = theta_R_dot_dot;
	float L0_r       = Chassis_Control_Struct.Right_Leg_Length;
	float L0_r_d     = Chassis_Control_Struct.Right_Leg_Length_dot;
	float L0_r_dd    = Chassis_Control_Struct.Right_Leg_Length_dot_dot;
	float P_r        = (Gravity_Comp + Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output)*arm_cos_f32(theta_r) + Tp_r*arm_sin_f32(theta_r)/L0_r;
	
//	Z_w_dd_L = Z_m_dd - L0_l_dd*arm_cos_f32(theta_l) + 2.0f*L0_l_d*theta_l_d*arm_sin_f32(theta_l) + L0_l*theta_l_dd*arm_sin_f32(theta_l) + L0_l*theta_l_d*theta_l_d*arm_cos_f32(theta_l);
//	Z_w_dd_R = Z_m_dd - L0_r_dd*arm_cos_f32(theta_r) + 2.0f*L0_r_d*theta_r_d*arm_sin_f32(theta_r) + L0_r*theta_r_dd*arm_sin_f32(theta_r) + L0_r*theta_r_d*theta_r_d*arm_cos_f32(theta_r);
	
	Chassis_Control_Struct.right_off_flag=0;
	Chassis_Control_Struct.left_off_flag =0;
	
//	Fn_l = M_wheel*9.8f + P_l + M_wheel*Z_w_dd_L;
//	Fn_r = M_wheel*9.8f + P_r + M_wheel*Z_w_dd_R;
	
	Fn_l = M_wheel*9.8f + P_l;
	Fn_r = M_wheel*9.8f + P_r;
	 
	if(Fn_l < Detect_Force)
	Chassis_Control_Struct.left_off_flag =1;
	else
	Chassis_Control_Struct.left_off_flag =0;
	
	if(Fn_r < Detect_Force)
	Chassis_Control_Struct.right_off_flag=1;
	else
	Chassis_Control_Struct.right_off_flag=0;
	
	if(Chassis_Control_Struct.Balance_Flag == 1)//平衡时才检测是否离地 倒地时不检测是否离地
	{
		if(Chassis_Control_Struct.right_off_flag==1 && Chassis_Control_Struct.left_off_flag==1)//当两腿同时离地时才认为离地
		{
			if(Chassis_Control_Struct.MotionAccel_n_Z < -5.0f)//特殊处理:防止与降腿时达到的负向输出限幅导致误判离地
			Chassis_Control_Struct.Off_ground_Flag=1;//判断离地
		}
		else
		{
			Chassis_Control_Struct.Off_ground_Flag=0;//判断未离地
		}
	}
}


void xvEstimateKF_Init(KalmanFilter_t *L_EstimateKF,KalmanFilter_t *R_EstimateKF)//卡尔曼滤波速度观测器初始化
{
    Kalman_Filter_Init(L_EstimateKF, 2, 0, 2);	// 状态向量2维 没有控制量 测量向量2维
	Kalman_Filter_Init(R_EstimateKF, 2, 0, 2);	// 状态向量2维 没有控制量 测量向量2维
	
	memcpy(L_EstimateKF->F_data, L_vaEstimateKF_F, sizeof(L_vaEstimateKF_F));
    memcpy(L_EstimateKF->P_data, L_vaEstimateKF_P, sizeof(L_vaEstimateKF_P));
    memcpy(L_EstimateKF->Q_data, L_vaEstimateKF_Q, sizeof(L_vaEstimateKF_Q));
    memcpy(L_EstimateKF->R_data, L_vaEstimateKF_R, sizeof(L_vaEstimateKF_R));
    memcpy(L_EstimateKF->H_data, L_vaEstimateKF_H, sizeof(L_vaEstimateKF_H));
	
	memcpy(R_EstimateKF->F_data, R_vaEstimateKF_F, sizeof(R_vaEstimateKF_F));
    memcpy(R_EstimateKF->P_data, R_vaEstimateKF_P, sizeof(R_vaEstimateKF_P));
    memcpy(R_EstimateKF->Q_data, R_vaEstimateKF_Q, sizeof(R_vaEstimateKF_Q));
    memcpy(R_EstimateKF->R_data, R_vaEstimateKF_R, sizeof(R_vaEstimateKF_R));
    memcpy(R_EstimateKF->H_data, R_vaEstimateKF_H, sizeof(R_vaEstimateKF_H));
}

void xvEstimateKF_Update(KalmanFilter_t *EstimateKF ,float acc,float vel)//卡尔曼滤波速度观测器数据更新
{   	
    //卡尔曼滤波器测量值更新
    EstimateKF->MeasuredVector[0] =	vel;//测量速度
    EstimateKF->MeasuredVector[1] = acc;//测量加速度
    		
    //卡尔曼滤波器更新函数
    Kalman_Filter_Update(EstimateKF);

    // 提取估计值
    for (uint8_t i = 0; i < 2; i++)
    {
      vel_acc[i] = EstimateKF->FilteredValue[i];
    }
}

/**
 * @brief 初始化氮气弹簧模型
 * @param model 模型指针
 * @param table 映射表数组
 * @param num 表中点数
 * @param min_len 最小腿长（压缩极限）
 * @param max_len 最大腿长（伸展极限）
 */
void GasSpring_Init(GasSpringModel_TypeDef *model, const SpringMapPoint_t *table, uint8_t num, float min_len, float max_len)
{
    model->points = (SpringMapPoint_t*)table;  // 若使用 const 需强制转换
    model->num_points = num;                   // 映射表数据点数量
    model->min_len = min_len;
    model->max_len = max_len;
    model->enable = 1;                         // 默认 1 启用补偿
    model->safety_force_limit = 200.0f;        // 最大输出补偿力限幅 可调
}


/**
 * @brief 根据当前腿长线性插值得到弹簧力
 * @param model 模型指针
 * @param leg_len 当前腿长 (m)
 * @return 弹簧力 (N)，方向为使腿伸长
 */
float GasSpring_GetForce(const GasSpringModel_TypeDef *model, float leg_len)
{
    if (!model->enable) return 0.0f;
    
    // 边界限幅
    if (leg_len <= model->min_len) return model->points[0].force;
    if (leg_len >= model->max_len) return model->points[model->num_points-1].force;
    
    // 查找区间并线性插值
    for (uint8_t i = 0; i < model->num_points - 1; i++) 
	{
        if (leg_len >= model->points[i].leg_len && leg_len <= model->points[i+1].leg_len) 
		{
            float t = (leg_len - model->points[i].leg_len) / (model->points[i+1].leg_len - model->points[i].leg_len);
            float force = model->points[i].force + t * (model->points[i+1].force - model->points[i].force);
            // 安全限幅
            if (force > model->safety_force_limit) force = model->safety_force_limit;
            if (force < 0) force = 0.0f;
            return force;
        }
    }
    return 0.0f;
}



void Jump_Control(void)
{
	//计算得氮气弹簧的动态补偿力
	Spring_comp_left  = -GasSpring_GetForce(&Chassis_Control_Struct.GasSpring_L, Chassis_Control_Struct.Left_Leg_Length);
	Spring_comp_right = -GasSpring_GetForce(&Chassis_Control_Struct.GasSpring_R, Chassis_Control_Struct.Right_Leg_Length);
	//正常运动腿长PID
	PID_Position_Calculate(&Chassis_Control_Struct.Left_leg_Length_PID,  RoboControl_Struct.Leg_Length ,Chassis_Control_Struct.Left_Leg_Length);
	PID_Position_Calculate(&Chassis_Control_Struct.Right_leg_Length_PID, RoboControl_Struct.Leg_Length ,Chassis_Control_Struct.Right_Leg_Length);
	PID_Position_Calculate(&Chassis_Control_Struct.Left_leg_Length_Speed_PID,  Chassis_Control_Struct.Left_leg_Length_PID.Output ,Chassis_Control_Struct.Left_Leg_Length_dot);
	PID_Position_Calculate(&Chassis_Control_Struct.Right_leg_Length_Speed_PID, Chassis_Control_Struct.Right_leg_Length_PID.Output,Chassis_Control_Struct.Right_Leg_Length_dot);
	
	//跳跃收腿PID
	PID_Position_Calculate(&Chassis_Control_Struct.Fold_left_Leg_Position_PID,  RoboControl_Struct.Leg_Length ,Chassis_Control_Struct.Left_Leg_Length);
	PID_Position_Calculate(&Chassis_Control_Struct.Fold_right_Leg_Position_PID, RoboControl_Struct.Leg_Length ,Chassis_Control_Struct.Right_Leg_Length);
	PID_Position_Calculate(&Chassis_Control_Struct.Fold_left_Leg_Speed_PID,  Chassis_Control_Struct.Fold_left_Leg_Position_PID.Output ,Chassis_Control_Struct.Left_Leg_Length_dot);
	PID_Position_Calculate(&Chassis_Control_Struct.Fold_right_Leg_Speed_PID, Chassis_Control_Struct.Fold_right_Leg_Position_PID.Output,Chassis_Control_Struct.Right_Leg_Length_dot);
	
	if(Chassis_Control_Struct.Jump_Flag == 1)//跳跃状态下腿长力控计算(前馈 + 腿长PD控制)
	{
		switch(Chassis_Control_Struct.Jump_Level)
		{
			case 1:
				Jump_Leg_length = 0.20f; 
				break; 
				
			case 2:
				Jump_Leg_length = 0.25f; 
				break;
				
			case 3:
				Jump_Leg_length = 0.33f; 
				break;
		}
		
		if(Chassis_Control_Struct.Jump_State == 0)//状态0：降腿蓄力跳跃
		{
			RoboControl_Struct.Leg_Length=0.15f;
			Chassis_Control_Struct.F_L = Spring_comp_left  + Gravity_Comp + Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output ;
			Chassis_Control_Struct.F_R = Spring_comp_right + Gravity_Comp + Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output;
			
			if(Chassis_Control_Struct.Left_Leg_Length < 0.16f && Chassis_Control_Struct.Right_Leg_Length < 0.16f)
			{
				Chassis_Control_Struct.Jump_state_change_Tick++;
			}
			else
			{
				Chassis_Control_Struct.Jump_state_change_Tick = 0;
			}
			if(Chassis_Control_Struct.Jump_state_change_Tick>=30)
			{
				Chassis_Control_Struct.Jump_state_change_Tick = 0;
				Chassis_Control_Struct.Jump_State = 1;//状态转移
			}
		}
		else if(Chassis_Control_Struct.Jump_State == 1)//状态1：全力蹬腿起跳 
		{
			K_slope += 25.0f*Chassis_dt;//32
			Limit_float(&K_slope,1.0f,0.0f);
			
			Chassis_Control_Struct.F_L = Spring_comp_left  + K_slope*330.0f;//340
			Chassis_Control_Struct.F_R = Spring_comp_right + K_slope*330.0f;
			
			if(Chassis_Control_Struct.Left_Leg_Length > Jump_Leg_length && Chassis_Control_Struct.Right_Leg_Length > Jump_Leg_length)
			{
				Chassis_Control_Struct.Jump_state_change_Tick++;
			}
			else
			{
				Chassis_Control_Struct.Jump_state_change_Tick = 0;
			}
			if(Chassis_Control_Struct.Jump_state_change_Tick>=15)
			{
				K_slope = 0;
				Chassis_Control_Struct.Jump_state_change_Tick = 0;
				Chassis_Control_Struct.Jump_State = 2;//状态转移
			}
		}
		else if(Chassis_Control_Struct.Jump_State == 2)//状态2：空中调整快速收腿 (放大收腿PID输出快速收腿)
		{
			RoboControl_Struct.Leg_Length=0.15f;

			K_slope += 20.0f*Chassis_dt;
			Limit_float(&K_slope,1.0f,0.0f);
			
			Chassis_Control_Struct.F_L = Spring_comp_left  + K_slope*Chassis_Control_Struct.Fold_left_Leg_Speed_PID.Output  - (1-K_slope) * 50;
			Chassis_Control_Struct.F_R = Spring_comp_right + K_slope*Chassis_Control_Struct.Fold_right_Leg_Speed_PID.Output - (1-K_slope) * 50;
			
			Chassis_Control_Struct.Torque_Joint_left =0.25f*(LQR_K_L[6]*(-Chassis_Control_Struct.State[1]) + LQR_K_L[7]*(-Chassis_Control_Struct.State[2])) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);//左关节力矩只保留摆角控制量
			Chassis_Control_Struct.Torque_Joint_right=0.25f*(LQR_K_R[6]*( Chassis_Control_Struct.State[7]) + LQR_K_R[7]*( Chassis_Control_Struct.State[8])) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);//右关节力矩只保留摆角控制量
			
			TP_wl_wr_jl_jr[4] = 0;//左轮电机输出置0
			TP_wl_wr_jl_jr[5] = 0;//右轮电机输出置0
			
			if(Chassis_Control_Struct.Left_Leg_Length < 0.17f && Chassis_Control_Struct.Right_Leg_Length < 0.17f)//稳定跳跃版数据 0.17f
			{
				Chassis_Control_Struct.Jump_state_change_Tick++;
			}
			else
			{
				Chassis_Control_Struct.Jump_state_change_Tick = 0;
			}
			if(Chassis_Control_Struct.Jump_state_change_Tick >= 40)//调整此状态转移时间阈值可改变在空中收腿的滞空时间 决定何时开始伸腿缓冲 50(长滞空) 30(短滞空)
			{
				K_slope = 0.0f;
				Chassis_Control_Struct.Jump_state_change_Tick = 0;
				Chassis_Control_Struct.Jump_State = 3;//状态转移
			}
		}
		else if(Chassis_Control_Struct.Jump_State == 3)//跳跃动作结束 
		{
			Chassis_Control_Struct.Jump_Flag  = 0;
			Chassis_Control_Struct.Jump_State = 0;
			Chassis_Control_Struct.Jump_state_change_Tick = 0;
			
			RoboControl_Struct.Leg_Length=0.20f;
			K_slope = 0.0f;
		}
	}
	
	else//常规控制模式(非跳跃状态)下的腿长及滚转角补偿力计算
	{
		if(Chassis_Control_Struct.Allow_to_Stand_Flag ==1)//允许起立标志位使能才可进入正常腿长推力运控
		{
			if(Chassis_Control_Struct.TipOver_Simple.Warning_Flag == 1)//检测到机体有倾倒趋势 主动降低腿长以提高稳定性
			{
				RoboControl_Struct.Leg_Length=0.15f;
			}
			
			if(Chassis_Control_Struct.Off_ground_Flag == 1)//检测到离地 取消重补 避免干扰空中腿部姿态
			{
				Chassis_Control_Struct.F_L = Soft_landing_Comp_F + Spring_comp_left  + Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output;
				Chassis_Control_Struct.F_R = Soft_landing_Comp_F + Spring_comp_right + Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output;
			}
			else//正常触地运动
			{
				Chassis_Control_Struct.F_L = Spring_comp_left  + Gravity_Comp + Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output  - Chassis_Control_Struct.Roll_Leg_PID.Output;
				Chassis_Control_Struct.F_R = Spring_comp_right + Gravity_Comp + Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output + Chassis_Control_Struct.Roll_Leg_PID.Output;
			}
		}
	}
}


void Recover_from_Ground(void)
{
	if(Chassis_Control_Struct.Recover_from_ground_Flag == 1)
	{
        Chassis_Control_Struct.Target_displacement = Chassis_Control_Struct.displacement;
        Chassis_Control_Struct.add_displacement = 0.0f;
        Chassis_Control_Struct.now_displacement = Chassis_Control_Struct.displacement;
		
		if(INS_Data_Self.Pitch < 0.0f)
		{
			Chassis_Control_Struct.Recover_Mode = 1;//机体前栽翻
		}
		else
		{
			Chassis_Control_Struct.Recover_Mode = 2;//机体后仰翻
		}
			
		if(Chassis_Control_Struct.Recover_Mode == 2 || Chassis_Control_Struct.Recover_Mode == 1)
		{
			if(Chassis_Control_Struct.Recover_State == 0)//状态0:平齐双腿并往前摆
			{
				RoboControl_Struct.Leg_Length = 0.36f;
				Chassis_Control_Struct.F_L = Spring_comp_left  + 2.0f*Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output;//取消Roll轴补偿 以免腿长不一
				Chassis_Control_Struct.F_R = Spring_comp_right + 2.0f*Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output;
				
				TP_wl_wr_jl_jr[4] = 0;//左轮电机输出置0
				TP_wl_wr_jl_jr[5] = 0;//右轮电机输出置0
				
				//根据摆角夹角设置固定速度归中
				float CenterSpeed_Target;
				CenterSpeed_Target = Caculate_Included_Angle(Left_angle, -Right_angle);
				Limit_float(&CenterSpeed_Target, 50, -50);

				//速度环
				PID_Position_Calculate(&Chassis_Control_Struct.Left_leg_AngleSpeed_PID,    125+CenterSpeed_Target, Chassis_Control_Struct.Left_Leg_Angle_dot);
				PID_Position_Calculate(&Chassis_Control_Struct.Right_leg_AngleSpeed_PID,  -125+CenterSpeed_Target, Chassis_Control_Struct.Right_Leg_Angle_dot);
				Chassis_Control_Struct.Torque_Joint_left  = -Chassis_Control_Struct.Left_leg_AngleSpeed_PID.Output;
				Chassis_Control_Struct.Torque_Joint_right = -Chassis_Control_Struct.Right_leg_AngleSpeed_PID.Output;
			
				//判断车身已经翻过来 呈现水平状态且摆角在-110~-45度区间
				if( fabsf(INS_Data_Self.Pitch) < 45.0f &&
					Caculate_Included_Angle(0,  Left_angle)  > -110 && Caculate_Included_Angle(0, Left_angle)   < 45 &&
					Caculate_Included_Angle(0, -Right_angle) > -110 && Caculate_Included_Angle(0, -Right_angle) < 45
				  )
				{
					Chassis_Control_Struct.Recover_state_change_Tick++;
				}
				else
				{
					Chassis_Control_Struct.Recover_state_change_Tick = 0;
				}
				if(Chassis_Control_Struct.Recover_state_change_Tick>=100)
				{
					Chassis_Control_Struct.Recover_state_change_Tick = 0;
					Chassis_Control_Struct.Recover_State = 2;
				}
			}
			else if(Chassis_Control_Struct.Recover_State == 2)//状态2:收腿至最短起立
			{
				RoboControl_Struct.Leg_Length = 0.15f;
//				Chassis_Control_Struct.F_L = Spring_comp_left  + 2.0f*Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output;
//				Chassis_Control_Struct.F_R = Spring_comp_right + 2.0f*Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output;
				Chassis_Control_Struct.F_L = Spring_comp_left  + Chassis_Control_Struct.Fold_left_Leg_Speed_PID.Output ;
				Chassis_Control_Struct.F_R = Spring_comp_right + Chassis_Control_Struct.Fold_right_Leg_Speed_PID.Output;
				Chassis_Control_Struct.Torque_Joint_left =0.15f*(LQR_K_L[6]*(-Chassis_Control_Struct.State[1]) + LQR_K_L[7]*(-Chassis_Control_Struct.State[2])) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);//左关节力矩只保留摆角控制量
				Chassis_Control_Struct.Torque_Joint_right=0.15f*(LQR_K_R[6]*( Chassis_Control_Struct.State[7]) + LQR_K_R[7]*( Chassis_Control_Struct.State[8])) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);//右关节力矩只保留摆角控制量
			
				TP_wl_wr_jl_jr[4] = 0;
				TP_wl_wr_jl_jr[5] = 0;
				
				if(Chassis_Control_Struct.Left_Leg_Length < 0.17f && Chassis_Control_Struct.Right_Leg_Length < 0.17f)//判断已经收腿至可起立长度
				{
					Chassis_Control_Struct.Recover_state_change_Tick++;
				}
				else
				{
					Chassis_Control_Struct.Recover_state_change_Tick = 0;
				}
				
				if(Chassis_Control_Struct.Recover_state_change_Tick>=200)
				{
					Chassis_Control_Struct.Recover_state_change_Tick = 0;
					Chassis_Control_Struct.Recover_State = 3;
				}
			}
			else if(Chassis_Control_Struct.Recover_State == 3)
			{
				Chassis_Control_Struct.Recover_from_ground_Flag  = 0;
				Chassis_Control_Struct.Recover_state_change_Tick = 0;
				Chassis_Control_Struct.Recover_State = 0;
				
				Chassis_Control_Struct.Target_displacement = Chassis_Control_Struct.displacement;
				Chassis_Control_Struct.add_displacement = 0.0f;
				Chassis_Control_Struct.now_displacement = Chassis_Control_Struct.displacement;
				Chassis_Control_Struct.Chassis_Vy_Target_last = 0.0f; 
				
				RoboControl_Struct.Leg_Length = 0.15f;
				RoboControl_Struct.Chassis_State = Chassis_STATIC;
				
				Chassis_Control_Struct.Allow_to_Stand_Flag = 1;//允许起立
			}
		}
	}
	if(Chassis_Control_Struct.Recover_from_ground_Flag > 0)
	{
		Buzzer_Set_SoundEffect(Buzzer_SoundEffect_Aim_ON);//到底自起过程中一直播放此音效
		if(RoboControl_Struct.Robo_Enable == 0)
		{
			Chassis_Control_Struct.Left_leg_AngleSpeed_PID.I_Output  = 0;
			Chassis_Control_Struct.Right_leg_AngleSpeed_PID.I_Output = 0;
		}
	}
}

void Up_Step_Dection(void)
{
	float left_leg_angle      = Chassis_Control_Struct.State[1]*Radain2Angle;
	float left_leg_angle_dot  = Chassis_Control_Struct.State[2];
	
	float right_leg_angle     = Chassis_Control_Struct.State[7]*Radain2Angle;
	float right_leg_angle_dot = Chassis_Control_Struct.State[8];
	
	//上台阶磕碰检测阈值(需根据当前车辆的LQR参数反映出的关节软硬实际调节)
	float angle_error_gate     = 14.0f;
	float angle_dot_error_gate = 1.7f;
	
	//上台阶磕碰检测标志位 
	uint8_t Angle_error_flag     = 0;
	uint8_t Angle_dot_error_flag = 0;
	
	//开启上台阶磕碰检测
	if(Chassis_Control_Struct.UpStep_Dect_Flag == 1)
	{
 		Chassis_Control_Struct.Chassis_Forward_Change_Flag = 0;
		RoboControl_Struct.Leg_Length=0.35f;//进入上台阶检测直接伸腿至最高 方便低腿刷完RFID高腿上台阶 手感好
		
		if(left_leg_angle>angle_error_gate && right_leg_angle>angle_error_gate)
			Angle_error_flag = 1;
		
		if(left_leg_angle_dot>angle_dot_error_gate && right_leg_angle_dot>angle_dot_error_gate)
			Angle_dot_error_flag = 1;
		
		if(Angle_error_flag==1 && Angle_dot_error_flag==1)
		{
			Chassis_Control_Struct.UpStep_Flag = 1;
			Angle_error_flag = 0;
			Angle_dot_error_flag = 0;
		}
	}	
}

void Up_Step_Func(void)//磕台阶前升到最高腿长
{
	float dect_angle=0.0f;
	
	if(Chassis_Control_Struct.UpStep_Flag == 1)
	{
		if(Chassis_Control_Struct.UpStep_State == 0)//磕到台阶后向后摆腿同时收腿
		{
			RoboControl_Struct.Leg_Length = 0.15f;
			RoboControl_Struct.Leg_Angle = -55.0f;
			dect_angle = 50.0f;
			TP_wl_wr_jl_jr[4] = 0;//左轮电机输出置0
			TP_wl_wr_jl_jr[5] = 0;//右轮电机输出置0
			
			PID_Position_Calculate(&Chassis_Control_Struct.Left_leg_Angle_PID,   RoboControl_Struct.Leg_Angle, Chassis_Control_Struct.Left_Leg_Angle);
			PID_Position_Calculate(&Chassis_Control_Struct.Right_leg_Angle_PID, -RoboControl_Struct.Leg_Angle, Chassis_Control_Struct.Right_Leg_Angle);
		
			Chassis_Control_Struct.F_L = Spring_comp_left  + 1.5f*Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output ;
			Chassis_Control_Struct.F_R = Spring_comp_right + 1.5f*Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output;
			Chassis_Control_Struct.Torque_Joint_left  = -0.15f*Chassis_Control_Struct.Left_leg_Angle_PID.Output;
			Chassis_Control_Struct.Torque_Joint_right = -0.15f*Chassis_Control_Struct.Right_leg_Angle_PID.Output;
			
			if( (fabsf(Chassis_Control_Struct.Left_Leg_Angle)>dect_angle) && (fabsf(Chassis_Control_Struct.Right_Leg_Angle)>dect_angle) && 
			    (Chassis_Control_Struct.Left_Leg_Length < 0.18f && Chassis_Control_Struct.Right_Leg_Length < 0.18f) )//判断双腿已经摆到合适位置且收腿完成
			{
				Chassis_Control_Struct.UpStep_state_change_Tick++;
			}
			else
			{
				Chassis_Control_Struct.UpStep_state_change_Tick = 0;
			}
			if(Chassis_Control_Struct.UpStep_state_change_Tick>=25)
			{
				Chassis_Control_Struct.UpStep_state_change_Tick = 0;
				Chassis_Control_Struct.UpStep_State = 2;//状态转移
			}
		}
		else if(Chassis_Control_Struct.UpStep_State == 2)//恢复到正常状态
		{
			RoboControl_Struct.Leg_Length = 0.15f;
			RoboControl_Struct.Leg_Angle  = 0.0f;
			Chassis_Control_Struct.Leg_length_Level = 1;
			
			Chassis_Control_Struct.UpStep_State = 0;
			Chassis_Control_Struct.UpStep_state_change_Tick = 0;
			Chassis_Control_Struct.UpStep_Flag = 0;
			Chassis_Control_Struct.UpStep_Dect_Flag = 0;//上台阶磕碰检测标志位清零
		}
	}
}


void Simple_TipOver_Init(void)//倾倒检测功能初始化
{
    Simple_TipOver_TypeDef *ts = &Chassis_Control_Struct.TipOver_Simple;
    
    ts->Warning_Flag    = 0;
    ts->Warning_Counter = 0;
    ts->Current_Pitch_Angle = 0.0f;
    ts->Current_Pitch_Rate  = 0.0f;
    ts->Current_Theta_Error = 0.0f;
}

void Simple_TipOver_Detection(void)//倾倒检测功能
{
    Simple_TipOver_TypeDef *ts = &Chassis_Control_Struct.TipOver_Simple;
    
    // 计算三个核心指标
    // 俯仰角 = 机体俯仰角
    ts->Current_Pitch_Angle = fabsf(INS_Data_Self.Pitch);
    
    // 俯仰角速度 = 机体俯仰角速度
    ts->Current_Pitch_Rate  = fabsf(Chassis_Control_Struct.Pitch_Speed_fdb);
    
    // 腿部角度误差 = 平均腿部摆角
    float theta_left  = fabsf(Chassis_Control_Struct.State[1]);  // 左腿θ
    float theta_right = fabsf(Chassis_Control_Struct.State[7]);  // 右腿θ
    ts->Current_Theta_Error = (theta_left + theta_right) / 2.0f *Radain2Angle;
    
    // 三个检测条件（可调节）
    uint8_t condition1 = (ts->Current_Pitch_Angle > 10.0f);     // 俯仰角误差>10°
    uint8_t condition2 = (ts->Current_Pitch_Rate  > 1.5f);      // 俯仰角速度>1.5rad/s
    uint8_t condition3 = (ts->Current_Theta_Error > 20.0f);     // 腿部角度>20°
    
    //简单逻辑：满足任意两个条件即认为有倾倒风险
    uint8_t risk_conditions = condition1 + condition2 + condition3;
    
    if (risk_conditions >= 2) 
	{
        ts->Warning_Counter++;
        
        if (ts->Warning_Counter >= 5)// 持续 10ms 触发 避免瞬时干扰
            ts->Warning_Flag = 1;
    } 
	else 
	{
        if (ts->Warning_Counter > 0) 
            ts->Warning_Counter--;
        
        if (ts->Warning_Counter == 0) 
            ts->Warning_Flag = 0;
    }
}


void State_Filter(void)
{
////	Chassis_Control_Struct.Left_Leg_Angle_dot_last  = Filter_Left_Leg_Angle_dot;
////	Chassis_Control_Struct.Right_Leg_Angle_dot_last = Filter_Right_Leg_Angle_dot;
////	Chassis_Control_Struct.Pitch_Speed_fdb_last		= Filter_Pitch_Speed_fdb;
////	Chassis_Control_Struct.Volecity_Fdb_last		= Filter_Volecity_Fdb;
	
////	Filter_Left_Leg_Angle_dot  = K_Lowpass_fliter*Chassis_Control_Struct.Left_Leg_Angle_dot  + (1.0f-K_Lowpass_fliter)*Chassis_Control_Struct.Left_Leg_Angle_dot_last;
////	Filter_Right_Leg_Angle_dot = K_Lowpass_fliter*Chassis_Control_Struct.Right_Leg_Angle_dot + (1.0f-K_Lowpass_fliter)*Chassis_Control_Struct.Right_Leg_Angle_dot_last;
////	Filter_Pitch_Speed_fdb     = K_Lowpass_fliter*Chassis_Control_Struct.Pitch_Speed_fdb     + (1.0f-K_Lowpass_fliter)*Chassis_Control_Struct.Pitch_Speed_fdb_last;
////	Filter_Volecity_Fdb		   = K_Lowpass_fliter*Chassis_Control_Struct.Volecity_Fdb        + (1.0f-K_Lowpass_fliter)*Chassis_Control_Struct.Volecity_Fdb_last;
	
	MotionAccel_n_Z_last = Filter_MotionAccel_n_Z;

//================== Z轴加速度：中值滤波(去毛刺) + 低通滤波(去波动) ==================
	static float z_buf[3] = {0.0f, 0.0f, 0.0f}; // 存储3次原始数据
	static uint8_t z_idx = 0;
	static uint8_t z_init_cnt = 0;
	float Median_MotionAccel_n_Z;
	float z_a, z_b, z_c;

	// 1. 更新滑动窗口
	z_buf[z_idx] = INS.MotionAccel_n[2];
	z_idx++;
	if(z_idx >= 3) z_idx = 0;

	// 2. 启动保护：前3次直接透传，防止初始0.0f污染中值
	if(z_init_cnt < 3) 
	{
		Median_MotionAccel_n_Z = INS.MotionAccel_n[2];
		z_init_cnt++;
	}
	else 
	{
		// 3. 提取窗口数据
		z_a = z_buf[0];
		z_b = z_buf[1];
		z_c = z_buf[2];

		// 4. 纯逻辑判断求中值（无排序、无额外函数调用，最高效）
		if (z_a > z_b) 
		{
			if (z_b > z_c) Median_MotionAccel_n_Z = z_b;
			else if (z_a > z_c) Median_MotionAccel_n_Z = z_c;
			else Median_MotionAccel_n_Z = z_a;
		} 
		else 
		{
			if (z_a > z_c) Median_MotionAccel_n_Z = z_a;
			else if (z_b > z_c) Median_MotionAccel_n_Z = z_c;
			else Median_MotionAccel_n_Z = z_b;
		}
	}

	// 5. 一阶低通滤波（输入源从原始数据改为中值数据）
	Filter_MotionAccel_n_Z = K_Lowpass_fliter * Median_MotionAccel_n_Z + (1.0f - K_Lowpass_fliter) * MotionAccel_n_Z_last;//中值滤波+低通滤波
//	Filter_MotionAccel_n_Z = Median_MotionAccel_n_Z;//中值滤波
//	Filter_MotionAccel_n_Z = K_Lowpass_fliter*INS.MotionAccel_n[2] + (1.0f-K_Lowpass_fliter)*MotionAccel_n_Z_last;//低通滤波
	
	Chassis_Control_Struct.MotionAccel_n_Z = Filter_MotionAccel_n_Z;
//=========================================================================
}



/**
 * @brief 六个状态量
 */
void add_six_state(Chassis_Control_StructTypeDef *leg_yak4)
{
	theta_L_dot_last = leg_yak4->State[2];
	theta_R_dot_last = leg_yak4->State[8];
	
	leg_yak4->State[1] = -(leg_yak4->Left_Leg_Angle)*Angle2Radain     -  *(leg_yak4->state_lqr + QB);		//θ  左腿后摆 θ  为正 增大
	leg_yak4->State[2] = -(leg_yak4->Left_Leg_Angle_dot)*Angle2Radain -  *(leg_yak4->state_lqr + QB_D);	    //θ’ 左腿后摆 θ’ 为正 增大
	
	leg_yak4->State[7] = (leg_yak4->Right_Leg_Angle)*Angle2Radain     -  *(leg_yak4->state_lqr + QB);		//θ  右腿后摆 θ  为正 增大
    leg_yak4->State[8] = (leg_yak4->Right_Leg_Angle_dot)*Angle2Radain -  *(leg_yak4->state_lqr + QB_D);		//θ’ 右腿后摆 θ’ 为正 增大

    leg_yak4->State[3] = *(leg_yak4->state_lqr + S);				//位移							
    leg_yak4->State[4] = *(leg_yak4->state_lqr + S_D);				//位移一阶导	
    leg_yak4->State[5] = *(leg_yak4->state_lqr + QB);				//机体pitch       //翘头pitch增大
    leg_yak4->State[6] = *(leg_yak4->state_lqr + QB_D);				//机体pitch一阶导	
	
	theta_L_dot_dot = (leg_yak4->State[2] - theta_L_dot_last) / Chassis_dt;
	theta_R_dot_dot = (leg_yak4->State[8] - theta_R_dot_last) / Chassis_dt;
}


void calucateK(float (*Poly_Coefficient)[4])  
{
    for(int i = 0; i < 12; i++)
    {
		LQR_K_L[i] = LQR_K_calc(Poly_Coefficient[i], Chassis_Control_Struct.Ave_Leg_Length);
        LQR_K_R[i] = LQR_K_calc(Poly_Coefficient[i], Chassis_Control_Struct.Ave_Leg_Length);
    }
}


float LQR_K_calc(float *coe,float len)
{
  float K = coe[0]*len*len*len + coe[1]*len*len + coe[2]*len + coe[3];
	
  return K;
}


void calucateLQR_L(float *lqr_l)
{
    float K11 = lqr_l[0];// 赋值K
    float K12 = lqr_l[1];
    float K13 = lqr_l[2];
    float K14 = lqr_l[3];
    float K15 = lqr_l[4];
    float K16 = lqr_l[5];		  
    float K21 = lqr_l[6];
    float K22 = lqr_l[7];
    float K23 = lqr_l[8];
    float K24 = lqr_l[9];
    float K25 = lqr_l[10];
    float K26 = lqr_l[11];

    float Left_theta   = -Chassis_Control_Struct.State[1];
    float Left_theta_d = -Chassis_Control_Struct.State[2];
    float xb    = -Chassis_Control_Struct.State[3];
    float xb_d  = -Chassis_Control_Struct.State[4];
    float phi   = -Chassis_Control_Struct.State[5];
    float phi_d = -Chassis_Control_Struct.State[6];
	
    //板凳状态下输出力矩锁定关节摆角 若用LQR控制关节则注释掉此处的Tp的输出
//	Chassis_Control_Struct.Torque_Joint_left=-Chassis_Control_Struct.Left_leg_Angle_PID.Output;
	
    Chassis_Control_Struct.Torque_wheel_left = -( K11 * Left_theta + K12 * Left_theta_d + K13 * xb + K14 * xb_d + K15 * phi + K16 * phi_d );
    Chassis_Control_Struct.Torque_Joint_left =  ( K21 * Left_theta + K22 * Left_theta_d + K23 * xb + K24 * xb_d + K25 * phi + K26 * phi_d ) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);

	if(Chassis_Control_Struct.Jump_Flag == 1)
	{
		TP_wl_wr_jl_jr[4] = Chassis_Control_Struct.Torque_wheel_left;
	}
	else if(Chassis_Control_Struct.Off_ground_Flag == 1)
	{
		TP_wl_wr_jl_jr[4] = 0;//左轮电机输出置0
		Chassis_Control_Struct.Torque_Joint_left =(LQR_K_L[6]*(-Chassis_Control_Struct.State[1]) + LQR_K_L[7]*(-Chassis_Control_Struct.State[2])) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);//左关节力矩只保留摆角控制量
	}
	else
	{
		TP_wl_wr_jl_jr[4] = Chassis_Control_Struct.Torque_wheel_left + Chassis_Control_Struct.Turn_Speed_PID.Output;
	}
	Limit_float(&Chassis_Control_Struct.Torque_Joint_left,80.0f,-80.0f);
	Limit_float(&TP_wl_wr_jl_jr[4],6.0f,-6.0f);
}

void calucateLQR_R(float *lqr_r)
{
    float K11 = lqr_r[0];// 赋值K
    float K12 = lqr_r[1];
    float K13 = lqr_r[2];
    float K14 = lqr_r[3];
    float K15 = lqr_r[4];
    float K16 = lqr_r[5];				  
    float K21 = lqr_r[6];
    float K22 = lqr_r[7];
    float K23 = lqr_r[8];
    float K24 = lqr_r[9];
    float K25 = lqr_r[10];
    float K26 = lqr_r[11];

	float Right_theta   = Chassis_Control_Struct.State[7];
    float Right_theta_d = Chassis_Control_Struct.State[8];
    float xb    = Chassis_Control_Struct.State[3];
    float xb_d  = Chassis_Control_Struct.State[4];
    float phi   = Chassis_Control_Struct.State[5];
    float phi_d = Chassis_Control_Struct.State[6];

	//板凳状态下输出力矩锁定关节摆角 若用LQR控制关节则注释掉此处的Tp的输出
//	Chassis_Control_Struct.Torque_Joint_right=-Chassis_Control_Struct.Right_leg_Angle_PID.Output;
	
    Chassis_Control_Struct.Torque_wheel_right = -( K11 * Right_theta + K12 * Right_theta_d + K13 * xb + K14 * xb_d + K15 * phi + K16 * phi_d );
    Chassis_Control_Struct.Torque_Joint_right =  ( K21 * Right_theta + K22 * Right_theta_d + K23 * xb + K24 * xb_d + K25 * phi + K26 * phi_d ) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);
	
	if(Chassis_Control_Struct.Jump_Flag == 1)
	{
		TP_wl_wr_jl_jr[5] = Chassis_Control_Struct.Torque_wheel_right;
	}
	else if(Chassis_Control_Struct.Off_ground_Flag == 1)
	{
		TP_wl_wr_jl_jr[5] = 0;//右轮电机输出置0
		Chassis_Control_Struct.Torque_Joint_right=(LQR_K_R[6]*( Chassis_Control_Struct.State[7]) + LQR_K_R[7]*( Chassis_Control_Struct.State[8])) + (-Chassis_Control_Struct.Tp_Comp_PID.Output);//右关节力矩只保留摆角控制量
	}
	else
	{
		TP_wl_wr_jl_jr[5] = Chassis_Control_Struct.Torque_wheel_right + Chassis_Control_Struct.Turn_Speed_PID.Output;
	}
	Limit_float(&Chassis_Control_Struct.Torque_Joint_right,80.0f,-80.0f);
	Limit_float(&TP_wl_wr_jl_jr[5],6.0f,-6.0f);
}

void calucateTp(Chassis_Control_StructTypeDef *leg_yak9)
{
	TP_wl_wr_jl_jr[1]= -leg_yak9->Jt_l[0][0]*leg_yak9->F_L + leg_yak9->Jt_l[0][1]*leg_yak9->Torque_Joint_left;
	TP_wl_wr_jl_jr[0]= -leg_yak9->Jt_l[1][0]*leg_yak9->F_L + leg_yak9->Jt_l[1][1]*leg_yak9->Torque_Joint_left;
	TP_wl_wr_jl_jr[3]=  leg_yak9->Jt_r[0][0]*leg_yak9->F_R + leg_yak9->Jt_r[0][1]*leg_yak9->Torque_Joint_right;
	TP_wl_wr_jl_jr[2]=  leg_yak9->Jt_r[1][0]*leg_yak9->F_R + leg_yak9->Jt_r[1][1]*leg_yak9->Torque_Joint_right;
}


void Smooth_Restand_Func(void)
{
	//动态补偿氮气弹簧力 再加上收腿PID输出实现缓收腿
	Chassis_Control_Struct.F_L = Spring_comp_left  + 2.0f*Chassis_Control_Struct.Left_leg_Length_Speed_PID.Output;
	Chassis_Control_Struct.F_R = Spring_comp_right + 2.0f*Chassis_Control_Struct.Right_leg_Length_Speed_PID.Output;
	
	Chassis_Control_Struct.Torque_Joint_left  = 0.0f;
	Chassis_Control_Struct.Torque_Joint_right = 0.0f;
	TP_wl_wr_jl_jr[4] = 0;
	TP_wl_wr_jl_jr[5] = 0;
	
	if(Chassis_Control_Struct.Left_Leg_Length < 0.17f && Chassis_Control_Struct.Right_Leg_Length < 0.17f)//判断已经收腿至可起立长度
	{
		Chassis_Control_Struct.Allow_to_Stand_Tick++;
	}
	else
	{
		Chassis_Control_Struct.Allow_to_Stand_Tick = 0;
	}
	
	if(Chassis_Control_Struct.Allow_to_Stand_Tick >= 100)
	{
		Chassis_Control_Struct.Target_displacement = Chassis_Control_Struct.displacement;
		Chassis_Control_Struct.add_displacement    = 0.0f;
		Chassis_Control_Struct.now_displacement    = Chassis_Control_Struct.displacement;
		Chassis_Control_Struct.Chassis_Vy_Target_last = 0.0f; 
		
		Chassis_Control_Struct.Allow_to_Stand_Tick = 0;
		Chassis_Control_Struct.Allow_to_Stand_Flag = 1;//允许起立
		Chassis_Control_Struct.Smooth_restand_Flag = 0;//清零缓起立功能执行标志位
	}	
}