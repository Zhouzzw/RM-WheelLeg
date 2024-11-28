//========================================================
//                      Commnuicate[双板通信]
//      双板通信的发送和解包
//      当前有；云台Vx,Vy,Wz,YawErr，以及一些状态标志位
//========================================================

#include "Communicate.h"
#include "RoboControl.h"
#include "INS.h"
#include "Shoot.h"
#include "SuperCap_Driver.h"

/*===| 通信错误看门狗---如果10ms内没有收到双板通信数据，则重置底盘状态 |===*/
uint8_t Board_Commnuicate_Error_Ticker;


/**
 * @brief 通信任务
 */
void Communicate_Task(void *argument)
{
    osDelay(50);
    
    for(;;)
    {
        /*===| 发送超电控制帧 |===*/
        SuperCap_Send_Control_Data();
        
        osDelay(10);
    }

        
}



