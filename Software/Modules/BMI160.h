#ifndef BMI160_LL_H
#define BMI160_LL_H
#include "SPI_LL.h"
#include "cmsis_os.h"
#ifdef __cplusplus

#define BMI160LL_INT1_Port	BMI160_INT1_GPIO_Port
#define BMI160LL_INT2_Port	BMI160_INT1_GPIO_Port
#define BMI160LL_INT1_Pin	BMI160_INT1_Pin
#define BMI160LL_INT2_Pin	BMI160_INT2_Pin
#define IMU_DATA_ADDR 0x08007800 //Page15
#define IMU_DATA_PAGE 15
#define IMU_GYR_ZERO_ADDR 0x08007800
#define IMU_ACC_ZERO_ADDR 0x08007818 //+12x2
class BMI160_C : public SPI_C
{
	protected:
	uint8_t Chip_ID = 0;
	uint8_t Error_Code = 0;
	public:
	
	float Gyro_ratio[3] = {0.0010652644360f,0.0010652644360f,0.0010652644360f};	//((tmp/32768)*(2000*pi/180))rad/s
	float Gyro[3]={0} ;
	float Gyro_zero[3]={0} ;
	float Gyro_calib[3]={0} ;
	float Accel_ratio[3] = {0.0047856933593f,0.004785693359375f,0.004785693359375f}; //(tmp/32768)*24*9.80665 m/s2
	float Accel[3]={0} ;
	float Accel_last[3]={0} ;
	float Accel_zero[3]={0} ;
	float Accel_calib[3]={0} ;
	float Temperature ;
	
	uint8_t IMU_Init(void);
	uint8_t Read_Reg(uint8_t reg);
	uint8_t Write_Reg(uint8_t Reg, uint8_t data);
	void Read_6Reg(uint8_t reg, uint8_t *buf);
};
extern "C"
{
	void BMI160_Rec_Task(void const * argument);
}
extern BMI160_C BMI160;
//typedef struct 
//{
//  uint8_t Chip_ID;
//  uint8_t ERROR_Code;
//  uint8_t Init_statue;
//  float bmi160_gyro[3] ;
//  float bmi160_accel[3] ;
//	
//  float bmi160_accel_last[3] ;
//  float bmi160_temperature ;

//  //��������Ư����
//  float bmi160_gyro_zero[3] ;
//  //���ٶ���Ư����
//  float bmi160_accel_zero[3] ;

//  //У׼������������
//  float bmi160_gyro_calib[3] ;
//  //У׼����ٶȼ�����
//  float bmi160_accel_calib[3] ;

//}BMI160_Data_t;

//extern BMI160_Data_t BMI160_Data;
//void BMI160_LL_EXIT_INT(uint16_t GPIO_Pin);
//void BMI160_LL_init(BMI160_Data_t *BMI160_Data);
//void BMI160_LL_Stop(BMI160_Data_t *BMI160_Data);
//uint8_t BMI160_LL_Exchange_Byte(uint8_t byte) ;
//uint8_t BMI160_LL_Read1Reg(uint8_t reg);
//void BMI160_LL_Read_GandA(BMI160_Data_t *BMI160_Data);
//void BMI160_LL_read_temperature(BMI160_Data_t *BMI160_Data);
//void BMI160_LL_calib_gyro_zero(BMI160_Data_t *BMI160_Data);
//extern uint8_t accel_calib_pause;
//void BMI160_LL_calib_accel_zero(BMI160_Data_t *BMI160_Data);
//void Advanced_Heating(void);
//void BMI160_LL_Delay(uint16_t time);
#endif
#endif