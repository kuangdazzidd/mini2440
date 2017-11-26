/*
 * dma.c
 *
 *  Created on: 2017Äê8ÔÂ6ÈÕ
 *      Author: Administrator
 */

#include "dma.h"
#include "interrupt.h"
#include "uart.h"

unsigned char uart0_dma_buf[512] = {"hello mini2440 dma test!\r\n"};

void isr_int_dma0(int vector, void *param);

int s3c2440_dma_init()
{
    DISRC0  = (uint32_t)uart0_dma_buf;
    DISRCC0 = (0 << 0) | (0 << 1);
    DIDST0  = (uint32_t)(&UTXH0);
    DIDSTC0 = (1 << 0) | (1 << 1) | (0 << 2);
    DCON0   = (1 << 31) | (0 << 30) | (1 << 29) | (0 << 28) | (0 << 27) |
              (0x1 << 24) | (1 << 23) | (1 << 22) | (0 << 20) | (512 << 0);

    s3c2440_interrupt_unmask(INTDMA0);
    s3c2440_interrupt_install_irq(INTDMA0, isr_int_dma0, NULL);

    return 0;
}

int s3c2440_dma_start()
{
    DMASKTRIG0 = (1 << 1);
    return 0;
}


void isr_int_dma0(int vector, void *param)
{
    int a = 0;
    a++;
}


