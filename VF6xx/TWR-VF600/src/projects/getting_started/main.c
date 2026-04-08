/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    File name   : main.c
 *    Description : main module
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale TWR-VF600 board. It shows basic use
 * of I/O, system initialization, UART and interrupts. The example blink LED
 * D1 and print a message to UART1 on SW1 pressing.
 *	UART settings - 115200, 8 bits, noparity, 1 stop bit
 *
 DIP Swiches (BOOT CONFIG):
  Boot from QSPI0 -
   1 - ON
   2,3,4,5,6 - OFF

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
  printf("Hello World from Cortex-M4 core, code is executed by QSPI...\r\n");
    #else
  printf("Hello World from Cortex-M4 core, code is executed by OCRAM...\r\n");
    #endif
  #endif
#else
  #if defined(DDR_CFG)
  printf("Hello World from Cortex-A5 core, code is executed by DDR...\r\n");
  #else
    #if defined(QSPI_CFG)
  printf("Hello World from Cortex-A5 core, code is executed by QSPI...\r\n");
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

  /* setup GPIO interrupt for PTB16 (SW1 on TWR-VF600) */
  IOMUXC->SINGLE.PTB16 = 0x0000000D; /* GPIO, input, 100k pull down */
  //reg32_write(IOMUXC_PTB16, 0x0000000D); /* GPIO, input, 100k pull down */
  PORT1->PCR[6] = 0xA << 16;  /* interrupt on falling edge */
  //reg32_write(PORT1_PCR6, 0xA << 16); /* interrupt on falling edge */
  PORT1->DFWR = 0x1F; /* set filter length */
  //reg32_write(PORT1_DFWR, 0x1F); /* set filter length */
  PORT1->DFER |= 1; /* enable filter */
  //reg32setbit(PORT1_DFER, 1); /* enable filter */

#if ( __CORE__ == __ARM7EM__)
  //Direct interrupt to core_1 (m4)
  MSCM->IRSPRC[108] |= MSCM_IRSPRC_CP1En_MASK;  //interrupt ID
  //Enable Interrupt
  enable_irq(108);
#else
  //Enable interrupt for LPTimer (Vector 72 on A5)
  enable_interrupt(140,GPIO1_IRQHandler);
#endif

  /* setup GPIO interrupt for PTB0(GPIO0[22]) (led D1 on TWR-VF600) */
  //reg32_write(IOMUXC_PTB0, 0x00000182); /* GPIO, output */
  IOMUXC->SINGLE.PTB0 = 0x00000182; /* GPIO, output */
  //reg32_write(GPIO0_PSOR,1UL<<22); /* output high */
  GPIO0->PSOR = 1UL<<22;

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
  /* Toggle PORT0.22*/
  GPIO0->PTOR = 1UL<<22; /* toggle high */
}

/*************************************************************************
 * Function Name: GPIO1_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: PORT3 interrupt handler
 *
 *************************************************************************/
void GPIO1_IRQHandler(void)
{
  static uint32_t io_int_count = 0;
  io_int_count++;
  printf("I/O interrupt number %d \r\n",io_int_count);
  /* clear flag */
  PORT1->PCR[6] |= 1UL<<24;
}
