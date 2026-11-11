#include <reg51.h>

sbit LED = P1^0;

/* Simple software delay tuned for ~500 ms on 11.0592 MHz 8051 (~1 Hz blink) */
void delay_500ms(void) {
    unsigned int i, j;
    for (i = 0; i < 500; i++) {
        for (j = 0; j < 92; j++) {
            /* Busy wait */
        }
    }
}

void main(void) {
    P1 = 0x00;  /* Configure Port 1 latches low */
    while (1) {
        LED = 1;
        delay_500ms();
        LED = 0;
        delay_500ms();
    }
}
