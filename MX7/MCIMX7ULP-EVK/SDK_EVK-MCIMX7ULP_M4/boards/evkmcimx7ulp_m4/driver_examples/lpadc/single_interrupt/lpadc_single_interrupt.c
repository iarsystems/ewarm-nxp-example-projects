/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#include "fsl_lpadc.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPADC_BASE ADC1
#define DEMO_LPADC_IRQn ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC1_IRQHandler


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitDebugConsole(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_LpadcConversionCompletedFlag = false;
lpadc_conv_result_t g_LpadcResultConfigStruct;
/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_LPADC_IRQ_HANDLER_FUNC(void)
{
    if (LPADC_GetConvResult(DEMO_LPADC_BASE, &g_LpadcResultConfigStruct))
    {
        g_LpadcConversionCompletedFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set the source for the ADC1 module */
    SCG0->FIRCDIV &= ~SCG_FIRCDIV_FIRCDIV2_MASK;
    SCG0->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(2); /* 24MHZ. */
    CLOCK_SetIpSrc(kCLOCK_Adc1, kCLOCK_IpSrcFircAsync);

    PRINTF("LPADC Interrupt Example\r\n");

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.FIFOWatermark = 0U; /* Set watermark as 0U. */
    LPADC_Init(DEMO_LPADC_BASE, &mLpadcConfigStruct);

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = 6U;                                 /* Take channel6A as ADC input.  */
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, 15U, &mLpadcCommandConfigStruct); /* Configurate the CMD15 buffer. */

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = 15U; /* CMD15 is executed. */
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */

    /* Enable the watermark interrupt. */
    LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFOWatermarkInterruptEnable);
    EnableIRQ(DEMO_LPADC_IRQn);

    /* When the number of datawords stored in the ADC Result FIFO is greater
    * than watermark value(0U), LPADC watermark interrupt would be triggered.
    */
    PRINTF("Please press any key to get user channel's ADC value.\r\n");
    GETCHAR();
    LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U);
    while (!g_LpadcConversionCompletedFlag)
    {
    }
    PRINTF("ADC value: %d\r\n", ((g_LpadcResultConfigStruct.convValue) >> 3U));
    while (1)
    {
    }
}
