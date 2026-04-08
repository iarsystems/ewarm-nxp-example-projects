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

#ifndef _FSL_PF1550_REMOTE_H_
#define _FSL_PF1550_REMOTE_H_

#include "FreeRTOS.h"
#include "task.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "fsl_pf1550_serv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _pf1550_remote_handle
{
    pf1550_serv_handle_t *serviceHandle;
    struct rpmsg_lite_instance * volatile rpmsgHandle;
    struct rpmsg_lite_endpoint * volatile rpmsgEndpoint;
    volatile rpmsg_queue_handle queue;
    uint32_t rpmsgShareMemBase;
    uint32_t rpmsgLinkId;
    uint32_t rpmsgLocalEpAddr;
    uint8_t* rpmsgNsAnnounceString;
    bool rpmsgWakeupInit;
} pf1550_remote_handle_t;

typedef struct _pf1550_remote_config
{
    pf1550_serv_handle_t *serviceHandle;
    uint32_t rpmsgShareMemBase;
    uint32_t rpmsgLinkId;
    uint32_t rpmsgLocalEpAddr;
    uint8_t* rpmsgNsAnnounceString;
    bool rpmsgWakeupInit;
} pf1550_remote_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes PF1550 Remote Control Service.
 *
 * This function initializes the PF1550 remote control service with user-defined settings.
 * This example shows how to set up the pf1550_config_t parameters and how
 * to call the PF1550_REMOTE_Init function by passing in these parameters.
 * @code
 *   pf1550_remote_config_t pf1550RemoteConfig;
 *   pf1550RemoteConfig.serviceHandle = pf1550ServiceHadnle;
 *   PF1550_REMOTE_Init(&pf1550RemoteHandle, &pf1550RemoteConfig);
 * @endcode
 *
 * @param handle PF1550 Remote Control Service Handle.
 * @param config Pointer to the user-defined configuration structure.
 */
void PF1550_REMOTE_Init(pf1550_remote_handle_t *handle, const pf1550_remote_config_t *config);

/*!
 * @brief PF1550 Remote Service Task.
 *
 * This function is used to service the PMIC control request from remote
 * core. This function should be registered to FreeRTOS and called by
 * RTOS Kernel. User need to provide the priority and stack size to the
 * task for the PMIC service task. This task is relying on PMIC Service
 * task and RPMSG_Lite stack.
 *
 * @param pvParameters Pointer to a valid PF1550 service instance structure.
 */
void PF1550_REMOTE_Task(void *pvParameters);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_PF1550_REMOTE_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
