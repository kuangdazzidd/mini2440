#include "led.h"
#include "clock.h"
#include "uart.h"
#include "mmu.h"
#include "key.h"
#include "nand.h"

#define nop()   __asm __volatile__ ("nop \n\t");

#define __SYS_MEM_ADDR_START 0x30000000

void cp15_mmu_init();


static unsigned char buf[2048] = {"hello nand_flash test !"};

static int j = 0;
int main()
{
    int i = 0;

    clock_init();

    uart_init();

    led_init();

    key_init();

    nand_init();

    uart_put_str("hello mini2440\n");

//    nand_erase_block(0);

//    for (j = 0; j < 2048 / 32; j++) {
//        ((unsigned long *)buf)[j] = (((unsigned long *)__SYS_MEM_ADDR_START)[j]);
//    }

//    nand_write(0, buf, sizeof(buf));

//    nand_read(buf, 0, 2048);

//    for (i = 0; i <= LED4; i++) {
//        led_on(i);
//    }
//
//    for (i = 0; i <= LED4; i++) {
//        led_off(i);
//    }

//    for (j = 0; j < 20; j++) {
//        ((unsigned long *)0x0)[j] = ((unsigned long *)__SYS_MEM_ADDR_START)[j];
//    }

    while(1);

//    mmu_init();
//    mmu_start();

//    for (i = 0; i <= LED4; i++) {
//        mmu_led_on(i);
//    }

//    __asm__ __volatile__(
//    "ldr pc, =0x30000018"
//    );
    return 0;
}

