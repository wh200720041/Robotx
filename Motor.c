
#include "Motor.h"

int left_speed = 0;
int right_speed = 0;
void My_SPI_Init(void){
		
		
		SPI_InitTypeDef SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
   
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
   // 	 SPI_I2S_DeInit(SPI2);
		SPI_Cmd(SPI2, DISABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);  
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
		
		//SPI FUNCTION initialize
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);  //??
    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //?????
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;   //??????
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;   //????8
    SPI_InitStructure.SPI_CPOL  = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //????NSS??
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
		//NSS Setting, we use PB12  PD8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
}
void Potentialmeter_SetValue(int value,int chip)
{
	if(value>255)
		value=255;
	if(value<0)
		value=0;
	switch(chip){
		case CHIP1: GPIO_ResetBits(GPIOB,GPIO_Pin_12);break;
		case CHIP2: GPIO_ResetBits(GPIOD,GPIO_Pin_8);break;
		default: break;
	}
	wait(10);
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);	
	SPI_SendData(SPI2,0x0000+(value));
	wait(10);
	switch(chip){
		case CHIP1: GPIO_SetBits(GPIOB,GPIO_Pin_12);break;
		case CHIP2: GPIO_SetBits(GPIOD,GPIO_Pin_8);;break;
		default: break;
	}
/*
	if(value>255)
		value=255;
	if(value<0)
		value=0;
	switch(chip){
		case CHIP1: Set_Left_Speed(value-64);break;
		case CHIP2: Set_Right_Speed(value-64);break;
		default: break;
	}
*/
	
}

void My_UD_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	GPIO_SetBits(GPIOD,GPIO_Pin_10);
	GPIO_SetBits(GPIOD,GPIO_Pin_11);
	My_UD_Reset();
	
}

void My_Left_Speed_Up(void){
	GPIO_SetBits(GPIOD,GPIO_Pin_8);//INC
	wait(1);
	GPIO_SetBits(GPIOD,GPIO_Pin_9);//UP
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);//CS select
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_10);
	wait(1);
	left_speed++;
	if(left_speed >64)
		left_speed = 64;
}

void My_Left_Speed_Down(void){
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_10);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	wait(1);
	left_speed--;
	if(left_speed <-64)
		left_speed = -64;
}
void My_Right_Speed_Up(void){
	GPIO_SetBits(GPIOD,GPIO_Pin_8);//INC
	wait(1);
	GPIO_SetBits(GPIOD,GPIO_Pin_9);//UP
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);//CS select
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_11);
	wait(1);
	right_speed++;
	if(right_speed >64)
		right_speed = 64;
}

void My_Right_Speed_Down(void){
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);
	wait(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_11);
	wait(5);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	wait(1);
	right_speed--;
	if(right_speed <-64)
		right_speed = -64;
}
void My_UD_Reset(void){
	int i;
	for(i = 0;i < 150;i++){
		My_Left_Speed_Down();
	}
	for(i = 0;i < 64;i++){
		My_Left_Speed_Up();
	}
	left_speed = 0;
	
	for(i = 0;i < 150;i++){
		My_Right_Speed_Down();
	}
	for(i = 0;i < 64;i++){
		My_Right_Speed_Up();
	}
	right_speed = 0;
}

void Set_Left_Speed(int value)
{
	int i;
	int count = value - left_speed;
	if(count > 0)
		for(i=0;i<count;i++)
			My_Left_Speed_Up();
	else if (count < 0)
		for(i=count;i<0;i++)
			My_Left_Speed_Down();
}
void Set_Right_Speed(int value)
{
	int i;
	int count = value - right_speed;
	if(count > 0)
		for(i=0;i<count;i++)
			My_Right_Speed_Up();
	else if (count < 0)
		for(i=count;i<0;i++)
			My_Right_Speed_Down();
}
