/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sys.c
 *    Description : System module
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "sys.h"

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

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

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

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

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

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

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

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 *              Fcckl = 96MHz
 *
 *************************************************************************/
void InitClock(void)
{
  // 1. Init OSC
  SCS_bit.OSCRANGE = 0;
  SCS_bit.OSCEN = 1;
  // 2.  Wait for OSC ready
  while(!SCS_bit.OSCSTAT);
  // 3. Disconnect PLL
  PLL0CON_bit.PLLC = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 4. Disable PLL
  PLL0CON_bit.PLLE = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 5. Select source clock for PLL
  CLKSRCSEL_bit.CLKSRC = 1;   // Selects the main oscillator as a PLL clock source.
  PCLKSEL0 = PCLKSEL1  = 0;   // other peripherals 96/4 = 24MHz
  // 6. Set PLL settings 288 MHz
  PLL0CFG_bit.MSEL = 24-1;
  PLL0CFG_bit.NSEL = 2-1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 7. Enable PLL
  PLL0CON_bit.PLLE = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 8. Wait for the PLL to achieve lock
  while(!PLL0STAT_bit.PLOCK);
  // 9. Set clk divider settings
  CCLKCFG   = 3-1;            // 1/3 Fpll
  USBCLKCFG = 6-1;            // 1/6 Fpll - 48 MHz
  // 10. Connect the PLL
  PLL0CON_bit.PLLC = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
}

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
  // Set to inputs
  FIO0DIR = 0;
  FIO1DIR = 0;
  FIO2DIR = 0;
  FIO3DIR = 0;
  FIO4DIR = 0;

  // clear mask registers
  FIO0MASK = 0;
  FIO1MASK = 0;
  FIO2MASK = 0;
  FIO3MASK = 0;
  FIO4MASK = 0;

  // Reset all GPIO pins to default primary function
  PINSEL0 = 0;
  PINSEL1 = 0;
  PINSEL2 = 0;
  PINSEL3 = 0;
  PINSEL4 = 0;
  PINSEL7 = 0;
  PINSEL8 = 0;
  PINSEL9 = 0;
}

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFsclk(void)
{
Int32U Mul = 1, Div = 1, Osc, Fsclk;
  if(PLL0STAT_bit.PLLC)
  {
    // when PLL is connected
    Mul = PLL0STAT_bit.MSEL + 1;
    Div = PLL0STAT_bit.NSEL + 1;
  }

  // Find clk source
  switch(CLKSRCSEL_bit.CLKSRC)
  {
  case 0:
    Osc = I_RC_OSC_FREQ;
    break;
  case 1:
    Osc = MAIN_OSC_FREQ;
    break;
  case 2:
    Osc = RTC_OSC_FREQ;
    break;
  default:
    Osc = 0;
  }
  // Calculate system frequency
  Fsclk = Osc*Mul*2;
  Fsclk /= Div*(CCLKCFG+1);
  return(Fsclk);
}

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: Int32U
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFpclk(Int32U Periphery)
{
Int32U Fpclk;
pInt32U pReg = (pInt32U)((Periphery < 32)?&PCLKSEL0:&PCLKSEL1);

  Periphery  &= 0x1F;   // %32
  Fpclk = SYS_GetFsclk();
  // find peripheral appropriate periphery divider
  switch((*pReg >> Periphery) & 3)
  {
  case 0:
    Fpclk /= 4;
    break;
  case 1:
    break;
  case 2:
    Fpclk /= 2;
    break;
  default:
    Fpclk /= 8;
  }
  return(Fpclk);
}


