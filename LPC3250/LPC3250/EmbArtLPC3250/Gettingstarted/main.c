/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : September, 2010
 *       Author      : Stoyan Choynev
 *       Description :
 *
 *  This example project shows how to use IAR Embedded Workbench for ARM
 *  to develop code for the Embedded Artists LPC3250 evaluation board. It shows
 *  the basic use of I/O, Timers, ADC, Interrup controller and DDRAM. The project
 *  also shows how to boot the system from the NAND Flash. It starts with blinking
 *  the LED1 on the LPC3250 OEM board. The blinking period can be changed by 
 *  the trimer.
 *   
 *COMPATIBILITY
 *=============
 *
 *   The Getting Started example project is compatible with Embedded Artsists
 *  LPC3250 evaluation board. By default the project is configured to use the 
 *  J-Link JTAG interface.
 *
 *CONFIGURATION
 *=============
 *
 *  The Project contains the following configurations:
 *
 *  Debug_iRAM: This configuration is intended for debugging in the
 *              microcontroller's internal RAM.
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <nxp/iolpc3250.h>
#include <stdio.h>
#include "math.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"
#include "drv_intc.h"
#include "stdint.h"
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

#define LED1          (1UL << 1)
#define LED2          (1UL << 14)

#define BAUD          115200

#define   TIMER1_IN_FREQ       13MHZ
#define   TIMER1_TICK_MAX      1000   //in miliseconds
#define   TIMER1_TICK_MIN      20     //in miliseconds

#define   ADC_MAX              1024

#define   EXP_COEF             0.0038 //it is ln(TIMER1_TICK_MAX/TIMER1_TICK_MIN)/ADC_MAX
#define   TMR1TICK(tick)       ((TIMER1_IN_FREQ/1000)*tick)/13 //Convert miliseconds in Tmr ticks

/** external data **/
Int32U Tmr1Tick = TMR1TICK(100);
/** internal functions **/
void DDRAM_Init(void);
/*************************************************************************
 * Function Name: ADCIntrHandler
 * Parameters: None
 *
 * Return: None
 *
 * Description: ADC Interrupt service routine. Take Cannel 2 result.
 *              Calculate Tmr1Tick. Start new AD Conversion
 *
 *************************************************************************/
void ADCIntrHandler (void)
{
volatile Int32U Result;

     
    Result = ADC_VALUE & 0x3FF;
    
   /*Timer period is linear funcion of ADC value.*/
    //Int32U tmp = (Result*(TIMER1_TICK_MAX-TIMER1_TICK_MIN)/ADC_MAX+TIMER1_TICK_MIN);
   /*Timer period is quadrature funcion of ADC value.*/ 
    //Int32U tmp = (Result*Result*(TIMER1_TICK_MAX-TIMER1_TICK_MIN))/(ADC_MAX*ADC_MAX)+TIMER1_TICK_MIN;
   /*Timer period is exponential funcion of ADC value.*/ 
    Int32U tmp = (Int32U)(TIMER1_TICK_MIN*exp(EXP_COEF*Result));

    Tmr1Tick = TMR1TICK(tmp);

    ADC_CTRL_bit.STROBE = 1;           //Start AD Conversion
   
}/*************************************************************************
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
  HSTIM_MATCH0 = Tmr1Tick;         // Set timer period
  if(++count & 1)
  {
    P3_OUTP_CLR = LED1;
  }
  else
  {
    P3_OUTP_SET = LED1;
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
  switch (U5IIR & 0x0F)
  {
  case 0x6:
    break;
  case 0x4:
        U5THR = U5RBR;
    break;
  case 0xC:
    break;
  case 0x2:
    break;
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

  //Interrupt Controller Init
  INTC_Init();

  // Set LEDs off (LED1 - GPO1  LED2 - GPO14)
  P3_OUTP_CLR = LED1 | LED2;

  // Init High speed timer
  TIMCLK_CTRL_bit.HST_CLK_ENA= 1;   // HSTimer clock enable
  HSTIM_CTRL_bit.COUNT_ENAB  = 0;   // Stop counting
  HSTIM_MCTRL_bit.MR0_INT    = 1;   // unmask MATCH0 intr flag
  HSTIM_MCTRL_bit.RESET_COUNT0=1;   // Enable reset of Timer Counter on Match 0
  HSTIM_MCTRL_bit.STOP_COUNT0 =0;   // Enable stop functionality on Match 0
  HSTIM_PMATCH = 13-1;              // Set prescaler 1us
  HSTIM_CTRL_bit.RESET_COUNT = 1;   // Reset the counter
  while(HSTIM_COUNTER);
  HSTIM_CTRL_bit.RESET_COUNT = 0;   // release reset of the counter
  HSTIM_CTRL_bit.PAUSE_EN = 1;      // stop the counter in debug mode
  HSTIM_MATCH0 = Tmr1Tick;          // set period
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATHC0 interrupt flag
  //Init timer interrupts
  MIC_IRQInstall(HSTimerHandler,    //ISR
                 MainHSTIMER_INT,   //Interrupt index
                 HIGH,              // Interrupt is generated on a high level signal or rising edge
                 LEVEL,             // Interrupt is level sensitive
                 TYPE_IRQ);         // The interrupt is routed to the IRQ output of the interrupt controller

  MIC_IntEnable(MainHSTIMER_INT, ENABLE_IRQ);
 
  HSTIM_CTRL_bit.COUNT_ENAB  = 1;   // Enable counting
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
#if 0
  U5FCR_bit.FCRFE = 1;              // Enable RX and TX FIFO
  U5FCR_bit.RTLS = 3;               // 60 RX FIFO characters
  U5FCR_bit.TTLS = 3;               // 16 TX FIFO characters
  U5FCR_bit.FCTRL = 1;
  U5FCR_bit.TFR = 1;                // Reset TX FIFO
  U5FCR_bit.RFR = 1;                // Reset RX FIFO
#endif
  U5IER = 0;
  U5IER_bit.RDAIE = 1;              // Enable Receive Data Available (RDA) interrupt
  U5IER_bit.THREIE = 0;             // Enable Transmit Holding Register Empty (THRE) interrupt
  U5IER_bit.RXLSIE = 0;             //
  UART_CTRL_bit.UART5_MODE = 0;     // route to U5_TX and U5_RX
  UART_CLKMODE_bit.UART5_CLK = 2;   // auto clock

  // Init UART5 interrupts
  MIC_IRQInstall(Uart5Handler,      //ISR
                 MainIIR5,          //Interrupt index
                 HIGH,              // Interrupt is generated on a high level signal or rising edge
                 LEVEL,             // Interrupt is level sensitive
                 TYPE_IRQ);         // The interrupt is routed to the IRQ output of the interrupt controller

  MIC_IntEnable(MainIIR5, ENABLE_IRQ);
 
  /*ADC initialization*/
  ADCLK_CTRL = 0;                   /*ADC RTC clock disable*/
  ADCLK_CTRL1_bit.ADCCLK_SEL = 1;    /*ADC clock from PERIPH_CLK*/
  ADCLK_CTRL1_bit.ADC_FREQ = 265-1; /*ADC clock devider 256*/
  ADC_SELECT_bit.IN_SEL = 2;        /*Select ADIN2*/
  ADC_SELECT_bit.P_Ref = 2;         /*Select VDD*/
  ADC_SELECT_bit.N_Ref = 2;         /*Select VSS*/
  ADC_CTRL_bit.PDN_CTRL = 1;        /*Power UP ADC*/
  ADC_VALUE;  /*Dummy read to clear interrupt*/
  
  SIC1_IRQInstall(ADCIntrHandler,   //ISR
                 Sub1ADC_INT,       //Interrupt index
                 HIGH,              // Interrupt is generated on a high level signal or rising edge
                 LEVEL,             // Interrupt is level sensitive
                 TYPE_IRQ);         // The interrupt is routed to the IRQ output of the interrupt controller

  SIC1_IntEnable(Sub1ADC_INT, ENABLE_IRQ);
  
  ADC_CTRL_bit.STROBE = 1;           //Start AD Conversion
  
  __enable_interrupt();

  // main loop
  while(1)
  {
  };
} // main(void)
