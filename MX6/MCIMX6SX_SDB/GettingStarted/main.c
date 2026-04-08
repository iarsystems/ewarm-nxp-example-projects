/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *   to develop code for Freescale MCIMX6SX-SDB board. It shows basic use of the
 *   GIC, the Private Timer and the UART module.
 *
 *    $Revision: 7082 $
 **************************************************************************/
/** include files **/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <intrinsics.h>
#include <yfuns.h>
#include <NXP/iMX6SX_A9.h>
#include "drv_gic.h"
#include "drv_A9_timers.h"
/** local definitions **/
/*ARM clock*/
#define ARM_CLK     (792000000UL)
/*Private Timer Prescaler*/
#define PTIM_PRESCALE  (256)
/*Timer ticks per second*/
#define TICKS          (1)
/*Private Timer Reload value*/
#define PTIM_LOAD   ((ARM_CLK/2)            /*Timer Runs @ ARM_CLK/2*/\
                    /PTIM_PRESCALE          /*Prescaler*/\
                    /TICKS)                 /*Ticks*/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
static void PTMR_Handler(void);
static void UART1_Init(void);
static int MyLowLevelGetchar(void);

/** public data **/

/** private data **/
static uint32_t tick;
static uint32_t cntr;

/** public functions **/

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
  /*cpu disable IRQ*/
  __disable_irq();
  /*Interrupt controller*/
  GIC_Init(0);
  /*Private timer Init*/
  PTMR_Init(PTIM_LOAD,PTIM_PRESCALE-1, PTMR_CTLR_AUTO_RELOAD |
                                       PTMR_CTLR_IRQ_ENABLE);
  /*configure Private Timer interrupt*/
  GIC_PpiIntInstall(PTMR_Handler,29,0x78);
  /*Enable Private Timer interrupt*/
  GIC_IntEnable(29,1);
  /*cpu enable IRQ*/
  __enable_irq();
  /*UART Initialization*/
  UART1_Init();
  /*Start Private Timer*/
  PTMR_Start();
  /*Print start string*/
  printf("\fMCIMX6SX-SDB GETTING STARTED\n\r\n\r");

  while(1)
  {
     if(tick)
     {
       tick = 0;
       /*Print counted seconds*/
       printf("Cortex A9 Seconds Count: %d\r", ++cntr);
     }
  }
}

/*************************************************************************
 * Function Name: __write
 * Parameters: int Handle, const unsigned char * Buf, size_t Bufsize
 *
 * Return: size_t
 *
 * Description: Low Level character output
 *
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while(UART1_UTS & (1<<UART_UTS_TXFULL_SHIFT));  //Wait
    UART1_UTXD = *Buf++;
    ++nChars;
  }
  return nChars;
}
/*************************************************************************
 * Function Name: __read
 * Parameters: int handle, unsigned char * buffer, size_t size
 *
 * Return: size_t
 *
 * Description: Low Level character input
 *
 *************************************************************************/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}

/** private functions **/

/*************************************************************************
 * Function Name: PTMR_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Private Timer interrupt handler
 *
 *************************************************************************/
static
void PTMR_Handler(void)
{
  PTMR_ClrIsr();
  tick++;
}

/*************************************************************************
 * Function Name: UART1_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART1 Initialization
 *
 *************************************************************************/
static void UART1_Init(void)
{
uint32_t tmp;
  /*Configure UART1_TX and UART1_RX pins*/
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04 =  (0<<IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04_MUX_MODE_SHIFT) \
                                    | (0<<IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04_SION_SHIFT);
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05 =  (0<<IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05_MUX_MODE_SHIFT) \
                                    | (0<<IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05_SION_SHIFT);

  IOMUXC_UART1_IPP_UART_RXD_MUX_SELECT_INPUT = (1<<IOMUXC_UART1_IPP_UART_RXD_MUX_SELECT_INPUT_DAISY_SHIFT);
  /*Set UART Clocks*/
  tmp = CCM_CSCDR1 & ~(CCM_CSCDR1_uart_clk_podf_MASK | CCM_CSCDR1_uart_clk_sel_MASK);

  CCM_CSCDR1 = tmp | (0<<CCM_CSCDR1_uart_clk_podf_SHIFT)   /*UART clock podf divide by 1*/\
                   | (1<<CCM_CSCDR1_uart_clk_sel_SHIFT);   /*Select Osc as clock source*/

  /*Enable Uart Clocks*/
  tmp = CCM_CCGR5 & ~(CCM_CCGR5_CG12_MASK | CCM_CCGR5_CG13_MASK);
  CCM_CCGR5 = tmp | (1<<CCM_CCGR5_CG12_SHIFT) | (1<<CCM_CCGR5_CG13_SHIFT);
  /*Uart1 Initialization*/
  UART1_UCR1 =   (1<<UART_UCR1_UARTEN_SHIFT)       /*Enable the UART*/\
               | (0<<UART_UCR1_DOZE_SHIFT)         /*The UART is enabled when in DOZE state*/\
               | (0<<UART_UCR1_ATDMAEN_SHIFT)      /*Disable AGTIM DMA request*/\
               | (0<<UART_UCR1_TXDMAEN_SHIFT)      /*Disable transmit DMA request*/\
               | (0<<UART_UCR1_SNDBRK_SHIFT)       /*Do not send a BREAK character*/\
               | (0<<UART_UCR1_RTSDEN_SHIFT)       /*Disable RTSD interrupt*/\
               | (0<<UART_UCR1_TXMPTYEN_SHIFT)     /*Disable the transmitter FIFO empty interrupt*/\
               | (0<<UART_UCR1_IREN_SHIFT)         /*Disable the IR interface*/\
               | (0<<UART_UCR1_RXDMAEN_SHIFT)      /*Disable DMA request*/\
               | (0<<UART_UCR1_RRDYEN_SHIFT)       /*Disables the RRDY interrupt*/\
               | (0<<UART_UCR1_ICD_SHIFT)          /*Idle for more than 4 frames*/\
               | (0<<UART_UCR1_IDEN_SHIFT)         /*Disable the IDLE interrupt*/\
               | (0<<UART_UCR1_TRDYEN_SHIFT)       /*Disable the transmitter ready interrupt*/\
               | (0<<UART_UCR1_ADBR_SHIFT)         /*Disable automatic detection of baud rate*/\
               | (0<<UART_UCR1_ADEN_SHIFT);        /*Disable the automatic baud rate detection interrupt*/


  /*Enable TX, 8Bits, No Parity, Ignore RTS*/
  UART1_UCR2 =   (1<<UART_UCR2_SRST_SHIFT)    /*1 No reset*/\
               | (0<<UART_UCR2_RXEN_SHIFT)    /*Disable the receiver*/\
               | (1<<UART_UCR2_TXEN_SHIFT)    /*Enable the transmitter*/\
               | (0<<UART_UCR2_ATEN_SHIFT)    /*AGTIM interrupt disabled*/\
               | (0<<UART_UCR2_RTSEN_SHIFT)   /*Disable request to send interrupt*/\
               | (1<<UART_UCR2_WS_SHIFT)      /*8-bit transmit and receive character length*/\
               | (0<<UART_UCR2_STPB_SHIFT)    /*The transmitter sends 1 stop bit. The receiver expects 1 or more stop bits*/\
               | (0<<UART_UCR2_PROE_SHIFT)    /*Even parity (Ignored since PREN=0)*/\
               | (0<<UART_UCR2_PREN_SHIFT)    /*Disable parity generator and checker*/\
               | (0<<UART_UCR2_RTEC_SHIFT)    /*Trigger interrupt on a rising edge Ignored (RTSEN = 0)*/\
               | (0<<UART_UCR2_ESCEN_SHIFT)   /*Disable escape sequence detection*/\
               | (0<<UART_UCR2_CTS_SHIFT)     /*The CTS_B pin is high (inactive)*/\
               | (0<<UART_UCR2_CTSC_SHIFT)    /*The CTS_B pin is controlled by the CTS bit*/\
               | (1<<UART_UCR2_IRTS_SHIFT)    /*Ignore the RTS pin*/\
               | (0<<UART_UCR2_ESCI_SHIFT);   /*Disable the escape sequence interrupt*/

  /*RFDIV=1, DCE Mode*/
  UART1_UFCR =    (1<<UART_UFCR_RXTL_SHIFT)   /*RxFIFO has 1 character(default)*/\
                | (0<<UART_UFCR_DCEDTE_SHIFT) /*DCE mode selected*/\
                | (5<<UART_UFCR_RFDIV_SHIFT)  /*Divide input clock by 1*/\
                | (2<<UART_UFCR_TXTL_SHIFT);  /*TxFIFO has 2 or fewer characters(default)*/

  /*RS-485 mode disabled*/
  UART1_UMCR =    (0<<UART_UMCR_MDEN_SHIFT)   /*Normal RS-232*/\
                | (0<<UART_UMCR_SLAM_SHIFT)   /*Select Normal Address Detect mode*/\
                | (0<<UART_UMCR_TXB8_SHIFT)   /*0 will be transmitted as the RS485 9th data bit*/\
                | (0<<UART_UMCR_SADEN_SHIFT)  /*Disable RS-485 Slave Address Detected Interrupt*/\
                | (0<<UART_UMCR_SLADDR_SHIFT);/*RS-485 Slave Address Character*/


  UART1_UCR2 &= ~(1<<UART_UCR2_SRST_SHIFT);       /*Software reset*/
  while(UART1_UTS & (1<<UART_UTS_SOFTRST_SHIFT)); /*Wait Software reset end*/
  /*Set Baud Rate 115200 bps*/
  UART1_UBIR = UART_UBIR_INC(0x03E7);             /* UBIR value 1000 */
  UART1_UBMR = UART_UBMR_MOD(0x32DC);             /* UBMR value 13021 */
}

static int MyLowLevelGetchar(void)
{
int ch;

  if(!(UART1_UTS & (1<<UART_UTS_RXEMPTY_SHIFT)))
  {
    ch = UART1_URXD;
    if(!(ch & (1<<UART_URXD_CHARRDY_SHIFT)) || (ch & (1<<UART_URXD_ERR_SHIFT)))
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff);
  }
  return -1;
}
