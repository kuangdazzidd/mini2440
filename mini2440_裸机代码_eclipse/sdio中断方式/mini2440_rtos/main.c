
#include "clock.h"
#include "uart.h"
#include "led.h"
#include "interrupt.h"
#include "key.h"
#include "dma.h"
#include "unistd.h"
#include "sdio.h"
#include "type.h"
#include "ff.h"
#include "string.h"

extern void soft_mdelay(int m);

extern void s3c2440_interrupt_enable(int);
extern int s3c2440_interrupt_disable();

FRESULT scan_dir(DIR* dp);

uint8_t test_buf[1024 * 1024];

int main()
{
    int i = 0;
    int len = 0;
    int level = 0;
    uint8_t fs_buffer[1024];
    unsigned char uart_buf[100];


    level = s3c2440_interrupt_disable();
    s3c2440_inetrrupt_init();
    s3c2440_uart_init(115200);
    s3c2440_interrupt_enable(level);

    led_init();
    key_init();


//    s3c2440_sd_init();
//    memset(fs_buffer, 0x0, sizeof(fs_buffer));
//    sdcard_write(0, fs_buffer, 2);

    FATFS fs;
    FIL f1;
    FRESULT ret = 0;
    UINT    bw = 0;
    DIR   r_dir;
    FILINFO f_info;

    ret = f_mount(&fs, "0:", 1);
//    ret = f_mkfs("0:", 0, 4096);
//    ret = f_opendir(&r_dir, "");

//    scan_dir(&r_dir);

    ret = f_open(&f1, "test2.txt", FA_OPEN_EXISTING |
                                   FA_WRITE         |
                                   FA__WRITTEN      |
                                   FA_READ);

//    ret = f_read(&f1, fs_buffer, 19, &bw);
    memset(test_buf, 'E', sizeof(test_buf));
    ret = f_write(&f1, test_buf, sizeof(test_buf), &bw);
    ret = f_close(&f1);

//    s3c2440_dma_init();

//    extern unsigned char uart0_dma_buf[512];
//    memset(uart0_dma_buf, 0 , 512);
//    s3c2440_dma_start();
//    uart0_dma_buf[0] = 'z';
//    uart0_dma_buf[1] = 'd';
//    s3c2440_dma_start();


    while (1) {
        led_toggle(LED1);
        soft_mdelay(1000);
        if (i < 10) {
            ex_kprintf(SHELL_SYMBLO"hello mini2440 %5d\r\n", i);
        }
        i++;
//        memset(uart_buf, 0x00, sizeof(uart_buf));
//        len = s3c2440_uart_read(uart_buf, sizeof(uart_buf));
//        if (len > 0) {
//            ex_kprintf(SHELL_SYMBLO"uart0 get:%s \r\n", uart_buf);
//        }
    }

    return 0;
}


FRESULT scan_dir(DIR* dp)
{
    FRESULT    ret;
    int        i = 0;
    static int f_dep = 0;
    FILINFO    f_info;
    while (1) {
        ret = f_readdir(dp, &f_info);
        if (ret != FR_OK || f_info.fname[0] == 0) {
            break;
        }

        if (f_info.fattrib & AM_DIR) {
            f_dep++;
            for (i = 0; i < f_dep; i++) {
                ex_kprintf("    ");
            }
            ex_kprintf("%s\r\n", f_info.fname);
            ret = f_opendir(dp, f_info.fname);
            ret = scan_dir(dp);
            ex_kprintf("#\n");
        } else if (f_info.fattrib & AM_ARC) {
            for (i = 0; i < f_dep; i++) {
                ex_kprintf("    ");
            }
            ex_kprintf("%s\r\n", f_info.fname);
        }
    }

    return FR_OK;
}



void soft_mdelay(int m)
{
   int i, j;
   for (i = 0; i < 100; i++) {
       for (j = 0; j < m; j++) {
           ;
       }
   }
}


