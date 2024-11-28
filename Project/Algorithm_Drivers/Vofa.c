//===================================================
//                    Voif[Vofa调试]
//===================================================

#include "VOFA.h"

#include "RoboControl.h"


/**
 * @brief VOFA串口接收中断回调函数
 *
 * @param 串口收到的数组
 */
void VOFA_Rx_CallBack(uint8_t *VOFA_RxDataBuff)
{
	float VOFA_RX_Float =   (VOFA_RxDataBuff[3]-0x30)*10000+
                            (VOFA_RxDataBuff[4]-0x30)*1000+
                            (VOFA_RxDataBuff[5]-0x30)*100+
                            (VOFA_RxDataBuff[6]-0x30)*10+
                            (VOFA_RxDataBuff[7]-0x30)*1+
                            (VOFA_RxDataBuff[9]-0x30)*0.1f+
                            (VOFA_RxDataBuff[10]-0x30)*0.01f+
                            (VOFA_RxDataBuff[11]-0x30)*0.001f;
//    if(VOFA_RxDataBuff[2] == '-') VOFA_RX_Float = -VOFA_RX_Float;
//	
//    
//	if		(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='P') 
//    {
//        Gimbal_Control_Struct.Yaw_Angle_PID_Struct.Kp = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='D')
//    {
//        Gimbal_Control_Struct.Yaw_Angle_PID_Struct.Kd = VOFA_RX_Float;
//    }
//    else if	(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='F')
//    {
//        Gimbal_Control_Struct.Yaw_Angle_PID_Struct.Kf = VOFA_RX_Float;
//    }
//    else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='P') 
//    {
//        Gimbal_Control_Struct.Yaw_Speed_PID_Struct.Kp = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='D')
//    {
//        Gimbal_Control_Struct.Yaw_Speed_PID_Struct.Kd = VOFA_RX_Float;
//    }
//    else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='F')
//    {
//        Gimbal_Control_Struct.Yaw_Speed_PID_Struct.Kf = VOFA_RX_Float;
//    }
//    

//	if		(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='P') 
//    {
//        Shoot_Control_Struct.Fric_Left_Target_Speed = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='I')
//    {
//        Shoot_Control_Struct.Fric_Right_Target_Speed = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='D')
//    {
//        Shoot_Control_Struct.Fric_Middle_Target_Speed = VOFA_RX_Float;
//    }

//	if		(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='P') 
//    {
//        Shoot_Control_Struct.Trigger_Angle_PID_Struct.Kp = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='I')
//    {
//        Shoot_Control_Struct.Trigger_Angle_PID_Struct.Ki = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='D')
//    {
//        Shoot_Control_Struct.Trigger_Angle_PID_Struct.Kd = VOFA_RX_Float;
//    }
//    else if	(VOFA_RxDataBuff[0]=='1' && VOFA_RxDataBuff[1]=='F')
//    {
//        Shoot_Control_Struct.Trigger_Angle_PID_Struct.I_Output_Max = VOFA_RX_Float;
//    }
//    
//    else if		(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='P') 
//    {
//        Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct.Kp = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='I')
//    {
//        Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct.Ki = VOFA_RX_Float;
//    }
//	else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='D')
//    {
//        Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct.Kd = VOFA_RX_Float;
//    }
//    else if	(VOFA_RxDataBuff[0]=='2' && VOFA_RxDataBuff[1]=='F')
//    {
//        Shoot_Control_Struct.Trigger_Single_Speed_PID_Struct.I_Output_Max = VOFA_RX_Float;
//    }
}

