
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

#define START_TASK_PRIO		1		     //�������ȼ�
#define START_STK_SIZE 		256				//�����ջ��С
TaskHandle_t StartTask_Handler;			//������
void start_task(void *pvParameters);//������


#define ECHO_TASK_PRIO 2   //�������ȼ�
#define ECHO_STK_SIZE 256	//�����ջ
TaskHandle_t echoTask_Handler;       //������
void echo_task(void *pv);    //������

#define TASK1_TASK_PRIO  3  //�������ȼ�
#define TASK1_STK_SIZE   128
TaskHandle_t Task1Task_Handler;
void task1_task(void *pv);

#define TASK2_TASK_PRIO  4  //�������ȼ�
#define TASK2_STK_SIZE   128
TaskHandle_t Task2Task_Handler;
void task2_task(void *pv);

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�ж����ȼ����� ����4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	xTaskCreate((TaskFunction_t )start_task,    //������
							 (const char*   )"start_task",  //��������
							 (uint16_t      )START_STK_SIZE, //�����ջ��С
							 (void*         )NULL, 						//������������Ĳ���
							 (UBaseType_t   )START_TASK_PRIO, //�������ȼ�
							 (TaskHandle_t* )&StartTask_Handler); //������
	
    vTaskStartScheduler();        
    return 0;
	
}

void start_task(void *pvParameters)
//��ʼ����������
{	
	taskENTER_CRITICAL();           //�����ٽ���
	
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
							
	vTaskDelete(StartTask_Handler);   //ɾ����ʼ����
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //������
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
				vTaskSuspend(Task1Task_Handler); //��������1
				printf("����task1\r\n");
			break;
			case 'k':
				vTaskResume(Task1Task_Handler);//�ָ�����1
				printf("�ָ�task1\r\n");
			break;
			case 'h':
				vTaskSuspend(Task2Task_Handler); //��������2
				printf("����task2\r\n");
			break;
			case 'i':
				vTaskResume(Task2Task_Handler);//�ָ�����2
				printf("�ָ�task2\r\n");
			break;
		}
	}
}

void task1_task(void *pv)    //������
{
	while(1)
	{
		vTaskDelay(1000);
		printf("task1\r\n");
	}
}

void task2_task(void *pv)    //������
{
	while(1)
	{
		vTaskDelay(1000);
		printf("task2\r\n");
	}
}





