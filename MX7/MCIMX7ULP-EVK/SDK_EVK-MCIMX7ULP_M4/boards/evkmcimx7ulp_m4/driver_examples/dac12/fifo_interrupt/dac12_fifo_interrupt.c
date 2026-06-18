/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
#include "fsl_dac12.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC12_BASE DAC0
#define DEMO_DAC12_IRQ_ID DAC0_IRQn
#define DEMO_DAC12_HANDLER_FUNC DAC0_IRQHandler

#define DEMO_DAC_VALUE_ARRAY_SIZE 32U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitDebugConsole(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_DacIndex = 0U;
volatile uint32_t g_DacInterruptDone = false;
/* User-defined wave for DAC output. */
const uint32_t g_DacValues[DEMO_DAC_VALUE_ARRAY_SIZE] = {
    0U,   100,  200,  300,  400,  500,  600,  700,  800,  900,  1000, 1100, 1200, 1300, 1400, 1500,
    1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief Main function
*/
int main(void)
{
    uint32_t mDacIndex = 0;
    dac12_config_t dacConfigStruct;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    EnableIRQ(DEMO_DAC12_IRQ_ID); /* Enable interrupt in NVIC. */

    /* Configure the DAC. */
    DAC12_GetDefaultConfig(&dacConfigStruct);
    dacConfigStruct.fifoWorkMode = kDAC12_FIFOWorkAsNormalMode;
    dacConfigStruct.fifoTriggerMode = kDAC12_FIFOTriggerBySoftwareMode;
    dacConfigStruct.fifoWatermarkLevel = 4U; /* Watermark event would occur when remaining FIFO data is less than 4U. */
    DAC12_Init(DEMO_DAC12_BASE, &dacConfigStruct);
    DAC12_Enable(DEMO_DAC12_BASE, true); /* Enable output. */

    PRINTF("Press any key to trigger the DAC...\r\n");
    /* Enable DAC interrupts. */
    DAC12_EnableInterrupts(DEMO_DAC12_BASE, kDAC12_WatermarkInterruptEnable);

    while (1)
    {
        /* Trigger the buffer and move the pointer. */
        GETCHAR();

        DAC12_DoSoftwareTrigger(DEMO_DAC12_BASE);
        /* Wait  */
        while (!g_DacInterruptDone)
        {
        }
        g_DacInterruptDone = false;
        PRINTF("DAC next output: %d\r\n", g_DacValues[mDacIndex]);
        if (mDacIndex >= (DEMO_DAC_VALUE_ARRAY_SIZE - 1U))
        {
            mDacIndex = 0U;
        }
        else
        {
            mDacIndex++;
        }
    }
}

/*!
 * @brief IRQ function for DAC buffer interrupt
 */
void DEMO_DAC12_HANDLER_FUNC(void)
{
    uint32_t flags = DAC12_GetStatusFlags(DEMO_DAC12_BASE);

    if (0U != (kDAC12_WatermarkFlag & flags))
    {
        DAC12_SetData(DEMO_DAC12_BASE, g_DacValues[g_DacIndex]);
        if (g_DacIndex >= (DEMO_DAC_VALUE_ARRAY_SIZE - 1U))
        {
            g_DacIndex = 0U;
        }
        else
        {
            g_DacIndex++;
        }
    }
    g_DacInterruptDone = true;
}
