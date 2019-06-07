
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

#define INTERRUPT_TASK_PRIO		2      //任务优先级
#define INTERRUPT_STK_SIZE			256			//任务堆栈大小
TaskHandle_t INTERRUPTTask_Handler;     //任务句柄
void interrupt_task(void *p_arg);       //任务函数

void TIM3_Int_Init(u16 arr, u16 pac);
void TIM5_Int_Init(u16 arr, u16 pac);


int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //中断优先级分组 分组4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	TIM3_Int_Init(1000-1, 7200-1);              //初始化定时器3， 定时器周期1S
	TIM5_Int_Init(1000-1, 7200-1);              //初始化定时器5， 定时器周期1S
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
	xTaskCreate((TaskFunction_t   )interrupt_task,
							(const char*      )"interrupt_task",
							(uint16_t         )INTERRUPT_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )INTERRUPT_TASK_PRIO,
							(TaskHandle_t*    )&INTERRUPTTask_Handler);
							
	vTaskDelete(StartTask_Handler);   //删除开始任务
	taskEXIT_CRITICAL();
}

void interrupt_task(void *pvparameters)
{
	static u32 total_num = 0;
	while(1)
	{
		total_num+=1;
		if(total_num==5)
		{
			printf("关闭中断.....\r\n");
			portDISABLE_INTERRUPTS();
			delay_xms(500);
			printf("打开中断.....\r\n");
			portENABLE_INTERRUPTS();
			
		}
		vTaskDelay(100);
		
	}
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍， 而APB1位36M
//arr; 自动重装
//psc: 时钟预分频数
//这里使用的是定时器3
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
	//TIM_Cmd(TIM3, DISABLE);
}


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍， 而APB1位36M
//arr; 自动重装
//psc: 时钟预分频数
//这里使用的是定时器3
void TIM5_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5, ENABLE);
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET)
	{
		printf("TIM3输出......\r\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
	}
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		printf("TIM5 输出......\r\n");
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		
}


