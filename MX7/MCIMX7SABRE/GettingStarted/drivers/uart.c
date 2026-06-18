/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : uart.c
 *    Description : MCIMX7D/MCIMX7S UART driver
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 7084 $
 **************************************************************************/
#include <stdint.h>
#include "device_registers.h"
#include "uart.h"

/*************************************************************************
 * Function Name: UART_Init
 * Parameters: UART_MemMapPtr UART_BASE
 *
 * Return: none
 *
 * Description: UART Initialization at 115200 baud.
 *
 *************************************************************************/
void UART_Init(UART_MemMapPtr UART_BASE)
{
  if (UART1_BASE_PTR == UART_BASE)
  {
    /*Configure UART1_TX and UART1_RX pins*/
    IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(0) \
                                          | (0<<IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT);
    IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(0)   \
                                          | (0<<IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT);

    IOMUXC_UART1_RX_DATA_SELECT_INPUT = IOMUXC_SELECT_INPUT_DAISY(0);
    /*Enable Uart1 Clock*/
    CCM_CCGR148_SET = 0x2;
  } else 
  if (UART2_BASE_PTR == UART_BASE)
  {
    /*Configure UART2_TX and UART2_RX pins*/
    IOMUXC_SW_MUX_CTL_PAD_UART2_RX_DATA = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(0) \
                                          | (0<<IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT);
    IOMUXC_SW_MUX_CTL_PAD_UART2_TX_DATA = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(0)   \
                                          | (0<<IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT);

    IOMUXC_UART2_RX_DATA_SELECT_INPUT = IOMUXC_SELECT_INPUT_DAISY(2);
    /*Enable Uart2 Clock*/
    CCM_CCGR149_SET = 0x2;
  }

  /*Uart Initialization*/
  /*Enable UART*/
  UART_BASE->UCR1 = (1<<UART_UCR1_UARTEN_SHIFT)       /*Enable the UART*/\
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
  UART_BASE->UCR2 = (1<<UART_UCR2_SRST_SHIFT)         /*1 No reset*/\
                  | (0<<UART_UCR2_RXEN_SHIFT)         /*Disable the receiver*/\
                  | (1<<UART_UCR2_TXEN_SHIFT)         /*Enable the transmitter*/\
                  | (0<<UART_UCR2_ATEN_SHIFT)         /*AGTIM interrupt disabled*/\
                  | (0<<UART_UCR2_RTSEN_SHIFT)        /*Disable request to send interrupt*/\
                  | (1<<UART_UCR2_WS_SHIFT)           /*8-bit transmit and receive character length*/\
                  | (0<<UART_UCR2_STPB_SHIFT)         /*The transmitter sends 1 stop bit. The receiver expects 1 or more stop bits*/\
                  | (0<<UART_UCR2_PROE_SHIFT)         /*Even parity (Ignored since PREN=0)*/\
                  | (0<<UART_UCR2_PREN_SHIFT)         /*Disable parity generator and checker*/\
                  | UART_UCR2_RTEC(0)                 /*Trigger interrupt on a rising edge Ignored (RTSEN = 0)*/\
                  | (0<<UART_UCR2_ESCEN_SHIFT)        /*Disable escape sequence detection*/\
                  | (0<<UART_UCR2_CTS_SHIFT)          /*The CTS_B pin is high (inactive)*/\
                  | (0<<UART_UCR2_CTSC_SHIFT)         /*The CTS_B pin is controlled by the CTS bit*/\
                  | (1<<UART_UCR2_IRTS_SHIFT)         /*Ignore the RTS pin*/\
                  | (0<<UART_UCR2_ESCI_SHIFT);        /*Disable the escape sequence interrupt*/

  /*RFDIV=1, DCE Mode*/
  UART_BASE->UFCR = UART_UFCR_RXTL(1)                 /*RxFIFO has 1 character(default)*/\
                  | (0<<UART_UFCR_DCEDTE_SHIFT)       /*DCE mode selected*/\
                  | UART_UFCR_RFDIV(5)                /*Divide input clock by 1*/\
                  | UART_UFCR_TXTL(2);                /*TxFIFO has 2 or fewer characters(default)*/

  /*RS-485 mode disabled*/
  UART_BASE->UMCR = (0<<UART_UMCR_MDEN_SHIFT)         /*Normal RS-232*/\
                  | (0<<UART_UMCR_SLAM_SHIFT)         /*Select Normal Address Detect mode*/\
                  | (0<<UART_UMCR_TXB8_SHIFT)         /*0 will be transmitted as the RS485 9th data bit*/\
                  | (0<<UART_UMCR_SADEN_SHIFT)        /*Disable RS-485 Slave Address Detected Interrupt*/\
                  | UART_UMCR_SLADDR(0);              /*RS-485 Slave Address Character*/


  UART_BASE->UCR2 &= ~(1<<UART_UCR2_SRST_SHIFT);       /*Software reset*/
  while(UART_BASE->UTS & (1<<UART_UTS_SOFTRST_SHIFT));  /*Wait Software reset end*/
  /*Set Baud Rate 115200 bps*/
  UART_BASE->UBIR = UART_UBIR_INC(0x03E7);             /* UBIR value 1000 */
  UART_BASE->UBMR = UART_UBMR_MOD(0x32DC);             /* UBMR value 13021 */
}
