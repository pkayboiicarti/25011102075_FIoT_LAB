#include <reg51.h>

/* Common-cathode 7-segment codes for digits 0-9 connected to Port 0 */
const unsigned char code seg_table[10] = {
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F  /* 9 */
};

unsigned char active_mode = 0xFF;
unsigned char pattern_step = 0;

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 112; j++);
}

/* Prototypes required by specification */
unsigned char keypad_scan(void);
void display_digit(unsigned char value);
void led_control(unsigned char key);
void running_led(void);
void binary_counter(void);
void alternate_led(void);
void blink_all(void);

/* Scans 4x4 keypad on Port 2 (P2.0-P2.3 rows, P2.4-P2.7 cols) */
unsigned char keypad_scan(void) {
    unsigned char row, col_val;
    const unsigned char code keymap[4][4] = {
        {1, 2, 3, 0xFF},
        {4, 5, 6, 0xFF},
        {7, 8, 9, 0xFF},
        {0xFF, 0, 0xFF, 0xFF}
    };

    for (row = 0; row < 4; row++) {
        P2 = 0xFF & ~(1 << row);  /* Pull one row LOW */
        col_val = (P2 >> 4) & 0x0F;
        if (col_val != 0x0F) {
            delay_ms(20);         /* Debounce */
            col_val = (P2 >> 4) & 0x0F;
            if (col_val != 0x0F) {
                unsigned char col = 0;
                if (!(col_val & 0x01)) col = 0;
                else if (!(col_val & 0x02)) col = 1;
                else if (!(col_val & 0x04)) col = 2;
                else if (!(col_val & 0x08)) col = 3;
                while (((P2 >> 4) & 0x0F) != 0x0F);
                return keymap[row][col];
            }
        }
    }
    return 0xFF;
}

void display_digit(unsigned char value) {
    if (value <= 9) {
        P0 = seg_table[value];
    }
}

void running_led(void) {
    P1 = (0x01 << (pattern_step & 0x03));
    pattern_step = (pattern_step + 1) & 0x03;
    delay_ms(200);
}

void alternate_led(void) {
    P1 = (pattern_step & 0x01) ? 0x05 : 0x0A;
    pattern_step++;
    delay_ms(250);
}

void binary_counter(void) {
    P1 = pattern_step & 0x0F;
    pattern_step = (pattern_step + 1) & 0x0F;
    delay_ms(250);
}

void blink_all(void) {
    P1 = (pattern_step & 0x01) ? 0x0F : 0x00;
    pattern_step++;
    delay_ms(250);
}

void led_control(unsigned char key) {
    switch (key) {
        case 1: P1 = 0x01; break;
        case 2: P1 = 0x02; break;
        case 3: P1 = 0x04; break;
        case 4: P1 = 0x08; break;
        case 5: running_led(); break;
        case 6: alternate_led(); break;
        case 7: binary_counter(); break;
        case 8: blink_all(); break;
        case 9: P1 = 0x00; break;
        case 0:
            P1 = 0x00;
            display_digit(0);
            pattern_step = 0;
            break;
        default: break;
    }
}

void main(void) {
    unsigned char key;
    P0 = 0x3F;  /* Show 0 initially */
    P1 = 0x00;  /* All LEDs OFF */
    P2 = 0xFF;

    while (1) {
        key = keypad_scan();
        if (key != 0xFF) {
            active_mode = key;
            pattern_step = 0;
            display_digit(key);
        }
        if (active_mode != 0xFF) {
            led_control(active_mode);
        }
    }
}
