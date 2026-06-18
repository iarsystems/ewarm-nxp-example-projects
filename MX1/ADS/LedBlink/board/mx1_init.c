/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : mx1_init.c
 *    Description : MX1 init module
 *
 *    History :
 *    1. Date        : 7 Feb. 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "mx1_init.h"

VoidFpnt_t pIRQ_INTR_HANDLES[64];

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ Handler
 *
 *************************************************************************/
__arm __irq void irq_handler (void)
{
volatile Int32U VectorInd = NIVECSR >> 16;
  if (VectorInd < 64)
  {
    pIRQ_INTR_HANDLES[VectorInd]();
  }
}

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: FIQ Handler
 *
 *************************************************************************/
__arm __fiq void fiq_handler (void)
{
}

/*************************************************************************
 * Function Name: AitcEnableInt
 * Parameters: VoidFpnt_t pIntrHandler,Int32U IntrInd,
 *						 Int32U IntrPtio, Boolean IntrType
 *
 * Return: none
 *
 * Description: Interrupt Handler registering
 *
 *************************************************************************/
void AitcEnableInt (VoidFpnt_t pIntrHandler,Int32U IntrInd, Int32U IntrPtio, Boolean IntrType)
{
Int32U Indx   =            (IntrInd > 31)?(IntrInd-32) : IntrInd;
Int32U *pData = (Int32U *)((IntrInd > 31)?&INTTYPEH    :&INTTYPEL);

  pIRQ_INTR_HANDLES[IntrInd] = pIntrHandler;
  if(IntrType)
  {
    // FIQ
    *pData |= 1 << Indx;
  }
  else
  {
    // IRQ
    *pData &= ~(1 << Indx);
  }
  // Set Interrupt priority
  pData   = (Int32U *)&NIPRIORITY7 + (63 - IntrInd)/8;
  Indx    = (IntrInd%8) * 4;
  *pData &= ~((IntrPtio & 0xF) << Indx);
  *pData |=   (IntrPtio & 0xF) << Indx;
  // Enable Interrupt
  INTENNUM_bit.ENNUM = IntrInd;
}

/*************************************************************************
 * Function Name: AitcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Interrupt controller init
 *
 *************************************************************************/
void AitcInit (void)
{
  //
  INTCNTL = 0;
  // Disable All interrupt source
  INTENABLEH = INTENABLEL = 0;
  // All interrupts is normal priority
  INTTYPEH = INTTYPEL = 0;
  // Reset all interrupt priority to lowest priority
  Int32U *pData = (Int32U *)&NIPRIORITY7;
  for(Int32U i = 0; i < 8; ++i)
  {
    *pData++ = 0;
  }
  // Do not disable any normal interrupts
  NIMASK_bit.NIMASK = 0x10;
}

/*************************************************************************
 * Function Name: DbgUartGetChar
 * Parameters: none
 *
 * Return: Int8U
 *
 * Description: Receive char by UART 1
 *
 *************************************************************************/
Int8U DbgUartGetChar (void)
{
  while(!USR1_1_bit.RRDY);
  return(URX0D_1_bit.RX_DATA);
}

/*************************************************************************
 * Function Name: DbgUartPutChar
 * Parameters: Int8U Data
 *
 * Return: none
 *
 * Description: Send char by UART 1
 *
 *************************************************************************/
void DbgUartPutChar (Int8U Data)
{
  while(!USR1_1_bit.TRDY);
  UTX0D_1_bit.TX_DATA = Data;
}

/*************************************************************************
 * Function Name: DbgUartPrint
 * Parameters: Int8U const *pData
 *
 * Return: none
 *
 * Description: Send zero terminated massive
 *
 *************************************************************************/
void DbgUartPrint (Int8U const *pData)
{
  while(*pData)
  {
    DbgUartPutChar(*pData++);
  }
}

/*************************************************************************
 * Function Name: DbgUartInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init UART1
 *
 *************************************************************************/
void DbgUartInit (void)
{
Int32U Num   = UART1_BAUD_RATE * 16;
Int32U Denum = (Int32U) PERCLK1;
  // Uart disable
  UCR1_1 = 0x4;
  // Assign pins RxD, TxD, RST and CTS
  GIUS_C_bit.PIN12 = 0;GPR_C_bit.PIN12 = 0;
  GIUS_C_bit.PIN11 = 0;GPR_C_bit.PIN11 = 0;
  GIUS_C_bit.PIN10 = 0;GPR_C_bit.PIN10 = 0;
  GIUS_C_bit.PIN9  = 0;GPR_C_bit.PIN9  = 0;
  // Div 1
  UFCR_1_bit.RFDIV = 5;
  UFCR_1_bit.TXTL  = 2;
  // 8 bit, No Parity, 1 stop, TX Enable, RX Enable
  UCR3_1 = UCR4_1 = 0;
  UCR2_1 = 0x4066;
  // Init Serial DBG 115200 PERCLK1 - MPLL_CLK / 16 - 16.625MHz
  while ((Num > USHRT_MAX) || (Denum > USHRT_MAX))
  {
    Num >>=1; Denum>>=1;
  }
  // update of baud only when write in this order
  UBIR_1 = Num   - 1;
  UBMR_1 = Denum - 1;
  // UART Enable
  UCR1_1 |= 0x1;
}

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: int
 *
 * Description: Low level init subroutine
 *
 *************************************************************************/
__root __arm  int __low_level_init(void)
{
	__disable_interrupt();	
  // Init PLL and Clock
  // MCU PLL Control Register 0 FOSC - 16 MHz Fout 192MHz
  MPCTL0_bit.PD  = _M_PD-1;
  MPCTL0_bit.MFD = _M_MFD-1;
  MPCTL0_bit.MFI = _M_MFI;
  MPCTL0_bit.MFN = _M_MFN;
  // System PLL Control Register 0 FOSC - 16 MHz Fout 96MHz
  SPCTL0_bit.PD  = _S_PD-1;
  SPCTL0_bit.MFD = _S_MFD-1;
  SPCTL0_bit.MFI = _S_MFI;
  SPCTL0_bit.MFN = _S_MFN;
  // Peripheral Clock Divider Register /13
  PCDR_bit.PCLK_DIV1 = PER1_DIV-1;
  PCDR_bit.PCLK_DIV2 = PER2_DIV-1;
  PCDR_bit.PCLK_DIV3 = PER3_DIV-1;
  CSCR   = 0x0F630003;    // Clock Source Control Register
  // Wait until PLL lock frequency
  while(!SPCTL1_bit.LF);

  AitcInit();

  DbgUartInit();

  return 1;
}
