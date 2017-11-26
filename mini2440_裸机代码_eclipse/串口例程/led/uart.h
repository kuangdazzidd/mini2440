/*
 * uart.h
 *
 *  Created on: 2017Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_


#define ULCON0      (*(volatile unsigned long*)0x50000000)
#define UCON0       (*(volatile unsigned long*)0x50000004)
#define UFCON0      (*(volatile unsigned long*)0x50000008)
#define UMCON0      (*(volatile unsigned long*)0x5000000C)
#define UBRDIV0     (*(volatile unsigned long*)0x50000028)
#define UTRSTAT0    (*(volatile unsigned long*)0x50000010)
#define UTXH0       (*(volatile unsigned char*)0x50000020)
#define URXH0       (*(volatile unsigned char*)0x50000024)
#define GPHCON      (*(volatile unsigned char*)0x56000070)
#define GPHUP       (*(volatile unsigned char*)0x56000078)


int uart_init();
unsigned char uart_get_char();
int uart_put_char(unsigned char c);
int uart_put_str(unsigned char *p_str);

#endif /* UART_H_ */


