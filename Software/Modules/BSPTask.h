#ifndef _BSPTASK_H
#define _BSPTASK_H
#include "main.h"
#include "cmsis_os.h"
#ifdef __cplusplus

extern "C"
{
	void RGB_Task(void const * argument);
	void KeyScan_Task(void const * argument);
	void SysInf_Task(void const * argument);
	void OLED_Task(void const * argument);
}
void usb_printf(const char *fmt,...);
#endif
#endif
