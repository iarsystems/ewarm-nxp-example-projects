/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : cortex_a7.c
 *    Description : Cortex-A7 timer application
 *
 *    History :
 *    1. Date        : February, 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *   to develop code for NXP MCIMX7ULP-EVK board. It shows a basic use of the
 *   GIC, the Low Power Periodic Interrupt Timer and the LPUART module.
 *   The project is set to be executed on the Cortex-A7 core.
 *
 *    $Revision: 7084 $
 **************************************************************************/
/** include files **/
#include <stdio.h>
#include <intrinsics.h>
#include "device_registers.h"
#include "drv_gic.h"
#include "llio.h"
#include "lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CPU_XTAL_SOSC_CLK_HZ       24000000
#define SCG_SPLLCFG_MULT_VAL       22
#define SCG_SPLLNUM_VAL            0x4DD2F15
#define SCG_SPLLDENOM_VAL          0x1FFFFFDB
#define SCG_SPLLPFD_SPLLPFD0_VAL   19
#define SCG_SPLLDIV_SPLLDIV2_VAL   3
#define SCG_SPLL_OUT_CLK_HZ        (uint32_t)(CPU_XTAL_SOSC_CLK_HZ*(SCG_SPLLCFG_MULT_VAL+((double)SCG_SPLLNUM_VAL/(double)SCG_SPLLDENOM_VAL))*18.0/(double)SCG_SPLLPFD_SPLLPFD0_VAL)
#define SCG_SPLL_BUS_CLK_HZ        (uint32_t)((double)SCG_SPLL_OUT_CLK_HZ/(double)(1<<(SCG_SPLLDIV_SPLLDIV2_VAL-1)))
#define PCS_SCG_SPLL_BUS_CLK_SEL   7

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DisableWatchdogs(void);
static void SetupClocks(void);
static void LPIT1_Init(void);
static void LPIT1_StartChannel0(uint32_t ticks);
void LPIT1_IRQHandler(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t tick;

extern uint32_t __vector[];

/*******************************************************************************
 * Code
 ******************************************************************************/
/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: int
 *
 * Description: Low level initialization, called at startup, before data
 *              initialization and main()
 *************************************************************************/
int __low_level_init(void)
{
  // Set VBAR address
  __MCR(15,0,(uint32_t)__vector,12,0,0);
  // Set Cortex-A7 entry point
  SIM_BASE_PTR->SIM_DGO_GP3 = (uint32_t)__vector;
  return 1;
}

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

  /* Disable WDOG2 and WDOG3 */
  DisableWatchdogs();

  /* Setup system clocks */
  SetupClocks();

  /* Init LPUART4 @ 115200 baud */
  LPUART_Init(LPUART4_BASE_PTR, 115200, SCG_SPLL_BUS_CLK_HZ, PCS_SCG_SPLL_BUS_CLK_SEL);

  /* Redirect printf to LPUART4 */
  LLIO_Init(LPUART4_BASE_PTR);

  /*CPU disable IRQ*/
  __disable_irq();
  /*Interrupt controller*/
  GIC_Init(0);
  /* Init LPIT1, channel 0 to 1 tick per second */
  LPIT1_Init();
  LPIT1_StartChannel0(1);
  /*Configure LPIT1 interrupt*/
  GIC_SpiIntInstall(LPIT1_IRQHandler,INT_LPIT1,0x78,0);
  /*Enable LPIT1 interrupt*/
  GIC_IntEnable(INT_LPIT1,1);
  /*CPU enable IRQ*/
  __enable_irq();

  /* Print few info messages */
  printf("IMX7ULP-EVK Cortex-A7 settings\r\n");
  printf("------------------------------\r\n");
  printf("Core clock (Hz) : %d\r\n", SCG_SPLL_OUT_CLK_HZ);
  printf("SPLL_BUS clock (Hz) : %d\r\n\r\n", SCG_SPLL_BUS_CLK_HZ);

  /* Start main loop */
  while(1)
  {
    if (tick)
    {
      tick = 0;
      printf("Cortex-A7 seconds count: %d\r\n",++cntr);
    }
  }
}

/*************************************************************************
 * Function Name: SetupClocks
 * Parameters: none
 *
 * Return: none
 *
 * Description: Configures SysPLL and RUN clocks/dividers for CA7 core
 *************************************************************************/
static void SetupClocks(void)
{
  /* Return RUN clock to FIRC (if set another) */
  SCG1_RCCR =  SCG_RCCR_SCS(3);
  /* Start SysPLL */
  SCG1_SPLLCSR = 0x0;
  SCG1_SPLLCFG = (((1<<SCG_SPLLCFG_PLLS_SHIFT)&SCG_SPLLCFG_PLLS_MASK) | SCG_SPLLCFG_MULT_VAL<<SCG_SPLLCFG_MULT_SHIFT);
  SCG1_SPLLNUM = SCG_SPLLNUM_VAL;
  SCG1_SPLLDENOM = SCG_SPLLDENOM_VAL;
  SCG1_SPLLCSR = 0x1;
  /* Wait for valid SysPLL clock */
  while(0 == (SCG1_SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));
  /* Setup PFD0 */
  SCG1_SPLLPFD = 0x80808080;
  SCG1_SPLLPFD = 0x80808000 | SCG_SPLLPFD_PFD0(SCG_SPLLPFD_SPLLPFD0_VAL);
  /* Wait for valid PFD0 clock */
  while(0 == (SCG0_SPLLPFD & SCG_SPLLPFD_PFD0_VALID_MASK));
  /* Setup spll_bus_clk */
  SCG1_SPLLDIV = SCG_SPLLDIV_SPLLDIV2(SCG_SPLLDIV_SPLLDIV2_VAL);
  /* Set RUN clock to be spll_out */
  SCG1_RCCR =  SCG_RCCR_SCS(6)
             | SCG_RCCR_DIVCORE(0)   // /1
             | SCG_RCCR_DIVBUS(4);   // /5
}

/*************************************************************************
 * Function Name: LPIT1_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Configures clock and enable LPIT1
 *************************************************************************/
static void LPIT1_Init(void)
{
  /* Configure clock source and enable clock for LPIT0 */
  PCC2_PCC_LPIT1 = PCC2_PCC_LPIT1_CGC_MASK | PCC2_PCC_LPIT1_PCS(PCS_SCG_SPLL_BUS_CLK_SEL);
  LPIT1_MCR = LPIT_MCR_M_CEN_MASK;
}

/*************************************************************************
 * Function Name: LPIT1_StartChannel0
 * Parameters: uint32_t ticks - ticks per second
 *
 * Return: none
 *
 * Description: Configures and starts channel 0 of LPIT1
 *************************************************************************/
static void LPIT1_StartChannel0(uint32_t ticks)
{
  LPIT1_TVAL0 = SCG_SPLL_BUS_CLK_HZ / ticks;
  LPIT1_MIER |= LPIT_MIER_TIE0_MASK;
  LPIT1_MSR |= LPIT_MSR_TIF0_MASK;
  LPIT1_TCTRL0 = LPIT_TCTRL_T_EN_MASK;
}

/*************************************************************************
 * Function Name: LPIT1_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: LPIT1 interrupt handler
 *************************************************************************/
void LPIT1_IRQHandler(void)
{
  if (LPIT1_MSR & LPIT_MSR_TIF0_MASK)
  {
    LPIT1_MSR |= LPIT_MSR_TIF0_MASK;
    tick = 1;
  }
}

/*************************************************************************
 * Function Name: DisableWatchdogs
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disables WDOG2 and WDOG3
 *************************************************************************/
static void DisableWatchdogs(void)
{
  __disable_interrupt();
  /*disable watchdog 1*/
  if((WDOG1_BASE_PTR->CS) & WDOG_CS_EN_MASK)
  {
    WDOG1_BASE_PTR->CS = (uint32_t) ((WDOG1_BASE_PTR->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK | WDOG_CS_CMD32EN_MASK;
    WDOG1_BASE_PTR->TOVAL = 0xFFFF;
    WDOG1_BASE_PTR->CNT = 0xB480A602U;
  }
  /*disable watchdog 2*/
  if((WDOG2_BASE_PTR->CS) & WDOG_CS_EN_MASK)
  {
    WDOG2_BASE_PTR->CS = (uint32_t) ((WDOG2_BASE_PTR->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK | WDOG_CS_CMD32EN_MASK;
    WDOG2_BASE_PTR->TOVAL = 0xFFFF;
    WDOG2_BASE_PTR->CNT = 0xB480A602U;
  }
  /*enable global interrupt*/
  __enable_interrupt();
}
