/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_CS43L22.h
 *    Description : CS43L22 driver include file
 *
 *    History :
 *    1. Date        : 14 January, 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

#ifndef __DRV_CS43L22_H
#define __DRV_CS43L22_H

#define CS43L22_SLAVE_ADDR  0x94

typedef enum _CS43L22_Reg_t
{
  CS43L22_ID_REG = 0x01,
  CS43L22_PWR_CTRL1_REG, CS43L22_PWR_CTRL2_REG = 0x04,
  CS43L22_CLK_CTRL_REG,
  CS43L22_INTF_CTRL1_REG, CS43L22_INTF_CTRL2_REG,
  CS43L22_PASSTHR_A_REG, CS43L22_PASSTHR_B_REG,
  CS43L22_ZC_SR_REG,
  CS43L22_PASSTHR_GAIN_CTRL_REG = 0x0C,
  CS43L22_PLAYBACK_CTRL1_REG, CS43L22_MISC_CTRL_REG, CS43L22_PLAYBACK_CTRL2_REG,
  CS43L22_PASSTHR_VOL_A_REG = 0x14, CS43L22_PASSTHR_VOL_B_REG,
  CS43L22_PCMA_VOL_REG = 0x1A, CS43L22_PCMB_VOL_REG,
  CS43L22_BEEP_FREQ_ON_REG, CS43L22_BEEP_VOL_OFF_REG, CS43L22_BEEP_TONE_REG,
  CS43L22_TONE_CTRL_REG,
  CS43L22_MASTER_VOL_A_REG, CS43L22_MASTER_VOL_B_REG,
  CS43L22_HEADPHONE_VOL_A_REG, CS43L22_HEADPHONE_VOL_B_REG,
  CS43L22_SPK_VOL_A_REG, CS43L22_SPK_VOL_B_REG,
  CS43L22_CH_MIX_SWAP_REG,
  CS43L22_LIM_CTRL1_REG, CS43L22_LIM_CTRL2_REG, CS43L22_LIM_ATTACK_REG,
  CS43L22_STA_REG = 0x2E,
  CS43L22_BATT_COMP_REG, CS43L22_VP_BATT_LEVEL_REG,
  CS43L22_SPK_STA_REG,
  CS43L22_CHARGE_PUMP_FREQ_REG = 0x34
} CS43L22_Reg_t;

#define CS43L22_ADDR_INC (1U<<7)

typedef enum _BeepType_t
{
  CS43L22_BEEP_OFF = 0, CS43L22_BEEP_SINGLE,
  CS43L22_BEEP_MULT, CS43L22_BEEP_CONT
} BeepType_t;

Boolean CS43L22_Init (void);
Boolean CS43L22_Vol (Int32U Vol);
Boolean CS43L22_BeepCfg (Int32U Freq, Int32U On, Int32U off);
Boolean CS43L22_Beep (BeepType_t Type);


#endif // __DRV_CS43L22_H
