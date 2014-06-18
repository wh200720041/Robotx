
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IMU_H
#define __IMU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f401_discovery.h"
#include "stm32f401_discovery_lsm303dlhc.h"
#include "stm32f401_discovery_l3gd20.h"
#include "main.h"
#include "Utility.h"
#include <stdio.h>


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* TIM2 Autoreload and Capture Compare register values */
/* Exported macro ------------------------------------------------------------*/
//MEMS
#define MEMS_PASSCONDITION              5

#define L3G_Sensitivity_250dps     (float)   114.285f         /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps] */
#define L3G_Sensitivity_500dps     (float)    57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps] */
#define L3G_Sensitivity_2000dps    (float)    14.285f	      /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */
#define PI                         (float)     3.14159265f

#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */

#define GYRO_INDEX 0x05
#define ACC_INDEX 0x01
#define COMPASS_INDEX 0x03
/* Exported functions ------------------------------------------------------- */
uint32_t LSM303DL_TIMEOUT_UserCallback(void);
uint32_t L3GD20_TIMEOUT_UserCallback(void);
void LSM303DLHC_CompassReadAcc(float* pfData);
void LSM303DLHC_MEMS_Init(void);
void L3GD20_MEMS_Init(void);
void GyroReadAngRate (float* pfData);
void LSM303DLHC_CompassReadMag(float* pfData);
void Read_Compass(int16_t* pfData);
void Read_Gyro(int16_t* RawData);
void Read_Acc(int16_t* pfData);
void Compass_Init(uint8_t speed, uint8_t sensitivity);
void Acc_Init(uint8_t speed, uint8_t sensitivity);
void Gyro_Init(uint8_t speed, uint8_t sensitivity);
#endif /* __STM32F4_DISCOVERY_DEMO_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
