/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "lcd.h"
#include "dht11.h"
#include <string.h>
#include <stdio.h>
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
extern float g_adc_val;
extern uint8_t g_led_cmd;
extern uint8_t g_temp, g_humi;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Task_UART_Cmd */
osThreadId_t Task_UART_CmdHandle;
const osThreadAttr_t Task_UART_Cmd_attributes = {
  .name = "Task_UART_Cmd",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 256 * 4
};
/* Definitions for Task_Sensor_Sam */
osThreadId_t Task_Sensor_SamHandle;
const osThreadAttr_t Task_Sensor_Sam_attributes = {
  .name = "Task_Sensor_Sam",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 256 * 4
};
/* Definitions for Task_LED_Ctrl */
osThreadId_t Task_LED_CtrlHandle;
const osThreadAttr_t Task_LED_Ctrl_attributes = {
  .name = "Task_LED_Ctrl",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Task_LCD_Refres */
osThreadId_t Task_LCD_RefresHandle;
const osThreadAttr_t Task_LCD_Refres_attributes = {
  .name = "Task_LCD_Refres",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};
/* Definitions for Queue_LED_Cmd */
osMessageQueueId_t Queue_LED_CmdHandle;
const osMessageQueueAttr_t Queue_LED_Cmd_attributes = {
  .name = "Queue_LED_Cmd"
};
/* Definitions for Queue_UART_RX */
osMessageQueueId_t Queue_UART_RXHandle;
const osMessageQueueAttr_t Queue_UART_RX_attributes = {
  .name = "Queue_UART_RX"
};
/* Definitions for mutex_lcd */
osMutexId_t mutex_lcdHandle;
const osMutexAttr_t mutex_lcd_attributes = {
  .name = "mutex_lcd"
};
/* Definitions for mutex_dht */
osMutexId_t mutex_dhtHandle;
const osMutexAttr_t mutex_dht_attributes = {
  .name = "mutex_dht"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask_UART_Cmd(void *argument);
void StartTask_Sensor_Sample(void *argument);
void StartTask_LED_Ctrl(void *argument);
void StartTask_LCD_Refresh(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of mutex_lcd */
  mutex_lcdHandle = osMutexNew(&mutex_lcd_attributes);

  /* creation of mutex_dht */
  mutex_dhtHandle = osMutexNew(&mutex_dht_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue_LED_Cmd */
  Queue_LED_CmdHandle = osMessageQueueNew (8, 1, &Queue_LED_Cmd_attributes);

  /* creation of Queue_UART_RX */
  Queue_UART_RXHandle = osMessageQueueNew (16, 32, &Queue_UART_RX_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Task_UART_Cmd */
  Task_UART_CmdHandle = osThreadNew(StartTask_UART_Cmd, NULL, &Task_UART_Cmd_attributes);

  /* creation of Task_Sensor_Sam */
  Task_Sensor_SamHandle = osThreadNew(StartTask_Sensor_Sample, NULL, &Task_Sensor_Sam_attributes);

  /* creation of Task_LED_Ctrl */
  Task_LED_CtrlHandle = osThreadNew(StartTask_LED_Ctrl, NULL, &Task_LED_Ctrl_attributes);

  /* creation of Task_LCD_Refres */
  Task_LCD_RefresHandle = osThreadNew(StartTask_LCD_Refresh, NULL, &Task_LCD_Refres_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_UART_Cmd */
/**
* @brief Function implementing the Task_UART_Cmd thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_UART_Cmd */
void StartTask_UART_Cmd(void *argument)
{
    /* USER CODE BEGIN StartTask_UART_Cmd */
  char rx_buf[32];
  for(;;)
  {
    if (osMessageQueueGet(Queue_UART_RXHandle, rx_buf, NULL, osWaitForever) == osOK)
    {
      if (strncmp(rx_buf, "LED_ON", 6) == 0)
      {
        g_led_cmd = 1;
        osMessageQueuePut(Queue_LED_CmdHandle, &g_led_cmd, 0, 0);
      }
      else if (strncmp(rx_buf, "LED_OFF", 7) == 0)
      {
        g_led_cmd = 0;
        osMessageQueuePut(Queue_LED_CmdHandle, &g_led_cmd, 0, 0);
      }
      else if (strncmp(rx_buf, "data", 4) == 0)
      {
        printf("[DATA] POT: %d.%02d V, Temp: %d C, Humi: %d%%\r\n",
               (int)g_adc_val, (int)((g_adc_val - (int)g_adc_val) * 100 + 0.5f),
               g_temp, g_humi);
      }
      else
      {
        printf("[WARN] Unknown: %s\r\n", rx_buf);
      }
    }
  }
  /* USER CODE END StartTask_UART_Cmd */
}

/* USER CODE BEGIN Header_StartTask_Sensor_Sample */
/**
* @brief Function implementing the Task_Sensor_Sam thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Sensor_Sample */
void StartTask_Sensor_Sample(void *argument)
{
    /* USER CODE BEGIN StartTask_Sensor_Sample */
  for(;;)
  {
    g_adc_val = Get_ADC_POT();
    osMutexAcquire(mutex_dhtHandle, osWaitForever);
    if (DHT11_ReadData(&g_temp, &g_humi) != HAL_OK) { g_temp = 0xFF; g_humi = 0xFF; }
    osMutexRelease(mutex_dhtHandle);
    int vw = (int)g_adc_val;
    int vf = (int)((g_adc_val - vw) * 100 + 0.5f);
    if (vf < 0) vf = -vf;
    printf("POT: %d.%02d V | T:%d H:%d\r\n", vw, vf, g_temp, g_humi);
    osDelay(500);
  }
  /* USER CODE END StartTask_Sensor_Sample */
}

/* USER CODE BEGIN Header_StartTask_LED_Ctrl */
/**
* @brief Function implementing the Task_LED_Ctrl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_LED_Ctrl */
void StartTask_LED_Ctrl(void *argument)
{
    /* USER CODE BEGIN StartTask_LED_Ctrl */
  uint8_t cmd;
  for(;;)
  {
    if (osMessageQueueGet(Queue_LED_CmdHandle, &cmd, NULL, osWaitForever) == osOK)
    {
      osMutexAcquire(mutex_lcdHandle, osWaitForever);
      if (cmd == 1) { LED_On(7); printf("[OK] LED8 ON\r\n"); }
      else          { LED_Off(7); printf("[OK] LED8 OFF\r\n"); }
      osMutexRelease(mutex_lcdHandle);
    }
  }
  /* USER CODE END StartTask_LED_Ctrl */
}

/* USER CODE BEGIN Header_StartTask_LCD_Refresh */
/**
* @brief Function implementing the Task_LCD_Refres thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_LCD_Refresh */
void StartTask_LCD_Refresh(void *argument)
{
    /* USER CODE BEGIN StartTask_LCD_Refresh */
  char lcd_buf[48];
  for(;;)
  {
    osMutexAcquire(mutex_lcdHandle, osWaitForever);
    int vw = (int)g_adc_val;
    int vf = (int)((g_adc_val - vw) * 100 + 0.5f);
    if (vf < 0) vf = -vf;
    sprintf(lcd_buf, "POT: %d.%02dV T:%d H:%d%%", vw, vf, g_temp, g_humi);
    LCD_ClearLine(Line1);
    LCD_DisplayStringLine(Line1, (u8*)lcd_buf);
    osMutexRelease(mutex_lcdHandle);
    osDelay(200);
  }
  /* USER CODE END StartTask_LCD_Refresh */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

