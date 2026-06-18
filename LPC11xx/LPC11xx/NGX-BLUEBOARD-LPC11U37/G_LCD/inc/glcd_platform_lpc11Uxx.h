#ifndef  _GLCD_PLATFORM_LPC1768_H_
#define  _GLCD_PLATFORM_LPC1768_H_
#if 0
#define LCD_DATA_DIR      LPC_GPIO2->DIR
#define LCD_DATA_SET      LPC_GPIO2->DATA
#define LCD_DATA_CLR      LPC_GPIO2->DATA


#define LCD_CTRL_DIR      LPC_GPIO1->DIR
#define LCD_CTRL_SET      LPC_GPIO1->DATA
#define LCD_CTRL_CLR      LPC_GPIO1->DATA


#endif


#define LCD_DATA_DIR      LPC_GPIO->DIR[1]
#define LCD_DATA_SET      LPC_GPIO->PIN[1]
#define LCD_DATA_CLR      LPC_GPIO->PIN[1]
#define LCD_DATA_PIN      LPC_GPIO->PIN[1]


#define LCD_CTRL_DIR      LPC_GPIO->DIR[0]
#define LCD_CTRL_SET      LPC_GPIO->PIN[0]
#define LCD_CTRL_CLR      LPC_GPIO->PIN[0]
#define LCD_CTRL_PIN      LPC_GPIO->PIN[0]

 
#define LCD_CTRL_CS_DIR   LPC_GPIO->DIR[1]
#define LCD_CTRL_CS_SET   LPC_GPIO->PIN[1]
#define LCD_CTRL_CS_CLR   LPC_GPIO->PIN[1]

#define LCD_CTRL_RST_DIR  LPC_GPIO->DIR[0]
#define LCD_CTRL_RST_SET  LPC_GPIO->PIN[0]
#define LCD_CTRL_RST_CLR  LPC_GPIO->PIN[0]

#define LCD_CTRL_BL_DIR   LPC_GPIO->DIR[1]
#define LCD_CTRL_BL_SET   LPC_GPIO->PIN[1]
#define LCD_CTRL_BL_CLR   LPC_GPIO->PIN[1]






#define LCD_D0    1 << 19
#define LCD_D1    1 << 20
#define LCD_D2    1 << 21
#define LCD_D3    1 << 22
#define LCD_D4    1 << 26
#define LCD_D5    1 << 27
#define LCD_D6    1 << 28
#define LCD_D7    1U <<31


  
#define LCD_BUSY_FLAG   LCD_D7                  /* Enables LCD_BUSY_FLAG */

#define LCDRST    (1 << 0)
#define LCDCS1B   (1 << 13)
#define LCDRS     (1 << 14)
#define LCDRW     (1 << 13)
#define LCDEN     (1 << 12)
#define LCDBL     (1 << 14)

#define GLCD_DATA_MASK_LOWER_NIBBLE  (LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 )
#define GLCD_DATA_MASK_HIGHER_NIBBLE  (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7 )
#define GLCD_DATA_MASK (GLCD_DATA_MASK_LOWER_NIBBLE | GLCD_DATA_MASK_HIGHER_NIBBLE)

#define LCD_CTRL_MASK   (LCDRW | LCDRS)
#define LCD_CS  LCDCS1B
#define LCD_RST  LCDRST
#define LCD_BL   LCDBL
#define HIGH    1
#define LOW     0

void lcd_write_cmd(unsigned char cmd);
void lcd_write_data(unsigned char data);
void lcd_set_page(unsigned char page,unsigned char col);
void init_glcd_ports(void);
#endif //_64x128CLCD_H_

