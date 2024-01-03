
#ifndef _USB_BSP_H
#define _USB_BSP_H
#include "main.h"

void usb_printf(const char *format, ...);
void usb_send_data(uint8_t* Buf, uint16_t Len);

#endif

