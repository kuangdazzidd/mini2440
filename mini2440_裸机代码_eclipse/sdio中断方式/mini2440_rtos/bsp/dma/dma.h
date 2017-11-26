/*
 * dma.h
 *
 *  Created on: 2017Äê8ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef BSP_DMA_DMA_H_
#define BSP_DMA_DMA_H_

// DMA
#define DISRC0     (*(volatile unsigned *)0x4b000000) //DMA 0 Initial source
#define DISRCC0    (*(volatile unsigned *)0x4b000004) //DMA 0 Initial source control
#define DIDST0     (*(volatile unsigned *)0x4b000008) //DMA 0 Initial Destination
#define DIDSTC0    (*(volatile unsigned *)0x4b00000c) //DMA 0 Initial Destination control
#define DCON0      (*(volatile unsigned *)0x4b000010) //DMA 0 Control
#define DSTAT0     (*(volatile unsigned *)0x4b000014) //DMA 0 Status
#define DCSRC0     (*(volatile unsigned *)0x4b000018) //DMA 0 Current source
#define DCDST0     (*(volatile unsigned *)0x4b00001c) //DMA 0 Current destination
#define DMASKTRIG0 (*(volatile unsigned *)0x4b000020) //DMA 0 Mask trigger

#define DISRC1     (*(volatile unsigned *)0x4b000040) //DMA 1 Initial source
#define DISRCC1    (*(volatile unsigned *)0x4b000044) //DMA 1 Initial source control
#define DIDST1     (*(volatile unsigned *)0x4b000048) //DMA 1 Initial Destination
#define DIDSTC1    (*(volatile unsigned *)0x4b00004c) //DMA 1 Initial Destination control
#define DCON1      (*(volatile unsigned *)0x4b000050) //DMA 1 Control
#define DSTAT1     (*(volatile unsigned *)0x4b000054) //DMA 1 Status
#define DCSRC1     (*(volatile unsigned *)0x4b000058) //DMA 1 Current source
#define DCDST1     (*(volatile unsigned *)0x4b00005c) //DMA 1 Current destination
#define DMASKTRIG1 (*(volatile unsigned *)0x4b000060) //DMA 1 Mask trigger

#define DISRC2     (*(volatile unsigned *)0x4b000080) //DMA 2 Initial source
#define DISRCC2    (*(volatile unsigned *)0x4b000084) //DMA 2 Initial source control
#define DIDST2     (*(volatile unsigned *)0x4b000088) //DMA 2 Initial Destination
#define DIDSTC2    (*(volatile unsigned *)0x4b00008c) //DMA 2 Initial Destination control
#define DCON2      (*(volatile unsigned *)0x4b000090) //DMA 2 Control
#define DSTAT2     (*(volatile unsigned *)0x4b000094) //DMA 2 Status
#define DCSRC2     (*(volatile unsigned *)0x4b000098) //DMA 2 Current source
#define DCDST2     (*(volatile unsigned *)0x4b00009c) //DMA 2 Current destination
#define DMASKTRIG2 (*(volatile unsigned *)0x4b0000a0) //DMA 2 Mask trigger

#define DISRC3     (*(volatile unsigned *)0x4b0000c0) //DMA 3 Initial source
#define DISRCC3    (*(volatile unsigned *)0x4b0000c4) //DMA 3 Initial source control
#define DIDST3     (*(volatile unsigned *)0x4b0000c8) //DMA 3 Initial Destination
#define DIDSTC3    (*(volatile unsigned *)0x4b0000cc) //DMA 3 Initial Destination control
#define DCON3      (*(volatile unsigned *)0x4b0000d0) //DMA 3 Control
#define DSTAT3     (*(volatile unsigned *)0x4b0000d4) //DMA 3 Status
#define DCSRC3     (*(volatile unsigned *)0x4b0000d8) //DMA 3 Current source
#define DCDST3     (*(volatile unsigned *)0x4b0000dc) //DMA 3 Current destination
#define DMASKTRIG3 (*(volatile unsigned *)0x4b0000e0) //DMA 3 Mask trigger


int s3c2440_dma_init();

int s3c2440_dma_start();


#endif /* BSP_DMA_DMA_H_ */
