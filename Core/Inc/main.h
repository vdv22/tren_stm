/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "i2c.h"
#include "memory.h"
#include "system.h"
#include "SetAdcBotton.h"
#include "SetParametr.h"
#include "stdlib.h"
#include "beg.h"
#include "inverse.h"
#include "doubleGame.h"
#include "doubleColor.h"
#include "test.h"
typedef uint8_t u8;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define in1_Pin GPIO_PIN_0
#define in1_GPIO_Port GPIOA
#define in2_Pin GPIO_PIN_1
#define in2_GPIO_Port GPIOA
#define in3_Pin GPIO_PIN_2
#define in3_GPIO_Port GPIOA
#define in4_Pin GPIO_PIN_3
#define in4_GPIO_Port GPIOA
#define in5_Pin GPIO_PIN_4
#define in5_GPIO_Port GPIOA
#define in6_Pin GPIO_PIN_5
#define in6_GPIO_Port GPIOA
#define in7_Pin GPIO_PIN_6
#define in7_GPIO_Port GPIOA
#define in8_Pin GPIO_PIN_7
#define in8_GPIO_Port GPIOA
#define out5_Pin GPIO_PIN_2
#define out5_GPIO_Port GPIOB
#define zumer_Pin GPIO_PIN_12
#define zumer_GPIO_Port GPIOB
#define in9_Pin GPIO_PIN_8
#define in9_GPIO_Port GPIOA
#define in10_Pin GPIO_PIN_9
#define in10_GPIO_Port GPIOA
#define out1_Pin GPIO_PIN_10
#define out1_GPIO_Port GPIOA
#define out2_Pin GPIO_PIN_11
#define out2_GPIO_Port GPIOA
#define out3_Pin GPIO_PIN_12
#define out3_GPIO_Port GPIOA
#define out4_Pin GPIO_PIN_15
#define out4_GPIO_Port GPIOA
#define out6_Pin GPIO_PIN_3
#define out6_GPIO_Port GPIOB
#define out7_Pin GPIO_PIN_4
#define out7_GPIO_Port GPIOB
#define out8_Pin GPIO_PIN_5
#define out8_GPIO_Port GPIOB
#define out9_Pin GPIO_PIN_8
#define out9_GPIO_Port GPIOB
#define out10_Pin GPIO_PIN_9
#define out10_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define  ADDRESS_MINUS  0x08020000  // адрес для первой переменной
#define  ADDRESS_PLUS   0x08020002  // адрес для второй переменной
#define  ADDRESS_RESET  0x08020004
#define  ADDRESS_MODE   0x08020006
#define  ADDRESS_CICLES 0x08020008
#define  ADDRESS_PERIOD 0x0802000A

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
