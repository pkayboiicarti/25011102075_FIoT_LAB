#include <reg51.h>
#include "lcd.h"

void delay_500ms(void) {
    unsigned int i, j;
    for (i = 0; i < 500; i++)
        for (j = 0; j < 112; j++);
}

void main(void) {
    unsigned int counter = 0;
    lcd_init();
    lcd_goto(0, 0);
    lcd_puts("AT89S52 Counter:");

    while (1) {
        lcd_goto(1, 0);
        lcd_puts("Value: ");
        lcd_print_number(counter);
        lcd_puts("    ");
        counter++;
        delay_500ms();
    }
}
