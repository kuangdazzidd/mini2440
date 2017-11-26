/*
 * uart.h
 *
 *  Created on: 2017Äê8ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef BSP_UART_UART_H_
#define BSP_UART_UART_H_

#include "gpio.h"
#include "type.h"

#define U0BASE     (*(volatile unsigned *)0x50000000) //UART 0 Line control
#define ULCON0     (*(volatile unsigned *)0x50000000) //UART 0 Line control
#define UCON0      (*(volatile unsigned *)0x50000004) //UART 0 Control
#define UFCON0     (*(volatile unsigned *)0x50000008) //UART 0 FIFO control
#define UMCON0     (*(volatile unsigned *)0x5000000c) //UART 0 Modem control
#define UTRSTAT0   (*(volatile unsigned *)0x50000010) //UART 0 Tx/Rx status
#define UERSTAT0   (*(volatile unsigned *)0x50000014) //UART 0 Rx error status
#define UFSTAT0    (*(volatile unsigned *)0x50000018) //UART 0 FIFO status
#define UMSTAT0    (*(volatile unsigned *)0x5000001c) //UART 0 Modem status
#define UBRDIV0    (*(volatile unsigned *)0x50000028) //UART 0 Baud ate divisor

#define U1BASE     (*(volatile unsigned *)0x50004000) //UART 1 Line control
#define ULCON1     (*(volatile unsigned *)0x50004000) //UART 1 Line control
#define UCON1      (*(volatile unsigned *)0x50004004) //UART 1 Control
#define UFCON1     (*(volatile unsigned *)0x50004008) //UART 1 FIFO control
#define UMCON1     (*(volatile unsigned *)0x5000400c) //UART 1 Modem control
#define UTRSTAT1   (*(volatile unsigned *)0x50004010) //UART 1 Tx/Rx status
#define UERSTAT1   (*(volatile unsigned *)0x50004014) //UART 1 Rx error status
#define UFSTAT1    (*(volatile unsigned *)0x50004018) //UART 1 FIFO status
#define UMSTAT1    (*(volatile unsigned *)0x5000401c) //UART 1 Modem status
#define UBRDIV1    (*(volatile unsigned *)0x50004028) //UART 1 Baud ate divisor

#define U2BASE     *(volatile unsigned *)0x50008000 //UART 2 Line control
#define ULCON2     (*(volatile unsigned *)0x50008000) //UART 2 Line control
#define UCON2      (*(volatile unsigned *)0x50008004) //UART 2 Control
#define UFCON2     (*(volatile unsigned *)0x50008008) //UART 2 FIFO control
#define UMCON2     (*(volatile unsigned *)0x5000800c) //UART 2 Modem control
#define UTRSTAT2   (*(volatile unsigned *)0x50008010) //UART 2 Tx/Rx status
#define UERSTAT2   (*(volatile unsigned *)0x50008014) //UART 2 Rx error status
#define UFSTAT2    (*(volatile unsigned *)0x50008018) //UART 2 FIFO status
#define UMSTAT2    (*(volatile unsigned *)0x5000801c) //UART 2 Modem status
#define UBRDIV2    (*(volatile unsigned *)0x50008028) //UART 2 Baud ate divisor


#define UTXH0 (*(volatile unsigned char *)0x50000020) //UART 0 Transmission Hold
#define URXH0 (*(volatile unsigned char *)0x50000024) //UART 0 Receive buffer
#define UTXH1 (*(volatile unsigned char *)0x50004020) //UART 1 Transmission Hold
#define URXH1 (*(volatile unsigned char *)0x50004024) //UART 1 Receive buffer
#define UTXH2 (*(volatile unsigned char *)0x50008020) //UART 2 Transmission Hold
#define URXH2 (*(volatile unsigned char *)0x50008024) //UART 2 Receive buffer

#define UART_RX_BUFFER_SIZE     512
#define UART_TX_BUFFER_SIZE     512


typedef struct uartport
{
    volatile uint32_t ulcon;
    volatile uint32_t ucon;
    volatile uint32_t ufcon;
    volatile uint32_t umcon;
    volatile uint32_t ustat;
    volatile uint32_t urxb;
    volatile uint32_t ufstat;
    volatile uint32_t umstat;
    volatile uint32_t utxh;
    volatile uint32_t urxh;
    volatile uint32_t ubrd;
}uartport;



struct serial_int_rx
{
    uint8_t  rx_buffer[UART_RX_BUFFER_SIZE];
    uint32_t out, in;
    int      cnt;
};

struct serial_int_tx
{
    uint8_t  tx_buffer[UART_TX_BUFFER_SIZE];
    uint32_t write_index, save_index;
    int      cnt;
};


struct serial_device
{
    uartport* uart_device;

    /* rx structure */
    struct serial_int_rx* int_rx;

    /* tx structure */
    struct serial_int_tx* int_tx;
};


int s3c2440_uart_init(int brd);
unsigned char s3c2440_uart_get_char();
int s3c2440_uart_put_char(unsigned char c);
int s3c2440_uart_put_str(unsigned char *p_str);
int s3c2440_uart_read(unsigned char *p_buf, int len);


#define SHELL_SYMBLO    "[mini2440]$ "
void ex_kprintf(const char *fmt, ...);

#endif /* BSP_UART_UART_H_ */
