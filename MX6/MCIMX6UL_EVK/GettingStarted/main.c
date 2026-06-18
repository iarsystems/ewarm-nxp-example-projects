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
 *   to develop code for NXP MCIMX6UL-EVK board. It shows basic use of the
 *   GIC, the General Purpose Timer and the UART module.
 *
 *    $Revision: 7084 $
 **************************************************************************/
/** include files **/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <intrinsics.h>
#include <yfuns.h>
#include <NXP/iMX6UL.h>
#include "drv_gic.h"
/** local definitions **/
/*ARM clock*/
#define ARM_CLK     (528000000UL)
/*GPTimer Prescaler*/
#define GPTIM_PRESCALE  (240)
/*Timer ticks per second*/
#define TICKS          (1)
/*GPTimer Reload value*/
#define GPTIM_LOAD   ((24000000)         /*Timer Runs @ 24MHz clock*/\
                    /GPTIM_PRESCALE      /*Prescaler*/\
                    /TICKS)              /*Ticks*/
/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
static void GPTimer_Handler(void);
static void GPTimer_Init(void);
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
  /*General Purpose Timer Init*/
  GPTimer_Init();
  /*configure GPT1 interrupt*/
  GIC_SpiIntInstall(GPTimer_Handler,87,0x78,0);
  /*Enable GPT1 Timer interrupt*/
  GIC_IntEnable(87,1);
  /*cpu enable IRQ*/
  __enable_irq();
  /*UART Initialization*/
  UART1_Init();
  /*Start General Purpose Timer*/
  GPT1_CR |= (1<<GPT_CR_EN_SHIFT);    /*Enable GPTimer*/
  /*Print start string*/
  printf("\fMCIMX6UL-EVK GETTING STARTED\n\r\n\r");

  while(1)
  {
     if(tick)
     {
       tick = 0;
       /*Print counted seconds*/
       printf("Cortex A7 Seconds Count: %d\r", ++cntr);
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
 * Function Name: GPTimer_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: General Purpose Timer Initialization
 *
 *************************************************************************/
static void GPTimer_Init(void)
{
  /*Configure Timer mode*/
  GPT1_CR =   (0<<GPT_CR_EN_SHIFT)       /*Timer still disalbed */\
            | (0<<GPT_CR_ENMOD_SHIFT)    /*GPT counter will retain its value when it is disabled*/\
            | (0<<GPT_CR_DBGEN_SHIFT)    /*GPT is disabled in debug mode*/\
            | (0<<GPT_CR_WAITEN_SHIFT)   /*GPT is disabled in wait mode*/\
            | (0<<GPT_CR_DOZEEN_SHIFT)   /*GPT is disabled in doze mode*/\
            | (0<<GPT_CR_STOPEN_SHIFT)   /*GPT is disabled in Stop mode*/\
            | GPT_CR_CLKSRC(5)           /*Clock Source select 24MHz*/\
            | (0<<GPT_CR_FRR_SHIFT)      /*Restart mode*/\
            | (1<<GPT_CR_EN_24M_SHIFT)   /*Enable 24MHz clock input from crystal.*/\
            | (0<<GPT_CR_SWR_SHIFT)      /*GPT is not in reset state*/\
            | GPT_CR_IM1(0)              /*capture disabled*/\
            | GPT_CR_IM2(0)              /*capture disabled*/\
            | GPT_CR_OM1(0)              /*Output disconnected. No response on pin.*/\
            | GPT_CR_OM2(0)              /*Output disconnected. No response on pin.*/\
            | GPT_CR_OM3(0)              /*Output disconnected. No response on pin.*/\
            | (0<<GPT_CR_FO1_SHIFT)      /*Output not forced*/\
            | (0<<GPT_CR_FO2_SHIFT)      /*Output not forced*/\
            | (0<<GPT_CR_FO3_SHIFT);     /*Output not forced*/

  /*Set Timer Prescalers*/
  GPT1_PR =   GPT_PR_PRESCALER(GPTIM_PRESCALE)  /*Set Timer prescaler*/\
            | GPT_PR_PRESCALER24M(0);           /*Do not divide cristal input*/

  /*Clear Timer Status*/
  GPT1_SR =   (1<<GPT_SR_OF1_SHIFT)     /*Clear Compare flag 1*/\
            | (1<<GPT_SR_OF2_SHIFT)     /*Clear Compare flag 2*/\
            | (1<<GPT_SR_OF3_SHIFT)     /*Clear Compare flag 3*/\
            | (1<<GPT_SR_IF1_SHIFT)     /*Clear Capture flag 1*/\
            | (1<<GPT_SR_IF2_SHIFT)     /*Clear Capture flag 1*/\
            | (1<<GPT_SR_ROV_SHIFT);    /*Clear Rollover Flag*/

  /*Enable interrupt*/
  GPT1_IR =   (1<<GPT_IR_OF1IE_SHIFT)   /*Enable Compare 1 Interrupt*/\
            | (0<<GPT_IR_OF2IE_SHIFT)   /*Disable Compare 2 Interrupt*/\
            | (0<<GPT_IR_OF3IE_SHIFT)   /*Disable Compare 3 Interrupt*/\
            | (0<<GPT_IR_IF1IE_SHIFT)   /*Disable Capture 1 Interrupt*/\
            | (0<<GPT_IR_IF2IE_SHIFT)   /*Disable Capture 1 Interrupt*/\
            | (0<<GPT_IR_ROVIE_SHIFT);  /*Disable Rollover Interrupt*/

 /*Set Reload period*/
  GPT1_OCR1 = GPT_OCR1_COMP(GPTIM_LOAD);   /*Set reload period*/
}
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
void GPTimer_Handler(void)
{
  /*Clear Timer Status*/
  GPT1_SR =   (1<<GPT_SR_OF1_SHIFT);     /*Clear Compare flag 1*/
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
  /*Configure UART1_TX and UART1_RX pins*/
  IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA =   IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA_MUX_MODE(0) \
                                        | (0<<IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA_SION_SHIFT);
  IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA_MUX_MODE(0)   \
                                        | (0<<IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA_SION_SHIFT);

  IOMUXC_UART1_RX_DATA_SELECT_INPUT = IOMUXC_UART1_RX_DATA_SELECT_INPUT_DAISY(3);

  /*Set UART Clocks*/
  CCM_CSCDR1 =   CCM_CSCDR1_UART_CLK_PODF(0)           /*UART clock podf divide by 1*/\
               | (1<<CCM_CSCDR1_UART_CLK_SEL_SHIFT)    /*Select Osc as clock source*/\
               | CCM_CSCDR1_USDHC1_PODF(1)             /*Divide by 2*/\
               | CCM_CSCDR1_USDHC2_PODF(1)             /*Divide by 2*/\
               | CCM_CSCDR1_BCH_PODF(1)                /*Divide by 2*/\
               | CCM_CSCDR1_GPMI_PODF(1);              /*Divide by 2*/

  /*Enable Uart Clock*/
  CCM_CCGR5 |= CCM_CCGR5_CG12(1);
  /*Uart1 Initialization*/
  /*Enable UART*/
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
               | UART_UCR1_ICD(0)                  /*Idle for more than 4 frames*/\
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
               | UART_UCR2_RTEC(0)            /*Trigger interrupt on a rising edge Ignored (RTSEN = 0)*/\
               | (0<<UART_UCR2_ESCEN_SHIFT)   /*Disable escape sequence detection*/\
               | (0<<UART_UCR2_CTS_SHIFT)     /*The CTS_B pin is high (inactive)*/\
               | (0<<UART_UCR2_CTSC_SHIFT)    /*The CTS_B pin is controlled by the CTS bit*/\
               | (1<<UART_UCR2_IRTS_SHIFT)    /*Ignore the RTS pin*/\
               | (0<<UART_UCR2_ESCI_SHIFT);   /*Disable the escape sequence interrupt*/

  /*RFDIV=1, DCE Mode*/
  UART1_UFCR =    UART_UFCR_RXTL(1)           /*RxFIFO has 1 character(default)*/\
                | (0<<UART_UFCR_DCEDTE_SHIFT) /*DCE mode selected*/\
                | UART_UFCR_RFDIV(5)          /*Divide input clock by 1*/\
                | UART_UFCR_TXTL(2);          /*TxFIFO has 2 or fewer characters(default)*/

  /*RS-485 mode disabled*/
  UART1_UMCR =    (0<<UART_UMCR_MDEN_SHIFT)   /*Normal RS-232*/\
                | (0<<UART_UMCR_SLAM_SHIFT)   /*Select Normal Address Detect mode*/\
                | (0<<UART_UMCR_TXB8_SHIFT)   /*0 will be transmitted as the RS485 9th data bit*/\
                | (0<<UART_UMCR_SADEN_SHIFT)  /*Disable RS-485 Slave Address Detected Interrupt*/\
                | UART_UMCR_SLADDR(0);        /*RS-485 Slave Address Character*/


  UART1_UCR2 &= ~(1<<UART_UCR2_SRST_SHIFT);       /*Software reset*/
  while(UART1_UTS & (1<<UART_UTS_SOFTRST_SHIFT));  /*Wait Software reset end*/
  /*Set Baud Rate 115200 bps*/
  UART1_UBIR = UART_UBIR_INC(0x03E7);             /* UBIR value 1000 */
  UART1_UBMR = UART_UBMR_MOD(0x32DC);             /* UBMR value 13021 */
}

static int MyLowLevelGetchar(void)
{
int ch;

  if(! (UART1_UTS & (1<<UART_UTS_RXEMPTY_SHIFT)))
  {
    ch = UART1_URXD;
    if(!(ch & (1<<UART_URXD_CHARRDY_SHIFT)) || (ch & (1<<UART_URXD_ERR_SHIFT)))
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
