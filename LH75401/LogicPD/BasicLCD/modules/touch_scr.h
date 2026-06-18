/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : touch_scr.h
 *    Description : Prototypes for the touch screen driver
 *
 *    History :
 *    1. Date        : 5 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "includes.h"

#ifndef __TOUCH_SCR_H
#define __TOUCH_SCR_H

// ADC clk frequency must be between 200kHz and 2 MHz
#define ADC_CLK_DIV         4             // OSC_FREQ/64

// Define Setup delays [us]
#define IDLY_DLY            100
#define PRE_CHRAGE_DLY      50
#define TOUCH_DET_DLY       50
#define X_MEAS_DLY          50
#define Y_MEAS_DLY          50


// Pen detect threshold
#define TOUCH_THRESHOLD     (1024*2/3)      // 2/3

// ADC Frequency
#define ADC_FREQ            (MAIN_OSC_CLK/(1UL << ADC_CLK_DIV))

// Number of Conversions in Sequence
#define TOUCH_MEASURE_STEPS 12

// Define High Word bitfields
#define RMP_REF_P           0
#define RMP_AN0             1
#define RMP_AN2             2
#define RMP_AN8             3

#define IMN_REF_N           0
#define IMN_GND             4

#define PMN_AN0             0x00
#define PMN_AN1             0x08
#define PMN_AN2             0x10
#define PMN_AN3             0x18
#define PMN_AN4             0x20
#define PMN_AN6             0x30
#define PMN_AN8             0x40
#define PMN_AN9             0x48
#define PMN_REF_N           0x50

// Delay [us]
#define ADC_SetTime(Setup) \
  (((ADC_FREQ*Setup)/1000000) << 7)

// Define Low Word bitfields
#define RMN_VREF_N          0
#define RMN_AN1             1
#define RMN_AN3             2
#define RMN_AN9             3

#define AN0_VDDA_STRONG    (1UL << 2 )
#define AN1_VDDA_STRONG    (1UL << 3 )
#define AN1_GND_STRONG     (1UL << 4 )
#define AN2_VDDA_STRONG    (1UL << 5 )
#define AN2_GND_STRONG     (1UL << 6 )
#define AN3_GND_STRONG     (1UL << 7 )
#define AN4_GND_STRONG     (1UL << 8 )
#define AN0_VDDA_WEAK      (1UL << 12)
#define AN4_VDDA_WEAK      (1UL << 13)

typedef enum
{
  TOUCH_SCR_IDLE = 0, TOUCH_SCR_PEN_DETECTION,
} TouchScrState_t;

typedef struct
{
  Int16U LowWord;
  Int16U HighWord;
} AdcCtrlWords_t;

// display upper left corner
#define LEFT_UP_X         70
#define LEFT_UP_Y         70

// display down right corner
#define RIGHT_UP_X        920
#define RIGHT_UP_Y        920

typedef union
{
  Int32U XY;
  struct
  {
    Int16U  X;
    Int16U  Y;
  };
} ToushRes_t;

/*************************************************************************
 * Function Name: TouchScrEOC_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen end of conversion interrupt handler
 *
 *************************************************************************/
void TouchScrEOC_Handler (void);

/*************************************************************************
 * Function Name: TouchScrPT_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen Pen interrupt handler
 *
 *************************************************************************/
void TouchScrPen_Handler (void);

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: VicIrqSlots_t PenTouchSlot,VicIrqSlots_t EndOfConvSlot
 *
 * Return: none
 *
 * Description: Init Touch screen sequence
 *
 *************************************************************************/
void TouchScrInit (VicIrqSlots_t PenTouchSlot,VicIrqSlots_t EndOfConvSlot);

/*************************************************************************
 * Function Name: TouchScrStartMesure
 * Parameters: none
 *
 * Return: none
 *
 * Description: Starting the measurement
 *
 *************************************************************************/
void TouchScrStartMesure (void);

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
Boolean TouchScrGetStatus (ToushRes_t * pData);

#endif // __TOUCH_SCR_H
