/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** 项目名称：   QCS四元数惯性坐标系统精简版
** 日    期：   2022-04-12
** 作    者：   qianwan
** 版    本：   1.2
**---------------------------------------------------------------
** 文 件 名：   QCS_Lite.c
** 代码需求：   CMSIS 5.8.0
** 文件补充：   默认右手系、弧度制
*****************************************************************/
//#include "main.h"
#include "QCS_Lite.h"
#include "MahonyAHRS.h"

/*---------------------VARIABLES---------------------*/
float IMU_quat[4] = {0.0f};
float QCS_AHRSq[4]={0};
/*---------------------FUNCTIONS---------------------*/
/***********************************************************************
** 函 数 名： QCS_Rotate
** 函数说明： 本函数适用于普通四元数,变换后依然是普通四元数
**---------------------------------------------------------------------
** 输入参数： 输入四元数、向量、旋转角度（弧度制）
** 返回参数： 旋转后的四元数
***********************************************************************/
void QCS_Rotate(float *Q_input,float *Q_output,float *Vector,float angel_Radian)
{
	float Q_transform[4]={0};
	float Sinf=0.0f,Cosf=0.0f;
	Sinf = arm_sin_f32(angel_Radian/2.0f);
	Cosf = arm_cos_f32(angel_Radian/2.0f);
	Q_transform[0]=Cosf;
	arm_mult_f32(&Sinf,&Vector[0],&Q_transform[1],1);
	arm_mult_f32(&Sinf,&Vector[1],&Q_transform[2],1);
	arm_mult_f32(&Sinf,&Vector[2],&Q_transform[3],1);

	arm_quaternion_product_single_f32(Q_transform,Q_input,Q_output);	
}

/**@brief 欧拉角采用X-Y-Z**/

/***********************************************************************
** 函 数 名： QCS_Rotate_Pure
** 函数说明： 本函数适用于纯四元数，变换后依然是纯四元数
**---------------------------------------------------------------------
** 输入参数： 输入四元数、向量、旋转角度（弧度制）
** 返回参数： 旋转后的四元数
***********************************************************************/
void QCS_Rotate_Pure(float *Q_input,float *Q_output,float *Vector,float angel_Radian)
{
	float Q[4]={arm_cos_f32(angel_Radian/2.0f),arm_sin_f32(angel_Radian/2.0f)*Vector[0],arm_sin_f32(angel_Radian/2.0f)*Vector[1],arm_sin_f32(angel_Radian/2)*Vector[2]};
	float Q_star[4],Q_temp[4];
	arm_quaternion_conjugate_f32(Q,Q_star,1);
	arm_quaternion_product_f32(Q,Q_input,Q_temp,1);
	arm_quaternion_product_f32(Q_temp,Q_star,Q_output,1);	
}

/**@brief 欧拉角采用X-Y-Z**/
/***********************************************************************
** 函 数 名： QCS_GetRollAngel
** 函数说明： 计算某四元数Roll欧拉角
**---------------------------------------------------------------------
** 输入参数： 四元数
** 返回参数： Roll欧拉角
***********************************************************************/
void QCS_GetRollAngel(float Q_input[4],float *Roll)
{
	//Roll = arctan((2 * q2 * q3 + 2 * q0 * q1) / (-2 * q1^2 - 2 * q2^2 + 1)) 
	float temp_a,temp_b,temp_c,temp_d;
	arm_mult_f32(&Q_input[2],&Q_input[3],&temp_a,1);
	arm_mult_f32(&Q_input[0],&Q_input[1],&temp_b,1);
	arm_add_f32(&temp_a,&temp_b,&temp_c,1);
	temp_a =2.0f;
	arm_mult_f32(&temp_a,&temp_c,&temp_c,1);

	arm_mult_f32(&Q_input[1],&Q_input[1],&temp_a,1);
	arm_mult_f32(&Q_input[2],&Q_input[2],&temp_b,1);
	arm_add_f32(&temp_a,&temp_b,&temp_d,1);
	temp_a = -2.0f;
	arm_mult_f32(&temp_a,&temp_d,&temp_d,1);
	
	*Roll = atan2f(temp_c,(temp_d+1));
}

/***********************************************************************
** 函 数 名： QCS_GetYawAngel
** 函数说明： 计算某四元数Yaw欧拉角
**---------------------------------------------------------------------
** 输入参数： 四元数
** 返回参数： Yaw欧拉角
***********************************************************************/
void QCS_GetYawAngel(float Q_input[4],float *Yaw)
{
	//Yaw = arctan((2*q1*q2 + 2*q0*q3) / (-2*q2^2 - 2*q3^2+1))
	float temp_a,temp_b,temp_c,temp_d;
	arm_mult_f32(&Q_input[1],&Q_input[2],&temp_a,1);
	arm_mult_f32(&Q_input[0],&Q_input[3],&temp_b,1);
	arm_add_f32(&temp_a,&temp_b,&temp_c,1);
	temp_a =2.0f;
	arm_mult_f32(&temp_a,&temp_c,&temp_c,1);

	arm_mult_f32(&Q_input[2],&Q_input[2],&temp_a,1);
	arm_mult_f32(&Q_input[3],&Q_input[3],&temp_b,1);
	arm_add_f32(&temp_a,&temp_b,&temp_d,1);
	temp_a = -2.0f;
	arm_mult_f32(&temp_a,&temp_d,&temp_d,1);
	
	*Yaw = atan2f(temp_c,(temp_d+1));
}

/***********************************************************************
** 函 数 名： QCS_GetPitchAngel
** 函数说明： 计算某四元数Pitch欧拉角
**---------------------------------------------------------------------
** 输入参数： 四元数
** 返回参数： Pitch欧拉角
***********************************************************************/
void QCS_GetPitchAngel(float Q_input[4],float *Pitch)
{
	//Pitch = arcsin(2 * q0* q2 - 2 * q1 * q3)
	float temp_a,temp_b,temp_c;
	arm_mult_f32(&Q_input[0],&Q_input[2],&temp_a,1);
	arm_mult_f32(&Q_input[1],&Q_input[3],&temp_b,1);
	arm_sub_f32(&temp_a,&temp_b,&temp_c,1);
	temp_a =2.0f;
	arm_mult_f32(&temp_a,&temp_c,&temp_c,1);
	
	*Pitch = asinf(temp_c);
}

/***********************************************************************
** 函 数 名： QCS_GetErrorQ
** 函数说明： 计算误差四元数
**---------------------------------------------------------------------
** 输入参数： 当前四元数、目标四元数
** 返回参数： 误差四元数
***********************************************************************/
void QCS_GetErrorQ(float Q_Now[4],float Q_Target[4],float Q_output[4])
{
	float InverseQ[4];
	arm_quaternion_inverse_f32(Q_Now,InverseQ,1);
	arm_quaternion_product_f32(Q_Target,InverseQ,Q_output,1);
}
 
/***********************************************************************
** 函 数 名： QCS_CorrectAHRSC
** 函数说明： AHRS四元数修正
**---------------------------------------------------------------------
** 输入参数： AHRS原始四元数，坐标系漂移弧度（单MCU时为0）
** 返回参数： AHRS修正后四元数
***********************************************************************/
void QCS_CorrectAHRSq(float q[4],float AHRSQ[4],float YC)
{
	//C板R标指向X正方向，Z方向垂直R标向外，右手系
	float V[4]={0,0,-1.0f};
	//旋转坐标轴到常规安装方向
	QCS_Rotate(q,AHRSQ,V,PI_Half-YC);
	
}

/***********************************************************************
** 函 数 名： QCS_init_data()
** 函数说明： 初始AHRS算法四元数
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void QCS_init_data(void)
{
	IMU_quat[0] = 1.0f; IMU_quat[1] = 0.0f; IMU_quat[2] = 0.0f; IMU_quat[3] = 0.0f; 
	QCS_AHRSq[0]=1.0f;QCS_AHRSq[1]=0.0f;QCS_AHRSq[2]=0.0f;QCS_AHRSq[3]=0.0f;
}

/***********************************************************************
** 函 数 名： QCS_IMU_update()
** 函数说明： 更新AHRS四元数(在0.8KHz外部中断调用)
**---------------------------------------------------------------------
** 输入参数： 三轴陀螺仪数据,三轴加速度数据,坐标系旋转弧度
** 返回参数： 无
***********************************************************************/
void QCS_AHRS_update(float gyro[], float accel[],float YC)
{	
	//不使用地磁时调用MahonyAHRSupdateINS
	#ifdef QCS_CORRECT
	MahonyAHRSupdateINS(IMU_quat, gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2]);
	QCS_CorrectAHRSq(IMU_quat,QCS_AHRSq,YC);
	#endif
	#ifndef QCS_CORRECT
	MahonyAHRSupdateINS(QCS_AHRSq, gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2]);
	#endif
	

}

/***********************************************************************
** 函 数 名： QCS_Show_Degree()
** 函数说明： 计算全部欧拉角
**---------------------------------------------------------------------
** 输入参数： AHRS四元数
** 返回参数： 姿态角
***********************************************************************/
void QCS_Show_Degree(float AHRSQ[4], float IMU_Degree[3])
{	
	float temp[3];
	QCS_GetRollAngel(AHRSQ,&temp[0]);
	QCS_GetYawAngel(AHRSQ,&temp[1]);
	QCS_GetPitchAngel(AHRSQ,&temp[2]);
	IMU_Degree[0]=temp[0]*57.2957795130f;//Roll
	IMU_Degree[1]=temp[1]*57.2957795130f;//Yaw
	IMU_Degree[2]=temp[2]*57.2957795130f;//Pitch
}
