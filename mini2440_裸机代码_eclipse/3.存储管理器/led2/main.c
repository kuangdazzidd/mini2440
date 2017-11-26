#include "led.h"



void delay_ms(int ms)
{
    int i;
    while(ms--) {
        i = 2500;
        while(i--);
    }
}


int main()
{
    int i  = 0;
    int sw = 0;

    led_init();

    while(1) {
        sw == 0 ? led_on(i) : led_off(i);
        delay_ms(1000);
        i++;
        if (i > LED4) {
            i = 0;
            sw = !sw;
        }
    }

    return 0;
}
