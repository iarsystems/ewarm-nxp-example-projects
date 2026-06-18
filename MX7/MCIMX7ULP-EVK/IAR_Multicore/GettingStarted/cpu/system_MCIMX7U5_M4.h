/*
** ###################################################################
**     Processors:          MCIMX7U5CVM05
**                          MCIMX7U5DVK08
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    i.MX7ULPRM, Rev. D, Nov. 2016
**     Version:             rev. 2.0.1, 2017-02
**     Build:               b160722
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright (c) 2016 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2016-04-13)
**         Initial version.
**     - rev. 2.0 (2016-07-19)
**         RevC Header ER
**     - rev. 2.0.1 (2017-02)
**         Edited by Atanas Uzunov @ IAR Systems. Added basic clock & DDR init.
** ###################################################################
*/

/*!
 * @file MCIMX7U5_M4
 * @version 2.0.1
 * @date 2016-07-19
 * @brief Device specific configuration file for MCIMX7U5_M4 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef _SYSTEM_MCIMX7U5_M4_H_
#define _SYSTEM_MCIMX7U5_M4_H_                   /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/* i.MX7ULP Definitions */
#ifndef DISABLE_WDOG
  #define DISABLE_WDOG      1
#endif

/* Reset and hold Core B at startup */
#ifndef RESET_CORE_B
  #define RESET_CORE_B      1
#endif

/* Define clock source values */
#define CPU_XTAL_SOSC_CLK_HZ           24000000u           /* Value of the external System Oscillator Clock(SOSC) frequency in Hz */
#define CPU_XTAL_LVDS_CLK_HZ           24000000u           /* Value of the external LVDS Clock frequency in Hz */
#define DEFAULT_SYSTEM_CLOCK           0                   /* Default System clock value (not set) */

#define CPU_FIRC_CLK_HZ                48000000u

#define SCG_SPLL_CLK_HZ                480000000u          /* Value of System PLL frequency in Hz (fixed) */

#define SCG_SPLLDIV_SPLLDIV1_VAL       0
#define SCG_SPLLDIV_SPLLDIV2_VAL       3
#define SCG_SPLLDIV_SPLLDIV3_VAL       0

#define SCG_SPLLCFG_PLLS_VAL           1

#define SCG_SPLLPFD_PFD0_VAL           30

#define SCG_RCCR_SCS_VAL               6                   /* Select SPLL as system clock */
#define SCG_RCCR_DIVCORE_VAL           2                   /* Core clock divide-by-3 */
#define SCG_RCCR_DIVBUS_VAL            5                   /* Core clock divide-by-6 */
#define SCG_RCCR_DIVSLOW_VAL           11                  /* Core clock divide-by-12 */

#define SCG_SPLL_PFD0_CLK_HZ           (uint32_t)((double)SCG_SPLL_CLK_HZ*18.0/(double)SCG_SPLLPFD_PFD0_VAL)

#if (SCG_SPLLCFG_PLLS_VAL == 1)
#define SCG_SPLL_OUT_CLK_HZ            SCG_SPLL_PFD0_CLK_HZ
#else
#define SCG_SPLL_OUT_CLK_HZ            SCG_SPLL_CLK_HZ
#endif

#if (SCG_SPLLDIV_SPLLDIV2_VAL > 0)
#define SCG_SPLL_BUS_CLK_HZ            (uint32_t)((double)(SCG_SPLL_OUT_CLK_HZ)/(double)(1<<(SCG_SPLLDIV_SPLLDIV2_VAL-1)))
#endif

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif  /* _SYSTEM_MCIMX7U5_M4_H_ */
