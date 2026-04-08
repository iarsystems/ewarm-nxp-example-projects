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
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI0_InitPins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AG18, peripheral: LPSPI0, signal: lpspi_sout, pin_signal: PTA16}
  - {pin_num: AD19, peripheral: LPSPI0, signal: lpspi_sck, pin_signal: PTA17}
  - {pin_num: AD22, peripheral: LPSPI0, signal: lpspi_sin, pin_signal: PTA20}
  - {pin_num: AD20, peripheral: LPSPI0, signal: 'lpspi_pcs, 3', pin_signal: PTA23}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : LPSPI0_InitPins */
void LPSPI0_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA16_LPSPI0_SOUT, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA17_LPSPI0_SCK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA20_LPSPI0_SIN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA23_LPSPI0_PCS3, 0U);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI0_DeinitPins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AG18, peripheral: GPIOA, signal: 'port, 16', pin_signal: PTA16}
  - {pin_num: AD19, peripheral: GPIOA, signal: 'port, 17', pin_signal: PTA17}
  - {pin_num: AD22, peripheral: GPIOA, signal: 'port, 20', pin_signal: PTA20}
  - {pin_num: AD20, peripheral: GPIOA, signal: 'port, 23', pin_signal: PTA23}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : LPSPI0_DeinitPins */
void LPSPI0_DeinitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA16_GPIOA16, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA17_GPIOA17, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA20_GPIOA20, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA23_GPIOA23, 0U);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI1_InitPins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AG16, peripheral: LPSPI1, signal: lpspi_sin, pin_signal: PTA12}
  - {pin_num: AF16, peripheral: LPSPI1, signal: lpspi_sout, pin_signal: PTA13}
  - {pin_num: AF17, peripheral: LPSPI1, signal: lpspi_sck, pin_signal: PTA14}
  - {pin_num: AF18, peripheral: LPSPI1, signal: 'lpspi_pcs, 0', pin_signal: PTA15}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : LPSPI1_InitPins */
void LPSPI1_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA12_LPSPI1_SIN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA13_LPSPI1_SOUT, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA14_LPSPI1_SCK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA15_LPSPI1_PCS0, 0U);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI1_DeinitPins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AG16, peripheral: GPIOA, signal: 'port, 12', pin_signal: PTA12}
  - {pin_num: AF16, peripheral: GPIOA, signal: 'port, 13', pin_signal: PTA13}
  - {pin_num: AF17, peripheral: GPIOA, signal: 'port, 14', pin_signal: PTA14}
  - {pin_num: AF18, peripheral: GPIOA, signal: 'port, 15', pin_signal: PTA15}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : LPSPI1_DeinitPins */
void LPSPI1_DeinitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA12_GPIOA12, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA13_GPIOA13, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA14_GPIOA14, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA15_GPIOA15, 0U);
}
