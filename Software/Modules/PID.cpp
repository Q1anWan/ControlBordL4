#include "PID.h"
#include "arm_math.h"

/*增量式PID*/
float PID_Inc_C::PID_Cal(float fdb)
{
	/*中间量*/
	float Kp =0.0f;
	float Ki =0.0f;
	float Kd =0.0f;
	float OUT =0.0f;
	
	/*前期准备*/
	this->FeedBack = fdb;
	//pid->Error = pid->Ref - pid->FeedBack;
	arm_sub_f32(&this->Ref,&this->FeedBack,&this->Error,1);
	//pid->DError = pid->Error - pid->PreError;
	arm_sub_f32(&this->Error,&this->PreError,&this->DError,1);
	//pid->DDError = pid->DError - pid->PreDError;
	arm_sub_f32(&this->DError,&this->PreDError,&this->DDError,1);
			
	this->PreError = this->Error;
	this->PreDError = this->DError;
			
	/*比例积分微分运算*/
	//pid->Out = pid->Out + (pid->Kp * pid->DError + pid->Ki * pid->Error + pid->Kd * pid->DDError);
	//P 	
	arm_mult_f32(&this->Kp,&this->DError,&Kp,1);			
	//I 积分分离
	if((fabs(this->Error)<this->IN_RANGE_EN_I)||!IN_RANGE_EN_I)
	{arm_mult_f32(&this->Ki,&this->Error,&Ki,1);}		
	//D 微分分离
	if((fabs(this->Error)<this->IN_RANGE_EN_D)||!IN_RANGE_EN_D)
	{arm_mult_f32(&this->Kd,&this->DDError,&Kd,1);}	
	//求和
	arm_add_f32(&this->Out,&Kp,&OUT,1);
	arm_add_f32(&Ki,&Kd,&Ki,1);
	arm_add_f32(&OUT,&Ki,&OUT,1);			
	
	/*后期处理*/
	//输出限幅
	OUT = (OUT > this->MaxOutValue)?this->MaxOutValue:OUT;
	OUT = (OUT < this->MinOutValue)?this->MinOutValue:OUT;
	//赋值
	this->Out = OUT;
	return OUT;
}
	
/*位置式PID*/
float PID_Pla_C::PID_Cal(float fdb)
{
	/*中间量*/
	float Kp =0.0f;
	float Ki =0.0f;
	float Kd =0.0f;
	float OUT =0.0f;
	
	/*前期准备*/
	this->FeedBack = fdb;
	//pid->Error = pid->Ref - pid->FeedBack;
	arm_sub_f32(&this->Ref,&this->FeedBack,&this->Error,1);	
	//pid->integral	+=	pid->Error;
	arm_add_f32(&this->integral,&this->Error,&OUT,1);
	//积分限幅
	this->integral = (OUT > this->Maxintegral)?this->Maxintegral:Out;
	this->integral = (OUT < this->Minintegral)?this->Minintegral:Out;
	
	/*比例积分微分运算*/
	//pid->Out = pid->Kp * pid->Error + pid->Ki * pid->integral  + pid->Kd * (pid->Error - pid->DError);		
	//P			
	arm_mult_f32(&this->Kp,&this->Error,&Kp,1);			
	//I 积分分离
	if((fabs(this->Error)<this->IN_RANGE_EN_I)||!IN_RANGE_EN_I)	
	{arm_mult_f32(&this->Ki,&this->integral,&Ki,1);}		
	//D 微分分离
	if((fabs(this->Error)<this->IN_RANGE_EN_D)||!IN_RANGE_EN_D){
	arm_sub_f32(&this->Error,&this->DError,&OUT,1);
	arm_mult_f32(&this->Kd,&OUT,&Kd,1);}		
	//求和
	arm_add_f32(&Ki,&Kd,&Ki,1);
	arm_add_f32(&Kp,&Ki,&OUT,1);	
			
	/*后期处理*/
	//输出限幅
	OUT = (OUT > this->MaxOutValue)?this->MaxOutValue:OUT;
	OUT = (OUT < this->MinOutValue)?this->MinOutValue:OUT;
	
	this->DError = this->Error;
	
	this->Out = OUT;
	return OUT;
}
