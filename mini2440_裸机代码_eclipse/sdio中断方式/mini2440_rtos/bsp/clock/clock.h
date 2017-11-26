/*
 * clock.h
 *
 *  Created on: 2017年7月31日
 *      Author: Administrator
 */

#ifndef BSP_CLOCK_CLOCK_H_
#define BSP_CLOCK_CLOCK_H_


#define LOCKTIME   (*(volatile unsigned *)0x4c000000) //PLL lock time counter
#define MPLLCON    (*(volatile unsigned *)0x4c000004) //MPLL Control
#define UPLLCON    (*(volatile unsigned *)0x4c000008) //UPLL Control
#define CLKCON     (*(volatile unsigned *)0x4c00000c) //Clock generator control
#define CLKSLOW    (*(volatile unsigned *)0x4c000010) //Slow clock control
#define CLKDIVN    (*(volatile unsigned *)0x4c000014) //Clock divider control
#define CAMDIVN    (*(volatile unsigned *)0x4c000018)   //USB, CAM Clock divider control


#define S3C2440_CLOCK_REG_BASE  (0x4c000000)

#define CONFIG_RUN_CLK          200000000   /* FCLK = 200M */
#define CONFIG_SYS_CLK_FREQ     12000000    /* Fin = 12.00MHz */


#define FCLK                    CONFIG_RUN_CLK
#define HCLK                    100000000   /* 100M */
#define PCLK                    50000000    /* 50M */
#define UCLK

/* 知识点 */
/*
FCLK 是提供给 ARM920T 的时钟
HCLK 是提供给用于 ARM920T，存储器控制器，中断控制器，LCD 控制器，DMA 和 USB 主机模块的 AHB总线的时钟
PCLK 是提供给用于外设如 WDT，IIS，I2C，PWM 定时器，MMC/SD 接口，ADC，UART，GPIO，RTC 和SPI 的 APB 总线的时钟。
*/

/* 计算方法 */
/*
Mpll = ( 2 × m × Fin ) / ( p × 2^s  )
 m = ( MDIV + 8 ), p = ( PDIV + 2 ), s = SDIV

Upll = ( m × Fin ) / ( p × 2^s  )
 m = ( MDIV + 8 ), p = ( PDIV + 2 ), s = SDIV

1 ≤ P ≤ 62，1 ≤ M ≤ 248
*/


#if CONFIG_SYS_CLK_FREQ == 12000000
#if CONFIG_RUN_CLK == 200000000
    /* MPLL=2*12*100/12=200MHz   */
    #define     MPL_MDIV    92      /* m=MPL_MDIV+8=100 */
    #define     MPL_PDIV    1       /* p=MPL_PDIV+2=3   */
    #define     MPL_SDIV    2       /* s=MPL_SDIV=2 */
    /* UPLL=12*64/8=96MHz */
    #define     UPL_MDIV    56      /* m=UPL_MDIV+8=64 */
    #define     UPL_PDIV    2       /* p=UPL_PDIV+2=4   */
    #define     UPL_SDIV    1       /* s=UPL_SDIV=1 */
    /* System clock divider FCLK:HCLK:PCLK=1:2:4 */
    #define     DIVN_UPLL       0x1     /* UCLK = UPLL clock / 2 必须为48M*/
    #define     HDIVN           0x1     /* HCLK = FCLK / 2 */
    #define     PDIVN           0x1     /* PCLK = HCLK / 2 */
#else
    /* MPLL=2*12*100/6=400MHz   */
    #define     MPL_MDIV    92      /* m=MPL_MDIV+8=100 */
    #define     MPL_PDIV    4       /* p=MPL_PDIV+2=6   */
    #define     MPL_SDIV    0       /* s=MPL_SDIV=0 */
    /* UPLL=12*64/8=96MHz */
    #define     UPL_MDIV    56      /* m=UPL_MDIV+8=64 */
    #define     UPL_PDIV    2       /* p=UPL_PDIV+2=4   */
    #define     UPL_SDIV    1       /* s=UPL_SDIV=1 */
    /* System clock divider FCLK:HCLK:PCLK=1:4:8 */
    #define     DIVN_UPLL       0x1     /* UCLK = UPLL clock / 2 必须为48M*/
    #define     HDIVN           0x2     /* HCLK = FCLK / 4 */
    #define     PDIVN           0x1     /* PCLK = HCLK / 2 */
#endif
#endif

typedef struct s3c2440_clk {
    volatile unsigned int locktime;
    volatile unsigned int mpllcon;
    volatile unsigned int upllcon;
    volatile unsigned int clkcon;
    volatile unsigned int clkslow;
    volatile unsigned int clkdivn;
    volatile unsigned int camdivn;
}s3c2440_clk_t;



void s3c2440_clock__init(void);
void s3c2440_clock_get(void);

#endif /* BSP_CLOCK_CLOCK_H_ */
