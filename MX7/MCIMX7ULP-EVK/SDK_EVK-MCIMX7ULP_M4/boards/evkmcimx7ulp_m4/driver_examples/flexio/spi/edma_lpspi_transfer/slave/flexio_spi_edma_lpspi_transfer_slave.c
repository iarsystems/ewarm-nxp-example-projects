/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "fsl_flexio_spi_edma.h"
#include "board.h"
#include "fsl_dmamux.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Master related*/
#define MASTER_LPSPI_BASEADDR LPSPI1
#define MASTER_LPSPI_IRQ_HANDLE LPSPI1_DriverIRQHandler
#define MASTER_LPSPI_IRQN LPSPI1_IRQn
#define MASTER_LPSPI_PCS_FOR_INIT kLPSPI_Pcs0
#define MASTER_LPSPI_PCS_FOR_TRANSFER kLPSPI_MasterPcs0
#define MASTER_LPSPI_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Lpspi1)

/*Slave related*/
#define SLAVE_FLEXIO_SPI_BASEADDR FLEXIO0
#define FLEXIO_SPI_SOUT_PIN 0U
#define FLEXIO_SPI_SIN_PIN 4U
#define FLEXIO_SPI_CLK_PIN 1U
#define FLEXIO_SPI_PCS_PIN 7U
#define SLAVE_FLEXIO_SPI_IRQ FLEXIO0_IRQn
#define FLEXIO_DMA_REQUEST_SOURCE_BASE kDmaRequestMux0FlexIO0Shifter0
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR DMA_CH_MUX0
#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX1_DMA_LPSPI_CHANNEL 0U
#define FLEXIO_SPI_RX1_DMA_LPSPI_CHANNEL 1U
#define FLEXIO_SPI_TX2_DMA_LPSPI_CHANNEL 16U
#define FLEXIO_SPI_RX2_DMA_LPSPI_CHANNEL 17U
#define TRANSFER_SIZE 256U        /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user callback */
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData);
void FLEXIO_SPI_SlaveUserCallback(FLEXIO_SPI_Type *base,
                                  flexio_spi_slave_edma_handle_t *handle,
                                  status_t status,
                                  void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};

lpspi_master_handle_t g_m_handle;
FLEXIO_SPI_Type spiDev;
flexio_spi_slave_edma_handle_t g_s_handle;

edma_handle_t txHandle;
edma_handle_t rxHandle;

volatile bool isSlaveTransferCompleted = false;
volatile bool isMasterTransferCompleted = false;
bool isMasterIrqInIntmux = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }
    isMasterTransferCompleted = true;
}

void FLEXIO_SPI_SlaveUserCallback(FLEXIO_SPI_Type *base,
                                  flexio_spi_slave_edma_handle_t *handle,
                                  status_t status,
                                  void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }

    isSlaveTransferCompleted = true;

    PRINTF("This is FLEXIO SPI slave call back.\r\n");
}

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Flexio0, kCLOCK_IpSrcFircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);

    PRINTF("LPSPI Master interrupt - FLEXIO SPI Slave edma example start.\r\n");
    PRINTF("This example use one lpspi instance as master and one flexio spi slave on one board.\r\n");
    PRINTF("Master uses interrupt and slave uses edma way.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- FLEXIO_SPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS  \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    uint32_t errorCount;
    uint32_t i;
    lpspi_master_config_t masterConfig;
    flexio_spi_slave_config_t slaveConfig;
    lpspi_transfer_t masterXfer;
    flexio_spi_transfer_t slaveXfer;
    edma_config_t config;

    /*Master config*/
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = MASTER_LPSPI_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit(MASTER_LPSPI_BASEADDR, &masterConfig, MASTER_LPSPI_CLOCK_FREQUENCY);

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    if (MASTER_LPSPI_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        INTMUX_Init(INTMUX0);
        INTMUX_SetChannelMode(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, kINTMUX_ChannelLogicOR);
        isMasterIrqInIntmux = true;
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, MASTER_LPSPI_IRQN);
    }
#endif

    /* Slave config */
    FLEXIO_SPI_SlaveGetDefaultConfig(&slaveConfig);

    spiDev.flexioBase = SLAVE_FLEXIO_SPI_BASEADDR;
    spiDev.SDOPinIndex = FLEXIO_SPI_SOUT_PIN;
    spiDev.SDIPinIndex = FLEXIO_SPI_SIN_PIN;
    spiDev.SCKPinIndex = FLEXIO_SPI_CLK_PIN;
    spiDev.CSnPinIndex = FLEXIO_SPI_PCS_PIN;
    spiDev.shifterIndex[0] = 0U;
    spiDev.shifterIndex[1] = 1U;
    spiDev.timerIndex[0] = 0U;
    FLEXIO_SPI_SlaveInit(&spiDev, &slaveConfig);

    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 256U;
        masterRxData[i] = 0U;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0U;
    }

    isSlaveTransferCompleted = false;

    /*Init DMA for example.*/
    DMAMUX_Init(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR);
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR, &config);


    /* Request DMA channels for TX & RX. */
    if ((FLEXIO_DMA_REQUEST_SOURCE_BASE+ spiDev.shifterIndex[0]) < 0x200)
    {
        DMAMUX_SetSource(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_TX1_DMA_LPSPI_CHANNEL, FLEXIO_DMA_REQUEST_SOURCE_BASE+ spiDev.shifterIndex[0]);
        DMAMUX_SetSource(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_RX1_DMA_LPSPI_CHANNEL, FLEXIO_DMA_REQUEST_SOURCE_BASE+ spiDev.shifterIndex[1]);
        DMAMUX_EnableChannel(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_TX1_DMA_LPSPI_CHANNEL);
        DMAMUX_EnableChannel(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_RX1_DMA_LPSPI_CHANNEL);
        EDMA_CreateHandle(&txHandle, EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR, FLEXIO_SPI_TX1_DMA_LPSPI_CHANNEL);
        EDMA_CreateHandle(&rxHandle, EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR, FLEXIO_SPI_RX1_DMA_LPSPI_CHANNEL);
    }
    else
    {
        DMAMUX_SetSource(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_TX2_DMA_LPSPI_CHANNEL, FLEXIO_DMA_REQUEST_SOURCE_BASE+ spiDev.shifterIndex[0]);
        DMAMUX_SetSource(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_RX2_DMA_LPSPI_CHANNEL, FLEXIO_DMA_REQUEST_SOURCE_BASE+ spiDev.shifterIndex[1]);
        DMAMUX_EnableChannel(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_TX2_DMA_LPSPI_CHANNEL);
        DMAMUX_EnableChannel(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_RX2_DMA_LPSPI_CHANNEL);
        EDMA_CreateHandle(&txHandle, EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR, FLEXIO_SPI_TX2_DMA_LPSPI_CHANNEL);
        EDMA_CreateHandle(&rxHandle, EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR, FLEXIO_SPI_RX2_DMA_LPSPI_CHANNEL);
    }
    /* Set up slave first */
    FLEXIO_SPI_SlaveTransferCreateHandleEDMA(&spiDev, &g_s_handle, FLEXIO_SPI_SlaveUserCallback, NULL, &txHandle,
                                             &rxHandle);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData = slaveTxData;
    slaveXfer.rxData = slaveRxData;
    slaveXfer.dataSize = TRANSFER_SIZE;
    slaveXfer.flags = kFLEXIO_SPI_8bitMsb;

    FLEXIO_SPI_SlaveTransferEDMA(&spiDev, &g_s_handle, &slaveXfer);

    /* Set up master transfer */
    LPSPI_MasterTransferCreateHandle(MASTER_LPSPI_BASEADDR, &g_m_handle, LPSPI_MasterUserCallback, NULL);

    /*Start master transfer*/
    masterXfer.txData = masterTxData;
    masterXfer.rxData = masterRxData;
    masterXfer.dataSize = TRANSFER_SIZE;
    masterXfer.configFlags = MASTER_LPSPI_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap;

    LPSPI_MasterTransferNonBlocking(MASTER_LPSPI_BASEADDR, &g_m_handle, &masterXfer);

    /* Wait slave received all data. */
    while (!(isSlaveTransferCompleted && isMasterTransferCompleted))
    {
    }

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
    if (errorCount == 0U)
    {
        PRINTF("LPSPI master <-> FLEXIO SPI slave transfer all data matched!\r\n");
    }
    else
    {
        PRINTF("Error occured in LPSPI master <-> FLEXIO SPI slave transfer!\r\n");
    }

    LPSPI_Deinit(MASTER_LPSPI_BASEADDR);
    FLEXIO_SPI_SlaveDeinit(&spiDev);

    PRINTF("\r\nEnd of Example. \r\n");

    while (1)
    {
    }
}
