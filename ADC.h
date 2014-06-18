
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f401_discovery.h"
#include "main.h"
#include "Utility.h"
#include <stdio.h>


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* TIM2 Autoreload and Capture Compare register values */
/* Exported macro ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS     ((uint32_t)0x4001204C)
#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)
/* Exported functions ------------------------------------------------------- */

void My_ADC_Init(void);
#endif /* __STM32F4_DISCOVERY_DEMO_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
