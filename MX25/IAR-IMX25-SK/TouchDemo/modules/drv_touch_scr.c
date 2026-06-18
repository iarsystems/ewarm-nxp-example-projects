/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touch screen driver module
 *
 *    History :
 *    1. Date        : January, 8 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_touch_scr.h"
#include "drv_asic.h"
#include "drv_glcd.h"

#define LEFT_UP_X     180
#define RIGHT_UP_X    3900

#define LEFT_UP_Y     180
#define RIGHT_UP_Y    3800

#define TSC_4WIRE_PRECHARGE     0x158C
#define TSC_4WIRE_TOUCH_DETECT  0x578E
#define TSC_4WIRE_X_MEASUMENT   0x1C90
#define TSC_4WIRE_Y_MEASUMENT   0x4604

typedef union _Fifo_reg_t
{
  Int32U Data;
  struct
  {
    Int32U ITEM_ID    : 4;
    Int32U ADCDATAOUT :12;
    Int32U            :16;
  };
} Fifo_reg_t;

static volatile Boolean Touch,VR_NewData;
static volatile Int16U  X,Y,VR;

/*************************************************************************
 * Function Name: TS_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen interrupt handler
 *
 *************************************************************************/
void TS_Handler (void)
{

Int32U count[2];
Int32U acc[2];
Fifo_reg_t reg;

  if (TCQSR_bit.EOQ)
  {
    TCQSR_bit.EOQ = 1;
    count[0] = 0;
    acc[0]   = 0;
    count[1] = 0;
    acc[1]   = 0;
    while(!TCQSR_bit.EMPT)
    {
      reg.Data = TCQFIFO;
      switch(reg.ITEM_ID)
      {
      case 2:
        ++count[0];
        acc[0] += reg.ADCDATAOUT;
        break;
      case 3:
        ++count[1];
        acc[1] += reg.ADCDATAOUT;
        break;
      }
    }
    X = acc[0] / count[0];
    Y = acc[1] / count[1];
    Touch = TCQSR_bit.PD;
  }

  if (GCQSR_bit.EOQ)
  {
    GCQSR_bit.EOQ = 1;
    count[0] = 0;
    acc[0]   = 0;
    while(!GCQSR_bit.EMPT)
    {
      reg.Data = GCQFIFO;
      switch(reg.ITEM_ID)
      {
      case 0:
        ++count[0];
        acc[0] += reg.ADCDATAOUT;
        break;
      }
    }
    VR = acc[0] / count[0];
    VR_NewData = TRUE;

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

  CGCR2_bit.ipg_clk_tchscrn = 1;  // enable ipclk
  TGCR_bit.IPGCLKEN = 1;          // enable tsc ipgclken bit
  TGCR_bit.TSCRST = 1;            // self-reset tsc
  while(TGCR_bit.TSCRST);         // wait until self-reset done

  TCC0         = TSC_4WIRE_PRECHARGE;
  TCC0_bit.IGS = 1;               //ignore the ADC data during precharge;
  TCC1         = TSC_4WIRE_TOUCH_DETECT;
  TCC1_bit.IGS = 1;               //ignore the ADC data during precharge;
  TCC2         = TSC_4WIRE_X_MEASUMENT;
  TCC2_bit.NOS = TS_SAMPLES-1;    //4 samples for X measurement;
  TCC3         = TSC_4WIRE_Y_MEASUMENT;
  TCC3_bit.NOS = TS_SAMPLES-1;    //4 samples for Y measurement;

  // item 0 = 4-wire precharge
  TCQ_ITEM_7_0_bit.ITEM0 = 0;
  // item 1 = 4-wire detect
  TCQ_ITEM_7_0_bit.ITEM1 = 1;
  // item 2 = 4-wire x
  TCQ_ITEM_7_0_bit.ITEM2 = 3;
  // item 3 = 4-wire y
  TCQ_ITEM_7_0_bit.ITEM3 = 2;
  // item 4 = 4-wire precharge
  TCQ_ITEM_7_0_bit.ITEM4 = 0;
  // item 5 = 4-wire detect
  TCQ_ITEM_7_0_bit.ITEM5 = 1;

  TCQCR_bit.LAST_ITEM_ID = 5;

  // Init VR measure
  GCC0_bit.SELREFN = 2;               // NGND_ADC
  GCC0_bit.YNLRSW  = 0;               // YN(LR) off
  GCC0_bit.XNURSW  = 1;               // XN(UR) off
  GCC0_bit.XPULSW  = 0;               // XP(UL) off
  GCC0_bit.SELREFP = 2;               // External reference
  GCC0_bit.WIPERSW = 0;               // WIPER off
  GCC0_bit.SELIN   = 4;               // WIPER input
  GCC0_bit.NOS = 4-1;                 // 4 samples
  GCC0_bit.SETTLING_TIME = 16;

  GCQ_ITEM_7_0_bit.ITEM0 = 0;
  GCQCR_bit.QSM = 2;                  // New conversion queue is started by FQS setting
  GCQCR_bit.LAST_ITEM_ID = 0;         // Set last item

  TICR = TSC_4WIRE_PRECHARGE;         // or TICR = TSC_5WIRE_PRECHARGE;
  for(volatile Int32U i=1000;i;--i);  // add some delay for pre-charge
  TICR = TSC_4WIRE_TOUCH_DETECT;      // or TICR = TSC_5WIRE_TOUCH_DETECT;
  TCQCR_bit.QSM = 1;                  // set qsm as pen down invoke
  TCQCR_bit.PDCFG = 1;                // tcq pen down unmasked;
  TCQCR_bit.PDMSK = 0;
  TCQCR_bit.RPT = 0;
  TGCR_bit.PDBTIME = 0x7F;            // set debound time
  TGCR_bit.POWERMODE = 1;             // power saving
  TGCR_bit.PDBEN = 1;                 // enable debounce
  TGCR_bit.PDEN = 1;                  // pen down enable
  TCQMR_bit.PDIRQMSK = 1;             // enable tcq pen down interrupt
  TCQMR_bit.EOQIRQMSK = 0;

  TCQMR_bit.EOQIRQMSK = 0;            // enable tcq End-of-queue interrupt
  GCQMR_bit.EOQIRQMSK = 0;            // enable gp End-of-queue interrupt

  // Init TS interrupt
  ASIC_SetupIntr(TS_Handler,FALSE,INT_TSC,2);
  ASIC_EnableIntSource(INT_TSC);

}

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touch screen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData)
{
Boolean  TouchResReadyHold;
Int32U X_coordinate, Y_coordinate;

  TouchResReadyHold = Touch;

  if(TouchResReadyHold)
  {
    X_coordinate = X;
    Y_coordinate = Y;
    // Calculate X coordinate in pixels
    if (X_coordinate <= LEFT_UP_X)
    {
      X_coordinate = 0;
    }
    else if (X_coordinate >= RIGHT_UP_X)
    {
      X_coordinate = IMAGE_WIDTH;
    }
    else
    {
      X_coordinate = ((X_coordinate - LEFT_UP_X) * IMAGE_WIDTH)/(RIGHT_UP_X-LEFT_UP_X);
    }
    // Calculate Y coordinate in pixels
    if (Y_coordinate <= LEFT_UP_Y)
    {
      Y_coordinate = 0;
    }
    else if (X_coordinate >= RIGHT_UP_Y)
    {
      Y_coordinate = IMAGE_HEIGHT;
    }
    else
    {
      Y_coordinate = ((RIGHT_UP_Y - Y_coordinate) * IMAGE_HEIGHT)/(RIGHT_UP_Y-LEFT_UP_Y);
    }
    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}

/*************************************************************************
 * Function Name: VR_Measure
 * Parameters: none
 *
 * Return: Int16U
 *
 * Description: Return current voltage on WIPER input
 *
 *************************************************************************/
Int16U VR_Measure(void)
{
  VR_NewData = FALSE;
  GCQCR_bit.FQS = 1; // FQS starts the conversion queue
  GCQCR_bit.FQS = 0;
  while(!VR_NewData);
  return(VR);
}
