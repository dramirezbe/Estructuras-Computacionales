/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <_ansi.h>
#include <stdlib.h>
#include <stddef.h>

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
#define BUTTON_Pin GPIO_PIN_13
#define BUTTON_GPIO_Port GPIOC
#define BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define DOOR_Pin GPIO_PIN_4
#define DOOR_GPIO_Port GPIOA
#define HEARTBIT_Pin GPIO_PIN_5
#define HEARTBIT_GPIO_Port GPIOA
#define TRIAC_PULSE_Pin GPIO_PIN_1
#define TRIAC_PULSE_GPIO_Port GPIOB
#define ZERO_DETECT_Pin GPIO_PIN_2
#define ZERO_DETECT_GPIO_Port GPIOB
#define ZERO_DETECT_EXTI_IRQn EXTI2_IRQn
#define COLUMN_1_Pin GPIO_PIN_10
#define COLUMN_1_GPIO_Port GPIOB
#define COLUMN_1_EXTI_IRQn EXTI15_10_IRQn
#define COLUMN_4_Pin GPIO_PIN_7
#define COLUMN_4_GPIO_Port GPIOC
#define COLUMN_4_EXTI_IRQn EXTI9_5_IRQn
#define COLUMN_3_Pin GPIO_PIN_8
#define COLUMN_3_GPIO_Port GPIOA
#define COLUMN_3_EXTI_IRQn EXTI9_5_IRQn
#define COLUMN_2_Pin GPIO_PIN_9
#define COLUMN_2_GPIO_Port GPIOA
#define COLUMN_2_EXTI_IRQn EXTI9_5_IRQn
#define ROW_1_Pin GPIO_PIN_10
#define ROW_1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define ROW_2_Pin GPIO_PIN_3
#define ROW_2_GPIO_Port GPIOB
#define ROW_4_Pin GPIO_PIN_4
#define ROW_4_GPIO_Port GPIOB
#define ROW_3_Pin GPIO_PIN_5
#define ROW_3_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_6
#define DHT11_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RX_WIFI_BUFFER_SIZE 16
#define TX_WIFI_BUFFER_SIZE 2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
