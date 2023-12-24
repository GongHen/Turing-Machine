/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "common_inc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void PID_execution_lock(void);
void PID_execution_unlock(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT_SENSOR_READY	( 1 << 0 )
#define SENSOR_COLLECTION_PERIOD    1000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

#define HMI_PACKET_LEN  8

uint8_t rxBuffer[HMI_PACKET_LEN];
uint8_t rxData;

typedef enum {
    PUFFER_MSG_HMI_RECV = 0,
    PUFFER_MSG_HMI_SEND,
}PUFFER_MSG_t;

// 定义邮件结构体
typedef struct
{
  PUFFER_MSG_t msg_type;
  uint8_t data[HMI_PACKET_LEN];
} MailData;

// 定义邮件队列控制块
osMailQDef(mailQueue, 4, MailData); // 邮件队列最大容量为4
osMailQId mailQueue;

osMutexDef(PIDExecution);
osMutexId gPIDExecutionMutexId_;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId OV7725ProcessHandle;
osThreadId PID_ThreadHandle;
osMessageQId mailQueueHandle;
osSemaphoreId presssHandle;



/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void OV7725ProcessTask(void const * argument);
void PIDThread(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */

void PID_execution_lock(void)
{
  osMutexWait(gPIDExecutionMutexId_, osWaitForever);

}

void PID_execution_unlock(void)
{
  osMutexRelease(gPIDExecutionMutexId_);
}

/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

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
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

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

  /* Create the semaphores(s) */
  /* definition and creation of presss */
  osSemaphoreDef(presss);
  presssHandle = osSemaphoreCreate(osSemaphore(presss), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of mailQueue */
  osMessageQDef(mailQueue, 16, uint16_t);
  mailQueueHandle = osMessageCreate(osMessageQ(mailQueue), NULL);

  // 初始化邮件队列
  mailQueue = osMailCreate(osMailQ(mailQueue), NULL);

  gPIDExecutionMutexId_ = osMutexCreate(osMutex(PIDExecution));

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityBelowNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of OV7725Process */
  osThreadDef(OV7725Process, OV7725ProcessTask, osPriorityNormal, 0, 128);
  OV7725ProcessHandle = osThreadCreate(osThread(OV7725Process), NULL);

  /* definition and creation of PID_Thread */
  osThreadDef(PID_Thread, PIDThread, osPriorityHigh, 0, 128);
  PID_ThreadHandle = osThreadCreate(osThread(PID_Thread), NULL);

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
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {

	  osThreadDef(OV7725Process, OV7725ProcessTask, osPriorityNormal, 0, 128);
	  OV7725ProcessHandle = osThreadCreate(osThread(OV7725Process), NULL);

	  vTaskDelete(defaultTaskHandle);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_OV7725ProcessTask */
/**
* @brief Function implementing the OV7725Process thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OV7725ProcessTask */
void OV7725ProcessTask(void const * argument)
{
  /* USER CODE BEGIN OV7725ProcessTask */

  UNUSED(argument);

  uint32_t PreviousWakeTime = 0;

  /* Infinite loop */
  for(;;)
  {
//	event = osSignalWait(BIT_SENSOR_READY, osWaitForever);
//	if(event.value.signals == (BIT_SENSOR_READY))
//	{
//	  printf("3333333333333333\r\n");
//	}

	/* completed packet from UART, Sensor */
//	event = osMailGet(mailQueue, osWaitForever);
//
//	if (event.status == osEventMail)
//	{
//
//	  // 从邮件中获取数据
//	  MailData *mail = (MailData *)event.value.p;
//
//	  switch (mail->msg_type)
//	  {
//	  case PUFFER_MSG_HMI_RECV:
//		printf("3333333333333333\r\n");
//
//		// 处理接收到的数据
//		// 在这里可以执行任意用户任务的操作
//		break;
//	  case PUFFER_MSG_HMI_SEND:
//		  printf("44444444444444\r\n");
//		break;
//	  default:
//		break;
//	  }
//
//	  // 释放邮件内存
//	  osMailFree(mailQueue, mail);
//	}
	  PreviousWakeTime = osKernelSysTick();

	  PID_execution_lock();
	  printf("11111111111111\r\n");
	  Main();
	  PID_execution_unlock();
	  osSignalSet( PID_ThreadHandle, BIT_SENSOR_READY );

	  osDelayUntil(&PreviousWakeTime, SENSOR_COLLECTION_PERIOD);
  }
  /* USER CODE END OV7725ProcessTask */
}


/* USER CODE BEGIN Header_PIDThread */
/**
* @brief Function implementing the PID_Thread thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PIDThread */
void PIDThread(void const * argument)
{
  /* USER CODE BEGIN PIDThread */
	 osEvent event;
  /* Infinite loop */
  for(;;)
  {
	  	event = osSignalWait(BIT_SENSOR_READY, osWaitForever);
	  	if(event.value.signals == (BIT_SENSOR_READY))
	  	{
	  	  PID_execution_lock();
	  	  printf("222222222222222222\r\n");
	  	  osDelay(500);
	  	  printf("3333333333333333\r\n");

	  	  PID_execution_unlock();
	  	}

//	    if (PreviousWakeTime % 3000 == 0) {
//			// 创建邮件
//			MailData *mail = (MailData *)osMailAlloc(mailQueue, osWaitForever);
//	//        memcpy(mail->data, rxBuffer, HMI_PACKET_LEN);
//			mail->msg_type = PUFFER_MSG_HMI_RECV;
//			// 发送邮件给用户任务
//			osMailPut(mailQueue, mail);
//	    }

//	    if (PreviousWakeTime % 4000 == 0) {
//			// 创建邮件
//			MailData *mail = (MailData *)osMailAlloc(mailQueue, osWaitForever);
//	//        memcpy(mail->data, rxBuffer, HMI_PACKET_LEN);
//			mail->msg_type = PUFFER_MSG_HMI_SEND;
//			// 发送邮件给用户任务
//			osMailPut(mailQueue, mail);
//	    }


  }
  /* USER CODE END PIDThread */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

