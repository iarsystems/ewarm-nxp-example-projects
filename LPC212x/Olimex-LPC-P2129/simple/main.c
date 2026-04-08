//This is the LPC2124 or LPC2129 board's factory test code, the CAN port test being applicable to the LPC2129 only
//Tests buttons, LEDS, variable resistor and the level shifters and sockets to RS232 and CAN ports

/*-------------------------------------------------------------------------------------------------------------------------------*/
#include <NXP/iolpc2129.h> //Needed by Macros only usually (but in this case 'SFR peek/poking' in main too)
#include "hardware.h"  //Supplies Hardware specific macros (e.g. Action A_LED1_ON)

#ifndef SRAM_VIA_JLINK
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;
#endif


/*-------------------------------------------------------------------------------------------------------------------------------*/
int main(void)
{
  U8  but1_old_state = S_BUT1;
  U8  but2_old_state = S_BUT2;
  CPUinit();
  UART0Init(9600);
  UART1Init(9600);

  while(1)
  {
    if (U0LSR_bit.RDR) UART0WriteChar(U0RBR + 2);     //IF Byte Received on UART0 then transmit ECHO+1
    if (U1LSR_bit.RDR) UART1WriteChar(U1RBR + 3);     //IF Byte Received on UART1 then transmit ECHO+1

    //Light LED1 if Button 1 is Pressed
    if (S_BUT1==DOWN && but1_old_state==UP)
    {
      A_LED1_ON;
      but1_old_state=DOWN;
    }
    if (S_BUT1==UP && but1_old_state==DOWN)
    {
      A_LED1_OFF;
      but1_old_state=UP;
    }

    //Light LED2 if Button 2 is Pressed AND flash LED1 if the CAN ports are comnnected
    if ((S_BUT2)==DOWN)
    {
      A_LED2_ON;
      A_TD2_LOW;
      Delay (TD2_TIME);
      if (S_RD2==LOW && S_RD1==LOW) A_LED1_ON;
      A_TD2_HIGH;
      Delay(TD2_TIME);
      if (S_RD2!=LOW && S_RD1!=LOW) A_LED1_OFF;
    }
    else A_LED2_OFF;

    //LED1 and LED2 flash if DALLAS or FREQ are pulled low by an attached device
    if (S_DALLAS==LOW || S_FREQ==HIGH)
    {
      A_LED1_ON;
      A_LED2_ON;
      Delay(DLS_FREQ_TIME);
      A_LED1_OFF;
      A_LED2_OFF;
    }

    //If BUT1 and BUT2 are pressed together - test ADC PIN/trimer by displaying digitised 1 BIT VALUE! on LEDS
    //Note that this is not a proper A/D conversion test
    while (S_BUT1==DOWN && S_BUT2==DOWN)
    {
      A_LED1_OFF;
      A_LED2_OFF;
      if ((S_ADC) == 0) A_LED1_ON; else A_LED2_ON;
    }
  }//Matching brace to main.c while(1)
}

/*-------------------------------------------------------------------------------------------------------------------------------*/

