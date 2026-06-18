/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : VirtualCOM.c
 *    Description : IAR-LPC4088-SK VirtualCOM Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board. It implements USB CDC (Communication
 * Device Class) device and install it like a Virtual COM port. The UART0 is used
 * for physical implementation of the RS232 port.
 * 
 * Jumpers:
 *  PWR_SEL    - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 * 
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <yfuns.h>
#include <stdint.h>

#include "LPC407x_8x.h"
#include "board.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"
#include "spifi_rom_api.h"

#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_buffer.h"

#include "uart.h"
#include "cd_class.h"

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/** public data **/
volatile uint32_t CriticalSecCntr;
SPIFIobj obj;
/** external data **/
#pragma section=".intvec"
/** public functions **/
/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: void
 * Return: 
 *
 * Description: Low level system init (clock, flash accelerator, 
 *              SDRAM and vector table address)
 *              
 *
 *************************************************************************/
int __low_level_init (void)
{
  /*Here the clock is already set by
    SystemInit function*/
  
  /*SDRAM init*/
  SDRAM_Init();

  /*SPIFI initialization*/
  LPC_SC->PCONP |= (1<<16);     /* enable SPIFI clock */
  LPC_SC->PCONP |= (1<<15);     /* enable IOCON clock */

  /*SPIFI Configure Pins*/
  SPIFI_IO0_IOCON = 0x35;
  SPIFI_IO1_IOCON = 0x35;
  SPIFI_IO2_IOCON = 0x35;
  SPIFI_IO3_IOCON = 0x35;
  SPIFI_CLK_IOCON = 0x35;
  SPIFI_CS_IOCON = 0x35;
	/* Initialize SPIFI driver */
	spifi_init(&obj, 7, S_RCVCLK | S_FULLCLK, SPIFIClock/1000000);
  
  return  1;
}

#define BMP_IMG_ADDR  0x28000000
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
pPic_t pPic; 
uint8_t Buffer[100];
uint8_t * pBuffer;
uint32_t Size,TranSize;

Boolean CdcConfigureStateHold;

#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
CDC_LineCoding_t CDC_LineCoding;
UartLineCoding_t UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

#if CDC_DEVICE_SUPPORT_LINE_STATE > 0

#if UART1_MODEM_STAT_ENA > 0
UartModemEvents_t     UartModemEvents;
UartModemLineState_t  UartModemLineState;
CDC_LineState_t CDC_LineState;
#endif // UART1_MODEM_STAT_ENA > 0

UartLineEvents_t      UartLineEvents;

SerialState_t   SerialState;
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0

  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Load Picture from bmp file to SDRAM*/
  pPic = GLCD_LoadPic((bmp_t *)BMP_IMG_ADDR);
  /*Init LCD and copy picture ot VRAM*/
  GLCD_Init (pPic, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /* Init UART 1 */
  UartInit(UART_0,4,NORM);

  /* Init USB */
  UsbCdcInit();

  __enable_interrupt();
  /* Soft connection enable */
  USB_ConnectRes(TRUE);

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x0000CCff);
  GLCD_SetWindow(84,124,396,148);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fCommunication device class");

  while(1)
  {
    if (IsUsbCdcConfigure())
    {
      // Data from USB
      Size = UsbCdcRead(Buffer,sizeof(Buffer)-1);
      if(Size)
      {
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("> %s\n",Buffer);
#endif // DATA_LOGGING
        TranSize = 0;
        pBuffer = Buffer;
        do
        {
          Size -= TranSize;
          pBuffer += TranSize;
          TranSize = UartWrite(UART_0,pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART1
      Size = UartRead(UART_0,Buffer,sizeof(Buffer)-1);
      if(Size)
      {
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("< %s\n",Buffer);
#endif  // DATA_LOGGING
        while(!UsbCdcWrite(Buffer,Size));
      }

      // Get line and modem events from UART
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
      // Get line events - BI, FE, PE, OE
      UartLineEvents = UartGetUartLineEvents(UART_0);
#if UART1_MODEM_STAT_ENA > 0
      // Get modem line events - RI, DCD and DSR
      UartModemEvents = Uart1GetUartModemEvents();
      if(UartLineEvents.Data || UartModemEvents.Data)
      {
        // Modem lines report - DCD, DSR and RI
        SerialState.bRxCarrier = UartModemEvents.bDCD;
        SerialState.bTxCarrier = UartModemEvents.bDSR;
        SerialState.bRingSignal = UartModemEvents.bDRI;
#else
      if(UartLineEvents.Data)
      {
        SerialState.Data = 0;
#endif // UART1_MODEM_STAT_ENA > 0
        // Line events report BI, PE, FE and OE
        SerialState.bBreak = UartLineEvents.bBI;
        SerialState.bFraming = UartLineEvents.bFE;
        SerialState.bOverRun = UartLineEvents.bOE;
        SerialState.bParity = UartLineEvents.bPE;
        // Send events
        UsbCdcReportSerialCommState(SerialState);
      }
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0
    }
    else
    {
      if(CdcConfigureStateHold == TRUE)
      {
        CdcConfigureStateHold = FALSE;
      }
    }

    // UART line coding - Baud rate, number of the stop bits,
    // number of bits of the data word and parity type
#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
    if(UsbCdcIsNewLineCodingSettings())
    {
      CDC_LineCoding = UsbCdcGetLineCodingSettings();
      // Update the baud rate
      UartLineCoding.dwDTERate = CDC_LineCoding.dwDTERate;
      // Update the stop bits number
      UartLineCoding.bStopBitsFormat = CDC_LineCoding.bCharFormat?UART_TWO_STOP_BIT:UART_ONE_STOP_BIT;
      // Update the parity type
      UartLineCoding.bParityType = (CDC_LineCoding.bParityType == 0)?UART_NO_PARITY:(UartParity_t)(CDC_LineCoding.bParityType-1);
      // Update the word width
      UartLineCoding.bDataBits = (UartWordWidth_t)(CDC_LineCoding.bDataBits - 5);
      // Set UART line coding
      UartSetLineCoding(UART_0,UartLineCoding);
    }
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

    // Get line and modem events from USB
#if UART1_MODEM_STAT_ENA > 0 && CDC_DEVICE_SUPPORT_LINE_STATE > 0
    // Modem lines - DTR, RTS
    if(UsbCdcIsNewLineStateSettings())
    {
      CDC_LineState = UsbCdcGetLineStateSettings();
      UartModemLineState.bDTR = CDC_LineState.DTR_State;
      UartModemLineState.bRTS = CDC_LineState.RTS_State;
      Uart1SetModemLineState(UartModemLineState);
    }
#endif // UART1_MODEM_STAT_ENA > 0 && CDC_DEVICE_SUPPORT_LINE_STATE > 0

#if CDC_DEVICE_SUPPORT_BREAK > 0
    // Break event
    UartSetUartLineState(UART_0,UsbCdcGetBreakState());
#endif // CDC_DEVICE_SUPPORT_BREAK > 0
  }
}
/** private functions **/
