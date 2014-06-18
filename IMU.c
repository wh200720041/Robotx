#include "IMU.h"


void LSM303DLHC_MEMS_Init(void)
{
	//accelemeter init
	//float pBuffer[6];
  LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitStructure;
  LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;
  LSM303DLHCMag_InitTypeDef LSM303DLHC_InitStruct;
	
  //uint8_t xdata = 0, ydata = 0;
  
  /* MEMS configuration ------------------------------------------------------*/
   /* Fill the accelerometer structure */
  LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
  LSM303DLHCAcc_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_100_HZ;
  LSM303DLHCAcc_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
  LSM303DLHCAcc_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
  LSM303DLHCAcc_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
  LSM303DLHCAcc_InitStructure.Endianness=LSM303DLHC_BLE_LSB;
  LSM303DLHCAcc_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;
  /* Configure the accelerometer main parameters */
  LSM303DLHC_AccInit(&LSM303DLHCAcc_InitStructure);
  
  /* Fill the accelerometer LPF structure */
  LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection =LSM303DLHC_HPM_NORMAL_MODE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE;

  /* Configure the accelerometer LPF main parameters */
  LSM303DLHC_AccFilterConfig(&LSM303DLHCFilter_InitStructure);

  /* Required delay for the MEMS Accelerometre: Turn-on time = 3/Output data Rate 
                                                             = 3/100 = 30ms */
  wait(10000);
  
 
		//Compass Init
		
		LSM303DLHC_InitStruct.MagFull_Scale = LSM303DLHC_TEMPSENSOR_DISABLE;
		LSM303DLHC_InitStruct.MagOutput_DataRate = LSM303DLHC_ODR_75_HZ;
		LSM303DLHC_InitStruct.MagFull_Scale = LSM303DLHC_FS_4_0_GA;
		LSM303DLHC_InitStruct.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION; 
		LSM303DLHC_MagInit(&LSM303DLHC_InitStruct);
		wait(10000);
}

/**
  * @brief Test L3GD20 MEMS Hardware.
  *   The main objectif of this test is to check the hardware connection of the 
  *   MEMS peripheral.
  * @param None
  * @retval None
  */
void L3GD20_MEMS_Init(void)
{
  L3GD20_InitTypeDef L3GD20_InitStructure;
  L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;
  /* Configure Mems L3GD20 */
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
  L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_3;
  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
  L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
  L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_250; 
  L3GD20_Init(&L3GD20_InitStructure);
  
  L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;
  
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);

}

void Gyro_Init(uint8_t speed, uint8_t sensitivity)
{
  L3GD20_InitTypeDef L3GD20_InitStructure;
  L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;
	if(speed>4||speed<1)
		return;
	if(sensitivity<1||sensitivity>3)
		return;
	sensitivity = (sensitivity-1)<<4;
	speed = (speed-1) <<6;
  /* Configure Mems L3GD20 */
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
  L3GD20_InitStructure.Output_DataRate = speed;
  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
  L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
  L3GD20_InitStructure.Full_Scale = sensitivity; 
  L3GD20_Init(&L3GD20_InitStructure);
  
  L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;
  
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);

}
void Acc_Init(uint8_t speed, uint8_t sensitivity)
{
	  //accelemeter init
  LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitStructure;
  LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;
	if(speed>9||speed<1)
		return;
	if(sensitivity<1||sensitivity>4)
		return;
	sensitivity = (sensitivity-1)<<4;
	speed = speed <<4;

  
  /* MEMS configuration ------------------------------------------------------*/
   /* Fill the accelerometer structure */
  LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
  LSM303DLHCAcc_InitStructure.AccOutput_DataRate = speed;
  LSM303DLHCAcc_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
  LSM303DLHCAcc_InitStructure.AccFull_Scale = sensitivity;
  LSM303DLHCAcc_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
  LSM303DLHCAcc_InitStructure.Endianness=LSM303DLHC_BLE_LSB;
  LSM303DLHCAcc_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;
  /* Configure the accelerometer main parameters */
  LSM303DLHC_AccInit(&LSM303DLHCAcc_InitStructure);
  
  /* Fill the accelerometer LPF structure */
  LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection =LSM303DLHC_HPM_NORMAL_MODE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
  LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE;

  /* Configure the accelerometer LPF main parameters */
  LSM303DLHC_AccFilterConfig(&LSM303DLHCFilter_InitStructure);

}
void Compass_Init(uint8_t speed, uint8_t sensitivity)
{
		LSM303DLHCMag_InitTypeDef LSM303DLHC_InitStruct;
	if(speed>8||speed<1)
		return;
	if(sensitivity<1||sensitivity>4)
		return;
	sensitivity = sensitivity<<5;
	speed = (speed-1) <<2;
		LSM303DLHC_InitStruct.MagFull_Scale = LSM303DLHC_TEMPSENSOR_DISABLE;
		LSM303DLHC_InitStruct.MagOutput_DataRate = speed;
		LSM303DLHC_InitStruct.MagFull_Scale = sensitivity;
		LSM303DLHC_InitStruct.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION; 
		LSM303DLHC_MagInit(&LSM303DLHC_InitStruct);
		wait(1000);

}
/**
* @brief Read LSM303DLHC output register, and calculate the acceleration ACC=(1/SENSITIVITY)* (out_h*256+out_l)/16 (12 bit rappresentation)
* @param pnData: pointer to float buffer where to store data
* @retval None
*/
void LSM303DLHC_CompassReadAcc(float* pfData)
{
  int16_t pnRawData[3];
  uint8_t ctrlx[2];
  uint8_t buffer[6], cDivider;
  uint8_t i = 0;
  float LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
  
  /* Read the register content */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, 2, ctrlx);
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, 6, buffer);
  
  if(ctrlx[1]&0x40)
    cDivider=64;
  else
    cDivider=16;
  
  /* check in the control register4 the data alignment*/
  if(!(ctrlx[0] & 0x40) || (ctrlx[1] & 0x40)) /* Little Endian Mode or FIFO mode */
  {
    for(i=0; i<3; i++)
    {
      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i])/cDivider;
    }
  }
  else /* Big Endian Mode */
  {
    for(i=0; i<3; i++)
      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i+1])/cDivider;
  }
  /* Read the register content */
  LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, 2, ctrlx);
  
  
  if(ctrlx[1]&0x40)
  {
    /* FIFO mode */
    LSM_Acc_Sensitivity = 0.25;
  }
  else
  {
    /* normal mode */
    /* switch the sensitivity value set in the CRTL4*/
    switch(ctrlx[0] & 0x30)
    {
    case LSM303DLHC_FULLSCALE_2G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
      break;
    case LSM303DLHC_FULLSCALE_4G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
      break;
    case LSM303DLHC_FULLSCALE_8G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
      break;
    case LSM303DLHC_FULLSCALE_16G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
      break;
    }
  }
  
  /* Obtain the mg value for the three axis */
  for(i=0; i<3; i++)
  {
    pfData[i]=(float)pnRawData[i]/LSM_Acc_Sensitivity;
  }
}

void LSM303DLHC_CompassReadMag(float* pfData)
{
  static uint8_t buffer[6] = {0};
  uint8_t CTRLB = 0;
  uint16_t Magn_Sensitivity_XY = 0, Magn_Sensitivity_Z = 0;
  uint8_t i =0;
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_CRB_REG_M, 1, &CTRLB);
  
  LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, 6, buffer);
  
  switch(CTRLB & 0xE0)
  {
  case LSM303DLHC_FS_1_3_GA:
   
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_3Ga; //??1100
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_3Ga;   //??980
    break;
  case LSM303DLHC_FS_1_9_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_9Ga;//??855
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_9Ga; //??760
    break;
  case LSM303DLHC_FS_2_5_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_2_5Ga; //??670
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_2_5Ga; //??600
    break;
  case LSM303DLHC_FS_4_0_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4Ga; //??450
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4Ga; //??400
    break;
  case LSM303DLHC_FS_4_7_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4_7Ga; //??400
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4_7Ga;//??355
    break;
  case LSM303DLHC_FS_5_6_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_5_6Ga;//??330
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_5_6Ga;//??295
    break;
  case LSM303DLHC_FS_8_1_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;//??230
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;//??205
    break;
  }
 
  for(i=0; i<2; i++)
  {
    pfData[i]=(float)((int16_t)(((uint16_t)buffer[2*i] << 8) + buffer[2*i+1])*1000)/Magn_Sensitivity_XY;
  }
  pfData[2]=(float)((int16_t)(((uint16_t)buffer[4] << 8) + buffer[5])*1000)/Magn_Sensitivity_Z;
}

/**
* @brief  Calculate the angular Data rate Gyroscope.
* @param  pfData : Data out pointer
* @retval None
*/
void GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  uint32_t wCounter =0;
  //L3GD20_MEMS_Init();
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(wCounter=0; wCounter<3; wCounter++)
    {
      RawData[wCounter]=(int16_t)(((uint16_t)tmpbuffer[2*wCounter+1] << 8) + tmpbuffer[2*wCounter]);
    }
  }
  else
  {
    for(wCounter=0; wCounter<3; wCounter++)
    {
      RawData[wCounter]=(int16_t)(((uint16_t)tmpbuffer[2*wCounter] << 8) + tmpbuffer[2*wCounter+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    sensitivity=L3G_Sensitivity_250dps;
    break;
    
  case 0x10:
    sensitivity=L3G_Sensitivity_500dps;
    break;
    
  case 0x20:
    sensitivity=L3G_Sensitivity_2000dps;
    break;
  }
  /* divide by sensitivity */
  for(wCounter=0; wCounter<3; wCounter++)
  {
    pfData[wCounter]=(float)RawData[wCounter]/sensitivity;
  }
}




void Read_Compass(int16_t* pfData){
	unsigned char buffer[6];
	uint8_t CTRLB = 0;
  uint16_t Magn_Sensitivity_XY = 0, Magn_Sensitivity_Z = 0;
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_CRB_REG_M, 1, &CTRLB);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, 6, buffer);
	switch(CTRLB & 0xE0)
  {
  case LSM303DLHC_FS_1_3_GA:
   
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_3Ga; //??1100
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_3Ga;   //??980
    break;
  case LSM303DLHC_FS_1_9_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_9Ga;//??855
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_9Ga; //??760
    break;
  case LSM303DLHC_FS_2_5_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_2_5Ga; //??670
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_2_5Ga; //??600
    break;
  case LSM303DLHC_FS_4_0_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4Ga; //??450
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4Ga; //??400
    break;
  case LSM303DLHC_FS_4_7_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4_7Ga; //??400
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4_7Ga;//??355
    break;
  case LSM303DLHC_FS_5_6_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_5_6Ga;//??330
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_5_6Ga;//??295
    break;
  case LSM303DLHC_FS_8_1_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;//??230
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;//??205
    break;
  }
/*
	pfData[0] = (int16_t)((((int)((int16_t)((uint16_t)buffer[0]<<8) +(uint16_t)buffer[1])))*4048/Magn_Sensitivity_XY);
	pfData[1] = (int16_t)((((int)((int16_t)((uint16_t)buffer[2]<<8) +(uint16_t)buffer[3])))*4048/Magn_Sensitivity_XY);
	pfData[2] = (int16_t)((((int)((int16_t)((uint16_t)buffer[4]<<8) +(uint16_t)buffer[5])))*4048/Magn_Sensitivity_Z);
*/	
	pfData[0] =(((uint16_t)buffer[0]<<8) +(uint16_t)buffer[1]);
	pfData[1] =(((uint16_t)buffer[2]<<8) +(uint16_t)buffer[3]);
	pfData[2] =(((uint16_t)buffer[4]<<8) +(uint16_t)buffer[5]);
}

void Read_Gyro(int16_t* RawData){
	uint8_t tmpbuffer[6] ={0};
  uint8_t tmpreg = 0;
  //float sensitivity = 0;
	uint8_t wCounter ;
  //L3GD20_MEMS_Init();
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(wCounter=0; wCounter<3; wCounter++)
    {
      RawData[wCounter]=(int16_t)(((uint16_t)tmpbuffer[2*wCounter+1] << 8) + tmpbuffer[2*wCounter]);
    }
  }
  else
  {
    for(wCounter=0; wCounter<3; wCounter++)
    {
      RawData[wCounter]=(int16_t)(((uint16_t)tmpbuffer[2*wCounter] << 8) + tmpbuffer[2*wCounter+1]);
    }
  }
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    //sensitivity=8;
		
    break;
    
  case 0x10:
    //sensitivity=4;
		RawData[0] *= 2;
		RawData[1] *= 2;
		RawData[2] *= 2;
    break;
    
  case 0x20:
    //sensitivity=1;
		RawData[0] *= 8;
		RawData[1] *= 8;
		RawData[2] *= 8;
    break;
  }
	/*
	 final processing, but will consume time, I leave it to the main computer
	RawData[0]=(int16_t)(((float)RawData[0]/sensitivity));
	RawData[1]=(int16_t)(((float)RawData[1]/sensitivity));
	RawData[2]=(int16_t)(((float)RawData[2]/sensitivity));
	*/

}

void Read_Acc(int16_t* pfData){
	unsigned char buff[1];
	unsigned char buffer[6];
	float LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
	uint8_t cDivider;
	uint8_t ctrlx[2];
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, 2, ctrlx);
	if(ctrlx[1]&0x40)
    cDivider=64;
  else
    cDivider=16;
	//LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, 6, buffer);
	
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, 1, buff);
	buffer[0] = buff[0];

	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A, 1, buff);
	buffer[1] = buff[0];

	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_A, 1, buff);
	buffer[2] = buff[0];

	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_A, 1, buff);
	buffer[3] = buff[0];

	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_A, 1, buff);
	buffer[4] = buff[0];

	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_A, 1, buff);
	buffer[5] = buff[0];

	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, 2, ctrlx);
  
  
  if(ctrlx[1]&0x40)
  {
    /* FIFO mode */
    LSM_Acc_Sensitivity = 0.25;
  }
  else
  {
    /* normal mode */
    /* switch the sensitivity value set in the CRTL4*/
    switch(ctrlx[0] & 0x30)
    {
    case LSM303DLHC_FULLSCALE_2G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
      break;
    case LSM303DLHC_FULLSCALE_4G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
      break;
    case LSM303DLHC_FULLSCALE_8G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
      break;
    case LSM303DLHC_FULLSCALE_16G:
      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
      break;
    }
  }
  /*
	pfData[0] = (int16_t)((float)(((int16_t)(((uint16_t)buffer[1]<<8) +(uint16_t)buffer[0]))/cDivider)/LSM_Acc_Sensitivity);
	pfData[1] = (int16_t)((float)(((int16_t)(((uint16_t)buffer[3]<<8) +(uint16_t)buffer[2]))/cDivider)/LSM_Acc_Sensitivity);
	pfData[2] = (int16_t)((float)(((int16_t)(((uint16_t)buffer[5]<<8) +(uint16_t)buffer[4]))/cDivider)/LSM_Acc_Sensitivity);
*/
	pfData[0] = ((uint16_t)buffer[1]<<8) +(uint16_t)buffer[0];
	pfData[1] = ((uint16_t)buffer[3]<<8) +(uint16_t)buffer[2];
	pfData[2] = ((uint16_t)buffer[5]<<8) +(uint16_t)buffer[4];
}

/**
  * @brief  LSM303DLHC MEMS accelerometre management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t LSM303DL_TIMEOUT_UserCallback(void)
{
  /* MEMS Accelerometer Timeout error occured during Test program execution */
  if (/*Button_state*/ BUTTON_STATE_2== BUTTON_STATE_2)
  {
    /* Timeout error occured for SPI TXE/RXNE flags waiting loops.*/
    Fail_Handler();    
  }
  /* MEMS Accelerometer Timeout error occured during Demo execution */
  else
  {
    while (1)
    {   
    }
  }
  return 0;  
}

/**
  * @brief  L3GD20 MEMS accelerometre management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  /* MEMS Accelerometer Timeout error occured during Test program execution */
  if (/*Button_state*/ BUTTON_STATE_2 == BUTTON_STATE_2)
  {
    /* Timeout error occured for SPI TXE/RXNE flags waiting loops.*/
    Fail_Handler();    
  }
  /* MEMS Accelerometer Timeout error occured during Demo execution */
  else
  {
    while (1)
    {   
    }
  }
  return 0;  
}
