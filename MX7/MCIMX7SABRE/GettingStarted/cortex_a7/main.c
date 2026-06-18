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
 *    1. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    2. Date        : July 2016
 *       Author      : Atanas Uzunov
 *       Description : Adapted for MCIMX7SABRE board. Added Cortex-M4 core
 *                     application.
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *   to develop code for NXP MCIMX7SABRE board. It shows a basic use of the
 *   GIC, the General Purpose Timer and the UART modules in single-core
 *   (standalone) and multi-core debug sessions.
 *
 *    $Revision: 7084 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include <stdio.h>
#include <intrinsics.h>
#include "device_registers.h"
#include "drv_gic.h"
#include "uart.h"
#include "llio.h"
#include "gpt.h"

/** internal functions **/
static void GPTimer_Handler(void);
#ifdef OCRAM_AMP_MULTICORE
static void RunM4app(void);
#endif

/** private data **/
static volatile uint32_t tick;

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
uint32_t cntr = 0;

  /*CPU disable IRQ*/
  __disable_irq();
  /*Interrupt controller*/
  GIC_Init(0);
  /*General Purpose Timer Init*/
  GPTimer_Init(GPT1_BASE_PTR, 1);
  /*Configure GPT1 interrupt*/
  GIC_SpiIntInstall(GPTimer_Handler,INT_GPT1,0x78,0);
  /*Enable GPT1 Timer interrupt*/
  GIC_IntEnable(INT_GPT1,1);
  /*CPU enable IRQ*/
  __enable_irq();
  /*UART Initialization*/
  UART_Init(UART1_BASE_PTR);
  /*Low Level IO redirection setup*/
  LLIO_Init(UART1_BASE_PTR);
  /*Start General Purpose Timer*/
  GPTimer_Start(GPT1_BASE_PTR);
#ifdef OCRAM_AMP_MULTICORE
  /*Run M4 Application*/
  RunM4app();
#endif
  /*Print start string*/
  printf("\fMCIMX7SABRE GETTING STARTED\n\r\n\r");

  while(1)
  {
     if(tick)
     {
       tick = 0;
       /*Print counted seconds*/
       printf("Cortex A7 Seconds Count: %d\r", ++cntr);
     }
  }
}

#ifdef OCRAM_AMP_MULTICORE
/*************************************************************************
 * Function Name: RunM4app
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable and start Cortex-M4 core
 *
 *************************************************************************/
static void RunM4app(void)
{
  /*Set M4 enable and reset bit, mask WDOG3 reset*/
  SRC_M4RCR |= SRC_M4RCR_SW_M4C_NON_SCLR_RST_MASK | SRC_M4RCR_ENABLE_M4_MASK;
  /*Enable clock for M4*/
  CCM_CCGR1 = 0x2;
  /*Release M4 reset*/
  SRC_M4RCR &= ~SRC_M4RCR_SW_M4C_NON_SCLR_RST_MASK;
}
#endif

/*************************************************************************
 * Function Name: GPTimer_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: General Purpose Timer interrupt handler
 *
 *************************************************************************/
static void GPTimer_Handler(void)
{
  /*Clear Timer Status*/
  GPTimer_ClearOF1Flag(GPT1_BASE_PTR);
  tick++;
}
