/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : May, 12 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH79524 evaluation boards.
 *
 *  The goal of this demonstration project is to show how to use the LCD and
 * the touch screen controller.
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "includes.h"

#define DRW_SCR_X       0
#define DRW_SCR_Y       16
#define DRW_SCR_SIZE_X  240
#define DRW_SCR_SIZE_Y  264

#define NEW_SCR_X       8
#define NEW_SCR_Y       287
#define NEW_SCR_SIZE_X  28
#define NEW_SCR_SIZE_Y  28

#define CB_X            44
#define CB_Y            288
#define CB_SIZE_X       185
#define CB_SIZE_Y       28

const Int8U Colors [] =
{
  0x00,0xFF,0x1F,0x1C,0xE0,0x03,
};

#pragma segment="DATA_DMA"
#pragma location="DATA_DMA"
#pragma data_alignment=4
__no_init Int8U FrameBuffer[LCD_WIDTH*LCD_HEIGHT*1/*BPP 8 bit*/];

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ Handler
 *
 *************************************************************************/
__irq __arm void irq_handler(void)
{
void (*interrupt_function)();
unsigned int vector;
  vector = VIC_VectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function
  VIC_VectAddr = 0;         // Clean interrupt in VIC
}

/*************************************************************************
 * Function Name: Timr0_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt Handler
 *
 *************************************************************************/
void Timr0_handler(void)
{
  TIMER_STATUS0_bit.CMP1_ST = 1; // Clean interrupt
  TouchScrStartMesure();
}

/*************************************************************************
 * Function Name: LoadPicture
 * Parameters: pInt32U pSrc, pInt32U pDst
 *
 * Return: none
 *
 * Description: Load Picture
 *
 *************************************************************************/
void LoadPicture (pInt32U pSrc, pInt32U pDst)
{
Int32U i, Size;
  if((pDst == NULL) || (pSrc == NULL))
  {
    return;
  }
  for(i = 0, Size = *pSrc, pSrc += 4;
      i < Size;
      ++i, ++pSrc, ++pDst)
  {
    *pDst = *pSrc;
  }
}

/*************************************************************************
 * Function Name: CheckRectangle
 * Parameters: Int32U X_up, Int32U Y_up - rectangle coordinate
 *             Int32U X_size, Int32U Y_size - rectangle size
 *             Int32U X_poin, Int32U Y_point - point coordinate
 *
 * Return: Boolean
 *    TRUE  - the point is inside  from the rectangle
 *    FALSE - the point is outside from the rectangle
 *
 * Description: Check whether the coordinate of point is inside from a rectangle
 *
 *************************************************************************/
Boolean CheckRectangle (Int32U X_up, Int32U Y_up,
                        Int32U X_size, Int32U Y_size,
                        Int32U X_point, Int32U Y_point )
{
  if((X_up <= X_point) && (X_point <= X_up+X_size) &&
     (Y_up <= Y_point) && (Y_point <= Y_up+Y_size))
  {
    return (TRUE);
  }
  return (FALSE);
}

/*************************************************************************
 * Function Name: DrawPoint
 * Parameters: Int16U X, Int16U Y, Int8U Paint
 *
 * Return: None
 *
 * Description: Draw a point at coordinates X,Y
 *
 *************************************************************************/
void DrawPoint(Int16U X, Int16U Y, Int8U Paint)
{
Int32U Indx;

  if((X >= LCD_WIDTH) || (Y >= LCD_HEIGHT))
  {
    return;
  }
  Indx = X + Y*LCD_WIDTH;
  FrameBuffer[Indx] = Paint;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main(void)
{
ToushRes_t xy;
Int8U Paint = Colors[0];
Boolean FromDrawingScr = FALSE;
Int32U ColorIndx;

  // Init VIC controller
  VIC_Init();

  // Init Tick timer
  TIMER_CTRL0_bit.CS        = 0; // Stops counter
  TIMER_CTRL0_bit.CCL       = 1; // Counter Clear
  TIMER_CTRL0_bit.SEL       = 6; // HCLK/128
  CMP_CAP_CTRL_bit.TC       = 1; // Clear the timer counter on Mach
  // Set Timer 0 period
  T0CMP1 = HCLK_FREQ/(128*TICK_PER_SECOND);
  TIMER_STATUS0_bit.CMP1_ST = 1; // Clear pending interrupt
  INTEN0_bit.CMP1_EN        = 1; // Enable interrupt request for compare 1
  TIMER_CTRL0_bit.CS        = 1; // Enable counter
  // Enable timer 0 interrupt
  VIC_IntEnable_bit.TIMER0INTR = 1;
  // registered the interrupt handler of Timer1
  VIC_SetVectoredIRQ(Timr0_handler,VIC_Slot0,Timer0_IRQ);
  VIC_EnableInt(1UL << Timer0_IRQ);

  // Init Touch Scr
  TouchScrInit(VIC_Slot1,VIC_Slot2);

  // Load Paint screen
  LoadPicture((pInt32U)Paint_bmp,(pInt32U)FrameBuffer);
  // Init LCD Controller
  LcdInit(&NXPLQ035Q7DB02,VIC_Slot3);
  //  Set Palette
  LcdIoCtrl(LCD_SET_PALETTE,(Int32U)Paint_Palette);
  //  Set LCD frame buffer
  LcdIoCtrl(LCD_SET_UP_FB,(Int32U)FrameBuffer);
  //  Enable LCD Controller
  LcdIoCtrl(LCD_ENABLE,0);
  //  LCD Power enable
  LcdIoCtrl(LCD_PW_ENABLE,0);

  // Enable interrupts.
  __enable_interrupt();
  while(1)
  {
    if(TouchScrGetStatus(&xy))
    {
      STAT_LED1_ON();
      if(CheckRectangle(DRW_SCR_X,DRW_SCR_Y,DRW_SCR_SIZE_X,DRW_SCR_SIZE_Y,xy.X,xy.Y))
      {
        // Draw a point
        DrawPoint(xy.X, xy.Y, Paint);
        FromDrawingScr = TRUE;
      }
      else if (!FromDrawingScr)
      {
        if(CheckRectangle(NEW_SCR_X,NEW_SCR_Y,NEW_SCR_SIZE_X,NEW_SCR_SIZE_Y,xy.X,xy.Y))
        {
          // Reload the Paint screen
          LoadPicture((pInt32U)Paint_bmp,(pInt32U)FrameBuffer);
          FromDrawingScr = TRUE;
        }
        else if(CheckRectangle(CB_X,CB_Y,CB_SIZE_X,CB_SIZE_Y,xy.X,xy.Y))
        {
          // Select a color
          xy.Y -= CB_Y;
          xy.X -= CB_X;
          ColorIndx = xy.X/31;
          Paint = Colors[ColorIndx];
        }
      }
    }
    else
    {
      FromDrawingScr = FALSE;
      STAT_LED1_OFF();
    }
  }

}
