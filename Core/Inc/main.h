/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdarg.h"

#include "cmsis_os.h"
#include "adc.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "fdcan.h"
#include "i2c.h"
#include "opamp.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

#include "define.h"
#include "PID.h"
#include "CAN_Driver.h"
#include "USART_Driver.h"
#include "USB_Driver.h"
#include "Function.h"
#include "DWT.h"
#include "crc8_crc16.h"
#include "Motor_Driver.h"
#include "flash.h"
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
#define IMU_CS_ACC_Pin GPIO_PIN_3
#define IMU_CS_ACC_GPIO_Port GPIOE
#define IMU_CS_GYRO_Pin GPIO_PIN_4
#define IMU_CS_GYRO_GPIO_Port GPIOE
#define IMU_INT_ACC_Pin GPIO_PIN_13
#define IMU_INT_ACC_GPIO_Port GPIOC
#define IMU_INT_GYRO_Pin GPIO_PIN_14
#define IMU_INT_GYRO_GPIO_Port GPIOC
#define Bat_ADC_Pin GPIO_PIN_7
#define Bat_ADC_GPIO_Port GPIOE
#define SPI2_CE_Pin GPIO_PIN_8
#define SPI2_CE_GPIO_Port GPIOD
#define K_L_Pin GPIO_PIN_4
#define K_L_GPIO_Port GPIOD
#define K_P_Pin GPIO_PIN_5
#define K_P_GPIO_Port GPIOD
#define K_R_Pin GPIO_PIN_6
#define K_R_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
