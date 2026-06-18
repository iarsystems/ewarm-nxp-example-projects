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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_clock.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "MCIMX7ULP-EVK"
#define MANUFACTURER_NAME "NXP"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BAUDRATE 115200u
#define BOARD_DEBUG_UART_BASEADDR LPUART0_BASE
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpuart0)
#define BOARD_UART_IRQ LPUART0_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART0_IRQHandler
#define BOARD_DEBUG_UART_PCC_ADDRESS kCLOCK_Lpuart0
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_IpSrcSystem

#define BOARD_PRESSURE_I2C_BASEADDR LPI2C0
#define BOARD_LPI2C_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c0)
#define BOARD_LPI2C_CLKSRC kCLOCK_IpSrcSystem

#define BOARD_EEPROM_LPSPI_BASEADDR LPSPI0
#define BOARD_LPSPI_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpspi0)
#define BOARD_LPSPI_CLKSRC kCLOCK_IpSrcSystem
#define BOARD_LPSPI_PCS_FOR_INIT kLPSPI_Pcs0
#define BOARD_LPSPI_PCS_FOR_TRANSFER kLPSPI_MasterPcs0

#define BOARD_VOLP_GPIO GPIOB
#define BOARD_VOLP_PORT PORTB
#define BOARD_VOLP_GPIO_PIN 14U
#define BOARD_VOLP_IRQ PCTLB_IRQn
#define BOARD_VOLP_IRQ_HANDLER PCTLB_IRQHandler
#define BOARD_VOLP_NAME "VOL+"

#define BOARD_VOLM_GPIO GPIOB
#define BOARD_VOLM_PORT PORTB
#define BOARD_VOLM_GPIO_PIN 11U
#define BOARD_VOLM_IRQ PCTLB_IRQn
#define BOARD_VOLM_IRQ_HANDLER PCTLB_IRQHandler
#define BOARD_VOLM_NAME "VOL-"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
