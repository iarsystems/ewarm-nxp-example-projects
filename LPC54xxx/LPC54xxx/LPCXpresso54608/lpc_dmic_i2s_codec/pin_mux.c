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

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_iocon.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitPins(void)
{
    /* enable clock for IOCON */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* USART0 RX/TX pin */
    IOCON_PinMuxSet(IOCON, 0, 29, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
    IOCON_PinMuxSet(IOCON, 0, 30, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

    /* I2C */
    IOCON_PinMuxSet(IOCON, 3, 23,
                    IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_FASTI2C_EN | IOCON_MODE_INACT | IOCON_INPFILT_OFF);
    IOCON_PinMuxSet(IOCON, 3, 24,
                    IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_FASTI2C_EN | IOCON_MODE_INACT | IOCON_INPFILT_OFF);

    /* I2S */
    IOCON_PinMuxSet(IOCON, 2, 18, IOCON_FUNC3 | IOCON_DIGITAL_EN); /* Flexcomm 7 / SCK */
    IOCON_PinMuxSet(IOCON, 2, 19, IOCON_FUNC3 | IOCON_DIGITAL_EN); /* Flexcomm 7 / SDA */
    IOCON_PinMuxSet(IOCON, 2, 20, IOCON_FUNC3 | IOCON_DIGITAL_EN); /* Flexcomm 7 / WS */

    IOCON_PinMuxSet(IOCON, 4, 1, IOCON_FUNC2 | IOCON_DIGITAL_EN); /* Flexcomm 6 / SCK */
    IOCON_PinMuxSet(IOCON, 4, 2, IOCON_FUNC2 | IOCON_DIGITAL_EN); /* Flexcomm 6 / SDA */
    IOCON_PinMuxSet(IOCON, 4, 3, IOCON_FUNC2 | IOCON_DIGITAL_EN); /* Flexcomm 6 / WS */

    /* MCLK output for I2S */
    IOCON_PinMuxSet(IOCON, 3, 11, IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    SYSCON->MCLKIO = 1U;

    /* PDM interface */
    IOCON_PinMuxSet(IOCON, 3, 0, IOCON_FUNC2 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF); /* PDM0 CLK */
    IOCON_PinMuxSet(IOCON, 3, 1, IOCON_FUNC2 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF); /* PDM0 DATA  */

    /* PDM interface */
    IOCON_PinMuxSet(IOCON, 1, 2, IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF); /* PDM1 CLK */
    IOCON_PinMuxSet(IOCON, 1, 3, IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF); /* PDM1 DATA  */
}
