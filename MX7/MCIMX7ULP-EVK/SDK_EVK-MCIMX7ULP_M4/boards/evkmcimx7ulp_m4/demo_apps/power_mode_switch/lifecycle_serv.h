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

#ifndef _LIFECYCLE_SERV_H_
#define _LIFECYCLE_SERV_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_srtm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
enum _lifecycle_serv_event_flags
{
    LIFECYCLE_SERV_EventGetHeartBeatFlag = 0x1U,
    LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag = 0x2U,
    LIFECYCLE_SERV_EventGetHeartBeatEnable = 0x4U,
    LIFECYCLE_SERV_EventGetHeartBeatDisable = 0x8U,
    LIFECYCLE_SERV_EventSwitchToRunModeFlag = 0x10U,
    LIFECYCLE_SERV_EventSwitchToSuspendModeFlag = 0x20U,
    LIFECYCLE_SERV_EventSwitchToRebootModeFlag = 0x40U,
    LIFECYCLE_SERV_EventSwitchToShutDownModeFlag = 0x80U,
};
typedef struct _lifecycle_serv lifecycle_serv_t;
typedef void (*lifecycle_serv_event_cb_t)(lifecycle_serv_t *me, uint32_t event, void *arg);
typedef struct _lifecycle_serv_config
{
    srtm_handle_t *srtmHandle;
    uint32_t srtmChannelId;
    uint32_t heartBeatTimeout_Ms;
    bool heartBeatEnable;
} lifecycle_serv_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

lifecycle_serv_t *LIFECYCLE_SERV_Init(const lifecycle_serv_config_t *config);
int32_t LIFECYCLE_SERV_Subscribe(lifecycle_serv_t *me, lifecycle_serv_event_cb_t callback, void *arg);
int32_t LIFECYCLE_SERV_Unsubscribe(lifecycle_serv_t *me, lifecycle_serv_event_cb_t callback);
void LIFECYCLE_SERV_RebootRemoteCoreFromISR(lifecycle_serv_t *me);
void LIFECYCLE_SERV_ProcessTask(void *pvParameters);

#if defined(__cplusplus)
}
#endif

#endif /* _LIFECYCLE_SERV_H_ */
