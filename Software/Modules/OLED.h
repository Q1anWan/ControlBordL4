#ifndef _OLED_H_
#define _OLED_H_

#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include "arm_math.h"
#include "string.h"
#include "i2c.h"
//
#define OLED_I2C_ADDRESS    0x78

#define OLED_CMD        0x00
#define OLED_DATA       0x01

#define H 0//Ë®Æ½
#define V 1//ÊúÖ±



#define UI_Setting 129
#define UI_Run 128
#define UI_Main 131
#define UI_Connect 127
#define UI_Other 130



#define MENU_Setting 3
#define MENU_Run 4
#define MENU_Main 0
#define MENU_Connect 1
#define MENU_Other 2



#ifdef __cplusplus
extern "C"
{
#endif
extern void OLED_Init(void);

extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);

extern void OLED_SetPos(uint8_t x, uint8_t y);
extern void OLED_Refresh_Gram(void);
extern void OLED_Clear(void);
extern void OLED_Message_Clear(void);

extern void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t dot);
extern void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
extern void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str, uint8_t size, uint8_t mode);
extern void OLED_printf(uint8_t x, uint8_t y, uint8_t size,uint8_t mode,const char *fmt,...);
extern void OLED_BMP(const unsigned char bmp[128][8]);
extern void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
extern void OLED_line(uint8_t x,uint8_t y,int8_t length,uint8_t direction,uint8_t mode);
extern void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);

//extern void OLED_LOGO(void);



extern const unsigned char ascii_1206[95][12];
extern const unsigned char ascii_0806[][6];
//extern const unsigned char LOGO_ROBOMASTER[128][8];
extern const unsigned char LOGO_DLUT[128][8];
//extern const unsigned char LOGO1_DUT0BUG[128][8];
//extern const unsigned char LOGO2_DUT0BUG[128][8];
//extern const unsigned char BAD_APPLE[128][8];
//extern const unsigned char LOGO_DLUT[128][8];

#ifdef __cplusplus
}
#endif
#endif
