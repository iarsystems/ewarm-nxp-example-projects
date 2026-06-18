/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

/* Standard C Included Files */
#include <stdio.h>
#include <string.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"

#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASE (LPI2C0_BASE)
#define EXAMPLE_I2C_MASTER_IRQN (LPI2C0_IRQn)

#define EXAMPLE_I2C_SLAVE_BASE (LPI2C3_BASE)
#define EXAMPLE_I2C_SLAVE_IRQN (LPI2C3_IRQn)

#define LPI2C_CLOCK_FREQUENCY 48000000U

/* clang-format off */
#define EXAMPLE_I2C_MASTER   ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define EXAMPLE_I2C_SLAVE   ((LPI2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_BAUDRATE (100000) /* 100K */

#define I2C_DATA_LENGTH (32) /* MAX is 256 */

/* Task priorities. */
#define lpi2c_task_PRIORITY (configMAX_PRIORITIES - 1)
/* clang-format on */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
uint8_t g_master_buff[I2C_DATA_LENGTH];

lpi2c_master_handle_t g_m_handle;
lpi2c_slave_handle_t g_s_handle;
volatile uint32_t timeout = 100000;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define slave_task_PRIORITY (configMAX_PRIORITIES - 2)
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)

#define LPI2C_NVIC_PRIO 2

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void slave_task(void *pvParameters);
static void master_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select the clock source for the LPI2C0 module as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);

    /* Select the clock source for the LPI2C3 module as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcFircAsync);
    PRINTF("\r\nLPI2C example -- MasterInterrupt_SlaveInterrupt.\r\n");

    if (xTaskCreate(slave_task, "Slave_task", configMINIMAL_STACK_SIZE + 64, NULL, slave_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Failed to create slave task");
    }

    vTaskStartScheduler();

    while (1)
    {
    }
}

/*!
 * @brief Data structure and callback function for slave I2C communication.
 */

typedef struct _callback_message_t
{
    status_t async_status;
    SemaphoreHandle_t sem;
} callback_message_t;

static void lpi2c_slave_callback(LPI2C_Type *base, lpi2c_slave_transfer_t *xfer, void *userData)
{
    callback_message_t *cb_msg = (callback_message_t *)userData;
    BaseType_t reschedule;

    switch (xfer->event)
    {
        case kLPI2C_SlaveReceiveEvent:
            xfer->data = g_slave_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;
        case kLPI2C_SlaveCompletionEvent:
            cb_msg->async_status = xfer->completionStatus;
            xSemaphoreGiveFromISR(cb_msg->sem, &reschedule);
            portYIELD_FROM_ISR(reschedule);
            break;
        default:
            break;
    }
}

/*!
 * @brief Task responsible for slave I2C communication.
 */

static void slave_task(void *pvParameters)
{
    uint32_t i;
    lpi2c_slave_config_t slaveConfig;
    callback_message_t cb_msg;

    cb_msg.sem = xSemaphoreCreateBinary();
    if (cb_msg.sem == NULL)
    {
        PRINTF("I2C slave: Error creating semaphore\r\n");
        vTaskSuspend(NULL);
    }

/*  Set i2c slave interrupt priority higher. */
#if (EXAMPLE_I2C_SLAVE_BASE == LPI2C0_BASE)
    NVIC_SetPriority(LPI2C0_IRQn, LPI2C_NVIC_PRIO);
#elif(EXAMPLE_I2C_SLAVE_BASE == LPI2C1_BASE)
    NVIC_SetPriority(LPI2C1_IRQn, LPI2C_NVIC_PRIO);
#elif(EXAMPLE_I2C_SLAVE_BASE == LPI2C2_BASE)
    NVIC_SetPriority(LPI2C2_IRQn, LPI2C_NVIC_PRIO);
#elif(EXAMPLE_I2C_SLAVE_BASE == LPI2C3_BASE)
    NVIC_SetPriority(LPI2C3_IRQn, LPI2C_NVIC_PRIO);
#if defined(FSL_FEATURE_INTMUX_IRQ_START_INDEX)
    /* Don't try to set LPI2C2 IRQ priority if it is an INTMUX IRQ */
    if (LPI2C2_IRQn < FSL_FEATURE_INTMUX_IRQ_START_INDEX)
#endif /* FSL_FEATURE_INTMUX_IRQ_START_INDEX */
    {
        NVIC_SetPriority(LPI2C2_IRQn, LPI2C_NVIC_PRIO);
    }
#endif

    /* Set up i2c slave first */
    /*
     * slaveConfig.address0 = 0U;
     * slaveConfig.address1 = 0U;
     * slaveConfig.addressMatchMode = kLPI2C_MatchAddress0;
     * slaveConfig.filterDozeEnable = true;
     * slaveConfig.filterEnable = true;
     * slaveConfig.enableGeneralCall = false;
     * slaveConfig.ignoreAck = false;
     * slaveConfig.enableReceivedAddressRead = false;
     * slaveConfig.sdaGlitchFilterWidth_ns = 0;
     * slaveConfig.sclGlitchFilterWidth_ns = 0;
     * slaveConfig.dataValidDelay_ns = 0;
     * slaveConfig.clockHoldTime_ns = 0;
     */
    LPI2C_SlaveGetDefaultConfig(&slaveConfig);

    slaveConfig.address0 = I2C_MASTER_SLAVE_ADDR_7BIT;
    slaveConfig.address1 = 0;
    LPI2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig, LPI2C_CLOCK_FREQUENCY);

    memset(&g_s_handle, 0, sizeof(g_s_handle));
    memset(&g_slave_buff, 0, sizeof(g_slave_buff));

    LPI2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, lpi2c_slave_callback, &cb_msg);
    LPI2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle,
                                   kLPI2C_SlaveReceiveEvent | kLPI2C_SlaveCompletionEvent);

    if (xTaskCreate(master_task, "Master_task", configMINIMAL_STACK_SIZE + 64, NULL, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        vTaskSuspend(NULL);
        PRINTF("Failed to create master task");
    }

    /* Wait for transfer to finish */
    xSemaphoreTake(cb_msg.sem, portMAX_DELAY);

    if (cb_msg.async_status == kStatus_Success)
    {
        PRINTF("I2C slave transfer completed successfully. \r\n\r\n");
    }
    else
    {
        PRINTF("I2C slave transfer completed with error. \r\n\r\n");
    }

    /* Transfer completed. Check the data. */
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occured in this transfer ! \r\n");
            break;
        }
    }

    if (i == 32)
    {
        PRINTF("\r\n Transfer successfully!\r\n ");
    }

    PRINTF("\r\nSlave received data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    vTaskSuspend(NULL);
}

static void master_task(void *pvParameters)
{
    lpi2c_master_config_t masterConfig;
    lpi2c_rtos_handle_t master_rtos_handle;
    lpi2c_master_transfer_t masterXfer;
    status_t status;
    uint32_t i = 0;

#if (EXAMPLE_I2C_MASTER_BASE == LPI2C0_BASE)
    NVIC_SetPriority(LPI2C0_IRQn, LPI2C_NVIC_PRIO + 1);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C1_BASE)
    NVIC_SetPriority(LPI2C1_IRQn, LPI2C_NVIC_PRIO + 1);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C2_BASE)
    NVIC_SetPriority(LPI2C2_IRQn, LPI2C_NVIC_PRIO + 1);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C3_BASE)
    NVIC_SetPriority(LPI2C3_IRQn, LPI2C_NVIC_PRIO + 1);
#if defined(FSL_FEATURE_INTMUX_IRQ_START_INDEX)
    /* Don't try to set LPI2C2 IRQ priority if it is an INTMUX IRQ */
    if (LPI2C2_IRQn < FSL_FEATURE_INTMUX_IRQ_START_INDEX)
#endif /* FSL_FEATURE_INTMUX_IRQ_START_INDEX */
    {
        NVIC_SetPriority(LPI2C2_IRQn, LPI2C_NVIC_PRIO + 1);
    }
#endif

    /* Set up i2c master to send data to slave */
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    PRINTF("Master will send data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = I2C_BAUDRATE;

    status = LPI2C_RTOS_Init(&master_rtos_handle, EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_CLOCK_FREQUENCY);
    if (status != kStatus_Success)
    {
        PRINTF("LPI2C master: Error initializing LPI2C!\r\n");
        vTaskSuspend(NULL);
    }

    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = g_master_buff;
    masterXfer.dataSize = I2C_DATA_LENGTH;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    status = LPI2C_RTOS_Transfer(&master_rtos_handle, &masterXfer);
    if (status != kStatus_Success)
    {
        PRINTF("LPI2C master: Error during transfer !\r\n");
        vTaskSuspend(NULL);
    }

    vTaskSuspend(NULL);
}
