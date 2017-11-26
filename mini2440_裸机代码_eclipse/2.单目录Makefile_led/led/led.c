/*
 * led.c
 *
 *  Created on: 2017��6��29��
 *      Author: Administrator
 */

#include "led.h"


int led_init()
{
    /* ��������LEDΪ��� */
    GPBCON = 0x00015400;
    /* ��������LED���� */
    GPBDAT = 0x000007ff;

    return 0;
}

int led_on(int led)
{
    GPBDAT &= (~(0x01 << (led + 5)));
    return 0;
}


int led_off(int led)
{
    GPBDAT |= 0x01 << (led + 5);
    return 0;
}



