/*
* Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_freertos.h"
#include "board.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

#include "pin_mux.h"
#include "fsl_common.h"
/*******************************************************************************
* Definitions
******************************************************************************/
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_BASEADDR LPSPI0
#define EXAMPLE_LPSPI_MASTER_IRQN LPSPI0_IRQn
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT kLPSPI_Pcs3
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs3

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ (CLOCK_GetIpFreq(kCLOCK_Lpspi0))

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR LPSPI1
#define EXAMPLE_LPSPI_SLAVE_IRQN LPSPI1_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define TRANSFER_SIZE (512U)        /*! Transfer dataSize.*/
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
* Prototypes
******************************************************************************/
/* LPSPI user callback */
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData);
extern uint32_t LPSPI_GetInstance(LPSPI_Type *base);

/*******************************************************************************
* Variables
******************************************************************************/
lpspi_slave_handle_t g_s_handle;

uint8_t masterReceiveBuffer[TRANSFER_SIZE] = {0};
uint8_t masterSendBuffer[TRANSFER_SIZE] = {0};
uint8_t slaveReceiveBuffer[TRANSFER_SIZE] = {0};
uint8_t slaveSendBuffer[TRANSFER_SIZE] = {0};

SemaphoreHandle_t lpspi_sem;
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define slave_task_PRIORITY (configMAX_PRIORITIES - 2)
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)

#define LPSPI_NVIC_PRIO 2

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void slave_task(void *pvParameters);
static void master_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    int i;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(kCLOCK_Lpspi0, kCLOCK_IpSrcFircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);

/* In case the LPSPI IRQ is handled by INTMUX, setup INTMUX. */
#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    INTMUX_Init(INTMUX0);
    INTMUX_SetChannelMode(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNEL, kINTMUX_ChannelLogicOR);
#endif

    PRINTF("FreeRTOS LPSPI example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on a single board.\r\n");
    PRINTF("Master and slave are both use interrupt way.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is:\r\n");
    PRINTF("LPSPI_master -- LPSPI_slave\r\n");
    PRINTF("    CLK      --    CLK\r\n");
    PRINTF("    PCS      --    PCS\r\n");
    PRINTF("    SOUT     --    SIN\r\n");
    PRINTF("    SIN      --    SOUT\r\n");
    PRINTF("\r\n");

    /* Initialize data in transfer buffers */
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        masterSendBuffer[i] = i % 256;
        masterReceiveBuffer[i] = 0;

        slaveSendBuffer[i] = ~masterSendBuffer[i];
        slaveReceiveBuffer[i] = 0;
    }

    xTaskCreate(slave_task, "Slave_task", configMINIMAL_STACK_SIZE + 64, NULL, slave_task_PRIORITY, NULL);
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Data structure and callback function for slave SPI communication.
 */

typedef struct _callback_message_t
{
    status_t async_status;
    SemaphoreHandle_t sem;
} callback_message_t;

void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData)
{
    callback_message_t *cb_msg = (callback_message_t *)userData;
    BaseType_t reschedule;

    cb_msg->async_status = status;
    xSemaphoreGiveFromISR(cb_msg->sem, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/*!
 * @brief Task responsible for slave SPI communication.
 */
static void slave_task(void *pvParameters)
{
    lpspi_slave_config_t slaveConfig;
    lpspi_transfer_t slaveXfer;

    uint32_t errorCount;
    uint32_t i;
    callback_message_t cb_msg;

    cb_msg.sem = xSemaphoreCreateBinary();
    lpspi_sem = cb_msg.sem;
    if (cb_msg.sem == NULL)
    {
        PRINTF("LPSPI slave: Error creating semaphore\r\n");
        vTaskSuspend(NULL);
    }

    /* Slave config */
    LPSPI_SlaveGetDefaultConfig(&slaveConfig);

    slaveConfig.bitsPerFrame = 8 * TRANSFER_SIZE;
    slaveConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    slaveConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    slaveConfig.direction = kLPSPI_MsbFirst;

    slaveConfig.whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;
    slaveConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    slaveConfig.pinCfg = kLPSPI_SdiInSdoOut;
    slaveConfig.dataOutConfig = kLpspiDataOutRetained;

/* In case the LPSPI IRQ is handled by INTMUX, setup both NVIC and INTMUX. */
#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    if (EXAMPLE_LPSPI_SLAVE_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNEL, EXAMPLE_LPSPI_SLAVE_IRQN);
        NVIC_SetPriority(INTMUX0_0_IRQn, LPSPI_NVIC_PRIO);
    }
    else
    {
        NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, LPSPI_NVIC_PRIO);
    }
#else
    NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, LPSPI_NVIC_PRIO);
#endif

    LPSPI_SlaveInit(EXAMPLE_LPSPI_SLAVE_BASEADDR, &slaveConfig);

    /*Set up slave first */
    LPSPI_SlaveTransferCreateHandle(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_handle, LPSPI_SlaveUserCallback, &cb_msg);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData = slaveSendBuffer;
    slaveXfer.rxData = slaveReceiveBuffer;
    slaveXfer.dataSize = TRANSFER_SIZE;
    slaveXfer.configFlags = EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER | kLPSPI_SlaveByteSwap;

    LPSPI_SlaveTransferNonBlocking(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);

    xTaskCreate(master_task, "Master_task", configMINIMAL_STACK_SIZE + 64, NULL, master_task_PRIORITY, NULL);

    /* Wait for transfer to finish */
    xSemaphoreTake(cb_msg.sem, portMAX_DELAY);
    if (cb_msg.async_status == kStatus_Success)
    {
        PRINTF("LPSPI slave transfer completed successfully.\r\n");
    }
    else
    {
        PRINTF("LPSPI slave transfer completed with error.\r\n");
    }

    errorCount = 0;
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        if (slaveReceiveBuffer[i] != masterSendBuffer[i])
        {
            errorCount++;
        }
    }

    if (errorCount == 0)
    {
        PRINTF("Master-to-slave data verified ok.\r\n");
    }
    else
    {
        PRINTF("Mismatch in master-to-slave data!\r\n");
    }

    vTaskSuspend(NULL);
}

/*!
 * @brief Task responsible for master SPI communication.
 */
static void master_task(void *pvParameters)
{
    lpspi_transfer_t masterXfer;
    lpspi_rtos_handle_t master_rtos_handle;
    lpspi_master_config_t masterConfig;
    uint32_t sourceClock;
    status_t status;

    uint32_t errorCount;
    uint32_t i;

    /*
     * masterConfig.bitsPerFrame = 8;
     * masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
     * masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
     * masterConfig.direction = kLPSPI_MsbFirst;
     * masterConfig.whichPcs = kLPSPI_Pcs0;
     * masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
     * masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
     * masterConfig.dataOutConfig = kLpspiDataOutRetained;
     */
    LPSPI_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame = 8 * TRANSFER_SIZE;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

/* In case the LPSPI IRQ is handled by INTMUX, setup both NVIC and INTMUX. */
#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    if (EXAMPLE_LPSPI_MASTER_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNEL, EXAMPLE_LPSPI_MASTER_IRQN);
        NVIC_SetPriority(INTMUX0_0_IRQn, LPSPI_NVIC_PRIO + 1);
    }
    else
    {
        NVIC_SetPriority(EXAMPLE_LPSPI_MASTER_IRQN, LPSPI_NVIC_PRIO + 1);
    }
#else
    NVIC_SetPriority(EXAMPLE_LPSPI_MASTER_IRQN, LPSPI_NVIC_PRIO + 1);
#endif

    sourceClock = EXAMPLE_LPSPI_MASTER_CLOCK_FREQ;

    status = LPSPI_RTOS_Init(&master_rtos_handle, EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, sourceClock);

    if (status != kStatus_Success)
    {
        PRINTF("LPSPI master: error during initialization. \r\n");
        vTaskSuspend(NULL);
    }
    /*Start master transfer*/
    masterXfer.txData = masterSendBuffer;
    masterXfer.rxData = masterReceiveBuffer;
    masterXfer.dataSize = TRANSFER_SIZE;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap;

    status = LPSPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);

    if (status == kStatus_Success)
    {
        PRINTF("LPSPI master transfer completed successfully.\r\n");
    }
    else
    {
        PRINTF("LPSPI master transfer completed with error.\r\n");
    }

    errorCount = 0;
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        if (masterReceiveBuffer[i] != slaveSendBuffer[i])
        {
            errorCount++;
        }
    }

    if (errorCount == 0)
    {
        PRINTF("Slave-to-master data verified ok.\r\n");
    }
    else
    {
        PRINTF("Mismatch in slave-to-master data!\r\n");
    }

    vTaskSuspend(NULL);
}
