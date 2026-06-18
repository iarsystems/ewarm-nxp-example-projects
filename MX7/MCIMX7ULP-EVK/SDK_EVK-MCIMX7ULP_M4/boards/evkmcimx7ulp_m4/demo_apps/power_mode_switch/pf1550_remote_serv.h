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

#ifndef _PF1550_REMOTE_SERV_H_
#define _PF1550_REMOTE_SERV_H_

#include "fsl_pf1550_serv.h"
#include "fsl_srtm.h"
#include "lifecycle_serv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _pf1550_remote_serv pf1550_remote_serv_t;
typedef struct _pf1550_remote_serv_config
{
    srtm_handle_t *srtmHandle;
    uint32_t srtmChannelId;
    pf1550_serv_handle_t *pf1550Handle;
    lifecycle_serv_t *lifecycleHandle;
    uint32_t queueSize;
} pf1550_remote_serv_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

pf1550_remote_serv_t *PF1550_REMOTE_SERV_Init(const pf1550_remote_serv_config_t *config);
void PF1550_REMOTE_SERV_ProcessTask(void *pvParameters);

#if defined(__cplusplus)
}
#endif

#endif /* _PF1550_REMOTE_SERV_H_ */
