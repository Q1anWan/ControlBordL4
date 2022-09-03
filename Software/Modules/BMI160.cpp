#include "BMI160.h"
#include "SPI_LL.h"
#include "myTaskList.h"
#include "arm_math.h"
BMI160_C BMI160;
void BMI160_Rec_Task(void const * argument)
{
	vTaskDelay(100);	
	BMI160.IMU_Init();
	for(;;)
	{
		/*获取二值量*/
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_12);
		xSemaphoreTake(IMURecSemHandle,portMAX_DELAY);
		uint8_t DRDY_ID = BMI160.Read_Reg(0x1B);
	
		DRDY_ID &= 0xC0;
		if(!DRDY_ID){continue;}
		if(DRDY_ID != 0x40){//加速度RDY 即不是仅角速度RDY
			
			uint8_t buf[6]={0};
			float raw_tmp[3];
			
			BMI160.Read_6Reg(0x12,buf);
			
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

			BMI160.Read_6Reg(0x0C,buf);

			raw_tmp[0] = (float)((int16_t)((buf[1]<<8)|buf[0]));	
			raw_tmp[1] = (float)((int16_t)((buf[3]<<8)|buf[2]));		
			raw_tmp[2] = (float)((int16_t)((buf[5]<<8)|buf[4]));

			arm_mult_f32(raw_tmp,BMI160.Gyro_ratio,BMI160.Gyro,3);
			//减去标定的零漂值
			arm_sub_f32(BMI160.Gyro,BMI160.Gyro_zero,BMI160.Gyro_calib,3);
		}
	}
}

uint8_t BMI160_C::IMU_Init(void)
{
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_12);
	this->SPI_C::Init(SPI1,SPI1_CS_GPIO_Port,SPI1_CS_Pin);
	this->Read_Reg(0x7F);
	vTaskDelay(1);
	this->Write_Reg(0x7E, 0xB6);
	vTaskDelay(1);
	this->Read_Reg(0x7F);
	vTaskDelay(1);
	this->Chip_ID = this->Read_Reg(0x00);
	
	vTaskDelay(2);
	this->Write_Reg(0x40, 0x2B);//ACC us=0 bwp=2 odr=800Hz
	vTaskDelay(1);
	this->Write_Reg(0x41, 0x0C);//ACC range=+-16g
	vTaskDelay(1);
	this->Write_Reg(0x42, 0x2B);//GYR bwp=2 odr=800Hz
	vTaskDelay(1);
	this->Write_Reg(0x43, 0x00);//GYR range=2000°
	vTaskDelay(1);
	this->Write_Reg(0x53, 0x88);//INT2 \ INT1 ENABLE PP  ACTIVE LVL = LOW(FALL EDGE)
	vTaskDelay(1);
	this->Write_Reg(0x56, 0x80);//MAP INT1 DATA_READY
	vTaskDelay(1);
	this->Write_Reg(0x7E, 0x15);//PWR NORMAL MODE GYR
	vTaskDelay(82);
	this->Write_Reg(0x7E, 0x11);//PWR NORMAL MODE ACC
	vTaskDelay(4);
	this->Write_Reg(0x51, 0x10);//IT DataReady ENABLE
	vTaskDelay(1);
	this->Error_Code = this->Read_Reg(0x02);
	

////	uint16_t temp[6]={0};
////	Flash_Data_Read(IMU_GYR_ZERO_ADDR,temp, 6);
////	U16_To_Float(temp,BMI160_Data->bmi160_gyro_zero,6);

////	Flash_Data_Read(IMU_ACC_ZERO_ADDR,temp, 6);
////	U16_To_Float(temp,BMI160_Data->bmi160_accel_zero,6);
//	
//	BMI160_Data->Init_statue = 1;
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
void BMI160_C::Read_6Reg(uint8_t reg, uint8_t *buf)
{
	this->CS_Enable();
	this->SPI_C::Read_Muti_Reg(reg,6,buf);
	this->CS_Disable();
}

//void BMI160_LL_EXIT_INT(uint16_t GPIO_Pin)
//{
//	static uint16_t i;
//	switch(GPIO_Pin)
//	{
//		case BMI160LL_INT1_Pin:
//			 
//			 BMI160_LL_Read_GandA(&BMI160_Data);
//			 if(++i==40)//20Hz
//			 {i=0;BMI160_LL_read_temperature(&BMI160_Data);}
//		break;
//		
//		case BMI160LL_INT2_Pin:
//			
//		break;
//	
//	}
//}


//void BMI160_LL_init(BMI160_Data_t *BMI160_Data)
//{
//	BMI160_LL_Read1Reg(0x7F);
//	BMI160_LL_Delay(300);
//	BMI160_LL_Write1Reg(0x7E, 0xB6);
//	BMI160_LL_Delay(300);
//	BMI160_LL_Read1Reg(0x7F);
//	BMI160_LL_Delay(100);
//	
//	BMI160_Data->Chip_ID = BMI160_LL_Read1Reg(0x00);
//	BMI160_LL_Delay(150);

//	
//	BMI160_LL_Write1Reg(0x40, 0x2C); //ACC us=0 bwp=2 odr=1600Hz
//	BMI160_LL_Delay(100);

//	BMI160_LL_Write1Reg(0x41, 0x0C); //ACC range=+-16g
//	BMI160_LL_Delay(100);

//	BMI160_LL_Write1Reg(0x42, 0x2C); //GYR bwp=2 odr=1600Hz
//	BMI160_LL_Delay(100);

//	BMI160_LL_Write1Reg(0x43, 0x00); //GYR range=2000°
//	BMI160_LL_Delay(100);
//	
//	BMI160_LL_Write1Reg(0x51, 0x10); //IT_DRDY ENABLE
//	BMI160_LL_Delay(100);

//	BMI160_LL_Write1Reg(0x53, 0x88); //INT2 \ INT1 ENABLE PP  ACTIVE LVL = LOW(FALL EDGE)
//	BMI160_LL_Delay(100);

//	BMI160_LL_Write1Reg(0x56, 0x80); //MAP INT1 DATA_READY
//	BMI160_LL_Delay(400);
//	
//	BMI160_LL_Write1Reg(0x7E, 0x15); //PWR NORMAL MODE ACC
//	BMI160_LL_Delay(200);

//	BMI160_LL_Write1Reg(0x7E, 0x11); //PWR NORMAL MODE GYR
//	BMI160_LL_Delay(200);
//	
//	BMI160_Data->ERROR_Code = BMI160_LL_Read1Reg(0x03);//
//	BMI160_LL_Delay(100);
//	
////	BMI160_Data->ERROR_Code = BMI160_LL_Read1Reg(0x02);//
////	BMI160_LL_Delay(100);
//	
////	uint16_t temp[6]={0};
////	Flash_Data_Read(IMU_GYR_ZERO_ADDR,temp, 6);
////	U16_To_Float(temp,BMI160_Data->bmi160_gyro_zero,6);

////	Flash_Data_Read(IMU_ACC_ZERO_ADDR,temp, 6);
////	U16_To_Float(temp,BMI160_Data->bmi160_accel_zero,6);
//	
//	BMI160_Data->Init_statue = 1;
//}

//void BMI160_LL_Stop(BMI160_Data_t *BMI160_Data)
//{
//	BMI160_LL_Write1Reg(0x7E, 0xB6);
//	BMI160_LL_Delay(300);
//	BMI160_LL_Read1Reg(0x7F);
//	BMI160_LL_Delay(100);
//	BMI160_LL_Write1Reg(0x03, 0x80); //PWR Suspend Everything
//	BMI160_LL_Delay(100);
//}

//void BMI160_LL_Read_GandA(BMI160_Data_t *BMI160_Data)
//{
//	uint8_t buf[6]={0};
//	int16_t bmi160_raw_tmp;
//	uint8_t DRDY_ID;
//	DRDY_ID = BMI160_LL_Read1Reg(0x1B);
//	DRDY_ID &= 0xC0;
//	if(!DRDY_ID){return;}
//		if(DRDY_ID != 0x40){//加速度RDY 即不是仅角速度RDY
//			BMI160_LL_Read6reg(0x12,buf);
//			
//			bmi160_raw_tmp = (int16_t)((buf[1]<<8)|buf[0]);
//			BMI160_Data->bmi160_accel[0] = bmi160_raw_tmp * 0.004785693359375f; //(tmp/32768)*24*9.80665 m/s2
//			bmi160_raw_tmp = (int16_t)((buf[3]<<8)|buf[2]);
//			BMI160_Data->bmi160_accel[1] = bmi160_raw_tmp * 0.004785693359375f;
//			bmi160_raw_tmp = (int16_t)((buf[5]<<8)|buf[4]);
//			BMI160_Data->bmi160_accel[2] = bmi160_raw_tmp * 0.004785693359375f;
//			
//			//一阶低通滤波
//			BMI160_Data->bmi160_accel_calib[0] = (BMI160_Data->bmi160_accel[0]-BMI160_Data->bmi160_accel_zero[0])*0.3f + BMI160_Data->bmi160_accel_last[0]*0.7f;
//			BMI160_Data->bmi160_accel_calib[1] = (BMI160_Data->bmi160_accel[1]-BMI160_Data->bmi160_accel_zero[1])*0.3f + BMI160_Data->bmi160_accel_last[1]*0.7f;
//			BMI160_Data->bmi160_accel_calib[2] = (BMI160_Data->bmi160_accel[2]-BMI160_Data->bmi160_accel_zero[2])*0.3f + BMI160_Data->bmi160_accel_last[2]*0.7f;

//			BMI160_Data->bmi160_accel_last[0] = BMI160_Data->bmi160_accel_calib[0];
//			BMI160_Data->bmi160_accel_last[1] = BMI160_Data->bmi160_accel_calib[1];
//			BMI160_Data->bmi160_accel_last[2] = BMI160_Data->bmi160_accel_calib[2];

//			QCS_AHRS_update(BMI160_Data->bmi160_gyro_calib,BMI160_Data->bmi160_accel_calib,0);
//			
//		}
//	
//		
//		if(DRDY_ID != 0x80){//陀螺仪RDY 即不仅是加速度DRY
//			BMI160_LL_Read6reg(0x0C,buf);
////			float Operator = 0.00106526443603169529841533860381f;//((tmp/32768)*(2000*pi/180))rad/s
//			bmi160_raw_tmp = (int16_t)((buf[1]<<8)|buf[0]);
//			BMI160_Data->bmi160_gyro[0] = bmi160_raw_tmp * 0.00106526443603169529841533860381f;
//			
//			bmi160_raw_tmp = (int16_t)((buf[3]<<8)|buf[2]);
//			BMI160_Data->bmi160_gyro[1] = bmi160_raw_tmp * 0.00106526443603169529841533860381f;
//			
//			bmi160_raw_tmp = (int16_t)((buf[5]<<8)|buf[4]);
//			BMI160_Data->bmi160_gyro[2] = bmi160_raw_tmp * 0.00106526443603169529841533860381f;
//			
//			//减去标定的零漂值
//			arm_sub_f32(&BMI160_Data->bmi160_gyro[0],&BMI160_Data->bmi160_gyro_zero[0],&BMI160_Data->bmi160_gyro_calib[0],1);
//			arm_sub_f32(&BMI160_Data->bmi160_gyro[1],&BMI160_Data->bmi160_gyro_zero[1],&BMI160_Data->bmi160_gyro_calib[1],1);
//			arm_sub_f32(&BMI160_Data->bmi160_gyro[2],&BMI160_Data->bmi160_gyro_zero[2],&BMI160_Data->bmi160_gyro_calib[2],1);
//			
//			GYRO_FPS_T++;
//			
//		}	
//}

//void BMI160_LL_read_temperature(BMI160_Data_t *BMI160_Data)
//{
//	uint8_t buf[2];
//	int16_t bmi160_raw_tmp;
//	//
//	BMI160_LL_NS_L();
//	BMI160_LL_Exchange_Byte(0xA0);
//	buf[0] = BMI160_LL_Exchange_Byte(0xFF);
//	buf[1] = BMI160_LL_Exchange_Byte(0xFF);
//	BMI160_LL_NS_H();
//	//
//	bmi160_raw_tmp = (int16_t)((buf[1]<<8)|(buf[0]));
//	BMI160_Data->bmi160_temperature = (bmi160_raw_tmp/512.0f)+23.0f;
//}

//void BMI160_LL_calib_gyro_zero(BMI160_Data_t *BMI160_Data)
//{
//	
//	float sum[3]={0.0f,0.0f,0.0f};
//	volatile uint16_t n=0;
//	volatile float cnt = 1000.0f;
//	for(n=0;n<cnt;n++)
//	{
//		sum[0] += BMI160_Data->bmi160_gyro[0];
//		sum[1] += BMI160_Data->bmi160_gyro[1];
//		sum[2] += BMI160_Data->bmi160_gyro[2];
//		
//		BMI160_LL_Delay(30);
//	}
//	
//	//
//	BMI160_Data->bmi160_gyro_zero[0] = sum[0]/cnt;
//	BMI160_Data->bmi160_gyro_zero[1] = sum[1]/cnt;
//	BMI160_Data->bmi160_gyro_zero[2] = sum[2]/cnt;
//	
//	//rewrite flash
//	uint16_t temp_accel[6];
////	float tttt[3]={-0.37506932f,-0.691533625,0.331102043};
////	Float_To_U16(tttt,temp_accel,3);
//	Flash_Data_Read(IMU_ACC_ZERO_ADDR,temp_accel,6);
//	Flash_Data_PageErase(IMU_DATA_PAGE);
//	uint16_t temp[6];
//	Float_To_U16(BMI160_Data->bmi160_gyro_zero,temp,3);
//	Flash_Data_Write(IMU_GYR_ZERO_ADDR,temp,6);
//	Flash_Data_Write(IMU_ACC_ZERO_ADDR,temp_accel,6);

//}
//uint8_t accel_calib_pause = 1;

//void BMI160_LL_calib_accel_zero(BMI160_Data_t *BMI160_Data)
//{
//	while(1)
//	{			
//		static uint8_t step = 1;
//		static float cnt = 1000.0f;
//		static float sum[3]={0.0f,0.0f,0.0f};
//		static float sum_p[3]={0.0f,0.0f,0.0f};
//		static float sum_n[3]={0.0f,0.0f,0.0f};
//		while (accel_calib_pause==1){;}
//		switch (step){
//			case 1:
//				for(uint16_t n=0;n<cnt;n++)
//				{
//					sum_p[0] += BMI160_Data->bmi160_accel[0];
//					BMI160_LL_Delay(30);
//				}
//				step += 1;
//				accel_calib_pause = 1;
//			break;
//			case 2:
//				for(uint16_t n=0;n<cnt;n++)
//				{
//					sum_p[1] += BMI160_Data->bmi160_accel[1];
//					BMI160_LL_Delay(30);
//				}
//				step += 1;
//				accel_calib_pause = 1;
//			break;
//			case 3:
//				for(uint16_t n=0;n<cnt;n++)
//				{
//					sum_p[2] += BMI160_Data->bmi160_accel[2];
//					BMI160_LL_Delay(30);
//				}
//				step += 1;
//				accel_calib_pause = 1;
//			break;
//			case 4:
//				for(uint16_t n=0;n<cnt;n++)
//				{
//					sum_n[0] += BMI160_Data->bmi160_accel[0];
//					BMI160_LL_Delay(30);
//				}
//				step += 1;
//				accel_calib_pause = 1;
//			break;
//			case 5:
//				for(uint16_t n=0;n<cnt;n++)
//				{
//					sum_n[1] += BMI160_Data->bmi160_accel[1];
//					BMI160_LL_Delay(30);
//				}
//				step += 1;
//				accel_calib_pause = 1;
//			break;
//			case 6:
//				for(uint16_t n=0;n<cnt;n++)
//				{
//					sum_n[2] += BMI160_Data->bmi160_accel[2];
//					BMI160_LL_Delay(30);
//				}
//				step += 1;
//				accel_calib_pause = 1;
//			break;
//			
//			default:
//			break;
//		
//		}
//		
//			BMI160_Data->bmi160_accel_zero[0] = (sum_n[0]+sum_p[0])/(2.0f*cnt);
//			BMI160_Data->bmi160_accel_zero[1] = (sum_n[1]+sum_p[1])/(2.0f*cnt);
//			BMI160_Data->bmi160_accel_zero[2] = (sum_n[2]+sum_p[2])/(2.0f*cnt);
//		
//		
////rewrite flash
//	uint16_t temp_gyro[6];
//	Flash_Data_Read(IMU_GYR_ZERO_ADDR,temp_gyro,6);
//	Flash_Data_PageErase(IMU_DATA_PAGE);
//	uint16_t temp[6];
//	Float_To_U16(BMI160_Data->bmi160_accel_zero,temp,3);
//	Flash_Data_Write(IMU_GYR_ZERO_ADDR,temp_gyro,6);
//	Flash_Data_Write(IMU_ACC_ZERO_ADDR,temp,6);
//	}
//}

///***********************************************************************
//** 函 数 名： Advanced_Heating()
//** 函数说明： 控制BMI160小板温度
//**---------------------------------------------------------------------
//** 输入参数： 无
//** 返回参数： 无
//***********************************************************************/
//void Advanced_Heating(void)
//{
//	int16_t tmp_out = 0;
//	if(BMI160_Data.bmi160_temperature < 44.5f)
//	{
//		LL_TIM_OC_SetCompareCH4(TIM2,7200);
//	}
//	else
//	{
//		tmp_out = PIDCal(&Pid_gyrotemp,BMI160_Data.bmi160_temperature * 100);
//        if(tmp_out<1000){tmp_out=1000;}
//		LL_TIM_OC_SetCompareCH4(TIM2,tmp_out);
//	}
//}
