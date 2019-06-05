
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



TaskHandle_t PrintTask_Handler;
void print_task(void *pvParameters)
{
	while (1)
	{
		printf("1Hello STM32!\n");
		vTaskDelay(5000);		
	}
}

TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters)
{
	while(1)
	{
		vTaskDelay(5000);
//		LED0 = ~LED0;
		printf("2Hello STM32b!\n");
	}
}

TaskHandle_t LEDTask_Handler;
void led_task( void * pvParameters )
{
    for( ;; )
    {
        //led_task();
				printf("led\n");
        vTaskDelay(1000);
    }
}

TaskHandle_t KeyTask_Handler;
void key_task( void * pvParameters )
{
    for( ;; )
    {
        //led_task();
				printf("key\n");
        vTaskDelay(1000);
    }
}

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	USART1_init(9600);
    delay_init();
    led_init();
	//printf("Hello world!\n");
    
    // ?????? xTaskCreate(????, ???, ???????, ??????????, ?????, ????);
    //xTaskCreate( start_task, "start_stask", start_task_zise, NULL, start_task_prio, &start_task_handler);    
		//key_init();
	
	xTaskCreate((TaskFunction_t )print_task,     //任务函数
							 (const char*   )"print_task",    //任务名称
							 (uint16_t      )256,                  //START_STK_SIZE, //任务堆栈大小
							 (void *        )NULL,           //传递给任务函数的参数
							 (UBaseType_t   )1,            //START_TASK_PRIO,//任务优先级
							 (TaskHandle_t* )&PrintTask_Handler); //任务句柄
	
	xTaskCreate((TaskFunction_t )start_task,     //任务函数
							 (const char*   )"start_task",    //任务名称
							 (uint16_t      )128,                  //START_STK_SIZE, //任务堆栈大小
							 (void *        )NULL,           //传递给任务函数的参数
							 (UBaseType_t   )1,            //START_TASK_PRIO,//任务优先级
							 (TaskHandle_t* )&StartTask_Handler); //任务句柄
							 
							 
	xTaskCreate((TaskFunction_t)led_task, (const char*)"led_task", (uint16_t)128, 
							(void*)NULL, (UBaseType_t)1, (TaskHandle_t*)&LEDTask_Handler);		
	
	xTaskCreate((TaskFunction_t)key_task, (const char*)"key_task", (uint16_t)128, 
							(void*)NULL, (UBaseType_t)1, (TaskHandle_t*)NULL);
							 
   // printf("Hello world!2\n");
    vTaskStartScheduler();        // ??????
		//printf("Hello world!3\n");
    return 0;
	
}

/*
void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();    // ?????
    // ??led1??
    xTaskCreate(led1_task, "led1_task", start_task_zise, NULL, led1_task_prio, &led1_task_handler);    
		printf("Hello world!2\n");
    vTaskDelete(start_task_handler); //??????
    taskEXIT_CRITICAL();    // ?????
}
*/
//LED1???? 
void led1_task( void * pvParameters )
{
    for( ;; )
    {
        //led_task();
				printf("Hello world!3\n");
        vTaskDelay(1000);
    }
}
