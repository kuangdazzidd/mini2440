/*
 * led.h
 *
 *  Created on: 2017Äê8ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef DRIVER_LED_H_
#define DRIVER_LED_H_

#include "gpio.h"

#define LED1    S3C2440_GPB(5)
#define LED2    S3C2440_GPB(6)
#define LED3    S3C2440_GPB(7)
#define LED4    S3C2440_GPB(8)


int led_init();
int led_on(int led);
int led_off(int led);
int led_toggle(int led);


#endif /* DRIVER_LED_H_ */
