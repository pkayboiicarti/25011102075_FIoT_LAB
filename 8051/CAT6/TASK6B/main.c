#include <reg51.h>

#define RX_BUF_SIZE 32

volatile char rx_buf[RX_BUF_SIZE];
volatile unsigned char rx_head = 0;
volatile unsigned char rx_tail = 0;
volatile bit tx_busy = 0;

void uart_init(void) {
    TMOD = (TMOD & 0x0F) | 0x20; /* Timer 1 Mode 2 */
    TH1  = 0xFD;                 /* 9600 baud @ 11.0592 MHz */
    TL1  = 0xFD;
    SCON = 0x50;                 /* 8-bit UART, REN=1 */
    TR1  = 1;
    ES   = 1;                    /* Enable serial interrupt */
    EA   = 1;                    /* Enable global interrupts */
}

void uart_isr(void) interrupt 4 {
    if (RI) {
        unsigned char next_head = (rx_head + 1) % RX_BUF_SIZE;
        char ch = SBUF;
        RI = 0;
        if (next_head != rx_tail) {
            rx_buf[rx_head] = ch;
            rx_head = next_head;
        }
    }
    if (TI) {
        TI = 0;
        tx_busy = 0;
    }
}

void uart_send_char(char c) {
    while (tx_busy);
    tx_busy = 1;
    SBUF = c;
}

void main(void) {
    uart_init();
    while (1) {
        if (rx_tail != rx_head) {
            char c = rx_buf[rx_tail];
            rx_tail = (rx_tail + 1) % RX_BUF_SIZE;
            uart_send_char(c);   /* Echo character back to PuTTY */
        }
    }
}
