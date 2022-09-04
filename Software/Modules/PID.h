#ifndef PID_H
#define PID_H
#include "main.h"
#ifdef __cplusplus
class PID_Inc_C
{
	//增量式
	public:
	void PID_Init(void);
	float PID_Cal(float fdb);
	float	Ref = 0.0f;
	protected:
	float 	Error = 0.0f; //误差
	float 	FeedBack = 0.0f;	
	float 	DError = 0.0f;
	float 	DDError = 0.0f;
	float 	PreError = 0.0f;
	float 	PreDError = 0.0f;
		
	float Kp=0.0f; //pid参数
	float Ki=0.0f;
	float Kd=0.0f;
	
	float IN_RANGE_EN_D = 0.0f;//开启微分项范围 值为0时始终开启
	float IN_RANGE_EN_I = 0.0f;//开启积分项范围 值为0时始终开启
	
	int16_t MaxOutValue=0; //输出限幅
	int16_t MinOutValue=0;
	
	public:
	float Out = 0.0f; //输出值
};

class PID_Pla_C
{
	//位置式
	public:
	void PID_Init(void);
	float PID_Cal(float fdb);
	float	Ref = 0.0f;
	protected:
	float 	Error = 0.0f; //误差
	float 	FeedBack = 0.0f;	
	float 	DError = 0.0f;

	float integral = 0.0f;//误差的积分
	
	float Kp = 0.0f; //pid参数
	float Ki = 0.0f;
	float Kd = 0.0f;
	
	float IN_RANGE_EN_D = 0.0f;//开启微分项范围 值为0时始终开启
	float IN_RANGE_EN_I = 0.0f;//开启积分项范围 值为0时始终开启
	
	int16_t MaxOutValue = 0; //输出限幅
	int16_t MinOutValue = 0;

	float Maxintegral = 0.0f; //积分限幅
	float Minintegral = 0.0f;
	
	float Out = 0.0f; //输出值

};

#endif
#endif