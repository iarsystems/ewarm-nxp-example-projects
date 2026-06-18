/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006 - 2009
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 2, December 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : 11, February 2009
 *       Author      : Stoyan Choynev
 *       Description : Port for IAR-LPC1768-SK
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-LPC1768-SK board. It shows basic use of the I/O,
 * the timer, the interrupt controllers and the LDC module for graphic and text
 * drawing.
 *  It starts by show the IAR logo on the LCD and after 5 second draw a test screen.
 *
 * Jumpers:
 *  PWR_SEL - depending of power source
 *  RST_E   - unfilled
 *  ISP_E   - unfilled
 *
 *  AN_TRIM - contrast / backlight
 *  BUT1    - adjust backlight
 *  BUT2    - adjust LCD contrast
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect. In this case adaptive clocking
 * should be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define TIMER0_TICK_PER_SEC   20
#define DLY_100US             500

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

volatile Boolean CntrSel = FALSE;

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
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TMR0_IRQHandler (void)
{
  if(!(B1_FIO & B1_MASK))
  {
    CntrSel = FALSE;
  }
  else if (!(B2_FIO & B2_MASK))
  {
    CntrSel = TRUE;
  }
  // clear interrupt
  T0IR_bit.MR0INT = 1;
  /**/
  NVIC_ClrPend(NVIC_TIMER0);
}

#define FCCLK_FREQ 100000000
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
  PCLKSEL0 = PCLKSEL1  = 0;   // other peripherals 100/4 = 25MHz
  // 6. Set PLL settings 300 MHz
  PLL0CFG_bit.MSEL = 25-1;
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

/*************************************************************************
 * Function Name: ClearFlag
 * Parameters: void
 * Return: void
 *
 * Description: clear arg
 *		
 *************************************************************************/
void ClearFlag (void* arg)
{
int* pFlag = arg;
  *pFlag = 0;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(;Dly;Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
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
void DrawTable (void)
{
Int32U x,y,j,k;

// xxxxRRRRGGGGBBBB
const Int32U TableColor [] =
{
  0xFFF, 0xAAA, 0x888, 0x666, 0x444, 0x000,   // Grey
  0x00F, 0x00A, 0x008, 0x006, 0x004, 0x001,   // Red
  0x0F0, 0x0A0, 0x080, 0x060, 0x040, 0x010,   // Green
  0xF00, 0xA00, 0x800, 0x600, 0x400, 0x100,   // Blue
};

  for(y = 0; y < 4; ++y)
  {
    for(x = 0; x < 6; ++x)
    {
      // set rectangle
      k = (x*22) | ((((x+1)*22)-1)<<8);
      GLCD_SendCmd(CASET,(pInt8U)&k,0);
      k = (y*29) | ((((y+1)*29)-1)<<8);
      GLCD_SendCmd(RASET,(pInt8U)&k,0);
      // fill rectangle
      for(j = 0; j < (22*29); ++j)
      {
        GLCD_SendCmd(RAMWR,(pInt8U)&TableColor[x+(y*6)],2);
      }
    }
  }
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
Boolean SelHold;
Int32U AdcData;
  // Flash accelerator init
  FLASHCFG = (0x5UL<<12) | 0x3AUL;
  // Init clock
  InitClock();
#if FCCLK_FREQ < 20000000
  FLASHCFG = (0x0UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 40000000
  FLASHCFG = (0x1UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 60000000
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 80000000
  FLASHCFG = (0x3UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 100000000
  FLASHCFG = (0x4UL<<12) | 0x3AUL;
#endif
  // Init GPIO
  GpioInit();

  // But 0,1 init
  B1_FDIR &= ~B1_MASK;
  B2_FDIR &= ~B2_MASK;

  // ADC Init
  // Assign P1.31 to AIN5
  ANALOG_TRIM_PIN_SEL = 3;
  ANALOG_TRIM_PIN_MODE = 2;
  PCONP_bit.PCAD = 1; // Enable ADC clk
  // Set ADC clk <4.5 MHz
  AD0CR_bit.CLKDIV = (SYS_GetFpclk(24) / 4500000) + 1;
  // Select AIN5
  AD0CR_bit.SEL = 1<<5;
  // Disable all interrupts
  ADINTEN = 0;
  // Enable ADC
  AD0CR_bit.PDN = 1;
  // Start conversion
  AD0CR_bit.START = 1;

  // Init Time0
  PCONP_bit.PCTIM0 = 1; // Enable TMR0 clk
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T0PR = 0;
  T0MR0 = SYS_GetFpclk(TIMER0_PCLK_OFFSET)/(TIMER0_TICK_PER_SEC);
  // init timer 0 interrupt
  T0IR_bit.MR0INT = 1;  // clear pending interrupt
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_TIMER0);

  T0TCR_bit.CE = 1;     // counting Enable

  __enable_interrupt();

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);

  GLCD_Backlight(BACKLIGHT_ON);
  DelayResolution100us(30000);
  DrawTable();

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,116,131,131);
  GLCD_TextSetPos(0,0);

  if(CntrSel)
  {
    SelHold = TRUE;
    GLCD_print("\fContrast adj.\r");
  }
  else
  {
    SelHold = FALSE;
    GLCD_print("\fBacklight adj.\r");
  }

  while(1)
  {
    AdcData = AD0GDR;
    if(AdcData & (1UL << 31))
    {
      AD0CR_bit.START = 1;
      AdcData >>= 10;
      AdcData  &= 0xFF;
      if(SelHold)
      {
        // Contract adj
        GLCD_SendCmd(SETCON,(pInt8U)&AdcData,0);
      }
      else
      {
        // Backlight adj
        AdcData >>= 1;
        GLCD_Backlight(AdcData);
      }
    }

    if(SelHold != CntrSel)
    {
      SelHold ^= 1;
      if(SelHold)
      {
        GLCD_print("\fContrast adj.\r");
      }
      else
      {
        GLCD_print("\fBacklight adj.\r");
      }
    }
  }
}
