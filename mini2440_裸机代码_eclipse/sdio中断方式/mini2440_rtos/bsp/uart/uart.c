/*
 * uart.c
 *
 *  Created on: 2017年8月5日
 *      Author: Administrator
 */

#include <uart/uart.h>
#include "clock.h"
#include "interrupt.h"
#include "string.h"

#define __TXD0READY   (1<<2)
#define __RXD0READY   (1)

#define __UART_RX_FIFO_SIZE     (64)
#define __UART_TX_FIFO_SIZE     (64)

#define UART_CLK        PCLK        //  UART0的时钟源设为PCLK
//#define UART_BAUD_RATE  115200      // 波特率
#define UART_BRD(x)        ((UART_CLK  / (x * 16)) - 1)


#define TXD    S3C2440_GPH(2)
#define RXD    S3C2440_GPH(3)


struct serial_config {
    uint8_t length;
#define __LENGTH_5      0x00
#define __LENGTH_6      0x01
#define __LENGTH_7      0x02
#define __LENGTH_8      0x03

    uint8_t stop_bit;
#define __STOP_0        0x00
#define __STOP_1        0x01

    uint8_t check_mode;
#define __CKMD_NO       0x00
#define __CKMD_ODD      0x04
#define __CKMD_EVEN     0x05
#define __CKMD_ODD1     0x06
#define __CKMD_ODD0     0x07

    bool_t  ir_mode;
};


struct serial_config uart0_cfg =
{
    __LENGTH_8,
    __STOP_0,
    __CKMD_NO,
    FALSE
};



#define UART0   ((struct uartport *)&U0BASE)
struct serial_int_rx rx_buf;

struct serial_device uart0 =
{
    UART0,
    &rx_buf,
    NULL
};

void isr_eint_uart0(int vector, void *param);
void __isr_uart0_rx();
void __isr_uart0_tx();

int s3c2440_uart_init(int brd)
{
    /* GPH2,GPH3用作TXD0,RXD0 */
    s3c2440_gpio_cfgpin(RXD, S3C2440_GPIO_SFN2);
    s3c2440_gpio_cfgpin(TXD, S3C2440_GPIO_SFN2);

    /* GPH2,GPH3内部上拉 */
    s3c2440_gpio_pullup(RXD, 1);
    s3c2440_gpio_pullup(TXD, 1);

    memset(&rx_buf, 0x0, sizeof(rx_buf));

    uart0.uart_device->ulcon = (uart0_cfg.length     << 0) |
                               (uart0_cfg.stop_bit   << 2) |
                               (uart0_cfg.check_mode << 3) |
                               (uart0_cfg.ir_mode    << 6);

    /* 查询或者中断方式 + 开启接收超时*/
//    uart0.uart_device->ucon  = 0x05 | (1 << 7);
    uart0.uart_device->ucon  = (1 << 0) | (2 << 2) | (1 << 7);

    /* 使能接收FIFO(8字节触发) +  */
    uart0.uart_device->ufcon = (1 << 0) | (1 << 4);
    uart0.uart_device->umcon = 0x00;
    uart0.uart_device->ubrd  = UART_BRD(brd);


    /* 绑定中断回调函数 */
    s3c2440_interrupt_install_irq(INTUART0, isr_eint_uart0, NULL);

    /* 取消屏蔽 */
    s3c2440_interrupt_unmask(INTUART0);
    /* 开启接收中断 */
    s3c2440_interrupt_unsubmask(INTSUB_RXD0);
//    s3c2440_interrupt_unsubmask(INTSUB_TXD0);

    return 0;
}


unsigned char s3c2440_uart_get_char()
{
    while (!(UTRSTAT0 & __RXD0READY));
    return URXH0;
}


int s3c2440_uart_put_char(unsigned char c)
{
    while (!(UTRSTAT0 & __TXD0READY));
    UTXH0 = c;
    return 0;
}

int s3c2440_uart_put_str(unsigned char *p_str)
{
    while(*p_str != '\0') {
        s3c2440_uart_put_char(*p_str);
        p_str++;
    }

    return 0;
}



int s3c2440_uart_read(unsigned char *p_buf, int len)
{
    int cnt = 0;
    if (uart0.int_rx->in == uart0.int_rx->out) {
        return -1;
    }

    cnt = uart0.int_rx->in - uart0.int_rx->out;
    len = (len < cnt) ? len : cnt;
    uart0.int_rx->out += len;
    memcpy(p_buf, uart0.int_rx->rx_buffer, len);
    return len;
}


void isr_eint_uart0(int vector, void *param)
{
    if (SUBSRCPND & (1 << INTSUB_RXD0)) {
        __isr_uart0_rx();
        SUBSRCPND |= (1 << INTSUB_RXD0);
    } else if (SUBSRCPND & (1 << INTSUB_TXD0)) {
        __isr_uart0_tx();
        SUBSRCPND |= (1 << INTSUB_TXD0);
    }

}


/* 串口接收中断 */
void __isr_uart0_rx()
{
    int i = 0;
    int fifo_count = 0;

    /* FIFO满了 */
    if( UFSTAT0 & (1<<6) ){
        fifo_count = __UART_RX_FIFO_SIZE;
        ex_kprintf(SHELL_SYMBLO"UART0 BUFFER OVERFLOW.\r\n");
    } else {
        fifo_count = ((UFSTAT0 & (0x3f)) >> 0);
    }

    for(i = 0; i < fifo_count; i++){
        uart0.int_rx->rx_buffer[uart0.int_rx->in++] = URXH0;
        uart0.int_rx->cnt++;
        if (uart0.int_rx->in >= UART_TX_BUFFER_SIZE) {
            uart0.int_rx->in = 0;
        }
    }
}


/* 串口发送中断 */
void __isr_uart0_tx()
{

}

