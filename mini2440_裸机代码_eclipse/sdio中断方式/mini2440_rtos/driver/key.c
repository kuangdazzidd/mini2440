/*
 * key.c
 *
 *  Created on: 2017年8月5日
 *      Author: Administrator
 */

#include "key.h"
#include "interrupt.h"
#include "uart.h"


void isr_eint8_23(int vector, void *param);

int key_init()
{
    /* 设置为中断管脚 */
    s3c2440_gpio_cfgpin(KEY1, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY2, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY3, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY4, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY5, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY6, S3C2440_GPIO_IRQ);

    /* 设置触发方式  默认低电平触发*/
    s3c2440_gpio_cfgirq(8,  EINT_TRG_LOW_EDGE, 0);
    s3c2440_gpio_cfgirq(11, EINT_TRG_HIGH_EDGE, 0);
    s3c2440_gpio_cfgirq(13, EINT_TRG_DB_EDGE, 0);
    s3c2440_gpio_cfgirq(14, EINT_TRG_LOW, 0);
    s3c2440_gpio_cfgirq(15, EINT_TRG_LOW_EDGE, 0);
    s3c2440_gpio_cfgirq(19, EINT_TRG_LOW_EDGE, 0);

    /* 写0关闭屏蔽 */
    EINTMASK &= ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));
//    s3c2440_gpio_eint_unmask(8);

    /* 绑定中断回调函数 */
    s3c2440_interrupt_install_irq(INTEINT8_23, isr_eint8_23, NULL);

    /* 取消屏蔽 */
    s3c2440_interrupt_unmask(INTEINT8_23);

    return 0;
}

void isr_eint8_23(int vector, void *param)
{
    int i = 0;
    int pend = EINTPEND;

    EINTPEND = EINTPEND;

    for (i = 4; i < 23; i++) {
        if (pend & (1 << i)) {
            s3c2440_uart_put_char(i + 0x30);
        }
    }
}



