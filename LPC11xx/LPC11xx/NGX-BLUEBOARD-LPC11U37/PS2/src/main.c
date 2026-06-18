/*
===============================================================================
 Name        : main.c
 Author      : Ashwin
 Version     :
 Copyright   : (C) Copyright
 Description : main definition
===============================================================================
*/

#include "LPC11uxx.h"
#include "ps2kbd.h"
#include "gpio.h"
#include "glcd_api.h"

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
  init_keyboard();
  init_glcd();
  glcd_clear();
  back_light_on();
  init_xy_axis(PAGE0,COL0);
  lcd_display_string((unsigned char *)"NGX TECHNOLOGIES");
  init_xy_axis(PAGE0 + 2,COL0);
  lcd_display_string((unsigned char *)"BANGALORE");

  while (1)
  {
    if(keyhit())
    {
      process_keyboard();
    }
  }
}

/**
********************************************************************************************
             End of File
********************************************************************************************
*/
