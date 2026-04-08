/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touch screen driver module
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 **************************************************************************/
#include <NXP/iomcimx31.h>
#include "arm_comm.h"
#include "board.h"
#include "drv_aitc.h"
#include "drv_touch_scr.h"
#include "drv_atlas.h"

/* GPT Tick per second*/
#define GPT_TICK_PER_SEC   50000

volatile Boolean Touch;
volatile Int16U  X,Y,Z;

/*************************************************************************
 * Function Name: Gpt_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT interrupt handler
 *
 *************************************************************************/
void GPT_Handler (void)
{
Int32U Data,Buffer[8];

  // Clear interrupt flag
  GPTSR_bit.OF1= 1;
  Data = Atlas_Rd_Wr(0, ATLAS_INTR_STAT0, 0);

  if(Data & 0x04)
  {
    // init measurement
    // ADREFEN, ADREFMODE, TSMOD = 2, ADINC1, ADINC2
    Atlas_Rd_Wr(0x033400, ATLAS_ADC0, 1);
    // clear interrupt
    Atlas_Rd_Wr(0x000005, ATLAS_INTR_STAT0, 1);
    // start conversion
    Atlas_Rd_Wr(0x310409, ATLAS_ADC1, 1);
  }
  else if (Data & 0x01)
  {
    Data = Atlas_Rd_Wr(0, ATLAS_ADC1, 0);
    Data &= 0x0007e0;
    Data |= 4UL << 8;
    Atlas_Rd_Wr(Data, ATLAS_ADC1, 1);
    for(Int32U i = 0; i < 4; ++i)
    {
      Data = Atlas_Rd_Wr(0, ATLAS_ADC2, 0);
      Buffer[i]   = (Data>> 2) & 0x3FF;
      Buffer[i+4] = (Data>>14) & 0x3FF;
    }
    // get pressure
    Z = Buffer[6] + Buffer[7];
    if(Z > TS_MIN_PRESSURE)
    {
      LED_ON(LED1);
      // clear interrupt
      Atlas_Rd_Wr(0x000001, ATLAS_INTR_STAT0, 1);
      // start conversion
      Atlas_Rd_Wr(0x310409, ATLAS_ADC1, 1);
      // collect data
      X = Buffer[0] + Buffer[1] + Buffer[2];
      Y = Buffer[3] + Buffer[4] + Buffer[5];
      Touch = TRUE;
    }
    else
    {
      LED_OFF(LED1);
      // init touch detection
      // ADREFEN, ADREFMODE, TSMOD = 1
      Atlas_Rd_Wr(0x001C00, ATLAS_ADC0, 1);
      // clear interrupt
      Atlas_Rd_Wr(0x00005, ATLAS_INTR_STAT0, 1);
      // touch interrupt mode
      Atlas_Rd_Wr(0x210001, ATLAS_ADC1, 1);
      Touch = FALSE;
    }
  }
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *
 *************************************************************************/
void TouchScrInit (void)
{
  // Init variable
  Touch = FALSE;
  X = Y = 0;
  // Init touch detection
  // ADC assignment to Prim SPI
  Atlas_Rd_Wr(Atlas_Rd_Wr(0, ATLAS_ARBT_PER_AUDIO, 0)|(1UL<<12),ATLAS_ARBT_PER_AUDIO,1);
  // ADREFEN, ADREFMODE, TSMOD = 1 - Touch screen interrupt mode
  Atlas_Rd_Wr(0x001c00, ATLAS_ADC0, 1);
  // clear interrupt
  Atlas_Rd_Wr(0xFFFFFF, ATLAS_INTR_STAT0, 1);
  // touch interrupt mode
  Atlas_Rd_Wr(0x210001, ATLAS_ADC1, 1);

  // Select IPG for timer clock
  GPTCR_bit.CLKSRC = 1;
  // Init the Timer prescaler
  GPTPR_bit.PRESCALER = 0;
  // Init timer tick interval
  GPTOCR1 = IPG_CLK/GPT_TICK_PER_SEC;
  // Init GPT1 interrupt
  AITC_SetupIntr(GPT_Handler,FALSE,INT_GPT,0);
  AITC_EnableIntSource(INT_GPT);
  //Enable Interrup on compare
  GPTIR_bit.OF1IE = 1;
  // Enable GPT1
  GPTCR_bit.EN = 1;
}

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData)
{
Boolean  TouchResReadyHold = Touch;
Int32U X_coordinate, Y_coordinate;

  if(TouchResReadyHold)
  {
    X_coordinate = X;
    Y_coordinate = Y;
    // Calculate X coordinate in pixels
    if ( LEFT_X <= X_coordinate)
    {
      X_coordinate = 0;
    }
    else if (RIGHT_X >= X_coordinate)
    {
      X_coordinate = IMAGE_WIDTH;
    }
    else
    {
      X_coordinate = ((LEFT_X - X_coordinate) * IMAGE_WIDTH)/(LEFT_X - RIGHT_X);
    }
    // Calculate Y coordinate in pixels
    if (UP_Y >= Y_coordinate)
    {
      Y_coordinate = 0;
    }
    else if (DOWN_Y <= Y_coordinate)
    {
      Y_coordinate = IMAGE_HEIGHT;
    }
    else
    {
      Y_coordinate = ((Y_coordinate - UP_Y) * IMAGE_HEIGHT)/(DOWN_Y - UP_Y);
    }
    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}
