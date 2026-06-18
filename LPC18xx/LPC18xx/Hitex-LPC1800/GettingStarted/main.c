/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006 - 2011
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 30, November 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : 9, February 2009
 *       Author      : Stoyan Choynev
 *       Description : Port for IAR-LPC-1768-SK
 *    2. Date        : 28, November 2011
 *       Author      : Stanimir Bonev
 *       Description : Port for Hitex-LPC1800 (LPC1850 rev. A)
 *
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Hitex LPC1800 board A2/A4 (LPC1850 rev. A). It shows basic
 * use of I2C, timer and interrupt controllers.
 *  It starts by blinking LED0 (PCA9502 or  PCA9673PW I2S to IO controller).
 *
 *  Note: If your application will be loaded to NORFLASH and is bigger than
 * 16Kb, verification may fail if you don't use the
 * FlashHitexLPC1850_NOR_debug.mac macro file when starting the
 * debugger. The macro file configures all of the address lines of EMC,
 * in order to address the whole NORFLASH memory.
 *
 *    $Revision: 28 $
 **************************************************************************/

#include <intrinsics.h>
#include <stdio.h>
#include <assert.h>
#include "board.h"
#include "i2c_drv.h"

#pragma section=".intvec"

#define TIMER0_TICK_PER_SEC   10
#define TIMER0_IN_FREQ        150000000

#define I2C0_IN_FREQ          50000000

#define PCA9502_IODir_Reg         (0xAu << 3)
#define PCA9502_IOState_Reg       (0xBu << 3)
#define PCA9502_IOIntEna_Reg      (0xCu << 3)
#define PCA9502_IOControl_Reg     (0xFu << 3)

volatile Boolean SysTick;

/*variable for clitical section entry control*/
Int32U CriticalSecCntr;
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &SETENA0;

  assert((NVIC_DAC <= IntNumber) && (NVIC_QEI >= IntNumber));
  IntNumber -= NVIC_DAC;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &CLRENA0;

  assert((NVIC_DAC <= IntNumber) && (NVIC_QEI >= IntNumber));
  IntNumber -= NVIC_DAC;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &CLRPEND0;

  assert((NVIC_DAC <= IntNumber) && (NVIC_QEI >= IntNumber));
  IntNumber -= NVIC_DAC;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(Int32U IntNumber, Int8U Priority)
{
volatile Int8U * pNVIC_IntPri = (Int8U *)&IP0;

  assert((NVIC_DAC <= IntNumber) && (NVIC_QEI >= IntNumber));
  IntNumber -= NVIC_DAC;

  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

/*************************************************************************
 * Function Name: TIMER0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TIMER0_IRQHandler (void)
{
  /* Toggle LED1 */
  SysTick = TRUE;
  /*clear interrupt */
  T0IR_bit.MR0INT = 1;
  /**/
  NVIC_ClrPend(NVIC_TIMER0);
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL and clocks' dividers. Hclk - 300MHz,
 *              Fcckl = 100MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /* 1. Init OSC */
  CGU_XTAL_OSC_CTRL_bit.HF = 0;
  CGU_XTAL_OSC_CTRL_bit.BYPASS = 0;
  CGU_XTAL_OSC_CTRL_bit.ENABLE = 0;
  /* 2.  Wait for OSC ready > 2ms */
  for(volatile Int32U j = 12000; j; j--);
  /* 3. Switch to OSC clk */
  /* BASE_M3_CLK */
  CGU_OUTCLK_4_CTRL = (1ul  << 11)
                    | (6ul  << 24)
                    ;
  /* BASE_APB1_CLK */
  CGU_OUTCLK_9_CTRL = (1ul  << 11)
                    | (6ul  << 24)
                    ;
  /* BASE_APB3_CLK */
  CGU_OUTCLK_10_CTRL = (1ul  << 11)
                    |  (6ul  << 24)
                    ;
  /* BASE_SPIFI_CLK */
  CGU_OUTCLK_5_CTRL = (1ul  << 11)
                    | (6ul  << 24)
                    ;
  /* 4. Init PLL1 - In clk - OSC (12MHz)
     N = 1, M = 16, P = 1 VCO - 192MHz, Pll OUT - 96MHz */
  CGU_PLL1_CTRL_bit.PD = 1;
  CGU_PLL1_CTRL_bit.BYPASS = 0;
  CGU_PLL1_CTRL_bit.DIRECT = 0;
  CGU_PLL1_CTRL_bit.FBSEL = 0;
  CGU_PLL1_CTRL_bit.PSEL = 1-1;
  CGU_PLL1_CTRL_bit.NSEL = 1-1;
  CGU_PLL1_CTRL_bit.MSEL = 16-1;
  CGU_PLL1_CTRL_bit.CLK_SEL = 6;
  CGU_PLL1_CTRL_bit.AUTOBLOCK = 1;
  /* 5. Enable PLL */
  CGU_PLL1_CTRL_bit.PD = 0;
  /* 5. Wait for the PLL to achieve lock */
  while(!CGU_PLL1_STAT_bit.LOCK);
  /* 6. Dividers Div A - /3 input 150MHz output 50MHz */
  CGU_IDIVA_CTRL_bit.AUTOBLOCK = 1;
  CGU_IDIVA_CTRL_bit.IDIV = 3-1;
  CGU_IDIVA_CTRL_bit.CLK_SEL = 9;
  CGU_IDIVA_CTRL_bit.PD = 0;

  CGU_IDIVB_CTRL_bit.AUTOBLOCK = 1;
  CGU_IDIVB_CTRL_bit.IDIV = 5-1;
  CGU_IDIVB_CTRL_bit.CLK_SEL = 9;
  CGU_IDIVB_CTRL_bit.PD = 0;
  /* 7. Init Clock output stages */
  /* BASE_M3_CLK, APB0, APB2 - 150MHz */
  CGU_OUTCLK_4_CTRL = (1ul  << 11)
                    | (9ul  << 24)
                    ;
  /* BASE_APB1_CLK - 50MHz*/
  CGU_OUTCLK_9_CTRL = (1ul  << 11)
                    | (12ul << 24)
                    ;
  /* BASE_SPIFI_CLK - 30MHz */
  CGU_OUTCLK_5_CTRL = (1ul  << 11)
                    | (13ul  << 24)
                    ;
  /* Enable M3 Bus clock */
  CCU1_CLK_M3_BUS_CFG_bit.RUN = 1;
  /* Enable APB 1 Bus clock */
  CCU1_CLK_APB1_BUS_CFG_bit.RUN = 1;
  /* Enable SCU clock */
  CCU1_CLK_M3_SCU_CFG_bit.RUN = 1;
  /* 7. Init PLL1 - In clk - OSC (12MHz)
     N = 1, M = 25, P = 1 VCO - 300MHz, Pll OUT - 150MHz */
  CGU_PLL1_CTRL_bit.BYPASS = 0;
  CGU_PLL1_CTRL_bit.DIRECT = 0;
  CGU_PLL1_CTRL_bit.FBSEL = 0;
  CGU_PLL1_CTRL_bit.PSEL = 1-1;
  CGU_PLL1_CTRL_bit.NSEL = 1-1;
  CGU_PLL1_CTRL_bit.MSEL = 25-1;
  CGU_PLL1_CTRL_bit.CLK_SEL = 6;
  CGU_PLL1_CTRL_bit.AUTOBLOCK = 1;
  /* 8. Wait for the PLL to achieve lock */
  while(!CGU_PLL1_STAT_bit.LOCK);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
  Int8U temp = 0xFFu;
  Int8U data[2];
  Int32U board_rev = BOARD_A4;


  /* Clock Init Core frecuency 150 MHz */
  InitClock();
  VTOR = (Int32U)__section_begin(".intvec");
  /* Init I2C0 */
  I2C_InitMaster(I2C_SPEED, I2C0_IN_FREQ);

  /* try to find board revision*/
  if(I2C_MasterWrite(PCA9673PW_LED_ADDR, &temp, 1))
  {
    /* Reset PCA9502 */
    data[0] = PCA9502_IOControl_Reg;
    data[1] = 0x01<<3;
    I2C_MasterWrite(PCA9502_LED_ADDR, data, 2);
    /* Configure LED pins as outputs */
    data[0] = PCA9502_IODir_Reg;
    data[1] = 0x0F;
    I2C_MasterWrite(PCA9502_LED_ADDR, data, 2);
    board_rev = BOARD_A2;
  }

  /* Init Time0 */
  /* Enable TIM0 clock */
  CCU1_CLK_M3_TIMER0_CFG_bit.RUN = 1;

  T0TCR_bit.CE = 0;     /* counting  disable */
  T0TCR_bit.CR = 1;     /* set reset */
  T0TCR_bit.CR = 0;     /* release reset */
  T0CTCR_bit.CTMODE = 0;/* Timer Mode: every rising PCLK edge */
  T0MCR_bit.MR0I = 1;   /* Enable Interrupt on MR0 */
  T0MCR_bit.MR0R = 1;   /* Enable reset on MR0 */
  T0MCR_bit.MR0S = 0;   /* Disable stop on MR0 */
  /* set timer 0 period */
  T0PR = 150-1;
  T0MR0 = (TIMER0_IN_FREQ)/(150 * TIMER0_TICK_PER_SEC) - 1;
  /* init timer 0 interrupt */
  T0IR_bit.MR0INT = 1;  /* clear pending interrupt */
  T0TCR_bit.CE = 1;     /* counting Enable */
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_TIMER0);

  __enable_interrupt();

  while(1)
  {
    if(SysTick)
    {
      SysTick = FALSE;
      /* Toggle LED0 */
      temp ^= 1;
      if(BOARD_A4 == board_rev)
      {
        I2C_MasterWrite(PCA9673PW_LED_ADDR, &temp, 1);
      }
      else
      {
        data[0] = PCA9502_IOState_Reg;
        data[1] = temp;
        I2C_MasterWrite(PCA9502_LED_ADDR, data, 2);
      }
    }
  }
}
