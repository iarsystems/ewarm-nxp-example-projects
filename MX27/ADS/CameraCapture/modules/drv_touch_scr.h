/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_touch_scr.h
 *    Description : Touch screen driver include file
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#ifndef __DRV_TOUCH_SCR_H
#define __DRV_TOUCH_SCR_H

// Touched to untouched time out
#define TS_MIN_PRESSURE   200

// display upper left corner
#define LEFT_UP_X         85*3
#define LEFT_UP_Y         85*3

// display down right corner
#define RIGHT_DOWN_X      950*3
#define RIGHT_DOWN_Y      950*3

// Atlas registers
typedef enum {
  ATLAS_INTR_STAT0 = 0, ATLAS_INTR_MASK0, ATLAS_INTR_SENSE0, ATLAS_INTR_STAT1,
  ATLAS_INTR_MASK1, ATLAS_INTR_SENSE1, ATLAS_PWR_MODE_SENSE, ATLAS_REV, ATLAS_SEM,
  ATLAS_ARBT_PER_AUDIO, ATLAS_ARBT_SWITCH, ATLAS_ARBT_REG0, ATLAS_ARBT_REG1,
  ATLAS_PWR0, ATLAS_PWR1, ATLAS_PWR2, ATLAS_REGEN_ASSIGN, ATLAS_CTRL_SPARE,
  ATLAS_MEMA, ATLAS_MEMB, ATLAS_RTC_TIME, ATLAS_RTC_ALRM, ATLAS_RTC_DAY,
  ATLAS_RTC_DAY_ALRM, ATLAS_SWITCH0, ATLAS_SWITCH1, ATLAS_SWITCH2, ATLAS_SWITCH3,
  ATLAS_SWITCH4, ATLAS_SWITCH5, ATLAS_REG_SET0, ATLAS_REG_SET1, ATLAS_REG_MODE0,
  ATLAS_REG_MODE1, ATLAS_PWR_MISC, ATLAS_PWR_SPARE, ATLAS_AUDIO_RX0, ATLAS_AUDIO_RX1,
  ATLAS_AUDIO_TX, ATLAS_SSI_NETWORK, ATLAS_CODEC, ATLAS_STEREO_DAC, ATLAS_AUDIO_SPARE,
  ATLAS_ADC0, ATLAS_ADC1, ATLAS_ADC2, ATLAS_ADC3, ATLAS_ADC4, ATLAS_CHARGER,
  ATLAS_USB, ATLAS_CHRG_USB_SPARE, ATLAS_LED0, ATLAS_LED1, ATLAS_LED2, ATLAS_LED3,
  ATLAS_LED4, ATLAS_LED5, ATLAS_SPARE, ATLAS_TRIM0, ATLAS_TRIM1, ATLAS_TEST0,
  ATLAS_TEST1, ATLAS_TEST2, ATLAS_TEST3
} atlas_reg_set_t;


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
 * Function Name: Gpt2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT2 interrupt handler
 *
 *************************************************************************/
void Gpt2_Handler (void);

/*************************************************************************
 * Function Name: Data_Transfer
 * Parameters: Int32U Data, Int8U Addr, Boolean Write
 *
 * Return: Int32U
 *
 * Description: Transfer data by CSPI1
 *
 *************************************************************************/
static
Int32U Data_Transfer(Int32U Data, Int8U Addr, Boolean Write);

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *
 *************************************************************************/
void TouchScrInit (void);

/*************************************************************************
 * Function Name: TouchGet
 * Parameters: ToushRes_t * pData -  X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData);

#endif // __DRV_TOUCH_SCR_H
