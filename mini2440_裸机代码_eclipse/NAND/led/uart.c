/*
 * uart.c
 *
 *  Created on: 2017��7��3��
 *      Author: Administrator
 */

#include "uart.h"


#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            50000000    // init.c�е�clock_init��������PCLKΪ50MHz
#define UART_CLK        PCLK        //  UART0��ʱ��Դ��ΪPCLK
#define UART_BAUD_RATE  115200      // ������
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

int uart_init()
{
    GPHCON  |= 0xa0;    // GPH2,GPH3����TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3�ڲ�����

    ULCON0 = 0x03;
    UCON0  = 0x05;  /* ��ѯ��ʽ */
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


