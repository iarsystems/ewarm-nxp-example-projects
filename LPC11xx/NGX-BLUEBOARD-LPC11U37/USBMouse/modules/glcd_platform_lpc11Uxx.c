#include "includes.h"
#include "glcd_platform_lpc11Uxx.h"
#include "glcd_8x8.h"
#include "glcd_api.h"

/**
******************************************************************************
  Function Name : glcd_delay()
  Description   : This function suspends the tasks for specified ticks.
  Input         : ticks: no of ticks in multiple of 1 usec
                  task : task to be suspended
  Output        : void
  Note          :
*******************************************************************************
*/

void glcd_delay(int count)
{
int j=0,i=0;

  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}

 /**
*******************************************************************
  Funtion Name : pin_function_set()
  Description  :
  Input        :
  Output       : None
  Note         :
*****************************************************************
*/
void pin_function_set()
{
  SYSAHBCLKCTRL |=(1<<16) ;
  IOCON_TMS_PIO0_12 |= 0x1;
  IOCON_TDO_PIO0_13 |= 0x1;
  IOCON_TRST_PIO0_14 |= 0x1;
}

/**
********************************************************************************************
  Function Name : lcd_write_cmd()
  Description   : writes particular command byte to lcd
  Input         : Void
  Output        : Void
  Note          :
********************************************************************************************
*/

void lcd_write_cmd(unsigned char command)
{
unsigned int temp1=0, temp2=0, temp3=0;

  temp1 = command & 0x0F;
  temp1=temp1<<19;
  temp2 = command & 0x70;
  temp2 = temp2 <<22;
  temp3= command&0x80;
  temp3= temp3<<24;


  LCD_CTRL_CLR &= ~(LCDRS);
  LCD_CTRL_CS_CLR &= ~(LCDCS1B);
  LCD_CTRL_CLR &= ~(LCDRW);
  LCD_DATA_CLR &= ~(GLCD_DATA_MASK);
  LCD_DATA_SET |= temp1 | temp2 |temp3;
  glcd_delay(10);
  LCD_CTRL_SET |= LCDRW;
  LCD_CTRL_CS_SET |= LCDCS1B;
  LCD_CTRL_SET |= LCDRS;
}

/**
********************************************************************************************
  Function Name : lcd_write_cmd()
  Description   : writes particular data byte to lcd
  Input         : Void
  Output        : Void
  Note          :
********************************************************************************************
*/
void lcd_write_data(unsigned char data)
{
unsigned int temp1=0, temp2=0, temp3=0;

  temp1 = data & 0x0F;
  temp1=temp1<<19;
  temp2 = data & 0x70;
  temp2 = temp2 <<22;
  temp3= data &0x80;
  temp3=temp3<<24;

  LCD_CTRL_SET |= LCDRS;
  LCD_CTRL_CS_CLR &= ~(LCDCS1B);
  LCD_CTRL_CLR &= ~(LCDRW);
  LCD_DATA_CLR &= ~(GLCD_DATA_MASK);
  LCD_DATA_SET |= temp1 | temp2 | temp3;
  glcd_delay(10);
  LCD_CTRL_SET |= LCDRW;
  LCD_CTRL_CS_SET |= LCDCS1B;
  LCD_CTRL_CLR &= ~(LCDRS);
}

/**
********************************************************************************************
  Function Name : init_glcd()
  Description   : Initialising Graphic lcd
  Input         : Void
  Output        : Void
  Note          :
********************************************************************************************
*/
void init_glcd(void)
{
  pin_function_set();
  init_glcd_ports();

  LCD_CTRL_CLR &= ~(LCDRST);
  LCD_CTRL_CLR = LCDBL;
  LCD_CTRL_SET |= LCDRST;
  glcd_clear();

  lcd_write_cmd(0xE2);          /*  S/W RESWT               */
  lcd_write_cmd(0xA0);          /*  ADC select              */
  lcd_write_cmd(0xC8);          /*  SHL Normal              */
  lcd_write_cmd(0xA3);          /*  LCD bias                */
  lcd_write_cmd(0x2F);          /*  Power control           */
  lcd_write_cmd(0x22);          /*  reg resistor select     */
  lcd_write_cmd(0x40);          /*  Initial display line 40 */
  lcd_write_cmd(0xA4);          /*  Normal display          */
  lcd_write_cmd(0xA6);          /*  Reverce display a7      */
  lcd_write_cmd(0x81);          /*  Ref vg select mode      */
  lcd_write_cmd(0x3f);          /*  Ref vg reg select       */
  lcd_write_cmd(0xB0);          /*  Set page address        */
  lcd_write_cmd(0x10);          /*  Set coloumn addr  MSB   */
  lcd_write_cmd(0x00);          /*  Set coloumn addr LSB    */
  lcd_write_cmd(0xAF);          /*  Display ON              */
}


/**
********************************************************************************************
  Function Name : lcd_set_page()
  Description   : set the cursor to perticular page and coloumn
  Input         : unsigned char page,unsigned char col
  Output        : Void
  Note          :
********************************************************************************************
*/
void lcd_set_page(unsigned char page,unsigned char col)
{
unsigned char msb,lsb;

  msb = (((col & 0xF0) >> 4)| 0x10);
  lsb = (col & 0x0F);
  lcd_write_cmd(page);
  lcd_write_cmd(msb);
  lcd_write_cmd(lsb);
}

/**
********************************************************************************************
  Function Name : glcd_clear()
  Description   : clears entire lcd or clears all page
  Input         : Void
  Output        : Void
  Note          :
********************************************************************************************
*/
/* */
void glcd_clear(void)
{
unsigned char i = 0,j = 0;

  lcd_set_page(PAGE0,COL0);             // initial page, col
  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 128; j++)
    {
      lcd_write_data(0x00);
      glcd_delay(10);
    }
    lcd_set_page(PAGE0 + i + 1 , COL0); // increment page
  }
  lcd_set_page(PAGE0,COL0);             // initial page,col
}

/**
********************************************************************************************
  Function Name : lcd_display_char()
  Description   :
  Input         : Void
  Output        : Void
  Note          :
**********************************************************************************************
*/
void lcd_display_char(unsigned char charecter)
{
unsigned char width = 0, index = 0, data = 0;

  width = get_width_of_char(charecter);
  for(index = 0; index < width; index++)
  {
    data = get_data_from_data_table(charecter, index);
    lcd_write_data(data);
  }
}

/**
********************************************************************************************
  Function Name : lcd_display_string()
  Description   : Displaying a string on GLCD
  Input         : unsigned char *str
  Output        : Void
  Note          :
**********************************************************************************************
*/
void lcd_display_string(unsigned char *str)
{
  if(str == 0)
    return;

  while(*str != 0)
  {
    lcd_display_char(*str);
    str++;
  }
}

/**
*****************************************************************************
  Function Name : init_glcd_ports()
  Description   :
  Input         : Void
  Output        : None
  Note          :
******************************************************************************
*/
void init_glcd_ports(void)
{
  LCD_DATA_DIR |= GLCD_DATA_MASK;
  LCD_CTRL_DIR |= LCD_CTRL_MASK;
  LCD_CTRL_CS_DIR |= LCD_CS;
  LCD_CTRL_RST_DIR |= LCD_RST;
  LCD_CTRL_BL_DIR |= LCD_BL;
 }

/**
*****************************************************************************
  Function Name : init_xy_axis()
  Description   : Set the curser to x , y co-ordinarte of GLCD
  Input         : x - col ,y - page
  Output        : None
  Note          :
******************************************************************************
*/
void init_xy_axis(int y,int x)
{
  lcd_set_page(y , x);
}

void back_light_on()
{
  LCD_CTRL_BL_SET |= LCDBL;
}

void back_light_off()
{
  LCD_CTRL_BL_SET &=~(LCDBL);
}
