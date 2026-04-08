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
  - {pin_num: AG14, peripheral: I2S0, signal: 'i2s_rxd, 0', pin_signal: PTA2}
  - {pin_num: AD15, peripheral: I2S0, signal: i2s_mclk, pin_signal: PTA4, DSE: DSE_1_Hi_Drive}
  - {pin_num: AC15, peripheral: I2S0, signal: i2s_tx_bclk, pin_signal: PTA5, DSE: DSE_1_Hi_Drive}
  - {pin_num: AB15, peripheral: I2S0, signal: i2s_tx_fs, pin_signal: PTA6, DSE: DSE_1_Hi_Drive}
  - {pin_num: AD14, peripheral: I2S0, signal: 'i2s_txd, 0', pin_signal: PTA7, DSE: DSE_1_Hi_Drive}
  - {pin_num: AG18, peripheral: GPIOA, signal: 'port, 16', pin_signal: PTA16, OBE: OBE_1_Enabled, ODE: ODE_0_Push_pull}
  - {pin_num: AD19, peripheral: GPIOA, signal: 'port, 17', pin_signal: PTA17, OBE: OBE_1_Enabled, ODE: ODE_0_Push_pull}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA16_GPIOA16, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA16_GPIOA16,
                        IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA17_GPIOA17, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA17_GPIOA17,
                        IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA18_LPUART0_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA18_LPUART0_TX,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA19_LPUART0_RX,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA2_I2S0_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA4_I2S0_MCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA4_I2S0_MCLK,
                        IOMUXC0_SW_MUX_CTL_PAD_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA5_I2S0_TX_BCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA5_I2S0_TX_BCLK,
                        IOMUXC0_SW_MUX_CTL_PAD_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA6_I2S0_TX_FS, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA6_I2S0_TX_FS,
                        IOMUXC0_SW_MUX_CTL_PAD_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA7_I2S0_TXD0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA7_I2S0_TXD0,
                        IOMUXC0_SW_MUX_CTL_PAD_DSE_MASK);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AG18, peripheral: LPI2C0, signal: lpi2c_scl, pin_signal: PTA16, ODE: ODE_1_Open_drain}
  - {pin_num: AD19, peripheral: LPI2C0, signal: lpi2c_sda, pin_signal: PTA17, ODE: ODE_1_Open_drain}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : BOARD_I2C_ConfigurePins */
void BOARD_I2C_ConfigurePins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA16_LPI2C0_SCL, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA16_LPI2C0_SCL,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA17_LPI2C0_SDA, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA17_LPI2C0_SDA,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
}
