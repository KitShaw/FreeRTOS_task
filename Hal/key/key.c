
/*
 * led.c
 */

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h" 
#include "key.h"
#include "system.h"
#include "led.h"
#include "stdio.h"

unsigned char k_up_count = 0;
unsigned char k_down_count = 0;
unsigned char k_left_count = 0;
unsigned char k_right_count = 0;

unsigned char k_up_flag = 0;
unsigned char k_down_flag = 0;
unsigned char k_left_flag = 0;
unsigned char k_right_flag = 0;





/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;	   //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   //设置传输速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);		  /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : key_task
* 函数功能		     : 按键任务， 1ms调用一次
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void key_task(void)
{
	if( key_scan() == 0) return;    // 没有键按下直接返回
	
	if((K_UP == 1) &&(K_DOWN == 1) && (K_RIGHT == 1) && (K_LEFT == 1))
		//K_UP按下
	{
		//k_up_count = 0;
		k_down_count = 0;
		k_right_count = 0;
		k_left_count = 0;
		if(k_up_flag == 0)
		{
			if(++k_up_count > 10)
			{
				//led_task();
				printf("k_up");
				k_up_flag = 1;
			}
		}
	}
	
	if((K_UP == 0) &&(K_DOWN == 0) && (K_RIGHT == 1) && (K_LEFT == 1))
		//K_DOWN按下
	{
		k_up_count = 0;
		//k_down_count = 0;
		k_right_count = 0;
		k_left_count = 0;
		if(k_down_flag == 0)
		{
			if(++k_down_count > 10)
			{
				//led_task();
				printf("k_down");
				k_down_flag = 1;
			}
		}
	}
	
	if((K_UP == 0) &&(K_DOWN == 1) && (K_RIGHT == 0) && (K_LEFT == 1))
		//K_RIGHT按下
	{
		k_up_count = 0;
		k_down_count = 0;
		//k_right_count = 0;
		k_left_count = 0;
		if(k_right_flag == 0)
		{
			if(++k_right_count > 10)
			{
				//led_task();
				printf("k_right");
				k_right_flag = 1;
			}
		}
	}
	
	if((K_UP == 0) &&(K_DOWN == 1) && (K_RIGHT == 1) && (K_LEFT == 0))
		//K_LEFT按下
	{
		k_up_count = 0;
		k_down_count = 0;
		k_right_count = 0;
		//k_left_count = 0;
		if(k_left_flag == 0)
		{
			if(++k_left_count > 10)
			{
				//led_task();
				printf("k_left");
				k_left_flag = 1;
			}
		}
	}
}

/*******************************************************************************
* 函 数 名         : key_scan
* 函数功能		     : 按键任务， key_task里调用
* 输    入         : 无
* 输    出         : 有键按下返回1， 没键按下返回0
*******************************************************************************/

unsigned char key_scan(void)
{
	unsigned char tmp;
	if((K_UP == 0) &&(K_DOWN == 1) && (K_RIGHT == 1) && (K_LEFT == 1))
	{
		k_up_count = 0;
		k_down_count = 0;
		k_right_count = 0;
		k_left_count = 0;
		tmp = 0; //没有按键按下返回0
	}
	else tmp = 1;  //有键按下返回1
	
	if(k_up_flag == 1)  //等待按键释放
	{
		if(K_UP == 0)
		{
			k_up_flag = 0;
			k_up_count = 0;
		}
	}
	
	if(k_down_flag == 1)  //等待按键释放
	{
		if(K_DOWN == 1)
		{
			k_down_flag = 0;
			k_down_count = 0;
		}
	}
	
	if(k_right_flag == 1)  //等待按键释放
	{
		if(K_RIGHT == 1)
		{
			k_right_flag = 0;
			k_right_count = 0;
		}
	}
	
	if(k_left_flag == 1)  //等待按键释放
	{
		if(K_LEFT == 1)
		{
			k_left_flag = 0;
			k_left_count = 0;
		}
	}
	
	return tmp;
}



