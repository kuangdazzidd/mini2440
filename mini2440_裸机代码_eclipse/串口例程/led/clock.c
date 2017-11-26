/*
 * clock.c
 *
 *  Created on: 2017年7月3日
 *      Author: Administrator
 */

#include "clock.h"


#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))


/**
 * MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
 * 其中:m = (MDIV + 8), p = (PDIV + 2), s = SDIV，Fin 输入的时钟频率
 */
int clock_init()
{
    CLKDIVN = 0x03; // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1

    MPLLCON = S3C2440_MPLL_200MHZ;  /* 现在，FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
}



