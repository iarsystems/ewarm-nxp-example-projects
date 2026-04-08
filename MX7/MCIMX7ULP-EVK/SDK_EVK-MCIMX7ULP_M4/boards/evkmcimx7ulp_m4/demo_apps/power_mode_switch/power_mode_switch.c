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

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "fsl_gpio.h"
#include "fsl_llwu.h"
#include "fsl_lptmr.h"
#include "fsl_msmc.h"
#include "fsl_mu.h"
#include "fsl_pf1550_serv.h"
#include "fsl_port.h"
#include "fsl_pmc0.h"
#include "fsl_debug_console.h"
#include "power_mode_switch.h"

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
#define LLWU_LPTMR_IDX (1U)       /* LLWU_M1IF */
#define LLWU_WAKEUP_PIN_IDX (13U) /* LLWU_P13 */
#define LLWU_WAKEUP_PIN_TYPE kLLWU_ExternalPinFallingEdge

#define APP_WAKEUP_BUTTON_GPIO GPIOB
#define APP_WAKEUP_BUTTON_PORT PORTB
#define APP_WAKEUP_BUTTON_GPIO_PIN (14U)
#define APP_WAKEUP_BUTTON_IRQ PCTLB_IRQn
#define APP_WAKEUP_BUTTON_IRQ_HANDLER PCTLB_IRQHandler
#define APP_WAKEUP_BUTTON_NAME "VOL+"
#define APP_WAKEUP_BUTTON_IRQ_TYPE kPORT_InterruptFallingEdge

typedef enum _app_wakeup_source {
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/* Set the clock configuration for HSRUN mode. */
extern void APP_SetClockHsrun(void);
/* Set the clock configuration for RUN mode from HSRUN mode. */
extern void APP_SetClockRunFromHsrun(void);
/* Set the clock configuration for RUN mode from VLPR mode. */
extern void APP_SetClockRunFromVlpr(void);
/* Set the clock configuration for VLPR mode. */
extern void APP_SetClockVlpr(void);

extern void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
extern void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern pf1550_serv_handle_t pf1550Handle;
extern TimerHandle_t suspendTimer;
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

/*******************************************************************************
 * Function Code
 ******************************************************************************/

/* LLWU interrupt handler. */
void LLWU0_IRQHandler(void)
{
    /* If wakeup by LPTMR. */
    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_LPTMR_IDX))
    {
        LPTMR_DisableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR1);
    }
    /* If wakeup by external pin. */
    if (LLWU_GetExternalWakeupPinFlag(LLWU, LLWU_WAKEUP_PIN_IDX))
    {
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT, (1U << APP_WAKEUP_BUTTON_GPIO_PIN));
        LLWU_ClearExternalWakeupPinFlag(LLWU, LLWU_WAKEUP_PIN_IDX);
    }
}

/* LPTMR1 interrupt handler. */
void LPTMR1_IRQHandler(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR1))
    {
        LPTMR_DisableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR1);
    }
}

static void APP_ShowPowerMode(smc_power_state_t powerMode)
{
    switch (powerMode)
    {
        case kSMC_PowerStateRun:
            PRINTF("    Power mode: RUN\r\n");
            break;
        case kSMC_PowerStateVlpr:
            PRINTF("    Power mode: VLPR\r\n");
            break;
        case kSMC_PowerStateHsrun:
            PRINTF("    Power mode: HSRUN\r\n");
            break;
        default:
            PRINTF("    Power mode wrong\r\n");
            break;
    }
}

/*
 * Check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
static bool APP_CheckPowerMode(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    bool modeValid = true;
    mu_power_mode_t powerMode;

    /*
     * Check wether the mode change is allowed.
     *
     * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
     * 2. If current mode is RUN mode, the target mode must not be VLPW mode.
     * 3. If current mode is VLPR mode, the target mode must not be HSRUN/WAIT/STOP mode.
     * 4. If already in the target mode.
     */
    switch (curPowerState)
    {
        case kSMC_PowerStateHsrun:
            if (kAPP_PowerModeRun != targetPowerMode)
            {
                PRINTF("Current mode is HSRUN, please choose RUN mode as target mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateRun:
            if (kAPP_PowerModeVlpw == targetPowerMode)
            {
                PRINTF("Could not enter VLPW mode from RUN mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kAPP_PowerModeWait == targetPowerMode) || (kAPP_PowerModeHsrun == targetPowerMode) ||
                (kAPP_PowerModeStop == targetPowerMode))
            {
                PRINTF("Could not enter HSRUN/STOP/WAIT modes from VLPR mode.\r\n");
                modeValid = false;
            }
            break;

        default:
            PRINTF("Wrong power state.\r\n");
            modeValid = false;
            break;
    }

    powerMode = MU_GetOtherCorePowerMode(MUA);
    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlpr:
            if ((powerMode != kMU_PowerModeStop) && (powerMode != kMU_PowerModeDsm))
            {
                PRINTF("M4 can enter VLPR Mode only when A7 in STOP/VLPS Mode or VLLS Mode!!!\r\n");
                modeValid = false;
            }
            break;

        case kAPP_PowerModeWait:
            if ((powerMode != kMU_PowerModeStop) && (powerMode != kMU_PowerModeDsm))
            {
                PRINTF("M4 can enter WAIT Mode only when A7 in STOP/VLPS Mode or VLLS Mode!!!\r\n");
                modeValid = false;
            }
            break;

        case kAPP_PowerModeStop:
            if ((powerMode != kMU_PowerModeStop) && (powerMode != kMU_PowerModeDsm))
            {
                PRINTF("M4 can enter STOP Mode only when A7 in STOP/VLPS Mode or VLLS Mode!!!\r\n");
                modeValid = false;
            }
            break;

        case kAPP_PowerModeVlpw:
            if ((powerMode != kMU_PowerModeStop) && (powerMode != kMU_PowerModeDsm))
            {
                PRINTF("M4 can enter VLPW Mode only when A7 in STOP/VLPS Mode or VLLS Mode!!!\r\n");
                modeValid = false;
            }
            break;

        case kAPP_PowerModeVlps:
            if ((powerMode != kMU_PowerModeStop) && (powerMode != kMU_PowerModeDsm))
            {
                PRINTF("M4 can enter VLPS Mode only when A7 in STOP/VLPS Mode or VLLS Mode!!!\r\n");
                modeValid = false;
            }
            break;

        case kAPP_PowerModeVlls:
            if (powerMode != kMU_PowerModeDsm)
            {
                PRINTF("M4 can enter VLLS Mode only when A7 in VLLS Mode!!!\r\n");
                modeValid = false;
            }
            break;

        case kAPP_PowerModeRun:
        case kAPP_PowerModeHsrun:
        default:
            break;
    }

    if (!modeValid)
    {
        return false;
    }

    /* Don't need to change power mode if current mode is already the target mode. */
    if (((kAPP_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == curPowerState)) ||
        ((kAPP_PowerModeHsrun == targetPowerMode) && (kSMC_PowerStateHsrun == curPowerState)) ||
        ((kAPP_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == curPowerState)))
    {
        PRINTF("Already in the target power mode.\r\n");
        return false;
    }

    return true;
}

/* Get input from user about wakeup timeout. */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(void)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for LPTMR - Low Power Timer\r\n");
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceLptmr;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(void)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource();

    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        /* Wakeup source is LPTMR, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Press %s to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

static void APP_SetWakeupConfig(app_power_mode_t targetMode)
{
    /* Set the wakeup module. */
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_SetTimerPeriod(LPTMR1, (1000U * s_wakeupTimeout) - 1U);
        LPTMR_StartTimer(LPTMR1);
        LPTMR_EnableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
    }

    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
        (kAPP_PowerModeVlps != targetMode) && (kAPP_PowerModeStop != targetMode))
    {
        if (kAPP_WakeupSourceLptmr == s_wakeupSource)
        {
            LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_LPTMR_IDX, true);
        }
        else
        {
            LLWU_SetExternalWakeupPinMode(LLWU, LLWU_WAKEUP_PIN_IDX, LLWU_WAKEUP_PIN_TYPE);
        }
        NVIC_EnableIRQ(LLWU0_IRQn);
    }
}

static void APP_PowerModeSwitch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    status_t status = kStatus_Success;

    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlpr:
            APP_SetClockVlpr();
            status = SMC_SetPowerModeVlpr(MSMC0);
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(MSMC0))
            {
            }
            break;

        case kAPP_PowerModeRun:
            /* If enter RUN from HSRUN, fisrt change clock. */
            if (kSMC_PowerStateHsrun == curPowerState)
            {
                APP_SetClockRunFromHsrun();
            }

            /* Power mode change. */
            status = SMC_SetPowerModeRun(MSMC0);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(MSMC0))
            {
            }

            /* If enter RUN from VLPR, change clock after the power mode change. */
            if (kSMC_PowerStateVlpr == curPowerState)
            {
                APP_SetClockRunFromVlpr();
            }
            break;

        case kAPP_PowerModeHsrun:
            status = SMC_SetPowerModeHsrun(MSMC0);
            while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(MSMC0))
            {
            }

            APP_SetClockHsrun(); /* Change clock setting after power mode change. */
            break;

        case kAPP_PowerModeWait:
            status = SMC_SetPowerModeWait(MSMC0);
            break;

        case kAPP_PowerModeStop:
            /* Need to switch RUN clock away from PLL and disable PLL */
            APP_SetClockVlpr();
            status = SMC_SetPowerModeStop(MSMC0, kSMC_PartialStop);
            /* Recover RUN clock to PLL */
            APP_SetClockRunFromVlpr();
            break;

        case kAPP_PowerModeVlpw:
            status = SMC_SetPowerModeVlpw(MSMC0);
            break;

        case kAPP_PowerModeVlps:
            status = SMC_SetPowerModeVlps(MSMC0);
            break;

#if 0
        case kAPP_PowerModeLls:
            status = SMC_SetPowerModeLls(MSMC0);
            break;
#endif

        case kAPP_PowerModeVlls:
            if (kSMC_PowerStateRun == curPowerState)
            { /* Need to switch RUN clock away from PLL and disable PLL */
                APP_SetClockVlpr();
            }
            status = SMC_SetPowerModeVlls(MSMC0);
            if (kSMC_PowerStateRun == curPowerState)
            {
                /* Recover RUN clock to PLL */
                APP_SetClockRunFromVlpr();
            }
            break;

        default:
            PRINTF("Wrong value\r\n");
            break;
    }

    if (status != kStatus_Success)
    {
        PRINTF("!!!! Power switch failed !!!!!\r\n");
    }
}

static uint32_t APP_GetInputNumWithEcho(uint32_t length, bool allowZero)
{
    uint8_t ch;
    uint8_t digBuffer[8U] = {0U};
    uint8_t i, j;
    uint8_t digCount = 0U;
    uint32_t temp1, temp2 = 0U;
    uint32_t result = 0U;
    bool getFirstDig = false;

    assert(length <= (sizeof(digBuffer) / sizeof(digBuffer[0U])));

    /* Get user input and echo it back to terminal. */
    for (;;)
    {
        ch = GETCHAR();

        if (('a' <= ch) && ('f' >= ch))
        {
            ch = ch - ('a' - 'A');
        }

        if (((('0' <= ch) && ('9' >= ch)) || (('A' <= ch) && ('F' >= ch))) && (digCount < length))
        {
            if (false == getFirstDig)
            {
                if (allowZero || ('0' < ch))
                {
                    getFirstDig = true;
                }
                else
                {
                    continue;
                }
            }

            PUTCHAR(ch);
            digBuffer[digCount] = ch;
            digCount++;
        }
        else if ((0x7FU == ch) && (0U != digCount))
        {
            digBuffer[digCount] = 0x0U;
            digCount--;
            PUTCHAR(0x7FU);
        }
        else
        {
            if ('\r' == ch)
            {
                break;
            }
        }
    }

    /* Reconstruct user input number. */
    temp1 = digCount - 1;
    for (i = 0; i < digCount; i++)
    {
        if (('0' <= digBuffer[i]) && ('9' >= digBuffer[i]))
        {
            temp2 = digBuffer[i] - '0';
        }
        else if (('A' <= digBuffer[i]) && ('F' >= digBuffer[i]))
        {
            temp2 = digBuffer[i] - 'A' + 10U;
        }
        else
        {
        }

        for (j = 0U; j < temp1; j++)
        {
            temp2 *= 16U;
        }
        temp1--;
        result += temp2;
    }

    return result;
}

static void APP_ReadPmicRegister(void)
{
    uint32_t reg;
    uint8_t value;

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("Please select the register address to dump(0~FF):");
        reg = APP_GetInputNumWithEcho(2U, true);
        if (0xFFU >= reg)
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Read register from PF1550 */
    PF1550_SERV_DumpReg(&pf1550Handle, reg, &value, 1U);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    PRINTF("[0x%x] = 0x%x\r\n", reg, value);
    PRINTF("\r\n");
    PRINTF("Press Any Key to Home Page...");
    GETCHAR();
}

static void APP_SetPmicRegister(void)
{
    uint32_t reg;
    uint32_t value;
    uint8_t temp;

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("Please input the register address to set(0~FF):");
        reg = APP_GetInputNumWithEcho(2U, true);
        if (255U >= reg)
        {
            break;
        }
    }
    PRINTF("\r\n");

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("Please input the register content to set(0~FF):");
        value = APP_GetInputNumWithEcho(2U, true);
        if (0xFFU >= value)
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Set register content to PF1550 */
    PF1550_SERV_WriteReg(&pf1550Handle, reg, value);

    /* Read register from PF1550 */
    PF1550_SERV_DumpReg(&pf1550Handle, reg, &temp, 1U);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    PRINTF("[0x%x] = 0x%x\r\n", reg, temp);
    PRINTF("\r\n");
    PRINTF("Press Any Key to Home Page...");
    GETCHAR();
}

/* Power Mode Switch task */
void POWER_MODE_SWITCH_Task(void *pvParameters)
{
    lptmr_config_t lptmrConfig;
    smc_power_state_t curPowerState;
    mu_power_mode_t powerMode;
    app_power_mode_t targetPowerMode;
    uint32_t resetSrc;
    uint32_t freq = 0U;
    uint8_t ch;
    bool needSetWakeup; /* Need to set wakeup. */

    /* Setup LPTMR. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1; /* Use LPO 1KHz as clock source. */
    lptmrConfig.bypassPrescaler = true;
    LPTMR_Init(LPTMR1, &lptmrConfig);

    /* Setup MUA. */
    MU_Init(MUA);

    EnableIRQ(LLWU0_IRQn);
    EnableIRQ(LPTMR1_IRQn);

    resetSrc = SMC_GetPreviousResetSources(MSMC0);
    PRINTF("\r\nMCU wakeup source 0x%x...\r\n", resetSrc);

    for (;;)
    {
        curPowerState = SMC_GetPowerModeState(MSMC0);

        freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
        PRINTF("\r\n####################  Power Mode Switch Task ####################\n\r\n");
        PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        PRINTF("    Core Clock: %dHz \r\n", freq);
        APP_ShowPowerMode(curPowerState);
        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: RUN      - Normal RUN mode\r\n", kAPP_PowerModeRun);
        PRINTF("Press  %c for enter: WAIT     - Wait mode\r\n", kAPP_PowerModeWait);
        PRINTF("Press  %c for enter: STOP     - Stop mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c for enter: VLPR     - Very Low Power Run mode\r\n", kAPP_PowerModeVlpr);
        PRINTF("Press  %c for enter: VLPW     - Very Low Power Wait mode\r\n", kAPP_PowerModeVlpw);
        PRINTF("Press  %c for enter: VLPS     - Very Low Power Stop mode\r\n", kAPP_PowerModeVlps);
        PRINTF("Press  %c for enter: HSRUN    - High Speed RUN mode\r\n", kAPP_PowerModeHsrun);
        /* PRINTF("Press  %c for enter: LLS      - Low Leakage Stop mode\r\n", kAPP_PowerModeLls); */
        PRINTF("Press  %c for enter: VLLS     - Very Low Leakage Stop mode\r\n", kAPP_PowerModeVlls);
        PRINTF("Press  Q for query CA7 core power status.\r\n");
        PRINTF("Press  W for wake up CA7 core in VLLS.\r\n");
        PRINTF("Press  R for read PF1550 Register.\r\n");
        PRINTF("Press  S for set PF1550 Register.\r\n");
        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        targetPowerMode = (app_power_mode_t)ch;
        if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
        {
            if (!APP_CheckPowerMode(curPowerState, targetPowerMode))
            {
                continue;
            }

            /* If target mode is RUN/VLPR/HSRUN, don't need to set wakeup source. */
            if ((kAPP_PowerModeRun == targetPowerMode) || (kAPP_PowerModeHsrun == targetPowerMode) ||
                (kAPP_PowerModeVlpr == targetPowerMode))
            {
                needSetWakeup = false;
            }
            else
            {
                needSetWakeup = true;
                APP_GetWakeupConfig();
            }

            APP_PowerPreSwitchHook(curPowerState, targetPowerMode);

            if (needSetWakeup)
            {
                APP_SetWakeupConfig(targetPowerMode);
            }

            APP_PowerModeSwitch(curPowerState, targetPowerMode);
            APP_PowerPostSwitchHook(curPowerState, targetPowerMode);
        }
        else if ('Q' == ch)
        {
            powerMode = MU_GetOtherCorePowerMode(MUA);
            switch (powerMode)
            {
                case kMU_PowerModeRun:
                    PRINTF("CA7 power mode RUN!\r\n");
                    break;

                case kMU_PowerModeWait:
                    PRINTF("CA7 power mode WAIT!\r\n");
                    break;

                case kMU_PowerModeStop:
                    PRINTF("CA7 power mode STOP/VLPS!\r\n");
                    break;

                case kMU_PowerModeDsm:
                    PRINTF("CA7 power mode VLLS!\r\n");
                    break;

                default:
                    PRINTF("Wrong power mode value %d!\r\n", (int32_t)powerMode);
                    break;
            }
        }
        else if ('W' == ch)
        {
            xTimerStop(suspendTimer, portMAX_DELAY);

            /* Enable LPDDR3 Power. */
            GPIO_WritePinOutput(GPIOB, 6U, 0U);
            vTaskDelay(1U);

            /* Enable A7 Core Power. */
            PF1550_SERV_EnableRegulator(&pf1550Handle,
                       kPF1550_ModuleSwitch1,
                       kPF1550_OperatingStatusRun,
                       true);
            vTaskDelay(100U);

            /* We use PMIC instead of LDO for A7 */
            PMC0_EnablePmc1LdoRegulator(false);
            PMC0_PowerOnPmc1();
            MU_BootOtherCore(MUA, kMU_CoreBootFromAddr0);

            MU_TriggerInterrupts(MUA, kMU_NmiInterruptTrigger);
        }
        else if ('R' == ch)
        {
            APP_ReadPmicRegister();
        }
        else if ('S' == ch)
        {
            APP_SetPmicRegister();
        }
        else
        {
        }

        PRINTF("\r\nNext loop\r\n");
    }
}
