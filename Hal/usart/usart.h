
/*
 * usart.h
 */
 
#ifndef __USART_H
#define __USART_H

#define USART_REC_LEN	200  	//定义最大接收字节数 200
#define EN_USART1_RX 	1			//使能（1）/禁止（0）串口1接收
	  	


void USART1_init(unsigned long bound);

#endif
