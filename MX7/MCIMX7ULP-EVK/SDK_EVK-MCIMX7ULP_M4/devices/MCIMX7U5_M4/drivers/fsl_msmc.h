/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _FSL_MSMC_H_
#define _FSL_MSMC_H_

#include "fsl_common.h"

/*! @addtogroup msmc */
/*! @{*/

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief MSMC driver version 2.0.0. */
#define FSL_MSMC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*!
 * @brief Power Modes Protection
 */
typedef enum _smc_power_mode_protection
{
    kSMC_AllowPowerModeVlls = SMC_PMPROT_AVLLS_MASK,   /*!< Allow Very-Low-Leakage Stop Mode. */
    kSMC_AllowPowerModeLls = SMC_PMPROT_ALLS_MASK,     /*!< Allow Low-Leakage Stop Mode.      */
    kSMC_AllowPowerModeVlp = SMC_PMPROT_AVLP_MASK,     /*!< Allow Very-Low-Power Mode.        */
    kSMC_AllowPowerModeHsrun = SMC_PMPROT_AHSRUN_MASK, /*!< Allow High Speed Run mode.        */
    kSMC_AllowPowerModeAll = SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLP_MASK |
                             SMC_PMPROT_AHSRUN_MASK /*!< Allow all power mode.              */
} smc_power_mode_protection_t;

/*!
 * @brief Power Modes in PMSTAT
 */
typedef enum _smc_power_state
{
    kSMC_PowerStateRun = 0x01U,        /*!< 0000_0001 - Current power mode is RUN   */
    kSMC_PowerStateStop = 0x01U << 1U, /*!< 0000_0010 - Current power mode is any STOP mode  */
    kSMC_PowerStateVlpr = 0x01U << 2U, /*!< 0000_0100 - Current power mode is VLPR  */
    kSMC_PowerStateHsrun = 0x01U << 7U /*!< 1000_0000 - Current power mode is HSRUN */
} smc_power_state_t;

/*!
 * @brief Run mode definition
 */
typedef enum _smc_run_mode
{
    kSMC_RunNormal = 0U, /*!< normal RUN mode.             */
    kSMC_RunVlpr = 2U,   /*!< Very-Low-Power RUN mode.     */
    kSMC_Hsrun = 3U      /*!< High Speed Run mode (HSRUN). */
} smc_run_mode_t;

/*!
 * @brief Stop mode definition
 */
typedef enum _smc_stop_mode
{
    kSMC_StopNormal = 0U, /*!< Normal STOP mode.           */
    kSMC_StopVlps = 2U,   /*!< Very-Low-Power STOP mode.   */
    kSMC_StopLls = 3U,    /*!< Low-Leakage Stop mode.      */
    kSMC_StopVlls = 4U    /*!< Very-Low-Leakage Stop mode. */
} smc_stop_mode_t;

/*!
 * @brief Partial STOP option
 */
typedef enum _smc_partial_stop_mode
{
    kSMC_PartialStop = 0U,  /*!< STOP - Normal Stop mode*/
    kSMC_PartialStop1 = 1U, /*!< Partial Stop with both system and bus clocks disabled*/
    kSMC_PartialStop2 = 2U, /*!< Partial Stop with system clock disabled and bus clock enabled*/
    kSMC_PartialStop3 = 3U, /*!< Partial Stop with system clock enabled and bus clock disabled*/
} smc_partial_stop_option_t;

/*!
 * @brief SMC configuration status
 */
enum _smc_status
{
    kStatus_SMC_StopAbort = MAKE_STATUS(kStatusGroup_POWER, 0), /*!< Entering Stop mode is abort*/
};

/*!
 * @brief System Reset Source Name definitions
 */
typedef enum _smc_reset_source
{
    kSMC_SourceWakeup = SMC_SRS_WAKEUP_MASK, /*!< Very low-leakage wakeup reset */
    kSMC_SourcePor = SMC_SRS_POR_MASK,       /*!< Power on reset */
    kSMC_SourceLvd = SMC_SRS_LVD_MASK,       /*!< Low-voltage detect reset */
    kSMC_SourceHvd = SMC_SRS_HVD_MASK,       /*!< High-voltage detect reset */
    kSMC_SourceWarm = SMC_SRS_WARM_MASK,     /*!< Warm reset. Warm Reset flag will assert if any of the system reset
                                                 sources in this register assert (SRS[31:8]) */
    kSMC_SourceFatal = SMC_SRS_FATAL_MASK,   /*!< Fatal reset */
    kSMC_SourceCore =
        SMC_SRS_CORE_MASK, /*!< Software reset that only reset the core, NOT a sticky system reset source. */
    kSMC_SourcePin = SMC_SRS_PIN_MASK,         /*!< RESET_B pin reset. */
    kSMC_SourceMdm = SMC_SRS_MDM_MASK,         /*!< MDM reset. */
    kSMC_SourceRstAck = SMC_SRS_RSTACK_MASK,   /*!< Reset Controller timeout reset. */
    kSMC_SourceStopAck = SMC_SRS_STOPACK_MASK, /*!< Stop timeout reset */
    kSMC_SourceScg = SMC_SRS_SCG_MASK,         /*!< SCG loss of lock or loss of clock */
    kSMC_SourceWdog = SMC_SRS_WDOG_MASK,       /*!< Watchdog reset */
    kSMC_SourceSoftware = SMC_SRS_SW_MASK,     /*!< Software reset */
    kSMC_SourceLockup = SMC_SRS_LOCKUP_MASK,   /*!< Lockup reset. Core lockup or exception. */
    kSMC_SourceJtag = SMC_SRS_JTAG_MASK,       /*!< JTAG system reset */
    kSMC_SourceSecVio = SMC_SRS_SECVIO_MASK,   /*!< Security violation reset */
    kSMC_SourceTamper = SMC_SRS_TAMPER_MASK,   /*!< Tamper reset */
    kSMC_SourceAll = SMC_SRS_WAKEUP_MASK | SMC_SRS_POR_MASK | SMC_SRS_LVD_MASK | SMC_SRS_HVD_MASK | SMC_SRS_WARM_MASK |
                     SMC_SRS_FATAL_MASK | SMC_SRS_CORE_MASK | SMC_SRS_PIN_MASK | SMC_SRS_MDM_MASK |
                     SMC_SRS_RSTACK_MASK | SMC_SRS_STOPACK_MASK | SMC_SRS_SCG_MASK | SMC_SRS_WDOG_MASK |
                     SMC_SRS_SW_MASK | SMC_SRS_LOCKUP_MASK | SMC_SRS_JTAG_MASK | SMC_SRS_SECVIO_MASK |
                     SMC_SRS_TAMPER_MASK,
} smc_reset_source_t;

/*!
 * @brief System reset interrupt enable bit definitions.
 */
typedef enum _smc_interrupt_enable
{
    kSMC_IntNone = 0U,                       /*!< No interrupt enabled.       */
    kSMC_IntPin = SMC_SRIE_PIN_MASK,         /*!< Pin reset interrupt.        */
    kSMC_IntMdm = SMC_SRIE_MDM_MASK,         /*!< MDM reset interrupt.        */
    kSMC_IntStopAck = SMC_SRIE_STOPACK_MASK, /*!< Stop timeout reset interrupt.  */
    kSMC_IntWdog = SMC_SRIE_WDOG_MASK,       /*!< Watchdog interrupt.         */
    kSMC_IntSoftware = SMC_SRIE_SW_MASK,     /*!< Software reset interrupts.  */
    kSMC_IntLockup = SMC_SRIE_LOCKUP_MASK,   /*!< Lock up interrupt           */
    kSMC_IntAll = SMC_SRIE_PIN_MASK |        /*!< All system reset interrupts.      */
                  SMC_SRIE_MDM_MASK |
                  SMC_SRIE_STOPACK_MASK | SMC_SRIE_WDOG_MASK | SMC_SRIE_SW_MASK | SMC_SRIE_LOCKUP_MASK
} smc_interrupt_enable_t;

/*!
 * @brief Reset pin filter configuration
 */
typedef struct _smc_reset_pin_filter_config
{
    uint8_t slowClockFilterCount; /*!< Reset pin bus clock filter width from 1 to 32 slow clock cycles.  */
    bool enableFilter;            /*!< Reset pin filter enable/disable. */
} smc_reset_pin_filter_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name System mode controller APIs*/
/*@{*/

/*!
 * @brief Configures all power mode protection settings.
 *
 * This function  configures the power mode protection settings for
 * supported power modes in the specified chip family. The available power modes
 * are defined in the smc_power_mode_protection_t. This should be done at an early
 * system level initialization stage. See the reference manual for details.
 * This register can only write once after the power reset.
 *
 * The allowed modes are passed as bit map, for example, to allow LLS and VLLS,
 * use SMC_SetPowerModeProtection(kSMC_AllowPowerModeLls | kSMC_AllowPowerModeVlls).
 * To allow all modes, use SMC_SetPowerModeProtection(kSMC_AllowPowerModeAll).
 *
 * @param allowedModes Bitmap of the allowed power modes.
 */
static inline void SMC_SetPowerModeProtection(SMC_Type *base, uint8_t allowedModes)
{
    base->PMPROT = allowedModes;
}

/*!
 * @brief Gets the current power mode status.
 *
 * This function  returns the current power mode stat. Once application
 * switches the power mode, it should always check the stat to check whether it
 * runs into the specified mode or not. An application  should  check
 * this mode before switching to a different mode. The system  requires that
 * only certain modes can switch to other specific modes. See the
 * reference manual for details and the smc_power_state_t for information about
 * the power stat.
 *
 * @return Current power mode status.
 */
static inline smc_power_state_t SMC_GetPowerModeState(SMC_Type *base)
{
    return (smc_power_state_t)base->PMSTAT;
}

/*!
 * @brief Prepare to enter stop modes
 *
 * This function should be called before entering STOP/VLPS/LLS/VLLS modes.
 */
static inline void SMC_PreEnterStopModes(void)
{
    __disable_irq();
    __ISB();
}

/*!
 * @brief Recovering after wake up from stop modes
 *
 * This function should be called after wake up from STOP/VLPS/LLS/VLLS modes.
 * It is used together with @ref SMC_PreEnterStopModes.
 */
static inline void SMC_PostExitStopModes(void)
{
    __enable_irq();
    __ISB();
}

/*!
 * @brief Prepare to enter wait modes
 *
 * This function should be called before entering WAIT/VLPW modes..
 */
static inline void SMC_PreEnterWaitModes(void)
{
    __disable_irq();
    __ISB();
}

/*!
 * @brief Recovering after wake up from stop modes
 *
 * This function should be called after wake up from WAIT/VLPW modes.
 * It is used together with @ref SMC_PreEnterWaitModes.
 */
static inline void SMC_PostExitWaitModes(void)
{
    __enable_irq();
    __ISB();
}

/*!
 * @brief Configure the system to RUN power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeRun(SMC_Type *base);

/*!
 * @brief Configure the system to HSRUN power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeHsrun(SMC_Type *base);

/*!
 * @brief Configure the system to WAIT power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeWait(SMC_Type *base);

/*!
 * @brief Configure the system to Stop power mode.
 *
 * @param base SMC peripheral base address.
 * @param  option Partial Stop mode option.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeStop(SMC_Type *base, smc_partial_stop_option_t option);

/*!
 * @brief Configure the system to VLPR power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlpr(SMC_Type *base);

/*!
 * @brief Configure the system to VLPW power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlpw(SMC_Type *base);

/*!
 * @brief Configure the system to VLPS power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlps(SMC_Type *base);
/*!
 * @brief Configure the system to LLS power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeLls(SMC_Type *base);

/*!
 * @brief Configure the system to VLLS power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlls(SMC_Type *base);

/*!
 * @brief Gets the reset source status which caused a previous reset.
 *
 * This function gets the current reset source status. Use source masks
 * defined in the smc_reset_source_t to get the desired source status.
 *
 * Example:
   @code
   uint32_t resetStatus;

   // To get all reset source statuses.
   resetStatus = SMC_GetPreviousResetSources(SMC0) & kSMC_SourceAll;

   // To test whether the MCU is reset using Watchdog.
   resetStatus = SMC_GetPreviousResetSources(SMC0) & kSMC_SourceWdog;

   // To test multiple reset sources.
   resetStatus = SMC_GetPreviousResetSources(SMC0) & (kSMC_SourceWdog | kSMC_SourcePin);
   @endcode
 *
 * @param base SMC peripheral base address.
 * @return All reset source status bit map.
 */
static inline uint32_t SMC_GetPreviousResetSources(SMC_Type *base)
{
    return base->SRS;
}

/*!
 * @brief Gets the sticky reset source status.
 *
 * This function gets the current reset source status that has not been cleared
 * by software for some specific source.
 *
 * Example:
   @code
   uint32_t resetStatus;

   // To get all reset source statuses.
   resetStatus = SMC_GetStickyResetSources(SMC0) & kSMC_SourceAll;

   // To test whether the MCU is reset using Watchdog.
   resetStatus = SMC_GetStickyResetSources(SMC0) & kSMC_SourceWdog;

   // To test multiple reset sources.
   resetStatus = SMC_GetStickyResetSources(SMC0) & (kSMC_SourceWdog | kSMC_SourcePin);
   @endcode
 *
 * @param base SMC peripheral base address.
 * @return All reset source status bit map.
 */
static inline uint32_t SMC_GetStickyResetSources(SMC_Type *base)
{
    return base->SSRS;
}

/*!
 * @brief Clears the sticky reset source status.
 *
 * This function clears the sticky system reset flags indicated by source masks.
 *
 * Example:
   @code
   // Clears multiple reset sources.
   SMC_ClearStickyResetSources(SMC0, (kSMC_SourceWdog | kSMC_SourcePin));
   @endcode
 *
 * @param base SMC peripheral base address.
 * @param sourceMasks reset source status bit map
 */
static inline void SMC_ClearStickyResetSources(SMC_Type *base, uint32_t sourceMasks)
{
    base->SSRS = sourceMasks;
}

/*!
 * @brief Configures the reset pin filter.
 *
 * This function sets the reset pin filter including the enablement/disablement and filter width.
 *
 * @param base SMC peripheral base address.
 * @param config Pointer to the configuration structure.
 */
void SMC_ConfigureResetPinFilter(SMC_Type *base, const smc_reset_pin_filter_config_t *config);

/*!
 * @brief Sets the system reset interrupt configuration.
 *
 * For a graceful shut down, the MSMC supports delaying the assertion of the system
 * reset for a period of time when the reset interrupt is generated. This function
 * can be used to enable the interrupt.
 * The interrupts are passed in as bit mask. See smc_interrupt_enable_t for details.
 * For example, to delay a reset after the WDOG timeout or PIN reset occurs, configure as follows:
 * SMC_SetSystemResetInterruptConfig(SMC0, (kSMC_IntWdog | kSMC_IntPin));
 *
 * @param base SMC peripheral base address.
 * @param intMask   Bit mask of the system reset interrupts to enable. See
 *                  smc_interrupt_enable_t for details.
 */
static inline void SMC_SetSystemResetInterruptConfig(SMC_Type *base, uint32_t intMask)
{
    base->SRIE = intMask;
}

/*!
 * @brief Gets the source status of the system reset interrupt.
 *
 * This function gets the source status of the reset interrupt. Use source masks
 * defined in the smc_interrupt_enable_t to get the desired source status.
 *
 * Example:
   @code
   uint32_t interruptStatus;

   // To get all reset interrupt source statuses.
   interruptStatus = SMC_GetResetInterruptSourcesStatus(SMC0) & kSMC_IntAll;

   // To test whether the reset interrupt of Watchdog is pending.
   interruptStatus = SMC_GetResetInterruptSourcesStatus(SMC0) & kSMC_IntWdog;

   // To test multiple reset interrupt sources.
   interruptStatus = SMC_GetResetInterruptSourcesStatus(SMC0) & (kSMC_IntWdog | kSMC_IntPin);
   @endcode
 *
 * @param base SMC peripheral base address.
 * @return All reset interrupt source status bit map.
 */
static inline uint32_t SMC_GetResetInterruptSourcesStatus(SMC_Type *base)
{
    return base->SRIF;
}

/*!
 * @brief Clears the source status of the system reset interrupt.
 *
 * This function clears the source status of the reset interrupt. Use source masks
 * defined in the smc_interrupt_enable_t to get the desired source status.
 *
 * Example:
   @code
   uint32_t interruptStatus;

   // To clear all reset interrupt source statuses.
   MMC_ClearResetInterruptSourcesStatus(SMC0, kSMC_IntAll);

   // To clear the reset interrupt of Watchdog.
   SMC_ClearResetInterruptSourcesStatus(SMC0, kSMC_IntWdog);

   // To clear multiple reset interrupt sources status.
   SMC_ClearResetInterruptSourcesStatus(SMC0, (kSMC_IntWdog | kSMC_IntPin));
   @endcode
 *
 * @param base SMC peripheral base address.
 * @param All reset interrupt source status bit map to clear.
 */
static inline void SMC_ClearResetInterruptSourcesStatus(SMC_Type *base, uint32_t intMask)
{
    base->SRIF = intMask;
}

/*!
 * @brief Sets the core software reset feature configuration.
 *
 * The MSMC supports delaying the assertion of the system reset for a period of time while a core
 * software reset is generated. This allows software to recover without reseting the entire system.
 * This function can be used to enable/disable the core software reset feature.
 * The interrupts are passed in as bit mask. See smc_interrupt_enable_t for details.
 * For example, to delay a system after the WDOG timeout or PIN core software reset occurs, configure as follows:
 * SMC_SetCoreSoftwareResetConfig(SMC0, (kSMC_IntWdog | kSMC_IntPin));
 *
 * @param base SMC peripheral base address.
 * @param intMask   Bit mask of the core software reset to enable. See
 *                  smc_interrupt_enable_t for details.
 */
static inline void SMC_SetCoreSoftwareResetConfig(SMC_Type *base, uint32_t intMask)
{
    base->CSRE = intMask;
}

/*!
 * @brief Gets the boot option configuration.
 *
 * This function gets the boot option configuration of MSMC.
 *
 * @param base SMC peripheral base address.
 * @return The boot option configuration. 1 means boot option enabled. 0 means not.
 */
static inline uint32_t SMC_GetBootOptionConfig(SMC_Type *base)
{
    return base->MR;
}
/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* _FSL_MSMC_H_ */
