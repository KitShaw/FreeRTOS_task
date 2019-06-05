
/*
 * led.h
 */
 
#ifndef __LED_H
#define __LED_H

//#include "system.h"
//#include "FreeRTOS.h"
//#include "task.h"

#define LED_ON	0
#define LED_OFF	1

#define LED0	PCout(0)
#define LED1	PCout(1)
#define LED2	PCout(2)
#define LED3	PCout(3)
#define LED4	PCout(4)
#define LED5	PCout(5)
#define LED6	PCout(6)
#define LED7	PCout(7)


void led_init(void);
void led0_on(void);
void led0_off(void);
//void led_task(void);
void led_task(void *pvParameters);
//TaskHandle_t read_led_task_Handler(void);

#endif
