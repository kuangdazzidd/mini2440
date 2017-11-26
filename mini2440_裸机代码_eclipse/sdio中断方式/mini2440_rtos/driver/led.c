/*
 * led.c
 *
 *  Created on: 2017年8月5日
 *      Author: Administrator
 */


#include "led.h"



int led_init()
{
    s3c2440_gpio_cfgpin(LED1, S3C2440_GPIO_OUTPUT);
    s3c2440_gpio_pullup(LED1, 1);
    s3c2440_gpio_cfgpin(LED2, S3C2440_GPIO_OUTPUT);
    s3c2440_gpio_pullup(LED2, 1);
    s3c2440_gpio_cfgpin(LED3, S3C2440_GPIO_OUTPUT);
    s3c2440_gpio_pullup(LED3, 1);
    s3c2440_gpio_cfgpin(LED4, S3C2440_GPIO_OUTPUT);
    s3c2440_gpio_pullup(LED4, 1);

    /* 默认关闭 */
    s3c2440_gpio_setpin(LED1, 1);
    s3c2440_gpio_setpin(LED2, 1);
    s3c2440_gpio_setpin(LED3, 1);
    s3c2440_gpio_setpin(LED4, 1);
    return 0;
}


int led_on(int led)
{
    s3c2440_gpio_setpin(led, 0);
    return 0;
}

int led_off(int led)
{
    s3c2440_gpio_setpin(led, 1);
    return 0;
}


int led_toggle(int led)
{
    int s = s3c2440_gpio_getpin(led);
    if (s) {
        s3c2440_gpio_setpin(led, 0);
    } else {
        s3c2440_gpio_setpin(led, 1);
    }
    return 0;
}


