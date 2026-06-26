//=====================================================================================
//                    USART_Driver[USART通信驱动]
//      包含了一些USART通信相关的封装好的函数
//
//      USART初始化 USART_Init(void)
//      USART发送 USART1/3/6_Printf(char *format, ...)
//      USART接收事件回调函数统一处理串口收到的数据
//=====================================================================================

#include "USART_Driver.h"
#include "Vofa.h"
#include "Remote_Control.h"
#include "Referee_Unpack.h"
#include "Laser.h"

uint8_t RS485_1_RX_Data[64]     = {0};
uint8_t RS485_2_RX_Data[64]     = {0};
uint8_t UART4_RX_Data[64]       = {0};
uint8_t UART5_RX_Data[256]       = {0};
uint8_t SBUS_RX_Data[64]        = {0};

/**
 * @brief USART接收空闲事件回调函数
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == RS485_1_UART)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(RS485_1_UART, RS485_1_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(RS485_1_UART_DMA, DMA_IT_HT);
	}
    else if(huart == RS485_2_UART)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(RS485_2_UART, RS485_2_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(RS485_2_UART_DMA, DMA_IT_HT);
	}
    else if(huart == &huart4)
	{
//        Laser_GetData_Call_Back(UART4_RX_Data);
        
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, UART4_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(UART4_DMA, DMA_IT_HT);
	}
    else if(huart == &huart5)
	{
//        VOFA_Rx_CallBack(UART5_RX_Data);
		Referee_Data_Unpack(UART5_RX_Data);
//        Laser_GetData_Call_Back(UART5_RX_Data);
        
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, UART5_RX_Data, 256);
        __HAL_DMA_DISABLE_IT(UART5_DMA, DMA_IT_HT);
	}
    else if(huart == SBUS_UART)
	{
//        Remote_Control_Rx_CallBack(SBUS_RX_Data);
//        
//		HAL_UARTEx_ReceiveToIdle_DMA(SBUS_UART, SBUS_RX_Data, 64);
//        __HAL_DMA_DISABLE_IT(SBUS_UART_DMA, DMA_IT_HT);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart == RS485_1_UART)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(RS485_1_UART, RS485_1_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(RS485_1_UART_DMA, DMA_IT_HT);
	}
    else if(huart == RS485_2_UART)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(RS485_2_UART, RS485_2_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(RS485_2_UART_DMA, DMA_IT_HT);
	}
    else if(huart == &huart4)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, UART4_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(UART4_DMA, DMA_IT_HT);
	}
    else if(huart == &huart5)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, UART5_RX_Data, 256);
        __HAL_DMA_DISABLE_IT(UART5_DMA, DMA_IT_HT);
	}
    else if(huart == SBUS_UART)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(SBUS_UART, SBUS_RX_Data, 64);
        __HAL_DMA_DISABLE_IT(SBUS_UART_DMA, DMA_IT_HT);
	}
}

/**
 * @brief 串口初始化
 *
 * @param 无
 */
void USART_Init(void)
{    
    HAL_UARTEx_ReceiveToIdle_DMA(RS485_1_UART, RS485_1_RX_Data, 64);
    HAL_UARTEx_ReceiveToIdle_DMA(RS485_2_UART, RS485_2_RX_Data, 64);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, UART4_RX_Data, 64);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart5, UART5_RX_Data, 256);
    HAL_UARTEx_ReceiveToIdle_DMA(SBUS_UART, SBUS_RX_Data, 64);
    
    __HAL_DMA_DISABLE_IT(RS485_1_UART_DMA, DMA_IT_HT);
    __HAL_DMA_DISABLE_IT(RS485_2_UART_DMA, DMA_IT_HT);
    __HAL_DMA_DISABLE_IT(SBUS_UART_DMA, DMA_IT_HT);
    __HAL_DMA_DISABLE_IT(UART4_DMA, DMA_IT_HT);
    __HAL_DMA_DISABLE_IT(UART5_DMA, DMA_IT_HT);
}

uint8_t UART4_Printf_Buff[128];
void UART4_Printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)UART4_Printf_Buff, fmt, ap);

    va_end(ap);

    HAL_UART_Transmit(&huart4, UART4_Printf_Buff, len, 1000);
}

uint8_t UART5_Printf_Buff[128];
void UART5_Printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)UART5_Printf_Buff, fmt, ap);

    va_end(ap);

    HAL_UART_Transmit(&huart5, UART5_Printf_Buff, len, 1000);
}
