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
  - {pin_num: AG2, peripheral: GPIOB, signal: 'port, 0', pin_signal: PTB0, PE: PE_1_pull_enabled}
  - {pin_num: AF2, peripheral: GPIOB, signal: 'port, 1', pin_signal: PTB1, PE: PE_1_pull_enabled}
  - {pin_num: AE3, peripheral: GPIOB, signal: 'port, 2', pin_signal: PTB2, PE: PE_1_pull_enabled}
  - {pin_num: AE4, peripheral: GPIOB, signal: 'port, 3', pin_signal: PTB3, PE: PE_1_pull_enabled}
  - {pin_num: AC7, peripheral: GPIOB, signal: 'port, 10', pin_signal: PTB10, PE: PE_1_pull_enabled}
  - {pin_num: AG4, peripheral: GPIOB, signal: 'port, 4', pin_signal: PTB4, PE: PE_1_pull_enabled}
  - {pin_num: AF4, peripheral: GPIOB, signal: 'port, 5', pin_signal: PTB5, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AF5, peripheral: GPIOB, signal: 'port, 6', pin_signal: PTB6, PE: PE_1_pull_enabled}
  - {pin_num: AF6, peripheral: GPIOB, signal: 'port, 7', pin_signal: PTB7, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AD7, peripheral: GPIOB, signal: 'port, 9', pin_signal: PTB9, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AB7, peripheral: GPIOB, signal: 'port, 11', pin_signal: PTB11, IBE: IBE_1_Enabled, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AG8, peripheral: GPIOB, signal: 'port, 14', pin_signal: PTB14, IBE: IBE_1_Enabled, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AG6, peripheral: GPIOB, signal: 'port, 8', pin_signal: PTB8, PE: PE_1_pull_enabled, PS: PS_1_pull_up}
  - {pin_num: AF8, peripheral: GPIOB, signal: 'port, 15', pin_signal: PTB15, PE: PE_1_pull_enabled}
  - {pin_num: AF9, peripheral: GPIOB, signal: 'port, 16', pin_signal: PTB16, PE: PE_1_pull_enabled}
  - {pin_num: AF10, peripheral: GPIOB, signal: 'port, 17', pin_signal: PTB17, PE: PE_1_pull_enabled}
  - {pin_num: AG10, peripheral: GPIOB, signal: 'port, 18', pin_signal: PTB18, PE: PE_1_pull_enabled}
  - {pin_num: AD10, peripheral: GPIOB, signal: 'port, 19', pin_signal: PTB19, PE: PE_1_pull_enabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTA25_GPIOA25, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA25_GPIOA25,
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
    IOMUXC_SetPinMux(IOMUXC_PTB0_GPIOB0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB0_GPIOB0,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB1_GPIOB1, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB1_GPIOB1,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB10_GPIOB10, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB10_GPIOB10,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB11_GPIOB11, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB11_GPIOB11,
                        IOMUXC0_SW_MUX_CTL_PAD_IBE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB14_GPIOB14, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB14_GPIOB14,
                        IOMUXC0_SW_MUX_CTL_PAD_IBE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB15_GPIOB15, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB15_GPIOB15,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB16_GPIOB16, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB16_GPIOB16,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB17_GPIOB17, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB17_GPIOB17,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB18_GPIOB18, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB18_GPIOB18,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB19_GPIOB19, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB19_GPIOB19,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB2_GPIOB2, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB2_GPIOB2,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB3_GPIOB3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB3_GPIOB3,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB4_GPIOB4, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB4_GPIOB4,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB5_GPIOB5, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB5_GPIOB5,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB6_GPIOB6, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB6_GPIOB6,
                        IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB7_GPIOB7, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB7_GPIOB7,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB8_GPIOB8, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB8_GPIOB8,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB9_GPIOB9, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB9_GPIOB9,
                        IOMUXC0_SW_MUX_CTL_PAD_PE_MASK |
                        IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AG18, peripheral: LPI2C0, signal: lpi2c_scl, pin_signal: PTA16, ODE: ODE_1_Open_drain}
  - {pin_num: AD19, peripheral: LPI2C0, signal: lpi2c_sda, pin_signal: PTA17, ODE: ODE_1_Open_drain}
  - {pin_num: AC8, peripheral: LPI2C3, signal: lpi2c_scl, pin_signal: PTB12, ODE: ODE_1_Open_drain}
  - {pin_num: AD8, peripheral: LPI2C3, signal: lpi2c_sda, pin_signal: PTB13, ODE: ODE_1_Open_drain}
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
    IOMUXC_SetPinMux(IOMUXC_PTB12_LPI2C3_SCL, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB12_LPI2C3_SCL,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB13_LPI2C3_SDA, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB13_LPI2C3_SDA,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
}
