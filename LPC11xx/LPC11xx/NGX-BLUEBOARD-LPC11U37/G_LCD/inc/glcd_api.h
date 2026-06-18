#ifndef _GLCD_API_H_
#define _GLCD_API_H_

#define   PAGE0 0xB0
#define   COL0  0
void pin_function_set(void);
void back_light_on(void);
void back_light_off(void);
void glcd_delay(int count);
void init_glcd(void);
void glcd_clear(void);
void init_xy_axis(int y,int x);
void lcd_display_char(unsigned char charecter);
void lcd_display_string(unsigned char *str);
#endif
