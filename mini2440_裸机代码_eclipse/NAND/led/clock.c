/*
 * clock.c
 *
 *  Created on: 2017��7��3��
 *      Author: Administrator
 */

#include "clock.h"


#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))


/**
 * MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
 * ����:m = (MDIV + 8), p = (PDIV + 2), s = SDIV��Fin �����ʱ��Ƶ��
 */
int clock_init()
{
    CLKDIVN = 0x03; // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1
    __asm__ __volatile__(
            "mrc p15,0,r0,c1,c0,0\n\t"
            "orr r0, r0, #0xc0000000\n\t"
            "mcr p15,0,r0,c1,c0,0"
            );
    MPLLCON = S3C2440_MPLL_200MHZ;  /* ���ڣ�FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
}



