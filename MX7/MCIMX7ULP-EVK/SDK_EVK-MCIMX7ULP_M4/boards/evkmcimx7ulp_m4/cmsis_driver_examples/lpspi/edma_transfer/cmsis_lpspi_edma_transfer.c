/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lpspi_cmsis.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*DMA related */
#define EXAMPLE_SPI_DMA_BASEADDR DMA0
#define EXAMPLE_SPI_DMAMUX_BASEADDR DMA_CH_MUX0

/*Master related*/
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME (kCLOCK_Lpspi1)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

/* Driver mapping */
#define DRIVER_MASTER_SPI Driver_SPI0
#define DRIVER_SLAVE_SPI Driver_SPI1
#define TRANSFER_SIZE (512U)        /*! Transfer dataSize */
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user SignalEvent */
void LPSPI_MasterSignalEvent_t(uint32_t event);
void LPSPI_SlaveSignalEvent_t(uint32_t event);
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};
uint8_t slaveRxData[TRANSFER_SIZE] = {0};
uint8_t slaveTxData[TRANSFER_SIZE] = {0};

volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t LPSPI0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi0);
}
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi1);
}

void LPSPI_MasterSignalEvent_t(uint32_t event)
{
    /* user code */
}
void LPSPI_SlaveSignalEvent_t(uint32_t event)
{
    /* user code */
    isTransferCompleted = true;
    PRINTF("This is SlaveSignalEvent.\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    uint32_t errorCount;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);

    /* Init EDMA and DMAMUX */
    edma_config_t edma_config = {0};
    EDMA_GetDefaultConfig(&edma_config);
    DMAMUX_Init(EXAMPLE_SPI_DMAMUX_BASEADDR);
    EDMA_Init(EXAMPLE_SPI_DMA_BASEADDR, &edma_config);
    PRINTF("\r\nLPSPI CMSIS edma transfer example start.\r\n");
    PRINTF("This example use one LPSPI instance as master and another as slave on one board.\r\n");
    PRINTF("Master use edma way , slave uses interrupt.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS  \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    /*LPSPI master init*/
    DRIVER_MASTER_SPI.Initialize(LPSPI_MasterSignalEvent_t);
    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER, TRANSFER_BAUDRATE);

    /*LPSPI slave init*/
    DRIVER_SLAVE_SPI.Initialize(LPSPI_SlaveSignalEvent_t);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_SLAVE_SPI.Control(ARM_SPI_MODE_SLAVE, false);

    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 256U;
        masterRxData[i] = 0U;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0U;
    }

    isTransferCompleted = false;
    /* Start slave transfer */
    DRIVER_SLAVE_SPI.Transfer(slaveTxData, slaveRxData, TRANSFER_SIZE);
    /* Start master transfer */
    DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, TRANSFER_SIZE);

    /* Wait until transfer completed */
    while (!isTransferCompleted)
    {
    }
    /* Check the data */
    errorCount = 0U;
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != slaveRxData[i])
        {
            errorCount++;
        }

        if (slaveTxData[i] != masterRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0)
    {
        PRINTF(" \r\nLPSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF(" \r\nError occured in LPSPI transfer ! \r\n");
    }

    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_OFF);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_OFF);
    DRIVER_MASTER_SPI.Uninitialize();
    DRIVER_SLAVE_SPI.Uninitialize();

    while (1)
    {
    }
}
