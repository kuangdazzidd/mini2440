#include "led.h"

int main()
{
    int i = 0;

    led_init();

    for (i = 0; i <= LED4; i++) {
        led_on(i);
    }

    for (i = 0; i <= LED4; i++) {
        led_off(i);
    }

    return 0;
}
