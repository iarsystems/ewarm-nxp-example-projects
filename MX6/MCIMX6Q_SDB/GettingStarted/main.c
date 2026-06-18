/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 07.2013
 *       Author      : Stoyan Choynev
 *       Description : Initial revison
 *    2. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : Added Private Timer driver
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *   to develop code for Freescale MCIMX6Q-SDB board. It shows basic use of I/O
 *   and GIC
 *
 *    $Revision: 7081 $
 **************************************************************************/
/** include files **/
#include <stddef.h>
#include <stdint.h>
#include <intrinsics.h>
#include <NXP/iomcimx6q6.h>
#include "drv_gic.h"
#include "drv_A9_timers.h"
/** local definitions **/
/*Arm clock*/
#define ARM_CLK     (792000000UL)
/*Private Timer Prescaler*/
#define PTIM_PRESCALE  (256)
/*Timer ticks per second*/
#define TICKS          (10)
/*Private Timer Reload value*/
#define PTIM_LOAD   ((ARM_CLK/2)            /*Timer Runs @ ARM_CLK/2*/\
                    /PTIM_PRESCALE          /*Prescaler*/\
                    /TICKS)                 /*Ticks*/
/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
static void PTMR_Handler(void);

/** public data **/

/** private data **/

/** public functions **/

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
  /* Interrupt controller */
  GIC_Init(0);
  /*cpu enable IRQ*/
  __enable_irq();
  /*Private timer Init*/
  PTMR_Init(PTIM_LOAD,PTIM_PRESCALE-1, PTMR_CTLR_AUTO_RELOAD |
                                       PTMR_CTLR_IRQ_ENABLE);
  /*configure Private Timer interrupt*/
  GIC_PpiIntInstall(PTMR_Handler,INT_PTIM,0x78);
  /*Enable Private Timer interrupt*/
  GIC_IntEnable(INT_PTIM,1);
  /*Start Private Timer*/
  PTMR_Start();
  /*Configure GPIO*/
  GPIO1_GDIR_bit.no2 = 1;

  while(1)
  {
  }
}
/** private functions **/

/*************************************************************************
 * Function Name: PTMR_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Private Timer interrupt handler
 *
 *************************************************************************/
static
void PTMR_Handler(void)
{
  PTMR_ClrIsr();
  GPIO1_DR_bit.no2 ^= 1;
}

