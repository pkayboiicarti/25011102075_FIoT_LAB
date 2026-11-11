#include <reg51.h>
#include "lcd.h"

static void debounce(void) {
    unsigned int i;
    for (i = 0; i < 2500; i++);
}

/* Scans 4x4 keypad on Port 1 (P1.0-P1.3 rows, P1.4-P1.7 cols) */
unsigned char keypad_scan(void) {
    unsigned char r, col;
    const unsigned char code keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    for (r = 0; r < 4; r++) {
        P1 = 0xFF & ~(1 << r);
        col = (P1 >> 4) & 0x0F;
        if (col != 0x0F) {
            debounce();
            col = (P1 >> 4) & 0x0F;
            if (col != 0x0F) {
                unsigned char c = 0;
                if (!(col & 0x01)) c = 0;
                else if (!(col & 0x02)) c = 1;
                else if (!(col & 0x04)) c = 2;
                else if (!(col & 0x08)) c = 3;
                while (((P1 >> 4) & 0x0F) != 0x0F);
                return keys[r][c];
            }
        }
    }
    return 0;
}

void main(void) {
    unsigned char row = 0, col = 0, key;
    P1 = 0xFF;
    lcd_init();
    lcd_goto(row, col);
    lcd_data('*');

    while (1) {
        key = keypad_scan();
        if (key == '2' || key == '8' || key == '4' || key == '6') {
            lcd_goto(row, col);
            lcd_data(' '); /* Erase previous character */

            if (key == '2' && row > 0) row--;
            else if (key == '8' && row < 1) row++;
            else if (key == '4' && col > 0) col--;
            else if (key == '6' && col < 15) col++;

            lcd_goto(row, col);
            lcd_data('*');
        }
    }
}
