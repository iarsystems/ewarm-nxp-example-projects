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
#include "rpmsg_lite.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c_freertos.h"
#include "fsl_gpio.h"
#include "fsl_msmc.h"
#include "fsl_mu.h"
#include "fsl_pf1550_serv.h"
#include "fsl_pmc0.h"
#include "fsl_srtm.h"
#include "fsl_srtm_rpmsg.h"
#include "board.h"
#include "lifecycle_serv.h"
#include "pf1550_remote_serv.h"
#include "audio_serv.h"
#include "keypad_serv.h"
#include "pingpong.h"
#include "power_mode_switch.h"

#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
#include "fsl_lptmr.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_DEBUG_UART_BAUDRATE (115200U)                   /* Debug console baud rate. */
#define APP_DEBUG_UART_DEFAULT_CLKSRC kCLOCK_IpSrcSircAsync /* SCG SIRC clock. */

#define APP_WAKEUP_BUTTON_CLOCK kCLOCK_PctlB

#define APP_PF1550_LPI2C_BASE LPI2C3
#define APP_PF1550_LPI2C_IRQ LPI2C3_IRQn
#define APP_PF1550_LPI2C_PRIO (5U)
#define APP_PF1550_LPI2C_CLKSRC_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define APP_PF1550_LPI2C_BAUDRATE (100000U)
#define APP_PF1550_LPI2C_SCL_GPIO GPIOB
#define APP_PF1550_LPI2C_SCL_PIN (12U)
#define APP_PF1550_LPI2C_SDA_GPIO GPIOB
#define APP_PF1550_LPI2C_SDA_PIN (13U)
#define APP_PF1550_LPI2C_DELAY (100U)
#define APP_PF1550_SERV_RPMSG_SHMEM_BASE (0x9FF10000U)
#define APP_PF1550_SERV_RPMSG_LINK_ID (RL_PLATFORM_IMX7ULP_M4_PMIC_LINK_ID)
#define APP_PF1550_SERV_RPMSG_LOCAL_EPT_ADDR (0x1EU)
#define APP_PF1550_SERV_RPMSG_REMOTE_EPT_ADDR (0x400U)
#define APP_PF1550_SERV_RPMSG_NS_ANNOUNCE_STRING (uint8_t *)"rpmsg-regulator-channel"

#define APP_LIFE_CYCLE_RPMSG_SHMEM_BASE (0x9FF20000U)
#define APP_LIFE_CYCLE_RPMSG_LINK_ID (RL_PLATFORM_IMX7ULP_M4_LIFE_CYCLE_LINK_ID)
#define APP_LIFE_CYCLE_RPMSG_LOCAL_EPT_ADDR (0x1EU)
#define APP_LIFE_CYCLE_RPMSG_REMOTE_EPT_ADDR (0x400U)
#define APP_LIFE_CYCLE_RPMSG_NS_ANNOUNCE_STRING (uint8_t *)"rpmsg-life-cycle-channel"

#define APP_AUDIO_SERV_RPMSG_SHMEM_BASE (0x9FF30000U)
#define APP_AUDIO_SERV_RPMSG_LINK_ID (RL_PLATFORM_IMX7ULP_M4_AUDIO_LINK_ID)
#define APP_AUDIO_SERV_RPMSG_LOCAL_EPT_ADDR (0x1EU)
#define APP_AUDIO_SERV_RPMSG_REMOTE_EPT_ADDR (0x400U)
#define APP_AUDIO_SERV_RPMSG_NS_ANNOUNCE_STRING (uint8_t *)"rpmsg-audio-channel"
#define APP_AUDIO_LPI2C_SCL_GPIO GPIOA
#define APP_AUDIO_LPI2C_SCL_PIN 16U
#define APP_AUDIO_LPI2C_SDA_GPIO GPIOA
#define APP_AUDIO_LPI2C_SDA_PIN 17U

#define APP_KEYPAD_SERV_RPMSG_SHMEM_BASE (0x9FF50000U)
#define APP_KEYPAD_SERV_RPMSG_LINK_ID (RL_PLATFORM_IMX7ULP_M4_KEYPAD_LINK_ID)
#define APP_KEYPAD_SERV_RPMSG_LOCAL_EPT_ADDR (0x1EU)
#define APP_KEYPAD_SERV_RPMSG_REMOTE_EPT_ADDR (0x400U)
#define APP_KEYPAD_SERV_RPMSG_NS_ANNOUNCE_STRING (uint8_t *)"rpmsg-keypad-channel"

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void APP_InitClock(void); /* SCG init function defined in clock_scg.c */
void BOARD_I2C_ReleaseBus(void);

/* Send data to device on I2C Bus. */
static bool I2C_SendFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize);

/* Receive data from device on I2C Bus. */
static bool I2C_ReceiveFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const pmc0_hsrun_mode_config_t s_pmc0HsrunModeConfig = {
    .coreRegulatorVoltLevel = 38U, /* 0.596 + 38 * 0.01083 = 1.00754 */
    .enableForwardBias = 0U};

static const pmc0_run_mode_config_t s_pmc0RunModeConfig = {.coreRegulatorVoltLevel =
                                                               28U}; /* 0.596 + 28 * 0.01083 = 0.89924 */

static const pmc0_vlpr_mode_config_t s_pmc0VlprModeConfig = {
    .arrayRegulatorSelect = 0U,
    .coreRegulatorSelect = 0U,
    .lvdMonitorSelect = 0U,
    .hvdMonitorSelect = 0U,
    .enableForceHpBandgap = 0U,
    .coreRegulatorVoltLevel = 10U, /* 0.596 + 10 * 0.01083 = 0.7043 */
    .enableReverseBackBias = 0U};
static const pmc0_stop_mode_config_t s_pmc0StopModeConfig = {.coreRegulatorVoltLevel =
                                                                 28U}; /* 0.596 + 28 * 0.01083 = 0.89924 */
static const pmc0_vlps_mode_config_t s_pmc0VlpsModeConfig = {
    .arrayRegulatorSelect = 0U,
    .coreRegulatorSelect = 0U,
    .lvdMonitorSelect = 0U,
    .hvdMonitorSelect = 0U,
    .enableForceHpBandgap = 0U,
    .coreRegulatorVoltLevel = 10U, /* 0.596 + 10 * 0.01083 = 0.7043 */
    .enableReverseBackBias = 0U};
static const pmc0_lls_mode_config_t s_pmc0LlsModeConfig = {
    .arrayRegulatorSelect = 0U,
    .coreRegulatorSelect = 0U,
    .lvdMonitorSelect = 0U,
    .hvdMonitorSelect = 0U,
    .enableForceHpBandgap = 0U,
    .coreRegulatorVoltLevel = 10U, /* 0.596 + 10 * 0.01083 = 0.7043 */
    .enableReverseBackBias = 0U};
static const pmc0_vlls_mode_config_t s_pmc0VllsModeConfig = {
    .arrayRegulatorSelect = 2U, .lvdMonitorSelect = 0U, .hvdMonitorSelect = 0U, .enableForceHpBandgap = 0U};

static audio_serv_t *audioHandle;
static keypad_serv_t *keypadHandle;
static lifecycle_serv_t *lifeCycleHandle;
static lpi2c_rtos_handle_t lpi2c_rtos_handle;
static pf1550_remote_serv_t *pf1550RemoteHandle;
static srtm_channel_config_t srtmChannelConfig[4U];
static srtm_rpmsg_remote_config_t srtmRpmsgDrvConfig[4U];
static TimerHandle_t rebootTimer;
#pragma location="M4SuspendRam"
static volatile bool heartBeatContext;

pf1550_serv_handle_t pf1550Handle;
TimerHandle_t suspendTimer;
srtm_handle_t *srtmHandle;
#pragma location="M4SuspendRam"
srtm_rpmsg_remote_context_t rpmsgContext[4];

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Program entry point. */
extern void Reset_Handler(void);

extern srtm_handle_t *srtmHandle;

static void APP_InitDebugConsole(void)
{
    CLOCK_SetIpSrc(BOARD_DEBUG_UART_PCC_ADDRESS, APP_DEBUG_UART_DEFAULT_CLKSRC);
    uint32_t uartClkSrcFreq = CLOCK_GetIpFreq(BOARD_DEBUG_UART_PCC_ADDRESS);
    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}


void BOARD_I2C_ReleaseBusDelay(void)
{
    uint32_t i = 0U;
    for (i = 0U; i < APP_PF1550_LPI2C_DELAY; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0U;
    gpio_pin_config_t pin_config;

    /* Initialize PTB12/PTB13 as GPIO */
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    IOMUXC_SetPinMux(IOMUXC_PTA16_GPIOA16, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA17_GPIOA17, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA16_GPIOA16, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTA17_GPIOA17, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB12_GPIOB12, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB13_GPIOB13, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB12_GPIOB12, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTB13_GPIOB13, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);
    GPIO_PinInit(APP_AUDIO_LPI2C_SCL_GPIO, APP_AUDIO_LPI2C_SCL_PIN, &pin_config);
    GPIO_PinInit(APP_AUDIO_LPI2C_SDA_GPIO, APP_AUDIO_LPI2C_SDA_PIN, &pin_config);
    GPIO_PinInit(APP_PF1550_LPI2C_SCL_GPIO, APP_PF1550_LPI2C_SCL_PIN, &pin_config);
    GPIO_PinInit(APP_PF1550_LPI2C_SDA_GPIO, APP_PF1550_LPI2C_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(APP_PF1550_LPI2C_SDA_GPIO, APP_PF1550_LPI2C_SDA_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (i = 0U; i < 9U; i++)
    {
        GPIO_WritePinOutput(APP_PF1550_LPI2C_SCL_GPIO, APP_PF1550_LPI2C_SCL_PIN, 0U);
        GPIO_WritePinOutput(APP_AUDIO_LPI2C_SCL_GPIO, APP_AUDIO_LPI2C_SCL_PIN, 0U);
        BOARD_I2C_ReleaseBusDelay();

        GPIO_WritePinOutput(APP_PF1550_LPI2C_SDA_GPIO, APP_PF1550_LPI2C_SDA_PIN, 1U);
        GPIO_WritePinOutput(APP_AUDIO_LPI2C_SDA_GPIO, APP_AUDIO_LPI2C_SDA_PIN, 1U);
        BOARD_I2C_ReleaseBusDelay();

        GPIO_WritePinOutput(APP_PF1550_LPI2C_SCL_GPIO, APP_PF1550_LPI2C_SCL_PIN, 1U);
        GPIO_WritePinOutput(APP_AUDIO_LPI2C_SCL_GPIO, APP_AUDIO_LPI2C_SCL_PIN, 1U);
        BOARD_I2C_ReleaseBusDelay();
        BOARD_I2C_ReleaseBusDelay();
    }

    /* Send stop */
    GPIO_WritePinOutput(APP_PF1550_LPI2C_SCL_GPIO, APP_PF1550_LPI2C_SCL_PIN, 0U);
    GPIO_WritePinOutput(APP_AUDIO_LPI2C_SCL_GPIO, APP_AUDIO_LPI2C_SCL_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_WritePinOutput(APP_PF1550_LPI2C_SDA_GPIO, APP_PF1550_LPI2C_SDA_PIN, 0U);
    GPIO_WritePinOutput(APP_AUDIO_LPI2C_SDA_GPIO, APP_AUDIO_LPI2C_SDA_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_WritePinOutput(APP_PF1550_LPI2C_SCL_GPIO, APP_PF1550_LPI2C_SCL_PIN, 1U);
    GPIO_WritePinOutput(APP_AUDIO_LPI2C_SCL_GPIO, APP_AUDIO_LPI2C_SCL_PIN, 1U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_WritePinOutput(APP_PF1550_LPI2C_SDA_GPIO, APP_PF1550_LPI2C_SDA_PIN, 1U);
    GPIO_WritePinOutput(APP_AUDIO_LPI2C_SDA_GPIO, APP_AUDIO_LPI2C_SDA_PIN, 1U);
    BOARD_I2C_ReleaseBusDelay();
}

void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }
    DbgConsole_Deinit();

    if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeHsrun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
    {
        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to analog.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_PTA19_CMP1_IN3_3V, 0);

        if (kAPP_PowerModeVlls == targetMode)
        {
            SRTM_Suspend(srtmHandle);

            IOMUXC_SetPinMux(IOMUXC_PTB12_GPIOB12, 0U);
            IOMUXC_SetPinMux(IOMUXC_PTB13_GPIOB13, 0U);
            IOMUXC_SetPinConfig(IOMUXC_PTB12_GPIOB12, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK | IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
            IOMUXC_SetPinConfig(IOMUXC_PTB13_GPIOB13, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK | IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);

            SIM->SIM_DGO_GP2 |= SIM_SIM_DGO_GP2_SIM_DGO_GP2(0xA);
            SIM->SIM_DGO_CTRL0 |= SIM_SIM_DGO_CTRL0_UPDATE_DGO_GP2_MASK;
            while (!(SIM->SIM_DGO_CTRL0 & SIM_SIM_DGO_CTRL0_WR_ACK_DGO_GP2_MASK))
            {
            }
            SIM->SIM_DGO_CTRL0 |= SIM_SIM_DGO_CTRL0_WR_ACK_DGO_GP2_MASK;
            SIM->SIM_DGO_CTRL0 &= ~SIM_SIM_DGO_CTRL0_UPDATE_DGO_GP2_MASK;

            /* Disable SOSC, FIRC and SIRC when M4 enter VLLS Mode. */
            SCG0->SOSCCSR &= ~(SCG_SOSCCSR_SOSCLPEN_MASK | SCG_SOSCCSR_SOSCSTEN_MASK);
            SCG0->FIRCCSR &= ~SCG_FIRCCSR_FIRCSTEN_MASK;
            SCG0->SIRCCSR &= ~SCG_SIRCCSR_SIRCSTEN_MASK;
        }
    }

    /* Disable SysTick. */
    LPTMR_StopTimer(LPTMR0);
}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    /* Enable SysTick. */
    LPTMR_StartTimer(LPTMR0);

    if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeHsrun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
    {
        /*
         * Debug console RX pin is set to disable for current leakage, need to re-configure pinmux.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0);
    }

    APP_InitDebugConsole();
}
void WDOG1_IRQHandler(void)
{
    DisableIRQ(WDOG1_IRQn);

    /* Assert A7 Core into Reset status. */
    SIM->SOPT1 |= SIM_SOPT1_A7_SW_RESET_MASK;

    /* Process Normal A7 Reboot Process. */
    LIFECYCLE_SERV_RebootRemoteCoreFromISR(lifeCycleHandle);
}

static void LIFECYCLE_SERV_Event_Callback(lifecycle_serv_t *me, uint32_t event, void *arg)
{
    if (event & (LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag | LIFECYCLE_SERV_EventSwitchToRebootModeFlag))
    {
        DisableIRQ(WDOG1_IRQn);
        xTimerStart(rebootTimer, portMAX_DELAY);
    }
    else if (event & LIFECYCLE_SERV_EventSwitchToSuspendModeFlag)
    {
        xTimerStart(suspendTimer, portMAX_DELAY);
    }
    else if (event & LIFECYCLE_SERV_EventSwitchToRunModeFlag)
    {
        NVIC_ClearPendingIRQ(WDOG1_IRQn);
        EnableIRQ(WDOG1_IRQn);
        xTimerStop(suspendTimer, portMAX_DELAY);
    }
    else if (event & LIFECYCLE_SERV_EventGetHeartBeatEnable)
    {
        heartBeatContext = true;
    }
    else if (event & LIFECYCLE_SERV_EventGetHeartBeatDisable)
    {
        heartBeatContext = false;
    }
    else
    {
    }
}

static void LIFECYCLE_SERV_Reboot_Callback(TimerHandle_t xTimer)
{
    /* Assert A7 Reset. */
    SIM->SOPT1 |= SIM_SOPT1_A7_SW_RESET_MASK;
    vTaskDelay(1U);

    /* Delay 500ms to wait A7 Domain Power-Off. */
    vTaskDelay(500U);

    /* Set A7 power to its default value. */
    PF1550_SERV_SetRegulatorOutput(&pf1550Handle, kPF1550_ModuleSwitch1, kPF1550_OperatingStatusRun, 1100000U);
    PF1550_SERV_SetRegulatorOutput(&pf1550Handle, kPF1550_ModuleSwitch2, kPF1550_OperatingStatusRun, 1200000U);
    PF1550_SERV_SetRegulatorOutput(&pf1550Handle, kPF1550_ModuleSwitch3, kPF1550_OperatingStatusRun, 1800000U);
    PF1550_SERV_SetRegulatorOutput(&pf1550Handle, kPF1550_ModuleLdo2, kPF1550_OperatingStatusRun, 3300000U);

    /* Delay 500ms to wait A7 Domain Power stable. */
    vTaskDelay(500U);

    /* De-assert A7 Reset. */
    SIM->SOPT1 &= ~SIM_SOPT1_A7_SW_RESET_MASK;
}

static void LIFECYCLE_SERV_Suspend_Callback(TimerHandle_t xTimer)
{
    if (kMU_PowerModeDsm == MU_GetOtherCorePowerMode(MUA))
    {
        /* Disable A7 Core Power. */
        PF1550_SERV_EnableRegulator(&pf1550Handle,
                                kPF1550_ModuleSwitch1,
                                kPF1550_OperatingStatusRun,
                                false);

        /* Disable LPDDR3 Power. */
        GPIO_WritePinOutput(GPIOB, 6U, 1U);
    }
}

void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}

/*! @brief Main function */
int main(void)
{
    audio_serv_config_t audioConfig;
    lifecycle_serv_config_t lifeCycleConfig;
    lpi2c_master_config_t lpi2cMasterConfig;
    pf1550_remote_serv_config_t pf1550RemoteConfig;
    pf1550_serv_config_t pf1550Config;
    keypad_serv_config_t keypadConfig;
    srtm_config_t srtmConfig;
    gpio_pin_config_t gpioConfig = {
        kGPIO_DigitalOutput, 0U,
    };

    /* Power related. */
    SMC_SetPowerModeProtection(MSMC0, kSMC_AllowPowerModeAll);
    if (kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0)) /* Wakeup from VLLS. */
    {
        PMC0_ClearPadsIsolation();
        NVIC_ClearPendingIRQ(LLWU0_IRQn);
    }

    /* Setup VLLS Resume Entry. */
    /* SIM_DGO_GP1 change from 0x%x to 0x%x */
    SIM->SIM_DGO_GP1 = (uint32_t)Reset_Handler;
    SIM->SIM_DGO_CTRL0 |= SIM_SIM_DGO_CTRL0_UPDATE_DGO_GP1_MASK;
    /* Wait DGO GP1 updated */
    while ((SIM->SIM_DGO_CTRL0 & SIM_SIM_DGO_CTRL0_WR_ACK_DGO_GP1_MASK) == 0)
    {
    }
    /* Clear DGO GP1 ACK and UPDATE bits */
    SIM->SIM_DGO_CTRL0 =
        (SIM->SIM_DGO_CTRL0 & (~SIM_SIM_DGO_CTRL0_UPDATE_DGO_GP1_MASK)) | SIM_SIM_DGO_CTRL0_WR_ACK_DGO_GP1_MASK;

    /* Recover Port B Voltage Range. */
    if (kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0))
    {
        SIM->SIM_DGO_GP2 &= ~SIM_SIM_DGO_GP2_SIM_DGO_GP2(0xF);
        SIM->SIM_DGO_CTRL0 |= SIM_SIM_DGO_CTRL0_UPDATE_DGO_GP2_MASK;
        while (!(SIM->SIM_DGO_CTRL0 & SIM_SIM_DGO_CTRL0_WR_ACK_DGO_GP2_MASK))
        {
        }
        SIM->SIM_DGO_CTRL0 |= SIM_SIM_DGO_CTRL0_WR_ACK_DGO_GP2_MASK;
        SIM->SIM_DGO_CTRL0 &= ~SIM_SIM_DGO_CTRL0_UPDATE_DGO_GP2_MASK;
    }

    BOARD_InitPins();
    APP_InitClock();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    APP_InitDebugConsole();

    /* Initialize MPU */
    /* Make sure outstanding transfers are done. */
    __DMB();
    /* Disable the MPU. */
    MPU->CTRL = 0U;
    /* Select Region 0 to configure. */
    MPU->RNR = 0U;
    /* Set base address of Region 0 to the base of DDR memory region(1GB). */
    MPU->RBAR = 0x60000000U;
    /* Region 0 setting:
     * 1) Enable Instruction Access;
     * 2) Full Data Access Permission;
     * 3) Outer and inner Non-Cacheable;
     * 4) Region is Sharable;
     * 5) All Sub-Region Enable;
     * 6) MPU Protection Region size = 1GB;
     * 7) Enable Region 0.
     */
    MPU->RASR = 0x030C003BU;

    /* Enable Privileged default memory map and the MPU. */
    MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
    /* Memory barriers to ensure subsequence data & instruction
     * transfers using updated MPU settings.
     */
    __DSB();
    __ISB();

    CLOCK_EnableClock(APP_WAKEUP_BUTTON_CLOCK);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSircAsync);

    /* Use external clock source */
    CLOCK_SetIpSrcDiv(kCLOCK_Sai0, kCLOCK_IpSrcNone, 0, 0);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcSystem);

    /* Enable SAI MCLK Output */
    CLOCK_SetClkOutSel(kClockClkoutSelScgAuxPll);
    IOMUXC_SetPinMux(IOMUXC_PTB0_CLKOUT0, 0);

    /* Initialize PMC0 */
    PMC0_ConfigureHsrunMode(&s_pmc0HsrunModeConfig);
    PMC0_ConfigureRunMode(&s_pmc0RunModeConfig);
    PMC0_ConfigureVlprMode(&s_pmc0VlprModeConfig);
    PMC0_ConfigureStopMode(&s_pmc0StopModeConfig);
    PMC0_ConfigureVlpsMode(&s_pmc0VlpsModeConfig);
    PMC0_ConfigureLlsMode(&s_pmc0LlsModeConfig);
    PMC0_ConfigureVllsMode(&s_pmc0VllsModeConfig);

    /* Init output A7_POW_EN#. */
    GPIO_PinInit(GPIOA, 25U, &gpioConfig);
    /* Init output DDR_SW_EN#. */
    GPIO_PinInit(GPIOB, 6U, &gpioConfig);

    suspendTimer = xTimerCreate("Suspend", 500U, pdFALSE, NULL, LIFECYCLE_SERV_Suspend_Callback);
    rebootTimer = xTimerCreate("Reboot", 100U, pdFALSE, NULL, LIFECYCLE_SERV_Reboot_Callback);

    /*
     * lpi2cMasterConfig.debugEnable = false;
     * lpi2cMasterConfig.ignoreAck = false;
     * lpi2cMasterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cMasterConfig.baudRate_Hz = 100000U;
     * lpi2cMasterConfig.busIdleTimeout_ns = 0;
     * lpi2cMasterConfig.pinLowTimeout_ns = 0;
     * lpi2cMasterConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cMasterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cMasterConfig);
    lpi2cMasterConfig.baudRate_Hz = APP_PF1550_LPI2C_BAUDRATE;
    /*  Set LPI2C Master Interrupt Priority. */
    NVIC_SetPriority(APP_PF1550_LPI2C_IRQ, APP_PF1550_LPI2C_PRIO);
    /* Initialize LPI2C RTOS driver. */
    LPI2C_RTOS_Init(&lpi2c_rtos_handle, APP_PF1550_LPI2C_BASE, &lpi2cMasterConfig, APP_PF1550_LPI2C_CLKSRC_FREQ);

    /* Initialize PMIC Local Service. */
    PF1550_SERV_GetDefaultConfig(&pf1550Config);
    pf1550Config.drvConfig.I2C_SendFunc = I2C_SendFunc;
    pf1550Config.drvConfig.I2C_ReceiveFunc = I2C_ReceiveFunc;
    PF1550_SERV_Init(&pf1550Handle, &pf1550Config);

    /* Initialize SRTM Protocol Service. */
    srtmRpmsgDrvConfig[0U].linkId = APP_PF1550_SERV_RPMSG_LINK_ID;
    srtmRpmsgDrvConfig[0U].shareMemBase = APP_PF1550_SERV_RPMSG_SHMEM_BASE;
    srtmRpmsgDrvConfig[0U].localEpAddr = APP_PF1550_SERV_RPMSG_LOCAL_EPT_ADDR;
    srtmRpmsgDrvConfig[0U].remoteEpAddr = APP_PF1550_SERV_RPMSG_REMOTE_EPT_ADDR;
    srtmRpmsgDrvConfig[0U].announceString = APP_PF1550_SERV_RPMSG_NS_ANNOUNCE_STRING;
    srtmRpmsgDrvConfig[0U].context = &rpmsgContext[0U];
    srtmChannelConfig[0U].channelType = SRTM_CHANNEL_TYPE_RPMSG_REMOTE;
    srtmChannelConfig[0U].driverConfig = (void *)&srtmRpmsgDrvConfig[0U];
    srtmRpmsgDrvConfig[1U].linkId = APP_LIFE_CYCLE_RPMSG_LINK_ID;
    srtmRpmsgDrvConfig[1U].shareMemBase = APP_LIFE_CYCLE_RPMSG_SHMEM_BASE;
    srtmRpmsgDrvConfig[1U].localEpAddr = APP_LIFE_CYCLE_RPMSG_LOCAL_EPT_ADDR;
    srtmRpmsgDrvConfig[1U].remoteEpAddr = APP_LIFE_CYCLE_RPMSG_REMOTE_EPT_ADDR;
    srtmRpmsgDrvConfig[1U].announceString = APP_LIFE_CYCLE_RPMSG_NS_ANNOUNCE_STRING;
    srtmRpmsgDrvConfig[1U].context = &rpmsgContext[1U];
    srtmChannelConfig[1U].channelType = SRTM_CHANNEL_TYPE_RPMSG_REMOTE;
    srtmChannelConfig[1U].driverConfig = (void *)&srtmRpmsgDrvConfig[1U];
    srtmRpmsgDrvConfig[2U].linkId = APP_AUDIO_SERV_RPMSG_LINK_ID;
    srtmRpmsgDrvConfig[2U].shareMemBase = APP_AUDIO_SERV_RPMSG_SHMEM_BASE;
    srtmRpmsgDrvConfig[2U].localEpAddr = APP_AUDIO_SERV_RPMSG_LOCAL_EPT_ADDR;
    srtmRpmsgDrvConfig[2U].remoteEpAddr = APP_AUDIO_SERV_RPMSG_REMOTE_EPT_ADDR;
    srtmRpmsgDrvConfig[2U].announceString = APP_AUDIO_SERV_RPMSG_NS_ANNOUNCE_STRING;
    srtmRpmsgDrvConfig[2U].context = &rpmsgContext[2U];
    srtmChannelConfig[2U].channelType = SRTM_CHANNEL_TYPE_RPMSG_REMOTE;
    srtmChannelConfig[2U].driverConfig = (void *)&srtmRpmsgDrvConfig[2U];
    srtmRpmsgDrvConfig[3U].linkId = APP_KEYPAD_SERV_RPMSG_LINK_ID;
    srtmRpmsgDrvConfig[3U].shareMemBase = APP_KEYPAD_SERV_RPMSG_SHMEM_BASE;
    srtmRpmsgDrvConfig[3U].localEpAddr = APP_KEYPAD_SERV_RPMSG_LOCAL_EPT_ADDR;
    srtmRpmsgDrvConfig[3U].remoteEpAddr = APP_KEYPAD_SERV_RPMSG_REMOTE_EPT_ADDR;
    srtmRpmsgDrvConfig[3U].announceString = APP_KEYPAD_SERV_RPMSG_NS_ANNOUNCE_STRING;
    srtmRpmsgDrvConfig[3U].context = &rpmsgContext[3U];
    srtmChannelConfig[3U].channelType = SRTM_CHANNEL_TYPE_RPMSG_REMOTE;
    srtmChannelConfig[3U].driverConfig = (void *)&srtmRpmsgDrvConfig[3U];
    srtmConfig.queueSize = 64U;
    srtmConfig.channelCount = 4U;
    srtmConfig.channelConfigs = srtmChannelConfig;
    srtmHandle = SRTM_Init(&srtmConfig);
    if (!srtmHandle)
    {
        assert(false);
    }

    if (kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0))
    {
        SRTM_Resume(srtmHandle);
    }

    lifeCycleConfig.srtmHandle = srtmHandle;
    lifeCycleConfig.srtmChannelId = 1U;
    lifeCycleConfig.heartBeatTimeout_Ms = 31000U;
    if (kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0))
    {
        lifeCycleConfig.heartBeatEnable = heartBeatContext;
    }
    else
    {
        lifeCycleConfig.heartBeatEnable = false;
    }
    lifeCycleHandle = LIFECYCLE_SERV_Init(&lifeCycleConfig);
    if (!lifeCycleHandle)
    {
        assert(false);
    }
    LIFECYCLE_SERV_Subscribe(lifeCycleHandle, LIFECYCLE_SERV_Event_Callback, NULL);

    pf1550RemoteConfig.lifecycleHandle = lifeCycleHandle;
    pf1550RemoteConfig.pf1550Handle = &pf1550Handle;
    pf1550RemoteConfig.queueSize = 10U;
    pf1550RemoteConfig.srtmHandle = srtmHandle;
    pf1550RemoteConfig.srtmChannelId = 0U;
    pf1550RemoteHandle = PF1550_REMOTE_SERV_Init(&pf1550RemoteConfig);
    if (!pf1550RemoteHandle)
    {
        assert(false);
    }

    audioConfig.srtmHandle = srtmHandle;
    audioConfig.srtmChannelId = 2U;
    audioConfig.lifecycleHandle = lifeCycleHandle;
    audioConfig.queueSize = 10U;
    audioHandle = AUDIO_REMOTE_SERV_Init(&audioConfig);

    keypadConfig.srtmHandle = srtmHandle;
    keypadConfig.srtmChannelId = 3U;
    keypadConfig.lifecycleHandle = lifeCycleHandle;
    keypadConfig.queueSize = 10U;
    keypadConfig.wakeUpPins = LLWU->PF;
    keypadHandle = KEYPAD_SERV_Init(&keypadConfig);

    /* Set A7 Core WDOG Interrupt. */
    NVIC_SetPriority(WDOG1_IRQn, 4U);

    /* Create User Application Task. */
    xTaskCreate(POWER_MODE_SWITCH_Task, "Power Mode", configMINIMAL_STACK_SIZE * 2U, NULL,
                tskIDLE_PRIORITY + 1U, NULL);
    /* Create Ping Pong Application Task. */
    xTaskCreate(PINGPONG_Task, "Ping Pong", configMINIMAL_STACK_SIZE * 2U, (void *)lifeCycleHandle,
                tskIDLE_PRIORITY + 2U, NULL);
    /* Create Keypad Service Task. */
    xTaskCreate(KEYPAD_SERV_ProcessTask, "Keypad", configMINIMAL_STACK_SIZE * 2U, (void *)keypadHandle,
                tskIDLE_PRIORITY + 3U, NULL);
    /* Create Audio Service Task. */
    xTaskCreate(AUDIO_SERV_ProcessTask, "Audio", configMINIMAL_STACK_SIZE * 2U, (void *)audioHandle,
                tskIDLE_PRIORITY + 4U, NULL);
    /* Create Remote PMIC Service Task. */
    xTaskCreate(PF1550_REMOTE_SERV_ProcessTask, "PMIC A7", configMINIMAL_STACK_SIZE * 2U, (void *)pf1550RemoteHandle,
                tskIDLE_PRIORITY + 5U, NULL);
    /* Create Life-Cycle Service Task. */
    xTaskCreate(LIFECYCLE_SERV_ProcessTask, "Lifecycle", configMINIMAL_STACK_SIZE * 2U, (void *)lifeCycleHandle,
                tskIDLE_PRIORITY + 6U, NULL);
    /* Create SRTM Protocol Process Task. */
    xTaskCreate(SRTM_ProcessTask, "SRTM", configMINIMAL_STACK_SIZE * 2U, (void *)srtmHandle,
                tskIDLE_PRIORITY + 7U, NULL);
    /* Create Local PMIC Service Task. */
    xTaskCreate(PF1550_SERV_Task, "PMIC M4", configMINIMAL_STACK_SIZE * 2U, (void *)&pf1550Handle,
                tskIDLE_PRIORITY + 8U, NULL);

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}

static bool I2C_SendFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize)
{
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data = txBuff;
    masterXfer.dataSize = txBuffSize;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    /* Calling I2C Transfer API to start send. */
    return (kStatus_Success == LPI2C_RTOS_Transfer(&lpi2c_rtos_handle, &masterXfer)) ? true : false;
}

static bool I2C_ReceiveFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data = rxBuff;
    masterXfer.dataSize = rxBuffSize;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    /* Calling I2C Transfer API to start receive. */
    return (kStatus_Success == LPI2C_RTOS_Transfer(&lpi2c_rtos_handle, &masterXfer)) ? true : false;
}
