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
#define ADC_MODULE_16B_SWTRG_BGAP_LP_INTERNAL_CLK_CONFIG                      \
(tADC){                                                                       \
/* CFG1   */  SET(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(0))|                 \
              CLR(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(3)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|SET(ADC_CFG2_ADACKEN_MASK)|           \
              CLR(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(3)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  CLR(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(2)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}

/******************************************************************************/
#endif /* __APPCONFIG_ */
