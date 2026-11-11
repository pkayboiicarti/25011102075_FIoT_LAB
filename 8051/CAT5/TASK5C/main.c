#include <reg51.h>

sbit BTN_SPEED = P3^2;
sbit BTN_DIR   = P3^3;

/* Wave-drive sequence: 1000 -> 0100 -> 0010 -> 0001 on upper nibble P1.7..P1.4 */
const unsigned char code step_seq[4] = {0x80, 0x40, 0x20, 0x10};
/* Speeds: Slow = 100 ms, Medium = 50 ms, Fast = 10 ms */
const unsigned int code speed_ms[3] = {100, 50, 10};

volatile unsigned char speed_idx = 0;
volatile bit dir_cw = 1;
volatile unsigned char step_idx = 0;
volatile unsigned int ms_count = 0;

void timer0_isr(void) interrupt 1 {
    TH0 = 0xFC;
    TL0 = 0x66; /* 1 ms tick */
    ms_count++;
    if (ms_count >= speed_ms[speed_idx]) {
        ms_count = 0;
        if (dir_cw) {
            step_idx = (step_idx + 1) & 0x03;
        } else {
            step_idx = (step_idx == 0) ? 3 : (step_idx - 1);
        }
        P1 = (P1 & 0x0F) | step_seq[step_idx];
    }
}

void debounce(void) {
    unsigned int i;
    for (i = 0; i < 2500; i++);
}

void main(void) {
    P1 = 0x00;
    BTN_SPEED = 1;
    BTN_DIR = 1;

    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = 0xFC;
    TL0 = 0x66;
    ET0 = 1;
    EA  = 1;
    TR0 = 1;

    while (1) {
        if (BTN_SPEED == 0) {
            debounce();
            if (BTN_SPEED == 0) {
                speed_idx = (speed_idx + 1) % 3;
                while (BTN_SPEED == 0);
                debounce();
            }
        }
        if (BTN_DIR == 0) {
            debounce();
            if (BTN_DIR == 0) {
                dir_cw = ~dir_cw;
                while (BTN_DIR == 0);
                debounce();
            }
        }
    }
}
