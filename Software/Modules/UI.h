#ifndef UI_H
#define UI_H

#include "main.h"
#include "OLED.h"
#include "control.h"


//typedef struct
//{
//	uint8_t Cur_Index;//��ǰ������
//	uint8_t previous;//��һҳ
//	uint8_t next;//��һҳ
//	uint8_t enter;//ȷ��
//	uint8_t back;//����
//	void (*current_operation)(uint8_t,uint8_t);//	��ǰ����ִ�еĺ���(����)
//}Main_Menu;

////�����������ֵ
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

////��������ֵ
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