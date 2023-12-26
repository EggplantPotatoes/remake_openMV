
#include "usb_vcp.h"
#include "stdarg.h"
#include "usbd_cdc_if.h"

//extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

void usb_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;
    uint8_t buf[APP_TX_DATA_SIZE];

    va_start(args, format);
    length = vsnprintf((char *)buf, APP_TX_DATA_SIZE, (char *)format, args);
    va_end(args);
    CDC_Transmit_FS(buf, length);
}

void usb_send_data(uint8_t* Buf, uint16_t Len)
{
	CDC_Transmit_FS(Buf,Len);

}
