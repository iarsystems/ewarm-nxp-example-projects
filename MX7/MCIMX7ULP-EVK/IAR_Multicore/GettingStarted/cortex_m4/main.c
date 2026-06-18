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
 *    1. Date        : February, 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *   to develop code for NXP MCIMX7ULP-EVK board. It shows a basic use of the
 *   GIC, the Low Power Periodic Interrupt Timer and the LPUART module.
 *   The project is set to be executed on the Cortex-M4 core.
 *
 *    $Revision: 7084 $
 **************************************************************************/
/** include files **/
#include "device_registers.h"
#include <intrinsics.h>
#include <stdio.h>
#include "llio.h"
#include "lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PCS_SCG_SPLL_BUS_CLK_SEL   6
#define PCS_SCG_DIVBUS_CLK_SEL     5
#define LPUART_BAUD_RATE       115200

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void MU_A_ReleaseCoreB(void);
void LPIT0_Init(void);
void LPIT0_StartChannel0(uint32_t ticks);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t tick;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *************************************************************************/
int main(void)
{
uint32_t cntr = 0;

  /* Init LPUART0 @ 115200 baud */
  LPUART_Init(LPUART0_BASE_PTR, LPUART_BAUD_RATE, SCG_SPLL_BUS_CLK_HZ, PCS_SCG_SPLL_BUS_CLK_SEL);

  /* Redirect printf to LPUART0 */
  LLIO_Init(LPUART0_BASE_PTR);

  /* Release Core B (Cortex-A7) from reset if multicore debug session */
#if ((defined OCRAM_AMP_MULTICORE) || (defined OCRAM_DDR_AMP_MULTICORE))
  MU_A_ReleaseCoreB();
#endif

  __disable_interrupt();

  /* Init LPIT0, channel 0 to 1 tick per second */
  LPIT0_Init();
  LPIT0_StartChannel0(1);

  /* Enable LPIT0 interrupt in NVIC */
  NVIC_EnableIRQ(LPIT0_IRQn);

  __enable_interrupt();

  /* Print few info messages */
  printf("IMX7ULP-EVB Cortex-M4 settings\r\n");
  printf("------------------------------\r\n");
  printf("Core clock (Hz) : %d\r\n", SCG_SPLL_OUT_CLK_HZ/(SCG_RCCR_DIVCORE_VAL+1));
  printf("Bus clock (Hz) : %d\r\n", SCG_SPLL_OUT_CLK_HZ/(SCG_RCCR_DIVBUS_VAL+1));
  printf("SPLL_BUS clock (Hz) : %d\r\n\r\n", SCG_SPLL_BUS_CLK_HZ);

  /* Start main loop */
  while (1)
  {
    if (tick)
    {
      tick = 0;
      printf("Cortex-M4 seconds count: %d\r\n", ++cntr);
    }
  }
}

/*************************************************************************
 * Function Name: LPIT0_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Configures clock and enable LPIT0
 *************************************************************************/
void LPIT0_Init(void)
{
  /* Configure clock source and enable clock for LPIT0 */
  PCC0_PCC_LPIT0 = PCC0_PCC_LPIT0_CGC_MASK | PCC0_PCC_LPIT0_PCS(PCS_SCG_SPLL_BUS_CLK_SEL);
  LPIT0_MCR = LPIT_MCR_M_CEN_MASK;
}

/*************************************************************************
 * Function Name: LPIT0_StartChannel0
 * Parameters: uint32_t ticks - ticks per second
 *
 * Return: none
 *
 * Description: Configures and starts channel 0 of LPIT0
 *************************************************************************/
void LPIT0_StartChannel0(uint32_t ticks)
{
  LPIT0_TVAL0 = SCG_SPLL_BUS_CLK_HZ / ticks;
  LPIT0_MIER |= LPIT_MIER_TIE0_MASK;
  LPIT0_MSR |= LPIT_MSR_TIF0_MASK;
  LPIT0_TCTRL0 = LPIT_TCTRL_T_EN_MASK;
}

/*************************************************************************
 * Function Name: LPIT0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: LPIT0 interrupt handler
 *************************************************************************/
void LPIT0_IRQHandler(void)
{
  if (LPIT0_MSR & LPIT_MSR_TIF0_MASK)
  {
    LPIT0_MSR |= LPIT_MSR_TIF0_MASK;
    tick = 1;
  }
}

/*************************************************************************
 * Function Name: MU_A_ReleaseCoreB
 * Parameters: none
 *
 * Return: none
 *
 * Description: Release Core B (Cortex-A7) from reset. Entry point should
 *              be already set before execution of this function.
 *************************************************************************/
void MU_A_ReleaseCoreB(void)
{
    PCC0_BASE_PTR->PCC_MU_A |= PCC0_PCC_MU_A_CGC_MASK;
    MUA_BASE_PTR->CR &= ~MU_CR_RSTH_MASK;
}
