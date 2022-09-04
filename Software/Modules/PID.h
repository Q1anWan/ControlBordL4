#ifndef PID_H
#define PID_H
#include "main.h"
#ifdef __cplusplus
class PID_Inc_C
{
	//����ʽ
	public:
	void PID_Init(void);
	float PID_Cal(float fdb);
	float	Ref = 0.0f;
	protected:
	float 	Error = 0.0f; //���
	float 	FeedBack = 0.0f;	
	float 	DError = 0.0f;
	float 	DDError = 0.0f;
	float 	PreError = 0.0f;
	float 	PreDError = 0.0f;
		
	float Kp=0.0f; //pid����
	float Ki=0.0f;
	float Kd=0.0f;
	
	float IN_RANGE_EN_D = 0.0f;//����΢���Χ ֵΪ0ʱʼ�տ���
	float IN_RANGE_EN_I = 0.0f;//���������Χ ֵΪ0ʱʼ�տ���
	
	int16_t MaxOutValue=0; //����޷�
	int16_t MinOutValue=0;
	
	public:
	float Out = 0.0f; //���ֵ
};

class PID_Pla_C
{
	//λ��ʽ
	public:
	void PID_Init(void);
	float PID_Cal(float fdb);
	float	Ref = 0.0f;
	protected:
	float 	Error = 0.0f; //���
	float 	FeedBack = 0.0f;	
	float 	DError = 0.0f;

	float integral = 0.0f;//���Ļ���
	
	float Kp = 0.0f; //pid����
	float Ki = 0.0f;
	float Kd = 0.0f;
	
	float IN_RANGE_EN_D = 0.0f;//����΢���Χ ֵΪ0ʱʼ�տ���
	float IN_RANGE_EN_I = 0.0f;//���������Χ ֵΪ0ʱʼ�տ���
	
	int16_t MaxOutValue = 0; //����޷�
	int16_t MinOutValue = 0;

	float Maxintegral = 0.0f; //�����޷�
	float Minintegral = 0.0f;
	
	float Out = 0.0f; //���ֵ

};

#endif
#endif