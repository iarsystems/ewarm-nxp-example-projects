#include "LPC11uxx.H"                       /* LPC11xx definitions */
#include "glcd_api.h"

/**
******************************************************************************
  Function Name : main()
  Description   :
  Input         :
  Output        : void
  Note          :
*******************************************************************************
*/
int main (void)
{
  init_glcd();
  glcd_clear();
  init_xy_axis(PAGE0,COL0);
  back_light_on();
  lcd_display_string((unsigned char *)"NGX TECHNOLOGIES");
  init_xy_axis(PAGE0 + 2,COL0);
  lcd_display_string((unsigned char *)"BANGALORE");
  init_xy_axis(PAGE0 + 4,COL0);
  lcd_display_string((unsigned char *)"INDIA");
  init_xy_axis(PAGE0 + 6,COL0);
  lcd_display_string((unsigned char *)"BLUEBOARD LPC11U37");
  while (1){}
}

/**
*******************************************************************************
          End of File
*******************************************************************************
*/
