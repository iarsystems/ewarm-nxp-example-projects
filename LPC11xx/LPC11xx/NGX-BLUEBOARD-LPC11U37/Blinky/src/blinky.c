#include "blinky.h"
#include "lpc11uxx.h"

void delay(int count)
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
    Funtion Name : led_test()
    Description  :
    Input        :
    Output       : None
    Note         :
 *****************************************************************
 */
void led_test(void)
{
  while(1)
  {
    LPC_GPIO->DIR[0] =TEST_LED1;
    LPC_GPIO->SET[0] |=  TEST_LED1;
    delay(100000);                         /* Delay 500ms          */
    LPC_GPIO->CLR[0] = TEST_LED1;
    delay(100000);

    LPC_GPIO->DIR[0] =TEST_LED2;
    LPC_GPIO->SET[0] |=  TEST_LED2;
    delay(100000);                         /* Delay 500ms          */
    LPC_GPIO->CLR[0] = TEST_LED2;
    delay(100000);
  }
}

int main(void)
{
  led_test();
  while(1){}
}
