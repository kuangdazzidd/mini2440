/*
 * interrupt.h
 *
 *  Created on: 2017Äê8ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef BSP_INTERRUPT_INTERRUPT_H_
#define BSP_INTERRUPT_INTERRUPT_H_


// INTERRUPT
#define SRCPND     (*(volatile unsigned *)0x4a000000) //Interrupt request status
#define INTMOD     (*(volatile unsigned *)0x4a000004) //Interrupt mode control
#define INTMSK     (*(volatile unsigned *)0x4a000008) //Interrupt mask control
#define PRIORITY   (*(volatile unsigned *)0x4a00000c) //IRQ priority control
#define INTPND     (*(volatile unsigned *)0x4a000010) //Interrupt request status
#define INTOFFSET  (*(volatile unsigned *)0x4a000014) //Interruot request source offset
#define SUBSRCPND  (*(volatile unsigned *)0x4a000018) //Sub source pending
#define INTSUBMSK  (*(volatile unsigned *)0x4a00001c) //Interrupt sub mask


// PENDING BIT
#define INTEINT0      (0)
#define INTEINT1      (1)
#define INTEINT2      (2)
#define INTEINT3      (3)
#define INTEINT4_7    (4)
#define INTEINT8_23   (5)
#define INTNOTUSED6   (6)
#define INTBAT_FLT    (7)
#define INTTICK       (8)
#define INTWDT        (9)
#define INTTIMER0     (10)
#define INTTIMER1     (11)
#define INTTIMER2     (12)
#define INTTIMER3     (13)
#define INTTIMER4     (14)
#define INTUART2      (15)
#define INTLCD        (16)
#define INTDMA0       (17)
#define INTDMA1       (18)
#define INTDMA2       (19)
#define INTDMA3       (20)
#define INTSDI        (21)
#define INTSPI0       (22)
#define INTUART1      (23)
#define INTNIC        (24)
#define INTUSBD       (25)
#define INTUSBH       (26)
#define INTIIC        (27)
#define INTUART0      (28)
#define INTSPI1       (29)
#define INTRTC        (30)
#define INTADC        (31)


// SUB PENDING BIT
#define INTSUB_ADC    (10)
#define INTSUB_TC     (9)
#define INTSUB_ERR2   (8)
#define INTSUB_TXD2   (7)
#define INTSUB_RXD2   (6)
#define INTSUB_ERR1   (5)
#define INTSUB_TXD1   (4)
#define INTSUB_RXD1   (3)
#define INTSUB_ERR0   (2)
#define INTSUB_TXD0   (1)
#define INTSUB_RXD0   (0)

#define BIT_ALLMSK     (0xffffffff)
#define BIT_SUB_ALLMSK (0x7ff)



/*
 * Interrupt handler definition
 */
typedef void (*pfn_isr_handler_t)(int vector, void *param);

struct irq_desc
{
    pfn_isr_handler_t handler;
    void              *param;
};


int s3c2440_inetrrupt_init();
int s3c2440_interrupt_mask(int vector);
int s3c2440_interrupt_unmask(int vector);
int s3c2440_interrupt_submask(int vector);
int s3c2440_interrupt_unsubmask(int vector);

int s3c2440_interrupt_install_irq(int               vector,
                                  pfn_isr_handler_t p_cb,
                                  void             *p_arg);

void eint_handle();


#endif /* BSP_INTERRUPT_INTERRUPT_H_ */
