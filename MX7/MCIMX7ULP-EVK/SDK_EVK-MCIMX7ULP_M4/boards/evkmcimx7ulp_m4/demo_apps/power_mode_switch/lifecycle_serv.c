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

#include "lifecycle_serv.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "event_groups.h"
#include "fsl_srtm.h"
#include "list.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SRTM_LIFECYCLE_SERV_CATEGORY (0x1U)
#define SRTM_LIFECYCLE_SERV_VERSION (0x0100U)
#define SRTM_LIFECYCLE_SERV_RETURN_CODE_SUCEESS (0x0U)
#define SRTM_LIFECYCLE_SERV_RETURN_CODE_FAIL (0x1U)
#define SRTM_LIFECYCLE_SERV_RETURN_CODE_UNSUPPORTED (0x2U)
#define SRTM_LIFECYCLE_SERV_CMD_CHANGE_POWER_MODE (0x00U)
#define SRTM_LIFECYCLE_SERV_CMD_HEART_BEAT (0x01U)
#define SRTM_LIFECYCLE_SERV_CMD_HEART_BEAT_ENABLE (0x02U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_HSRUN (0x00U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_RUN (0x01U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_VLPR (0x02U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_WAIT (0x03U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_VLPS (0x04U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_VLLS (0x05U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_REBOOT (0x06U)
#define SRTM_LIFECYCLE_SERV_POWER_MODE_SHUTDOWN (0x07U)
#define SRTM_LIFECYCLE_SERV_HEART_BEAT_DISABLE (0x00U)
#define SRTM_LIFECYCLE_SERV_HEART_BEAT_ENABLE (0x01U)

typedef enum _LIFECYCLE_SERV_state {
    LIFECYCLE_SERV_StateStart = 0U,
    LIFECYCLE_SERV_StateStop = 1U,
    LIFECYCLE_SERV_StateRun = 2U,
} LIFECYCLE_SERV_state_t;

struct _lifecycle_serv
{
    srtm_handle_t *srtmHandle;
    srtm_channel_t *srtmChannel;
    uint32_t srtmChannelId;
    List_t eventCallbackList;
    SemaphoreHandle_t lock;
    EventGroupHandle_t eventGroup;
    TimerHandle_t timer;
    uint32_t heartBeatTimeout;
    LIFECYCLE_SERV_state_t state;
    volatile bool heartBeatEnableFlag;
};

typedef struct _LIFECYCLE_SERV_event_callback_element
{
    ListItem_t listItem;
    lifecycle_serv_event_cb_t callback;
    void *arg;
} lifecycle_serv_event_callback_element_t;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void LIFECYCLE_SERV_RequestServiceCallback(srtm_channel_t *channel, srtm_request_t *request, void *arg)
{
    lifecycle_serv_t *me = (lifecycle_serv_t *)arg;
    uint8_t payload;
    uint8_t command;

    command = SRTM_Request_GetCommand(request);
    if (SRTM_LIFECYCLE_SERV_CMD_CHANGE_POWER_MODE == command)
    {
        SRTM_Request_GetPayload(request, &payload, 1U);
        if (SRTM_LIFECYCLE_SERV_POWER_MODE_RUN == payload)
        {
            /* Kick A7 HeartBeat Software WatchDog Timer. */
            if (me->heartBeatEnableFlag)
            {
                xTimerReset(me->timer, portMAX_DELAY);
            }
            xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventSwitchToRunModeFlag);
        }
        else if (SRTM_LIFECYCLE_SERV_POWER_MODE_VLLS == payload)
        {
            xTimerStop(me->timer, portMAX_DELAY);
            xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventSwitchToSuspendModeFlag);
        }
        else if (SRTM_LIFECYCLE_SERV_POWER_MODE_REBOOT == payload)
        {
            xTimerStop(me->timer, portMAX_DELAY);
            xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventSwitchToRebootModeFlag);
        }
        else if (SRTM_LIFECYCLE_SERV_POWER_MODE_SHUTDOWN == payload)
        {
            xTimerStop(me->timer, portMAX_DELAY);
            xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventSwitchToShutDownModeFlag);
        }
        else
        {
        }
    }
    else if (SRTM_LIFECYCLE_SERV_CMD_HEART_BEAT_ENABLE == command)
    {
        SRTM_Request_GetPayload(request, &payload, 1U);
        if (SRTM_LIFECYCLE_SERV_HEART_BEAT_ENABLE == payload)
        {
            xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventGetHeartBeatEnable);
        }
        else
        {
            xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventGetHeartBeatDisable);
        }
    }
    else
    {
    }
}

static void LIFECYCLE_SERV_HeartBeat_Callback(srtm_channel_t *channel, srtm_notification_t *notification, void *arg)
{
    lifecycle_serv_t *me = (lifecycle_serv_t *)arg;

    if (me->heartBeatEnableFlag)
    {
        /* Kick A7 HeartBeat Software WatchDog Timer. */
        xTimerReset(me->timer, portMAX_DELAY);
    }

    xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventGetHeartBeatFlag);
}

static void LIFECYCLE_SERV_HeartBeatTimeout_Callback(TimerHandle_t xTimer)
{
    lifecycle_serv_t *me = (lifecycle_serv_t *)pvTimerGetTimerID(xTimer);

    xEventGroupSetBits(me->eventGroup, LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag);
}

static void LIFECYCLE_SERV_Start(lifecycle_serv_t *me)
{
    me->srtmChannel = SRTM_Channel_Create(me->srtmHandle, me->srtmChannelId);
    if (!me->srtmChannel)
    {
        assert(false);
    }

    if (SRTM_SUCCESS != SRTM_Channel_RegisterService(me->srtmChannel, SRTM_LIFECYCLE_SERV_CATEGORY,
                                                     LIFECYCLE_SERV_RequestServiceCallback, (void *)me))
    {
        assert(false);
    }
    if (SRTM_SUCCESS != SRTM_Channel_RegisterNotification(me->srtmChannel, SRTM_LIFECYCLE_SERV_CATEGORY,
                                                          SRTM_LIFECYCLE_SERV_CMD_HEART_BEAT,
                                                          LIFECYCLE_SERV_HeartBeat_Callback, (void *)me))
    {
        assert(false);
    }

    me->state = LIFECYCLE_SERV_StateRun;
}

static void LIFECYCLE_SERV_Stop(lifecycle_serv_t *me)
{
    if (pdPASS != xTimerStop(me->timer, 0U))
    {
        assert(false);
    }
    SRTM_Channel_Destroy(me->srtmChannel);

    vTaskDelay(200U);
    me->state = LIFECYCLE_SERV_StateStart;
}

static void LIFECYCLE_SERV_Run(lifecycle_serv_t *me)
{
    ListItem_t *listItem;
    lifecycle_serv_event_callback_element_t *eventCallbackElement;
    srtm_response_t *response;
    EventBits_t eventBits;
    uint8_t retVal;

    for (;;)
    {
        eventBits = xEventGroupWaitBits(
            me->eventGroup,
            LIFECYCLE_SERV_EventGetHeartBeatFlag | LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag |
                LIFECYCLE_SERV_EventSwitchToRunModeFlag | LIFECYCLE_SERV_EventSwitchToSuspendModeFlag |
                LIFECYCLE_SERV_EventSwitchToRebootModeFlag | LIFECYCLE_SERV_EventSwitchToShutDownModeFlag |
                LIFECYCLE_SERV_EventGetHeartBeatEnable | LIFECYCLE_SERV_EventGetHeartBeatDisable,
            pdTRUE, pdFALSE, portMAX_DELAY);

        for (listItem = listGET_HEAD_ENTRY(&me->eventCallbackList);
             listItem != listGET_END_MARKER(&me->eventCallbackList); listItem = listGET_NEXT(listItem))
        {
            eventCallbackElement = (lifecycle_serv_event_callback_element_t *)listGET_LIST_ITEM_VALUE(listItem);
            eventCallbackElement->callback(me, eventBits, eventCallbackElement->arg);
        }

        /* Process Power Mode Change Request. */
        if (eventBits & (LIFECYCLE_SERV_EventGetHeartBeatFlag | LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag |
                         LIFECYCLE_SERV_EventSwitchToRunModeFlag | LIFECYCLE_SERV_EventSwitchToSuspendModeFlag |
                         LIFECYCLE_SERV_EventSwitchToRebootModeFlag | LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
        {
            if (eventBits & (LIFECYCLE_SERV_EventSwitchToRunModeFlag | LIFECYCLE_SERV_EventSwitchToSuspendModeFlag |
                             LIFECYCLE_SERV_EventSwitchToRebootModeFlag | LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
            {
                response =
                    SRTM_Response_Create(me->srtmChannel, SRTM_LIFECYCLE_SERV_CATEGORY, SRTM_LIFECYCLE_SERV_VERSION,
                                         SRTM_LIFECYCLE_SERV_CMD_CHANGE_POWER_MODE, 1U);
                if (!response)
                {
                    assert(false);
                }
                retVal = 0U;
                SRTM_Response_SetPayload(response, &retVal, 1U);
                SRTM_Response_Deliver(response);
            }
            if (eventBits & (LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag | LIFECYCLE_SERV_EventSwitchToRebootModeFlag |
                             LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
            {
                me->state = LIFECYCLE_SERV_StateStop;
                break;
            }
        }
        /* Process Heart Beat Enable. */
        if (eventBits & (LIFECYCLE_SERV_EventGetHeartBeatEnable | LIFECYCLE_SERV_EventGetHeartBeatDisable))
        {
            if (eventBits & LIFECYCLE_SERV_EventGetHeartBeatEnable)
            {
                me->heartBeatEnableFlag = true;
                xTimerReset(me->timer, portMAX_DELAY);
            }
            else
            {
                xTimerStop(me->timer, portMAX_DELAY);
                me->heartBeatEnableFlag = false;
            }
            response = SRTM_Response_Create(me->srtmChannel, SRTM_LIFECYCLE_SERV_CATEGORY, SRTM_LIFECYCLE_SERV_VERSION,
                                            SRTM_LIFECYCLE_SERV_CMD_HEART_BEAT_ENABLE, 1U);
            if (!response)
            {
                assert(false);
            }
            retVal = 0U;
            SRTM_Response_SetPayload(response, &retVal, 1U);
            SRTM_Response_Deliver(response);
        }
    }
}

lifecycle_serv_t *LIFECYCLE_SERV_Init(const lifecycle_serv_config_t *config)
{
    lifecycle_serv_t *me;

    assert(config);

    me = pvPortMalloc(sizeof(lifecycle_serv_t));
    if (!me)
    {
        return NULL;
    }
    me->eventGroup = xEventGroupCreate();
    if (!me->eventGroup)
    {
        vPortFree(me);

        return NULL;
    }
    me->heartBeatEnableFlag = config->heartBeatEnable;
    me->heartBeatTimeout = config->heartBeatTimeout_Ms;
    me->timer =
        xTimerCreate("HeartBeat", me->heartBeatTimeout, pdFALSE, (void *)me, LIFECYCLE_SERV_HeartBeatTimeout_Callback);
    if (!me->timer)
    {
        vEventGroupDelete(me->eventGroup);
        vPortFree(me);

        return NULL;
    }
    me->lock = xSemaphoreCreateMutex();
    if (!me->lock)
    {
        xTimerStop(me->timer, portMAX_DELAY);
        xTimerDelete(me->timer, portMAX_DELAY);
        vEventGroupDelete(me->eventGroup);
        vPortFree(me);

        return NULL;
    }

    vListInitialise(&me->eventCallbackList);
    me->srtmHandle = config->srtmHandle;
    me->srtmChannel = NULL;
    me->srtmChannelId = config->srtmChannelId;

    me->state = LIFECYCLE_SERV_StateStart;

    return me;
}

int32_t LIFECYCLE_SERV_Subscribe(lifecycle_serv_t *me, lifecycle_serv_event_cb_t callback, void *arg)
{
    int32_t result = -1;
    lifecycle_serv_event_callback_element_t *eventCallback;

    if (me)
    {
        eventCallback = pvPortMalloc(sizeof(lifecycle_serv_event_callback_element_t));
        if (eventCallback)
        {
            eventCallback->callback = callback;
            eventCallback->arg = arg;
            vListInitialiseItem(&eventCallback->listItem);
            listSET_LIST_ITEM_VALUE(&eventCallback->listItem, (uint32_t)eventCallback);

            xSemaphoreTake(me->lock, portMAX_DELAY);
            vListInsertEnd(&me->eventCallbackList, &eventCallback->listItem);
            xSemaphoreGive(me->lock);

            result = 0;
        }
    }

    return result;
}

int32_t LIFECYCLE_SERV_Unsubscribe(lifecycle_serv_t *me, lifecycle_serv_event_cb_t callback)
{
    int32_t result = -1;
    ListItem_t *listItem;
    lifecycle_serv_event_callback_element_t *eventCallback;

    if (me)
    {
        xSemaphoreTake(me->lock, portMAX_DELAY);
        for (listItem = listGET_HEAD_ENTRY(&me->eventCallbackList);
             listItem != listGET_END_MARKER(&me->eventCallbackList); listItem = listGET_NEXT(listItem))
        {
            eventCallback = (lifecycle_serv_event_callback_element_t *)listGET_LIST_ITEM_VALUE(listItem);
            if (callback == eventCallback->callback)
            {
                uxListRemove(listItem);
                result = 0;
            }
        }
        xSemaphoreGive(me->lock);
    }

    return result;
}

void LIFECYCLE_SERV_RebootRemoteCoreFromISR(lifecycle_serv_t *me)
{
     BaseType_t reschedule = pdFALSE;

    if (me)
    {
        xTimerStopFromISR(me->timer, &reschedule);
        xEventGroupSetBitsFromISR(me->eventGroup, LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag, &reschedule);
    }

     portYIELD_FROM_ISR(reschedule);
}

void LIFECYCLE_SERV_ProcessTask(void *pvParameters)
{
    lifecycle_serv_t *me = (lifecycle_serv_t *)pvParameters;

    for (;;)
    {
        switch (me->state)
        {
            case LIFECYCLE_SERV_StateStart:
                LIFECYCLE_SERV_Start(me);
                break;

            case LIFECYCLE_SERV_StateStop:
                LIFECYCLE_SERV_Stop(me);
                break;

            case LIFECYCLE_SERV_StateRun:
                LIFECYCLE_SERV_Run(me);
                break;

            default:
                break;
        }
    }
}
