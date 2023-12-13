/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>

#include "usart.h"
#include "nr_micro_shell.h"
#include "wifi.h"
#include "spi.h"
#include "gpio.h"

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
TaskHandle_t uart1_task = NULL;
uint8_t uart1_rx = 0;
uint8_t uart2_rx = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void uart1_task_entry(void *arg);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  if(xTaskCreate(uart1_task_entry, 
  "uat1", 8*1024, NULL, 
  (osPriority_t) osPriorityNormal, &uart1_task) != 0)
  {

  }
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
  
  for (;;)
  {
    
    osDelay(200);
     break;

  }
  osThreadExit();
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
uint8_t send_buf[100];
uint8_t recv_buf[100];
void dump_hex(void *buf, uint32_t size, uint32_t number);

static void uart1_task_entry(void *arg)
{
  wifi_protocol_init();
  HAL_UART_Receive_IT(&huart1, &uart1_rx, 1);
  HAL_UART_Receive_IT(&huart2, &uart2_rx, 1);
  int ret = 0;
  int slave_data_len = 0;
  unsigned int cnt= 0;
  while (1)
  {

    wifi_uart_service();
    for (size_t i = 0; i < 100; i++)
    {
      send_buf[i] = i+cnt;
      
    }
    cnt++;

    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
    {
      osDelay(5);
    }
    send_buf[0] = 0x51;
    send_buf[1] = 0x00;
    HAL_GPIO_WritePin(SPI2CS_GPIO_Port, SPI2CS_Pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(&hspi2, (uint8_t *)send_buf, 10, 0xFFFF);
    HAL_GPIO_WritePin(SPI2CS_GPIO_Port, SPI2CS_Pin, GPIO_PIN_SET);
    printf("send_buf[%d]:\r\n", ret);
    dump_hex(send_buf+2, 8, 8);

    // do
    // {
    //   while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
    //   {
    //     osDelay(5);
    //   }
    //   send_buf[0] = 0xaa;
    //   send_buf[1] = 0x00;
    //   HAL_GPIO_WritePin(SPI2CS_GPIO_Port, SPI2CS_Pin, GPIO_PIN_RESET);
    //   ret = HAL_SPI_Transmit(&hspi2, (uint8_t *)send_buf, 2, 0xFFFF);

    //   HAL_SPI_Receive(&hspi2, (uint8_t *)&slave_data_len, sizeof(slave_data_len), 0xFFFF);
    //   HAL_GPIO_WritePin(SPI2CS_GPIO_Port, SPI2CS_Pin, GPIO_PIN_SET);
    //   printf("recv_buf[%d]:\r\n", ret);
    //   dump_hex(&slave_data_len, sizeof(slave_data_len), 8);
    //   // 转换字节序 32 位
    //   slave_data_len = ((slave_data_len & 0x000000FF) << 24) | ((slave_data_len & 0x0000FF00) << 8) | ((slave_data_len & 0x00FF0000) >> 8) | ((slave_data_len & 0xFF000000) >> 24);
    //   printf("slave_data_len[%d]\r\n", slave_data_len);
    // } while (slave_data_len <= 0);

    slave_data_len = 8;
    if (slave_data_len > 0)
    {
      while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
      {
        osDelay(5);
      }
      send_buf[0] = 0x0b;
      send_buf[1] = 0x00;
      HAL_GPIO_WritePin(SPI2CS_GPIO_Port, SPI2CS_Pin, GPIO_PIN_RESET);
      ret = HAL_SPI_Transmit(&hspi2, (uint8_t *)send_buf, 2, 0xFFFF);
      HAL_SPI_Receive(&hspi2, (uint8_t *)recv_buf, slave_data_len, 0xFFFF);
      HAL_GPIO_WritePin(SPI2CS_GPIO_Port, SPI2CS_Pin, GPIO_PIN_SET);
      printf("recv_buf[%d]:\r\n", ret);
      dump_hex(recv_buf, slave_data_len, 8);
    }

    osDelay(1000);
  }
  uart1_task = NULL;
  vTaskDelete(NULL);
}

int _write(int fd, char *ptr, int len)  
{  
  HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 0xFFFF);
  return len;
}

int _write2(int fd, char *ptr, int len)  
{  
  HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 0xFFFF);
  return len;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART1)
  {

    shell(uart1_rx);
    HAL_UART_Receive_IT(&huart1, &uart1_rx, 1);
  }
  else if (huart->Instance == USART2)
  {
    uart_receive_input(uart2_rx);
    HAL_UART_Receive_IT(&huart2, &uart2_rx, 1);
  }
}

/* USER CODE END Application */

