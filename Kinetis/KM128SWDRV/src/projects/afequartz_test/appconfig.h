/******************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      appconfig.h
 * @author
 * @version
 * @date
 * @brief     Application configuration and interrupt vector definitions.
 ******************************************************************************/
#ifndef __APPCONFIG_H
#define __APPCONFIG_H

/***************************************************************************//*!
 * @brief   Reserve trace buffer in RAM for Micro Trace Buffer (MTB) trace
 *          capabilities.
 * @param   MTB_RAM_RESERVE - this define reserves trace buffer in RAM for use
 *          by Micro Trace Buffer (MTB). The trace buffer starts at 0x1ffff000
 *          and it has size of 64-bytes. You can modify size of the trace buffer
 *          for example to 256-bytes by the following definition:
 *          #define MTB_RAM_SIZE 0x100
 * @note    Tracing capabilities reduces performance of the application.
 ******************************************************************************/
//#define MTB_RAM_RESERVE

/***************************************************************************//*!
 * @brief   Modify default boot options. The boot options allows the MCU to
 *          modify default operations from NVM at reset.
 *          Supported options:
 * @param   BOOT_FLASH_SECURE - MCU Secured (MCU is unsecured by default; secure
 *                              command is blocked in IAR 6.40.2 flash loader)
 * @param   BOOT_DIV_CLK_BY_8 - Divide clock by 8 (MCU boots up with system clock
 *                              divide by 1 by default).
 * @param   BOOT_NMI_DISABLE  - Disable NMI (NMI is enabled by default)
 * @param   BOOT_EXT_CLK      - Device uses external slower clock for flash
 *                              calibration after reset. Internal 25 MHz flash
 *                              clock is disabled.
 * @param   BOOT_HWVLPR_MODE  - Device boots in VLPR mode starting next VDD power
 *                              cycle provided VBAT power is retained (forced by HW).
 * @param   BOOT_SWVLPR_MODE  - Device boots in RUN mode however device enters
 *                              VLPR by first software instructions of the device
 *                              startup (forced by SW).
 * @note    If neither BOOT_EXE_MODE, BOOT_VLPR_MODE nor BOOT_EXT_FLASH_CLK
 *          constants are defined then device boots in default mode which is RUN
 *          mode with internal 25 MHz clock enabled and used for flash calibration.
 ******************************************************************************/
#define BOOT_NMI_DISABLE

/***************************************************************************//*!
 * @brief   Define specific peripheral configuration structures.
 * @example
 *          #define PIT_CH_CONFIG                                              \
 *          (tPIT_CH){                                                         \
 *            SET(PIT_TCTRL_TIE_MASK)|SET(PIT_TCTRL_TEN_MASK),                 \
 *          }
 ******************************************************************************/

/***************************************************************************//*!
 * @brief FLL Engaged Internal (DCO Range: 40-50 MHz). FLL clocked by
 * 32 kHz Internal Reference Clock. External reference oscillator operation
 * enabled.
 ******************************************************************************/
#define FLL_MODULE_FEI_40_50MHZ_OSC_RANGE_02_EN_CONFIG                        \
(tFLL){                                                                       \
/* C1 */ SET(MCG_C1_CLKS(0x00))|SET(MCG_C1_FRDIV(0x00))|                      \
         SET(MCG_C1_IREFS_MASK)|CLR(MCG_C1_IRCLKEN_MASK)|                     \
         CLR(MCG_C1_IREFSTEN_MASK),                                           \
/* C2 */ SET(MCG_C2_LOCRE0_MASK)|SET(MCG_C2_RANGE0(0x02))|CLR(MCG_C2_HGO0_MASK)|\
         SET(MCG_C2_EREFS0_MASK)|CLR(MCG_C2_LP_MASK)|SET(MCG_C2_IRCS_MASK),   \
/* C4 */ CLR(MCG_C4_DMX32_MASK)|SET(MCG_C4_DRST_DRS(0x01)),                   \
/* C6 */ CLR(MCG_C6_LOLIE0_MASK)|CLR(MCG_C6_PLLS_MASK)|CLR(MCG_C6_CME0_MASK)| \
         SET(MCG_C6_CHGPMP_BIAS(0x04)),                                       \
/* SC */ CLR(MCG_SC_ATME_MASK)|CLR(MCG_SC_ATMS_MASK)|                         \
         CLR(MCG_SC_FLTPRSRV_MASK)|SET(MCG_SC_FCRDIV(0x01)),                  \
/* C7 */ SET(MCG_C7_PLL32KREFSEL(0))|CLR(MCG_C7_OSCSEL_MASK),                 \
/* C8 */ SET(MCG_C8_LOCRE1_MASK)|CLR(MCG_C8_LOLRE_MASK)|CLR(MCG_C8_CME1_MASK)|\
         CLR(MCG_C8_COARSE_LOLIE_MASK),                                       \
}

/******************************************************************************/
#endif /* __APPCONFIG_ */
