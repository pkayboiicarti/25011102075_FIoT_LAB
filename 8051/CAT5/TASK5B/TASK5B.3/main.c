#include <reg51.h>

sbit SIG_OUT = P1^1;
sbit WAVE_BTN = P3^2;

/* 4 waveforms of 24 bit-slots each (MSB first) */
const unsigned long code waveforms[4] = {
    0xAAAAAAUL, /* 1. Square Wave:        101010101010101010101010 */
    0xF0F0F0UL, /* 2. Variable Duty Wave: 111100001111000011110000 */
    0x802008UL, /* 3. Pulse Train:        100000000010000000001000 */
    0xAA00AAUL  /* 4. Burst Signal:       10101010 00000000 10101010 */
};

volatile unsigned char wave_mode = 0;
volatile unsigned char bit_idx = 0;

void timer0_isr(void) interrupt 1 {
    TH0 = 0xFE;
    TL0 = 0x33; /* 500 us slot time @ 11.0592 MHz */
    SIG_OUT = (waveforms[wave_mode] >> (23 - bit_idx)) & 0x01;
    bit_idx = (bit_idx + 1) % 24;
}

void delay_debounce(void) {
    unsigned int i;
    for (i = 0; i < 2500; i++);
}

void main(void) {
    SIG_OUT = 0;
    WAVE_BTN = 1;
    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = 0xFE;
    TL0 = 0x33;
    ET0 = 1;
    EA  = 1;
    TR0 = 1;

    while (1) {
        if (WAVE_BTN == 0) {
            delay_debounce();
            if (WAVE_BTN == 0) {
                wave_mode = (wave_mode + 1) & 0x03;
                bit_idx = 0;
                while (WAVE_BTN == 0);
                delay_debounce();
            }
        }
    }
}
