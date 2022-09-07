#include "BMI160.h"
#include "SPI_LL.h"
#include "myTaskList.h"
#include "arm_math.h"
#include "Flash.h"
#include "QCS_Lite.h"
#include "PID.h"
#include "tim.h"
#include "usart.h"

BMI160_C BMI160;
#if (GYRO_CAIL_GO==1)
float GYR_ZERO[3];
uint8_t zero_flag=0;
uint16_t times = 0;
#endif
void BMI160_Rec_Task(void const * argument)
{
	vTaskDelay(100);	
	BMI160.IMU_Init();
	volatile uint8_t DRDY_ID;

	for(;;)
	{
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_12);
		/*获取二值量*/
		xSemaphoreTake(IMURecSemHandle,portMAX_DELAY);
		DRDY_ID = BMI160.Read_Reg(0x1B);
		DRDY_ID &= 0xC0;
		if(!DRDY_ID){continue;}
		if(DRDY_ID != 0x40){//加速度RDY 即不是仅角速度RDY
			
			uint8_t buf[6]={0};
			float raw_tmp[3];
			
			BMI160.Read_MultiReg(0x12,buf,6);
		
			raw_tmp[0] = (float)((int16_t)((buf[1]<<8)|buf[0]));
			raw_tmp[1] = (float)((int16_t)((buf[3]<<8)|buf[2]));
			raw_tmp[2] = (float)((int16_t)((buf[5]<<8)|buf[4]));
			
			arm_mult_f32(raw_tmp,BMI160.Accel_ratio,BMI160.Accel,3);
			//一阶低通滤波
			arm_sub_f32(BMI160.Accel,BMI160.Accel_zero,raw_tmp,3);
			BMI160.Accel_calib[0] = raw_tmp[0]*0.3f + BMI160.Accel_last[0]*0.7f;
			BMI160.Accel_calib[1] = raw_tmp[1]*0.3f + BMI160.Accel_last[1]*0.7f;
			BMI160.Accel_calib[2] = raw_tmp[2]*0.3f + BMI160.Accel_last[2]*0.7f;

			BMI160.Accel_last[0] = BMI160.Accel_calib[0];
			BMI160.Accel_last[1] = BMI160.Accel_calib[1];
			BMI160.Accel_last[2] = BMI160.Accel_calib[2];
			
		}
	
		if(DRDY_ID != 0x80){//陀螺仪RDY 即不仅是加速度DRY
			uint8_t buf[6]={0};
			float raw_tmp[3];

			BMI160.Read_MultiReg(0x0C,buf,6);

			raw_tmp[0] = (float)((int16_t)((buf[1]<<8)|buf[0]));	
			raw_tmp[1] = (float)((int16_t)((buf[3]<<8)|buf[2]));		
			raw_tmp[2] = (float)((int16_t)((buf[5]<<8)|buf[4]));

			arm_mult_f32(raw_tmp,BMI160.Gyro_ratio,BMI160.Gyro,3);
			//减去标定的零漂值
			arm_sub_f32(BMI160.Gyro,BMI160.Gyro_zero,BMI160.Gyro_calib,3);
		
		}

		#if (GYRO_CAIL_GO==1)/*加速度校准并写入内存*/
		if(zero_flag){
		GYR_ZERO[0] += BMI160.Gyro[0];
		GYR_ZERO[1] += BMI160.Gyro[1];
		GYR_ZERO[2] += BMI160.Gyro[2];
			
		if(++times==50000){
			zero_flag = 0;
			GYR_ZERO[0] = GYR_ZERO[0]/times;
			GYR_ZERO[1] = GYR_ZERO[1]/times;
			GYR_ZERO[2] = GYR_ZERO[2]/times;

			uint64_t *Temp;
			uint32_t *Temp2;
				
			Temp = (uint64_t *)pvPortMalloc(256);
			Temp2 = (uint32_t *)pvPortMalloc(96);
			
			
			Temp2[0] = *(uint32_t *)&GYR_ZERO[0];
			Temp2[1] = *(uint32_t *)&GYR_ZERO[1];
			Temp2[2] = *(uint32_t *)&GYR_ZERO[2];
			
			Temp[0] = ((uint64_t)Temp2[0]<<32)+(uint64_t)Temp2[1];
			Temp[1] = ((uint64_t)Temp2[2]<<32);
			Temp[3] = 0;Temp[4] = 0;
			qFlash_ErasePage(127);
			qFlash_WriteData(IMU_DATA_ADDR1,Temp,4);
			vPortFree(Temp);
			vPortFree(Temp2);
			}
		}
		#endif
	}
}

void BMI160_Tem_Task(void const * argument)
{
	uint8_t rec_buf[2];
	int16_t raw_tmp;
	vTaskDelay(400);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		uint32_t Tem = 0;
		vTaskDelayUntil(&xLastWakeTime,40);//25Hz执行
		/*读取温度*/
		BMI160.Read_MultiReg(0x20,rec_buf,2);
		raw_tmp = (int16_t)((rec_buf[1]<<8)|(rec_buf[0]));
		BMI160.Temperature = (raw_tmp/512.0f)+23.0f;
		/*快速温度控制*/
		if(BMI160.Temperature!=0)
		{Tem = (BMI160.Temperature<41.5f)?700:(uint32_t)BMI160.PID_Cal(BMI160.Temperature);}
		else{Tem=0;}	
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,Tem);
	}
}

void BMI160_InsUpdate_Task(void const * argument)
{
	vTaskDelay(10000);
	QCS_init_data();
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{	
	vTaskDelayUntil(&xLastWakeTime,2);
	QCS_AHRS_update(BMI160.AHRS_Q,BMI160.Gyro_calib,BMI160.Accel_calib,0);
	}	
}

uint8_t BMI160_C::IMU_Init(void)
{
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_12);
	this->SPI_C::Init(SPI1,SPI1_CS_GPIO_Port,SPI1_CS_Pin);
	this->Read_Reg(0x7F);
	vTaskDelay(10);
	this->Write_Reg(0x7E, 0xB6);
	vTaskDelay(10);
	this->Read_Reg(0x7F);
	vTaskDelay(10);
	this->Chip_ID = this->Read_Reg(0x00);
	
	vTaskDelay(20);
	this->Write_Reg(0x40, 0x2B);//ACC us=0 bwp=2 odr=800Hz
	vTaskDelay(10);
	this->Write_Reg(0x41, 0x0C);//ACC range=+-16g
	vTaskDelay(10);
	this->Write_Reg(0x42, 0x2B);//GYR bwp=2 odr=800Hz
	vTaskDelay(10);
	this->Write_Reg(0x43, 0x00);//GYR range=2000°
	vTaskDelay(10);
	this->Write_Reg(0x53, 0x88);//INT2 \ INT1 ENABLE PP  ACTIVE LVL = LOW(FALL EDGE)
	vTaskDelay(10);
	this->Write_Reg(0x56, 0x80);//MAP INT1 DATA_READY
	vTaskDelay(10);
	this->Write_Reg(0x51, 0x10);//IT DataReady ENABLE
	vTaskDelay(10);
	this->Write_Reg(0x7E, 0x15);//PWR NORMAL MODE GYR
	vTaskDelay(100);
	this->Write_Reg(0x7E, 0x11);//PWR NORMAL MODE ACC
	vTaskDelay(100);
	

	this->Error_Code = this->Read_Reg(0x02);
	
	uint64_t *Temp;
	uint32_t *Temp32;
	Temp = (uint64_t *)pvPortMalloc(256);
	Temp32 = (uint32_t *)pvPortMalloc(192);
	Temp[0]=qFlash_ReadData(IMU_DATA_ADDR1);
	Temp[1]=qFlash_ReadData(IMU_DATA_ADDR2);
	Temp[2]=qFlash_ReadData(IMU_DATA_ADDR3);
	Temp[3]=qFlash_ReadData(IMU_DATA_ADDR4);
	
	Temp32[0] =	(uint32_t)(Temp[0]>>32);
	Temp32[1] =	(uint32_t)(Temp[0]&0xFFFFFFFF);
	Temp32[2] =	(uint32_t)(Temp[1]>>32);
	Temp32[3]=	(uint32_t)(Temp[2]>>32);
	Temp32[4]=	(uint32_t)(Temp[2]&0xFFFFFFFF);
	Temp32[5]=	(uint32_t)(Temp[3]>>32);
	
	BMI160.Gyro_zero[0] =	*(float *)&Temp32[0];
	BMI160.Gyro_zero[1] =	*(float *)&Temp32[1];
	BMI160.Gyro_zero[2] =	*(float *)&Temp32[2];
	BMI160.Accel_zero[0]=	*(float *)&Temp32[3];
	BMI160.Accel_zero[1]=	*(float *)&Temp32[4];
	BMI160.Accel_zero[2]=	*(float *)&Temp32[5];
	
	vPortFree(Temp32);
	vPortFree(Temp);
	this->PID_Inc_C::PID_Init();
	vTaskDelay(10); 
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_12);
	return 1;
}
/*重定义寄存器读函数*/
uint8_t BMI160_C::Read_Reg(uint8_t reg)
{	uint8_t data;
	this->CS_Enable();
	data = this->SPI_C::Read_Reg(reg);
	this->CS_Disable();
	return data;
}

/*重定义寄存器写函数*/
uint8_t BMI160_C::Write_Reg(uint8_t Reg, uint8_t data)
{	uint8_t rdata;
	this->CS_Enable();
	rdata = this->SPI_C::Write_Reg(Reg,data);
	this->CS_Disable();
	return data;
}

/*多寄存器读函数*/
void BMI160_C::Read_MultiReg(uint8_t reg,uint8_t *buf,uint16_t num)
{
	this->CS_Enable();
	this->SPI_C::Read_Muti_Reg(reg,num,buf);
	this->CS_Disable();
}

void BMI160_C::PID_Inc_C::PID_Init(void)
{
	this->PID_Inc_C::Ref = 45.0f;
	this->PID_Inc_C::Kp = 100.0f;
	this->PID_Inc_C::Ki = 40.0f;
	this->PID_Inc_C::Kd = 10.0f;
	this->PID_Inc_C::IN_RANGE_EN_D = 1.0f;
	this->PID_Inc_C::IN_RANGE_EN_I = 3.0f;
	this->PID_Inc_C::MaxOutValue = 300;
	this->PID_Inc_C::MinOutValue = 100;	
}
