

#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_spi.h"
#include "stm32f401_discovery.h"
#include "Utility.h"
#define CHIP1 1
#define CHIP2 2
void My_SPI_Init(void);
void Potentialmeter_SetValue(int value,int chip);
void My_Left_Speed_Up(void);
void My_Left_Speed_Down(void);
void My_Right_Speed_Down(void);
void My_Right_Speed_Up(void);
void My_UD_Init(void);
void My_UD_Reset(void);
void Set_Left_Speed(int value);
void Set_Right_Speed(int value);
#endif 
