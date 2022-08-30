#ifndef __CA_H
#define __CA_H
/*****************************************************************
**                大连理工大学 凌BUG战队
**---------------------------------------------------------------
** 项目名称：   CA控制算法
** 日    期：   2022-03-07
** 作    者：   qianwan
** 版    本：   1.1
**---------------------------------------------------------------
** 文 件 名：   CA.c
** 代码需求：   arm_math.h
** 文件补充：   与原PID.C相比，PID的K需缩小10倍，D放大100倍
*****************************************************************/
#include "main.h"

/*---------------------VARIABLES---------------------*/
typedef struct _PID_Data
{
	//类别设置
	uint8_t type_selection;
		
	float	Ref;
	float 	FeedBack;	
	float 	Error; //误差
	float 	DError;
	float 	DDError;
	float 	PreError;
	float 	PreDError;
	
	float integral;//误差的积分
	
	float Ki_able_error_range;
	float Kd_able_error_range;
	
	float Kp; //pid参数
	float Ki;
	float Kd;
	
	int32_t MaxOutValue; //输出限幅
	int32_t MinOutValue;

	float Maxintegral; //积分限幅
	float Minintegral;
	
	float Out; //输出值
	
}PID_Data;

typedef struct _FWC_Data
{
	//类别设置
	float F;//频率，时间常数的倒数
	float M;//转动惯量 单位 kg/mm~2
	float K;//调节系数
	
	float MKF;//中间系数，自动计算
	int16_t Error;
	int16_t DInput;//上一次输入
	int16_t Output;//上一次输入
	
}FWC_Data;

/*---------------------DECLARES----------------------*/
void InitPID(void);
void InitFWC(void);
int32_t PIDCal(PID_Data *pid, float fdb);
int16_t FWCCal(FWC_Data *fwd, int16_t Input);
extern PID_Data Pid_gyrotemp;
extern FWC_Data FWC_YAW_Mouse,FWC_Pitch_Mouse,FWC_YAW_Tip;
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...); //串口发送
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value);
#endif
