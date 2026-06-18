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
  - {pin_num: AC8, peripheral: LPI2C3, signal: lpi2c_scl, pin_signal: PTB12, ODE: ODE_1_Open_drain}
  - {pin_num: AD8, peripheral: LPI2C3, signal: lpi2c_sda, pin_signal: PTB13, ODE: ODE_1_Open_drain}
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
    IOMUXC_SetPinMux(IOMUXC_PTB12_LPI2C3_SCL, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB12_LPI2C3_SCL,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB13_LPI2C3_SDA, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB13_LPI2C3_SDA,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: M4}
- pin_list:
  - {pin_num: AC8, peripheral: LPI2C3, signal: lpi2c_scl, pin_signal: PTB12, ODE: ODE_1_Open_drain}
  - {pin_num: AD8, peripheral: LPI2C3, signal: lpi2c_sda, pin_signal: PTB13, ODE: ODE_1_Open_drain}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/* Function Name : BOARD_I2C_ConfigurePins */
void BOARD_I2C_ConfigurePins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PTB12_LPI2C3_SCL, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB12_LPI2C3_SCL,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB13_LPI2C3_SDA, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB13_LPI2C3_SDA,
                        IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
}
