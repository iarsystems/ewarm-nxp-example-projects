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
 *    1. Date        : 12, September 2006
 *       Author      : Stanimir Bonev
 *       Description :
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Phytec LPC3250 evaluation board. It shows
 * basic use of CP15, I/O, timer, uart and the interrupt controllers.
 * It starts by blinking LED1 and LED2. The button BTN1 change
 * blinking speed while the button BTN2 change the pattern.  The button
 * action will be displayed to the U5 BD9 connector at 15200-baud, 8-bit,
 * 1-stop and no parity/flow control.
 *  The jumpers settings are regarding Phytec LPC3250 user manual (default)
 *
 *    $Revision: 28 $
 **************************************************************************/


#include <intrinsics.h>
#include <nxp/iolpc3250.h>
#include <stdio.h>
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"


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

// TICK_PER_SECOND
#define TICK_SEC_S1   (100  * (PER_CLK/(13*1000)))
#define TICK_SEC_S1_5 (150  * (PER_CLK/(13*1000)))
#define TICK_SEC_S3   (300  * (PER_CLK/(13*1000)))
#define TICK_SEC_1S   (1000 * (PER_CLK/(13*1000)))

#define LED1          (1UL << 1)
#define LED2          (1UL << 14)

#define BUTTON_DLY    5000

#define BAUD          115200

volatile int ButtonDly = 0;
volatile int BTN2_count = 0;
#define TX_BUFFER_SIZE 2000
char TX_Buffer[TX_BUFFER_SIZE];
volatile int TX_Buffer_Start = 0, TX_Buffer_End = 0;
/*************************************************************************
 * Function Name: HSTimerHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: High speed timer handler
 *
 *************************************************************************/
static void HSTimerHandler (void)
{
static int count;
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATHC0 interrupt flag

  switch (BTN2_count)
  {
    case 0:
      if(++count & 1)
      {
        P3_OUTP_CLR = LED1;
        P3_OUTP_SET = LED2;
      }
      else
      {
        P3_OUTP_CLR = LED2;
        P3_OUTP_SET = LED1;
      }
      break;
    case 1:
      if(++count & 1)
      {
        P3_OUTP_CLR = LED1;
        P3_OUTP_SET = LED2;
      }
      else
      {
        P3_OUTP_CLR = LED2 | LED1;
      }
      break;
    case 2:
      if(++count & 1)
      {
        P3_OUTP_CLR = LED1;
        P3_OUTP_SET = LED2;
      }
      else
      {
        P3_OUTP_SET = LED2 | LED1;
      }
      break;
  };
}

/*************************************************************************
 * Function Name: Gpi03_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_03 handler
 *
 *************************************************************************/
static void Gpi03_handler (void)
{
  ButtonDly = BUTTON_DLY;
  printf("BTN1 button is pressed\r\n");
  SIC2_RSR_bit.GPI_03 = 1;              // clear interrupt flag (for edge only)
}

/*************************************************************************
 * Function Name: Gpi02_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPI_02 handler
 *
 *************************************************************************/
static void Gpi02_handler (void)
{
  BTN2_count = (BTN2_count + 1) % 3;
  printf("BTN2 button is pressed\r\n");
  SIC2_RSR_bit.GPI_02 = 1;              // clear interrupt flag (for edge only)
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
  if (SIC2_SR_bit.GPI_03)
  {
    Gpi03_handler();
  }
  if (SIC2_SR_bit.GPI_02)
  {
    Gpi02_handler();
  }
}

/*************************************************************************
 * Function Name: Uart5Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART 5 handler
 *
 *************************************************************************/
static void Uart5Handler (void)
{
  int i = TX_Buffer_End;
  switch (U5IIR & 0x0F)
  {
  case 0x6:
    break;
  case 0x4:
    break;
  case 0xC:
    break;
  case 0x2:
    if(i != TX_Buffer_Start)
    {
      i = (i + 1) % TX_BUFFER_SIZE;
      TX_Buffer_End = i;
      U5THR = TX_Buffer[i];
    }
    break;
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
  if(MIC_SR_bit.HSTIMER_INT)
  {
    HSTimerHandler();
  }
  else if (MIC_SR_bit.IIR5)
  {
    Uart5Handler();
  }
  else if (MIC_SR_bit.Sub2IRQn)
  {
    Sic2Handler();
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
int Speed = 0;

  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

  // Disable all interrupts
  MIC_ER  = 0;
  SIC1_ER = 0;
  SIC2_ER = 0;

  // Set LEDs off (LED1 - GPO1  LED2 - GPO14)
  P3_OUTP_CLR = LED1 | LED2;

  // Init High speed timer
  TIMCLK_CTRL_bit.HST_CLK_ENA= 1;   // HSTimer clock enable
  HSTIM_CTRL_bit.COUNT_ENAB  = 0;   // Stop counting
  HSTIM_MCTRL_bit.MR0_INT    = 1;   // unmask MATCH0 intr flag
  HSTIM_MCTRL_bit.RESET_COUNT0=1;   // Enable reset of Timer Counter on Match 0
  HSTIM_MCTRL_bit.STOP_COUNT0 =0;   // Disable the stop functionality on Match 0
  HSTIM_PMATCH = 13-1;              // Set prescaler 1us
  HSTIM_CTRL_bit.RESET_COUNT = 1;   // Reset the counter
  while(HSTIM_COUNTER);
  HSTIM_CTRL_bit.RESET_COUNT = 0;   // release reset of the counter
  HSTIM_CTRL_bit.PAUSE_EN = 1;      // stop the counter in debug mode
  HSTIM_MATCH0 = TICK_SEC_S1;       // set period
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATHC0 interrupt flag
  // Init timer interrupts
  MIC_APR_bit.HSTIMER_INT    = 1;   // Interrupt is generated on a high level signal or rising edge
  MIC_ATR_bit.HSTIMER_INT    = 0;   // Interrupt is level sensitive
  MIC_ITR_bit.HSTIMER_INT    = 0;   // The interrupt is routed to the IRQ output of the interrupt controller
  MIC_ER_bit.HSTIMER_INT     = 1;   // Enable high speed timer interrupts
  HSTIM_CTRL_bit.COUNT_ENAB  = 1;   // Enable counting

  // Init button (BTN1 - GPI3)
  // Init GPI_03
  SIC2_APR_bit.GPI_03     = 1;      // Interrupt is generated on a high level signal or rising edge
  SIC2_ATR_bit.GPI_03     = 1;      // Interrupt is edge sensitive
  SIC2_ITR_bit.GPI_03     = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  SIC2_RSR_bit.GPI_03     = 1;      // clear interrupt flag (for edge only)
  SIC2_ER_bit.GPI_03      = 1;      // Enable GPI_03 interrupt

  // Init button (BTN2 - GPI2)
  // Init GPI_02
  SIC2_APR_bit.GPI_02     = 1;      // Interrupt is generated on a high level signal or rising edge
  SIC2_ATR_bit.GPI_02     = 1;      // Interrupt is edge sensitive
  SIC2_ITR_bit.GPI_02     = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  SIC2_RSR_bit.GPI_02     = 1;      // clear interrupt flag (for edge only)
  SIC2_ER_bit.GPI_02      = 1;      // Enable GPI_02 interrupt

  // Init SIR2 IRQ interrupt
  MIC_APR_bit.Sub2IRQn    = 0;      // Interrupt is generated on a low level signal or falling edge
  MIC_ATR_bit.Sub2IRQn    = 0;      // Interrupt is level sensitive
  MIC_ITR_bit.Sub2IRQn    = 0;      // The interrupt is routed to the IRQ output of the interrupt controller
  MIC_ER_bit.Sub2IRQn     = 1;      // Enable SIC2 interrupts

  // Init UART5
  UARTCLK_CTRL_bit.UART5_CLK_ENA = 1;
  U5CLK_bit.X = 1;
  U5CLK_bit.Y = 1;
  U5CLK_bit.CLK_SEL = 0;            // Select Periph Clk
  U5LCR_bit.DLAB = 1;               // Enable access to the baud rate Divisor Latches
  U5DLL = PER_CLK / BAUD / 16;
  U5DLM = (PER_CLK / BAUD / 16)>>8;
  U5LCR = 0x3;                      // 8 Data bits, 1 stop bits, no parity
  U5FCR = 0;
#if 1
  U5FCR_bit.FCRFE = 1;              // Enable RX and TX FIFO
  U5FCR_bit.RTLS = 3;               // 60 RX FIFO characters
  U5FCR_bit.TTLS = 3;               // 16 TX FIFO characters
  U5FCR_bit.FCTRL = 1;
  U5FCR_bit.TFR = 1;                // Reset TX FIFO
  U5FCR_bit.RFR = 1;                // Reset RX FIFO
#endif
  U5IER = 0;
  U5IER_bit.RDAIE = 1;              // Enable Receive Data Available (RDA) interrupt
  U5IER_bit.THREIE = 1;             // Enable Transmit Holding Register Empty (THRE) interrupt
  U5IER_bit.RXLSIE = 1;             //
  UART_CTRL_bit.UART5_MODE = 0;     // route to U5_TX and U5_RX
  UART_CLKMODE_bit.UART5_CLK = 2;   // auto clock

  // Init UART5 interrupts
  MIC_APR_bit.IIR5    = 1;          // Interrupt is generated on a high level signal or rising edge
  MIC_ATR_bit.IIR5    = 0;          // Interrupt is level sensitive
  MIC_ITR_bit.IIR5    = 0;          // The interrupt is routed to the IRQ output of the interrupt controller
  MIC_ER_bit.IIR5     = 1;          // Enable UART 5 interrupt

  __enable_interrupt();

  // main loop
  while(1)
  {
    if(ButtonDly)
    {
      // Button Dly
      for(;ButtonDly;--ButtonDly);
      // Change Speed
      // Stop and reset the HS timer counter
      HSTIM_CTRL_bit.COUNT_ENAB  = 0;
      HSTIM_CTRL_bit.RESET_COUNT = 1;
      while(HSTIM_COUNTER);
      HSTIM_CTRL_bit.RESET_COUNT = 0;
      switch (++Speed)
      {
      case 1:
        HSTIM_MATCH0 = TICK_SEC_S3;         // Set timer period
        break;
      case 2:
        HSTIM_MATCH0 = TICK_SEC_S1_5;       // Set timer period
        break;
      default:
        Speed        = 0;
        HSTIM_MATCH0 = TICK_SEC_S1;         // Set timer period
      }
      HSTIM_CTRL_bit.COUNT_ENAB  = 1;       // Enable counting
    }
  };
} // main(void)


int MyLowLevelPutchar(int x)
{
  int i = TX_Buffer_Start;
  int count;

  if((i == TX_Buffer_End) && (U5LSR_bit.THRE == 1))
    U5THR = x;
  else
  {
    i = (i + 1) % TX_BUFFER_SIZE;
    count = 0;
    while(i != TX_Buffer_End)
    {
      TX_Buffer[i] = x;
      TX_Buffer_Start = i;
      count++;
      if(count > 0x1000)
        break;
    }
  }

  return 0;
}

