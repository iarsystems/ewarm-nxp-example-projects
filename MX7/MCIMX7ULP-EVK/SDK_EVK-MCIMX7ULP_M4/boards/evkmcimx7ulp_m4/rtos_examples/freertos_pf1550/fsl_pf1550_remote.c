/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#include "FreeRTOS.h"
#include "task.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"
#include "fsl_pf1550_serv.h"
#include "fsl_pf1550_remote.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* regulators ID define */
typedef enum pf1550_remote_regulator
{
    PF1550_REMOTE_RegulatorSwitch1 = 0U,
    PF1550_REMOTE_RegulatorSwitch2 = 1U,
    PF1550_REMOTE_RegulatorSwitch3 = 2U,
    PF1550_REMOTE_RegulatorVrefDdr = 3U,
    PF1550_REMOTE_RegulatorLdo1    = 4U,
    PF1550_REMOTE_RegulatorLdo2    = 5U,
    PF1550_REMOTE_RegulatorLdo3    = 6U,
} pf1550_remote_regulator_t;

/* regulator command */
typedef enum _pf1550_remote_command
{
    PF1550_REMOTE_CommandEnable            = 0U,
    PF1550_REMOTE_CommandDisable           = 1U,
    PF1550_REMOTE_CommandIsEnabled         = 2U,
    PF1550_REMOTE_CommandSetVoltage        = 3U,
    PF1550_REMOTE_CommandGetVoltage        = 4U,
    PF1550_REMOTE_CommandGetRegister       = 5U,
    PF1550_REMOTE_CommandSetRegister       = 6U,
    PF1550_REMOTE_CommandSetStandbyVoltage = 7U,
} pf1550_remote_command_t;

/* regulator rpmsg response, Linux will wait for response for every  the regulator command */
typedef enum _pf1550_remote_response
{
    PF1550_REMOTE_ResponseNone       = 0U,
    PF1550_REMOTE_ResponseSuccess    = 1U,
    PF1550_REMOTE_ResponseDisabled   = 2U, /* Used for PF1550_IS_ENABLED command to know regulator DISABLED or SUCCESS(ENABLED). */
    PF1550_REMOTE_ResponseNotAllowed = 3U, /* Sometimes M4 may refused to set the voltage Linux request if this power rail shared by M4 too */
} pf1550_remote_response_t;

/* data format for rpmsg, 4*32bit */
typedef struct _pf1550_remote_request
{
    pf1550_remote_command_t command;
    union {
        pf1550_remote_regulator_t regulator;
        uint32_t reg;
    };
    pf1550_remote_response_t response;
    union {
        uint32_t voltage;
        uint32_t value;
    };
} pf1550_remote_request_t;

/*******************************************************************************
 * Code
 ******************************************************************************/

static pf1550_module_t PF1550_REMOTE_GetLocalModuleName(pf1550_remote_regulator_t regulator)
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
    uint8_t temp;

    switch (request->command)
    {
        case PF1550_REMOTE_CommandEnable:
            PF1550_SERV_EnableRegulator(handle,
                                        PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                        kPF1550_OperatingStatusRun, true);
            request->response = PF1550_REMOTE_ResponseSuccess;
            break;

        case PF1550_REMOTE_CommandDisable:
            PF1550_SERV_EnableRegulator(handle,
                                        PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                        kPF1550_OperatingStatusRun, false);
            request->response = PF1550_REMOTE_ResponseSuccess;
            break;

        case PF1550_REMOTE_CommandIsEnabled:
            request->response = PF1550_SERV_IsRegulatorEnabled(handle,
                                                               PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                                               kPF1550_OperatingStatusRun) ?
                                                               PF1550_REMOTE_ResponseSuccess :
                                                               PF1550_REMOTE_ResponseDisabled;
            break;

        case PF1550_REMOTE_CommandSetVoltage:
            request->response = PF1550_SERV_SetRegulatorOutput(handle,
                                                               PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                                               kPF1550_OperatingStatusRun,
                                                               request->voltage) ?
                                                               PF1550_REMOTE_ResponseSuccess :
                                                               PF1550_REMOTE_ResponseNone;
            break;

        case PF1550_REMOTE_CommandGetVoltage:
            request->voltage = PF1550_SERV_GetRegulatorOutput(handle,
                                                              PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                                              kPF1550_OperatingStatusRun);
            request->response = PF1550_REMOTE_ResponseSuccess;
            break;

        case PF1550_REMOTE_CommandSetRegister:
            request->response = PF1550_SERV_WriteReg(handle, request->reg, request->value) ?
                                                     PF1550_REMOTE_ResponseSuccess :
                                                     PF1550_REMOTE_ResponseNone;
            break;

        case PF1550_REMOTE_CommandGetRegister:
            request->response = PF1550_SERV_DumpReg(handle, request->reg, &temp, 1U) ?
                                                    PF1550_REMOTE_ResponseSuccess :
                                                    PF1550_REMOTE_ResponseNone;
            request->value = temp;
            break;

        case PF1550_REMOTE_CommandSetStandbyVoltage:
            if (0U != request->voltage)
            {
                request->response = PF1550_SERV_SetRegulatorOutput(handle,
                                                                   PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                                                   kPF1550_OperatingStatusStandby,
                                                                   request->voltage) ?
                                                                   PF1550_REMOTE_ResponseSuccess :
                                                                   PF1550_REMOTE_ResponseNone;
            }
            else
            {
                PF1550_SERV_EnableRegulator(handle,
                                            PF1550_REMOTE_GetLocalModuleName(request->regulator),
                                            kPF1550_OperatingStatusStandby, false);
                request->response = PF1550_REMOTE_ResponseSuccess;
            }
            break;

        default:
            request->response = PF1550_REMOTE_ResponseNotAllowed;
            break;
    }
}

void PF1550_REMOTE_Init(pf1550_remote_handle_t *handle, const pf1550_remote_config_t *config)
{
    /* Initialize PF1550 Remote handle. */
    handle->serviceHandle         = config->serviceHandle;
    handle->rpmsgShareMemBase     = config->rpmsgShareMemBase;
    handle->rpmsgLinkId           = config->rpmsgLinkId;
    handle->rpmsgLocalEpAddr      = config->rpmsgLocalEpAddr;
    handle->rpmsgNsAnnounceString = config->rpmsgNsAnnounceString;
    handle->rpmsgWakeupInit       = config->rpmsgWakeupInit;
}

void PF1550_REMOTE_Task(void *pvParameters)
{
    unsigned long remoteAddress;
    pf1550_remote_handle_t *handle = (pf1550_remote_handle_t *)pvParameters;
    pf1550_remote_request_t request;

    /* Initialize RPMSG-Lite Stack. */
    handle->rpmsgHandle = rpmsg_lite_remote_init((void *)handle->rpmsgShareMemBase,
                                                 handle->rpmsgLinkId, RL_NO_FLAGS);

    /* Reset from VLLS Wakeup. */
    if (handle->rpmsgWakeupInit)
    {
        handle->rpmsgHandle->link_state = 1U;

        /* FIXME: THIS IS JUST A WORKAROUND TO SUSPEND/RESUME CASE FOR REMOTE SIDE.
         * Here assumes each link up is clean and all buffers are unused
         * tvq available buffers should be full, and rvq available buffers should be 0 */
        handle->rpmsgHandle->tvq->vq_available_idx = handle->rpmsgHandle->tvq->vq_ring.avail->idx -
                                                     handle->rpmsgHandle->tvq->vq_nentries;
        handle->rpmsgHandle->rvq->vq_available_idx = handle->rpmsgHandle->rvq->vq_ring.avail->idx;
    }
    else
    {
        /* Wait until RPMSG_Lite Link Up. */
        while (!rpmsg_lite_is_link_up(handle->rpmsgHandle))
        {
            vTaskDelay(10U);
        }
    }

    /* Create PMIC Remote Receive Queue. */
    handle->queue = rpmsg_queue_create(handle->rpmsgHandle);

    /* Create PMIC Remote Service Endpoint. */
    handle->rpmsgEndpoint = rpmsg_lite_create_ept(handle->rpmsgHandle,
                                                  handle->rpmsgLocalEpAddr,
                                                  rpmsg_queue_rx_cb, handle->queue);

    if (!(handle->rpmsgWakeupInit))
    {
        /* Announce PMIC Remote Control Endpoint. */
        rpmsg_ns_announce(handle->rpmsgHandle, handle->rpmsgEndpoint,
                          (char *)handle->rpmsgNsAnnounceString, RL_NS_CREATE);
    }

    for(;;)
    {
        /* Get PMIC Service from Remote Processor. */
        rpmsg_queue_recv(handle->rpmsgHandle, handle->queue, &remoteAddress,
                         (char *)&request, sizeof(pf1550_remote_request_t), NULL, RL_BLOCK);
        /* Decode and execute remote request. */
        PF1550_REMOTE_ProcessRemoteRequest(handle->serviceHandle, &request);
        /* Send Request Response back to Remote Processor. */
        rpmsg_lite_send(handle->rpmsgHandle, handle->rpmsgEndpoint, (unsigned long)remoteAddress,
                        (char *)&request, sizeof(pf1550_remote_request_t), RL_BLOCK);
    }
}
