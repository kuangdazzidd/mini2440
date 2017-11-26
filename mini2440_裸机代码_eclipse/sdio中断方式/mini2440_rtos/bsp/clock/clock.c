/*
 * clock.c
 *
 *  Created on: 2017年7月31日
 *      Author: Administrator
 */

#include "clock.h"
#include "type.h"
#include "io.h"



int s3c2440_clock_set_mpll(uint8_t        sdiv,
                           uint8_t        pdiv,
                           uint8_t        mdiv)
{
    MPLLCON = sdiv | (pdiv<<4) | (mdiv<<12);
    return 0;
}

int s3c2440_clock_set_upll(uint8_t        sdiv,
                            uint8_t        pdiv,
                            uint8_t        mdiv)
{
    UPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
    return 0;
}

int s3c2440_clock_set_divider(uint8_t        hdivn,
                               uint8_t        pdivn)
{
    CLKDIVN = (hdivn<<1) | pdivn;
    return 0;
}



/**
 * System Clock Configuration
 */
void s3c2440_clock__init(void)
{
    LOCKTIME = 0xFFFFFFFF;
    __asm__ __volatile__(
            "mrc p15,0,r0,c1,c0,0\n\t"
            "orr r0, r0, #0xc0000000\n\t"
            "mcr p15,0,r0,c1,c0,0"
            );

    /* DIV的设置必须放在最前面 */
    s3c2440_clock_set_divider(HDIVN, PDIVN);
    s3c2440_clock_set_mpll(MPL_SDIV, MPL_PDIV, MPL_MDIV);
    s3c2440_clock_set_upll(UPL_SDIV, UPL_PDIV, UPL_MDIV);
}


