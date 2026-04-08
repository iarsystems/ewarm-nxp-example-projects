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

#include <assert.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "fsl_srtm.h"
#include "fsl_clock.h"
#include "fsl_dmamux.h"
#include "fsl_lpi2c.h"
#include "fsl_sai_edma.h"
#include "fsl_wm8960.h"
#include "lifecycle_serv.h"
#include "audio_serv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SAI_TX_EDMA_CHANNEL                             (1U)
#define SAI_RX_EDMA_CHANNEL                             (0U)

/* Audio Service Category definition */
#define SRTM_AUDIO_CATEGORY                             (0x3U)
#define SRTM_AUDIO_VERSION                              (0x0100U)

/* Audio Service Request Command definition */
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_OPEN             (0x0U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_START            (0x1U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_PAUSE            (0x2U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_RESTART          (0x3U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_TERMINATE        (0x4U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_CLOSE            (0x5U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_SET_PARAMETER    (0x6U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_SET_BUFFER       (0x7U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_SUSPEND          (0x8U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_RESUME           (0x9U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_OPEN             (0xAU)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_START            (0xBU)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_PAUSE            (0xCU)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_RESTART          (0xDU)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_TERMINATE        (0xEU)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_CLOSE            (0xFU)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_SET_PARAMETER    (0x10U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_SET_BUFFER       (0x11U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_SUSPEND          (0x12U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_RESUME           (0x13U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RESET               (0x14U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_TX_PERIOD_DONE      (0x15U)
#define SRTM_AUDIO_SERV_REQUEST_CMD_RX_PERIOD_DONE      (0x16U)

/* Audio Service Notification Command definition */
#define SRTM_AUDIO_SERV_NOTIFICATION_CMD_TX_PERIOD_DONE (0x0U)
#define SRTM_AUDIO_SERV_NOTIFICATION_CMD_RX_PERIOD_DONE (0x1U)

/* Audio Service Sample Format definition */
#define SRTM_AUDIO_SERV_SAMPLE_FORMAT_S16_LE            (0x0U)
#define SRTM_AUDIO_SERV_SAMPLE_FORMAT_S24_LE            (0x1U)

/* Audio Service Channel identifier definition */
#define SRTM_AUDIO_SERV_CHANNEL_LEFT                    (0x0U)
#define SRTM_AUDIO_SERV_CHANNEL_RIGHT                   (0x1U)
#define SRTM_AUDIO_SERV_CHANNEL_STEREO                  (0x2U)

/* Audio Request Structure definition */
#if defined(__GNUC__)
typedef struct _audio_request
#else
typedef __packed struct _audio_request
#endif
{
    uint8_t command;
    union {
#if defined(__GNUC__)
        struct {
#else
        __packed struct {
#endif
            uint8_t index;
            uint8_t sampleFormat;
            uint8_t channels;
            uint32_t sampleRate;
            uint32_t bufferAddress;
            uint32_t bufferSize;
            uint32_t periodSize;
            uint32_t periodOffset;
#if defined(__GNUC__)
        } __attribute__((packed));
#else
        };
#endif
        uint8_t data[23U];
    };
#if defined(__GNUC__)
} __attribute__((packed)) audio_request_t;
#else
} audio_request_t;
#endif

typedef enum _AUDIO_SERV_state {
    AUDIO_SERV_StateStart = 0U,
    AUDIO_SERV_StateStop = 1U,
    AUDIO_SERV_StateRun = 2U,
} AUDIO_SERV_state_t;

struct _audio_serv
{
    srtm_handle_t *srtmHandle;
    srtm_channel_t *srtmChannel;
    uint32_t srtmChannelId;
    lifecycle_serv_t *lifecycleHandle;
    QueueHandle_t queue;
    AUDIO_SERV_state_t state;
};

/*******************************************************************************
 * Variables
 ******************************************************************************/
static lpi2c_master_handle_t i2cHandle = {0U};
static wm8960_handle_t codecHandle = {0U};
static edma_handle_t txDmaHandle = {0U};
static edma_handle_t rxDmaHandle = {0U};
static sai_edma_handle_t txHandle = {0U};
static sai_edma_handle_t rxHandle = {0U};
static uint32_t txBufferAddress;
static uint32_t currentTxAddress;
static uint32_t txBufferSize;
static uint32_t txPeriodSize;
static uint32_t rxBufferAddress;
static uint32_t currentRxAddress;
static uint32_t rxBufferSize;
static uint32_t rxPeriodSize;
static volatile bool txPauseFlag = false;
static volatile bool rxPauseFlag = false;
static bool txSuspendFlag = false;
static bool rxSuspendFlag = false;
static bool txWorking = false;
static bool rxWorking = false;
static bool txOpenedFlag = false;
static bool rxOpenedFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void I2S0_IRQHandler(void)
{
    sai_transfer_t xfer;

    if (kSAI_FIFOErrorFlag & SAI_TxGetStatusFlag(I2S0))
    {
        /* Clear FIFO error flag and Reset Tx FIFO to continue transfer */
        SAI_TxClearStatusFlags(I2S0, kSAI_FIFOErrorFlag);
        SAI_TxSoftwareReset(I2S0, kSAI_ResetTypeFIFO);

        SAI_TransferAbortSendEDMA(I2S0, &txHandle);

        xfer.data = (uint8_t *)currentTxAddress;
        xfer.dataSize = txPeriodSize;
        SAI_TransferSendEDMA(I2S0, &txHandle, &xfer);
    }

    if (kSAI_FIFOErrorFlag & SAI_RxGetStatusFlag(I2S0))
    {
        /* Clear FIFO error flag and Reset Rx FIFO to continue transfer */
        SAI_RxClearStatusFlags(I2S0, kSAI_FIFOErrorFlag);
        SAI_RxSoftwareReset(I2S0, kSAI_ResetTypeFIFO);

        SAI_TransferAbortReceiveEDMA(I2S0, &rxHandle);

        xfer.data = (uint8_t *)currentRxAddress;
        xfer.dataSize = rxPeriodSize;
        SAI_TransferReceiveEDMA(I2S0, &rxHandle, &xfer);
    }
}

static void AUDIO_SERV_RequestServiceCallback(srtm_channel_t *channel, srtm_request_t *request, void *arg)
{
    audio_serv_t *me = (audio_serv_t *)arg;
    audio_request_t remoteRequest;

    remoteRequest.command = SRTM_Request_GetCommand(request);

    SRTM_Request_GetPayload(request, remoteRequest.data, sizeof(remoteRequest.data));
    xQueueSend(me->queue, &remoteRequest, 0U);
}

static void AUDIO_SERV_LifecycleServEventCallback(lifecycle_serv_t *lifecycleHandle, uint32_t event, void *arg)
{
    audio_serv_t *me = (audio_serv_t *)arg;
    audio_request_t request;

    if (event & (LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag | LIFECYCLE_SERV_EventSwitchToRebootModeFlag |
                 LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
    {
        request.command = SRTM_AUDIO_SERV_REQUEST_CMD_RESET;
        xQueueSendToFront(me->queue, &request, 0U);
    }
    else if (event & LIFECYCLE_SERV_EventSwitchToSuspendModeFlag)
    {
        /* ADCDAT is tristated; DACLRC and BCLK are inputs;
         * ADCLRC is an input
         */
        WM8960_ModifyReg(&codecHandle, WM8960_ADDCTL2, 0x08, 0x08);
    }
    else if (event & LIFECYCLE_SERV_EventSwitchToRunModeFlag)
    {
        /* ADCDAT is an output; ADCLRC, DACLRC
         * and BCLK are inputs (slave mode) or outputs (master mode)
         */
        WM8960_ModifyReg(&codecHandle, WM8960_ADDCTL2, 0x08, 0x0);
    }
    else
    {
    }
}

static void SAI_TX_Callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    audio_request_t request;
    sai_transfer_t xfer;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    audio_serv_t *me = (audio_serv_t *)userData;

    request.command = SRTM_AUDIO_SERV_REQUEST_CMD_TX_PERIOD_DONE;
    xQueueSendToBackFromISR(me->queue, &request, &xHigherPriorityTaskWoken);

    currentTxAddress = txBufferAddress + (((currentTxAddress - txBufferAddress) + txPeriodSize) % txBufferSize);
    if (txPauseFlag != true)
    {
        xfer.data = (uint8_t *)currentTxAddress;
        xfer.dataSize = txPeriodSize;
        SAI_TransferSendEDMA(I2S0, handle, &xfer);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void SAI_RX_Callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    audio_request_t request;
    sai_transfer_t xfer;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    audio_serv_t *me = (audio_serv_t *)userData;

    request.command = SRTM_AUDIO_SERV_REQUEST_CMD_RX_PERIOD_DONE;
    xQueueSendToBackFromISR(me->queue, &request, &xHigherPriorityTaskWoken);

    currentRxAddress = rxBufferAddress + (((currentRxAddress - rxBufferAddress) + rxPeriodSize) % rxBufferSize);
    if (rxPauseFlag != true)
    {
        xfer.data = (uint8_t *)currentRxAddress;
        xfer.dataSize = rxPeriodSize;
        SAI_TransferReceiveEDMA(I2S0, handle, &xfer);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static uint32_t AUDIO_SERV_TX_Open(audio_serv_t *me)
{
    sai_config_t saiConfig;

    if (!rxOpenedFlag)
    {
        SAI_TxGetDefaultConfig(&saiConfig);
        saiConfig.syncMode = kSAI_ModeAsync;
        SAI_TxInit(I2S0, &saiConfig);
        SAI_TxEnableInterrupts(I2S0, kSAI_FIFOErrorInterruptEnable);
        SAI_TransferTxCreateHandleEDMA(I2S0, &txHandle, SAI_TX_Callback, (void *)me, &txDmaHandle);
    }

    txOpenedFlag = true;

    return 0U;
}

static uint32_t AUDIO_SERV_TX_Start(void)
{
    sai_transfer_t xfer;

    xfer.data = (uint8_t *)currentTxAddress;
    xfer.dataSize = txPeriodSize;
    SAI_TransferSendEDMA(I2S0, &txHandle, &xfer);

    txWorking = true;

    return 0U;
}

static uint32_t AUDIO_SERV_TX_Pause(void)
{
    txPauseFlag = true;

    if (txWorking == true)
    {
        SAI_TransferTerminateSendEDMA(I2S0, &txHandle);

        txWorking = false;
    }

    return 0U;
}

static uint32_t AUDIO_SERV_TX_Restart(void)
{
    sai_transfer_t xfer;

    txPauseFlag = false;

    xfer.data = (uint8_t *)currentTxAddress;
    xfer.dataSize = txPeriodSize;
    SAI_TransferSendEDMA(I2S0, &txHandle, &xfer);

    txWorking = true;

    return 0U;
}

static uint32_t AUDIO_SERV_TX_Terminate(void)
{
    if (txWorking == true)
    {
        SAI_TransferTerminateSendEDMA(I2S0, &txHandle);

        txWorking = false;
    }

    return 0U;
}

static uint32_t AUDIO_SERV_TX_Close(void)
{
    if (txOpenedFlag)
    {
        if (!rxOpenedFlag)
        {
            SAI_TxReset(I2S0);
        }

        txOpenedFlag = false;
    }

    return 0U;
}

static uint32_t AUDIO_SERV_TX_SetParameter(audio_request_t *request)
{
    sai_transfer_format_t format;

    if (request->sampleRate)
    {
        format.channel = 0U;
        format.sampleRate_Hz = request->sampleRate;
        format.masterClockHz = CLOCK_GetFreq(kCLOCK_ScgAuxPllClk);
        format.protocol = kSAI_BusLeftJustified;
        format.watermark = 15U;

        switch (request->sampleFormat)
        {
            case SRTM_AUDIO_SERV_SAMPLE_FORMAT_S16_LE:
                format.bitWidth = kSAI_WordWidth16bits;
                break;

            case SRTM_AUDIO_SERV_SAMPLE_FORMAT_S24_LE:
                format.bitWidth = kSAI_WordWidth24bits;
                break;

            default:
                break;
        }

        switch (request->channels)
        {
            case SRTM_AUDIO_SERV_CHANNEL_LEFT:
                format.stereo = kSAI_MonoLeft;
                break;

            case SRTM_AUDIO_SERV_CHANNEL_RIGHT:
                format.stereo = kSAI_MonoRight;
                break;

            case SRTM_AUDIO_SERV_CHANNEL_STEREO:
                format.stereo = kSAI_Stereo;
                break;

            default:
                break;
        }

        WM8960_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
        SAI_TransferTxSetFormatEDMA(I2S0, &txHandle, &format, format.masterClockHz, format.masterClockHz);
    }

    return 0U;
}

static uint32_t AUDIO_SERV_TX_SetBuffer(audio_request_t *request)
{
    txBufferAddress = request->bufferAddress;
    currentTxAddress = txBufferAddress + request->periodSize * request->periodOffset;
    txBufferSize = request->bufferSize;
    txPeriodSize = request->periodSize;

    return 0U;
}

static uint32_t AUDIO_SERV_TX_Suspend(void)
{
    uint32_t result = 0U;

    txSuspendFlag = true;

    result |= AUDIO_SERV_TX_Terminate();
    result |= AUDIO_SERV_TX_Close();

    return result;
}

static uint32_t AUDIO_SERV_TX_Resume(audio_serv_t *me, audio_request_t *request)
{
    uint32_t result = 0U;

    WM8960_Init(&codecHandle, NULL);
    result |= AUDIO_SERV_TX_Open(me);
    result |= AUDIO_SERV_TX_SetParameter(request);
    result |= AUDIO_SERV_TX_SetBuffer(request);

    txSuspendFlag = false;

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Open(audio_serv_t *me)
{
    sai_config_t txconfig;
    sai_config_t rxconfig;

    SAI_RxGetDefaultConfig(&rxconfig);
    SAI_RxInit(I2S0, &rxconfig);
    SAI_RxEnableInterrupts(I2S0, kSAI_FIFOErrorInterruptEnable);
    SAI_TransferRxCreateHandleEDMA(I2S0, &rxHandle, SAI_RX_Callback, (void *)me, &rxDmaHandle);

    rxOpenedFlag = true;

    if (!txOpenedFlag)
    {
        SAI_TxGetDefaultConfig(&txconfig);
        txconfig.syncMode = kSAI_ModeAsync;
        SAI_TxInit(I2S0, &txconfig);
        SAI_TxEnableInterrupts(I2S0, kSAI_FIFOErrorInterruptEnable);
        SAI_TransferTxCreateHandleEDMA(I2S0, &txHandle, SAI_TX_Callback, (void *)me, &txDmaHandle);
    }

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Start(void)
{
    sai_transfer_t xfer;

    xfer.data = (uint8_t *)currentRxAddress;
    xfer.dataSize = rxPeriodSize;
    SAI_TransferReceiveEDMA(I2S0, &rxHandle, &xfer);

    rxWorking = true;

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Pause(void)
{
    rxPauseFlag = true;

    if (rxWorking)
    {
        SAI_TransferTerminateReceiveEDMA(I2S0, &rxHandle);

        rxWorking = false;
    }

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Restart(void)
{
    sai_transfer_t xfer;

    rxPauseFlag = false;

    xfer.data = (uint8_t *)currentRxAddress;
    xfer.dataSize = rxPeriodSize;
    SAI_TransferReceiveEDMA(I2S0, &rxHandle, &xfer);

    rxWorking = true;

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Terminate(void)
{
    if (rxWorking)
    {
        SAI_TransferTerminateReceiveEDMA(I2S0, &rxHandle);

        rxWorking = false;
    }

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Close(void)
{
    if (rxOpenedFlag)
    {
        if (!txOpenedFlag)
        {
            SAI_TxReset(I2S0);
        }
        SAI_RxReset(I2S0);

        rxOpenedFlag = false;
    }

    return 0U;
}

static uint32_t AUDIO_SERV_RX_SetParameter(audio_request_t *request)
{
    sai_transfer_format_t format;

    if (request->sampleRate)
    {
        format.channel = 0U;
        format.sampleRate_Hz = request->sampleRate;
        format.masterClockHz = CLOCK_GetFreq(kCLOCK_ScgAuxPllClk);
        format.protocol = kSAI_BusLeftJustified;

        switch (request->sampleFormat)
        {
            case SRTM_AUDIO_SERV_SAMPLE_FORMAT_S16_LE:
                format.bitWidth = kSAI_WordWidth16bits;
                break;

            case SRTM_AUDIO_SERV_SAMPLE_FORMAT_S24_LE:
                format.bitWidth = kSAI_WordWidth24bits;
                break;

            default:
                break;
        }

        switch (request->channels)
        {
            case SRTM_AUDIO_SERV_CHANNEL_LEFT:
                format.stereo = kSAI_MonoLeft;
                break;

            case SRTM_AUDIO_SERV_CHANNEL_RIGHT:
                format.stereo = kSAI_MonoRight;
                break;

            case SRTM_AUDIO_SERV_CHANNEL_STEREO:
                format.stereo = kSAI_Stereo;
                break;

            default:
                break;
        }

        if (!txWorking)
        {
            WM8960_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
            format.watermark = 15U;
            SAI_TransferTxSetFormatEDMA(I2S0, &txHandle, &format, format.masterClockHz, format.masterClockHz);
        }
        format.watermark = 1U;
        SAI_TransferRxSetFormatEDMA(I2S0, &rxHandle, &format, format.masterClockHz, format.masterClockHz);
    }

    return 0U;
}

static uint32_t AUDIO_SERV_RX_SetBuffer(audio_request_t *request)
{
    rxBufferAddress = request->bufferAddress;
    currentRxAddress = rxBufferAddress + request->periodSize * request->periodOffset;
    rxBufferSize = request->bufferSize;
    rxPeriodSize = request->periodSize;

    return 0U;
}

static uint32_t AUDIO_SERV_RX_Suspend(void)
{
    uint32_t result = 0U;

    rxSuspendFlag = true;

    result |= AUDIO_SERV_RX_Terminate();
    result |= AUDIO_SERV_RX_Close();

    return result;
}

static uint32_t AUDIO_SERV_RX_Resume(audio_serv_t *me, audio_request_t *request)
{
    uint32_t result = 0U;

    WM8960_Init(&codecHandle, NULL);
    result |= AUDIO_SERV_RX_Open(me);
    result |= AUDIO_SERV_RX_SetParameter(request);
    result |= AUDIO_SERV_RX_SetBuffer(request);

    rxSuspendFlag = false;

    return 0U;
}

static uint32_t AUDIO_ProcessRemoteRequest(audio_serv_t *me, audio_request_t *request)
{
    uint32_t result = 0U;

    switch (request->command)
    {
        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_OPEN:
            result = AUDIO_SERV_TX_Open(me);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_START:
            result = AUDIO_SERV_TX_Start();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_PAUSE:
            result = AUDIO_SERV_TX_Pause();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_RESTART:
            result = AUDIO_SERV_TX_Restart();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_TERMINATE:
            result = AUDIO_SERV_TX_Terminate();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_CLOSE:
            result = AUDIO_SERV_TX_Close();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_SET_PARAMETER:
            result = AUDIO_SERV_TX_SetParameter(request);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_SET_BUFFER:
            result = AUDIO_SERV_TX_SetBuffer(request);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_SUSPEND:
            result = AUDIO_SERV_TX_Suspend();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_TX_RESUME:
            result = AUDIO_SERV_TX_Resume(me, request);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_OPEN:
            result = AUDIO_SERV_RX_Open(me);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_START:
            result = AUDIO_SERV_RX_Start();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_PAUSE:
            result = AUDIO_SERV_RX_Pause();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_RESTART:
            result = AUDIO_SERV_RX_Restart();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_TERMINATE:
            result = AUDIO_SERV_RX_Terminate();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_CLOSE:
            result = AUDIO_SERV_RX_Close();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_SET_PARAMETER:
            result = AUDIO_SERV_RX_SetParameter(request);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_SET_BUFFER:
            result = AUDIO_SERV_RX_SetBuffer(request);
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_SUSPEND:
            result = AUDIO_SERV_RX_Suspend();
            break;

        case SRTM_AUDIO_SERV_REQUEST_CMD_RX_RESUME:
            result = AUDIO_SERV_RX_Resume(me, request);
            break;

        default:
            break;
    }

    return result;
}

static void AUDIO_SERV_Start(audio_serv_t *me)
{
    me->srtmChannel = SRTM_Channel_Create(me->srtmHandle, me->srtmChannelId);
    if (!me->srtmChannel)
    {
        assert(false);
    }
    if (SRTM_SUCCESS != SRTM_Channel_RegisterService(me->srtmChannel, SRTM_AUDIO_CATEGORY,
                                                     AUDIO_SERV_RequestServiceCallback, (void *)me))
    {
        assert(false);
    }

    me->state = AUDIO_SERV_StateRun;
}

static void AUDIO_SERV_Stop(audio_serv_t *me)
{
    SRTM_Channel_Destroy(me->srtmChannel);

    vTaskDelay(200U);
    me->state = AUDIO_SERV_StateStart;
}

static void AUDIO_SERV_Run(audio_serv_t *me)
{
    audio_request_t request;
    srtm_response_t *response;
    srtm_notification_t *notification;
    uint32_t result;

    for (;;)
    {
        xQueueReceive(me->queue, &request, portMAX_DELAY);
        if (request.command <= SRTM_AUDIO_SERV_REQUEST_CMD_RX_RESUME)
        {
            result = AUDIO_ProcessRemoteRequest(me, &request);
            response = SRTM_Response_Create(me->srtmChannel, SRTM_AUDIO_CATEGORY, SRTM_AUDIO_VERSION,
                                            request.command, 2U);
            if (!response)
            {
                assert(false);
            }

            request.data[0U] = 0x0U;
            if (0U == result)
            {
                request.data[1U] = 0x0U;
            }
            else
            {
                request.data[1U] = 0x1U;
            }
            SRTM_Response_SetPayload(response, request.data, 2U);
            SRTM_Response_Deliver(response);
        }
        else if (SRTM_AUDIO_SERV_REQUEST_CMD_TX_PERIOD_DONE == request.command)
        {
            if (!txSuspendFlag)
            {
                notification = SRTM_Notification_Create(me->srtmChannel, SRTM_AUDIO_CATEGORY, SRTM_AUDIO_VERSION,
                                                        SRTM_AUDIO_SERV_NOTIFICATION_CMD_TX_PERIOD_DONE, 1U);
                if (!notification)
                {
                    assert(false);
                }
                request.data[0U] = 0U;
                SRTM_Notification_SetPayload(notification, request.data, 1U);
                SRTM_Notification_Deliver(notification);
            }
        }
        else if (SRTM_AUDIO_SERV_REQUEST_CMD_RX_PERIOD_DONE == request.command)
        {
            if (!rxSuspendFlag)
            {
                notification = SRTM_Notification_Create(me->srtmChannel, SRTM_AUDIO_CATEGORY, SRTM_AUDIO_VERSION,
                                                        SRTM_AUDIO_SERV_NOTIFICATION_CMD_RX_PERIOD_DONE, 1U);
                if (!notification)
                {
                    assert(false);
                }
                request.data[0U] = 0U;
                SRTM_Notification_SetPayload(notification, request.data, 1U);
                SRTM_Notification_Deliver(notification);
            }
        }
        else
        {
            me->state = AUDIO_SERV_StateStop;
            break;
        }
    }
}

audio_serv_t *AUDIO_REMOTE_SERV_Init(const audio_serv_config_t *config)
{
    audio_serv_t *me;

    me = pvPortMalloc(sizeof(audio_serv_t));
    if (!me)
    {
        return NULL;
    }
    me->queue = xQueueCreate(config->queueSize, sizeof(audio_request_t));
    if (!me->queue)
    {
        vPortFree(me);

        return NULL;
    }

    me->srtmHandle = config->srtmHandle;
    me->srtmChannel = NULL;
    me->srtmChannelId = config->srtmChannelId;
    me->lifecycleHandle = config->lifecycleHandle;

    if (0U != LIFECYCLE_SERV_Subscribe(me->lifecycleHandle, AUDIO_SERV_LifecycleServEventCallback, (void *)me))
    {
        assert(false);
    }

    me->state = AUDIO_SERV_StateStart;

    return me;
}

void AUDIO_SERV_ProcessTask(void *pvParameters)
{
    audio_serv_t *me = (audio_serv_t *)pvParameters;
    lpi2c_master_config_t i2cConfig = {0U};
    edma_config_t dmaConfig = {0U};

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DMA0, &dmaConfig);
    EDMA_CreateHandle(&rxDmaHandle, DMA0, SAI_RX_EDMA_CHANNEL);
    EDMA_CreateHandle(&txDmaHandle, DMA0, SAI_TX_EDMA_CHANNEL);

    DMAMUX_Init(DMA_CH_MUX0);
    DMAMUX_SetSource(DMA_CH_MUX0, SAI_RX_EDMA_CHANNEL, kDmaRequestMux0SAI0Rx);
    DMAMUX_SetSource(DMA_CH_MUX0, SAI_TX_EDMA_CHANNEL, kDmaRequestMux0SAI0Tx);
    DMAMUX_EnableChannel(DMA_CH_MUX0, SAI_TX_EDMA_CHANNEL);
    DMAMUX_EnableChannel(DMA_CH_MUX0, SAI_RX_EDMA_CHANNEL);

    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(LPI2C0, &i2cConfig, CLOCK_GetIpFreq(kCLOCK_Lpi2c0));
    LPI2C_MasterTransferCreateHandle(LPI2C0, &i2cHandle, NULL, NULL);

    /* Configure WM8960 I2C */
    codecHandle.base = LPI2C0;
    codecHandle.i2cHandle = &i2cHandle;
    WM8960_Init(&codecHandle, NULL);

    NVIC_SetPriority(DMA0_0_4_IRQn, 6U);
    NVIC_SetPriority(DMA0_1_5_IRQn, 6U);
    NVIC_SetPriority(I2S0_IRQn, 6U);
    EnableIRQ(I2S0_IRQn);

    for (;;)
    {
        switch (me->state)
        {
            case AUDIO_SERV_StateStart:
                AUDIO_SERV_Start(me);
                break;

            case AUDIO_SERV_StateStop:
                AUDIO_SERV_Stop(me);
                break;

            case AUDIO_SERV_StateRun:
                AUDIO_SERV_Run(me);
                break;

            default:
                break;
        }
    }
}
