/*
 * key.c
 *
 *  Created on: 2017年7月9日
 *      Author: Administrator
 */

#include "key.h"
#include "2440addr.h"


/* 控制寄存器 */
#define GPGCON      (*(volatile unsigned long*)0x56000060)
/* 数据寄存器 */
#define GPGDAT      (*(volatile unsigned long*)0x56000064)
/* 上拉寄存器 */
#define GPGUP       (*(volatile unsigned long*)0x56000068)

#define EINTMASK    (*(volatile unsigned long*)0x560000A4)
#define INTMSK      (*(volatile unsigned long*)0x4A000008)
#define EXTINT1     (*(volatile unsigned long*)0x5600008C)
#define EINTPEND    (*(volatile unsigned long*)0x560000A8)
#define INTMOD      (*(volatile unsigned long*)0X4A000004)

#define KEY1    GPG0    /* EINT8 */
#define KEY2    GPG3    /* EINT11 */
#define KEY3    GPG5    /* EINT13 */
#define KEY4    GPG6    /* EINT14 */
#define KEY5    GPG7    /* EINT15 */
#define KEY6    GPG11   /* EINT19 */



int isr_init()
{
    rINTMOD     = 0x0;                     // All=IRQ mode
    rSRCPND     = rSRCPND;                 // clear all interrupt
    rINTPND     = rINTPND;                 // clear all interrupt
    rINTMSK     = BIT_ALLMSK;              // All interrupt is masked.
    rINTSUBMSK  = BIT_SUB_ALLMSK;          // All sub-interrupt is masked.
// External interrupt will be falling edge triggered.
    rEXTINT0 = 0x22222222;    // EINT[7:0]
    rEXTINT1 = 0x22222222;    // EINT[15:8]
    rEXTINT2 = 0x22222222;    // EINT[23:16]

    return 0;
}

void EINT_Handle();

int key_init()
{
    // 配置GPGCON，设置6按键对应的GPGx管脚功能为外部中断引脚EINT
    rGPGCON = rGPGCON & (~((3<<0)|(3<<6))) | ((2<<0)|(2<<6)) ;      //GPG0,11 set EINT
    rGPGCON = rGPGCON & (~((3<<10)|(3<<12))) | ((2<<10)|(2<<12)) ;      //GPG5,6 set EINT
    rGPGCON = rGPGCON & (~((3<<14)|(3<<22))) | ((2<<14)|(2<<22)) ;      //GPG7,11 set EINT

    // 设置中断触发方式
    rEXTINT1 &= ~(7<<0);
    rEXTINT1 |= (2<<0);      //set eint8 falling edge int

    rEXTINT1 &= ~(7<<12);
    rEXTINT1 |= (2<<12);    //set eint11 falling edge int

    rEXTINT1 &= ~(7<<20);
    rEXTINT1 |= (2<<20);    //set eint13 falling edge int

    rEXTINT1 &= ~(7<<24);
    rEXTINT1 |= (2<<24);    //set eint14 falling edge int

    rEXTINT1 &= ~(7<<28);
    rEXTINT1 |= (2<<28);    //set eint15 falling edge int

    rEXTINT2 &= ~(7<<12);
    rEXTINT2 |= (2<<12);    //set eint19 falling edge int

    // 将按键中断处理程序注册，入口地址对应EINT8_23中断IRQ
    pISR_EINT8_23 = (unsigned int)EINT_Handle;

    rEINTPEND = 0xFFFFFF;     //清空 EINTPEND中断请求
    rSRCPND = BIT_EINT8_23; //to clear the previous pending states in SRCPND
    rINTPND = BIT_EINT8_23;  //  to clear the previous pending states in INTPND

    rEINTMASK=~( (1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19) );  //清空六个外部中断对应的中断屏蔽位
    rINTMSK=~(BIT_EINT8_23); // 清空BIT_EINT8_23对应的中断屏蔽位

    return 0;
}



void EINT_Handle()
{
    unsigned long oft = 0;
    unsigned long val;

    rSRCPND     = rSRCPND;                 // clear all interrupt
    rINTPND     = rINTPND;                 // clear all interrupt
    rEINTPEND   = rEINTPEND;

    oft += 1;
}

