#ifndef UI_H
#define UI_H

#include "main.h"
#include "OLED.h"
#include "control.h"


//typedef struct
//{
//	uint8_t Cur_Index;//当前索引项
//	uint8_t previous;//上一页
//	uint8_t next;//下一页
//	uint8_t enter;//确认
//	uint8_t back;//返回
//	void (*current_operation)(uint8_t,uint8_t);//	当前索引执行的函数(界面)
//}Main_Menu;

////各界面的索引值
//enum
//{
//	_Main_UI=0,
//	_Run_Option,
//	_Ammo_Option,
//	_Connect_Option,
//	_Info_Option,
//	_Run_Child,
//	_Ammo_Child,
//	_Connect_Child,
//	_Info_Child,
//	_OLED_Lock,
//};

////按键索引值
//enum
//{
//	KEY_PREVIOUS=2,
//	KEY_ENTER,
//	KEY_NEXT,
//	KEY_BACK
//};

//void Main_UI(uint8_t page_index,uint8_t key_val);
//void Main_Menu_Func(uint8_t page_index,uint8_t key_val);
//void Ammo_Child(uint8_t page_index,uint8_t key_val);
//void Connect_Child(uint8_t page_index,uint8_t key_val);
//void Run_Child(uint8_t page_index,uint8_t key_val);
//void Info_Child(uint8_t page_index,uint8_t key_val);
//void OLED_Lock(uint8_t page_index,uint8_t key_val);
//void GUI_Refresh(void);
void Simple_UI(int8_t index,int8_t X,int8_t Y);

#endif