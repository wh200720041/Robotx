

#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_spi.h"
#include "stm32f401_discovery.h"
#include "Utility.h"
#define CHIP1 1
#define CHIP2 2
void My_SPI_Init(void);
void Potentialmeter_SetValue(int value,int chip);
#endif 
