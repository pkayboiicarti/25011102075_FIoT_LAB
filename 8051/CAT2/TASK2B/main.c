#include <reg51.h>
#include "delay.h"

sbit LED = P1^0;

void main(void) {
    P1 = 0x00;
    while (1) {
        LED = 1;
        delay_ms(500);
        LED = 0;
        delay_ms(500);
    }
}
