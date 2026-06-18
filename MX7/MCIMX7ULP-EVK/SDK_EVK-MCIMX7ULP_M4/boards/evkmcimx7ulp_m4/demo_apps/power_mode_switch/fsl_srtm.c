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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "FreeRTOS.h"
#include "list.h"
#include "queue.h"
#include "semphr.h"
#include "fsl_srtm.h"
#include "fsl_srtm_rpmsg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SRTM_MESSAGE_TYPE_REQUEST (0x0U)
#define SRTM_MESSAGE_TYPE_RESPONSE (0x1U)
#define SRTM_MESSAGE_TYPE_NOTIFICATION (0x2U)

typedef void (*srtm_receive_cb_fun_t)(void *handle, uint8_t *payload, uint8_t payloadLen, void *arg);
typedef void *(*srtm_create_fun_t)(const void *config, srtm_receive_cb_fun_t callback, void *arg, bool wakeup);
typedef void (*srtm_destroy_fun_t)(void *handle);
typedef int32_t (*srtm_is_ready_fun_t)(void *handle);
typedef int32_t (*srtm_send_fun_t)(void *handle, uint8_t *data, uint32_t size);
typedef int32_t (*srtm_release_rx_buf_fun_t)(void *handle, void *rxBuffer);
typedef void (*srtm_suspend_fun_t)(void *handle);
typedef void (*srtm_resume_fun_t)(void *handle);

typedef enum _strm_process_event_type {
    SRTM_ProcessEventTypeRxRequest = 0x0U,
    SRTM_ProcessEventTypeRxResponse = 0x1U,
    SRTM_ProcessEventTypeRxNotification = 0x2U,
    SRTM_ProcessEventTypeRxUnsupported = 0x3U,
    SRTM_ProcessEventTypeTxRequest = 0x4U,
    SRTM_ProcessEventTypeTxResponse = 0x5U,
    SRTM_ProcessEventTypeTxNotification = 0x6U,
    SRTM_ProcessEventTypeChannelRegister = 0x7U,
    SRTM_ProcessEventTypeChannelUnregister = 0x8U,
} strm_process_event_type_t;

#if defined(__GNUC__)
typedef struct _srtm_packet_head
#else
typedef __packed struct _srtm_packet_head
#endif
{
    union {
        struct
        {
            uint8_t category;
            uint8_t majorVersion;
            uint8_t minorVersion;
            uint8_t type;
            uint8_t command;
            uint8_t reserved[5U];
        };
        uint8_t header[10U];
    };
#if defined(__GNUC__)
} __attribute__((packed)) srtm_packet_head_t;
#else
} srtm_packet_head_t;
#endif

typedef struct _strm_process_event
{
    strm_process_event_type_t type;
    srtm_channel_t *channel;
    uint8_t *payload;
    uint32_t payloadLen;
} strm_process_event_t;

typedef struct _srtm_operator
{
    srtm_create_fun_t create;
    srtm_destroy_fun_t destroy;
    srtm_is_ready_fun_t isReady;
    srtm_send_fun_t send;
    srtm_release_rx_buf_fun_t releaseRxBuffer;
    srtm_suspend_fun_t suspend;
    srtm_resume_fun_t resume;
} srtm_operator_t;

typedef struct _srtm_request_service
{
    ListItem_t listItem;
    srtm_channel_request_service_cb_t callback;
    void *arg;
    uint8_t category;
} srtm_request_service_t;

typedef struct _srtm_notification_service
{
    ListItem_t listItem;
    srtm_channel_notification_service_cb_t callback;
    void *arg;
    uint8_t category;
    uint8_t command;
} srtm_notification_service_t;

struct _srtm_request
{
    ListItem_t listItem;
    SemaphoreHandle_t semaphore;
    srtm_channel_t *channel;
    uint8_t *payload;
    uint32_t payloadLen;
    srtm_request_cb_t callback;
    void *arg;
    srtm_response_t *response;
    bool responseReceived;
};

struct _srtm_response
{
    ListItem_t listItem;
    srtm_channel_t *channel;
    uint8_t *payload;
    uint32_t payloadLen;
};

struct _srtm_notification
{
    ListItem_t listItem;
    srtm_channel_t *channel;
    uint8_t *payload;
    uint32_t payloadLen;
};

struct _srtm_handle
{
    List_t channelList;
    srtm_channel_config_t *channelConfigs;
    uint32_t channelCount;
    QueueHandle_t queue;
    SemaphoreHandle_t lock;
    bool resumeFlag;
    uint32_t resumeCount;
};

struct _srtm_channel
{
    List_t requestServList;
    List_t notificationServList;
    List_t requestList;
    List_t responseList;
    List_t notificationList;
    ListItem_t listItem;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t semaphore;
    srtm_handle_t *handle;
    uint32_t id;
    void *deviceHandle;
    srtm_operator_t operators;
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void srtm_receive_callback(void *handle, uint8_t *payload, uint8_t payloadLen, void *arg)
{
    BaseType_t reschedule = pdFALSE;
    strm_process_event_t event;
    srtm_packet_head_t *packetHead;

    assert(handle);
    assert(payload);

    /* Copy packet header into event. */
    event.payload = payload;
    event.payloadLen = payloadLen;

    packetHead = (srtm_packet_head_t *)event.payload;
    switch (packetHead->type)
    {
        case SRTM_MESSAGE_TYPE_REQUEST:
            event.type = SRTM_ProcessEventTypeRxRequest;
            break;

        case SRTM_MESSAGE_TYPE_RESPONSE:
            event.type = SRTM_ProcessEventTypeRxResponse;
            break;

        case SRTM_MESSAGE_TYPE_NOTIFICATION:
            event.type = SRTM_ProcessEventTypeRxNotification;
            break;

        default:
            event.type = SRTM_ProcessEventTypeRxUnsupported;
            break;
    }
    event.channel = (srtm_channel_t *)arg;
    xQueueSendToBackFromISR(event.channel->handle->queue, &event, &reschedule);

    portYIELD_FROM_ISR(reschedule);
}

static bool SRTM_IsChannelValid(srtm_handle_t *handle, srtm_channel_t *channel)
{
    bool result = false;
    ListItem_t *listItem;

    xSemaphoreTake(handle->lock, portMAX_DELAY);
    for (listItem = listGET_HEAD_ENTRY(&handle->channelList); listItem != listGET_END_MARKER(&handle->channelList);
         listItem = listGET_NEXT(listItem))
    {
        if (channel == (srtm_channel_t *)listGET_LIST_ITEM_VALUE(listItem))
        {
            result = true;
            break;
        }
    }
    xSemaphoreGive(handle->lock);

    return result;
}

static void SRTM_ProcessRxRequest(srtm_channel_t *channel, uint8_t *buffer, uint32_t size)
{
    ListItem_t *listItem;
    srtm_request_service_t *service;
    srtm_request_t request;

    assert(channel);
    assert(buffer);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    for (listItem = listGET_HEAD_ENTRY(&channel->requestServList);
         listItem != listGET_END_MARKER(&channel->requestServList); listItem = listGET_NEXT(listItem))
    {
        service = (srtm_request_service_t *)listGET_LIST_ITEM_VALUE(listItem);
        if (service->category == buffer[0U])
        {
            request.channel = channel;
            request.payload = buffer;
            request.payloadLen = size;

            service->callback(channel, &request, service->arg);
            break;
        }
    }
    channel->operators.releaseRxBuffer(channel->deviceHandle, buffer);
    xSemaphoreGive(channel->lock);
}

static void SRTM_ProcessRxResponse(srtm_channel_t *channel, uint8_t *buffer, uint32_t size)
{
    ListItem_t *listItem;
    srtm_request_t *request;
    srtm_response_t response;
    srtm_packet_head_t *packetHead;

    assert(channel);
    assert(buffer);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    for (listItem = listGET_HEAD_ENTRY(&channel->requestList); listItem != listGET_END_MARKER(&channel->requestList);
         listItem = listGET_NEXT(listItem))
    {
        request = (srtm_request_t *)listGET_LIST_ITEM_VALUE(listItem);
        packetHead = (srtm_packet_head_t *)request->payload;

        if ((packetHead->category == buffer[0U]) && (packetHead->command == buffer[4U]))
        {
            /* Un-register request from channel's request list. */
            xSemaphoreTake(request->channel->lock, portMAX_DELAY);
            uxListRemove(&request->listItem);
            xSemaphoreGive(request->channel->lock);

            request->responseReceived = true;
            /* Process Sync calling. */
            if (!request->callback)
            {
                request->response->channel = channel;
                request->response->payload = buffer;
                request->response->payloadLen = size;

                xSemaphoreGive(request->semaphore);
            }
            /* Process Async calling. */
            else
            {
                response.channel = channel;
                request->response->payload = buffer;
                request->response->payloadLen = size;
                request->callback(request, &response, request->arg);

                SRTM_Request_Destroy(request);
            }

            break;
        }
    }
    channel->operators.releaseRxBuffer(channel->deviceHandle, buffer);
    xSemaphoreGive(channel->lock);
}

static void SRTM_ProcessRxNotification(srtm_channel_t *channel, uint8_t *buffer, uint32_t size)
{
    ListItem_t *listItem;
    srtm_notification_service_t *service;
    srtm_notification_t notification;

    assert(channel);
    assert(buffer);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    for (listItem = listGET_HEAD_ENTRY(&channel->notificationServList);
         listItem != listGET_END_MARKER(&channel->notificationServList); listItem = listGET_NEXT(listItem))
    {
        service = (srtm_notification_service_t *)listGET_LIST_ITEM_VALUE(listItem);

        if ((service->category == buffer[0U]) && (service->command == buffer[4U]))
        {
            notification.channel = channel;
            notification.payload = buffer;
            notification.payloadLen = size;
            service->callback(channel, &notification, service->arg);

            break;
        }
    }
    channel->operators.releaseRxBuffer(channel->deviceHandle, buffer);
    xSemaphoreGive(channel->lock);
}

static void SRTM_ProcessTxRequest(srtm_channel_t *channel, uint8_t *buffer, uint32_t size)
{
    assert(channel);
    assert(buffer);

    channel->operators.send(channel->deviceHandle, buffer, size);
}

static void SRTM_ProcessTxResponse(srtm_channel_t *channel, uint8_t *buffer, uint32_t size)
{
    ListItem_t *listItem;
    srtm_response_t *response;
    srtm_packet_head_t *packetHead;

    assert(channel);
    assert(buffer);

    channel->operators.send(channel->deviceHandle, buffer, size);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    for (listItem = listGET_HEAD_ENTRY(&channel->responseList); listItem != listGET_END_MARKER(&channel->responseList);
         listItem = listGET_NEXT(listItem))
    {
        response = (srtm_response_t *)listGET_LIST_ITEM_VALUE(listItem);
        packetHead = (srtm_packet_head_t *)response->payload;

        if ((packetHead->category == buffer[0U]) && (packetHead->command == buffer[4U]))
        {
            uxListRemove(&response->listItem);
            SRTM_Response_Destroy(response);

            break;
        }
    }
    xSemaphoreGive(channel->lock);
}

static void SRTM_ProcessTxNotification(srtm_channel_t *channel, uint8_t *buffer, uint32_t size)
{
    ListItem_t *listItem;
    srtm_notification_t *notification;
    srtm_packet_head_t *packetHead;

    assert(channel);
    assert(buffer);

    channel->operators.send(channel->deviceHandle, buffer, size);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    for (listItem = listGET_HEAD_ENTRY(&channel->notificationList);
         listItem != listGET_END_MARKER(&channel->notificationList); listItem = listGET_NEXT(listItem))
    {
        notification = (srtm_notification_t *)listGET_LIST_ITEM_VALUE(listItem);
        packetHead = (srtm_packet_head_t *)notification->payload;

        if ((packetHead->category == buffer[0U]) && (packetHead->command == buffer[4U]))
        {
            uxListRemove(&notification->listItem);
            SRTM_Notification_Destroy(notification);

            break;
        }
    }
    xSemaphoreGive(channel->lock);
}

static void SRTM_ProcessChannelRegister(srtm_channel_t *channel)
{
    xSemaphoreTake(channel->lock, portMAX_DELAY);
    vListInitialiseItem(&channel->listItem);
    listSET_LIST_ITEM_VALUE(&channel->listItem, (uint32_t)channel);
    vListInsertEnd(&channel->handle->channelList, &channel->listItem);
    xSemaphoreGive(channel->lock);

    if (true == channel->handle->resumeFlag)
    {
        channel->handle->resumeCount++;
        if (channel->handle->resumeCount == channel->handle->channelCount)
        {
            channel->handle->resumeFlag = false;
        }
    }

    xSemaphoreGive(channel->semaphore);
}

static void SRTM_ProcessChannelUnregister(srtm_channel_t *channel)
{
    xSemaphoreTake(channel->lock, portMAX_DELAY);
    uxListRemove(&channel->listItem);
    xSemaphoreGive(channel->lock);

    xSemaphoreGive(channel->semaphore);
}

srtm_handle_t *SRTM_Init(const srtm_config_t *config)
{
    srtm_handle_t *handle;
    assert(config);

    handle = pvPortMalloc(sizeof(srtm_handle_t));
    if (!handle)
    {
        return NULL;
    }

    handle->queue = xQueueCreate(config->queueSize, sizeof(strm_process_event_t));
    if (!handle->queue)
    {
        vPortFree(handle);

        return NULL;
    }

    handle->lock = xSemaphoreCreateMutex();
    if (!handle->lock)
    {
        vQueueDelete(handle->queue);
        vPortFree(handle);

        return NULL;
    }

    vListInitialise(&handle->channelList);
    handle->channelCount = config->channelCount;
    handle->channelConfigs = config->channelConfigs;

    handle->resumeFlag = false;

    return handle;
}

void SRTM_Suspend(srtm_handle_t *handle)
{
    ListItem_t *listItem;
    srtm_channel_t *channel;

    assert(handle);

    for (listItem = listGET_HEAD_ENTRY(&handle->channelList);
         listItem != listGET_END_MARKER(&handle->channelList); listItem = listGET_NEXT(listItem))
    {
        channel = (srtm_channel_t *)listGET_LIST_ITEM_VALUE(listItem);
        channel->operators.suspend(channel->deviceHandle);
    }
}

void SRTM_Resume(srtm_handle_t *handle)
{
    assert(handle);

    handle->resumeFlag = true;
    handle->resumeCount = 0U;
}

void SRTM_ProcessTask(void *pvParameters)
{
    strm_process_event_t event;
    srtm_handle_t *handle = (srtm_handle_t *)pvParameters;

    assert(handle);

    for (;;)
    {
        xQueueReceive(handle->queue, &event, portMAX_DELAY);
        switch (event.type)
        {
            case SRTM_ProcessEventTypeRxRequest:
                if (SRTM_IsChannelValid(handle, event.channel))
                {
                    SRTM_ProcessRxRequest(event.channel, event.payload, event.payloadLen);
                }
                break;

            case SRTM_ProcessEventTypeRxResponse:
                if (SRTM_IsChannelValid(handle, event.channel))
                {
                    SRTM_ProcessRxResponse(event.channel, event.payload, event.payloadLen);
                }
                break;

            case SRTM_ProcessEventTypeRxNotification:
                if (SRTM_IsChannelValid(handle, event.channel))
                {
                    SRTM_ProcessRxNotification(event.channel, event.payload, event.payloadLen);
                }
                break;

            case SRTM_ProcessEventTypeTxRequest:
                if (SRTM_IsChannelValid(handle, event.channel))
                {
                    SRTM_ProcessTxRequest(event.channel, event.payload, event.payloadLen);
                }
                break;

            case SRTM_ProcessEventTypeTxResponse:
                if (SRTM_IsChannelValid(handle, event.channel))
                {
                    SRTM_ProcessTxResponse(event.channel, event.payload, event.payloadLen);
                }
                break;

            case SRTM_ProcessEventTypeTxNotification:
                if (SRTM_IsChannelValid(handle, event.channel))
                {
                    SRTM_ProcessTxNotification(event.channel, event.payload, event.payloadLen);
                }
                break;

            case SRTM_ProcessEventTypeChannelRegister:
                SRTM_ProcessChannelRegister(event.channel);
                break;

            case SRTM_ProcessEventTypeChannelUnregister:
                SRTM_ProcessChannelUnregister(event.channel);
                break;

            default:
                break;
        }
    }
}

srtm_channel_t *SRTM_Channel_Create(srtm_handle_t *handle, uint32_t id)
{
    srtm_channel_t *channel;
    strm_process_event_t event;

    assert(handle);

    if (id >= handle->channelCount)
    {
        return NULL;
    }

    channel = pvPortMalloc(sizeof(srtm_channel_t));
    if (!channel)
    {
        return NULL;
    }

    channel->semaphore = xSemaphoreCreateBinary();
    if (!channel->semaphore)
    {
        vPortFree(channel);

        return NULL;
    }

    channel->lock = xSemaphoreCreateMutex();
    if (!channel->lock)
    {
        vSemaphoreDelete(channel->semaphore);
        vPortFree(channel);

        return NULL;
    }

    channel->handle = handle;
    channel->id = id;
    vListInitialise(&channel->requestServList);
    vListInitialise(&channel->notificationServList);
    vListInitialise(&channel->requestList);
    vListInitialise(&channel->responseList);
    vListInitialise(&channel->notificationList);

    switch (channel->handle->channelConfigs[id].channelType)
    {
        case SRTM_CHANNEL_TYPE_RPMSG_REMOTE:
            channel->operators.create = (srtm_create_fun_t)SRTM_RPMSG_Remote_Create;
            channel->operators.destroy = (srtm_destroy_fun_t)SRTM_RPMSG_Remote_Destroy;
            channel->operators.isReady = (srtm_is_ready_fun_t)SRTM_RPMSG_Remote_IsReady;
            channel->operators.send = (srtm_send_fun_t)SRTM_RPMSG_Remote_Send;
            channel->operators.releaseRxBuffer = (srtm_release_rx_buf_fun_t)SRTM_RPMSG_Remote_ReleaseRxBuffer;
            channel->operators.suspend = (srtm_suspend_fun_t)SRTM_RPMSG_Remote_Suspend;
            channel->operators.resume = (srtm_resume_fun_t)SRTM_RPMSG_Remote_Resume;
            break;

        default:
            break;
    }

    if (handle->resumeFlag)
    {
        channel->deviceHandle =
            channel->operators.create(handle->channelConfigs[id].driverConfig, srtm_receive_callback, channel, true);
        channel->operators.resume(channel->deviceHandle);
    }
    else
    {
        channel->deviceHandle =
            channel->operators.create(handle->channelConfigs[id].driverConfig, srtm_receive_callback, channel, false);
    }

    if (!channel->deviceHandle)
    {
        vSemaphoreDelete(channel->lock);
        vSemaphoreDelete(channel->semaphore);
        vPortFree(channel);

        return NULL;
    }

    xSemaphoreTake(channel->semaphore, 0U);
    event.type = SRTM_ProcessEventTypeChannelRegister;
    event.channel = channel;
    xQueueSendToBack(channel->handle->queue, &event, portMAX_DELAY);
    xSemaphoreTake(channel->semaphore, portMAX_DELAY);

    return channel;
}

void SRTM_Channel_Destroy(srtm_channel_t *channel)
{
    ListItem_t *listItem;
    strm_process_event_t event;
    srtm_request_service_t *requestService;
    srtm_notification_service_t *notificationService;
    srtm_request_t *request;
    srtm_response_t *response;
    srtm_notification_t *notification;

    assert(channel);

    xSemaphoreTake(channel->semaphore, 0U);
    event.type = SRTM_ProcessEventTypeChannelUnregister;
    event.channel = channel;
    xQueueSendToBack(channel->handle->queue, &event, portMAX_DELAY);
    xSemaphoreTake(channel->semaphore, portMAX_DELAY);

    /* Deinit underlying hardware. */
    channel->operators.destroy(channel->deviceHandle);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    /* Clean all request service. */
    for (listItem = listGET_HEAD_ENTRY(&channel->requestServList);
         listItem != listGET_END_MARKER(&channel->requestServList); listItem = listGET_NEXT(listItem))
    {
        requestService = (srtm_request_service_t *)listGET_LIST_ITEM_VALUE(listItem);
        vPortFree(requestService);
        uxListRemove(listItem);
    }
    /* Clean all notification service. */
    for (listItem = listGET_HEAD_ENTRY(&channel->notificationServList);
         listItem != listGET_END_MARKER(&channel->notificationServList); listItem = listGET_NEXT(listItem))
    {
        notificationService = (srtm_notification_service_t *)listGET_LIST_ITEM_VALUE(listItem);
        vPortFree(notificationService);
        uxListRemove(listItem);
    }
    /* Clean all request. */
    for (listItem = listGET_HEAD_ENTRY(&channel->requestList); listItem != listGET_END_MARKER(&channel->requestList);
         listItem = listGET_NEXT(listItem))
    {
        request = (srtm_request_t *)listGET_LIST_ITEM_VALUE(listItem);
        request->responseReceived = false;
        if (!request->callback)
        {
            /* Release all request semaphore and let user to destory. */
            xSemaphoreGive(request->semaphore);
        }
        else
        {
            /* Destory Request. */
            SRTM_Request_Destroy(request);
        }
        uxListRemove(listItem);
    }
    /* Clean all response. */
    for (listItem = listGET_HEAD_ENTRY(&channel->responseList); listItem != listGET_END_MARKER(&channel->responseList);
         listItem = listGET_NEXT(listItem))
    {
        response = (srtm_response_t *)listGET_LIST_ITEM_VALUE(listItem);
        SRTM_Response_Destroy(response);
        uxListRemove(listItem);
    }
    /* Clean all notification. */
    for (listItem = listGET_HEAD_ENTRY(&channel->notificationList);
         listItem != listGET_END_MARKER(&channel->notificationList); listItem = listGET_NEXT(listItem))
    {
        notification = (srtm_notification_t *)listGET_LIST_ITEM_VALUE(listItem);
        SRTM_Notification_Destroy(notification);
        uxListRemove(listItem);
    }
    xSemaphoreGive(channel->lock);

    vSemaphoreDelete(channel->lock);
    vSemaphoreDelete(channel->semaphore);

    vPortFree(channel);
}

int32_t SRTM_Channel_IsReady(srtm_channel_t *channel)
{
    assert(channel);

    return channel->operators.isReady(channel->deviceHandle);
}

int32_t SRTM_Channel_RegisterService(srtm_channel_t *channel,
                                     uint8_t category,
                                     srtm_channel_request_service_cb_t callback,
                                     void *arg)
{
    srtm_request_service_t *service;

    assert(channel);
    assert(callback);

    service = pvPortMalloc(sizeof(srtm_request_service_t));
    if (!service)
    {
        return SRTM_ERROR_MEM_ALLOC;
    }

    service->category = category;
    service->callback = callback;
    service->arg = arg;
    vListInitialiseItem(&service->listItem);
    listSET_LIST_ITEM_VALUE(&service->listItem, (uint32_t)service);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    vListInsertEnd(&channel->requestServList, &service->listItem);
    xSemaphoreGive(channel->lock);

    return SRTM_SUCCESS;
}

int32_t SRTM_Channel_RegisterNotification(srtm_channel_t *channel,
                                          uint8_t category,
                                          uint8_t command,
                                          srtm_channel_notification_service_cb_t callback,
                                          void *arg)
{
    srtm_notification_service_t *service;

    assert(channel);
    assert(callback);

    service = pvPortMalloc(sizeof(srtm_notification_service_t));
    if (!service)
    {
        return SRTM_ERROR_MEM_ALLOC;
    }

    service->category = category;
    service->command = command;
    service->callback = callback;
    service->arg = arg;

    vListInitialiseItem(&service->listItem);
    listSET_LIST_ITEM_VALUE(&service->listItem, (uint32_t)service);

    xSemaphoreTake(channel->lock, portMAX_DELAY);
    vListInsertEnd(&channel->notificationServList, &service->listItem);
    xSemaphoreGive(channel->lock);

    return SRTM_SUCCESS;
}

srtm_request_t *SRTM_Request_Create(
    srtm_channel_t *channel, uint8_t category, uint16_t version, uint8_t command, uint8_t payloadLen)
{
    srtm_packet_head_t *packetHead;
    srtm_request_t *request;

    assert(channel);

    request = pvPortMalloc(sizeof(srtm_request_t));
    if (!request)
    {
        return NULL;
    }
    memset(request, 0x0U, sizeof(srtm_request_t));

    request->payloadLen = sizeof(srtm_packet_head_t) + payloadLen;
    request->payload = pvPortMalloc(request->payloadLen);
    if (!request->payload)
    {
        vPortFree(request);

        return NULL;
    }
    memset(request->payload, 0x0U, request->payloadLen);

    request->semaphore = xSemaphoreCreateBinary();
    if (!request->semaphore)
    {
        vPortFree(request->payload);
        vPortFree(request);

        return NULL;
    }

    vListInitialiseItem(&request->listItem);
    listSET_LIST_ITEM_VALUE(&request->listItem, (uint32_t)request);

    request->channel = channel;
    packetHead = (srtm_packet_head_t *)request->payload;
    packetHead->category = category;
    packetHead->majorVersion = (uint8_t)((version & 0xFF00U) >> 8U);
    packetHead->minorVersion = (uint8_t)(version & 0xFFU);
    packetHead->type = SRTM_MESSAGE_TYPE_REQUEST;
    packetHead->command = command;

    return request;
}

void SRTM_Request_Destroy(srtm_request_t *request)
{
    assert(request);

    vSemaphoreDelete(request->semaphore);
    vPortFree(request->payload);
    vPortFree(request);
}

uint8_t SRTM_Request_GetCategory(srtm_request_t *request)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)request->payload;

    assert(request);

    return packetHead->category;
}

uint16_t SRTM_Request_GetVersion(srtm_request_t *request)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)request->payload;

    assert(request);

    return (uint16_t)(((uint16_t)packetHead->majorVersion << 8U) + packetHead->minorVersion);
}

uint8_t SRTM_Request_GetCommand(srtm_request_t *request)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)request->payload;

    assert(request);

    return packetHead->command;
}

void SRTM_Request_GetPayload(srtm_request_t *request, uint8_t *payload, uint8_t payloadLen)
{
    assert(request);
    assert(payload);

    memcpy(payload, &request->payload[sizeof(srtm_packet_head_t)], payloadLen);
}

void SRTM_Request_SetPayload(srtm_request_t *request, uint8_t *payload, uint8_t payloadLen)
{
    assert(request);
    assert(payload);

    memcpy(&request->payload[sizeof(srtm_packet_head_t)], payload, payloadLen);
}

uint32_t SRTM_Request_GetPayloadLen(srtm_request_t *request)
{
    assert(request);

    return request->payloadLen - sizeof(srtm_packet_head_t);
}

int32_t SRTM_Request_Send(srtm_request_t *request, srtm_response_t *response, uint32_t timeout)
{
    int32_t result;
    strm_process_event_t event;

    assert(request);
    assert(response);

    request->response = response;
    request->responseReceived = false;
    request->callback = NULL;
    request->arg = NULL;

    /* Register request into channel's request list. */
    xSemaphoreTake(request->channel->lock, portMAX_DELAY);
    vListInsertEnd(&request->channel->requestList, &request->listItem);

    /* Send out request. */
    event.type = SRTM_ProcessEventTypeTxRequest;
    event.payload = request->payload;
    event.payloadLen = request->payloadLen;
    event.channel = request->channel;
    result = xQueueSendToBack(request->channel->handle->queue, &event, timeout);
    xSemaphoreGive(request->channel->lock);

    if (pdTRUE == result)
    {
        request->responseReceived = false;

        /* Waiting for response from remote. */
        result = xSemaphoreTake(request->semaphore, timeout);

        if (request->responseReceived)
        {
            xSemaphoreGive(request->semaphore);

            result = SRTM_SUCCESS;
        }
        else
        {
            result = SRTM_FAIL;
        }
    }
    else
    {
        /* Un-register request from channel's request list. */
        xSemaphoreTake(request->channel->lock, portMAX_DELAY);
        uxListRemove(&request->listItem);
        xSemaphoreGive(request->channel->lock);

        result = SRTM_FAIL;
    }

    return result;
}

int32_t SRTM_Request_Deliver(srtm_request_t *request, srtm_request_cb_t callback, void *arg)
{
    int32_t result;
    strm_process_event_t event;

    assert(request);
    assert(callback);

    request->callback = callback;
    request->arg = arg;

    /* Register request into channel's request list. */
    xSemaphoreTake(request->channel->lock, portMAX_DELAY);
    vListInsertEnd(&request->channel->requestList, &request->listItem);

    /* Send out request. */
    event.type = SRTM_ProcessEventTypeTxRequest;
    event.payload = request->payload;
    event.payloadLen = request->payloadLen;
    event.channel = request->channel;
    result = xQueueSendToBack(request->channel->handle->queue, &event, 0U);
    xSemaphoreGive(request->channel->lock);

    if (pdTRUE == result)
    {
        result = SRTM_SUCCESS;
    }
    else
    {
        result = SRTM_FAIL;
    }

    return result;
}

srtm_response_t *SRTM_Response_Create(
    srtm_channel_t *channel, uint8_t category, uint16_t version, uint8_t command, uint8_t payloadLen)
{
    srtm_response_t *response;
    srtm_packet_head_t *packetHead;

    assert(channel);

    response = pvPortMalloc(sizeof(srtm_response_t));
    if (!response)
    {
        return NULL;
    }
    memset(response, 0x0U, sizeof(srtm_response_t));

    response->payloadLen = sizeof(srtm_packet_head_t) + payloadLen;
    response->payload = pvPortMalloc(response->payloadLen);
    if (!response->payload)
    {
        vPortFree(response);

        return NULL;
    }
    memset(response->payload, 0x0U, response->payloadLen);

    vListInitialiseItem(&response->listItem);
    listSET_LIST_ITEM_VALUE(&response->listItem, (uint32_t)response);

    response->channel = channel;
    packetHead = (srtm_packet_head_t *)response->payload;
    packetHead->category = category;
    packetHead->majorVersion = (uint8_t)((version & 0xFF00U) >> 8U);
    packetHead->minorVersion = (uint8_t)(version & 0xFFU);
    packetHead->type = SRTM_MESSAGE_TYPE_RESPONSE;
    packetHead->command = command;

    return response;
}

void SRTM_Response_Destroy(srtm_response_t *response)
{
    assert(response);

    vPortFree(response->payload);
    vPortFree(response);
}

uint8_t SRTM_Response_GetCategory(srtm_response_t *response)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)response->payload;

    assert(response);

    return packetHead->category;
}

uint16_t SRTM_Response_GetVersion(srtm_response_t *response)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)response->payload;

    assert(response);

    return (uint16_t)(((uint16_t)packetHead->majorVersion << 8U) + packetHead->minorVersion);
}

uint8_t SRTM_Response_GetCommand(srtm_response_t *response)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)response->payload;

    assert(response);

    return packetHead->command;
}

void SRTM_Response_GetPayload(srtm_response_t *response, uint8_t *payload, uint8_t payloadLen)
{
    assert(response);
    assert(payload);

    memcpy(payload, &response->payload[sizeof(srtm_packet_head_t)], payloadLen);
}

void SRTM_Response_SetPayload(srtm_response_t *response, uint8_t *payload, uint8_t payloadLen)
{
    assert(response);
    assert(payload);

    memcpy(&response->payload[sizeof(srtm_packet_head_t)], payload, payloadLen);
}

int32_t SRTM_Response_Deliver(srtm_response_t *response)
{
    int32_t result;
    strm_process_event_t event;

    assert(response);

    /* Register request into channel's request list. */
    xSemaphoreTake(response->channel->lock, portMAX_DELAY);
    vListInsertEnd(&response->channel->responseList, &response->listItem);

    /* Send out response. */
    event.type = SRTM_ProcessEventTypeTxResponse;
    event.channel = response->channel;
    event.payload = response->payload;
    event.payloadLen = response->payloadLen;
    result = xQueueSendToBack(response->channel->handle->queue, &event, 0U);
    xSemaphoreGive(response->channel->lock);

    if (pdTRUE == result)
    {
        result = SRTM_SUCCESS;
    }
    else
    {
        result = SRTM_FAIL;
    }

    return result;
}

srtm_notification_t *SRTM_Notification_Create(
    srtm_channel_t *channel, uint8_t category, uint16_t version, uint8_t command, uint8_t payloadLen)
{
    srtm_notification_t *notification;
    srtm_packet_head_t *packetHead;

    assert(channel);

    notification = pvPortMalloc(sizeof(srtm_notification_t));
    if (!notification)
    {
        return NULL;
    }
    memset(notification, 0x0U, sizeof(srtm_notification_t));

    notification->payloadLen = sizeof(srtm_packet_head_t) + payloadLen;
    notification->payload = pvPortMalloc(notification->payloadLen);
    if (!notification->payload)
    {
        vPortFree(notification);

        return NULL;
    }
    memset(notification->payload, 0x0U, notification->payloadLen);

    vListInitialiseItem(&notification->listItem);
    listSET_LIST_ITEM_VALUE(&notification->listItem, (uint32_t)notification);

    notification->channel = channel;
    packetHead = (srtm_packet_head_t *)notification->payload;
    packetHead->category = category;
    packetHead->majorVersion = (uint8_t)((version & 0xFF00U) >> 8U);
    packetHead->minorVersion = (uint8_t)(version & 0xFFU);
    packetHead->type = SRTM_MESSAGE_TYPE_NOTIFICATION;
    packetHead->command = command;

    return notification;
}

void SRTM_Notification_Destroy(srtm_notification_t *notification)
{
    assert(notification);

    vPortFree(notification->payload);
    vPortFree(notification);
}

uint8_t SRTM_Notification_GetCategory(srtm_notification_t *notification)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)notification->payload;

    assert(notification);

    return packetHead->category;
}

uint16_t SRTM_Notification_GetVersion(srtm_notification_t *notification)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)notification->payload;

    assert(notification);

    return (uint16_t)(((uint16_t)packetHead->majorVersion << 8U) + packetHead->minorVersion);
}

uint8_t SRTM_Notification_GetCommand(srtm_notification_t *notification)
{
    srtm_packet_head_t *packetHead = (srtm_packet_head_t *)notification->payload;

    assert(notification);

    return packetHead->command;
}

void SRTM_Notification_GetPayload(srtm_notification_t *notification, uint8_t *payload, uint8_t payloadLen)
{
    assert(notification);
    assert(payload);

    memcpy(payload, &notification->payload[sizeof(srtm_packet_head_t)], payloadLen);
}

void SRTM_Notification_SetPayload(srtm_notification_t *notification, uint8_t *payload, uint8_t payloadLen)
{
    assert(notification);
    assert(payload);

    memcpy(&notification->payload[sizeof(srtm_packet_head_t)], payload, payloadLen);
}

int32_t SRTM_Notification_Deliver(srtm_notification_t *notification)
{
    int32_t result;
    strm_process_event_t event;

    assert(notification);

    /* Register request into channel's request list. */
    xSemaphoreTake(notification->channel->lock, portMAX_DELAY);
    vListInsertEnd(&notification->channel->notificationList, &notification->listItem);

    /* Send out notification. */
    event.type = SRTM_ProcessEventTypeTxNotification;
    event.payload = notification->payload;
    event.payloadLen = notification->payloadLen;
    event.channel = notification->channel;
    result = xQueueSendToBack(notification->channel->handle->queue, &event, 0U);
    xSemaphoreGive(notification->channel->lock);

    if (pdTRUE == result)
    {
        result = SRTM_SUCCESS;
    }
    else
    {
        result = SRTM_FAIL;
    }

    return result;
}
