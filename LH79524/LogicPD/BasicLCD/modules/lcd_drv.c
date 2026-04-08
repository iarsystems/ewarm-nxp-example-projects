/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lcd_drv.c
 *    Description : Driver for the LH7952x LCD controller
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
Int32U Temp = INTERRUPT;
  if(LcdCallBack != NULL)
  {
    // User callback function
    LcdCallBack((void *)Temp);
  }
  // Clear Interrupts flag
  INTCLR = Temp;
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
  LCDPRE_bit.LCDDIV  = 0; // HCLK/1
  AHBCLKCTRL_bit.LCD = 0; // Enables the LCD AHB clock
  PCLKCTRL1_bit.LCD  = 0; // Enables the LCD Data Clock
	// Disable LCD Controller
	CLCDC_CTRL_bit.LCDEN = 0;
	// Init LCD controller
	// Timing 0 register
	TIMING0_bit.HBP = LcdPram->h_BackPorch-1;				// Horizontal back porch in clocks
	TIMING0_bit.HFP = LcdPram->h_FrontPorch-1;			// Horizontal front porch in clocks
	TIMING0_bit.HSW = LcdPram->h_SyncPulseWidth-1;	// HSYNC pulse width in clocks
	// Pixels per line
	TIMING0_bit.PPL = (LcdPram->PixelsPerLine/16) - 1;
	
	// Timing 1 register
	TIMING1_bit.VBP = LcdPram->v_BackPorch-1;				// Vertical back porch in clocks
	TIMING1_bit.VFP = LcdPram->v_FrontPorch-1;			// Vertical front porch in clocks
	TIMING1_bit.VSW = LcdPram->v_SyncPulseWidth-1;	// VSYNC pulse width in clocks
	// Lines per panel
	TIMING1_bit.LPP = LcdPram->LinesPerPanel-1;
	
	// Timing 2 Clock and Polarity register
	TIMING2_bit.CPL = LcdPram->ClocksPerLine;
	// AC Bias Pin Frequency
	TIMING2_bit.ACB = LcdPram->AC_BiasFrequency-1;
	// Invert Output Enable
	TIMING2_bit.IOE = LcdPram->InvertOutputEnable;
	// Invert Panel Clock
	TIMING2_bit.IPC = LcdPram->InvertPanelClock;
	// Invert Horizontal Synchronization
	TIMING2_bit.IHS = LcdPram->InvertHsync;
	// Invert Vertical Synchronization
	TIMING2_bit.IVS = LcdPram->InvertHsync;
	// Bypass Pixel Clock Divider
	if(LcdPram->PixelClkDivider == 0)
	{
		TIMING2_bit.BCD = 1;
	}
	else
	{
		TIMING2_bit.BCD    = 0;
		TIMING2_bit.PCD_LO = (LcdPram->PixelClkDivider-2) & 0x1F;
		TIMING2_bit.PCD_HI = (LcdPram->PixelClkDivider-2) >> 5;
	}
	
	// Disable all interrupts
	INTREN = 0;
	
	// Set LCD Control register	
	CLCDC_CTRL_bit.BPP  = LcdPram->BitsPerPixel;			// Set BPP
	CLCDC_CTRL_bit.DUAL = LcdPram->DualPanel;				// Set Dual/Single panel
	CLCDC_CTRL_bit.BGR 	= LcdPram->RgbNormalMode;		// Set swap red and blue	
  CLCDC_CTRL_bit.BEPO = 0;  // Little-endian ordering within a byte
  CLCDC_CTRL_bit.BEBO = 0;  // Little-endian byte order
	// LCD DMA FIFO Watermark Level
	CLCDC_CTRL_bit.WATERMARK = LcdPram->DmaOn4Empty;
	switch(LcdPram->LCD_PanelType)
	{
	case TFT: 				// Panel type is standard TFT
	case ADTFT:  			// Panel type is advanced TFT
	case HRTFT: 			// Panel type is highly reflective TFT
    // Assign GPIO to LCD
    MUXCTL19_bit.PE6   = 2;   // LCDMOD
    MUXCTL19_bit.PE5   = 1;   // LCDVDDEN
    MUXCTL19_bit.PE4   = 2;   // LCDREV
    MUXCTL19_bit.PE3   = 1;   // LCDCLS
    MUXCTL20_bit.PF6   = 2;   // LCDSPL
    MUXCTL20_bit.PF7   = 2;   // LCDSPS
    MUXCTL20_bit.PE0   = 2;   // LCDHRLP
    MUXCTL20_bit.PE1   = 1;   // LCDDCLK
    MUXCTL20_bit.PE2   = 1;   // LCDPS

    MUXCTL22_bit.PG2   = 1;   // LCDVD0
    MUXCTL22_bit.PG3   = 1;   // LCDVD1
    MUXCTL22_bit.PG4   = 1;   // LCDVD2
    MUXCTL22_bit.PG5   = 1;   // LCDVD3
    MUXCTL22_bit.PG6   = 1;   // LCDVD4
    MUXCTL22_bit.PG7   = 1;   // LCDVD5
    MUXCTL22_bit.PF0   = 1;   // LCDVD6
    MUXCTL22_bit.PF1   = 1;   // LCDVD7
    MUXCTL21_bit.PF2   = 1;   // LCDVD8
    MUXCTL21_bit.PF3   = 1;   // LCDVD9
    MUXCTL21_bit.PF4   = 1;   // LCDVD10
    MUXCTL21_bit.PF5   = 1;   // LCDVD11
    MUXCTL21_bit.PL2   = 1;   // LCDVD12
    MUXCTL21_bit.PL3   = 1;   // LCDVD13
    MUXCTL1_bit.PL0    = 1;   // LCDVD14
    MUXCTL1_bit.PL1    = 1;   // LCDVD15

    CLCDC_CTRL_bit.TFT = 1;
		break;
	default:
		return(FALSE);
	};

	// Set ALI
	if((LcdPram->LCD_PanelType == HRTFT) ||
	 	 (LcdPram->LCD_PanelType == ADTFT))
	{
		// Pixels Per Line
		ALISETUP_bit.PPL = LcdPram->PixelsPerLine - 1;
		// HRTFTC Control Register
		ALICTRL_bit.CLSEN = LcdPram->HrtftClsEnable;
		ALICTRL_bit.SPSEN = LcdPram->HrtftSpsEnable;
		// HRTFTC Timing 1 Register
		ALITIMING1_bit.LPDEL = LcdPram->HrtftLpDelay - 1;
		ALITIMING1_bit.REVDEL = LcdPram->HrtftPolarityDelay - 1;
		ALITIMING1_bit.PSCLS = LcdPram->HrtftLpToPsDelay - 1;
		// HRTFTC Timing 2 Register
		ALITIMING2_bit.PS2CLS2 = LcdPram->HrtftSplToClsDelay - 1;
		ALITIMING2_bit.SPLDEL = LcdPram->HrtftSplDelay - 1;
		ALISETUP_bit.CR = 1;	// Active mode
	}
	else
	{
		ALISETUP_bit.CR = 0;	// Bypass mode
	}
  // Registered interrupt handler
  VIC_SetVectoredIRQ(Lcd_Handler,Slot,CLCD_IRQ);
  VIC_EnableInt(1UL << CLCD_IRQ);
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
    CLCDC_CTRL_bit.LCDEN = 1;
    break;
  case LCD_DISABLE:     // Disable
    CLCDC_CTRL_bit.LCDEN = 0;
    break;
  case LCD_PW_ENABLE:   // Enable LCD power
    CLCDC_CTRL_bit.PWR = 1;
    CLCDC_CTRL_bit.LCDEN = 1;
    break;
  case LCD_PW_DISABLE:  // Enable LCD power
    CLCDC_CTRL_bit.PWR = 0;
    CLCDC_CTRL_bit.LCDEN = 0;
    break;
  case LCD_SET_UP_FB:   // Update the upper frame buffer pointer
    // Sanity check
    if (Arg == 0)
    {
      return(FALSE);
    }
    // Set frame buffer address
    UPBASE = Arg;
    break;
  case LCD_SET_LOW_FB:   // Update the lower frame buffer pointer
    // Sanity check
    if (Arg == 0)
    {
      return(FALSE);
    }
    // Set frame buffer address
    LPBASE = Arg;
    break;
  case LCD_GET_STATUS:
    // Sanity check
    pArg = (Int32U *)Arg;
    if (pArg == NULL)
    {
      /* Null pointer */
      return(FALSE);
    }
    *pArg = CLCDC_CTRL_bit.LCDEN;
    break;
  case LCD_DISABLE_INT:   // Mask interrupts at the source
    // Update the interrupt control register
    INTREN = INTREN & ~Arg;
    break;
  case LCD_ENABLE_INT:    // Unmask interrupts at the source
    // Update the interrupt control register
    INTREN = INTREN | Arg;
    break;
  case LCD_ENABLE_VCOMP_INT:
    // Write the new value to the control register
    CLCDC_CTRL_bit.VCI = Arg;
    break;
  case LCD_DISABLE_VCOMP_INT:
    // Write the new value to the control register
    CLCDC_CTRL_bit.VCI = 0;
    break;
  case LCD_SET_BGR:
    CLCDC_CTRL_bit.BGR = 1;
    break;
  case LCD_SET_RGB:
    CLCDC_CTRL_bit.BGR = 0;
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
    for(Count = 0, pDst = (pInt32U)&PALETTE_1_0;
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

