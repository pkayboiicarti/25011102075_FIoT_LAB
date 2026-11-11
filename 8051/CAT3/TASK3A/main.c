#include <reg51.h>

sbit BTN = P3^2;  /* Push-button (active-low with pull-up) */

unsigned char mode = 0;
unsigned char step = 0;
unsigned int tick_ms = 0;

void timer0_init(void) {
    TMOD = (TMOD & 0xF0) | 0x01;  /* Timer 0 Mode 1 (16-bit) */
    TH0 = 0xFC;
    TL0 = 0x66;                   /* 1 ms preload @ 11.0592 MHz */
    ET0 = 1;
    EA  = 1;
    TR0 = 1;
}

void timer0_isr(void) interrupt 1 {
    TH0 = 0xFC;
    TL0 = 0x66;
    tick_ms++;
}

void debounce_delay(void) {
    unsigned int i;
    for (i = 0; i < 3000; i++);   /* ~25 ms debounce */
}

void check_button(void) {
    if (BTN == 0) {
        debounce_delay();
        if (BTN == 0) {
            mode = (mode % 4) + 1; /* Cycle Modes 1 -> 2 -> 3 -> 4 -> 1 */
            step = 0;
            while (BTN == 0);      /* Wait for release: 1 press = 1 transition */
            debounce_delay();
        }
    }
}

void main(void) {
    P1 = 0x00;  /* All 4 LEDs (P1.0-P1.3) initially OFF */
    BTN = 1;    /* Configure P3.2 as input */
    timer0_init();

    while (1) {
        check_button();

        if (tick_ms >= 250) {
            tick_ms = 0;
            switch (mode) {
                case 0:
                    P1 = 0x00;
                    break;
                case 1:  /* Mode 1: Running LED (LED1 -> LED2 -> LED3 -> LED4) */
                    P1 = (0x01 << (step & 0x03));
                    step = (step + 1) & 0x03;
                    break;
                case 2:  /* Mode 2: 4-bit Binary Counter (0000 -> 1111) */
                    P1 = step & 0x0F;
                    step = (step + 1) & 0x0F;
                    break;
                case 3:  /* Mode 3: Alternating Pattern (1010 <-> 0101) */
                    P1 = (step & 0x01) ? 0x05 : 0x0A;
                    step++;
                    break;
                case 4:  /* Mode 4: All LED Flash (1111 <-> 0000) */
                    P1 = (step & 0x01) ? 0x00 : 0x0F;
                    step++;
                    break;
            }
        }
    }
}
