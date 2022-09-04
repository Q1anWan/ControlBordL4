#include "OLED.h" 
#include "OLEDfont.h"
//OLED的显存
uint8_t OLED_GRAM[128][8];

//双色OLED在16到17行出现断层（1/4）
//向SSD1306写入一个字节
void oled_write_byte(uint8_t ctrl, uint8_t dat)
{
	uint8_t buf[2];

	if(ctrl==OLED_CMD) {buf[0]=0x00;}
	else if(ctrl==OLED_DATA) {buf[0]=0x40;}
	buf[1]=dat;
	
	HAL_I2C_Master_Transmit(&hi2c3, OLED_I2C_ADDRESS, buf, 2, 100);
}

//初始化OLED模块
void OLED_Init(void)
{
	oled_write_byte(OLED_CMD, 0xAE); //Display OFF
	oled_write_byte(OLED_CMD, 0x20); oled_write_byte(OLED_CMD,0x02); //Set Memory Addressing Mode--0x00,Horizontal Addressing Mode;0x01,Vertical Addressing Mode;0x02,Page Addressing Mode;0x03,Invalid
	oled_write_byte(OLED_CMD, 0x00); //set Lower Column Start Address
	oled_write_byte(OLED_CMD, 0x10); //set higher Column Start Address
	oled_write_byte(OLED_CMD, 0xB0); //Set Page Start Address
	oled_write_byte(OLED_CMD, 0xA8); oled_write_byte(OLED_CMD, 0x3F); //Set Multiplex Ratio--63
	oled_write_byte(OLED_CMD, 0xD3); oled_write_byte(OLED_CMD, 0x00); //Set Display Offset--0
	oled_write_byte(OLED_CMD, 0x40); //Set Display Start Line--0
	oled_write_byte(OLED_CMD, 0xA1); //Set Segment Re-map--reverse
	oled_write_byte(OLED_CMD, 0xC8); //Set COM Output Scan Direction--reverse
	oled_write_byte(OLED_CMD, 0xDA); oled_write_byte(OLED_CMD, 0x12); //Set COM Pins Hardware Configuration
	oled_write_byte(OLED_CMD, 0x81); oled_write_byte(OLED_CMD, 0xFF); //set Contrast Control 0x00~0xFF
	oled_write_byte(OLED_CMD, 0xA4); //Disable Entire Display On
	oled_write_byte(OLED_CMD, 0xA6); //Set Normal Display
	oled_write_byte(OLED_CMD, 0xD5); oled_write_byte(OLED_CMD, 0xF0); //set Oscillator Frequency/Display Clock Divide Ratio
	oled_write_byte(OLED_CMD, 0xD9); oled_write_byte(OLED_CMD, 0x22); //Set Pre-charge Period
	oled_write_byte(OLED_CMD, 0xDB); oled_write_byte(OLED_CMD, 0x20); //Set Vcomh Deselect Level
	oled_write_byte(OLED_CMD, 0x8D); oled_write_byte(OLED_CMD, 0x14); //Set Charge Pump Enable
	oled_write_byte(OLED_CMD, 0xAF); //Display ON	
}

//开启OLED显示
void OLED_Display_On(void)
{
	oled_write_byte(OLED_CMD, 0x8D); oled_write_byte(OLED_CMD, 0x14); //Set Charge Pump Enable
	oled_write_byte(OLED_CMD, 0xAF); //Display ON
}

//关闭OLED显示
void OLED_Display_Off(void)
{
	oled_write_byte(OLED_CMD, 0x8D); oled_write_byte(OLED_CMD, 0x10); //Set Charge Pump Disable
	oled_write_byte(OLED_CMD, 0xAE); //Display OFF
}

//设置光标起点,横轴x,纵轴y
void OLED_SetPos(uint8_t x, uint8_t y)
{
	oled_write_byte(OLED_CMD, (0xB0+y)); //set page address y
	oled_write_byte(OLED_CMD, ((x&0xF0)>>4)|0x10); //set column high address
	oled_write_byte(OLED_CMD, (x&0x0F)); //set column low address
}

//更新显存到OLED
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;

	for(i=0;i<8;i++)
	{
		OLED_SetPos(0,i);
		for(n=0;n<128;n++)
		{
			oled_write_byte(OLED_DATA, OLED_GRAM[n][i]);
		}
	}
}

//清屏
void OLED_Clear(void)
{
	uint8_t i,n;

	for(i=0;i<8;i++)
	{
		for(n=0;n<128;n++)
		{
			OLED_GRAM[n][i]=0x00;
		}
	}
	//OLED_Refresh_Gram();//更新显示
}

void OLED_Message_Clear(void)
{
	uint8_t i,n;

	for(i=3;i<8;i++)
	{
		for(n=0;n<128;n++)
		{
			OLED_GRAM[n][i]=0x00;
		}
	}
	//OLED_Refresh_Gram();//更新显示
}

//画点
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t dot)
{
	uint8_t page,row;

	if((x>127)||(y>63)) {return;}
	
	page = y/8;
	row = y%8;

	if(dot==1)
	{
		OLED_GRAM[x][page] |= (1<<row);
	}
	else if(dot==0)
	{
		OLED_GRAM[x][page] &= ~(1<<row);
	}
}

//显示一个字符
//size: 8/12
//mode: 0/1 	0取反 1正常
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
	uint16_t tmp;
	uint8_t i, j, index;
	
	index = chr-' ';
	switch(size) {
		case 12:
		for(i=0;i<6;i++)
		{
			tmp = (uint16_t)((ascii_1206[index][2*i]<<8)+ascii_1206[index][2*i+1]);
			
			for(j=0;j<12;j++)
			{
				if(tmp&0x8000) {OLED_DrawPoint(x+i, y+j, mode);}
				else {OLED_DrawPoint(x+i, y+j, !mode);} 
				
				tmp<<=1;
			}
		}
		break;
		
		case 8:
		for(i=0;i<6;i++)
		{
			tmp = (uint16_t)((ascii_0806[index][5-i]<<8)+ascii_0806[index][6-i]);		
			for(j=0;j<8;j++)
			{
				if(tmp&0x8000) {OLED_DrawPoint(x+7-i, y+7-j, mode);}
				else {OLED_DrawPoint(x+7-i, y+7-j, !mode);} 
				
				tmp<<=1;
			}
		}
		break;
	}}
//显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str, uint8_t size, uint8_t mode)
{
	uint16_t n=0;

	while(str[n]!='\0')
	{
		OLED_ShowChar(x, y, str[n], size, mode);
		
		x+=6;
		if(x>120){x=0;y+=12;}
		
		n++;
	}
}


void OLED_printf(uint8_t x, uint8_t y, uint8_t size,uint8_t mode,const char *fmt,...)
{
    static uint8_t oled_buf[128]={0};
    static va_list ap;
    static uint16_t len=0;

    va_start(ap, fmt);
    len = vsprintf((char *)oled_buf, fmt, ap);
    va_end(ap);
			
    oled_buf[len] = '\0';
    OLED_ShowString(x, y, oled_buf, size ,mode);
		
}




//显示LOGO图片 ~90msunsigned char
//void OLED_LOGO(void)	
//{
//	uint8_t tmp;
//	uint8_t x, y, i;
//	
//	OLED_Clear();
//	for(y=0;y<64;y+=8)
//	{
//		for(x=0;x<128;x++)
//		{
//			tmp = LOGO2_DUT0BUG[x][y/8];
//		
//			for(i=0;i<8;i++)
//			{
//				if(tmp&0x80) {OLED_DrawPoint(x, y+i, 1);}
//				else {OLED_DrawPoint(x, y+i, 0);}
//				tmp<<=1;
//			}
//		}
//	}
//	OLED_Refresh_Gram();//更新显示
//}

void OLED_BMP(const unsigned char bmp[128][8])//显示某张图片，图片需要在oled.h中给个extern声明	
{
	uint8_t tmp;
	uint8_t x, y, i;
	
	OLED_Clear();
	
	for(y=0;y<64;y+=8)
	{
		for(x=0;x<128;x++)
		{
			tmp = bmp[x][y/8];
		
			for(i=0;i<8;i++)
			{
				if(tmp&0x80) {OLED_DrawPoint(x, y+i, 1);}
				else {OLED_DrawPoint(x, y+i, 0);}
				tmp<<=1;
			}
		}
	}
	
	OLED_Refresh_Gram();//更新显示
}


//填充或者清除区域
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)	
{
	uint8_t x_L,x_H,y_L,y_H,i=0,j=0;
	if(x1>x2){x_H = x1;x_L = x2;}
	else{x_H = x2;x_L = x1;}
	if(y1>y2){y_H = y1;y_L = y2;}
	else{y_H = y2;y_L = y1;}
	if(x_H > 127 || y_H > 63){return;}
	
	for(i=x_L;i<=x_H;i++)
	{
		for(j=y_L;j<=y_H;j++)
		{
			OLED_DrawPoint(i, j, mode);
		}
	}	
}
//x1,y1确定起始坐标
//direction确定方向 ↑

void OLED_line(uint8_t x,uint8_t y,int8_t length,uint8_t direction,uint8_t mode)
{
	uint8_t x_L,x_H,y_L,y_H,i=0;
	if(length == 0){OLED_DrawPoint(x,y,mode);return;}
	if(x>127){x=127;}
	if(y>63){x=63;}
	switch(direction)
	{
		case H:
			if(x+length>127){x_H = 127;x_L = x;}
			else if(x-length>127){x_H = x;x_L = 0;}
			else{if(length > 0){x_H = length+x;x_L = x;}
			else{x_H = x;x_L = length+x;}}
			for(i=x_L;i<=x_H;i++)
			{OLED_DrawPoint(i, y, mode);}
			return;		
		case V:
			if(y+length>63){y_H = 63;y_L = y;}
			else if(y-length>63){y_H = y;y_L = 0;}
			else{if(length > 0){y_H = length+y;y_L = y;}
			else{y_H = y;y_L = length+y;break;}}
			for(i=y_L;i<=y_H;i++)
			{OLED_DrawPoint(x, i, mode);}
			return;
		default:return;
	}
}


void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)	
{
	OLED_Fill(x1,y1,x1,y2,mode);
	OLED_Fill(x2,y1,x2,y2,mode);
	OLED_Fill(x1,y1,x2,y1,mode);
	OLED_Fill(x1,y2,x2,y2,mode);
}
