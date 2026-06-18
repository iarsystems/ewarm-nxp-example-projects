/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lcd_drv.c
 *    Description : Driver for the LH754xx LCD controller
 *
 *    History :
 *    1. Date        : 7 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "lcd_drv.h"

CommUserFpnt_t LcdCallBack;

/*************************************************************************
 * Function Name: Lcd_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: The LCD Controller Interrupt handler
 *
 *************************************************************************/
void Lcd_Handler(void)
{
Int32U Temp = CLCDC_Status;
  if(LcdCallBack != NULL)
  {
    // User callback function
    LcdCallBack((void *)Temp);
  }
  // Clear Interrupts flag
  CLCDC_Status = Temp;
}

/*************************************************************************
 * Function Name: LcdInit
 * Parameters: const LcdParam_t * LcdPram, VicIrqSlots_t Slot
 *
 * Return: Boolean
 *
 * Description: Init LCD Controller
 *
 *************************************************************************/
Boolean LcdInit(const LcdParam_t * LcdPram, VicIrqSlots_t Slot)
{
  // Set LCD Controller clock
  RCPC_LCDPrescaler_bit.LCDPRESCALER = 0;
  RCPC_APBPeriphClkCtrl1_bit.LCD = 0;
	// Disable LCD Controller
	CLCDC_Ctrl_bit.LcdEn = 0;
	// Init LCD controller
	// Timing 0 register
	CLCDC_Timing0_bit.HBP = LcdPram->h_BackPorch-1;				// Horizontal back porch in clocks
	CLCDC_Timing0_bit.HFP = LcdPram->h_FrontPorch-1;			// Horizontal front porch in clocks
	CLCDC_Timing0_bit.HSW = LcdPram->h_SyncPulseWidth-1;	// HSYNC pulse width in clocks
	// Pixels per line
	CLCDC_Timing0_bit.PPL = (LcdPram->PixelsPerLine/16) - 1;
	
	// Timing 1 register
	CLCDC_Timing1_bit.VBP = LcdPram->v_BackPorch-1;				// Vertical back porch in clocks
	CLCDC_Timing1_bit.VFP = LcdPram->v_FrontPorch-1;			// Vertical front porch in clocks
	CLCDC_Timing1_bit.VSW = LcdPram->v_SyncPulseWidth-1;	// VSYNC pulse width in clocks
	// Lines per panel
	CLCDC_Timing1_bit.LPP = LcdPram->LinesPerPanel-1;
	
	// Timing 2 Clock and Polarity register
	CLCDC_Timing2_bit.CPL = LcdPram->ClocksPerLine;
	// AC Bias Pin Frequency
	CLCDC_Timing2_bit.ACB = LcdPram->AC_BiasFrequency-1;
	// Invert Output Enable
	CLCDC_Timing2_bit.IOE = LcdPram->InvertOutputEnable;
	// Invert Panel Clock
	CLCDC_Timing2_bit.IPC = LcdPram->InvertPanelClock;
	// Invert Horizontal Synchronization
	CLCDC_Timing2_bit.HIS = LcdPram->InvertHsync;
	// Invert Vertical Synchronization
	CLCDC_Timing2_bit.IVS = LcdPram->InvertHsync;
	// Use bus clock
	CLCDC_Timing2				 |= LcdPram->Clock14mhzSelect << 5;
	// Bypass Pixel Clock Divider
	if(LcdPram->PixelClkDivider == 0)
	{
		CLCDC_Timing2_bit.BCD = 1;
	}
	else
	{
		CLCDC_Timing2_bit.BCD = 0;
		CLCDC_Timing2_bit.PCD = LcdPram->PixelClkDivider-2;
	}
	
	// Disable all interrupts
	CLCDC_INTRENABLE = 0;
	
	// Set LCD Control register	
	CLCDC_Ctrl_bit.LcdBpp = LcdPram->BitsPerPixel;			// Set BPP
	CLCDC_Ctrl_bit.LcdDual = LcdPram->DualPanel;				// Set Dual/Single panel
	CLCDC_Ctrl_bit.BGR 		 = LcdPram->RgbNormalMode;		// Set swap red and blue	
	// LCD DMA FIFO Watermark Level
	CLCDC_Ctrl_bit.WATERMARK = LcdPram->DmaOn4Empty;
	// LCD DMA FIFO Test Mode Disable
	CLCDC_Ctrl_bit.LDmaFIFOTME = 0;
	switch(LcdPram->LCD_PanelType)
	{
	case TFT: 				// Panel type is standard TFT
	case ADTFT:  			// Panel type is advanced TFT
	case HRTFT: 			// Panel type is highly reflective TFT
	  IOCON_LCD_MUX_bit.MODE = 4;  // Assign GPIO to LCD
		CLCDC_Ctrl_bit.LcdTFT = 1;
		break;
	case MONO_4BIT:		// Panel type is 4-bit mono		
    if(LcdPram->DualPanel)
    {
      IOCON_LCD_MUX_bit.MODE = 2;  // Dual Mode Assign GPIO to LCD
    }
    else
    {
	    IOCON_LCD_MUX_bit.MODE = 1;  // Assign GPIO to LCD
    }
		CLCDC_Ctrl_bit.LcdMono8 = 0;
		CLCDC_Ctrl_bit.LcdBW 	= 1;
		CLCDC_Ctrl_bit.LcdTFT = 0;
		break;
	case MONO_8BIT:		// Panel type is 8-bit mono
	  IOCON_LCD_MUX_bit.MODE = 3;  // Assign GPIO to LCD
		CLCDC_Ctrl_bit.LcdMono8 = 1;
		CLCDC_Ctrl_bit.LcdBW 	= 1;
		CLCDC_Ctrl_bit.LcdTFT = 0;
		break;
	case CSTN:				// Panel type is color STN
	  IOCON_LCD_MUX_bit.MODE = 3;  // Assign GPIO to LCD
		CLCDC_Ctrl_bit.LcdBW 	= 0;
		CLCDC_Ctrl_bit.LcdTFT = 0;
  	break;
	default:
	  IOCON_LCD_MUX_bit.MODE = 0; // No LCD
		return(FALSE);
	};

	// Set ALI
	if((LcdPram->LCD_PanelType == HRTFT) ||
	 	 (LcdPram->LCD_PanelType == ADTFT))
	{
		// Pixels Per Line
		HRTFTC_Setup_bit.PPL = LcdPram->PixelsPerLine - 1;
		// HRTFTC Control Register
		HRTFTC_CTRL_bit.CLSEN = LcdPram->HrtftClsEnable;
		HRTFTC_CTRL_bit.SPSEN = LcdPram->HrtftSpsEnable;
		// HRTFTC Timing 1 Register
		HRTFTC_Timing1_bit.LPDEL = LcdPram->HrtftLpDelay - 1;
		HRTFTC_Timing1_bit.REVDEL = LcdPram->HrtftPolarityDelay - 1;
		HRTFTC_Timing1_bit.PSDEL = LcdPram->HrtftLpToPsDelay - 1;
		// HRTFTC Timing 2 Register
		HRTFTC_Timing2_bit.PSDEL2 = LcdPram->HrtftSplToClsDelay - 1;
		HRTFTC_Timing2_bit.SPLVALUE = LcdPram->HrtftSplDelay - 1;
		HRTFTC_Setup_bit.CR = 1;	// Active mode
	}
	else
	{
		HRTFTC_Setup_bit.CR = 0;	// Bypass mode
	}
  // Registered interrupt handler
  VIC_SetVectoredIRQ(Lcd_Handler,Slot,VIC_LCD);
  VIC_EnableInt(1UL << VIC_LCD);
  // Init variables
  LcdCallBack = NULL;
  return(TRUE);
}

/*************************************************************************
 * Function Name: LcdIoCtrl
 * Parameters: LcdCommand_t Command, Int32U Arg
 *
 * Return: Boolean
 *
 * Description: LCD configuration block  This function is a large case block.
 *              Based on the passed function and option values, set or get
 *              the appropriate real time clock parameter.
 *
 *************************************************************************/
Boolean LcdIoCtrl (LcdCommand_t Command, Int32U Arg)
{
Int32U Count, Temp, *pArg, *pDst;

  /* Validate the request */
  switch (Command)
  {
  case LCD_ENABLE:      // Enable
    CLCDC_Ctrl_bit.LcdEn = 1;
    break;
  case LCD_DISABLE:     // Disable
    CLCDC_Ctrl_bit.LcdEn = 0;
    break;
  case LCD_PW_ENABLE:   // Enable LCD power
    CLCDC_Ctrl_bit.LcdPwr = 1;
    CLCDC_Ctrl_bit.LcdEn = 1;
    break;
  case LCD_PW_DISABLE:  // Enable LCD power
    CLCDC_Ctrl_bit.LcdPwr = 0;
    CLCDC_Ctrl_bit.LcdEn = 0;
    break;
  case LCD_SET_UP_FB:   // Update the upper frame buffer pointer
    // Sanity check
    if (Arg == 0)
    {
      return(FALSE);
    }
    // Set frame buffer address
    CLCDC_UPBASE = Arg;
    break;
  case LCD_SET_LOW_FB:   // Update the lower frame buffer pointer
    // Sanity check
    if (Arg == 0)
    {
      return(FALSE);
    }
    // Set frame buffer address
    CLCDC_LPCURR = Arg;
    break;
  case LCD_GET_STATUS:
    // Sanity check
    pArg = (Int32U *)Arg;
    if (pArg == NULL)
    {
      /* Null pointer */
      return(FALSE);
    }
    *pArg = CLCDC_Ctrl_bit.LcdEn;
    break;
  case LCD_DISABLE_INT:   // Mask interrupts at the source
    // Update the interrupt control register
    CLCDC_INTRENABLE = CLCDC_INTRENABLE & ~Arg;
    break;
  case LCD_ENABLE_INT:    // Unmask interrupts at the source
    // Update the interrupt control register
    CLCDC_INTRENABLE = CLCDC_INTRENABLE | Arg;
    break;
  case LCD_ENABLE_VCOMP_INT:
    // Write the new value to the control register
    CLCDC_Ctrl_bit.LcdVComp = Arg;
    break;
  case LCD_DISABLE_VCOMP_INT:
    // Write the new value to the control register
    CLCDC_Ctrl_bit.LcdVComp = 0;
    break;
  case LCD_SET_BGR:
    CLCDC_Ctrl_bit.BGR = 1;
    break;
  case LCD_SET_RGB:
    CLCDC_Ctrl_bit.BGR = 0;
    break;
  case LCD_SET_CALLBACK:
    // Save the interrupt callback method
    Temp = (Int32U)LcdCallBack;
    LcdCallBack = (CommUserFpnt_t) *(pInt32U)Arg;
    *(pInt32U)Arg = Temp;
    break;
  case LCD_SET_PALETTE: // Update LCD Palette RAM
    // Sanity check
    pArg = (Int32U *)Arg;
    if (pArg == NULL)
    {
      /* Null pointer */
      return(FALSE);
    }
    for(Count = 0, pDst = (pInt32U)&CLCDC_Palette;
        Count < 256/2;
        ++Count,++pDst,++pArg)
    {
      Temp  = (*pArg >>  5) & 0x0000001F;
      Temp |= (*pArg >>  6) & 0x000003E0;
      Temp |= (*pArg >>  9) & 0x00007C00;
      pArg++;
      Temp |= (*pArg << 13) & 0x001F0000;
      Temp |= (*pArg << 10) & 0x03E00000;
      Temp |= (*pArg <<  7) & 0x7C000000;
      *pDst = Temp;
    }
    break;
  default:
    /* Unsupported parameter */
    return(FALSE);
  }

  return(TRUE);
}

