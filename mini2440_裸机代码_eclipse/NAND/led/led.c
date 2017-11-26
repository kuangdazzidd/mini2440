/*
 * led.c
 *
 *  Created on: 2017年6月29日
 *      Author: Administrator
 */

#include "led.h"


int led_init()
{
    /* 配置所有LED为输出 */
    GPBCON = 0x00015400;
    /* 配置所有LED不亮 */
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



