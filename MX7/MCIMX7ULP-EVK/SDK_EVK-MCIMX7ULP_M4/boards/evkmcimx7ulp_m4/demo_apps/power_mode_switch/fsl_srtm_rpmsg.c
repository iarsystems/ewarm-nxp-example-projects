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
#include <assert.h>
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_srtm.h"
#include "fsl_srtm_rpmsg.h"
#include "rpmsg_lite.h"
#include "rpmsg_ns.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _srtm_rpmsg_remote_handle
{
    struct rpmsg_lite_instance *volatile rpmsgHandle;
    struct rpmsg_lite_endpoint *volatile rpmsgEndpoint;
    uint32_t remoteEpAddr;
    srtm_rpmsg_rx_cb_t callback;
    void *arg;
    srtm_rpmsg_remote_context_t *context;
} srtm_rpmsg_remote_handle_t;

/*******************************************************************************
 * Code
 ******************************************************************************/
static int SRTM_RPMSG_Endpoint_RX_Callback(void *payload, int payloadLen, unsigned long src, void *priv)
{
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)priv;

    if (privateHandle->callback)
    {
        privateHandle->callback((void *)privateHandle, payload, payloadLen, privateHandle->arg);

        return RL_HOLD;
    }

    return RL_RELEASE;
}

void *SRTM_RPMSG_Remote_Create(const srtm_rpmsg_remote_config_t *config, srtm_rpmsg_rx_cb_t callback, void *arg, bool wakeup)
{
    assert(config);

    srtm_rpmsg_remote_handle_t *privateHandle;

    privateHandle = pvPortMalloc(sizeof(srtm_rpmsg_remote_handle_t));
    privateHandle->rpmsgHandle = rpmsg_lite_remote_init((void *)config->shareMemBase, config->linkId, RL_NO_FLAGS);
    if (!privateHandle->rpmsgHandle)
    {
        vPortFree(privateHandle);

        return NULL;
    }

    if (true == wakeup)
    {
        privateHandle->rpmsgHandle->link_state = 1U;
    }
    else
    {
        while (!rpmsg_lite_is_link_up(privateHandle->rpmsgHandle))
        {
            vTaskDelay(10U);
        }
    }

    privateHandle->remoteEpAddr = config->remoteEpAddr;
    privateHandle->context = config->context;
    privateHandle->callback = callback;
    privateHandle->arg = arg;
    privateHandle->rpmsgEndpoint = rpmsg_lite_create_ept(privateHandle->rpmsgHandle, config->localEpAddr,
                                                         SRTM_RPMSG_Endpoint_RX_Callback, (void *)privateHandle);

    if (!wakeup)
    {
        rpmsg_ns_announce(privateHandle->rpmsgHandle, privateHandle->rpmsgEndpoint, (char *)config->announceString,
                          RL_NS_CREATE);
    }

    return privateHandle;
}

void SRTM_RPMSG_Remote_Destroy(void *handle)
{
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)handle;

    assert(handle);

    rpmsg_lite_destroy_ept(privateHandle->rpmsgHandle, privateHandle->rpmsgEndpoint);
    rpmsg_lite_deinit(privateHandle->rpmsgHandle);

    vPortFree(privateHandle);
}

int32_t SRTM_RPMSG_Remote_IsReady(void *handle)
{
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)handle;

    assert(handle);

    return rpmsg_lite_is_link_up(privateHandle->rpmsgHandle) ? SRTM_READY : SRTM_NOT_READT;
}

int32_t SRTM_RPMSG_Remote_Send(void *handle, uint8_t *data, uint32_t size)
{
    int32_t result;
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)handle;

    assert(handle);
    assert(data);

    result = rpmsg_lite_send(privateHandle->rpmsgHandle, privateHandle->rpmsgEndpoint, privateHandle->remoteEpAddr,
                             (char *)data, size, RL_BLOCK);

    return (RL_SUCCESS == result) ? SRTM_SUCCESS : SRTM_FAIL;
}

int32_t SRTM_RPMSG_Remote_ReleaseRxBuffer(void *handle, void *rxBuffer)
{
    int32_t result;
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)handle;

    result = rpmsg_lite_release_rx_buffer(privateHandle->rpmsgHandle, rxBuffer);

    return (RL_SUCCESS == result) ? SRTM_SUCCESS : SRTM_FAIL;
}

void SRTM_RPMSG_Remote_Suspend(void *handle)
{
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)handle;

    assert(handle);

    privateHandle->context->tvq_available_idx = privateHandle->rpmsgHandle->tvq->vq_available_idx;
    privateHandle->context->rvq_available_idx = privateHandle->rpmsgHandle->rvq->vq_available_idx;
}

void SRTM_RPMSG_Remote_Resume(void *handle)
{
    srtm_rpmsg_remote_handle_t *privateHandle = (srtm_rpmsg_remote_handle_t *)handle;

    assert(handle);

    privateHandle->rpmsgHandle->tvq->vq_available_idx = privateHandle->context->tvq_available_idx;
    privateHandle->rpmsgHandle->rvq->vq_available_idx = privateHandle->context->rvq_available_idx;
}
