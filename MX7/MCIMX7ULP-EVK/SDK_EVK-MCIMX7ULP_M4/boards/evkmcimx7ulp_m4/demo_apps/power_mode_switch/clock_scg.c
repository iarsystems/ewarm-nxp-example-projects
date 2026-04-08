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

#include "fsl_common.h"
#include "clock_config.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * In SCG based example, SCG clock sources are configured as:
 *
 * SCG_SOSC: 24MHz.
 * SCG_SIRC: 16MHz.
 * SCG_FIRC: 48MHz.
 * SCG_SPLL: 480MHz.
 * SCG_APLL: disabled.
 *
 * VLPR:
 *  - Clock source: SIRC
 * RUN:
 *  - Clock source: SPLL PFD0
 * HSRUN:
 *  - Clock source: SPLL PFD0
 */

/*
 * SYSOSC clock ROM setting.
 * SYSOSC clock         : 24MHz
 * SOSCDIV1_CLK output  : 0MHz (async platform clock)
 * SOSCDIV2_CLK output  : 0MHz (async bus clock)
 * SOSCDIV3_CLK output  : 0MHz (async slow clock)
 */
const scg_sosc_config_t s_scgSysOscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
                                             .enableMode = kSCG_SysOscEnable,
                                             .monitorMode = kSCG_SysOscMonitorDisable,
                                             .div1 = kSCG_AsyncClkDisable,
                                             .div2 = kSCG_AsyncClkDisable,
                                             .div3 = kSCG_AsyncClkDisable,
                                             .workMode = kSCG_SysOscModeOscLowPower};

/*
 * SIRC clock ROM setting.
 * SIRC clock           : 16MHz
 * SIRCDIV1_CLK output  : 0MHz  (async platform clock)
 * SIRCDIV2_CLK output  : 0MHz  (async bus clock)
 * SIRCDIV3_CLK output  : 0MHz  (async slow clock)
 */
const scg_sirc_config_t s_scgSircConfig = {.enableMode = kSCG_SircEnable | kSCG_SircEnableInLowPower,
                                           .div1 = kSCG_AsyncClkDisable,
                                           .div2 = kSCG_AsyncClkDisable,
                                           .div3 = kSCG_AsyncClkDisable,
                                           .range = kSCG_SircRangeHigh};

/*
 * FIRC clock ROM setting.
 * FIRC clock           : 48MHz
 * FIRCDIV1_CLK output  : 0MHz (async platform clock)
 * FIRCDIV2_CLK output  : 0MHz (async bus clock)
 * FIRCDIV3_CLK output  : 0MHz (async slow clock)
 */
const scg_firc_config_t s_scgFircConfig = {.enableMode = kSCG_FircEnable,
                                           .div1 = kSCG_AsyncClkDisable,
                                           .div2 = kSCG_AsyncClkDisable,
                                           .div3 = kSCG_AsyncClkDisable,
                                           .range = kSCG_FircRange48M,
                                           .trimConfig = NULL};

/*
 * SYSPLL clock setting.
 * SYSPLL clock           : 480MHz
 * SYSPLLDIV1_CLK output  : 480MHz (async platform clock)
 * SYSPLLDIV2_CLK output  : 240MHz (async bus clock)
 * SYSPLLDIV3_CLK output  : 0MHz   (async slow clock)
 * PFD0 selected for sync system clock
 */
const scg_spll_config_t s_scgSysPllConfig = {.enableMode = kSCG_SysPllEnable,
                                             .div1 = kSCG_AsyncClkDivBy1,
                                             .div2 = kSCG_AsyncClkDivBy2,
                                             .div3 = kSCG_AsyncClkDisable,
                                             .src = kSCG_SysPllSrcSysOsc,
                                             .isBypassSelected = false,
                                             .isPfdSelected = true,
                                             .prediv = 0U,
                                             .pfdClkout = kSCG_SysPllPfd0Clk,
                                             .mult = 3U}; /* x20 */

/*
 * AUXPLL clock setting.
 * AUXPLL clock           : 540.672MHz / 88 = 6.144MHz
 * AUXPLLDIV1_CLK output  : 6.144MHz (async platform clock)
 * AUXPLLDIV2_CLK output  : 6.144MHz (async bus clock)
 * AUXPLLDIV3_CLK output  : 0MHz     (async slow clock)
 */
const scg_apll_config_t s_scgAuxPllConfig = {.enableMode = kSCG_AuxPllEnable,
                                             .div1 = kSCG_AsyncClkDivBy1,
                                             .div2 = kSCG_AsyncClkDivBy1,
                                             .div3 = kSCG_AsyncClkDisable,
                                             .src = kSCG_AuxPllSrcSysOsc,
                                             .isBypassSelected = false,
                                             .isPfdSelected = false,
                                             .prediv = 0U,
                                             .pfdClkout = kSCG_AuxPllPfd0Clk,
                                             .mult = 22U,
                                             .pllPostdiv1 = kSCG_SysClkDivBy11,
                                             .pllPostdiv2 = kSCG_SysClkDivBy8,
                                             .num = 528000U,
                                             .denom = 1000000U}; /* 24 x 22.528 = 540.672MHz */

/*
 * System clock source and divider in VLPR mode.
 * Clock source   : SIRC
 * Core clock     : 16MHz
 * Platform clock : 16MHz
 * Bus clock      : 16MHz
 * Slow clock     : 8MHz
 */
const scg_sys_clk_config_t s_sysClkConfigVlpr = {
    .divCore = kSCG_SysClkDivBy1, /* Core clock divider. */
    .divPlat = kSCG_SysClkDivBy1, /* Platform clock divider. */
    .divBus = kSCG_SysClkDivBy1,  /* Bus clock divider. */
    .divSlow = kSCG_SysClkDivBy2, /* Slow clock divider. */
    .src = kSCG_SysClkSrcSirc     /* System clock source. */
};

/*
 * System clock source and divider in Normal RUN mode.
 * Clock source   : SYSPLL PFD0 392MHz
 * Core clock     : 98MHz
 * Platform clock : 98MHz
 * Bus clock      : 49MHz
 * Slow clock     : 24.5MHz
 */
const scg_sys_clk_config_t s_sysClkConfigNormalRun = {
    .divCore = kSCG_SysClkDivBy4, /* Core clock divider. */
    .divPlat = kSCG_SysClkDivBy1, /* Platform clock divider. */
    .divBus = kSCG_SysClkDivBy2,  /* Bus clock divider. */
    .divSlow = kSCG_SysClkDivBy4, /* Slow clock divider. */
    .src = kSCG_SysClkSrcSysPll   /* System clock source. */
};

/*
 * System clock source and divider in HSRUN mode.
 * Clock source   : SYSPLL PFD0 392MHz
 * Core clock     : 196MHz
 * Platform clock : 196MHz
 * Bus clock      : 98MHz
 * Slow clock     : 49MHz
 */
const scg_sys_clk_config_t s_sysClkConfigHsrun = {
    .divCore = kSCG_SysClkDivBy2, /* Core clock divider. */
    .divPlat = kSCG_SysClkDivBy1, /* Platform clock divider. */
    .divBus = kSCG_SysClkDivBy2,  /* Bus clock divider. */
    .divSlow = kSCG_SysClkDivBy4, /* Slow clock divider. */
    .src = kSCG_SysClkSrcSysPll   /* System clock source. */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_InitClock(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
extern uint16_t BOARD_SetRunModeCode[];

typedef void (*APP_SetRunModeFunc)(
    SCG_Type *scg, uint32_t scgRunConfig, QuadSPI_Type *qspi, clock_ip_name_t qspiClock, uint32_t qspiClockConfig);

static APP_SetRunModeFunc APP_SetRunMode;

static bool APP_IsRunOnQSPI(void)
{
    if (((uint32_t)APP_InitClock >= 0x04000000U) && ((uint32_t)APP_InitClock < 0x0C000000U))
    {
        return true;
    }
    return false;
}

static void InitClock(void)
{
    /* First initialize same clock settings as ROM does. It's necessary for debugging
       in case ROM doesn't initialize well. */
    if (!CLOCK_IsSircValid())
    {
        CLOCK_InitSirc(&s_scgSircConfig);
    }

    if (!CLOCK_IsFircValid())
    {
        CLOCK_InitFirc(&s_scgFircConfig);
    }

    if (!CLOCK_IsSysOscValid())
    {
        CLOCK_InitSysOsc(&s_scgSysOscConfig);
    }

    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
    SCG0->SOSCCSR |= SCG_SOSCCSR_SOSCLPEN_MASK | SCG_SOSCCSR_SOSCSTEN_MASK;
    SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCSTEN_MASK;
    SCG0->SIRCCSR |= SCG_SIRCCSR_SIRCSTEN_MASK;

    /* Then set SOSC, FIRC and SIRC DIV needed by application */
    CLOCK_SetSysOscAsyncClkDiv(kSCG_AsyncDiv1Clk, kSCG_AsyncClkDivBy1);
    CLOCK_SetSysOscAsyncClkDiv(kSCG_AsyncDiv2Clk, kSCG_AsyncClkDivBy1);

    CLOCK_SetFircAsyncClkDiv(kSCG_AsyncDiv1Clk, kSCG_AsyncClkDivBy1);
    CLOCK_SetFircAsyncClkDiv(kSCG_AsyncDiv2Clk, kSCG_AsyncClkDivBy1);

    CLOCK_SetSircAsyncClkDiv(kSCG_AsyncDiv1Clk, kSCG_AsyncClkDivBy1);
    CLOCK_SetSircAsyncClkDiv(kSCG_AsyncDiv2Clk, kSCG_AsyncClkDivBy1);
}

/*
 * Initialize SCG setting after system boot up.
 */
void APP_InitClock(void)
{
    APP_SetRunMode = (APP_SetRunModeFunc)(((uint32_t)BOARD_SetRunModeCode) | 1U); /* Thumb code */
    QuadSPI_Type *qspi = APP_IsRunOnQSPI() ? QuadSPI0 : NULL;

    InitClock();

    /* Change to use SIRC as system clock source to prepare to initialize other clock sources */
    APP_SetRunMode(SCG0, *((uint32_t *)(&s_sysClkConfigVlpr)), qspi, kCLOCK_Qspi,
                   PCC1_PCC_QSPI_OTFAD_CGC_MASK | PCC1_PCC_QSPI_OTFAD_PCS(2)); /* QSPI source: 16M SIRC Async */

    /* Initialize SysPll 480MHz */
    CLOCK_InitSysPll(&s_scgSysPllConfig);

    /* Enable SysPll Pfd0 392MHz */
    CLOCK_EnableSysPllPfdClkout(kSCG_SysPllPfd0Clk, 22U);
    /* Enable SysPll Pfd3 480MHz (same as SysPll main clock) */
    CLOCK_EnableSysPllPfdClkout(kSCG_SysPllPfd3Clk, 18U);

    /* Initialize AuxPll */
    CLOCK_InitAuxPll(&s_scgAuxPllConfig);
    /* Enable AuxPll Pfd0 540.672MHz (same as AuxPll before postdiv) */
    CLOCK_EnableAuxPllPfdClkout(kSCG_AuxPllPfd0Clk, 18U);

    /* Change to use SysPll Pfd0 as system clock source */
    APP_SetRunMode(SCG0, *((uint32_t *)(&s_sysClkConfigNormalRun)), qspi, kCLOCK_Qspi,
                   PCC1_PCC_QSPI_OTFAD_CGC_MASK | PCC1_PCC_QSPI_OTFAD_PCS(3)); /* QSPI source: 48M FIRC Async */

    CLOCK_SetVlprModeSysClkConfig(&s_sysClkConfigVlpr);
    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigHsrun);
}

/*
 * Set the clock configuration for HSRUN mode.
 */
void APP_SetClockHsrun(void)
{
}

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
void APP_SetClockRunFromHsrun(void)
{
}

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void)
{
    QuadSPI_Type *qspi = APP_IsRunOnQSPI() ? QuadSPI0 : NULL;

    /* Recover SPLL */
    SCG0->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;
    while (!CLOCK_IsSysPllValid())
    {
    }

    /* When switching from VLPR to RUN, switch RUN clock source back to RUN source */
    APP_SetRunMode(SCG0, *((uint32_t *)(&s_sysClkConfigNormalRun)), qspi, kCLOCK_Qspi,
                   PCC1_PCC_QSPI_OTFAD_CGC_MASK | PCC1_PCC_QSPI_OTFAD_PCS(3)); /* QSPI source: 48M FIRC Async */
}

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void)
{
    QuadSPI_Type *qspi = APP_IsRunOnQSPI() ? QuadSPI0 : NULL;

    /* When switching from RUN to VLPR, first switch RUN clock source to VLPR source */
    APP_SetRunMode(SCG0, *((uint32_t *)(&s_sysClkConfigVlpr)), qspi, kCLOCK_Qspi,
                   PCC1_PCC_QSPI_OTFAD_CGC_MASK | PCC1_PCC_QSPI_OTFAD_PCS(2)); /* QSPI source: 16M SIRC Async */

    /* Disable SPLL to work around hardware issue */
    SCG0->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK;
}
