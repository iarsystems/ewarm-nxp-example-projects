/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : January 15, 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR KSK-i.MX25 board. It shows basic use
 * of I/O, system initialization (PLL, MAX, AIPI, EMI, MMU, AITC), timers
 * and UART1 (CN3). The example return echo on every received characher, LED0
 * indicate framing error, LED1 indicate overrun erro and LED2 indicate brake.
 *
 *  Terminal settings:
 *   Communication speed: 115200 bps
 *   Number of data bits:    8
 *   Number of stop bits:    1
 *   Parity:              none
 *
 *  Set the jumpers and the switches regarding the IAR KSK-i.MX25 User's Manual
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_asic.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"
#include "drv_i2c1.h"
#include "drv_MC34704.h"

static volatile Int32U Gpt1Counter;
Int8U  Buffer[100];
Int32U Push;
Int32U Pop;
volatile Boolean FramingError;
volatile Boolean OverrunError;
volatile Boolean BreakDetect;


/*************************************************************************
 * Function Name: GPT1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT1 interrupt handler
 *
 *************************************************************************/
static
void GPT1_Handler(void)
{
  // Clear interrupt flag
  GPT1SR_bit.OF1 = 1;
  ++Gpt1Counter;
}

/*************************************************************************
 * Function Name: UART1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART interrupt handler
 *
 *************************************************************************/
static
void UART1_Handler(void)
{
Int32U count;
  // Errors
  if(USR2_1_bit.BRCD)
  {
    // break
    USR2_1_bit.BRCD = 1;
    BreakDetect = TRUE;
  }
  if(USR1_1_bit.FRAMERR)
  {
    // framing error
    USR1_1_bit.FRAMERR = 1;
    FramingError = TRUE;
  }
  if(USR2_1_bit.ORE)
  {
    // overrun error
    USR2_1_bit.ORE = 0;
    OverrunError = TRUE;
  }
  // receiver part
  if(USR1_1_bit.AGTIM)
  {
    USR1_1_bit.AGTIM = 1;
    // Pop remaing data from RX FIFO
    do
    {
      Buffer[Push++] = URXD_1;
      if(Push == sizeof(Buffer)/sizeof(Buffer[0]))
      {
        Push = 0;
      }
    }
    while(USR2_1_bit.RDR);
  }

  while(USR1_1_bit.RRDY)
  {
    Buffer[Push++] = URXD_1;
    if(Push == sizeof(Buffer)/sizeof(Buffer[0]))
    {
      Push = 0;
    }
  }
  // transmitter part
  if(   UCR1_1_bit.TRDYEN
     && USR1_1_bit.TRDY)
  {
    for(count = 30; count; count--)
    {
      UTXD_1 = Buffer[Pop++];
      if (Pop == sizeof(Buffer)/sizeof(Buffer[0]))
      {
        Pop = 0;
      }
      if(Pop == Push)
      {
        Pop = Push = 0;
        UCR1_1_bit.TRDYEN = 0;    // Transmitter Ready Interrupt disable
        break;
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
 * Description:
 *
 *************************************************************************/
int main(void)
{
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

  // LEDs Init
  IOMUXC_SW_MUX_CTL_PAD_A14_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A15_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A16_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A17_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A18_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A19_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A20_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A21_bit.MUX_MODE = ALT5_MUX_MODE;
  LED_PORT |= LED_MASK;
  LED_DIR  |= LED_MASK;

  // Init AITC
  ASIC_Init();

  // Init GTIM
  Gpt1Counter = 0;
  // Enable GPT1 clock
  CGCR0_bit.ipg_per_gpt  = 1;
  CGCR1_bit.ipg_clk_gpt1 = 1;

  // Disable timer
  GPT1CR = 0;
  // Disable interrupt
  GPT1IR_bit.OF1IE = 0;
  // Reset Timer1
  GPT1CR_bit.SWR = 1;
  while(GPT1CR_bit.SWR);
  // ipg_clk_highfreq - PERCLK5
  GPT1CR_bit.CLKSRC= 2;
  // reset counters
  GPT1CR_bit.ENMOD = 1;
  // Restart counter
  GPT1CR_bit.FRR = 0;

  // Init the Timer's period
  GPT1PR_bit.PRESCALER = 0;
  GPT1OCR1 = HCLK / ((PCDR1_bit.PERDIV5+1) * GPT1_TICK_PER_SEC);
  // Init GPT1 interrupt
  ASIC_SetupIntr(GPT1_Handler,FALSE,INT_GPT1,1);
  ASIC_EnableIntSource(INT_GPT1);
  // Enable GPT1
  GPT1CR_bit.EN = 1;
  // Enable Oput compare 1 interrupt
  GPT1IR_bit.OF1IE = 1;

  I2C1_Init();

  // UART Init
  // Enable UART1 clock
  CGCR0_bit.ipg_per_uart  = 1;
  CGCR2_bit.ipg_clk_uart1 = 1;

  UCR2_1_bit.SRST = 0;
  while(!UCR2_1_bit.SRST);
  UCR1_1_bit.RRDYEN = 1;    // Receiver Ready Interrupt Enable
  UCR2_1_bit.RXEN = 1;      // receiver enable
  UCR2_1_bit.TXEN = 1;      // Transmitter enable
  UCR2_1_bit.ATEN = 1;      // Aging Timer Enable
  UCR2_1_bit.WS = 1;        // 8 bits word
  UCR2_1_bit.STPB = 0;      // 1 stop bit
  UCR2_1_bit.PREN = 0;      // no parity
  UCR2_1_bit.IRTS = 1;      // flowcontrol disable
  UCR3_1_bit.RXDMUXSEL = 1; // RXD Muxed Input Selected
  UCR3_1_bit.FRAERREN = 1;  // enable framing error
  UCR4_1_bit.OREN = 1;      // enable overrun error
  UCR4_1_bit.BKEN = 1;      // enable break condition detection interrupt
  UFCR_1_bit.RXTL = 1;      // RxFIFO has 1 character
  UFCR_1_bit.TXTL = 2;      // TxFIFO has 2 or fewer characters
  UFCR_1_bit.RFDIV = 5;     // Divide input clock by 1
  UFCR_1_bit.DCEDTE = 0;    // Select data communication equipment (DCE)

  ONEMS_1 =  HCLK /((PCDR3_bit.PERDIV15+1) + 1000);
  UBIR_1 = 1152-1;
  UBMR_1 = 41562-1;
  USR1_1 = 0xFFFFFFFF;      // clear pending interrupts
  USR2_1 = 0xFFFFFFFF;      // clear pending interrupts

  IOMUXC_SW_MUX_CTL_PAD_UART1_RXD_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_UART1_TXD_bit.MUX_MODE = ALT0_MUX_MODE;

  // Init UART1 interrupt
  ASIC_SetupIntr(UART1_Handler,FALSE,INT_UART1,1);
  ASIC_EnableIntSource(INT_UART1);

  UCR1_1_bit.UARTEN = 1;    // enable UART
  Push = 0;
  Pop = 0;

  // Enable interrupts
  ASIC_EnableIRQ();
  ASIC_EnableFIQ();
  __enable_interrupt();

  // MC34704 init
  MC34704_Init();

  while(1)
  {
    if(   Push
       && !Pop)
    {
      // begin transmit
      UCR1_1_bit.TRDYEN = 1;    // Transmitter Ready Interrupt Enable
    }
    if(Gpt1Counter >= 8)
    {
      Gpt1Counter = 0;
      LED_PORT |= LED_MASK;
      if(FramingError)
      {
        FramingError = FALSE;
        LED_PORT &= ~LED0;
      }
      if(OverrunError)
      {
        OverrunError = FALSE;
        LED_PORT &= ~LED1;
      }
      if(BreakDetect)
      {
        BreakDetect = FALSE;
        LED_PORT &= ~LED2;
      }
    }
  }
}
