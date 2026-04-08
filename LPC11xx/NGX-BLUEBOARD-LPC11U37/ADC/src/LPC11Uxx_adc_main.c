#include "glcd_api.h"
#include "lpc11Uxx_adc.h"


/**
********************************************************************************************
  Function Name : main()
  Description   :
  Input         :
  Output        : Void
  Note          :
********************************************************************************************
*/
int main(void)
{
  init_glcd();
  glcd_clear();
  back_light_on();
  init_xy_axis(PAGE0,COL0);
  lcd_display_string((unsigned char *)"NGX TECHNOLOGIES");
  init_xy_axis(PAGE0 + 2,COL0);
  lcd_display_string((unsigned char *)"BANGALORE");
  init_xy_axis(PAGE0 + 4,COL0);
  lcd_display_string((unsigned char *)"INDIA");
  ADCExp();
  while(1){}
}
/**
********************************************************************************************
        End of File
********************************************************************************************
*/
