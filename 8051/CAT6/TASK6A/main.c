#include <reg51.h>

void uart_init(void) {
    TMOD = (TMOD & 0x0F) | 0x20; /* Timer 1, Mode 2 (8-bit auto-reload) */
    TH1  = 0xFD;                 /* 9600 baud @ 11.0592 MHz, SMOD=0 */
    TL1  = 0xFD;
    SCON = 0x50;                 /* Mode 1 (8-bit UART, variable baud), REN=1 */
    TR1  = 1;                    /* Start Timer 1 */
}

void uart_tx_char(char c) {
    SBUF = c;
    while (TI == 0);
    TI = 0;
}

void uart_tx_string(const char *str) {
    while (*str) {
        uart_tx_char(*str++);
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 112; j++);
}

void main(void) {
    uart_init();
    while (1) {
        uart_tx_string("Hello from 8051\r\n");
        delay_ms(1000);
    }
}
