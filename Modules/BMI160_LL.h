#ifndef BMI160_LL_H
#define BMI160_LL_H
#include "spi.h"

#define BMI160LL_SPI SPI1
#define BMI160LL_NS_Port 	SPI1_CS_GPIO_Port
#define BMI160LL_NS_Pin 	SPI1_CS_Pin
#define BMI160LL_INT1_Port	BMI160_INT1_GPIO_Port
#define BMI160LL_INT2_Port	BMI160_INT1_GPIO_Port
#define BMI160LL_INT1_Pin	BMI160_INT1_Pin
#define BMI160LL_INT2_Pin	BMI160_INT2_Pin
#define IMU_DATA_ADDR 0x08007C00 //Page31
#define IMU_DATA_PAGE 31
#define IMU_GYR_ZERO_ADDR 0x08007C00
#define IMU_ACC_ZERO_ADDR 0x08007C18 //+12x2
typedef struct 
{
  uint8_t Chip_ID;
  uint8_t ERROR_Code;
  uint8_t Init_statue;
  float bmi160_gyro[3] ;
  float bmi160_accel[3] ;
	
  float bmi160_accel_last[3] ;
  float bmi160_temperature ;

  //陀螺仪零漂常数
  float bmi160_gyro_zero[3] ;
  //加速度零漂常数
  float bmi160_accel_zero[3] ;

  //校准后陀螺仪数据
  float bmi160_gyro_calib[3] ;
  //校准后加速度计数据
  float bmi160_accel_calib[3] ;

}BMI160_Data_t;

extern BMI160_Data_t BMI160_Data;
void BMI160_LL_EXIT_INT(uint16_t GPIO_Pin);
void BMI160_LL_init(BMI160_Data_t *BMI160_Data);
void BMI160_LL_Stop(BMI160_Data_t *BMI160_Data);
uint8_t BMI160_LL_Exchange_Byte(uint8_t byte) ;
uint8_t BMI160_LL_Read1Reg(uint8_t reg);
void BMI160_LL_Read_GandA(BMI160_Data_t *BMI160_Data);
void BMI160_LL_read_temperature(BMI160_Data_t *BMI160_Data);
void BMI160_LL_calib_gyro_zero(BMI160_Data_t *BMI160_Data);
extern uint8_t accel_calib_pause;
void BMI160_LL_calib_accel_zero(BMI160_Data_t *BMI160_Data);
void Advanced_Heating(void);
void BMI160_LL_Delay(uint16_t time);
#endif