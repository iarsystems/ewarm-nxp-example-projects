/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 01.03.2011
 *       Author      : Stanimir Bonev
 *       Description : initial vestion
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale iMX535 quick start board. It shows basic use
 * of I/O, system initialization and GP timer.
 *
 *    $Revision: 5880 $
 **************************************************************************/

/** include files **/
#include <NXP/iomcimx535.h>
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"

/** local definitions **/
static volatile Int8U Gpt1Counter ;

/** default settings **/

/* GPT1 Tick per second*/
#define GPT_TICK_PER_SEC   4

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main(void)
{
  // USER LED init - GPIO_7[7]
  IOMUXC_SW_MUX_CTL_PAD_PATA_DA_1_bit.MUX_MODE = 1;
  GPIO7_DR_bit.no7   = 0;
  GPIO7_GDIR_bit.no7 = 1;

  // Init GPT
  Gpt1Counter = 0;
  // Select CKIL for timer clock - Low Frequency Reference Clock
  GPTCR_bit.CLKSRC = 4;
  // Init the Timer prescaler
  GPTPR_bit.PRESCALER = 0;
  // Init timer tick interval
  GPTOCR1 = CKIL/GPT_TICK_PER_SEC;
  
  //Enable Interrup on compare
  GPTIR_bit.OF1IE = 1;
  // Enable GPT1
  GPTCR_bit.EN = 1;

  __enable_interrupt();
  while(1)
  {
    if(GPTSR_bit.OF1)
    {
      GPTSR_bit.OF1= 1;
      ++Gpt1Counter;
      GPIO7_DR_bit.no7 = 1 & Gpt1Counter;
    }
  }
}
