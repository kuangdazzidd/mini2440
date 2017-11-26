/*
 * interrupt.c
 *
 *  Created on: 2017年8月5日
 *      Author: Administrator
 */

#include "interrupt.h"
#include "error.h"
#include "string.h"

#define __SYS_MEM_ADDR_START  (0x30000000)

#define __IRQ_MAX_HANDLE      (32)

struct irq_desc  __g_irq_desc_tab[__IRQ_MAX_HANDLE];

int s3c2440_inetrrupt_init()
{
    int j = 0;
    int i = 0;

    /* 写一清零所有中断源和子中断源 */
    SRCPND    = BIT_ALLMSK;
    SUBSRCPND = BIT_SUB_ALLMSK;

    /* 所有IRQ模式 */
    INTMOD = 0x0;

    /* 1为禁止所有中断源和子中断源 */
    INTMSK    = BIT_ALLMSK;
    INTSUBMSK = BIT_SUB_ALLMSK;

    /* 优先级默认 */
    PRIORITY = 0x0;

    /* 写一清零挂起寄存器 */
    INTPND = BIT_ALLMSK;

    /* 拷贝中断向量表到内存 */
    extern unsigned long _vector_end;
    for (j = (unsigned long)__SYS_MEM_ADDR_START; j < (unsigned long)&_vector_end; j++) {
        ((unsigned long *)0x0)[i] = ((unsigned long *)__SYS_MEM_ADDR_START)[i];
        i++;
    }

    memset(__g_irq_desc_tab, 0x0, sizeof(__g_irq_desc_tab));

    return 0;
}


int s3c2440_interrupt_mask(int vector)
{
    INTMSK |= (1 << vector);
    return 0;
}


int s3c2440_interrupt_unmask(int vector)
{
    INTMSK &= ~(1 << vector);
    return 0;
}


int s3c2440_interrupt_submask(int vector)
{
    INTSUBMSK |= (1 << vector);
    return 0;
}


int s3c2440_interrupt_unsubmask(int vector)
{
    INTSUBMSK &= ~(1 << vector);
    return 0;
}


extern void s3c2440_interrupt_enable(int);
extern int s3c2440_interrupt_disable();
int s3c2440_interrupt_install_irq(int               vector,
                                  pfn_isr_handler_t p_cb,
                                  void             *p_arg)
{
    if (p_cb == NULL  || vector >= __IRQ_MAX_HANDLE) {
        return -EINVAL;
    }
    __g_irq_desc_tab[vector].handler = p_cb;
    __g_irq_desc_tab[vector].param   = p_arg;
    return 0;
}


void eint_handle()
{
    int level = 0;
    int vector = 0;
    void *p_arg = NULL;
//    level = s3c2440_interrupt_disable();

    /* 清SRCPND和INTPND后会自动清零 */
    vector = INTOFFSET;

    SRCPND   |= (1 << vector);
    INTPND   |= (1 << vector);


    if (__g_irq_desc_tab[vector].handler) {
        __g_irq_desc_tab[vector].handler(vector, p_arg);
    } else {
        //todo
        ;
    }

//    s3c2440_interrupt_enable(level);
}

