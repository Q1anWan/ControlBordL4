/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** ????????   CA??????
** ??    ???   2022-03-07
** ??    ???   qianwan
** ??    ????   1.1
**---------------------------------------------------------------
** ?? ?? ????   CA.c
** ????????   arm_math.h
** ???????   ?PID.C????PID??P????§³100????K??§³1000??
*****************************************************************/

/*---------------------INCLUDES----------------------*/
#include "stdio.h"
#include "CA.h"
#include "arm_math.h"
#include "tim.h"
#include "string.h"
#include "stdarg.h"
/*---------------------VARIABLES---------------------*/
PID_Data Pid_gyrotemp;
FWC_Data FWC_YAW_Mouse,FWC_Pitch_Mouse,FWC_YAW_Tip;

/***********************************************************************
** ?? ?? ???? InitPID()
** ????????? PID?????????
**---------------------------------------------------------------------
** ????????? ??
** ????????? ??
***********************************************************************/
void InitPID(void)
{
	//?????????
			Pid_gyrotemp.type_selection = 0;
			Pid_gyrotemp.Ref =4500.0f;
			Pid_gyrotemp.FeedBack = 0.0f;
			Pid_gyrotemp.Error = 0.0f ;
			Pid_gyrotemp.DError = 0.0f ;
			Pid_gyrotemp.DDError = 0.0f ;
			Pid_gyrotemp.PreError = 0.0f ;
			Pid_gyrotemp.PreDError = 0.0f ;
			Pid_gyrotemp.Kp = 3.0f;
			Pid_gyrotemp.Ki = 0.14f;
			Pid_gyrotemp.Kd = 0.0f;
			Pid_gyrotemp.Ki_able_error_range = 200.0f;//100.0f;
			Pid_gyrotemp.Kd_able_error_range = 100.0f;//50.0f;
			Pid_gyrotemp.MaxOutValue = 1200.0f;
			Pid_gyrotemp.MinOutValue = -400.0f;
			Pid_gyrotemp.Out = 0.0f;
}

/***********************************************************************
** ?? ?? ???? InitFWC()
** ????????? ???????????????
**---------------------------------------------------------------------
** ????????? ??
** ????????? ??
***********************************************************************/
void InitFWC(void)
{
		//???????Yaw?????????
		FWC_YAW_Mouse.F = 0.0f;//???
		FWC_YAW_Mouse.M = 0.0f;//YAW?????????????? ??¦Ë kg/mm~2
		FWC_YAW_Mouse.K = 0.0f;//???????
		FWC_YAW_Mouse.MKF = FWC_YAW_Mouse.F*FWC_YAW_Mouse.K*FWC_YAW_Mouse.M*0.0001f;
		
		//§³?????Yaw?????????
		FWC_YAW_Tip.F = 0.0f;//???
		FWC_YAW_Tip.M = 0.0f;//YAW?????????????? ??¦Ë kg/mm~2
		FWC_YAW_Tip.K = 0.0f;//???????
		FWC_YAW_Tip.MKF = FWC_YAW_Tip.F*FWC_YAW_Tip.K*FWC_YAW_Tip.M*0.0001f;
		
		//???????Pitch?????????
		FWC_Pitch_Mouse.F = 0.0f;//???
		FWC_Pitch_Mouse.M = 0.0f;//Pitch?????????????? ??¦Ë kg/mm~2
		FWC_Pitch_Mouse.K = 0.0f;//???????
		FWC_Pitch_Mouse.MKF = FWC_Pitch_Mouse.F*FWC_Pitch_Mouse.K*FWC_Pitch_Mouse.M*0.0001f;			
}

/***********************************************************************
** ?? ?? ???? PIDCal()
** ????????? ???PID??????
**---------------------------------------------------------------------
** ????????? PID???????‰]?????
** ????????? PID??????
***********************************************************************/
int32_t PIDCal(PID_Data *pid, float fdb)
{
			float Kp =0.0f;
			float Ki =0.0f;
			float Kd =0.0f;
	switch(pid->type_selection)
		{
		case 0://?????
			
			pid->FeedBack = fdb;
			//pid->Error = pid->Ref - pid->FeedBack;
			arm_sub_f32(&pid->Ref,&pid->FeedBack,&pid->Error,1);
			//pid->DError = pid->Error - pid->PreError;
			arm_sub_f32(&pid->Error,&pid->PreError,&pid->DError,1);
			//pid->DDError = pid->DError - pid->PreDError;
			arm_sub_f32(&pid->DError,&pid->PreDError,&pid->DDError,1);
			
			pid->PreError = pid->Error;
			
			pid->PreDError = pid->DError;
			
			//???Ú…???????????????????????
			//PID???????
		
		//P????		
			arm_mult_f32(&pid->Kp,&pid->DError,&Kp,1);			
		//I???????????	
			if((fabs(pid->Error)<pid->Ki_able_error_range)||!pid->Ki_able_error_range)
				{arm_mult_f32(&pid->Ki,&pid->Error,&Ki,1);}		
		//D??????§³????	
			if((fabs(pid->Error)>pid->Kd_able_error_range)||!pid->Kd_able_error_range)
				{arm_mult_f32(&pid->Kd,&pid->DDError,&Kd,1);}
			
			//??????
			arm_add_f32(&Ki,&Kd,&Ki,1);
			arm_add_f32(&pid->Out,&Kp,&pid->Out,1);
			arm_add_f32(&pid->Out,&Ki,&pid->Out,1);			
			//pid->Out += (pid->Kp * pid->DError + pid->Ki * pid->Error  + pid->Kd * pid->DDError );//?????????
			
			//??????
			if(pid->Out > pid->MaxOutValue)
			{pid->Out = (pid->MaxOutValue);}
			else if(pid->Out < pid->MinOutValue)
			{pid->Out = pid->MinOutValue;}			
		break;
		
		case 1://¦Ë???
			pid->FeedBack = fdb;
			//pid->Error = pid->Ref - pid->FeedBack;
			arm_sub_f32(&pid->Ref,&pid->FeedBack,&pid->Error,1);	
			
			//pid->integral	+=	pid->Error;
			arm_add_f32(&pid->integral,&pid->Error,&pid->integral,1);
		
			//???????
			if(pid->integral > (pid->Maxintegral))
			{pid->integral = (pid->Maxintegral);}
			else if(pid->integral < (pid->Minintegral))
			{pid->integral = (pid->Minintegral);}	
			
			//pid->Out = pid->Kp * pid->Error + pid->Ki * pid->integral  + pid->Kd * (pid->Error - pid->DError);		
			//P????			
			arm_mult_f32(&pid->Kp,&pid->Error,&Kp,1);			
		//I??????§³?????	
			if((fabs(pid->Error)<pid->Ki_able_error_range)||!pid->Ki_able_error_range)
			{arm_mult_f32(&pid->Ki,&pid->integral,&Ki,1);}		
		//D??????§³???????	
			if((fabs(pid->Error)<pid->Kd_able_error_range)||!pid->Kd_able_error_range)
			{
				arm_sub_f32(&pid->Error,&pid->DError,&pid->DDError,1);
				arm_mult_f32(&pid->Kd,&pid->DDError,&Kd,1);}		
			//??????
			
			arm_add_f32(&Ki,&Kd,&Ki,1);
			arm_add_f32(&Kp,&Ki,&pid->Out,1);	
			
			pid->DError = pid->Error;
			
			//??????
			if(pid->Out > pid->MaxOutValue)
			{pid->Out = (pid->MaxOutValue);}
			else if(pid->Out < pid->MinOutValue)
			{pid->Out = pid->MinOutValue;}	
		break;
	}
		
	return pid->Out;
}

/***********************************************************************
** ?? ?? ???? PID_QCS()
** ????????? ??????QCS?????????????PID
**---------------------------------------------------------------------
** ????????? PID???????‰]???
** ????????? PID??????
***********************************************************************/
int32_t PID_QCS(PID_Data *pid, float Error)
{
			float Kp =0.0f;
			float Ki =0.0f;
			float Kd =0.0f;
			//¦Ë???

			//pid->Error = pid->Ref - pid->FeedBack;
			//QCS?????????
			pid->Error = Error;
			
			//pid->integral	+=	pid->Error;
			arm_add_f32(&pid->integral,&pid->Error,&pid->integral,1);
		
			//???????
			if(pid->integral > (pid->Maxintegral))
			{pid->integral = (pid->Maxintegral);}
			else if(pid->integral < (pid->Minintegral))
			{pid->integral = (pid->Minintegral);}
			
			
			//pid->Out = pid->Kp * pid->Error + pid->Ki * pid->integral  + pid->Kd * (pid->Error - pid->DError);	
			//P????			
			arm_mult_f32(&pid->Kp,&pid->Error,&Kp,1);			
			//I??????§³?????	
			if((fabs(pid->Error)<pid->Ki_able_error_range)||!pid->Ki_able_error_range)
			{arm_mult_f32(&pid->Ki,&pid->integral,&Ki,1);}		
			//D??????§³???????	
			if((fabs(pid->Error)<pid->Kd_able_error_range)||!pid->Kd_able_error_range)
				{
				arm_sub_f32(&pid->Error,&pid->DError,&pid->DDError,1);
				arm_mult_f32(&pid->Kd,&pid->DDError,&Kd,1);}			
			//??????
			arm_add_f32(&Ki,&Kd,&Ki,1);
			arm_add_f32(&Kp,&Ki,&pid->Out,1);	
			
			pid->DError = pid->Error;
			
			//??????
			if(pid->Out > pid->MaxOutValue)
			{pid->Out = (pid->MaxOutValue);}
			else if(pid->Out < pid->MinOutValue)
			{pid->Out = pid->MinOutValue;}	
		return pid->Out;
}

/***********************************************************************
** ?? ?? ???? FWCCal()
** ????????? ??????????????????????????
**---------------------------------------------------------------------
** ????????? FWC???????‰]??????????????????
** ????????? FWC??????
***********************************************************************/
int16_t FWCCal(FWC_Data *fwc, int16_t Input)
{
	arm_sub_q15(&Input,&fwc->DInput,&fwc->Error,1);
	fwc->Output = fwc->MKF*fwc->Error;
	return fwc->Output;
}

/***********************************************************************
** ?? ?? ???? Usart_Printf()
** ????????? ??????????
**---------------------------------------------------------------------
** ????????? ???????????????
** ????????? ??
***********************************************************************/
void Usart_Printf(UART_HandleTypeDef *huart,char *fmt, ...) //???????
{
  uint8_t tx_buf[40] = {0};
  va_list ap;
  uint16_t len;
  va_start(ap, fmt);

  len = vsprintf((char *)tx_buf, fmt, ap);
  va_end(ap);
  HAL_UART_Transmit(huart,tx_buf,len,1000);
}
/***********************************************************************
** ?? ?? ???? Usart_Printf()
** ????????? ??????????
**---------------------------------------------------------------------
** ????????? ?????????????????
** ????????? ??
***********************************************************************/
void imPtintf(UART_HandleTypeDef *huart, uint8_t name, float value)
{
  char buf[20];
  sprintf(buf, "Line%d=%f,", name, value);
  Usart_Printf(huart,buf);
}