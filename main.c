/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "Utility.h"
#include "Motor.h"
#include "IMU.h"
#include "ADC.h"
//Library config for this project!!!!!!!!!!!
#include "stm32f4xx_conf.h"

/** @addtogroup STM32F4-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

__IO uint8_t UserButtonPressed = 0x00;
uint8_t Button_state = BUTTON_STATE_1;
 __IO uint32_t TimingDelay=10;
 __IO uint16_t Sample_time = 10;
unsigned char buffer[100];

/* Private function prototypes -----------------------------------------------*/
void run_function1(void);//test function
void run_function2(void);//test  state2
/* Private functions ---------------------------------------------------------*/
extern __IO uint16_t ADC3ConvertedValue;
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	 	
int i =0;
int main(void)
{
  
  /* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/
 __IO uint32_t i = 0;  
	Init_Clock();
	//My_SPI_Init();
	
 USBD_Init(&USB_OTG_dev,     
            USB_OTG_FS_CORE_ID, 
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);

 STM_EVAL_LEDInit(LED6);
 STM_EVAL_LEDInit(LED4);
 STM_EVAL_LEDInit(LED3);
 STM_EVAL_LEDInit(LED5);
	
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED3);
 	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 
	LSM303DLHC_MEMS_Init();
	L3GD20_MEMS_Init();
	My_UD_Init();//init X9C102
	//My_ADC_Init();
	//ADC_SoftwareStartConv(ADC1);
  /* Infinite loop */
		while (1)
		{
			while(UserButtonPressed==0x00){
				if(Button_state == BUTTON_STATE_1){ wait(10000);}
				if(Button_state == BUTTON_STATE_2){ My_UD_Test_Down();STM_EVAL_LEDToggle(LED3);wait(10000);/*run_function1();wait(10000);*/}
				if(Button_state == BUTTON_STATE_3){ My_UD_Test_Up();STM_EVAL_LEDToggle(LED4);wait(10000);/*run_function2();*/}
			
			}
			wait(1000);
			while (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET){}	
			wait(1000);
			if(Button_state == BUTTON_STATE_3){
				Button_state = BUTTON_STATE_1;
				STM_EVAL_LEDOff(LED4);
				STM_EVAL_LEDOff(LED3);
				STM_EVAL_LEDOff(LED5);
				STM_EVAL_LEDOff(LED6); 
			}else if(Button_state == BUTTON_STATE_1){
					Button_state = BUTTON_STATE_2;
					//run_function();
			}else if(Button_state == BUTTON_STATE_2){
					Button_state = BUTTON_STATE_3;
					//run_function();
			}
			wait(1000);	
			UserButtonPressed = 0x00;		
		}
		
	 
	
	
	
	
}

void run_function1(void){
//DATA TRANSMISSION
	unsigned char buff[100];
	float IMU_Buffer1[3];
	  //float Buffer[6];
  uint8_t Xval, Yval = 0x00;
	int16_t IMU_Buffer[3];
//	unsigned char start[] = "start";
//	LSM303DLHC_MEMS_Test();
	int speed=0, accuracy=0; 
		int sum = 0;
	if(VCP_receive_string(buff)){//if receive command
		if(buff[0]=='C'){
			switch(buff[1]){
				case 'A': 
								Read_Acc(IMU_Buffer);
									switch(buff[2]){
											case 'X': VCP_send_IMU_Single(ACC_INDEX,IMU_Buffer[0]);break;
											case 'Y': VCP_send_IMU_Single(ACC_INDEX,IMU_Buffer[1]);break; 
											case 'Z': VCP_send_IMU_Single(ACC_INDEX,IMU_Buffer[2]);break; 
											case 'A': VCP_send_IMU_Multi(ACC_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);break;
											default :break;
									} 
									break; 
				case 'G': //GyroReadAngRate(IMU_Buffer);
									Read_Gyro(IMU_Buffer);
									switch(buff[2]){
											case 'X': VCP_send_IMU_Single(GYRO_INDEX,IMU_Buffer[0]);break;
											case 'Y': VCP_send_IMU_Single(GYRO_INDEX,IMU_Buffer[1]);break; 
											case 'Z': VCP_send_IMU_Single(GYRO_INDEX,IMU_Buffer[2]);break; 
											case 'A': VCP_send_IMU_Multi(GYRO_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);break;
											default :break;
									} 
									break;
				case 'C': //LSM303DLHC_CompassReadMag(IMU_Buffer);
									Read_Compass(IMU_Buffer);
									switch(buff[2]){
											case 'X': VCP_send_IMU_Single(COMPASS_INDEX,IMU_Buffer[0]);break;
											case 'Y': VCP_send_IMU_Single(COMPASS_INDEX,IMU_Buffer[1]);break; 
											case 'Z': VCP_send_IMU_Single(COMPASS_INDEX,IMU_Buffer[2]);break; 
											case 'A': VCP_send_IMU_Multi(COMPASS_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);break;
											default :break;
									} 
									break;
				case 'I': speed = buff[3]-'0';accuracy = buff[4]-'0';
									switch(buff[2]){
											case 'A': Acc_Init(speed,accuracy);break;
											case 'C': Compass_Init(speed,accuracy);break;
											case 'G': Gyro_Init(speed,accuracy);break;
											default : break;
									} 
									break;
				case 'M':sum = (buff[3]-'0')*100+(buff[4]-'0')*10+(buff[5]-'0') ;Potentialmeter_SetValue(sum,buff[2]-'0'); break;
				case 'L':if(buff[3]=='1') STM_EVAL_LEDOn(buff[2]-'1'); else STM_EVAL_LEDOff(buff[2]-'1'); break;
				case 'S': sum = (buff[2]-'0')*100+(buff[3]-'0')*10+(buff[4]-'0') ;Sample_time=sum; break;
				default :break;				
			}	
		}

	}
		
//	Potentialmeter_SetValue(i++,CHIP1);
	
//	VCP_receive_string(buff);
//	VCP_send_str(buff);
	/*
//MEMS303	
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, 6, buff);
	VCP_send_str(buff);
	VCP_put_char(100);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M, 6, buff);
	VCP_send_str(buff);
	VCP_put_char(100);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M, 6, buff);
	VCP_send_str(buff);
	VCP_put_char(100);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M, 6, buff);
	VCP_send_str(buff);
	VCP_put_char(100);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M, 6, buff);
	VCP_send_str(buff);
	VCP_put_char(100);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M, 6, buff);
	VCP_send_str(buff);
	VCP_put_char(101);
	//VCP_put_char('\n');
	
	L3GD20_Read(buff,L3GD20_OUT_X_L_ADDR,6);
	VCP_send_str(buff);
	VCP_put_char(102);
	L3GD20_Read(buff,L3GD20_OUT_X_H_ADDR,6);
	VCP_send_str(buff);
	VCP_put_char(102);
	L3GD20_Read(buff,L3GD20_OUT_Y_L_ADDR,6);
	VCP_send_str(buff);
	VCP_put_char(102);
	L3GD20_Read(buff,L3GD20_OUT_Y_H_ADDR,6);
	VCP_send_str(buff);
	VCP_put_char(102);
	L3GD20_Read(buff,L3GD20_OUT_Z_L_ADDR,6);
	VCP_send_str(buff);
	VCP_put_char(102);
	L3GD20_Read(buff,L3GD20_OUT_Z_H_ADDR,6);
	VCP_send_str(buff);
	VCP_put_char(103);
	
	
	*/
	
  
  /* Demo Gyroscope */
  //Demo_GyroConfig();
  /*
  // Read Gyro Angular data 
  Read_Gyro(IMU_Buffer);
  
  // Update autoreload and capture compare registers value
  Xval = ABS(IMU_Buffer[0]);
  Yval = ABS(IMU_Buffer[1]);
  
  if ( Xval>Yval)
  {
    if (IMU_Buffer[0] > 1115.0f)
    {       
      STM_EVAL_LEDOn(LED4);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
    }
    if (IMU_Buffer[0] < -1115.0f)
    {
      STM_EVAL_LEDOn(LED5);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED6);
    }
  }
  else
  {
    if (IMU_Buffer[1]< -1115.0f)
    {
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
    }
    if (IMU_Buffer[1] > 1115.0f)
    {
      STM_EVAL_LEDOn(LED6);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED5);
    } 
  } 
	
	
	LSM303DLHC_CompassReadAcc(IMU_Buffer1);
	Xval = ABS(IMU_Buffer1[0]);
  Yval = ABS(IMU_Buffer1[1]);
  
  if ( Xval>Yval)
  {
    if (IMU_Buffer1[0] > 1115.0f)
    {       
      STM_EVAL_LEDOn(LED4);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
    }
    if (IMU_Buffer1[0] < -1115.0f)
    {
      STM_EVAL_LEDOn(LED5);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED6);
    }
  }
  else
  {
    if (IMU_Buffer1[1]< -1115.0f)
    {
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
    }
    if (IMU_Buffer1[1] > 1115.0f)
    {
      STM_EVAL_LEDOn(LED6);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED5);
    } 
  } 
	//MEMS303	
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, 6, buff);
	VCP_put_char(buff[0]);
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A, 6, buff);
	VCP_put_char(buff[0]);
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_A, 6, buff);
	VCP_put_char(buff[0]);
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_A, 6, buff);
	VCP_put_char(buff[0]);
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_A, 6, buff);
	VCP_put_char(buff[0]);
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_A, 6, buff);
	VCP_put_char(buff[0]);
	
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, 6, buff);
	VCP_send_str(buff);
	//VCP_put_char('\n');*/
	STM_EVAL_LEDOn(LED5);
	wait(10000);
//	Potentialmeter_SetValue(i++,CHIP2);
	STM_EVAL_LEDOff(LED5);
	wait(10000);

	
}
void run_function2(void){
	uint8_t Xval, Yval = 0x00;
	int16_t IMU_Buffer[3];
	unsigned char buff[100];
	float IMU_Buffer1[3];
	uint8_t speed=0, accuracy=0; 
	int sum = 0;
	uint32_t ADC_temp = 0;
	if(VCP_receive_string(buff)){//if receive command
		if(buff[0]=='C'){
			Button_state = BUTTON_STATE_1;
			switch(buff[1]){
				case 'A': 
								Read_Acc(IMU_Buffer);
									switch(buff[2]){
											case 'X': VCP_send_IMU_Single(ACC_INDEX,IMU_Buffer[0]);break;
											case 'Y': VCP_send_IMU_Single(ACC_INDEX,IMU_Buffer[1]);break; 
											case 'Z': VCP_send_IMU_Single(ACC_INDEX,IMU_Buffer[2]);break; 
											case 'A': VCP_send_IMU_Multi(ACC_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);break;
											default :break;
									} 
									break; 
				case 'G': //GyroReadAngRate(IMU_Buffer);
									Read_Gyro(IMU_Buffer);
									switch(buff[2]){
											case 'X': VCP_send_IMU_Single(GYRO_INDEX,IMU_Buffer[0]);break;
											case 'Y': VCP_send_IMU_Single(GYRO_INDEX,IMU_Buffer[1]);break; 
											case 'Z': VCP_send_IMU_Single(GYRO_INDEX,IMU_Buffer[2]);break; 
											case 'A': VCP_send_IMU_Multi(GYRO_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);break;
											default :break;
									} 
									break;
				case 'C': //LSM303DLHC_CompassReadMag(IMU_Buffer);
									Read_Compass(IMU_Buffer);
									switch(buff[2]){
											case 'X': VCP_send_IMU_Single(COMPASS_INDEX,IMU_Buffer[0]);break;
											case 'Y': VCP_send_IMU_Single(COMPASS_INDEX,IMU_Buffer[1]);break; 
											case 'Z': VCP_send_IMU_Single(COMPASS_INDEX,IMU_Buffer[2]);break; 
											case 'A': VCP_send_IMU_Multi(COMPASS_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);break;
											default :break;
									} 
									break;
				case 'I': speed = buff[3]-'0';accuracy = buff[4]-'0';
									switch(buff[2]){
											case 'A': Acc_Init(speed,accuracy);break;
											case 'C': Compass_Init(speed,accuracy);break;
											case 'G': Gyro_Init(speed,accuracy);break;
											default : break;
									} 
									break;
				case 'M':sum = (buff[3]-'0')*100+(buff[4]-'0')*10+(buff[5]-'0') ;Potentialmeter_SetValue(sum,buff[2]-'0'); break;
				case 'L':if(buff[3]=='1') STM_EVAL_LEDOn(buff[2]-'1'); else STM_EVAL_LEDOff(buff[2]-'1'); break;
				case 'S': sum = (buff[2]-'0')*100+(buff[3]-'0')*10+(buff[4]-'0') ;Sample_time=sum; break;
				default :break;				
			}	
				Button_state = BUTTON_STATE_3;
		}
	}
	/*
	ADC_temp = ADC3ConvertedValue*3000/0xFFF;
	if(ADC_temp > 0&&ADC_temp<1000)
		STM_EVAL_LEDOn(LED3);
	else if(ADC_temp > 1000&&ADC_temp<2000 )
		STM_EVAL_LEDOn(LED4);
	else if(ADC_temp > 2000&&ADC_temp<3000 )
		STM_EVAL_LEDOn(LED5);
	else{
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
		STM_EVAL_LEDOff(LED5);
	}
	VCP_send_int((uint16_t)ADC_temp);
	wait(1000);*/
}

void Send_IMU_Info(void){
	
	int16_t IMU_Buffer[9];
	Read_Acc(&IMU_Buffer[0]);
	//VCP_send_IMU_Multi(ACC_INDEX,IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2]);
	Read_Gyro(&IMU_Buffer[3]);
	//VCP_send_IMU_Multi(GYRO_INDEX,IMU_Buffer[3],IMU_Buffer[4],IMU_Buffer[5]);
	Read_Compass(&IMU_Buffer[6]);
	//VCP_send_IMU_Multi(COMPASS_INDEX,IMU_Buffer[6],IMU_Buffer[7],IMU_Buffer[8]);

	/*Read_Acc(&IMU_Buffer[0]);
	Read_Gyro(&IMU_Buffer[3]);
	Read_Compass(&IMU_Buffer[6]);*/
	VCP_send_IMU_All(IMU_Buffer[0],IMU_Buffer[1],IMU_Buffer[2],IMU_Buffer[3],IMU_Buffer[4],IMU_Buffer[5],IMU_Buffer[6],IMU_Buffer[7],IMU_Buffer[8]);
}


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
	
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
void Init_Clock(void){
	//external clock
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_ClocksTypeDef RCC_Clocks;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = UPLOAD_FREQ;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
  
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
  
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
	
	/* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

	
	
}


/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Fail_Handler(void)
{
  /* Erase last sector */ 
  FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
  /* Write FAIL code at last word in the flash memory */
  FLASH_ProgramWord(TESTRESULT_ADDRESS, ALLTEST_FAIL);
  
  while(1)
  {
    /* Toggle Red LED */
    STM_EVAL_LEDToggle(LED5);
    Delay(5);
  }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
