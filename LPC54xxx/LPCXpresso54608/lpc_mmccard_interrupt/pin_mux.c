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
    CLOCK_EnableClock(kCLOCK_InputMux);

    /* SDIF PIN CONFIG */

    IOCON_PinMuxSet(IOCON, 2, 5, (IOCON_FUNC2 | IOCON_DIGITAL_EN | IOCON_MODE_INACT));  /* power enable pin */
    IOCON_PinMuxSet(IOCON, 3, 15, (IOCON_FUNC2 | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* SD write protect */
    IOCON_PinMuxSet(IOCON, 2, 10, (IOCON_FUNC2 | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* card detect */

    IOCON_PinMuxSet(IOCON, 2, 9,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* sd data[3] */
    IOCON_PinMuxSet(IOCON, 2, 8,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* sd data[2] */
    IOCON_PinMuxSet(IOCON, 2, 7,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* sd data[1] */
    IOCON_PinMuxSet(IOCON, 2, 6,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* sd data[0] */
    IOCON_PinMuxSet(IOCON, 2, 3,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* sd clk */
    IOCON_PinMuxSet(IOCON, 2, 4, (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_INACT)); /* sd cmd*/

    /* it is important to config the other 4 data bus IO , even if you don't use 8bit mode,but still need config it ,
     if not ,4bit mode will not work property */
    IOCON_PinMuxSet(IOCON, 4, 29,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[4] */
    IOCON_PinMuxSet(IOCON, 4, 30,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[5] */
    IOCON_PinMuxSet(IOCON, 4, 31,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[6] */
    IOCON_PinMuxSet(IOCON, 5, 0,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[7] */
    /* USART0 RX/TX pins */
    IOCON_PinMuxSet(IOCON, 0, 29, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
    IOCON_PinMuxSet(IOCON, 0, 30, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
}
