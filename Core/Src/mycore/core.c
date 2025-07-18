/*
 * core.c
 *
 *  Created on: Jul 18, 2025
 *      Author: qinyuan
 */

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"	//消息队列

/* 系统头文件 */
#include "main.h"
#include "gpio.h"
#include "usart.h"

/* BSP头文件 */
#include "bsp_led.h"
#include "bsp_key.h"
#include "miscdev.h"

/* 私有函数声明 */
static void AppTaskCreate(void);
static void LED_Task(void* pvParameters);
static void KEY_Task(void* pvParameters);
static void RTOS_BSP_Init(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);

/**************************** 任务句柄 ********************************/
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;
/* KEY任务句柄 */
static TaskHandle_t KEY_Task_Handle = NULL;
/* 消息队列接收任务句柄 */
static TaskHandle_t Receive_Task_Handle = NULL;
/* 消息队列发送任务句柄 */
static TaskHandle_t Send_Task_Handle = NULL;

/********************************** 内核对象句柄 *********************************
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 *******************************************************************************/
QueueHandle_t Test_Queue =NULL;		//队列句柄

/******************************* 全局变量声明 ************************************/


/******************************* 宏定义 *****************************************/
/* 队列的长度，最大可包含多少个消息 */
#define  QUEUE_LEN    4
/* 队列中每个消息大小（字节） */
#define  QUEUE_SIZE   4

/******************************* 静态内存分配 ************************************/
/* AppTaskCreate任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];
/* LED任务堆栈 */
static StackType_t LED_Task_Stack[128];
/* KEY任务堆栈 */
static StackType_t KEY_Task_Stack[128];

/* AppTaskCreate 任务控制块 */
static StaticTask_t AppTaskCreate_TCB;
/* LED任务控制块 */
static StaticTask_t LED_Task_TCB;
/* KEY任务控制块 */
static StaticTask_t KEY_Task_TCB;

/* 空闲任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;

/**
 * @brief  FreeRTOS应用初始化
 * @param  无
 * @retval 无
 * @note   创建应用任务并启动调度器
 */
void FreeRTOS_App_Init(void)
{
    BaseType_t xReturn = pdPASS;	/* 定义一个创建消息队列返回值，默认为pdPASS */

    /* 板级外设初始化 */
    RTOS_BSP_Init();

    printf("This is a test about message queue:\r\n");
    printf("1.press KEY1 or KEY2 to send queue msg\n");
    printf("2.Receive task will show msg in uart if received msg\n\n");

#if 0 // 静态创建任务，用于任务管理实验
    /* 创建 AppTaskCreate 任务 */
    AppTaskCreate_Handle = xTaskCreateStatic(
        (TaskFunction_t)AppTaskCreate,      // 任务函数
        (const char*)"AppTaskCreate",       // 任务名称
        (uint32_t)128,                      // 任务堆栈大小
        (void*)NULL,                        // 传递给任务函数的参数
        (UBaseType_t)3,                     // 任务优先级
        (StackType_t*)AppTaskCreate_Stack,  // 任务堆栈
        (StaticTask_t*)&AppTaskCreate_TCB   // 任务控制块
    );

    if(NULL != AppTaskCreate_Handle) {
        printf("AppTaskCreate successful\r\n");
#if 0 // 会与 main() 中的oskelnelStart()函数冲突
        vTaskStartScheduler();	/* 启动任务，开启调度 */
#endif
    } else {
        printf("AppTaskCreate failure\r\n");
    }
#endif

#if 1 // 动态创建任务，用于消息队列实验
       /* 创建AppTaskCreate任务 */
      xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                            (const char*    )"AppTaskCreate",/* 任务名字 */
                            (uint16_t       )512,  /* 任务栈大小 */
                            (void*          )NULL,/* 任务入口函数参数 */
                            (UBaseType_t    )1, /* 任务的优先级 */
                            (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */
      /* 检查创建任务是否成功 */
      if(pdPASS == xReturn)
      {
          printf("AppTaskCreate successful\r\n");
//        vTaskStartScheduler();   /* 启动任务，开启调度 */
      }
      else
      {
    	  printf("AppTaskCreate failure\r\n");
      }
#endif
}

/**
 * @brief  应用任务创建函数
 * @param  无
 * @retval 无
 * @note   为了方便管理，所有的任务创建函数都放在这个函数里面
 */
static void AppTaskCreate(void)
{
#if 0 // 任务管理实验
    taskENTER_CRITICAL();  // 进入临界区

    /* 创建LED_Task任务 */
    LED_Task_Handle = xTaskCreateStatic(
        (TaskFunction_t)LED_Task,           // 任务函数
        (const char*)"LED_Task",            // 任务名称
        (uint32_t)128,                      // 任务堆栈大小
        (void*)NULL,                        // 传递给任务函数的参数
        (UBaseType_t)4,                     // 任务优先级
        (StackType_t*)LED_Task_Stack,       // 任务堆栈
        (StaticTask_t*)&LED_Task_TCB        // 任务控制块
    );

    if(NULL != LED_Task_Handle) {
        printf("LED_Task create successful\n");
    } else {
        printf("LED_Task create failure\n");
    }

    /* 创建KEY_Task任务 */
    KEY_Task_Handle = xTaskCreateStatic(
        (TaskFunction_t)KEY_Task,           // 任务函数
        (const char*)"KEY_Task",            // 任务名称
        (uint32_t)128,                      // 任务堆栈大小
        (void*)NULL,                        // 传递给任务函数的参数
        (UBaseType_t)3,                     // 任务优先级
        (StackType_t*)KEY_Task_Stack,       // 任务堆栈
        (StaticTask_t*)&KEY_Task_TCB        // 任务控制块
    );

    if(NULL != KEY_Task_Handle) {
        printf("KEY_Task create successful\r\n");
    } else {
        printf("KEY_Task create failure\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle);  // 删除AppTaskCreate任务

    taskEXIT_CRITICAL();  // 退出临界区
#endif
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL();           // 进入临界区

    /* 创建Test_Queue */
    Test_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,		/* 消息队列的长度 */
                              (UBaseType_t ) QUEUE_SIZE);	/* 消息的大小 */
    if(NULL != Test_Queue)
      printf("create Test_Queue successful\r\n");

    /* 创建Receive_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )Receive_Task, 	/* 任务入口函数 */
                          (const char*    )"Receive_Task",	/* 任务名字 */
                          (uint16_t       )512,   			/* 任务栈大小 */
                          (void*          )NULL,			/* 任务入口函数参数 */
                          (UBaseType_t    )2,	    		/* 任务的优先级 */
                          (TaskHandle_t*  )&Receive_Task_Handle);	/* 任务控制块指针 */
    if(pdPASS == xReturn)
      printf("create Receive_Task successful\r\n");

    /* 创建Send_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )Send_Task,  /* 任务入口函数 */
                          (const char*    )"Send_Task",/* 任务名字 */
                          (uint16_t       )512,  /* 任务栈大小 */
                          (void*          )NULL,/* 任务入口函数参数 */
                          (UBaseType_t    )3, /* 任务的优先级 */
                          (TaskHandle_t*  )&Send_Task_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
      printf("create Send_Task successful\n\n");

    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务

    taskEXIT_CRITICAL();            //退出临界区
}

/**
 * @brief  LED任务函数
 * @param  parameter: 任务参数
 * @retval 无
 */
static void LED_Task(void* parameter)
{
    while (1) {
        LED1_ON;
        vTaskDelay(500);   /* 延时500个tick */
        printf("LED_Task Running, LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(500);   /* 延时500个tick */
        printf("LED_Task Running, LED1_OFF\r\n");
    }
}

/**
 * @brief  KEY任务函数
 * @param  parameter: 任务参数
 * @retval 无
 */
static void KEY_Task(void* parameter)
{
    while (1) {
        if( Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON ) {
            /* K1 被按下 */
            printf("suspend LED task\n");
            vTaskSuspend(LED_Task_Handle);/* 挂起LED任务 */
            printf("suspend LED task successful\n");
        }
        if( Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON ) {
            /* K2 被按下 */
            printf("resume LED task\n");
            vTaskResume(LED_Task_Handle);/* 恢复LED任务！ */
            printf("resume LED task successful\n");
        }
        vTaskDelay(20);/* 延时20个tick */
    }
}

/**********************************************************************
  * @ 函数名  ： Receive_Task
  * @ 功能说明： 读消息队列：Receive_Task任务主体
  * @ 参数    ：
  * @ 返回值  ： 无
  ********************************************************************/
static void Receive_Task(void* parameter)
{
  BaseType_t 	xReturn = pdTRUE;	/* 定义一个创建信息返回值，默认为pdTRUE */
  uint32_t 		r_queue;			/* 定义一个接收消息的变量 */

  while (1)
  {
    xReturn = xQueueReceive( Test_Queue,    /* 消息队列的句柄 */
                             &r_queue,      /* 发送的消息内容 */
                             portMAX_DELAY); /* 等待时间 一直等 */
    if(pdTRUE == xReturn)
      printf("Receive data: %ld\n\n", r_queue);
    else
      printf("Receive error: 0x%lx\n", xReturn);
  }
}

/**********************************************************************
  * @ 函数名  ： Send_Task
  * @ 功能说明： 写消息队列：Send_Task任务主体
  * @ 参数    ：
  * @ 返回值  ： 无
  ********************************************************************/
static void Send_Task(void* parameter)
{
  BaseType_t 	xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  uint32_t 		send_data1 = 1;
  uint32_t 		send_data2 = 2;

  while (1)
  {
	/* K1 被按下 */
    if( Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON )
    {
      printf("Send_data1...\n");
      xReturn = xQueueSend( Test_Queue, /* 消息队列的句柄 */
                            &send_data1,/* 发送的消息内容 */
                            0 );        /* 等待时间 0 */
      if(pdPASS == xReturn)
        printf("Send_data1 successfully!\n\n");
    }

    /* K2 被按下 */
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
    {
      printf("Send_data2...\n");
      xReturn = xQueueSend( Test_Queue, /* 消息队列的句柄 */
                            &send_data2,/* 发送的消息内容 */
                            0 );        /* 等待时间 0 */
      if(pdPASS == xReturn)
        printf("Send_data2 successfully!\n\n");
    }

    vTaskDelay(20);/* 延时20个tick */
  }
}

/**
 * @brief  FreeRTOS相关BSP初始化
 * @param  无
 * @retval 无
 * @note   只处理FreeRTOS特定的初始化，硬件初始化由main.c完成
 */
static void RTOS_BSP_Init(void)
{
    /*
     * 配置中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
     * FreeRTOS推荐使用NVIC_PRIORITYGROUP_4
     * 注意：这个设置会影响整个系统的中断优先级，需要在任务创建前设置
     */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* 简单的LED功能测试（GPIO已经在main.c中初始化） */
    LED1_ON;
    HAL_Delay(100);  // 短暂点亮LED测试硬件是否正常
    LED1_OFF;

    printf("FreeRTOS BSP Init successful\r\n");
}

/**
 * @brief  获取空闲任务的任务堆栈和任务控制块内存
 * @param  ppxIdleTaskTCBBuffer: 任务控制块内存
 * @param  ppxIdleTaskStackBuffer: 任务堆栈内存
 * @param  pulIdleTaskStackSize: 任务堆栈大小
 * @retval 无
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @brief  获取定时器任务的任务堆栈和任务控制块内存
 * @param  ppxTimerTaskTCBBuffer: 任务控制块内存
 * @param  ppxTimerTaskStackBuffer: 任务堆栈内存
 * @param  pulTimerTaskStackSize: 任务堆栈大小
 * @retval 无
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB;
    *ppxTimerTaskStackBuffer = Timer_Task_Stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/********************************END OF FILE****************************/
