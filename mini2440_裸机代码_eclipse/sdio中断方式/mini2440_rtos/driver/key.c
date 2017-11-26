/*
 * key.c
 *
 *  Created on: 2017��8��5��
 *      Author: Administrator
 */

#include "key.h"
#include "interrupt.h"
#include "uart.h"


void isr_eint8_23(int vector, void *param);

int key_init()
{
    /* ����Ϊ�жϹܽ� */
    s3c2440_gpio_cfgpin(KEY1, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY2, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY3, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY4, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY5, S3C2440_GPIO_IRQ);
    s3c2440_gpio_cfgpin(KEY6, S3C2440_GPIO_IRQ);

    /* ���ô�����ʽ  Ĭ�ϵ͵�ƽ����*/
    s3c2440_gpio_cfgirq(8,  EINT_TRG_LOW_EDGE, 0);
    s3c2440_gpio_cfgirq(11, EINT_TRG_HIGH_EDGE, 0);
    s3c2440_gpio_cfgirq(13, EINT_TRG_DB_EDGE, 0);
    s3c2440_gpio_cfgirq(14, EINT_TRG_LOW, 0);
    s3c2440_gpio_cfgirq(15, EINT_TRG_LOW_EDGE, 0);
    s3c2440_gpio_cfgirq(19, EINT_TRG_LOW_EDGE, 0);

    /* д0�ر����� */
    EINTMASK &= ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));
//    s3c2440_gpio_eint_unmask(8);

    /* ���жϻص����� */
    s3c2440_interrupt_install_irq(INTEINT8_23, isr_eint8_23, NULL);

    /* ȡ������ */
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



