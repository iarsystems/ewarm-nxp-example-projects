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
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "fsl_pf1550_serv.h"
#include "fsl_srtm.h"
#include "lifecycle_serv.h"
#include "pf1550_remote_serv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SRTM_PMIC_CATEGORY (0x2U)
#define SRTM_PMIC_VERSION (0x0100U)
#define SRTM_PMIC_RETURN_CODE_SUCEESS (0x0U)
#define SRTM_PMIC_RETURN_CODE_FAIL (0x1U)
#define SRTM_PMIC_RETURN_CODE_UNSUPPORTED (0x2U)
#define SRTM_PMIC_REGULATOR_STATUS_DISABLED (0x0U)
#define SRTM_PMIC_REGULATOR_STATUS_ENABLED (0x1U)

/* regulators ID define */
enum pf1550_remote_regulator
{
    PF1550_REMOTE_RegulatorSwitch1 = 0U,
    PF1550_REMOTE_RegulatorSwitch2 = 1U,
    PF1550_REMOTE_RegulatorSwitch3 = 2U,
    PF1550_REMOTE_RegulatorVrefDdr = 3U,
    PF1550_REMOTE_RegulatorLdo1 = 4U,
    PF1550_REMOTE_RegulatorLdo2 = 5U,
    PF1550_REMOTE_RegulatorLdo3 = 6U,
};

/* regulator command */
enum _pf1550_remote_command
{
    PF1550_REMOTE_CommandEnable = 0U,
    PF1550_REMOTE_CommandDisable = 1U,
    PF1550_REMOTE_CommandIsEnabled = 2U,
    PF1550_REMOTE_CommandSetVoltage = 3U,
    PF1550_REMOTE_CommandGetVoltage = 4U,
    PF1550_REMOTE_CommandGetRegister = 5U,
    PF1550_REMOTE_CommandSetRegister = 6U,
    PF1550_REMOTE_CommandSetStandbyVoltage = 7U,
    PF1550_REMOTE_CommandRestart = 8U,
};

#if defined(__GNUC__)
typedef struct _pf1550_remote_request
#else
typedef __packed struct _pf1550_remote_request
#endif
{
    uint8_t command;
    union {
#if defined(__GNUC__)
        struct
#else
        __packed struct
#endif
        {
            union {
                uint8_t regulator;
                uint8_t reg;
            };
            uint8_t returnCode;
            uint8_t regulatorStatus;
            union {
                uint32_t voltage;
                uint8_t value;
            };
#if defined(__GNUC__)
        } __attribute__((packed));
#else
        };
#endif
        uint8_t data[7U];
    };
#if defined(__GNUC__)
} __attribute__((packed)) pf1550_remote_request_t;
#else
} pf1550_remote_request_t;
#endif

typedef enum _PF1550_REMOTE_SERV_state {
    PF1550_REMOTE_SERV_StateStart = 0U,
    PF1550_REMOTE_SERV_StateStop = 1U,
    PF1550_REMOTE_SERV_StateRun = 2U,
} PF1550_REMOTE_SERV_state_t;

struct _pf1550_remote_serv
{
    srtm_handle_t *srtmHandle;
    srtm_channel_t *srtmChannel;
    uint32_t srtmChannelId;
    pf1550_serv_handle_t *pf1550Handle;
    lifecycle_serv_t *lifecycleHandle;
    QueueHandle_t queue;
    PF1550_REMOTE_SERV_state_t state;
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void PF1550_REMOTE_SERV_RequestServiceCallback(srtm_channel_t *channel, srtm_request_t *request, void *arg)
{
    pf1550_remote_serv_t *me = (pf1550_remote_serv_t *)arg;
    pf1550_remote_request_t remoteRequest;

    remoteRequest.command = SRTM_Request_GetCommand(request);
    SRTM_Request_GetPayload(request, remoteRequest.data, sizeof(remoteRequest.data));
    xQueueSend(me->queue, &remoteRequest, 0U);
}

static void PF1550_REMOTE_SERV_LifecycleServEventCallback(lifecycle_serv_t *lifecycleHandle, uint32_t event, void *arg)
{
    pf1550_remote_serv_t *me = (pf1550_remote_serv_t *)arg;
    pf1550_remote_request_t remoteRequest;

    if (event & (LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag | LIFECYCLE_SERV_EventSwitchToRebootModeFlag |
                 LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
    {
        remoteRequest.command = PF1550_REMOTE_CommandRestart;
        xQueueSendToFront(me->queue, &remoteRequest, 0U);
    }
}

static pf1550_module_t PF1550_REMOTE_SERV_GetLocalModuleName(uint8_t regulator)
{
    pf1550_module_t module;

    switch (regulator)
    {
        case PF1550_REMOTE_RegulatorSwitch1:
            module = kPF1550_ModuleSwitch1;
            break;

        case PF1550_REMOTE_RegulatorSwitch2:
            module = kPF1550_ModuleSwitch2;
            break;

        case PF1550_REMOTE_RegulatorSwitch3:
            module = kPF1550_ModuleSwitch3;
            break;

        case PF1550_REMOTE_RegulatorVrefDdr:
            module = kPF1550_ModuleVrefDdr;
            break;

        case PF1550_REMOTE_RegulatorLdo1:
            module = kPF1550_ModuleLdo1;
            break;

        case PF1550_REMOTE_RegulatorLdo2:
            module = kPF1550_ModuleLdo2;
            break;

        case PF1550_REMOTE_RegulatorLdo3:
            module = kPF1550_ModuleLdo3;
            break;

        default:
            module = kPF1550_ModuleVsnvs;
            break;
    }

    return module;
}

static void PF1550_REMOTE_ProcessRemoteRequest(pf1550_serv_handle_t *handle, pf1550_remote_request_t *request)
{
    switch (request->command)
    {
        case PF1550_REMOTE_CommandEnable:
            PF1550_SERV_EnableRegulator(handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator),
                                        kPF1550_OperatingStatusRun, true);
            request->returnCode = SRTM_PMIC_RETURN_CODE_SUCEESS;
            break;

        case PF1550_REMOTE_CommandDisable:
            PF1550_SERV_EnableRegulator(handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator),
                                        kPF1550_OperatingStatusRun, false);
            request->returnCode = SRTM_PMIC_RETURN_CODE_SUCEESS;
            break;

        case PF1550_REMOTE_CommandIsEnabled:
            request->regulatorStatus =
                PF1550_SERV_IsRegulatorEnabled(handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator),
                                               kPF1550_OperatingStatusRun) ?
                    SRTM_PMIC_REGULATOR_STATUS_ENABLED :
                    SRTM_PMIC_REGULATOR_STATUS_DISABLED;
            request->returnCode = SRTM_PMIC_RETURN_CODE_SUCEESS;
            break;

        case PF1550_REMOTE_CommandSetVoltage:
            request->returnCode =
                PF1550_SERV_SetRegulatorOutput(handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator),
                                               kPF1550_OperatingStatusRun, request->voltage) ?
                    SRTM_PMIC_RETURN_CODE_SUCEESS :
                    SRTM_PMIC_RETURN_CODE_FAIL;
            break;

        case PF1550_REMOTE_CommandGetVoltage:
            request->voltage = PF1550_SERV_GetRegulatorOutput(
                handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator), kPF1550_OperatingStatusRun);
            request->returnCode = SRTM_PMIC_RETURN_CODE_SUCEESS;
            break;

        case PF1550_REMOTE_CommandSetRegister:
            request->returnCode = PF1550_SERV_WriteReg(handle, request->reg, request->value) ?
                                      SRTM_PMIC_RETURN_CODE_SUCEESS :
                                      SRTM_PMIC_RETURN_CODE_FAIL;
            break;

        case PF1550_REMOTE_CommandGetRegister:
            request->returnCode = PF1550_SERV_DumpReg(handle, request->reg, &request->value, 1U) ?
                                      SRTM_PMIC_RETURN_CODE_SUCEESS :
                                      SRTM_PMIC_RETURN_CODE_FAIL;
            break;

        case PF1550_REMOTE_CommandSetStandbyVoltage:
            if (0U != request->voltage)
            {
                request->returnCode =
                    PF1550_SERV_SetRegulatorOutput(handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator),
                                                   kPF1550_OperatingStatusStandby, request->voltage) ?
                        SRTM_PMIC_RETURN_CODE_SUCEESS :
                        SRTM_PMIC_RETURN_CODE_FAIL;
            }
            else
            {
                PF1550_SERV_EnableRegulator(handle, PF1550_REMOTE_SERV_GetLocalModuleName(request->regulator),
                                            kPF1550_OperatingStatusStandby, false);
                request->returnCode = SRTM_PMIC_RETURN_CODE_SUCEESS;
            }
            break;

        default:
            request->returnCode = SRTM_PMIC_RETURN_CODE_UNSUPPORTED;
            break;
    }
}

static void PF1550_REMOTE_SERV_Start(pf1550_remote_serv_t *me)
{
    me->srtmChannel = SRTM_Channel_Create(me->srtmHandle, me->srtmChannelId);
    if (!me->srtmChannel)
    {
        assert(false);
    }
    if (SRTM_SUCCESS != SRTM_Channel_RegisterService(me->srtmChannel, SRTM_PMIC_CATEGORY,
                                                     PF1550_REMOTE_SERV_RequestServiceCallback, (void *)me))
    {
        assert(false);
    }

    me->state = PF1550_REMOTE_SERV_StateRun;
}

static void PF1550_REMOTE_SERV_Stop(pf1550_remote_serv_t *me)
{
    SRTM_Channel_Destroy(me->srtmChannel);

    vTaskDelay(200U);
    me->state = PF1550_REMOTE_SERV_StateStart;
}

static void PF1550_REMOTE_SERV_Run(pf1550_remote_serv_t *me)
{
    pf1550_remote_request_t request;
    srtm_response_t *response;

    for (;;)
    {
        xQueueReceive(me->queue, &request, portMAX_DELAY);
        if (PF1550_REMOTE_CommandRestart != request.command)
        {
            PF1550_REMOTE_ProcessRemoteRequest(me->pf1550Handle, &request);
            response = SRTM_Response_Create(me->srtmChannel, SRTM_PMIC_CATEGORY, SRTM_PMIC_VERSION, request.command,
                                            sizeof(pf1550_remote_request_t) - 1U);
            if (!response)
            {
                assert(false);
            }
            SRTM_Response_SetPayload(response, request.data, sizeof(pf1550_remote_request_t) - 1U);
            SRTM_Response_Deliver(response);
        }
        else
        {
            me->state = PF1550_REMOTE_SERV_StateStop;
            break;
        }
    }
}

pf1550_remote_serv_t *PF1550_REMOTE_SERV_Init(const pf1550_remote_serv_config_t *config)
{
    pf1550_remote_serv_t *me;

    me = pvPortMalloc(sizeof(pf1550_remote_serv_t));
    if (!me)
    {
        return NULL;
    }
    me->queue = xQueueCreate(config->queueSize, sizeof(pf1550_remote_request_t));
    if (!me->queue)
    {
        vPortFree(me);

        return NULL;
    }

    me->srtmHandle = config->srtmHandle;
    me->srtmChannel = NULL;
    me->srtmChannelId = config->srtmChannelId;
    me->pf1550Handle = config->pf1550Handle;
    me->lifecycleHandle = config->lifecycleHandle;

    if (0U != LIFECYCLE_SERV_Subscribe(me->lifecycleHandle, PF1550_REMOTE_SERV_LifecycleServEventCallback, (void *)me))
    {
        assert(false);
    }

    me->state = PF1550_REMOTE_SERV_StateStart;

    return me;
}

void PF1550_REMOTE_SERV_ProcessTask(void *pvParameters)
{
    pf1550_remote_serv_t *me = (pf1550_remote_serv_t *)pvParameters;

    for (;;)
    {
        switch (me->state)
        {
            case PF1550_REMOTE_SERV_StateStart:
                PF1550_REMOTE_SERV_Start(me);
                break;

            case PF1550_REMOTE_SERV_StateStop:
                PF1550_REMOTE_SERV_Stop(me);
                break;

            case PF1550_REMOTE_SERV_StateRun:
                PF1550_REMOTE_SERV_Run(me);
                break;

            default:
                break;
        }
    }
}
