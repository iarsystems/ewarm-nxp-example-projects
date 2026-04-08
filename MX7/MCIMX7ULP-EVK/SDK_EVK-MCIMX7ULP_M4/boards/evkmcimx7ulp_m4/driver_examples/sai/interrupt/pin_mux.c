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

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    /* Initialize LPUART0 pins below */
    IOMUXC_SetPinMux(IOMUXC_PTA18_LPUART0_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_PTA18_LPUART0_TX, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK  |
                                                 IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTA19_LPUART0_RX, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK  |
                                                 IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);

    /* Initialize SAI0 pins below */
    IOMUXC_SetPinMux(IOMUXC_PTA4_I2S0_MCLK, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA2_I2S0_RXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA7_I2S0_TXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA5_I2S0_TX_BCLK, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA6_I2S0_TX_FS, 0);
    IOMUXC_SetPinConfig(IOMUXC_PTA4_I2S0_MCLK,    IOMUXC0_SW_MUX_CTL_PAD_DSE(1));
    IOMUXC_SetPinConfig(IOMUXC_PTA2_I2S0_RXD0,    0);
    IOMUXC_SetPinConfig(IOMUXC_PTA7_I2S0_TXD0,    IOMUXC0_SW_MUX_CTL_PAD_DSE(1));
    IOMUXC_SetPinConfig(IOMUXC_PTA5_I2S0_TX_BCLK, IOMUXC0_SW_MUX_CTL_PAD_DSE(1));
    IOMUXC_SetPinConfig(IOMUXC_PTA6_I2S0_TX_FS,   IOMUXC0_SW_MUX_CTL_PAD_DSE(1));
	
    /* Initialize GPIOA pins below */
    IOMUXC_SetPinMux(IOMUXC_PTA16_GPIOA16, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA17_GPIOA17, 0);
    IOMUXC_SetPinConfig(IOMUXC_PTA16_GPIOA16, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTA17_GPIOA17, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
}

void BOARD_I2C_ConfigurePins(void)
{
    /* Initialize I2C2 pins below */
    IOMUXC_SetPinMux(IOMUXC_PTA16_LPI2C0_SCL, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA17_LPI2C0_SDA, 0);
    IOMUXC_SetPinConfig(IOMUXC_PTA16_LPI2C0_SCL, IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTA17_LPI2C0_SDA, IOMUXC0_SW_MUX_CTL_PAD_ODE_MASK);
}

