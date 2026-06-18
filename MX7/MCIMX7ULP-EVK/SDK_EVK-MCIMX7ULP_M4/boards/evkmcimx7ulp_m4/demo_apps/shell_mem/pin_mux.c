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
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AC19, peripheral: LPUART0, signal: lpuart_tx, pin_signal: PTA18, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AB19, peripheral: LPUART0, signal: lpuart_rx, pin_signal: PTA19, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AG6, peripheral: QSPIA, signal: 'qspia_ss_b, 0', pin_signal: PTB8}
  - {pin_num: AF8, peripheral: QSPIA, signal: qspia_sclk, pin_signal: PTB15}
  - {pin_num: AF9, peripheral: QSPIA, signal: 'qspia_data, 3', pin_signal: PTB16}
  - {pin_num: AF10, peripheral: QSPIA, signal: 'qspia_data, 2', pin_signal: PTB17}
  - {pin_num: AG10, peripheral: QSPIA, signal: 'qspia_data, 1', pin_signal: PTB18}
  - {pin_num: AD10, peripheral: QSPIA, signal: 'qspia_data, 0', pin_signal: PTB19}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA18_LPUART0_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA18_LPUART0_TX,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA19_LPUART0_RX,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB15_QSPIA_SCLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB16_QSPIA_DATA3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB17_QSPIA_DATA2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB18_QSPIA_DATA1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB19_QSPIA_DATA0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB8_QSPIA_SS0_B, 0U);
}
