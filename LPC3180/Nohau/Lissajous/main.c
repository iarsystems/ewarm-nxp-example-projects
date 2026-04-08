/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 20, January 2006
 *       Author      : Stanimir Bonev
 *       Description : Main module
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the NOHAU LPC3100 evaluation board. It shows
 * basic use of LCD, the interrupt controllers and mathematics routine.
 *  It starts by show of Lissajous figures on the LCD. The button S3 and
 * the button S2 change the phase between signals and the button S4 and
 * the button S5 the frequency.
 *  The jumpers settings are regarding Nohau LPC3100 user manual
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <math.h>
#include <stdbool.h>
#include <nxp/iolpc3180.h>

// System OSC 13MHz
#define OSC           (13000000UL)
// ARM_CLK 208MHz
#define CORE_CLK      (OSC*16)
// HCLK 104MHz
#define AHB_CLK       (CORE_CLK/2)
// PER_CLK 13MHz
#define PER_CLK       (CORE_CLK/16)
// RTC_CLK
#define RTC_CLK       (32768UL)
// I2C Freq
#define I2C_FREQ      100000UL
// I2C Def
#define START_COND    (1UL<<8)
#define STOP_COND     (1UL<<9)
// LCD Slave Add
#define LCD_SLAVE_ADD (0x3DUL)
// LCD Slave Add
#define LCD_WRITE     ((LCD_SLAVE_ADD << 1) | START_COND)
// LCD Slave Add
#define LCD_READ      ((LCD_SLAVE_ADD << 1) | 1 | START_COND)

#define MAX_COUNT   (360UL*10)
#define M_PI        (3.14159265358979323846f)

#define X_LCD       (96UL)
#define Y_LCD       (40UL)
#define X_MAX       ((X_LCD/2) - 1)
#define Y_MAX       ((Y_LCD/2) - 1)

#define LCD_PAGE_S  (X_LCD*Y_LCD)/(8*sizeof(unsigned int))

#define DELTA_P     (M_PI/(2*20))
#define DELTA_F     (0.05f)

volatile float P = M_PI/2,F = 2.05f;
unsigned int LcdPage0[LCD_PAGE_S];

/*************************************************************************
 * Function Name: Gpi06_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_06 handler
 *
 *************************************************************************/
static void Gpi06_handler (void)
{
  P -= DELTA_P;
  SIC2_RSR_bit.GPI_06 = 1;              // clear interrupt flag (for edge only)
}

/*************************************************************************
 * Function Name: Gpi04_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_04 handler
 *
 *************************************************************************/
static void Gpi04_handler (void)
{
  P += DELTA_P;
  SIC2_RSR_bit.GPI_04 = 1;              // clear interrupt flag (for edge only)
}
/*************************************************************************
 * Function Name: Gpi07_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_07 handler
 *
 *************************************************************************/
static void Gpi07_handler (void)
{
  F -= DELTA_F;
  SIC2_RSR_bit.GPI_07 = 1;              // clear interrupt flag (for edge only)
}
/*************************************************************************
 * Function Name: Gpi11_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_11 handler
 *
 *************************************************************************/
static void Gpi11_handler (void)
{
  F += DELTA_F;
  SIC1_RSR_bit.GPI_11 = 1;              // clear interrupt flag (for edge only)
}

/*************************************************************************
 * Function Name: Sir1Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sub1 Interrupt Controller handler
 *
 *************************************************************************/
static void Sic1Handler (void)
{
  if (SIC1_SR_bit.GPI_11)
  {
    Gpi11_handler();
  }
}

/*************************************************************************
 * Function Name: Sir2Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sub2 Interrupt Controller handler
 *
 *************************************************************************/
static void Sic2Handler (void)
{
  if (SIC2_SR_bit.GPI_04)
  {
    Gpi04_handler();
  }
  else if (SIC2_SR_bit.GPI_06)
  {
    Gpi06_handler();
  }
  else if (SIC2_SR_bit.GPI_07)
  {
    Gpi07_handler();
  }
}

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void irq_handler (void)
{
  if (MIC_SR_bit.Sub1IRQn)
  {
    Sic1Handler();
  } else if (MIC_SR_bit.Sub2IRQn)
  {
    Sic2Handler();
  }
}

/*************************************************************************
 * Function Name: ClockInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Pll, HCLK, PERIPH_CLK,
 *  Pll_clk_out = Osc * 16         = 208MHz
 *  HCLK        = 1/2 Pll_clk_out  = 104MHz
 *  PERIPH_CLK  = 1/16 Pll_clk_out = 13MHz
 *
 *************************************************************************/
void ClockInit (void)
{
  // Set Clk dividers
  HCLKDIV_CTRL_bit.HCLK = 2-1;        // 1/2  Pll_clk_out
  HCLKDIV_CTRL_bit.PERIPH_CLK = 16-1; // 1/16 Pll_clk_out
  // PLL Init - OSC * 16 = 208MHz
  HCLKPLL_CTRL_bit.BYPASS   = 0;      // OSC connected to PLL input
  HCLKPLL_CTRL_bit.DIRECT   = 1;      // CCO connected to PLL_Clk output
  HCLKPLL_CTRL_bit.FEEDBACK = 0;      // CC0 connected to N divider input
  HCLKPLL_CTRL_bit.N = 0;             // set divide
  HCLKPLL_CTRL_bit.M = 16;            // set multiplier
  HCLKPLL_CTRL_bit.POWER_DOWN = 1;    // Enable Pll
  // Wait until PLL lock
  while(!HCLKPLL_CTRL_bit.PLL_LOCK);
  // Connect Pll_clk_out
  PWR_CTRL_bit.RUN_MODE = 1;
}

/*************************************************************************
 * Function Name: I2cWrite
 * Parameters: none
 *
 * Return: none
 *
 * Description: Write in I2C1
 *
 *************************************************************************/
void I2cWrite(unsigned data)
{
  while(I2C1_STS_bit.TFF);
  I2C1_TX = data;
}

/*************************************************************************
 * Function Name: LcdClearPage
 * Parameters: none
 *
 * Return: none
 *
 * Description: Clear LCD Page
 *
 *************************************************************************/
void LcdClearPage (void)
{
  for(int i = 0; i < LCD_PAGE_S; i++)
  {
    LcdPage0[i] = 0;
  }
}

/*************************************************************************
 * Function Name: LcdTransferPage
 * Parameters: none
 *
 * Return: none
 *
 * Description: Transfer page to LCD
 *
 *************************************************************************/
void LcdTransferPage (void)
{
unsigned int Temp;
  I2cWrite(LCD_WRITE);
  I2cWrite(0x68);
  I2cWrite(0x0 );
  for(int i = 0; i < LCD_PAGE_S-1; i++)
  {
    Temp = LcdPage0[i];
    I2cWrite( Temp        & 0xFF);
    I2cWrite((Temp >> 8 ) & 0xFF);
    I2cWrite((Temp >> 16) & 0xFF);
    I2cWrite((Temp >> 24) & 0xFF);
  }
  Temp = LcdPage0[LCD_PAGE_S-1];
  I2cWrite( Temp        & 0xFF);
  I2cWrite((Temp >> 8 ) & 0xFF);
  I2cWrite((Temp >> 16) & 0xFF);
  I2cWrite(((Temp >> 24) & 0xFF)|STOP_COND);
}

/*************************************************************************
 * Function Name: PainPoint
 * Parameters: unsigned int X,unsigned int Y
 *
 * Return: bool
 *
 * Description: Paint point in LCD page
 *
 *************************************************************************/
bool PainPoint (unsigned int X,unsigned int Y, bool Color)
{
unsigned int Temp;
  if((X >= X_LCD) || (Y >= Y_LCD))
  {
    return(false);
  }
  // Calculate bit number
  Temp = (Y_LCD-Y) + X*Y_LCD;
  if(Color)
  {
    LcdPage0[Temp/(8*sizeof(unsigned int))] |=\
                 1 << (Temp%(8*sizeof(unsigned int)));
  }
  else
  {
    LcdPage0[Temp/(8*sizeof(unsigned int))] &=\
               ~(1 << (Temp%(8*sizeof(unsigned int))));
  }
  return(true);
}

/*************************************************************************
 * Function Name: LcdInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init LCD graph mode
 *
 *************************************************************************/
void LcdInit (void)
{
  // Init I2C
  // Init I2C
  I2CCLK_CTRL_bit.I2C1_CLK_ENA = 1;  // Enable I2C clock
  // Set I2C SDA frequency
  I2C1_CLK_HI = AHB_CLK/(I2C_FREQ*2);
  I2C1_CLK_LO = AHB_CLK/(I2C_FREQ*2);
  // Clear LCD and set text mode
  I2cWrite(0x17A);
  I2cWrite(0x08);
  I2cWrite(0x20);
  // Clear LCD Page0
  LcdClearPage();
}

/*************************************************************************
 * Function Name: ButInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Buttons
 *
 *************************************************************************/
void ButInit (void)
{
  // Init button (S5 - GPI11) Frequency increase
  SIC1_APR_bit.GPI_11     = 0;      // on a falling edge signal
  SIC1_ATR_bit.GPI_11     = 1;      // Set edge sensitive interrupt
  SIC1_ITR_bit.GPI_11     = 0;      // assign to IRQ line
  SIC1_ER_bit.GPI_11      = 1;      // Enable GPI_06 interrupt
  // Init button (S4 - GPI7) Frequency decrease
  SIC2_APR_bit.GPI_07     = 0;      // on a falling edge signal
  SIC2_ATR_bit.GPI_07     = 1;      // Set edge sensitive interrupt
  SIC2_ITR_bit.GPI_07     = 0;      // assign to IRQ line
  SIC2_ER_bit.GPI_07      = 1;      // Enable GPI_06 interrupt
  // Init button (S2 - GPI4) Phase increase
  SIC2_APR_bit.GPI_04     = 0;      // on a falling edge signal
  SIC2_ATR_bit.GPI_04     = 1;      // Set edge sensitive interrupt
  SIC2_ITR_bit.GPI_04     = 0;      // assign to IRQ line
  SIC2_ER_bit.GPI_04      = 1;      // Enable GPI_06 interrupt
  // Init button (S3 - GPI6) Phase decrease
  SIC2_APR_bit.GPI_06     = 0;      // on a falling edge signal
  SIC2_ATR_bit.GPI_06     = 1;      // Set edge sensitive interrupt
  SIC2_ITR_bit.GPI_06     = 0;      // assign to IRQ line
  SIC2_ER_bit.GPI_06      = 1;      // Enable GPI_06 interrupt
  // Init SIR2 IRQ interrupt
  MIC_APR_bit.Sub1IRQn    = 0;      // on a low level signal
  MIC_ATR_bit.Sub1IRQn    = 0;      // Set level sensitive interrupt
  MIC_ITR_bit.Sub1IRQn    = 0;      // assign to IRQ line
  MIC_ER_bit.Sub1IRQn     = 1;      // Enable SIC1 interrupts
  // Init SIR2 IRQ interrupt
  MIC_APR_bit.Sub2IRQn    = 0;      // on a low level signal
  MIC_ATR_bit.Sub2IRQn    = 0;      // Set level sensitive interrupt
  MIC_ITR_bit.Sub2IRQn    = 0;      // assign to IRQ line
  MIC_ER_bit.Sub2IRQn     = 1;      // Enable SIC2 interrupts

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
int X,Y;
unsigned int i,j;
  // Init Clocks
  ClockInit();
  // Init LCD
  LcdInit();
  // Init Buttons
  ButInit();
  __enable_interrupt();
  // main loop
  i = 0;
  j = MAX_COUNT;
  while(1)
  {
    float P_Hold = P,F_Hold = F;
    for(; i < j; ++i)
    {
      X = (int)(X_MAX*cos (2*M_PI*i/(float)MAX_COUNT))               + X_MAX+1;
      Y = (int)(Y_MAX*cos((2*M_PI*i/(float)MAX_COUNT)*F_Hold+P_Hold))+ Y_MAX+1;
      PainPoint(X,Y,1);
    }
    j += MAX_COUNT;
    LcdTransferPage();
    LcdClearPage();
  };
} // main(void)
