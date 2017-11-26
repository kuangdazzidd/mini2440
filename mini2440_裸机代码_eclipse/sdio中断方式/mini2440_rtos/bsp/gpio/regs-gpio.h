/*
 * Regs-gpio.h
 *
 *  Created on: 2017Äê7ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef BSP_GPIO_REGS_GPIO_H_
#define BSP_GPIO_REGS_GPIO_H_

#define S3C24XX_GPIO_BASE(x)  ((((x) & ~31) >> 1) + 0x56000000)
#define S3C24XX_MISCCR        (0x56000000 + 0x80)
#define S3C2440_GPIO_OFFSET(pin) ((pin) & 31)


/* general configuration options */
#define S3C2440_GPIO_LEAVE   (0xFFFFFFFF)
#define S3C2440_GPIO_INPUT   (0xFFFFFFF0)   /* not available on A */
#define S3C2440_GPIO_OUTPUT  (0xFFFFFFF1)
#define S3C2440_GPIO_IRQ     (0xFFFFFFF2)   /* not available for all */
#define S3C2440_GPIO_SFN2    (0xFFFFFFF2)   /* bank A => addr/cs/nand */
#define S3C2440_GPIO_SFN3    (0xFFFFFFF3)   /* not available on A */


#define S3C2440_GPIO_BANKA   (32*0)
#define S3C2440_GPIO_BANKB   (32*1)
#define S3C2440_GPIO_BANKC   (32*2)
#define S3C2440_GPIO_BANKD   (32*3)
#define S3C2440_GPIO_BANKE   (32*4)
#define S3C2440_GPIO_BANKF   (32*5)
#define S3C2440_GPIO_BANKG   (32*6)
#define S3C2440_GPIO_BANKH   (32*7)


/* GPIO bank sizes */
#define S3C2440_GPIO_A_NR   (32)
#define S3C2440_GPIO_B_NR   (32)
#define S3C2440_GPIO_C_NR   (32)
#define S3C2440_GPIO_D_NR   (32)
#define S3C2440_GPIO_E_NR   (32)
#define S3C2440_GPIO_F_NR   (32)
#define S3C2440_GPIO_G_NR   (32)
#define S3C2440_GPIO_H_NR   (32)

#define S3C2440_GPIO_NEXT(__gpio) \
    ((__gpio##_START) + (__gpio##_NR) + 0)

enum s3c_gpio_number {
    S3C2440_GPIO_A_START = 0,
    S3C2440_GPIO_B_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_A),
    S3C2440_GPIO_C_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_B),
    S3C2440_GPIO_D_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_C),
    S3C2440_GPIO_E_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_D),
    S3C2440_GPIO_F_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_E),
    S3C2440_GPIO_G_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_F),
    S3C2440_GPIO_H_START = S3C2440_GPIO_NEXT(S3C2440_GPIO_G),
};

/* S3C2440 GPIO number definitions. */

#define S3C2440_GPA(_nr)    (S3C2440_GPIO_A_START + (_nr))
#define S3C2440_GPB(_nr)    (S3C2440_GPIO_B_START + (_nr))
#define S3C2440_GPC(_nr)    (S3C2440_GPIO_C_START + (_nr))
#define S3C2440_GPD(_nr)    (S3C2440_GPIO_D_START + (_nr))
#define S3C2440_GPE(_nr)    (S3C2440_GPIO_E_START + (_nr))
#define S3C2440_GPF(_nr)    (S3C2440_GPIO_F_START + (_nr))
#define S3C2440_GPG(_nr)    (S3C2440_GPIO_G_START + (_nr))
#define S3C2440_GPH(_nr)    (S3C2440_GPIO_H_START + (_nr))




#define MISCCR    (*(volatile unsigned *)0x56000080) //Miscellaneous control
#define DCLKCON   (*(volatile unsigned *)0x56000084) //DCLK0/1 control
#define EXTINT0   (*(volatile unsigned *)0x56000088) //External interrupt control egister 0
#define EXTINT1   (*(volatile unsigned *)0x5600008c) //External interrupt control egister 1
#define EXTINT2   (*(volatile unsigned *)0x56000090) //External interrupt control egister 2
#define EINTFLT0  (*(volatile unsigned *)0x56000094) //Reserved
#define EINTFLT1  (*(volatile unsigned *)0x56000098) //Reserved
#define EINTFLT2  (*(volatile unsigned *)0x5600009c) //External interrupt filter control egister 2
#define EINTFLT3  (*(volatile unsigned *)0x560000a0) //External interrupt filter control egister 3
#define EINTMASK  (*(volatile unsigned *)0x560000a4) //External interrupt mask
#define EINTPEND  (*(volatile unsigned *)0x560000a8) //External interrupt pending
#define GSTATUS0  (*(volatile unsigned *)0x560000ac) //External pin status
#define GSTATUS1  (*(volatile unsigned *)0x560000b0) //Chip ID(0x32410000)
#define GSTATUS2  (*(volatile unsigned *)0x560000b4) //Reset type
#define GSTATUS3  (*(volatile unsigned *)0x560000b8) //Saved data0(32-bit) before entering POWER_OFF mode
#define GSTATUS4  (*(volatile unsigned *)0x560000bc) //Saved data0(32-bit) before entering POWER_OFF mode



#define EINT_TRG_LOW            0x00
#define EINT_TRG_HIGH           0x01
#define EINT_TRG_LOW_EDGE       0x02
#define EINT_TRG_HIGH_EDGE      0x04
#define EINT_TRG_DB_EDGE        0x06


#endif /* BSP_GPIO_REGS_GPIO_H_ */
