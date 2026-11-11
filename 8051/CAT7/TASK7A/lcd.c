#include <reg51.h>
#include "lcd.h"

sbit LCD_RS = P2^0;
sbit LCD_EN = P2^1;

static void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 112; j++);
}

static void lcd_pulse_en(void) {
    LCD_EN = 1;
    delay_ms(1);
    LCD_EN = 0;
    delay_ms(1);
}

static void lcd_write_nibble(unsigned char nibble) {
    P2 = (P2 & 0x0F) | (nibble & 0xF0);
    lcd_pulse_en();
}

void lcd_cmd(unsigned char cmd) {
    LCD_RS = 0;
    lcd_write_nibble(cmd & 0xF0);
    lcd_write_nibble((cmd << 4) & 0xF0);
    delay_ms(2);
}

void lcd_data(unsigned char dat) {
    LCD_RS = 1;
    lcd_write_nibble(dat & 0xF0);
    lcd_write_nibble((dat << 4) & 0xF0);
    delay_ms(2);
}

void lcd_init(void) {
    delay_ms(20);         /* Power-on stabilization */
    LCD_RS = 0;
    lcd_write_nibble(0x30);
    delay_ms(5);
    lcd_write_nibble(0x30);
    delay_ms(1);
    lcd_write_nibble(0x30);
    lcd_write_nibble(0x20); /* Switch to 4-bit interface */

    lcd_cmd(0x28);        /* 4-bit, 2-line, 5x8 font */
    lcd_cmd(0x0C);        /* Display ON, Cursor OFF */
    lcd_cmd(0x06);        /* Entry mode: increment cursor */
    lcd_cmd(0x01);        /* Clear DDRAM */
}

void lcd_puts(const char *s) {
    while (*s) {
        lcd_data(*s++);
    }
}
