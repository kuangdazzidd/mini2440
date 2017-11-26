/*
 * uart.c
 *
 *  Created on: 2017年7月3日
 *      Author: Administrator
 */

#include "uart.h"


#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            50000000    // init.c中的clock_init函数设置PCLK为50MHz
#define UART_CLK        PCLK        //  UART0的时钟源设为PCLK
#define UART_BAUD_RATE  115200      // 波特率
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

int uart_init()
{
    GPHCON  |= 0xa0;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3内部上拉

    ULCON0 = 0x03;
    UCON0  = 0x05;  /* 查询方式 */
    UFCON0 = 0x00;
    UMCON0 = 0x00;
    UBRDIV0 = UART_BRD;
}


unsigned char uart_get_char()
{
    while (!(UTRSTAT0 & RXD0READY));
    return URXH0;
}


int uart_put_char(unsigned char c)
{
    while (!(UTRSTAT0 & TXD0READY));
    UTXH0 = c;
    return 0;
}

int uart_put_str(unsigned char *p_str)
{
    while(*p_str != '\0') {
        uart_put_char(*p_str);
        p_str++;
    }

    return 0;
}


