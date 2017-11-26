#include "led.h"
#include "clock.h"
#include "uart.h"

int main()
{
    int i = 0;

    unsigned char buf[1024*30];

    clock_init();

    uart_init();

    led_init();

    uart_put_str("hello mini2440\n");

    for (i = 0; i <= LED4; i++) {
        led_on(i);
    }

    for (i = 0; i <= LED4; i++) {
        led_off(i);
    }

    return 0;
}
