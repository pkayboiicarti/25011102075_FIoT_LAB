#ifndef LCD_H
#define LCD_H

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char dat);
void lcd_goto(unsigned char row, unsigned char col);

#endif
