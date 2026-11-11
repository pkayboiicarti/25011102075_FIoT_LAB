#ifndef LCD_H
#define LCD_H

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char dat);
void lcd_puts(const char *s);

#endif
