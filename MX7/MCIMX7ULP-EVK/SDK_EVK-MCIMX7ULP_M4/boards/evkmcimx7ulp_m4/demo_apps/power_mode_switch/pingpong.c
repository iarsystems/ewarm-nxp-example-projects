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
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_msmc.h"
#include "lifecycle_serv.h"
#include "fsl_srtm_rpmsg.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PINGPONG_RPMSG_SHMEM_BASE         (0x9FF00000U)
#define PINGPONG_RPMSG_LINK_ID            (RL_PLATFORM_IMX7ULP_M4_USER_LINK_ID)
#define PINGPONG_RPMSG_LOCAL_EPT_ADDR     (0x1EU)
#define PINGPONG_RPMSG_REMOTE_EPT_ADDR    (0x400U)
#define PINGPONG_RPMSG_NS_ANNOUNCE_STRING "rpmsg-openamp-demo-channel"

typedef enum _PINGPONG_state {
    PINGPONG_StateStart = 0U,
    PINGPONG_StateStop = 1U,
    PINGPONG_StateRun = 2U,
} PINGPONG_state_t;

typedef struct the_message
{
    uint32_t DATA;
} volatile THE_MESSAGE, *THE_MESSAGE_PTR;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern srtm_rpmsg_remote_context_t rpmsgContext[4];
static volatile PINGPONG_state_t state;
static struct rpmsg_lite_instance * volatile my_rpmsg;
static struct rpmsg_lite_endpoint * volatile my_ept;
static volatile rpmsg_queue_handle my_queue;
volatile THE_MESSAGE msg = {0};
static uint8_t hello[12U] = {0};
static bool wakeupFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void PINGPONG_LifecycleServEventCallback(lifecycle_serv_t *lifecycleHandle, uint32_t event, void *arg)
{
    if (event & (LIFECYCLE_SERV_EventGetHeartBeatTimeoutFlag | LIFECYCLE_SERV_EventSwitchToRebootModeFlag |
                 LIFECYCLE_SERV_EventSwitchToShutDownModeFlag))
    {
        state = PINGPONG_StateStop;
    }
    else if (event & LIFECYCLE_SERV_EventSwitchToSuspendModeFlag)
    {
        if (my_rpmsg)
        {
            rpmsgContext[3].tvq_available_idx = my_rpmsg->tvq->vq_available_idx;
            rpmsgContext[3].rvq_available_idx = my_rpmsg->rvq->vq_available_idx;
        }
    }
    else
    {
    }
}

static void PINGPONG_Start(void)
{
    my_rpmsg = rpmsg_lite_remote_init((void *)PINGPONG_RPMSG_SHMEM_BASE, PINGPONG_RPMSG_LINK_ID, RL_NO_FLAGS);

    if (!wakeupFlag)
    {
        while (!rpmsg_lite_is_link_up(my_rpmsg))
        {
            vTaskDelay(10U);
        }
    }
    else
    {
        my_rpmsg->link_state = 1U;

        my_rpmsg->tvq->vq_available_idx = rpmsgContext[3].tvq_available_idx;
        my_rpmsg->rvq->vq_available_idx = rpmsgContext[3].rvq_available_idx;
    }

    my_queue = rpmsg_queue_create(my_rpmsg);
    my_ept = rpmsg_lite_create_ept(my_rpmsg, PINGPONG_RPMSG_LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, my_queue);

    if (!wakeupFlag)
    {
        rpmsg_ns_announce(my_rpmsg, my_ept, PINGPONG_RPMSG_NS_ANNOUNCE_STRING, RL_NS_CREATE);
    }

    wakeupFlag = false;
    state = PINGPONG_StateRun;
}

static void PINGPONG_Stop(void)
{
    rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
    my_ept = NULL;
    rpmsg_queue_destroy(my_rpmsg, my_queue);
    my_queue = NULL;
    rpmsg_lite_deinit(my_rpmsg);
    my_rpmsg = NULL;

    vTaskDelay(200U);
    state = PINGPONG_StateStart;
}

static void PINGPONG_Run(void)
{
    volatile unsigned long remote_addr;
    int32_t result;

    for (;;)
    {
        result = rpmsg_queue_recv(my_rpmsg, my_queue, (unsigned long*)&remote_addr,
                                  (char *)hello, sizeof(hello), NULL, 50U);
        if (RL_SUCCESS == result)
        {
            break;
        }
        else
        {
            if (PINGPONG_StateStop == state)
            {
                return;
            }
        }
    }

    for (;;)
    {
        result = rpmsg_queue_recv(my_rpmsg, my_queue, (unsigned long*)&remote_addr,
                                  (char *)&msg, sizeof(THE_MESSAGE), NULL, 50U);
        if (RL_SUCCESS == result)
        {
            msg.DATA++;
            rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg,
                            sizeof(THE_MESSAGE), 0U);
        }
        else
        {
            if (PINGPONG_StateStop == state)
            {
                break;
            }
        }
    }
}

void PINGPONG_Task(void *pvParameters)
{
    lifecycle_serv_t *lifeCycleHandle = (lifecycle_serv_t *)pvParameters;

    state = PINGPONG_StateStart;

    if (kSMC_SourceWakeup & SMC_GetPreviousResetSources(MSMC0))
    {
        wakeupFlag = true;
    }
    else
    {
        wakeupFlag = false;
    }

    if (0U != LIFECYCLE_SERV_Subscribe(lifeCycleHandle, PINGPONG_LifecycleServEventCallback, NULL))
    {
        assert(false);
    }

    for (;;)
    {
        switch (state)
        {
            case PINGPONG_StateStart:
                PINGPONG_Start();
                break;

            case PINGPONG_StateStop:
                PINGPONG_Stop();
                break;

            case PINGPONG_StateRun:
                PINGPONG_Run();
                break;

            default:
                break;
        }
    }
}
