//======================================================================
//                    Define[全局定义]
//      这里定义了一些可能需要经常修改或者引用的宏定义
//======================================================================

#ifndef __Define__
#define __Define__


/*===| 外设接口定义 |===*/
#define IMU_SPI &hspi4
#define IMU_Heat_TIM &htim15
#define IMU_Heat_TIM_CHANNAL TIM_CHANNEL_1
#define IMU_Heat_TIM_ARR 20000

#define LED_TIM &htim1
#define LED_R_TIM_CHANNAL TIM_CHANNEL_2
#define LED_G_TIM_CHANNAL TIM_CHANNEL_3
#define LED_B_TIM_CHANNAL TIM_CHANNEL_4
#define LED_TIM_ARR 1000

//485 4Mbits/s
#define RS485_1_UART &huart3
#define RS485_2_UART &huart2
#define SBUS_UART &huart1

#define SBUS_UART_DMA &hdma_usart1_rx
#define RS485_2_UART_DMA &hdma_usart2_rx
#define RS485_1_UART_DMA &hdma_usart3_rx
#define UART4_DMA &hdma_uart4_rx
#define UART5_DMA &hdma_uart5_rx

#define PWM_TIM &htim3
#define PWM_1_TIM_CHANNAL TIM_CHANNEL_1
#define PWM_2_TIM_CHANNAL TIM_CHANNEL_2
#define PWM_3_TIM_CHANNAL TIM_CHANNEL_3
#define PWM_4_TIM_CHANNAL TIM_CHANNEL_4

#define Buzzer_TIM &htim5
#define Buzzer_TIM_CHANNAL TIM_CHANNEL_1

#define USER_SPI &hspi2
#define USER_I2C &hi2c2

////////////////////////////////////////////////////////////////////
//                                                               //
// 修改了CANID后接收数据不用动，需要在发送函数处根据ID重新排序 //
//                                                               //
////////////////////////////////////////////////////////////////////
/*===| 底盘电机通信相关定义 |===*/
#define Chassis_DriverMotor_CAN          	    hfdcan2     //轮电机CAN        
#define Chassis_DriverMotor_Send_CAN_ID         0x200       //轮电机CAN发送ID
#define Chassis_Motor1_Feedback_CAN_ID          0x203       //轮电机1CAN反馈ID 	
#define Chassis_Motor2_Feedback_CAN_ID      	0x204       //轮电机2CAN反馈ID

#define Chassis_JointMotor_CAN					hfdcan1            			   
#define Chassis_JointMotor1_Send_CAN_ID			0x02
#define Chassis_JointMotor1_Feedback_ID			0x12
#define Chassis_JointMotor2_Send_CAN_ID			0x01
#define Chassis_JointMotor2_Feedback_ID			0x11
#define Chassis_JointMotor3_Send_CAN_ID			0x03
#define Chassis_JointMotor3_Feedback_ID			0x13
#define Chassis_JointMotor4_Send_CAN_ID			0x04
#define Chassis_JointMotor4_Feedback_ID			0x14
	
/*===| 云台电机通信相关定义 |===*/
    #define Gimbal_Pitch_CAN                    hfdcan2   //Pitch_使用的CAN
    #define Gimbal_Pitch_Send_CAN_ID            0x1FE    //Pitch_CAN发送ID
    #define Gimbal_Pitch_Feedback_CAN_ID        0x205    //Pitch_CAN反馈ID
    
    #define Gimbal_Yaw_CAN                      hfdcan3    //Yaw_使用的CAN
    #define Gimbal_Yaw_Send_CAN_ID              0x1FF    //Yaw_CAN发送ID
    #define Gimbal_Yaw_Feedback_CAN_ID          0x206   //Yaw_CAN反馈ID

/*===| 发射电机通信相关定义 |===*/
    #define Shoot_Fric_First_CAN                      hfdcan3    //Fric_使用的CAN
    #define Shoot_Fric_First_Send_CAN_ID              0x200    //Fric_CAN发送ID
    #define Shoot_Fric_First_Right_Feedback_CAN_ID    0x202    //前右Fric_CAN反馈ID
    #define Shoot_Fric_First_Left_Feedback_CAN_ID     0x201    //前左Fric_CAN反馈ID
    #define Shoot_Fric_First_Middle_Feedback_CAN_ID   0x203    //前中Fric_CAN反馈ID
    
    #define Shoot_Fric_Second_CAN                      hfdcan3    //Fric_使用的CAN
    #define Shoot_Fric_Second_Send_CAN_ID              0x1FF    //Fric_CAN发送ID
    #define Shoot_Fric_Second_Right_Feedback_CAN_ID    0x206    //前右Fric_CAN反馈ID
    #define Shoot_Fric_Second_Left_Feedback_CAN_ID     0x207    //前左Fric_CAN反馈ID
    #define Shoot_Fric_Second_Middle_Feedback_CAN_ID   0x208    //前中Fric_CAN反馈ID

    #define Shoot_Trigger_CAN                   hfdcan2    //Trigger_使用的CAN
    #define Shoot_Trigger_Send_CAN_ID           0x1FF    //Trigger_CAN发送ID
    #define Shoot_Trigger_Feedback_CAN_ID       0x205    //Trigger_CAN反馈ID

/*===| 超电通信相关定义 |==*/
    #define SuperCap_CAN                        hfdcan1     //hcan1
    #define SuperCap_Control_CAN_ID             0x030     //超电控制CANID
    #define SuperCap_FeedBack_CAN_ID            0x031     //超电反馈CANID
    
/*===| 陀螺仪通信相关定义 |==*/
    #define IMU_CAN                             hfdcan3   //hcan3
    #define IMU_Control_CAN_ID                  0x001     //IMU控制CANID
    #define IMU_FeedBack_CAN_ID                 0x002     //IMU反馈CANID

/*===| 电机定义 |===*/
    #define Motor_3508_Reduction                16.71f
    #define Motor_2006_Reduction                36.0f

/*===| 底盘相关参数定义 |===*/
    #define Chassis_Wheel_Radius                0.06f   //轮半径
    #define Chassis_RPM_to_m_s                  2.0f*PI*Chassis_Wheel_Radius/(60.0f*Motor_3508_Reduction)  //底盘RPM转换成m/s的系数
    #define Robo_Vy_MaxSpeed                    4.0f     //机器最大Y速度
	#define Robo_Vx_MaxSpeed					4.0f	 //机器最大X速度
    #define Robo_Vx_MaxSpeed_SPIN               1.0f     //机器最大X速度
    #define Robo_Vy_MaxSpeed_SPIN               1.0f     //机器最大Y速度	
    #define Robo_Wz_MaxSpeed                    12.0f    //机器最大Z速度				
	
	#define Chassis_Acc                         15.0f     //底盘加速度
	#define Chassis_Dec                         15.0f     //底盘加速度
	#define Chassis_Acc_SPIN                    5.0f     //底盘加速度
    #define Chassis_Slow                        15.0f    //底盘减速度

	/*===| 实际力矩(Nm)=发送的力矩电流整数 * (20/16384) * 3508屁股力矩常数(0.01562Nm/A) * 减速比 |===*/  
	#define Current2Torque 					   3.186647037e-4f	//    (20/16384)              *    (0.3)    / (3591/187)  *  (16.71)
	#define Torque2Current 					   3.138094644e+3f  //电调最大持续电流/输出限幅      转矩系数   19.2减速比    16.71减速比
	#define Radain2Angle					   57.29578f
	#define Angle2Radain					   0.0174533f
	
	#define K_gear2chain                       1.0f
        
/*===| 云台相关参数定义 |===*/  
    #define Gimbal_Median_Yaw                 	0     //云台朝前时Yaw电机的编码值
    #define Gimbal_Median_Pitch                 0     //云台水平时Pitch电机的角度
 
    #define Gimbal_Pitch_Limit_Up               90.0f
    #define Gimbal_Pitch_Limit_Down             -90.0f
/*===| 发射相关参数定义 |===*/
    #define Fric_Acc                 0.1f
    #define Shoot_Trigger_Speed_Max  -20.0f
    #define Fric_Radius 			 0.03f
    #define Fric_RPM_to_m_s     	 (2*PI*Fric_Radius/60.0f)
    #define Triger_Reduction 		 (Motor_2006_Reduction * 55/23)


/*===| 底盘计算参数定义 |===*/
	#define    L1u  0.21
	#define    L1d  0.25
	#define    L2u  0.21
	#define    L2d  0.25
	
	#define 	MB    13
	#define 	RL_W  0.523
	#define 	Q_L_F 0 
	#define 	Q_L_B 1 
	#define 	Q_R_F 2 
	#define 	Q_R_B 3 
	
	#define 	S  	  0
	#define 	S_D   1
	#define 	YAW   2
	#define 	YAW_D 3
	#define 	QB    4
	#define 	QB_D  5
	
/*===| 电机ID，对应电机数据结构体数组下标 |===*/
    #define Chassis_DriverMotor1_ID     5
    #define Chassis_DriverMotor2_ID     6
	
	#define Chassis_Joint_ID        	0 
	#define Chassis_Joint1_ID      		1
    #define Chassis_Joint2_ID      		2
	#define Chassis_Joint3_ID      		3
    #define Chassis_Joint4_ID      		4
		
    #define Gimbal_Pitch_ID             5
    #define Gimbal_Yaw_ID          		6
    #define Shoot_Fric_First_Left_ID    7
    #define Shoot_Fric_First_Right_ID   8
    #define Shoot_Fric_First_Middle_ID  9
    #define Shoot_Trigger_ID            10
    #define Shoot_Fric_Second_Left_ID   11
    #define Shoot_Fric_Second_Right_ID  12
    #define Shoot_Fric_Second_Middle_ID 13
 
#endif
