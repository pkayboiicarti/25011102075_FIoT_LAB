#include <reg51.h>
#include "lcd.h"

sbit LCD_RS = P2^0;
sbit LCD_EN = P2^1;

static void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 112; j++);
}

static void lcd_write_nibble(unsigned char nibble) {
    P2 = (P2 & 0x0F) | (nibble & 0xF0);
    LCD_EN = 1;
    delay_ms(1);
    LCD_EN = 0;
    delay_ms(1);
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
    delay_ms(20);
    LCD_RS = 0;
    lcd_write_nibble(0x30); delay_ms(5);
    lcd_write_nibble(0x30); delay_ms(1);
    lcd_write_nibble(0x30);
    lcd_write_nibble(0x20);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_goto(unsigned char row, unsigned char col) {
    lcd_cmd((row == 0) ? (0x80 + (col & 0x0F)) : (0xC0 + (col & 0x0F)));
}
