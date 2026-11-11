#include <reg51.h>

sbit WAVE_OUT = P1^1;
sbit MODE_BTN = P3^2;

/* Half-period preload values for 11.0592 MHz (921.6 kHz timer clock):
   Mode 1: 100 Hz -> 5000 us -> 4608 ticks -> 60928 = 0xEE00
   Mode 2: 500 Hz -> 1000 us ->  922 ticks -> 64614 = 0xFC66
   Mode 3:   1 kHz -> 500 us ->  461 ticks -> 65075 = 0xFE33
   Mode 4:   2 kHz -> 250 us ->  230 ticks -> 65306 = 0xFF1A
   Mode 5:   5 kHz -> 100 us ->   92 ticks -> 65444 = 0xFFA4 */
const unsigned char code th_table[5] = {0xEE, 0xFC, 0xFE, 0xFF, 0xFF};
const unsigned char code tl_table[5] = {0x00, 0x66, 0x33, 0x1A, 0xA4};

volatile unsigned char freq_mode = 0;

void timer0_isr(void) interrupt 1 {
    TH0 = th_table[freq_mode];
    TL0 = tl_table[freq_mode];
    WAVE_OUT = ~WAVE_OUT;
}

void delay_debounce(void) {
    unsigned int i;
    for (i = 0; i < 2500; i++);
}

void main(void) {
    WAVE_OUT = 0;
    MODE_BTN = 1;
    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = th_table[0];
    TL0 = tl_table[0];
    ET0 = 1;
    EA  = 1;
    TR0 = 1;

    while (1) {
        if (MODE_BTN == 0) {
            delay_debounce();
            if (MODE_BTN == 0) {
                freq_mode = (freq_mode + 1) % 5;
                while (MODE_BTN == 0);
                delay_debounce();
            }
        }
    }
}
