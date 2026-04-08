#include "glcd_api.h"
#include "rtc_api.h"
#include "utils_api.h"
#include "stdio.h"
#include "string.h"

/**
********************************************************************************************
  Function Name : main()
  Description   :
  Input         :
  Output        :
  Note          :
********************************************************************************************
*/
int main(void)
{
  char buffer[20];
  char date[3];
  int error=0;
  DATETIMEINFO rd_dti,wr_dti;
  init_glcd();
  glcd_clear();
  back_light_on();
  error=pcf8523t_get((uint8_t *)&rd_dti);

  if(RTC_NO_ERROR == error)
  {
#if 0
      wr_dti.day=8;
      wr_dti.month=12;
      wr_dti.year=11;
      wr_dti.hour=12;
      wr_dti.minute=5;
      wr_dti.second=30;
      pcf8523t_set((uint8_t *)&wr_dti);
#endif
    error=pcf8523t_get((uint8_t *)&wr_dti);
    if(error == RTC_NO_ERROR && error != RTC_ERR_WRONG_PARAM && error != RTC_ERR_NO_RESPONSE)
    {
      while(1)
      {
        pcf8523t_get((uint8_t *)&wr_dti);
        s_memset(buffer,0,sizeof(buffer));s_memset(date,0,sizeof(date));strcpy(buffer,"DATE:");
        u_itoa((int)wr_dti.day, date,2);strcat(buffer,date);s_memset(date,0,sizeof(date));strcat(buffer,":");
        u_itoa((int)wr_dti.month, date,2);strcat(buffer,date);s_memset(date,0,sizeof(date));strcat(buffer,":");
        u_itoa((int)wr_dti.year, date,2);strcat(buffer,date);s_memset(date,0,sizeof(date));

        init_xy_axis(PAGE0 ,COL0);
        lcd_display_string((unsigned char *)buffer);
        s_memset(buffer,0,sizeof(buffer));s_memset(date,0,sizeof(date));strcpy(buffer,"TIME:");
        u_itoa((int)wr_dti.hour, date,2);strcat(buffer,date);s_memset(date,0,sizeof(date));strcat(buffer,":");
        u_itoa((int)wr_dti.minute, date,2);strcat(buffer,date);s_memset(date,0,sizeof(date));strcat(buffer,":");
        u_itoa((int)wr_dti.second, date,2);strcat(buffer,date);s_memset(date,0,sizeof(date));

        init_xy_axis(PAGE0 + 2 ,COL0);
        lcd_display_string((unsigned char *)buffer);
      }
    }
  }
  else if(error == RTC_ERR_WRONG_PARAM)
  {
    init_xy_axis(PAGE0 , COL0);
    lcd_display_string((unsigned char *)"Wrong parameters");
  }
  else if(error == RTC_ERR_NO_RESPONSE)
  {
    init_xy_axis(PAGE0 , COL0);
    lcd_display_string((unsigned char *)"RTC - Fail");
  }
  return 0;
}

/**
********************************************************************************************
             End of File
********************************************************************************************
*/
