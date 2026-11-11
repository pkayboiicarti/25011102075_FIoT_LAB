#include <reg51.h>

sbit LED = P1^0;
volatile unsigned int ms_ticks = 0;

/* 11.0592 MHz -> T_machine = 1.085 us -> 922 counts = 1 ms -> 65536 - 922 = 0xFC66 */
void timer0_init(void) {
    TMOD = (TMOD & 0xF0) | 0x01; /* Timer 0, Mode 1 (16-bit) */
    TH0 = 0xFC;
    TL0 = 0x66;
    ET0 = 1;                     /* Enable Timer 0 interrupt */
    EA  = 1;                     /* Enable global interrupts */
    TR0 = 1;                     /* Start Timer 0 */
}

void timer0_isr(void) interrupt 1 {
    TH0 = 0xFC;
    TL0 = 0x66;
    ms_ticks++;
    if (ms_ticks >= 1000) {
        ms_ticks = 0;
        LED = ~LED;              /* Toggle every 1.000 second */
    }
}

void main(void) {
    LED = 0;
    timer0_init();
    while (1) {
        /* Non-blocking main loop: hardware timer drives timing */
    }
}
