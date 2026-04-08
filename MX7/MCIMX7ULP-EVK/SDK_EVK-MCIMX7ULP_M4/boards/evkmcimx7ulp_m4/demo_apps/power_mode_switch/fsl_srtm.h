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

#ifndef _FSL_SRTM_H_
#define _FSL_SRTM_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SRTM Error Macros. */
#define SRTM_SUCCESS (0x0U)
#define SRTM_FAIL (0x1U)
#define SRTM_READY (SRTM_SUCCESS)
#define SRTM_NOT_READT (SRTM_FAIL)
#define SRTM_ERROR (0x8000U)
#define SRTM_ERROR_MEM_ALLOC (SRTM_ERROR | 0x1U)
#define SRTM_ERROR_OUT_OF_RANGE (SRTM_ERROR | 0x2U)
#define SRTM_ERROR_COMM_INIT_FAIL (SRTM_ERROR | 0x3U)

typedef struct _srtm_handle srtm_handle_t;
typedef struct _srtm_channel srtm_channel_t;
typedef struct _srtm_request srtm_request_t;
typedef struct _srtm_response srtm_response_t;
typedef struct _srtm_notification srtm_notification_t;
typedef void (*srtm_channel_request_service_cb_t)(srtm_channel_t *channel, srtm_request_t *request, void *arg);
typedef void (*srtm_channel_notification_service_cb_t)(srtm_channel_t *channel,
                                                       srtm_notification_t *notification,
                                                       void *arg);
typedef void (*srtm_request_cb_t)(srtm_request_t *request, srtm_response_t *response, void *arg);

typedef struct _srtm_channel_config
{
    uint32_t channelType;
    void *driverConfig;
} srtm_channel_config_t;

typedef struct _srtm_config
{
    uint32_t queueSize;
    uint32_t channelCount;
    srtm_channel_config_t *channelConfigs;
} srtm_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* SRTM Instance Control API */
srtm_handle_t *SRTM_Init(const srtm_config_t *config);
void SRTM_Suspend(srtm_handle_t *handle);
void SRTM_Resume(srtm_handle_t *handle);
void SRTM_ProcessTask(void *pvParameters);

/* SRTM Channel Control API */
srtm_channel_t *SRTM_Channel_Create(srtm_handle_t *handle, uint32_t id);
void SRTM_Channel_Destroy(srtm_channel_t *channel);
int32_t SRTM_Channel_IsReady(srtm_channel_t *channel);
int32_t SRTM_Channel_RegisterService(srtm_channel_t *channel,
                                     uint8_t category,
                                     srtm_channel_request_service_cb_t callback,
                                     void *arg);
int32_t SRTM_Channel_RegisterNotification(srtm_channel_t *channel,
                                          uint8_t category,
                                          uint8_t command,
                                          srtm_channel_notification_service_cb_t callback,
                                          void *arg);

/* SRTM Communication Control API */
/* Request Control APIs */
srtm_request_t *SRTM_Request_Create(
    srtm_channel_t *channel, uint8_t category, uint16_t version, uint8_t command, uint8_t payloadLen);
void SRTM_Request_Destroy(srtm_request_t *request);
uint8_t SRTM_Request_GetCategory(srtm_request_t *request);
uint16_t SRTM_Request_GetVersion(srtm_request_t *request);
uint8_t SRTM_Request_GetCommand(srtm_request_t *request);
void SRTM_Request_GetPayload(srtm_request_t *request, uint8_t *payload, uint8_t payloadLen);
void SRTM_Request_SetPayload(srtm_request_t *request, uint8_t *payload, uint8_t payloadLen);
uint32_t SRTM_Request_GetPayloadLen(srtm_request_t *request);
int32_t SRTM_Request_Send(srtm_request_t *request, srtm_response_t *response, uint32_t timeout);
int32_t SRTM_Request_Deliver(srtm_request_t *request, srtm_request_cb_t callback, void *arg);

/* Response Control APIs */
srtm_response_t *SRTM_Response_Create(
    srtm_channel_t *channel, uint8_t category, uint16_t version, uint8_t command, uint8_t payloadLen);
void SRTM_Response_Destroy(srtm_response_t *response);
uint8_t SRTM_Response_GetCategory(srtm_response_t *response);
uint16_t SRTM_Response_GetVersion(srtm_response_t *response);
uint8_t SRTM_Response_GetCommand(srtm_response_t *response);
void SRTM_Response_GetPayload(srtm_response_t *response, uint8_t *payload, uint8_t payloadLen);
void SRTM_Response_SetPayload(srtm_response_t *response, uint8_t *payload, uint8_t payloadLen);
int32_t SRTM_Response_Deliver(srtm_response_t *response);

/* Notification Control APIs */
srtm_notification_t *SRTM_Notification_Create(
    srtm_channel_t *channel, uint8_t category, uint16_t version, uint8_t command, uint8_t payloadLen);
void SRTM_Notification_Destroy(srtm_notification_t *notification);
uint8_t SRTM_Notification_GetCategory(srtm_notification_t *notification);
uint16_t SRTM_Notification_GetVersion(srtm_notification_t *notification);
uint8_t SRTM_Notification_GetCommand(srtm_notification_t *notification);
void SRTM_Notification_GetPayload(srtm_notification_t *notification, uint8_t *payload, uint8_t payloadLen);
void SRTM_Notification_SetPayload(srtm_notification_t *notification, uint8_t *payload, uint8_t payloadLen);
int32_t SRTM_Notification_Deliver(srtm_notification_t *notification);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_SRTM_H_ */
