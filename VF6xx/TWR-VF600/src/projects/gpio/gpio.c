#include "common.h"
#include "sysinit.h"
#include "gpio.h"
#include "lpt.h"

int main()
{
  printf("GPIO Example!\n");
  
  led_blink_tower();  

  return 0;
}

/* Blink LEDs on TWR-VF600 */
void led_blink_tower()
{
  //Setup GPIO output on LED pins (PTB0-PTB3)
  IOMUXC->SINGLE.PTB0 = IOMUXC_PTB0_DSE(0x1)| IOMUXC_PTB0_OBE_MASK; //enable GPIO output for PTB0
  IOMUXC->SINGLE.PTB1 = IOMUXC_PTB1_DSE(0x1)| IOMUXC_PTB1_OBE_MASK; //enable GPIO output for PTB1 
  IOMUXC->SINGLE.PTB2 = IOMUXC_PTB2_DSE(0x1)| IOMUXC_PTB2_OBE_MASK; //enable GPIO output for PTB2 
  IOMUXC->SINGLE.PTB3 = IOMUXC_PTB3_DSE(0x1)| IOMUXC_PTB3_OBE_MASK; //enable GPIO output for PTB3 

  IOMUXC->SINGLE.PTB16 = IOMUXC_PTB16_PUS(0x1)|IOMUXC_PTB16_PKE_MASK|IOMUXC_PTB16_PUE_MASK|IOMUXC_PTB16_IBE_MASK; //enable GPIO input for PTB16 
  IOMUXC->SINGLE.PTB17 = IOMUXC_PTB17_PUS(0x1)|IOMUXC_PTB17_PKE_MASK|IOMUXC_PTB17_PUE_MASK|IOMUXC_PTB17_IBE_MASK; //enable GPIO input for PTB17  
  
  //Flash on and off LED's
  while(1)
  {
    //Turn on blue LED on PTB0
    GPIO0->PDOR&=~PIN(22);
    
    //Toggle PTB1
    GPIO0->PTOR|=PIN(23);
    
    //If press SW1, turn off PTB2 LED. 
    if(GPIO1->PDIR & GPIO_PDIR_PDI(PIN(6)))
    {
      //Turn on LED by driving 0 (active low)
      GPIO0->PDOR&=~(PIN(24)); //corresponds with PTB2    
    }
    else
    {
      //Turn off LED by driving 1
      GPIO0->PDOR|=PIN(24); //corresponds with PTB2
    }
    
    //If press SW2, turn off PTB3 LED.
    if(GPIO1->PDIR & GPIO_PDIR_PDI(PIN(7)))
    {
      //Turn on LED by driving 0 (active low)
      GPIO0->PDOR&=~(PIN(25)); //corresponds with PTB3   
    }
    else
    {
      //Turn off LED by driving 1
      GPIO0->PDOR|=PIN(25); //corresponds with PTB3
    }
    
    time_delay_ms(300);
  }
}
