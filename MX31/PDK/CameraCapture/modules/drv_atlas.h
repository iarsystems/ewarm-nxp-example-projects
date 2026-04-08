/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_atlas.h
 *    Description : Atlas MC13783 SPI driver header file 
 *
 *    History :
 *    1. Date        : 29.10.2008.  
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *    $Revision: 16 $
 **************************************************************************/
#ifndef __DRV_ATLAS_H
#define __DRV_ATLAS_H
/** include files **/

/** defines **/
#define ATLAS_RD 0
#define ATLAS_WR 1
/*Atlas Regs Definitions*/
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

/** public data **/
/** public functions **/
void Init_Atlas_SPI(void);
Int32U Atlas_Rd_Wr(Int32U Data, Int8U Addr, Boolean Rd_Wr);
#endif /* __DRV_ATLAS_H */
