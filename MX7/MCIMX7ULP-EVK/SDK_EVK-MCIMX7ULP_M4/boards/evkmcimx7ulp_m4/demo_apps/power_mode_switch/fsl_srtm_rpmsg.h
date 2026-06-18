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

#ifndef _FSL_SRTM_RPMSG_H_
#define _FSL_SRTM_RPMSG_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_srtm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SRTM_CHANNEL_TYPE_RPMSG_REMOTE (0x0)

typedef struct _srtm_rpmsg_remote_context
{
    uint16_t tvq_available_idx;
    uint16_t rvq_available_idx;
} srtm_rpmsg_remote_context_t;

typedef struct _srtm_rpmsg_remote_config
{
    uint32_t linkId;
    uint32_t shareMemBase;
    uint32_t localEpAddr;
    uint32_t remoteEpAddr;
    uint8_t *announceString;
    srtm_rpmsg_remote_context_t *context;
} srtm_rpmsg_remote_config_t;

typedef void (*srtm_rpmsg_rx_cb_t)(void *handle, uint8_t *payload, uint8_t payloadLen, void *arg);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

void *SRTM_RPMSG_Remote_Create(const srtm_rpmsg_remote_config_t *config, srtm_rpmsg_rx_cb_t callback, void *arg, bool wakeup);
void SRTM_RPMSG_Remote_Destroy(void *handle);
int32_t SRTM_RPMSG_Remote_IsReady(void *handle);
int32_t SRTM_RPMSG_Remote_Send(void *handle, uint8_t *data, uint32_t size);
int32_t SRTM_RPMSG_Remote_ReleaseRxBuffer(void *handle, void *rxBuffer);
void SRTM_RPMSG_Remote_Suspend(void *handle);
void SRTM_RPMSG_Remote_Resume(void *handle);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_SRTM_RPMSG_H_ */
