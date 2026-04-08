/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : This example project shows how to use the IAR Embedded Workbench for ARM
 *    to develop code for NXP LPC family microcontrollers. It is developed for
 *    Olimex LPC-P2103 board.
 *
 *    This project implements sine modulated PWM using the Timer 2 module of the LPC2103
 *    microcontroller. The PWM period is 20kHz. Three sine wave singals with same frequancy
 *    but different phase(120) can be seen if simple Low Pass filters are connected to each of the outputs.
 *    The frequency and the level of the sine waves can be changed with commands from UART0
 *    The UART configuration is: 115200bps, 8-data bits, no parity(), 1 - stop bit (115200-8-N-1)
 *    Use "Help" Command for details
 *
 *    The project also shows how the FIQ can be used. The PWM Interrup Service Routine
 *    is installed as FIQ and resides in the RAM for faster execution.
 *
 *    History :
 *    1. Date        : 28.3.2008
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include <NXP\iolpc2103.h>
#include <intrinsics.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "PWM.h"
#include "interrupts.h"
#include "commands.h"

/** local definitions **/
#define MAX_CMD 20

/** default settings **/
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;

/** external functions **/
#ifndef SIM
extern void UART0_Init(void);
#endif

/** external data **/
extern const struct s_cmd MainCmd[3];
/** internal functions **/
//System initialization
void Init(void);
/** public data **/

/** private data **/
//buffer for command and history
char buffer[250];

/** public functions **/
void main(void)
{
  __disable_interrupt();
  //System Init
  Init();
  //Init VIC
  VIC_Init();
#ifndef SIM
  //Init UART0
  UART0_Init();
#endif
  //Init PWM module
  PWM_Init();

   __enable_interrupt();
  //Enable Fast GPIO
  SCS = 1;
  //LED pin as ouput
  FIODIR |= (1<<26);
  FIOSET = (1<<26);
   // FIOCLR = (1<<26);

  printf("\n\rSine PWM Project Started\n\r");


  while(1)
  {

    int argc;
    char * argv[MAX_CMD];
    char ** pargv = argv;

    CmdEdit(buffer, buffer+50, 50,200);
    argc = CmdParse(argv,buffer);

    while(argc)
    {
      printf("\n\r");
      if( 0 > CmdExec(&argc, &pargv, MainCmd))
      {
        if(argc) printf("\n\r\tUnkonwn command %s",*pargv);
        else printf("\n\r\tWrong or missing parameter");
        argc = 0;
      }
    }
  }
}
/** private functions **/
//System initialization
void Init(void)
{
  //Disabel Memory Accelerator Module
  MAMCR_bit.MODECTRL = 0;
  //Set fetch cycles
#if CCLK < 20000000
  MAMTIM_bit.CYCLES = 1;
#elif CCLK < 40000000
  MAMTIM_bit.CYCLES = 2;
#else
  MAMTIM_bit.CYCLES = 3;
#endif
  //Here MAM can be enabled
  MAMCR_bit.MODECTRL = 0; //MAM is disabled
  //Disable PLL
  PLLCON = 0;
  // Write Feed
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  //Set MSEL and PSEL
  PLLCFG = PLL_M_VAL | (PLL_P_VAL<<5);  //PLL 14,7456*4 = 58,9824MHz
  // Write Feed
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
  // Enable PLL, disconnected
  PLLCON = 1;
  // Write Feed
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
  //Set periferial clock
  APBDIV = APBDIV_VAL;
  //Wait PLL Lock
#ifndef SIM
  while(!PLLSTAT_bit.PLOCK);
#endif
  // connect PLL
  PLLCON = 3;
  // Write Feed
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
}
