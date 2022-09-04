#include "UI.h"
void Simple_UI(int8_t index,int8_t X,int8_t Y)
{
static int8_t Pr_index;
//´ó×´Ì¬À¸Ë¢ÐÂ
if(index!=Pr_index)
{	
		OLED_Clear();
		uint8_t anti_color[5] = {1,1,1,1,1};
		anti_color[index] = 0;
		OLED_Rectangle(0,0,127,15,1);
		OLED_ShowChar(60,3,UI_Main,8,anti_color[0]);
		OLED_ShowChar(80,3,UI_Connect,8,anti_color[1]);
		OLED_ShowChar(100,3,UI_Other,8,anti_color[2]); 
		OLED_ShowChar(20,3,UI_Setting,8,anti_color[3]);
		OLED_ShowChar(40,3,UI_Run,8,anti_color[4]);
		Pr_index = index;
		plate_control.Y=0;
}
OLED_Message_Clear();
switch(index)
{
	case MENU_Main:
				
				OLED_printf(5,20,8,1,"SBL");OLED_printf(30,20,8,1,"%4d",plate_control.Booster[0]);
				OLED_printf(5,35,8,1,"SBR");OLED_printf(30,35,8,1,"%4d",plate_control.Booster[1]);
				OLED_printf(5,50,8,1,"STG");OLED_printf(30,50,8,1,"%4d",plate_control.Triggle);
				OLED_printf(65,20,8,1,"RBL");OLED_printf(95,20,8,1,"%4d",-motor_status_ammobooster[0].speed_rpm);
				OLED_printf(65,35,8,1,"RBR");OLED_printf(95,35,8,1,"%4d",motor_status_ammobooster[1].speed_rpm);
				OLED_printf(65,50,8,1,"ALS");OLED_printf(95,50,8,1,"%4.1f",plate_control.speed_lt);
				if(KEY.up == 1){plate_control.index=4;KEY.up=0;}
			
	break;
	case MENU_Connect:
	if(KEY.up == 1){plate_control.index=0;KEY.up=0;}
	OLED_printf(30,30,12,1,"Hai mei zuo");
	break;
	case MENU_Other:
	if(KEY.up == 1){plate_control.index=1;KEY.up=0;}
	OLED_printf(0,20,12,1,"Designed by qianwan");
	OLED_printf(0,35,12,1,"Dalian University of technology");
	break;
	case MENU_Run:
	if(KEY.up == 1){plate_control.index=3;KEY.up=0;}
	if(KEY.mid == 1){plate_control.Y++;KEY.mid=0;if(plate_control.Y>3)plate_control.Y=0;}
			OLED_printf(30,25,8,1,"S-ONE");
			OLED_printf(30,40,8,1,"SAUTO");
			OLED_printf(30,55,8,1,"Count:%d",plate_control.count);
			OLED_printf(80,25,8,1,"BST");
			switch(plate_control.Y)
			{
				
				case 0:
					OLED_ShowChar(20,25,123,8,1);
					if(KEY.set){plate_control.shoot_one+=1;KEY.set=0;plate_control.shoot_auto=0;}
					if(plate_control.shoot_one>1){plate_control.shoot_one=0;}

				
				break;
				case 1:
					OLED_ShowChar(20,40,123,8,1);
					if(KEY.set){plate_control.shoot_auto+=1;KEY.set=0;plate_control.shoot_one=0;}
					if(plate_control.shoot_auto>1){plate_control.shoot_auto=0;}
				break;
				case 2:
					OLED_ShowChar(20,55,123,8,1);
				if(KEY.set){plate_control.count=0;KEY.set=0;}
				break;
				case 3:
					OLED_ShowChar(70,25,123,8,1);
				if(KEY.set){plate_control.triggle_on+=1;KEY.set=0;}
				if(plate_control.triggle_on>1){plate_control.triggle_on=0;plate_control.shoot_one=0;plate_control.shoot_auto=0;}
				break;
				
			
			}
			if(plate_control.triggle_on){OLED_printf(5,45,8,1,"B");}
			if(plate_control.shoot_one){OLED_printf(5,25,12,1,"S");}
			else if(plate_control.shoot_auto){OLED_printf(5,25,12,1,"A");}
	
	
	
	break;
	case MENU_Setting:
	if(KEY.up == 1){plate_control.index=2;KEY.up=0;}
	if(KEY.mid == 1){plate_control.Y++;KEY.mid=0;if(plate_control.Y>2)plate_control.Y=0;}
			OLED_printf(50,25,8,1,"SBL:%d",plate_control.Booster[0]);
			OLED_printf(50,40,8,1,"SBR:%d",plate_control.Booster[1]);
			OLED_printf(50,55,8,1,"STG:%d",plate_control.Triggle);
			
			switch(plate_control.Y)
			{
				
				case 0:
					OLED_ShowChar(40,25,123,8,1);
					if(KEY.set){plate_control.Booster[0]+=50;}
					if(plate_control.Booster[0]>8000){plate_control.Booster[0]=4000;}
					else if(plate_control.Booster[0]<4000){plate_control.Booster[0]=4000;}
				
				break;
				case 1:
					OLED_ShowChar(40,40,123,8,1);
					if(KEY.set){plate_control.Booster[1]+=50;}
					if(plate_control.Booster[1]>8000){plate_control.Booster[1]=4000;}
					else if(plate_control.Booster[1]<4000){plate_control.Booster[1]=4000;}
				break;
				case 2:
					OLED_ShowChar(40,55,123,8,1);
					if(KEY.set){plate_control.Triggle+=50;}
					if(plate_control.Triggle>2000){plate_control.Triggle=300;}
					else if(plate_control.Triggle<300){plate_control.Triggle=300;}
				break;
			
			}
			
	
	break;
	
	default:
	break;
}



}
