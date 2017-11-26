/*
 * led.h
 *
 *  Created on: 2017年6月29日
 *      Author: Administrator
 */

#ifndef LED_H_
#define LED_H_


/* 控制寄存器 */
#define GPBCON      (*(volatile unsigned long*)0x56000010)
/* 数据寄存器 */
#define GPBDAT      (*(volatile unsigned long*)0x56000014)
/* 上拉寄存器 */
#define GPBUP       (*(volatile unsigned long*)0x56000018)


#define LED1        0
#define LED2        1
#define LED3        2
#define LED4        3


int led_init();
int led_on(int led);
int led_off(int led);



#endif /* LED_H_ */
