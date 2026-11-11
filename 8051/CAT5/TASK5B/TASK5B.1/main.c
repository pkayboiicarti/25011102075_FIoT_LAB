#include <reg51.h>

sbit PWM_LED = P1^0;

/* 100 us ISR tick -> 20 steps = 2 ms period = 500 Hz PWM */
volatile unsigned char pwm_tick = 0;
volatile unsigned char duty_steps = 0;  /* 0..20 (0%..100% in 5% steps) */
volatile unsigned int fade_counter = 0;
bit fade_up = 1;

void timer0_init(void) {
    TMOD = (TMOD & 0xF0) | 0x02; /* Timer 0 Mode 2 (8-bit auto-reload) */
    TH0 = 256 - 92;              /* 92 * 1.085 us = ~100 us */
    TL0 = 256 - 92;
    ET0 = 1;
    EA  = 1;
    TR0 = 1;
}

void timer0_isr(void) interrupt 1 {
    pwm_tick++;
    if (pwm_tick >= 20) {
        pwm_tick = 0;
    }
    PWM_LED = (pwm_tick < duty_steps) ? 1 : 0;

    /* Adjust duty cycle smoothly every 50 ms */
    fade_counter++;
    if (fade_counter >= 500) {
        fade_counter = 0;
        if (fade_up) {
            if (++duty_steps >= 20) fade_up = 0;
        } else {
            if (duty_steps == 0) fade_up = 1;
            else duty_steps--;
        }
    }
}

void main(void) {
    PWM_LED = 0;
    timer0_init();
    while (1) {
        /* Purely interrupt-driven software PWM */
    }
}
