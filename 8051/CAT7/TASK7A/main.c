#include <reg51.h>
#include "lcd.h"

void main(void) {
    lcd_init();
    lcd_cmd(0x80);            /* Row 1, Col 0 */
    lcd_data('A');
    lcd_puts("T89S52 4-Bit");
    lcd_cmd(0xC0);            /* Row 2, Col 0 */
    lcd_puts("16x2 LCD Ready!");
    while (1);
}
