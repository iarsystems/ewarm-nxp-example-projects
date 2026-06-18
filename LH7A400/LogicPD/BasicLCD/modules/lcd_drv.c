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
Int32U Temp = LCD_INTERRUPT;
  if(LcdCallBack != NULL)
  {
    // User callback function
    LcdCallBack((void *)Temp);
  }
  // Clear Interrupts flag
  LCD_STATUS = Temp & 0x1E;
}

/*************************************************************************
 * Function Name: LcdInit
 * Parameters: const LcdParam_t * LcdPram
 *
 * Return: Boolean
 *
 * Description: Init LCD Controller
 *
 *************************************************************************/
Boolean LcdInit(const LcdParam_t * LcdPram)
{
	// Disable LCD Controller
	LCD_CONTROL_bit.LCDEN = 0;
	// Init LCD controller
	// Timing 0 register
	LCD_TIMING0_bit.HBP = LcdPram->h_BackPorch-1;				// Horizontal back porch in clocks
	LCD_TIMING0_bit.HFP = LcdPram->h_FrontPorch-1;			// Horizontal front porch in clocks
	LCD_TIMING0_bit.HSW = LcdPram->h_SyncPulseWidth-1;	// HSYNC pulse width in clocks
	// Pixels per line
	LCD_TIMING0_bit.PPL = (LcdPram->PixelsPerLine/16) - 1;
	
	// Timing 1 register
	LCD_TIMING1_bit.VBP = LcdPram->v_BackPorch-1;				// Vertical back porch in clocks
	LCD_TIMING1_bit.VFP = LcdPram->v_FrontPorch-1;			// Vertical front porch in clocks
	LCD_TIMING1_bit.VSW = LcdPram->v_SyncPulseWidth-1;	// VSYNC pulse width in clocks
	// Lines per panel
	LCD_TIMING1_bit.LPP = LcdPram->LinesPerPanel-1;
	
	// Timing 2 Clock and Polarity register
	LCD_TIMING2_bit.CPL = LcdPram->ClocksPerLine;
	// AC Bias Pin Frequency
	LCD_TIMING2_bit.ACB = LcdPram->AC_BiasFrequency-1;
	// Invert Output Enable
	LCD_TIMING2_bit.IOE = LcdPram->InvertOutputEnable;
	// Invert Panel Clock
	LCD_TIMING2_bit.IPC = LcdPram->InvertPanelClock;
	// Invert Horizontal Synchronization
	LCD_TIMING2_bit.IHS = LcdPram->InvertHsync;
	// Invert Vertical Synchronization
	LCD_TIMING2_bit.IVS = LcdPram->InvertHsync;
  // Use HCLK as the LCD Clock
  LCD_TIMING2_bit.CSEL = 0;
	// Bypass Pixel Clock Divider
	if(LcdPram->PixelClkDivider == 0)
	{
		LCD_TIMING2_bit.BCD = 1;
	}
	else
	{
		LCD_TIMING2_bit.BCD = 0;
		LCD_TIMING2_bit.PCD = LcdPram->PixelClkDivider-2;
	}
	
	// Disable all interrupts
	LCD_INTREN = 0;
	
	// Set LCD Control register	
	LCD_CONTROL_bit.BPP  = LcdPram->BitsPerPixel;			// Set BPP
	LCD_CONTROL_bit.DUAL = LcdPram->DualPanel;				// Set Dual/Single panel
	LCD_CONTROL_bit.BGR 	= LcdPram->RgbNormalMode;		// Set swap red and blue	
  LCD_CONTROL_bit.BEPO = 0;  // Little-endian ordering within a byte
  LCD_CONTROL_bit.BEBO = 0;  // Little-endian byte order
	// LCD DMA FIFO Watermark Level
	LCD_CONTROL_bit.WATERMARK = LcdPram->DmaOn4Empty;
	switch(LcdPram->LCD_PanelType)
	{
	case TFT: 				// Panel type is standard TFT
	case ADTFT:  			// Panel type is advanced TFT
	case HRTFT: 			// Panel type is highly reflective TFT
    LCD_CONTROL_bit.TFT = 1;
    GPIO_PINMUX_bit.PEOCON = 1;
    GPIO_PINMUX_bit.PDOCON = 1;
		break;
	default:
    GPIO_PINMUX_bit.PEOCON = 0;
    GPIO_PINMUX_bit.PDOCON = 0;
		return(FALSE);
	};

	// Set ALI
	if((LcdPram->LCD_PanelType == HRTFT) ||
	 	 (LcdPram->LCD_PanelType == ADTFT))
	{
		// Pixels Per Line
		ALISETUP_bit.PPL = LcdPram->PixelsPerLine - 1;
		// HRTFTC Control Register
		ALICONTROL_bit.LCDCLSEN = LcdPram->HrtftClsEnable;
		ALICONTROL_bit.LCDSPSEN = LcdPram->HrtftSpsEnable;
		// HRTFTC Timing 1 Register
		ALITIMING1_bit.LPDEL = LcdPram->HrtftLpDelay - 1;
		ALITIMING1_bit.REVDEL = LcdPram->HrtftPolarityDelay - 1;
		ALITIMING1_bit.PSCLS = LcdPram->HrtftLpToPsDelay - 1;
		// HRTFTC Timing 2 Register
		ALITIMING2_bit.PS2CLS2 = LcdPram->HrtftSplToClsDelay - 1;
		ALITIMING2_bit.SPLDEL = LcdPram->HrtftSplDelay - 1;
		ALISETUP_bit.CR = 1;	  // Active mode
    ALISETUP_bit.ALIEN = 1; // Enable the ALI interface
	}
	else
	{
		ALISETUP_bit.CR = 0;	  // Bypass mode
    ALISETUP_bit.ALIEN = 0; // Disable the ALI interface
	}

  // Registered interrupt handler
  IC_SetVectoredIRQ(Lcd_Handler,IC_LCDINTR);
  IC_EnableInt(1<<IC_LCDINTR);

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
    LCD_CONTROL_bit.LCDEN = 1;
    break;
  case LCD_DISABLE:     // Disable
    LCD_CONTROL_bit.LCDEN = 0;
    break;
  case LCD_PW_ENABLE:   // Enable LCD power
    LCD_CONTROL_bit.PWR = 1;
    LCD_CONTROL_bit.LCDEN = 1;
    break;
  case LCD_PW_DISABLE:  // Enable LCD power
    LCD_CONTROL_bit.PWR = 0;
    LCD_CONTROL_bit.LCDEN = 0;
    break;
  case LCD_SET_UP_FB:   // Update the upper frame buffer pointer
    // Sanity check
    if (Arg == 0)
    {
      return(FALSE);
    }
    // Set frame buffer address
    LCD_UPBASE = Arg;
    break;
  case LCD_SET_LOW_FB:   // Update the lower frame buffer pointer
    // Sanity check
    if (Arg == 0)
    {
      return(FALSE);
    }
    // Set frame buffer address
    LCD_LPBASE = Arg;
    break;
  case LCD_GET_STATUS:
    // Sanity check
    pArg = (Int32U *)Arg;
    if (pArg == NULL)
    {
      /* Null pointer */
      return(FALSE);
    }
    *pArg = LCD_CONTROL_bit.LCDEN;
    break;
  case LCD_DISABLE_INT:   // Mask interrupts at the source
    // Update the interrupt control register
    LCD_INTREN = LCD_INTREN & ~Arg;
    break;
  case LCD_ENABLE_INT:    // Unmask interrupts at the source
    // Update the interrupt control register
    LCD_INTREN = LCD_INTREN | Arg;
    break;
  case LCD_ENABLE_VCOMP_INT:
    // Write the new value to the control register
    LCD_CONTROL_bit.VCI = Arg;
    break;
  case LCD_DISABLE_VCOMP_INT:
    // Write the new value to the control register
    LCD_CONTROL_bit.VCI = 0;
    break;
  case LCD_SET_BGR:
    LCD_CONTROL_bit.BGR = 1;
    break;
  case LCD_SET_RGB:
    LCD_CONTROL_bit.BGR = 0;
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

