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
osThreadId CalibrateTaskHandle;
osThreadId ChassisTaskHandle;
osThreadId gimbalTaskHandle;
osThreadId imuTaskHandle;
osThreadId detectTaskHandle;
osThreadId ledTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId usb_task_handle;
osThreadId decode_task_handle;
osThreadId cap_task_handle;
osThreadId UI_task_handle;
osThreadId servo_task_handle;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void calibrate_task(void const * argument);
void chassis_task(void const * argument);
void gimbal_task(void const * argument);
void INS_task(void const * argument);
void detect_task(void const * argument);
void led_task(void const * argument);
extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void UI_paint_task(void const*argument);
void cap_task(void const *pvParameters);
void servo_task(void const *pvParameters);
/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
QueueHandle_t CDC_send_queue;

_Noreturn extern void usb_task(void const * argument);
extern void decode_task(void const * arg);
/* USER CODE END GET_IDLE_TASK_MEMORY */

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


  /* definition and creation of CalibrateTask */
  osThreadDef(CalibrateTask, calibrate_task, osPriorityNormal, 0, 512);
  CalibrateTaskHandle = osThreadCreate(osThread(CalibrateTask), NULL);

  /* definition and creation of ChassisTask */
  osThreadDef(ChassisTask, chassis_task, osPriorityLow, 0, 512);
  ChassisTaskHandle = osThreadCreate(osThread(ChassisTask), NULL);

  /* definition and creation of gimbalTask */
  osThreadDef(gimbalTask, gimbal_task, osPriorityHigh, 0, 512);
  gimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);

  /* definition and creation of imuTask */
  osThreadDef(imuTask, INS_task, osPriorityIdle, 0, 512);
  imuTaskHandle = osThreadCreate(osThread(imuTask), NULL);

  /* definition and creation of detectTask */
  osThreadDef(detectTask, detect_task, osPriorityIdle, 0, 128);
  detectTaskHandle = osThreadCreate(osThread(detectTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    osThreadDef(USBtask,usb_task,osPriorityHigh,0,128);
    usb_task_handle= osThreadCreate(osThread(USBtask),NULL);

    osThreadDef(DecodeTask, decode_task, osPriorityHigh, 0, 128);
    decode_task_handle = osThreadCreate(osThread(DecodeTask), NULL);
    CDC_send_queue = xQueueCreate(1, 128);


    osThreadDef(CapTask,cap_task,osPriorityAboveNormal,0,256);
    cap_task_handle=osThreadCreate(osThread(CapTask),NULL);

    osThreadDef(UIPaintTask, UI_paint_task,osPriorityNormal,0,256);
    UI_task_handle= osThreadCreate(osThread(UIPaintTask),NULL);

    /* definition and creation of ledTask */
    osThreadDef(ledTask, led_task, osPriorityLow, 0, 128);
    ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);
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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_calibrate_task */
/**
* @brief Function implementing the CalibrateTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_calibrate_task */
__weak void calibrate_task(void const * argument)
{
  /* USER CODE BEGIN calibrate_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END calibrate_task */
}

/* USER CODE BEGIN Header_chassis_task */
/**
* @brief Function implementing the ChassisTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_chassis_task */
__weak void chassis_task(void const * argument)
{
  /* USER CODE BEGIN chassis_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END chassis_task */
}

/* USER CODE BEGIN Header_gimbal_task */
/**
* @brief Function implementing the gimbalTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_gimbal_task */
__weak void gimbal_task(void const * argument)
{
  /* USER CODE BEGIN gimbal_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END gimbal_task */
}

/* USER CODE BEGIN Header_INS_task */
/**
* @brief Function implementing the imuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_INS_task */
__weak void INS_task(void const * argument)
{
  /* USER CODE BEGIN INS_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END INS_task */
}

/* USER CODE BEGIN Header_detect_task */
/**
* @brief Function implementing the detectTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_detect_task */
__weak void detect_task(void const * argument)
{
  /* USER CODE BEGIN detect_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END detect_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
