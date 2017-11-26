/*
 * led.h
 *
 *  Created on: 2017��6��29��
 *      Author: Administrator
 */

#ifndef LED_H_
#define LED_H_


/* ���ƼĴ��� */
#define GPBCON      (*(volatile unsigned long*)0x56000010)
/* ���ݼĴ��� */
#define GPBDAT      (*(volatile unsigned long*)0x56000014)
/* �����Ĵ��� */
#define GPBUP       (*(volatile unsigned long*)0x56000018)


#define LED1        0
#define LED2        1
#define LED3        2
#define LED4        3


int led_init();
int led_on(int led);
int led_off(int led);



#endif /* LED_H_ */
