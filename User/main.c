
#include "stm32f10x.h"
#include "misc.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
//#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "delay.h"
#include "stm32f10x_tim.h"

#define START_TASK_PRIO		1		     //任务优先级
#define START_STK_SIZE 		256				//任务堆栈大小
TaskHandle_t StartTask_Handler;			//任务句柄
void start_task(void *pvParameters);//任务函数


#define ECHO_TASK_PRIO 2   //任务优先级
#define ECHO_STK_SIZE 256	//任务堆栈
TaskHandle_t echoTask_Handler;       //任务句柄
void echo_task(void *pv);    //任务函数

#define TASK1_TASK_PRIO  3  //任务优先级
#define TASK1_STK_SIZE   128
TaskHandle_t Task1Task_Handler;
void task1_task(void *pv);

#define TASK2_TASK_PRIO  4  //任务优先级
#define TASK2_STK_SIZE   128
TaskHandle_t Task2Task_Handler;
void task2_task(void *pv);

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //中断优先级分组 分组4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	xTaskCreate((TaskFunction_t )start_task,    //任务函数
							 (const char*   )"start_task",  //任务名称
							 (uint16_t      )START_STK_SIZE, //任务堆栈大小
							 (void*         )NULL, 						//传达给任务函数的参数
							 (UBaseType_t   )START_TASK_PRIO, //任务优先级
							 (TaskHandle_t* )&StartTask_Handler); //任务句柄
	
    vTaskStartScheduler();        
    return 0;
	
}

void start_task(void *pvParameters)
//开始任务任务函数
{	
	taskENTER_CRITICAL();           //进入临界区
	
	xTaskCreate((TaskFunction_t   )echo_task,
							(const char*      )"echo_task",
							(uint16_t         )ECHO_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )ECHO_TASK_PRIO,
							(TaskHandle_t*    )&echoTask_Handler);
							
	xTaskCreate((TaskFunction_t   )task1_task,
							(const char*      )"task1_task",
							(uint16_t         )TASK1_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )TASK1_TASK_PRIO,
							(TaskHandle_t*    )&Task1Task_Handler);
							
	xTaskCreate((TaskFunction_t   )task2_task,
							(const char*      )"task2_task",
							(uint16_t         )TASK2_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )TASK2_TASK_PRIO,
							(TaskHandle_t*    )&Task2Task_Handler);
							
	vTaskDelete(StartTask_Handler);   //删除开始任务
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //任务函数
{
	char c1;
	while(1)
	{
		//printf("%s", scanf(""));
		c1 = fgetc(stdin);
		//if(c1 == 'j') printf("k\r\n");
		switch(c1)
		{
			case 'j':
				vTaskSuspend(Task1Task_Handler); //挂起任务1
				printf("挂起task1\r\n");
			break;
			case 'k':
				vTaskResume(Task1Task_Handler);//恢复任务1
				printf("恢复task1\r\n");
			break;
			case 'h':
				vTaskSuspend(Task2Task_Handler); //挂起任务2
				printf("挂起task2\r\n");
			break;
			case 'i':
				vTaskResume(Task2Task_Handler);//恢复任务2
				printf("恢复task2\r\n");
			break;
		}
	}
}

void task1_task(void *pv)    //任务函数
{
	while(1)
	{
		vTaskDelay(1000);
		printf("task1\r\n");
	}
}

void task2_task(void *pv)    //任务函数
{
	while(1)
	{
		vTaskDelay(1000);
		printf("task2\r\n");
	}
}





