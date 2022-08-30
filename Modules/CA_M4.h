#ifndef __CA_H
#define __CA_H
/*****************************************************************
**                ��������ѧ ��BUGս��
**---------------------------------------------------------------
** ��Ŀ���ƣ�   CA�����㷨
** ��    �ڣ�   2022-03-07
** ��    �ߣ�   qianwan
** ��    ����   1.1
**---------------------------------------------------------------
** �� �� ����   CA.c
** ��������   arm_math.h
** �ļ����䣺   ��ԭPID.C��ȣ�PID��K����С10����D�Ŵ�100��
*****************************************************************/
#include "main.h"

/*---------------------VARIABLES---------------------*/
typedef struct _PID_Data
{
	//�������
	uint8_t type_selection;
		
	float	Ref;
	float 	FeedBack;	
	float 	Error; //���
	float 	DError;
	float 	DDError;
	float 	PreError;
	float 	PreDError;
	
	float integral;//���Ļ���
	
	float Ki_able_error_range;
	float Kd_able_error_range;
	
	float Kp; //pid����
	float Ki;
	float Kd;
	
	int32_t MaxOutValue; //����޷�
	int32_t MinOutValue;

	float Maxintegral; //�����޷�
	float Minintegral;
	
	float Out; //���ֵ
	
}PID_Data;

typedef struct _FWC_Data
{
	//�������
	float F;//Ƶ�ʣ�ʱ�䳣���ĵ���
	float M;//ת������ ��λ kg/mm~2
	float K;//����ϵ��
	
	float MKF;//�м�ϵ�����Զ�����
	int16_t Error;
	int16_t DInput;//��һ������
	int16_t Output;//��һ������
	
}FWC_Data;

/*---------------------DECLARES----------------------*/
void InitPID(void);
void InitFWC(void);
int32_t PIDCal(PID_Data *pid, float fdb);
int16_t FWCCal(FWC_Data *fwd, int16_t Input);
extern PID_Data Pid_gyrotemp;
extern FWC_Data FWC_YAW_Mouse,FWC_Pitch_Mouse,FWC_YAW_Tip;
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...); //���ڷ���
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value);
#endif
