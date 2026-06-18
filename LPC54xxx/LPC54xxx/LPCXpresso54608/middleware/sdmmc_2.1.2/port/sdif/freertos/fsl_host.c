/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_host.h"
#include "event.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static sdif_handle_t g_sdifHandle;
static uint32_t g_sdifDmaTable[SDIF_DMA_TABLE_WORDS];
/*******************************************************************************
 * Code
 ******************************************************************************/
/* Delay some time united in milliseconds. */
static void Delay(uint32_t milliseconds)
{
    uint32_t i;
    uint32_t j;

    for (i = 0; i < milliseconds; i++)
    {
        for (j = 0; j < 20000U; j++)
        {
            __asm("NOP");
        }
    }
}

status_t CardInsertDetect(HOST_TYPE *hostBase)
{
    /* Wait card insert */
    while (SDIF_DetectCardInsert(hostBase, false) != 0U)
        ;

    /* Enable the card power */
    SDIF_EnableCardPower(hostBase, true);

    /* Delay some time to make card stable. */
    Delay(1000U);

    return kStatus_Success;
}

/* Transfer complete callback function. */
static void SDIF_TransferCompleteCallback(SDIF_Type *base, void *handle, status_t status, void *userData)
{
    status_t waitStatus;

    /* when transfer with data, should wait data transfer success, otherwise
     should wait command success */
    if (((sdif_handle_t *)handle)->data != NULL)
    {
        /* we use DMA transfer in this example ,so wait the DMA transfer success,otherwise wait data
        transfer success*/
        waitStatus = kStatus_SDIF_DMATransferSuccess;
    }
    else
    {
        waitStatus = kStatus_SDIF_CommandTransferSuccess;
    }

    /* receive the right status, notify the event */
    if (status == waitStatus)
    {
        EVENT_Notify(kEVENT_TransferComplete);
    }
}

/* User defined transfer function. */
static status_t SDIF_TransferFunction(SDIF_Type *base, sdif_transfer_t *content)
{
    status_t error = kStatus_Success;

    sdif_dma_config_t dmaConfig;

    memset(g_sdifDmaTable, 0, sizeof(g_sdifDmaTable));
    memset(&dmaConfig, 0, sizeof(dmaConfig));

    /* user DMA mode transfer data */
    if (content->data != NULL)
    {
        dmaConfig.enableFixBurstLen = false;
        dmaConfig.mode = kSDIF_DualDMAMode;
        dmaConfig.dmaDesBufferStartAddr = g_sdifDmaTable;
        dmaConfig.dmaDesBufferLen = SDIF_DMA_TABLE_WORDS;
        dmaConfig.dmaDesSkipLen = 0U;
    }

    do
    {
        error = SDIF_TransferNonBlocking(base, &g_sdifHandle, &dmaConfig, content);
    } while (error == kStatus_SDIF_SyncCmdTimeout);

    if ((error != kStatus_Success) || (false == EVENT_Wait(kEVENT_TransferComplete, EVENT_TIMEOUT_TRANSFER_COMPLETE)))
    {
        error = kStatus_Fail;
    }

    return error;
}

status_t HOST_Init(void *host)
{
    sdif_transfer_callback_t sdifCallback = {0};
    sdif_host_t *sdifHost = (sdif_host_t *)host;

    /* Initialize SDIF. */
    sdifHost->config.endianMode = kHOST_EndianModeLittle;
    sdifHost->config.responseTimeout = 0xFFU;
    sdifHost->config.cardDetDebounce_Clock = 0xFFFFFFU;
    sdifHost->config.dataTimeout = 0xFFFFFFU;
    SDIF_Init(sdifHost->base, &(sdifHost->config));

    /* Set callback for SDHC driver. */
    sdifCallback.TransferComplete = SDIF_TransferCompleteCallback;
    /* Create handle for SDHC driver */
    SDIF_TransferCreateHandle(sdifHost->base, &g_sdifHandle, &sdifCallback, NULL);

    /* Create transfer complete event. */
    if (false == EVENT_Create(kEVENT_TransferComplete))
    {
        return kStatus_Fail;
    }

    /* Define transfer function. */
    sdifHost->transfer = SDIF_TransferFunction;

    return kStatus_Success;
}

void HOST_Deinit(void *host)
{
    sdif_host_t *sdifHost = (sdif_host_t *)host;
    SDIF_Deinit(sdifHost->base);
    EVENT_Delete(kEVENT_TransferComplete);
}
