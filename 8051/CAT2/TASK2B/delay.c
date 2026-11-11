#include "delay.h"

/* Approximate 1 ms software delay calibrated for 11.0592 MHz 12-clock 8051 */
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 112; j++) {
            /* 1 ms inner loop */
        }
    }
}

/* Short microsecond busy-wait helper */
void delay_us(unsigned int us) {
    while (us--) {
        /* ~1-2 us per iteration overhead */
    }
}
