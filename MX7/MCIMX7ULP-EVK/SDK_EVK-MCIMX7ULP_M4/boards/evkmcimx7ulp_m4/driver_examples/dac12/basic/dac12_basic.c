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


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitDebugConsole(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    dac12_config_t dacConfigStruct;
    uint32_t dacValue;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nDAC basic Example.\r\n");

    /* Configure the DAC. */
    /*
     * dacConfigStruct.fifoWatermarkLevel = 4U;
     * dacConfigStruct.fifoWorkMode = kDAC12_FIFODisabled;
     * dacConfigStruct.referenceVoltageSource = kDAC12_ReferenceVoltageSourceAlt1;
     * dacConfigStruct.fifoTriggerMode = kDAC12_FIFOTriggerByHardwareMode;
     * dacConfigStruct.referenceCurrentSource = kDAC12_ReferenceCurrentSourceAlt0;
     * dacConfigStruct.speedMode = kDAC12_SpeedLowMode;
     * dacConfigStruct.enableAnalogBuffer = false;
     * dacConfigStruct.currentReferenceInternalTrimValue = 0x4;
     */
    DAC12_GetDefaultConfig(&dacConfigStruct);
    dacConfigStruct.fifoWatermarkLevel = 4U;
    DAC12_Init(DEMO_DAC12_BASE, &dacConfigStruct);
    DAC12_Enable(DEMO_DAC12_BASE, true); /* Enable output. */
    /* The fifo is not enabled, so the read pointer can not move automatically. */

    while (1)
    {
        PRINTF("\r\nPlease input a value (0 - 4095) to output with DAC: ");
        SCANF("%d", &dacValue);
        PRINTF("\r\nInput value is %d\r\n", dacValue);
        if (dacValue > 0xFFFU)
        {
            PRINTF("Your value is output of range.\r\n");
            continue;
        }
        DAC12_SetData(DEMO_DAC12_BASE, dacValue);
        PRINTF("DAC out: %d\r\n", dacValue);
        /*
        * The value in the first item would be converted. User can measure the output voltage from DAC_OUTx pin.
        */
    }
}
