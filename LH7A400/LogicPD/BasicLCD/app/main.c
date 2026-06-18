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
 *    1. Date        : May, 15 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH7A400 evaluation boards.
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
unsigned long IntFlag = IC_GetIRQStatus(), Mask = 1, Count = 0;
  while(IntFlag)
  {
    if(IntFlag & Mask)
    {
      IntFlag &= ~Mask;
      IntrHanders[Count]();
    }
    Mask <<= 1;
    ++Count;
  }
}

/*************************************************************************
 * Function Name: Timer_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer interrupt Handler
 *
 *************************************************************************/
void Timer_handler(void)
{
static Int32U Dly  = TICK_PER_SECOND/TC_MEASURE_PER_SECOND;
  TIMER_TCEOI1 = 0;  // Clear interrupt flag
  if(--Dly == 0)
  {
    Dly  = TICK_PER_SECOND/TC_MEASURE_PER_SECOND;
    TouchScrStartMesure();
  }
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
Int32U Data;
int main(void)
{
ToushRes_t xy;
Int8U Paint = Colors[0];
Boolean FromDrawingScr = FALSE;
Int32U ColorIndx;

  // Init IO ports
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  IC_Init();

  // Init System timer 1ms
  TIMER_CONTROL1_bit.ENABLE = 0;  // Stop Timer 3 Count
  TIMER_CONTROL1_bit.CLKSEL = 1;  // Use Fast clock
  TIMER_CONTROL1_bit.MODE   = 1;  // Periodic
  // Set period
  TIMER_LOAD1_bit.LOAD      = TIMR12_FAST_CLK/(TICK_PER_SECOND);
  TIMER_TCEOI1 = 0;               // Clear interrupt flag
  TIMER_CONTROL1_bit.ENABLE = 1;  // Enable Timer 3 Count
  // registered the interrupt handler of Timer
  // Set address of the handler
  IC_SetVectoredIRQ(Timer_handler,IC_TC1OI);
  // Enable timer 0 interrupt
  IC_EnableInt(1<<IC_TC1OI);

  // Init Touch Scr
  TouchScrInit();
  // Load Paint screen
  LoadPicture((pInt32U)Paint_bmp,(pInt32U)FrameBuffer);
  // Init LCD Controller
  LcdInit(&NXPLQ035Q7DB02);
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
    TouchImp();
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
