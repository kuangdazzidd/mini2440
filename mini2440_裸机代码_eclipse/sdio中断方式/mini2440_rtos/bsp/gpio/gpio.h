/*
 * gpio.h
 *
 *  Created on: 2017Äê7ÔÂ25ÈÕ
 *      Author: Administrator
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <error.h>
#include <regs-gpio.h>
#include "io.h"


void s3c2440_gpio_cfgpin(unsigned int pin, unsigned int function);

unsigned int s3c2440_gpio_getcfg(unsigned int pin);

void s3c2440_gpio_pullup(unsigned int pin, unsigned int to);

int s3c2440_gpio_getpull(unsigned int pin);

void s3c2440_gpio_setpin(unsigned int pin, unsigned int to);

unsigned int s3c2440_gpio_getpin(unsigned int pin);

int s3c2440_gpio_getirq(unsigned int pin);

void s3c2440_gpio_cfgirq(unsigned int eint, unsigned int trg_way, uint8_t filter);

void s3c2440_gpio_eint_mask(unsigned int eint);

void s3c2440_gpio_eint_unmask(unsigned int eint);


#endif /* GPIO_H_ */
