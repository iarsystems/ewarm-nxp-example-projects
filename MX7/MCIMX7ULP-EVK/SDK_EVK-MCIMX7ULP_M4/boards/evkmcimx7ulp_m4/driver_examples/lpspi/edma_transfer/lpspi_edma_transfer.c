/*
* Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
 *
 * Copyright (c) 2016, NXP Semiconductors, Inc.
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_edma.h"
#include "board.h"

#include "fsl_edma.h"
#include "fsl_dmamux.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
* Definitions
******************************************************************************/
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_BASEADDR LPSPI1
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT kLPSPI_Pcs0
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs0
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ (CLOCK_GetIpFreq(kCLOCK_Lpspi1))

#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE DMA_CH_MUX0_BASE
#define EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI1Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI1Tx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE DMA0_BASE
/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR LPSPI0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT kLPSPI_Pcs3
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs3

#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE DMA_CH_MUX0_BASE
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Tx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE DMA0_BASE

#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR ((DMAMUX_Type *)(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE))
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR ((DMA_Type *)(EXAMPLE_LPSPI_MASTER_DMA_BASE))

#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR ((DMAMUX_Type *)(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE))
#define EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR ((DMA_Type *)(EXAMPLE_LPSPI_SLAVE_DMA_BASE))

#define TRANSFER_SIZE (512U)        /*! Transfer dataSize .*/
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate .*/

/*******************************************************************************
* Prototypes
******************************************************************************/
/* LPSPI user callback */
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData);
void LPSPI_SlaveUserEdmaCallback(LPSPI_Type *base, lpspi_slave_edma_handle_t *handle, status_t status, void *userData);

extern uint32_t LPSPI_GetInstance(LPSPI_Type *base);
/*******************************************************************************
* Variables
******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};
uint8_t slaveRxData[TRANSFER_SIZE] = {0};
uint8_t slaveTxData[TRANSFER_SIZE] = {0};

lpspi_master_edma_handle_t g_m_edma_handle;
edma_handle_t lpspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t lpspiEdmaMasterTxDataToTxRegHandle;

lpspi_slave_edma_handle_t g_s_edma_handle;
edma_handle_t lpspiEdmaSlaveRxRegToRxDataHandle;
edma_handle_t lpspiEdmaSlaveTxDataToTxRegHandle;

volatile bool isSlaveTransferCompleted = false;
volatile bool isMasterTransferCompleted = false;

/*******************************************************************************
* Code
******************************************************************************/
void LPSPI_SlaveUserEdmaCallback(LPSPI_Type *base, lpspi_slave_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is LPSPI slave edma transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n");
    }

    if (status == kStatus_LPSPI_Error)
    {
        PRINTF("This is LPSPI slave edma transfer completed callback. \r\n");
        PRINTF("Error occured in this transfer. \r\n");
    }

    isSlaveTransferCompleted = true;
}

void LPSPI_MasterUserEdmaCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    isMasterTransferCompleted = true;
}

/*!
* @brief Main function
*/
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpspi0, kCLOCK_IpSrcFircAsync);

    PRINTF("LPSPI edma example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on one board.\r\n");
    PRINTF("Both master and slave use edma way.\r\n");

    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    /*DMA Mux setting and EDMA init*/
    uint32_t masterRxChannel, masterTxChannel;
    uint32_t slaveRxChannel, slaveTxChannel;
    edma_config_t userConfig;

    masterRxChannel = 0;
    masterTxChannel = 1;

    slaveRxChannel = 3;
    slaveTxChannel = 4;

    /* DMA MUX init*/
    DMAMUX_Init(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR);

#if (EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE != EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE)
    DMAMUX_Init(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR);
#endif

    DMAMUX_SetSource(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel,
                     EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel);

    DMAMUX_SetSource(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel,
                     EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel);

    DMAMUX_SetSource(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, slaveRxChannel, EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, slaveRxChannel);

    DMAMUX_SetSource(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel, EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel);

    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASEADDR, &userConfig);

#if (EXAMPLE_LPSPI_SLAVE_DMA_BASE != EXAMPLE_LPSPI_MASTER_DMA_BASE)
    EDMA_Init(EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR, &userConfig);
#endif

    uint32_t errorCount;
    uint32_t i;
    lpspi_master_config_t masterConfig;
    lpspi_slave_config_t slaveConfig;
    lpspi_transfer_t masterXfer;
    lpspi_transfer_t slaveXfer;

    /*Set up the transfer data*/
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = (i + 1) % 256;
        masterRxData[i] = 0x55;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0x55;
    }

    /*Master config*/
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame = 8 * TRANSFER_SIZE;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);

    /*Slave config*/
    slaveConfig.bitsPerFrame = masterConfig.bitsPerFrame;
    slaveConfig.cpol = masterConfig.cpol;
    slaveConfig.cpha = masterConfig.cpha;
    slaveConfig.direction = masterConfig.direction;

    slaveConfig.whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;
    slaveConfig.pcsActiveHighOrLow = masterConfig.pcsActiveHighOrLow;

    slaveConfig.pinCfg = kLPSPI_SdiInSdoOut;
    slaveConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_SlaveInit(EXAMPLE_LPSPI_SLAVE_BASEADDR, &slaveConfig);

    /*Set up slave first */
    memset(&(lpspiEdmaSlaveRxRegToRxDataHandle), 0, sizeof(lpspiEdmaSlaveRxRegToRxDataHandle));
    memset(&(lpspiEdmaSlaveTxDataToTxRegHandle), 0, sizeof(lpspiEdmaSlaveTxDataToTxRegHandle));

    EDMA_CreateHandle(&(lpspiEdmaSlaveRxRegToRxDataHandle), EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR, slaveRxChannel);
    EDMA_CreateHandle(&(lpspiEdmaSlaveTxDataToTxRegHandle), EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR, slaveTxChannel);

    LPSPI_SlaveTransferCreateHandleEDMA(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_edma_handle, LPSPI_SlaveUserEdmaCallback,
                                        NULL, &lpspiEdmaSlaveRxRegToRxDataHandle, &lpspiEdmaSlaveTxDataToTxRegHandle);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData = slaveTxData;
    slaveXfer.rxData = slaveRxData;
    slaveXfer.dataSize = TRANSFER_SIZE;
    slaveXfer.configFlags = EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER | kLPSPI_SlaveByteSwap;

    isSlaveTransferCompleted = false;
    LPSPI_SlaveTransferEDMA(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_edma_handle, &slaveXfer);

    /*Set up lpspi master*/
    memset(&(lpspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(lpspiEdmaMasterRxRegToRxDataHandle));
    memset(&(lpspiEdmaMasterTxDataToTxRegHandle), 0, sizeof(lpspiEdmaMasterTxDataToTxRegHandle));

    EDMA_CreateHandle(&(lpspiEdmaMasterRxRegToRxDataHandle), EXAMPLE_LPSPI_MASTER_DMA_BASEADDR, masterRxChannel);
    EDMA_CreateHandle(&(lpspiEdmaMasterTxDataToTxRegHandle), EXAMPLE_LPSPI_MASTER_DMA_BASEADDR, masterTxChannel);

    LPSPI_MasterTransferCreateHandleEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, LPSPI_MasterUserEdmaCallback,
                                         NULL, &lpspiEdmaMasterRxRegToRxDataHandle,
                                         &lpspiEdmaMasterTxDataToTxRegHandle);

    /*Start master transfer*/
    masterXfer.txData = masterTxData;
    masterXfer.rxData = masterRxData;
    masterXfer.dataSize = TRANSFER_SIZE;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap | kLPSPI_MasterPcsContinuous;

    isMasterTransferCompleted = false;
    LPSPI_MasterTransferEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, &masterXfer);

    /*Wait slave received all data .*/
    while ((!isSlaveTransferCompleted) || (!isMasterTransferCompleted))
    {
    }

    errorCount = 0;
    for (i = 0; i < TRANSFER_SIZE; i++)
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
        PRINTF("\r\nLPSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF("\r\nError occured in LPSPI transfer ! \r\n");
    }

    LPSPI_Deinit(EXAMPLE_LPSPI_MASTER_BASEADDR);
    LPSPI_Deinit(EXAMPLE_LPSPI_SLAVE_BASEADDR);

    PRINTF("End of example. \r\n");

    while (1)
    {
    }
}
