/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
void SystemClockConfigHighSpeed(void);
void SystemClockConfigLowPower(void);
void SystemInitPeripheral(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPS_TX_Pin GPIO_PIN_2
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_3
#define GPS_RX_GPIO_Port GPIOA
#define ACC_INT_1_Pin GPIO_PIN_1
#define ACC_INT_1_GPIO_Port GPIOB
#define ACC_INT_1_EXTI_IRQn EXTI1_IRQn
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOD
#define USB_ext_power_Pin GPIO_PIN_9
#define USB_ext_power_GPIO_Port GPIOA
#define ACC_SCLK_SCL_Pin GPIO_PIN_3
#define ACC_SCLK_SCL_GPIO_Port GPIOB
#define ACC_MISO_SDO_Pin GPIO_PIN_4
#define ACC_MISO_SDO_GPIO_Port GPIOB
#define ACC_MOSI_SDA_Pin GPIO_PIN_5
#define ACC_MOSI_SDA_GPIO_Port GPIOB
#define ACC_nCS0_Pin GPIO_PIN_6
#define ACC_nCS0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
