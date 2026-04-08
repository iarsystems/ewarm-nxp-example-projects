/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_lpit.h"
#include "fsl_lptmr.h"
#include "fsl_trgmux.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT 500000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool lpitIsrFlag = false;
volatile uint32_t lpitCounter = 0U;
/*******************************************************************************
 * Code
 ******************************************************************************/
void LPTMR0_IRQHandler(void)
{
      LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    /*
     * Workaround for TWR-KV58: because write buffer is enabled, adding
     * memory barrier instructions to make sure clearing interrupt flag completed
     * before go out ISR
     */
    __DSB();
    __ISB();
}
void LPIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel0TimerFlag);
    
    lpitCounter++;
    lpitIsrFlag = true;
}
static void DEMO_InitLptmrTrigger(void)
{
    lptmr_config_t lptmrConfig;

    /* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);

    /* Initialize the LPTMR */
    LPTMR_Init(LPTMR0, &lptmrConfig);

    /*
     * Set timer period.
     * Note : the parameter "ticks" of LPTMR_SetTimerPeriod should be equal or greater than 1.
    */
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));

    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(LPTMR0_IRQn);

    PRINTF("Example Starts!\r\n");
    /* Start counting */
    LPTMR_StartTimer(LPTMR0);
}

void ConfigTriggerSource(void)
{
  TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX0_LPIT0, kTRGMUX_TriggerInput0, kTRGMUX0_SourceLPTMR0);
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize LPIT */
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;
    
    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSystem);
    
    ConfigTriggerSource();
    /*
     * lpitConfig.enableRunInDebug = false;
     * lpitConfig.enableRunInDoze = false;
     */
    LPIT_GetDefaultConfig(&lpitConfig);

    /* Init lpit module */
    LPIT_Init(LPIT0, &lpitConfig);

    lpitChannelConfig.chainChannel = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger = false;
    lpitChannelConfig.enableStopOnTimeout = false;
    lpitChannelConfig.timerMode = kLPIT_TriggerAccumulator;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_0, &lpitChannelConfig);

    /* Set timer period for channel 0 */
    LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_0, 1);

    /* Enable timer interrupts for channel 0 */
    LPIT_EnableInterrupts(LPIT0, kLPIT_Channel0TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(LPIT0_IRQn);
   
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);

    DEMO_InitLptmrTrigger();

    while (true)
    {
        /* Check whether occur interupt */
        if (true == lpitIsrFlag)
        {
          PRINTF("LPIT interrupt No.%d\r\n", lpitCounter);
          lpitIsrFlag = false;
        }      
        
    }
}
