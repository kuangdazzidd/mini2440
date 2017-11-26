/*
 * gpio.c
 *
 *  Created on: 2017Äê7ÔÂ25ÈÕ
 *      Author: Administrator
 */

#include "gpio.h"

void s3c2440_gpio_cfgpin(unsigned int pin, unsigned int function)
{
    void  *base = (void*)S3C24XX_GPIO_BASE(pin);
    unsigned long mask;
    unsigned long con;
    unsigned long flags;

    if (pin < S3C2440_GPIO_BANKB) {
        mask = 1 << S3C2440_GPIO_OFFSET(pin);
    } else {
        mask = 3 << S3C2440_GPIO_OFFSET(pin)*2;
    }

    switch (function) {
    case S3C2440_GPIO_LEAVE:
        mask = 0;
        function = 0;
        break;

    case S3C2440_GPIO_INPUT:
    case S3C2440_GPIO_OUTPUT:
    case S3C2440_GPIO_SFN2:
    case S3C2440_GPIO_SFN3:
        if (pin < S3C2440_GPIO_BANKB) {
            function -= 1;
            function &= 1;
            function <<= S3C2440_GPIO_OFFSET(pin);
        } else {
            function &= 3;
            function <<= S3C2440_GPIO_OFFSET(pin)*2;
        }
    }

    /* modify the specified register wwith IRQs off */
//    local_irq_save(flags);

    con  = __raw_readl(base + 0x00);
    con &= ~mask;
    con |= function;

    __raw_writel(con, base + 0x00);

//    local_irq_restore(flags);
}


unsigned int s3c2440_gpio_getcfg(unsigned int pin)
{
    void  *base = (void*)S3C24XX_GPIO_BASE(pin);
    unsigned long val = __raw_readl(base);

    if (pin < S3C2440_GPIO_BANKB) {
        val >>= S3C2440_GPIO_OFFSET(pin);
        val &= 1;
        val += 1;
    } else {
        val >>= S3C2440_GPIO_OFFSET(pin)*2;
        val &= 3;
    }

    return val | S3C2440_GPIO_INPUT;
}

void s3c2440_gpio_pullup(unsigned int pin, unsigned int to)
{
    void  *base = (void*)S3C24XX_GPIO_BASE(pin);
    unsigned long offs = S3C2440_GPIO_OFFSET(pin);
    unsigned long flags;
    unsigned long up;

    if (pin < S3C2440_GPIO_BANKB)
        return;

//    local_irq_save(flags);

    up = __raw_readl(base + 0x08);
    up &= ~(1L << offs);
    up |= to << offs;
    __raw_writel(up, base + 0x08);

//    local_irq_restore(flags);
}


int s3c2440_gpio_getpull(unsigned int pin)
{
    void  *base = (void*)S3C24XX_GPIO_BASE(pin);
    unsigned long offs = S3C2440_GPIO_OFFSET(pin);

    if (pin < S3C2440_GPIO_BANKB)
        return -EINVAL;

    return (__raw_readl(base + 0x08) & (1L << offs)) ? 1 : 0;
}


void s3c2440_gpio_setpin(unsigned int pin, unsigned int to)
{
    void  *base = (void*)S3C24XX_GPIO_BASE(pin);
    unsigned long offs = S3C2440_GPIO_OFFSET(pin);
    unsigned long flags;
    unsigned long dat;

//    local_irq_save(flags);

    dat = __raw_readl(base + 0x04);
    dat &= ~(1 << offs);
    dat |= to << offs;
    __raw_writel(dat, base + 0x04);

//    local_irq_restore(flags);
}


unsigned int s3c2440_gpio_getpin(unsigned int pin)
{
    void  *base = (void*)S3C24XX_GPIO_BASE(pin);
    unsigned long offs = S3C2440_GPIO_OFFSET(pin);

    return __raw_readl(base + 0x04) & (1<< offs);
}


int s3c2440_gpio_getirq(unsigned int pin)
{
//    if (pin < S3C2440_GPF(0) || pin > S3C2440_GPG(15))
//        return -EINVAL; /* not valid interrupts */
//
//    if (pin < S3C2440_GPG(0) && pin > S3C2440_GPF(7))
//        return -EINVAL; /* not valid pin */
//
//    if (pin < S3C2440_GPF(4))
//        return (pin - S3C2440_GPF(0)) + IRQ_EINT0;
//
//    if (pin < S3C2440_GPG(0))
//        return (pin - S3C2440_GPF(4)) + IRQ_EINT4;
//
//    return (pin - S3C2440_GPG(0)) + IRQ_EINT8;

    return 0;
}


void s3c2440_gpio_cfgirq(unsigned int eint, unsigned int trg_way, uint8_t filter)
{
    void  *base = NULL;
    unsigned long dat;

    if (eint <= 7) {
        base = (void*)(S3C24XX_MISCCR + 0x08);
    } else if (eint <= 15) {
        base = (void*)(S3C24XX_MISCCR + 0x0c);
        eint = eint - 8;
    } else {
        base = (void*)(S3C24XX_MISCCR + 0x10);
        eint = eint - 16;
    }

    dat = __raw_readl(base);
    dat &= ~(0xf << (eint * 4));
    dat |= (trg_way << (eint * 4) | filter << ((eint * 4) + 3));
    __raw_writel(dat, base);

}


void s3c2440_gpio_eint_mask(unsigned int eint)
{
    EINTMASK |= (1<<eint);
}

void s3c2440_gpio_eint_unmask(unsigned int eint)
{
    EINTMASK &= ~(1<<eint);
}


