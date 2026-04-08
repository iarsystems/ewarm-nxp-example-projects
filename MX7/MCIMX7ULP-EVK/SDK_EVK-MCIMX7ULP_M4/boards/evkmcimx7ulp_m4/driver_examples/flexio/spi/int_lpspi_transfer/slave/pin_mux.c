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
  - {pin_num: AG16, peripheral: LPSPI1, signal: lpspi_sin, pin_signal: PTA12}
  - {pin_num: AF16, peripheral: LPSPI1, signal: lpspi_sout, pin_signal: PTA13}
  - {pin_num: AF17, peripheral: LPSPI1, signal: lpspi_sck, pin_signal: PTA14}
  - {pin_num: AF18, peripheral: LPSPI1, signal: 'lpspi_pcs, 0', pin_signal: PTA15}
  - {pin_num: AG18, peripheral: FXIO0, signal: 'fxio_d, 0', pin_signal: PTA16}
  - {pin_num: AD19, peripheral: FXIO0, signal: 'fxio_d, 1', pin_signal: PTA17}
  - {pin_num: AD22, peripheral: FXIO0, signal: 'fxio_d, 4', pin_signal: PTA20}
  - {pin_num: AD20, peripheral: FXIO0, signal: 'fxio_d, 7', pin_signal: PTA23}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA12_LPSPI1_SIN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA13_LPSPI1_SOUT, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA14_LPSPI1_SCK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA15_LPSPI1_PCS0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA16_FXIO0_D0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA17_FXIO0_D1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA18_LPUART0_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA18_LPUART0_TX,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA19_LPUART0_RX,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA20_FXIO0_D4, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA23_FXIO0_D7, 0U);
}
