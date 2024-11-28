/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId RoboHandle;
osThreadId INSHandle;
osThreadId GimbalHandle;
osThreadId ChassisHandle;
osThreadId CommunicateHandle;
osThreadId Key_LEDHandle;
osThreadId BuzzerHandle;
osThreadId UIHandle;
osThreadId ShootHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Robo_Task(void const * argument);
void INS_Task(void const * argument);
void Gimbal_Task(void const * argument);
void Chassis_Task(void const * argument);
void Communicate_Task(void const * argument);
void Key_LED_Task(void const * argument);
void Buzzer_Task(void const * argument);
void UI_Task(void const * argument);
void Shoot_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Robo */
  osThreadDef(Robo, Robo_Task, osPriorityIdle, 0, 512);
  RoboHandle = osThreadCreate(osThread(Robo), NULL);

  /* definition and creation of INS */
  osThreadDef(INS, INS_Task, osPriorityIdle, 0, 1024);
  INSHandle = osThreadCreate(osThread(INS), NULL);

  /* definition and creation of Gimbal */
  osThreadDef(Gimbal, Gimbal_Task, osPriorityIdle, 0, 512);
  GimbalHandle = osThreadCreate(osThread(Gimbal), NULL);

  /* definition and creation of Chassis */
  osThreadDef(Chassis, Chassis_Task, osPriorityIdle, 0, 512);
  ChassisHandle = osThreadCreate(osThread(Chassis), NULL);

  /* definition and creation of Communicate */
  osThreadDef(Communicate, Communicate_Task, osPriorityIdle, 0, 512);
  CommunicateHandle = osThreadCreate(osThread(Communicate), NULL);

  /* definition and creation of Key_LED */
  osThreadDef(Key_LED, Key_LED_Task, osPriorityIdle, 0, 256);
  Key_LEDHandle = osThreadCreate(osThread(Key_LED), NULL);

  /* definition and creation of Buzzer */
  osThreadDef(Buzzer, Buzzer_Task, osPriorityIdle, 0, 256);
  BuzzerHandle = osThreadCreate(osThread(Buzzer), NULL);

  /* definition and creation of UI */
  osThreadDef(UI, UI_Task, osPriorityIdle, 0, 256);
  UIHandle = osThreadCreate(osThread(UI), NULL);

  /* definition and creation of Shoot */
  osThreadDef(Shoot, Shoot_Task, osPriorityIdle, 0, 512);
  ShootHandle = osThreadCreate(osThread(Shoot), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_Device */
  MX_USB_Device_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Robo_Task */
/**
* @brief Function implementing the Robo thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Robo_Task */
void Robo_Task(void const * argument)
{
  /* USER CODE BEGIN Robo_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Robo_Task */
}

/* USER CODE BEGIN Header_INS_Task */
/**
* @brief Function implementing the INS thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_INS_Task */
void INS_Task(void const * argument)
{
  /* USER CODE BEGIN INS_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END INS_Task */
}

/* USER CODE BEGIN Header_Gimbal_Task */
/**
* @brief Function implementing the Gimbal thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Gimbal_Task */
void Gimbal_Task(void const * argument)
{
  /* USER CODE BEGIN Gimbal_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Gimbal_Task */
}

/* USER CODE BEGIN Header_Chassis_Task */
/**
* @brief Function implementing the Chassis thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Chassis_Task */
void Chassis_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}

/* USER CODE BEGIN Header_Communicate_Task */
/**
* @brief Function implementing the Communicate thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Communicate_Task */
void Communicate_Task(void const * argument)
{
  /* USER CODE BEGIN Communicate_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Communicate_Task */
}

/* USER CODE BEGIN Header_Key_LED_Task */
/**
* @brief Function implementing the Key_LED thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Key_LED_Task */
void Key_LED_Task(void const * argument)
{
  /* USER CODE BEGIN Key_LED_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Key_LED_Task */
}

/* USER CODE BEGIN Header_Buzzer_Task */
/**
* @brief Function implementing the Buzzer thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Buzzer_Task */
void Buzzer_Task(void const * argument)
{
  /* USER CODE BEGIN Buzzer_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Buzzer_Task */
}

/* USER CODE BEGIN Header_UI_Task */
/**
* @brief Function implementing the UI thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_UI_Task */
void UI_Task(void const * argument)
{
  /* USER CODE BEGIN UI_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END UI_Task */
}

/* USER CODE BEGIN Header_Shoot_Task */
/**
* @brief Function implementing the Shoot thread.
* @param argument: Not used
* @retval None
*/__weak
/* USER CODE END Header_Shoot_Task */
void Shoot_Task(void const * argument)
{
  /* USER CODE BEGIN Shoot_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Shoot_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

