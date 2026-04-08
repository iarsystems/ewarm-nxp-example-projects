/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
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

#include "keypad_serv.h"
#include "FreeRTOS.h"
#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "fsl_pf1550_serv.h"
#include "fsl_msmc.h"
#include "fsl_mu.h"
#include "fsl_pmc0.h"
#include "fsl_port.h"
#include "semphr.h"
#include "timers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Keypad Service Category definition */
#define SRTM_KEYPAD_SERV_CATEGORY                           (0x4U)
#define SRTM_KEYPAD_SERV_VERSION                            (0x0100U)

/* Keypad Service Return Code definition */
#define SRTM_KEYPAD_SERV_RETURN_CODE_SUCEESS                (0x0U)
#define SRTM_KEYPAD_SERV_RETURN_CODE_FAIL                   (0x1U)
#define SRTM_KEYPAD_SERV_RETURN_CODE_UNSUPPORTED            (0x2U)

/* Keypad Service Request Command definition */
#define SRTM_KEYPAD_SERV_REQUEST_CMD_SET_KEY_PARAMETER      (0x0U)
#define SRTM_KEYPAD_SERV_REQUEST_CMD_KEY_EVENT              (0x80U)
#define SRTM_KEYPAD_SERV_REQUEST_CMD_RESET                  (0x81U)

/* Keypad Service Notification Command definition */
#define SRTM_KEYPAD_SERV_NOTIFICATION_CMD_KEY_EVENT         (0x0U)

/* Keypad Instance definition */
#define SRTM_KEYPAD_INSTANCE_INDEX_ONOFF                    (255U)
#define SRTM_KEYPAD_INSTANCE_INDEX_VOL_P                    (116U)
#define SRTM_KEYPAD_INSTANCE_INDEX_VOL_M                    (114U)

/* Keypad Config definition */
#define SRTM_KEYPAD_CONFIG_IGNORE                           (0x0U)
#define SRTM_KEYPAD_CONFIG_PRESS                            (0x1U)
#define SRTM_KEYPAD_CONFIG_RELEASE                          (0x2U)
#define SRTM_KEYPAD_CONFIG_PRESS_OR_RELEASE                 (0x3U)

/* Keypad Event definition */
#define SRTM_KEYPAD_EVENT_RELEASE                           (0x0U)
#define SRTM_KEYPAD_EVENT_PRESS                             (0x1U)

#define SRTM_LLWU_WAKEUP_PIN_IDX                            (13U) /* LLWU_P13 */

typedef __packed struct _keypad_request
{
    uint8_t command;
    union {
        __packed struct
        {
            uint8_t index;
            union {
                uint8_t eventConfig;
                uint8_t event;
                uint8_t returnCode;
            };
            uint8_t wakeupConfig;
        };
        uint8_t data[3U];
    };
} keypad_request_t;

typedef struct _keypad
{
    keypad_serv_t *handle;
    TimerHandle_t timer;
    uint8_t index;
    uint8_t eventConfig;
    uint8_t wakeupConfig;
    uint8_t keyValue;
} keypad_t;

typedef enum _KEYPAD_SERV_state {
    KEYPAD_SERV_StateStart = 0U,
    KEYPAD_SERV_StateStop = 1U,
    KEYPAD_SERV_StateRun = 2U,
} KEYPAD_SERV_state_t;

struct _keypad_serv
{
    srtm_handle_t *srtmHandle;
    srtm_channel_t *srtmChannel;
    uint32_t srtmChannelId;
    lifecycle_serv_t *lifecycleHandle;
    QueueHandle_t queue;
    uint32_t wakeUpPins;
    KEYPAD_SERV_state_t state;
};

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern pf1550_serv_handle_t pf1550Handle;
extern TimerHandle_t suspendTimer;
#pragma location = "M4SuspendRam"
static volatile keypad_t keypadArray[3U];

/*******************************************************************************
 * Code
 ******************************************************************************/
/* VOL+, VOL- Key ISR. */
void PCTLB_IRQHandler(void)
{
    BaseType_t reschedule = pdFALSE;

    /* VOL+ Key process. */
    if ((1U << BOARD_VOLP_GPIO_PIN) & PORT_GetPinsInterruptFlags(BOARD_VOLP_PORT))
    {
        PORT_SetPinInterruptConfig(BOARD_VOLP_PORT, BOARD_VOLP_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(BOARD_VOLP_PORT, (1U << BOARD_VOLP_GPIO_PIN));

        keypadArray[1U].keyValue = GPIO_ReadPinInput(BOARD_VOLP_GPIO, BOARD_VOLP_GPIO_PIN);
        xTimerStartFromISR(keypadArray[1U].timer, &reschedule);
    }
    /* VOL- Key process. */
    else if ((1U << BOARD_VOLM_GPIO_PIN) & PORT_GetPinsInterruptFlags(BOARD_VOLM_PORT))
    {
        PORT_SetPinInterruptConfig(BOARD_VOLM_PORT, BOARD_VOLM_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(BOARD_VOLM_PORT, (1U << BOARD_VOLM_GPIO_PIN));

        keypadArray[2U].keyValue = GPIO_ReadPinInput(BOARD_VOLM_GPIO, BOARD_VOLM_GPIO_PIN);
        xTimerStartFromISR(keypadArray[2U].timer, &reschedule);
    }

    portYIELD_FROM_ISR(reschedule);
}

static void KEYPAD_SERV_VolP_Callback(TimerHandle_t xTimer)
{
    keypad_request_t request;

    /* Verify Keypad event. */
    if (keypadArray[1U].keyValue == GPIO_ReadPinInput(BOARD_VOLP_GPIO, BOARD_VOLP_GPIO_PIN))
    {
        request.command = SRTM_KEYPAD_SERV_REQUEST_CMD_KEY_EVENT;
        request.index = keypadArray[1U].index;
        request.event = (0U == keypadArray[1U].keyValue) ? SRTM_KEYPAD_EVENT_PRESS : SRTM_KEYPAD_EVENT_RELEASE;

        xQueueSendToBack(keypadArray[1U].handle->queue, &request, 0U);
    }

    PORT_SetPinInterruptConfig(BOARD_VOLP_PORT, BOARD_VOLP_GPIO_PIN, kPORT_InterruptEitherEdge);
}

static void KEYPAD_SERV_VolM_Callback(TimerHandle_t xTimer)
{
    keypad_request_t request;

    /* Verify Keypad event. */
    if (keypadArray[2U].keyValue == GPIO_ReadPinInput(BOARD_VOLM_GPIO, BOARD_VOLM_GPIO_PIN))
    {
        request.command = SRTM_KEYPAD_SERV_REQUEST_CMD_KEY_EVENT;
        request.index = keypadArray[2U].index;
        request.event = (0U == keypadArray[2U].keyValue) ? SRTM_KEYPAD_EVENT_PRESS : SRTM_KEYPAD_EVENT_RELEASE;

        xQueueSendToBack(keypadArray[2U].handle->queue, &request, 0U);
    }

    PORT_SetPinInterruptConfig(BOARD_VOLM_PORT, BOARD_VOLM_GPIO_PIN, kPORT_InterruptEitherEdge);
}

static void KEYPAD_SERV_RequestServiceCallback(srtm_channel_t *channel, srtm_request_t *request, void *arg)
{
    keypad_serv_t *me = (keypad_serv_t *)arg;
    keypad_request_t remoteRequest;

    remoteRequest.command = SRTM_Request_GetCommand(request);

    SRTM_Request_GetPayload(request, remoteRequest.data, sizeof(remoteRequest.data));
    xQueueSend(me->queue, &remoteRequest, 0U);
}

static void KEYPAD_SERV_LifecycleServEventCallback(lifecycle_serv_t *lifecycleHandle, uint32_t event, void *arg)
{
    keypad_serv_t *me = (keypad_serv_t *)arg;
    keypad_request_t request;

    if (event & (LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag | LIFECYCLE_SERV_EventSwitchToRebootModeFlag |
                 LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
    {
        request.command = SRTM_KEYPAD_SERV_REQUEST_CMD_RESET;
        xQueueSendToFront(me->queue, &request, 0U);
    }
    else
    {
    }
}

static void KEYPAD_SERV_Start(keypad_serv_t *me)
{
    me->srtmChannel = SRTM_Channel_Create(me->srtmHandle, me->srtmChannelId);
    if (!me->srtmChannel)
    {
        assert(false);
    }
    if (SRTM_SUCCESS != SRTM_Channel_RegisterService(me->srtmChannel, SRTM_KEYPAD_SERV_CATEGORY,
                                                     KEYPAD_SERV_RequestServiceCallback, (void *)me))
    {
        assert(false);
    }

    me->state = KEYPAD_SERV_StateRun;
}

static void KEYPAD_SERV_Stop(keypad_serv_t *me)
{
    SRTM_Channel_Destroy(me->srtmChannel);

    vTaskDelay(200U);
    me->state = KEYPAD_SERV_StateStart;
}

static void KEYPAD_SERV_Run(keypad_serv_t *me)
{
    keypad_request_t request;
    srtm_response_t *response;
    srtm_notification_t *notification;
    uint32_t i;

    for (;;)
    {
        xQueueReceive(me->queue, &request, portMAX_DELAY);

        if (SRTM_KEYPAD_SERV_REQUEST_CMD_SET_KEY_PARAMETER == request.command)
        {
            for (i = 0U; i < (sizeof(keypadArray) / sizeof(keypadArray[0U])); i++)
            {
                if (request.index == keypadArray[i].index)
                {
                    keypadArray[i].eventConfig = request.eventConfig;
                    keypadArray[i].wakeupConfig = request.wakeupConfig;
                }
            }

            response = SRTM_Response_Create(me->srtmChannel, SRTM_KEYPAD_SERV_CATEGORY, SRTM_KEYPAD_SERV_VERSION,
                                            request.command, 2U);
            if (!response)
            {
                assert(false);
            }

            request.data[0U] = keypadArray[i].index;
            request.data[1U] = 0x0U;
            SRTM_Response_SetPayload(response, request.data, 2U);
            SRTM_Response_Deliver(response);
        }
        else if (SRTM_KEYPAD_SERV_REQUEST_CMD_KEY_EVENT == request.command)
        {
            for (i = 0U; i < (sizeof(keypadArray) / sizeof(keypadArray[0U])); i++)
            {
                if (request.index == keypadArray[i].index)
                {
                    break;
                }
            }

            if (i == (sizeof(keypadArray) / sizeof(keypadArray[0U])))
            {
                assert(false);
            }

            if ((keypadArray[i].eventConfig == SRTM_KEYPAD_CONFIG_PRESS_OR_RELEASE) ||
                ((keypadArray[i].eventConfig == SRTM_KEYPAD_CONFIG_PRESS) && (request.event == SRTM_KEYPAD_EVENT_PRESS)) ||
                ((keypadArray[i].eventConfig == SRTM_KEYPAD_CONFIG_RELEASE) && (request.event == SRTM_KEYPAD_EVENT_RELEASE)))
            {
                if ((keypadArray[i].wakeupConfig) && (kMU_PowerModeDsm == MU_GetOtherCorePowerMode(MUA)))
                {
                    xTimerStop(suspendTimer, portMAX_DELAY);

                    /* Enable LPDDR3 Power. */
                    GPIO_WritePinOutput(GPIOB, 6U, 0U);
                    vTaskDelay(1U);

                    /* Enable A7 Core Power. */
                    PF1550_SERV_EnableRegulator(&pf1550Handle,
                               kPF1550_ModuleSwitch1,
                               kPF1550_OperatingStatusRun,
                               true);
                    vTaskDelay(100U);

                    /* We use PMIC instead of LDO for A7 */
                    PMC0_EnablePmc1LdoRegulator(false);
                    PMC0_PowerOnPmc1();
                    MU_BootOtherCore(MUA, kMU_CoreBootFromAddr0);

                    MU_TriggerInterrupts(MUA, kMU_NmiInterruptTrigger);

                    while (kMU_PowerModeDsm == MU_GetOtherCorePowerMode(MUA))
                    {
                        vTaskDelay(10U);
                    }
                }

                notification = SRTM_Notification_Create(me->srtmChannel, SRTM_KEYPAD_SERV_CATEGORY, SRTM_KEYPAD_SERV_VERSION,
                                    SRTM_KEYPAD_SERV_NOTIFICATION_CMD_KEY_EVENT, 2U);
                if (!notification)
                {
                    assert(false);
                }
                request.data[0U] = request.index;
                request.data[1U] = request.event;
                SRTM_Notification_SetPayload(notification, request.data, 2U);
                SRTM_Notification_Deliver(notification);
            }
        }
        else if (SRTM_KEYPAD_SERV_REQUEST_CMD_RESET == request.command)
        {
            me->state = KEYPAD_SERV_StateStop;
            break;
        }
        else
        {
        }
    }
}

keypad_serv_t *KEYPAD_SERV_Init(const keypad_serv_config_t *config)
{
    keypad_serv_t *me;

    me = pvPortMalloc(sizeof(keypad_serv_t));
    if (!me)
    {
        return NULL;
    }

    me->queue = xQueueCreate(config->queueSize, sizeof(keypad_request_t));
    if (!me->queue)
    {
        vPortFree(me);

        return NULL;
    }

    me->srtmHandle = config->srtmHandle;
    me->srtmChannel = NULL;
    me->srtmChannelId = config->srtmChannelId;
    me->lifecycleHandle = config->lifecycleHandle;
    me->wakeUpPins = config->wakeUpPins;

    if (0U != LIFECYCLE_SERV_Subscribe(me->lifecycleHandle, KEYPAD_SERV_LifecycleServEventCallback, (void *)me))
    {
        assert(false);
    }

    me->state = KEYPAD_SERV_StateStart;

    return me;
}

void KEYPAD_SERV_ProcessTask(void *pvParameters)
{
    keypad_serv_t *me = (keypad_serv_t *)pvParameters;
    gpio_pin_config_t keyConfig = {
        kGPIO_DigitalInput, 0,
    };

    if (!(kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0)))
    {
        keypadArray[0U].index = SRTM_KEYPAD_INSTANCE_INDEX_ONOFF;
        keypadArray[0U].eventConfig = 0U;
        keypadArray[0U].wakeupConfig = 0U;
        keypadArray[0U].keyValue = 0U;
        keypadArray[1U].index = SRTM_KEYPAD_INSTANCE_INDEX_VOL_P;
        keypadArray[1U].eventConfig = 0U;
        keypadArray[1U].wakeupConfig = 0U;
        keypadArray[1U].keyValue = 0U;
        keypadArray[2U].index = SRTM_KEYPAD_INSTANCE_INDEX_VOL_M;
        keypadArray[2U].eventConfig = 0U;
        keypadArray[2U].wakeupConfig = 0U;
        keypadArray[2U].keyValue = 0U;
    }
    keypadArray[0U].handle = me;
    keypadArray[1U].handle = me;
    keypadArray[1U].timer = xTimerCreate("VOLP Key", 50U, pdFALSE, (void *)0U, KEYPAD_SERV_VolP_Callback);
    keypadArray[2U].handle = me;
    keypadArray[2U].timer = xTimerCreate("VOLM Key", 50U, pdFALSE, (void *)0U, KEYPAD_SERV_VolM_Callback);

    GPIO_PinInit(BOARD_VOLP_GPIO, BOARD_VOLP_GPIO_PIN, &keyConfig);
    GPIO_PinInit(BOARD_VOLM_GPIO, BOARD_VOLM_GPIO_PIN, &keyConfig);
    PORT_SetPinInterruptConfig(BOARD_VOLP_PORT, BOARD_VOLP_GPIO_PIN, kPORT_InterruptEitherEdge);
    PORT_SetPinInterruptConfig(BOARD_VOLM_PORT, BOARD_VOLM_GPIO_PIN, kPORT_InterruptEitherEdge);

    if ((kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0)) &&
        (me->wakeUpPins))
    {
        if (me->wakeUpPins & (1 << SRTM_LLWU_WAKEUP_PIN_IDX))
        {
            keypadArray[1U].keyValue = GPIO_ReadPinInput(BOARD_VOLP_GPIO, BOARD_VOLP_GPIO_PIN);
            xTimerStart(keypadArray[1U].timer, portMAX_DELAY);
        }
        else
        {
        }
    }
    else
    {
    }

    NVIC_SetPriority(BOARD_VOLP_IRQ, 7U);
    NVIC_SetPriority(BOARD_VOLM_IRQ, 7U);
    EnableIRQ(BOARD_VOLP_IRQ);
    EnableIRQ(BOARD_VOLM_IRQ);

    for (;;)
    {
        switch (me->state)
        {
            case KEYPAD_SERV_StateStart:
                KEYPAD_SERV_Start(me);
                break;

            case KEYPAD_SERV_StateStop:
                KEYPAD_SERV_Stop(me);
                break;

            case KEYPAD_SERV_StateRun:
                KEYPAD_SERV_Run(me);
                break;

            default:
                break;
        }
    }
}
