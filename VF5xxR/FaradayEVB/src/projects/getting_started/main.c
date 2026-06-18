/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    File name   : main.c
 *    Description : main module
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale Faraday EVB board. It shows basic use
 * of I/O, system initialization, UART and interrupts. The example blink LED
 *(P17 pin1 - PTD16 GPIO2[14]) and print a message to UART1 on SW6 pressing.
 *  UART settings - 115200, 8 bits, noparity, 1 stop bit
 *
 * Note: For boot in quad mode, add --quad_mode extra paramet for flashloader
 * DIP switches:
 * SW10 - 2
 * OFF - boot from QSPI0
 * ON  - boot from QSPI1
 *
 *    $Revision: 8050 $
 **************************************************************************/
#include <stdio.h>
#include <intrinsics.h>
#if ( __CORE__ == __ARM7EM__)
 #include "arm_cm4.h"
#endif
#include "sysinit.h"
#include "serial.h"
#include "ddr.h"

extern int_hdlr_t interrupt_handlers[144];

void LPTMR_IRQHandler(void);
void GPIO1_IRQHandler(void);

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main()
{

#if ( __CORE__ == __ARM7EM__)
  #if defined(DDR_CFG)
  printf("Hello World from Cortex-M4 core, code is executed by DDR...\r\n");
  #else
    #if defined(QSPI_CFG)
      #if 1 == QSPI_CFG
  printf("Hello World from Cortex-M4 core, code is executed by QSPI1...\r\n");
      #else
  printf("Hello World from Cortex-M4 core, code is executed by QSPI0...\r\n");
      #endif
    #else
  printf("Hello World from Cortex-M4 core, code is executed by OCRAM...\r\n");
    #endif
  #endif
#else
  #if defined(DDR_CFG)
  printf("Hello World from Cortex-A5 core, code is executed by DDR...\r\n");
  #else
    #if defined(QSPI_CFG)
      #if 1 == QSPI_CFG
  printf("Hello World from Cortex-A5 core, code is executed by QSPI1...\r\n");
      #else
  printf("Hello World from Cortex-A5 core, code is executed by QSPI0...\r\n");
      #endif
    #else
  printf("Hello World from Cortex-A5 core, code is executed by OCRAM...\r\n");
    #endif
  #endif
#endif

  //Now setup the LPTimer to create an interrupt after 0.5 second
  /* Make sure the clock to the LPTMR is enabled */
  CCM->CCGR2 |= CCM_CCGR2_CG0(1);
  /* Reset LPTMR settings */
  LPTMR0->CSR=0;
  /* Set the compare value to the number of ms to delay */
  LPTMR0->CMR = 500;
  /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
  LPTMR0->PSR |= LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;
  /* Start the timer */
  LPTMR0->CSR |= LPTMR_CSR_TIE_MASK | LPTMR_CSR_TEN_MASK;

#if ( __CORE__ == __ARM7EM__)
  //Direct interrupt to core_1 (m4)
  MSCM->IRSPRC[40] |= MSCM_IRSPRC_CP1En_MASK;  //interrupt ID
  //Enable Interrupt
  enable_irq(40);
#else
  //Enable interrupt for LPTimer (Vector 72 on A5)
  enable_interrupt(72,LPTMR_IRQHandler);
#endif

  /* setup GPIO interrupt for PTD31(GPIO1[31] SW6) */
  PUSH_BUTTON0_INIT;
  PORT1->PCR[31] = 0x9 << 16;  /* interrupt on rising edge */
  PORT1->DFWR = 0x1F; /* set filter length */
  PORT1->DFER |= 1; /* enable filter */

#if ( __CORE__ == __ARM7EM__)
  //Direct interrupt to core_1 (m4)
  MSCM->IRSPRC[108] |= MSCM_IRSPRC_CP1En_MASK;  //interrupt ID
  //Enable interrupt for GPIO 1 (Vector 124-16 on A4)
  enable_irq(108);
#else
  //Enable interrupt for GPIO 2 (Vector 72 on A5)
  enable_interrupt(140,GPIO1_IRQHandler);
#endif

  ENABLE_LEDS;

  // Enable the IRQ, FIQ and Aborts in A5 Core
  __enable_interrupt();
	while(1) {
	};
} /* main */

/*************************************************************************
 * Function Name: LPTMR_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: LPTMR interrupt handler
 *
 *************************************************************************/
void LPTMR_IRQHandler(void)
{
  /* clear flag */
  LPTMR0->CSR |= 1UL << 7;   //Clear LPT Compare flag

  LEDS_TOGGLE;
}

/*************************************************************************
 * Function Name: GPIO2_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: PORT2 interrupt handler
 *
 *************************************************************************/
void GPIO1_IRQHandler(void)
{
  static uint32_t io_int_count = 0;
  io_int_count++;
  printf("I/O interrupt number %d \r\n",io_int_count);
  /* clear flag */
  PORT1->PCR[31] |= 1UL<<24;
}
