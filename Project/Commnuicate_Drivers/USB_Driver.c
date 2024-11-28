#include "USB_Driver.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

/**
 * @brief USB发送数据
 */
void USB_Send(uint8_t *Data, uint8_t Length)
{
    CDC_Transmit_FS(Data, Length);
}


/**
 * @brief USB接收数据回调函数
 */
void USB_ReceiveCallback(uint8_t *buf, uint32_t len)
{
    
}

/**
 * @brief USB_Printf
 */
uint8_t USB_Printf_Buff[128];
void USB_Printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)USB_Printf_Buff, fmt, ap);

    va_end(ap);

    CDC_Transmit_FS(USB_Printf_Buff, len);
}
