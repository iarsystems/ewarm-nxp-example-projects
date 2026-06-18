/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *   to develop code for NXP MCIMX7SABRE board. It shows a basic use of the
 *   GIC, the General Purpose Timer and the UART module. The project is set
 *   to be executed on the Cortex-M4 core.
 *
 *    $Revision: 7084 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include <stdio.h>
#include <intrinsics.h>
#include "device_registers.h"
#include "NVIC.h"
#include "gpt.h"
#include "uart.h"
#include "llio.h"

/** interrupt handler functions **/
void GPTimer_Handler(void);

/** private data **/
static volatile uint32_t tick;

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
uint32_t cntr = 0;

  /*CPU disable IRQ*/
  __disable_interrupt();
  /*General Purpose Timer Init*/
  GPTimer_Init(GPT2_BASE_PTR, 1);
  /*Enable GPT2 Timer interrupt*/
  NVIC_EnableIRQ( INT_GPT2 );
  /*CPU enabNVIC_EnableIRQ()le IRQ*/
  __enable_interrupt();
  /*UART Initialization*/
  UART_Init(UART2_BASE_PTR);
  /*Low level redirect setup*/
  LLIO_Init(UART2_BASE_PTR);
  /*Start General Purpose Timer*/
  GPTimer_Start(GPT2_BASE_PTR);
  /*Print start string*/
  printf("\fMCIMX7SABRE GETTING STARTED\n\r\n\r");

  while(1)
  {
     if(tick)
     {
       tick = 0;
       /*Print counted seconds*/
       printf("Cortex M4 Seconds Count: %d\r", ++cntr);
     }
  }
}

/*************************************************************************
 * Function Name: GPTimer_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: General Purpose Timer interrupt handler
 *************************************************************************/
void GPT2_IRQHandler(void)
{
  /*Clear Timer Status*/
  GPTimer_ClearOF1Flag(GPT2_BASE_PTR);
  tick++;
}

