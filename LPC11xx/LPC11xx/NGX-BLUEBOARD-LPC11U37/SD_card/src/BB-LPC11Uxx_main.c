/*
===============================================================================
 Name        : main.c
 Author      :
 Version     :
 Copyright   : Copyright (C)
 Description : main definition
===============================================================================
*/
#include "glcd_api.h"
#include "type.h"
#include "types.h"
#include "sd.h"
#include "utils.h"
#define SECTOR_SIZE 512
uint8_t buf[SECTOR_SIZE]={0};

U8 sdcard_init(void)
{
 if(if_initInterface(0)==0)
 return 1;
 else
 return 0;
}
/**
*******************************************************************
  Funtion Name : sd_card_test()

  Description  :

  Input        :

  Output       : None

  Note         :
*****************************************************************
*/
int8_t sd_card_test(void)
{
uint16_t byte_number=0;
uint8_t status=1;
uint8_t sector_address=0;

  if(sdcard_init())
  {
    for(sector_address=0;sector_address<10;sector_address++)
    {
      for(byte_number=0;byte_number<sizeof(buf);byte_number++)
      {
        buf[byte_number] = 0;
      }
      sd_writeSector(sector_address,buf);

      for(byte_number=0;byte_number<sizeof(buf);byte_number++)
      {
        if(byte_number%2)
          buf[byte_number]=0xAA;
        else
          buf[byte_number]=0x55;
      }
      sd_writeSector(sector_address,buf);

      for(byte_number=0;byte_number<sizeof(buf);byte_number++)
      {
        buf[byte_number]=0;
      }

      sd_readSector( sector_address, buf, sizeof(buf));

      for(byte_number=0;byte_number<sizeof(buf);byte_number++)
      {
        if(byte_number%2)
        {
          if ( buf[byte_number]!=0xAA )
          {
            status  = 0;
            break ;
          }
        }
        else
        {
          if (buf[byte_number]!= 0x55)
          {
            status = 0 ;
            break ;
          }
        }
      }

      if(status == 1)
      {
        return SDCARD_FOUND;
      }
      else
      {
        return SDCARD_FAIL;
      }
    }
  }
  else
  {
    return SDCARD_NOT_DETECTED;
  }
  return 0;
}

/**
*******************************************************************
  Funtion Name : main()

  Description  :

  Input        :

  Output       : None

  Note         :
*****************************************************************
*/
int main (void)
{
int8_t sdcard_test;
init_glcd();
  glcd_clear();
  back_light_on();
  init_xy_axis(PAGE0,COL0);
  lcd_display_string((unsigned char *)"NGX TECHNOLOGIES");
  init_xy_axis(PAGE0 + 2,COL0);
  lcd_display_string((unsigned char *)"BANGALORE");
  init_xy_axis(PAGE0 + 4,COL0);
  lcd_display_string((unsigned char *)"INDIA");
  sdcard_test=sd_card_test();

  if(sdcard_test== SDCARD_FOUND)
  {
    init_xy_axis(PAGE0 + 6,COL0);
    lcd_display_string((unsigned char *)"SDcard PASS");
  }
  else if(sdcard_test==SDCARD_FAIL)
  {
    init_xy_axis(PAGE0 + 6,COL0);
    lcd_display_string((unsigned char *)"SDcard FAIL");
  }
  else if(sdcard_test==SDCARD_NOT_DETECTED)
  {
    init_xy_axis(PAGE0 + 6,COL0);
    lcd_display_string((unsigned char *)"SDcard misssing");
  }
  while (1){}
}

 /**
*******************************************************************
          End of File
*****************************************************************
*/
