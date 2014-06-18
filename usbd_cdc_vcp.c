/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Generic media access Layer.
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

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "stm32f401_discovery.h"
//Library config for this project!!!!!!!!!!!
#include "stm32f4xx_conf.h"
#include "IMU.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LINE_CODING linecoding =
  {
    9600, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };


USART_InitTypeDef USART_InitStructure;

/* These are external variables imported from CDC core to be used for IN 
   transfer management. */
extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);

static uint16_t VCP_COMConfig(uint8_t Conf);

CDC_IF_Prop_TypeDef VCP_fops = 
{
  VCP_Init,
  VCP_DeInit,
  VCP_Ctrl,
  VCP_DataTx,
  VCP_DataRx
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  VCP_Init
  *         Initializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Init(void)
{
  return USBD_OK;
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_DeInit(void)
{
  return USBD_OK;
}


/**
  * @brief  VCP_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    /* Not  needed for this driver */
    break;

  case GET_ENCAPSULATED_RESPONSE:
    /* Not  needed for this driver */
    break;

  case SET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case GET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case CLEAR_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case SET_LINE_CODING:
	/* Not  needed for this driver */ 
    break;

  case GET_LINE_CODING:
    Buf[0] = (uint8_t)(linecoding.bitrate);
    Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
    Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
    Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
    Buf[4] = linecoding.format;
    Buf[5] = linecoding.paritytype;
    Buf[6] = linecoding.datatype; 
    break;

  case SET_CONTROL_LINE_STATE:
    /* Not  needed for this driver */
    break;

  case SEND_BREAK:
    /* Not  needed for this driver */
    break;    
    
  default:
    break;
  }

  return USBD_OK;
}


/**
  * @brief  putchar
  *         Sends one char over the USB serial link.
  * @param  buf: char to be sent
  * @retval none
  */

void VCP_put_char(uint8_t buf)
{
	VCP_DataTx(&buf,1);
}

void VCP_send_str(uint8_t* buf)
{
	uint32_t i=0;
	while(*(buf + i))
	{
		i++;
	}
	VCP_DataTx(buf, i);
}
void VCP_send_int(int16_t temp){
	unsigned char tempInt[2];
	tempInt[0] = (unsigned char)((temp & 0xFF00)>>8);
	tempInt[1] = (unsigned char)(temp & 0x00FF);
	VCP_DataTx(tempInt, 2);
}

void VCP_send_IMU_Single(unsigned char IMU_Index,int16_t temp){//refer to command description
	unsigned char tempInt[5];
	tempInt[0] =IMU_Index;
	tempInt[2] = (unsigned char)((temp & 0xFF00)>>8); //due to the problem of C# "bitconverter.toint16" problem, I change the sequence
	tempInt[1] = (unsigned char)(temp & 0x00FF);
	tempInt[3] = '\r';
	tempInt[4] = '\n';
	VCP_DataTx(tempInt, 5);
}
void VCP_send_IMU_Multi(unsigned char IMU_Index,int16_t temp1,int16_t temp2,int16_t temp3){//refer to command description
	unsigned char tempInt[9];
	tempInt[0] = IMU_Index + 1;
	tempInt[2] = (unsigned char)((temp1 & 0xFF00)>>8);
	tempInt[1] = (unsigned char)(temp1 & 0x00FF);
	tempInt[4] = (unsigned char)((temp2 & 0xFF00)>>8);
	tempInt[3] = (unsigned char)(temp2 & 0x00FF);
	tempInt[6] = (unsigned char)((temp3 & 0xFF00)>>8);
	tempInt[5] = (unsigned char)(temp3 & 0x00FF);
	tempInt[7] = '\r';
	tempInt[8] = '\n';
	VCP_DataTx(tempInt, 9);
}
//sequence are ACC,COMPASS,GYRO
void VCP_send_IMU_All(int16_t temp1,int16_t temp2,int16_t temp3,int16_t temp4,int16_t temp5,int16_t temp6,int16_t temp7,int16_t temp8,int16_t temp9){//refer to command description
	unsigned char tempInt[21];
	tempInt[0] = 7;
	tempInt[2] = (unsigned char)((temp1 & 0xFF00)>>8);
	tempInt[1] = (unsigned char)(temp1 & 0x00FF);
	tempInt[4] = (unsigned char)((temp2 & 0xFF00)>>8);
	tempInt[3] = (unsigned char)(temp2 & 0x00FF);
	tempInt[6] = (unsigned char)((temp3 & 0xFF00)>>8);
	tempInt[5] = (unsigned char)(temp3 & 0x00FF);
	tempInt[8] = (unsigned char)((temp4 & 0xFF00)>>8);
	tempInt[7] = (unsigned char)(temp4 & 0x00FF);
	tempInt[10] = (unsigned char)((temp5 & 0xFF00)>>8);
	tempInt[9] = (unsigned char)(temp5 & 0x00FF);
	tempInt[12] = (unsigned char)((temp6 & 0xFF00)>>8);
	tempInt[11] = (unsigned char)(temp6 & 0x00FF);
	tempInt[14] = (unsigned char)((temp7 & 0xFF00)>>8);
	tempInt[13] = (unsigned char)(temp7 & 0x00FF);
	tempInt[16] = (unsigned char)((temp8 & 0xFF00)>>8);
	tempInt[15] = (unsigned char)(temp8 & 0x00FF);
	tempInt[18] = (unsigned char)((temp9 & 0xFF00)>>8);
	tempInt[17] = (unsigned char)(temp9 & 0x00FF);
	tempInt[19] = '\r';
	tempInt[20] = '\n';
	VCP_DataTx(tempInt, 21);
}
/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in 
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len)
{
	uint32_t i=0;
	while(i < Len)
	{
		APP_Rx_Buffer[APP_Rx_ptr_in] = *(Buf + i);
		APP_Rx_ptr_in++;
  		i++;
		/* To avoid buffer overflow */
		if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
		{
			APP_Rx_ptr_in = 0;
		}  
	}
	
  return USBD_OK;
}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */

//Ha Ha ,FIFO we studied in ee2008
#define APP_TX_BUF_SIZE 128
static uint8_t APP_Tx_Buffer[APP_TX_BUF_SIZE];
static int APP_tx_ptr_in_start = 0;
static int APP_tx_ptr_in_end = 0;//in this position no data

static uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
//#if 0
  uint32_t i;
	
  for (i = 0; i < Len; i++)
  {
	if((APP_tx_ptr_in_end ==  (APP_tx_ptr_in_start-1))||(APP_tx_ptr_in_start==0&&APP_tx_ptr_in_end==127))
		  return USBD_OK;
	 APP_Tx_Buffer[APP_tx_ptr_in_end]= *(Buf + i);
	 APP_tx_ptr_in_end++;
	if(APP_tx_ptr_in_end==128) 
		 APP_tx_ptr_in_end=0;
		
  } 
//#endif 
  return USBD_OK;
}

unsigned char VCP_receive_char(void)
{
	if(APP_tx_ptr_in_start==APP_tx_ptr_in_end)
		return NULL;
	APP_tx_ptr_in_start++;
	if(APP_tx_ptr_in_start==128){
		APP_tx_ptr_in_start=0;
		return APP_Tx_Buffer[127];
	}
	return APP_Tx_Buffer[APP_tx_ptr_in_start-1];
}

int VCP_receive_string(uint8_t* buf)
{
	int count =0;
	unsigned char temp;
	if(APP_tx_ptr_in_start==APP_tx_ptr_in_end)
		return 0;
	while(1)
  {
		temp = APP_Tx_Buffer[APP_tx_ptr_in_start];
		APP_tx_ptr_in_start++;
		if(APP_tx_ptr_in_start==128)
			APP_tx_ptr_in_start=0;
		if(temp!='\r'&& temp!='\n'){
			buf[count] = temp;
		}else{
			if(APP_tx_ptr_in_start==APP_tx_ptr_in_end)
				{if(count>0)return 1;else return 0;} 
			break;
		}		
		count++;
		if(APP_tx_ptr_in_start==APP_tx_ptr_in_end)
		return 1;
  } 
	
	if(APP_Tx_Buffer[APP_tx_ptr_in_start]=='\n'){
		APP_tx_ptr_in_start++;
		if(APP_tx_ptr_in_start==128)
			APP_tx_ptr_in_start=0;
	}
	return 1;

}
/**
  * @brief  VCP_COMConfig
  *         Configure the COM Port with default values or values received from host.
  * @param  Conf: can be DEFAULT_CONFIG to set the default configuration or OTHER_CONFIG
  *         to set a configuration received from the host.
  * @retval None.
  */
static uint16_t VCP_COMConfig(uint8_t Conf)
{
	//printf("test");
  return USBD_OK;
}

/**
  * @brief  EVAL_COM_IRQHandler
  *         
  * @param  None.
  * @retval None.
  */
void EVAL_COM_IRQHandler(void)
{

}
#if 0
int _write_r(void *reent, uint16_t fd, const char *ptr, uint32_t len) {
  		uint32_t counter = len;
#define STDERR_FILENO 2
#define STDOUT_FILENO 1 
        if(fd != STDOUT_FILENO && fd != STDERR_FILENO ) {                       // stdout goes to UARTx                              // stderr goes to UARTd
                return len;
        }
	
        while(counter-- > 0) {                          // Send the character from the buffer to UART
        	APP_Rx_Buffer[APP_Rx_ptr_in]= (*ptr++);
	 		APP_Rx_ptr_in++;

			/* To avoid buffer overflow */
			if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
			{
				APP_Rx_ptr_in = 0;
			}
        }
        return len;
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
