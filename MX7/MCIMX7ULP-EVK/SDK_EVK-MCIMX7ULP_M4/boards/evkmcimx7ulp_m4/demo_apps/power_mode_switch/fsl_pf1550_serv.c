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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "fsl_pf1550.h"
#include "fsl_pf1550_serv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _pf1550_serv_request_api
{
    kPF1550_SERV_RequestApiWriteReg           = 0U,
    kPF1550_SERV_RequestApiDumpReg            = 1U,
    kPF1550_SERV_RequestApiEnableRegulator    = 2U,
    kPF1550_SERV_RequestApiSetRegulatorOutput = 3U,
    kPF1550_SERV_RequestApiGetRegulatorOutput = 4U,
    kPF1550_SERV_RequestIsRegulatorEnabled    = 5U,
    kPF1550_SERV_RequestGetOperatingStatus    = 6U,
    kPF1550_SERV_RequestHandleIRQ             = 7U,
} pf1550_serv_request_api_t;

typedef struct _pf1550_serv_write_reg_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    uint8_t reg;
    uint8_t value;
    bool *result;
} pf1550_serv_write_reg_request_t;

typedef struct _pf1550_serv_dump_reg_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    uint8_t *buffer;
    uint8_t size;
    uint8_t reg;
    bool *result;
} pf1550_serv_dump_reg_request_t;

typedef struct _pf1550_serv_enable_regulator_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    pf1550_module_t module;
    pf1550_operating_status_t status;
    bool enable;
} pf1550_serv_enable_regulator_request_t;

typedef struct _pf1550_serv_set_regulator_output_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    pf1550_module_t module;
    pf1550_operating_status_t status;
    uint32_t voltage;
    bool *result;
} pf1550_serv_set_regulator_output_request_t;

typedef struct _pf1550_serv_get_regulator_output_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    pf1550_module_t module;
    pf1550_operating_status_t status;
    uint32_t *voltage;
} pf1550_serv_get_regulator_output_request_t;

typedef struct _pf1550_serv_is_regulator_enabled_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    pf1550_module_t module;
    pf1550_operating_status_t status;
    bool *enable;
} pf1550_serv_is_regulator_enabled_request_t;

typedef struct _pf1550_serv_get_operating_status_request
{
    pf1550_serv_request_api_t requestApi;
    SemaphoreHandle_t semaphore;
    pf1550_operating_status_t *status;
} pf1550_serv_get_operating_status_request_t;

typedef struct _pf1550_serv_handle_irq_request
{
    pf1550_serv_request_api_t requestApi;
} pf1550_serv_handle_irq_request_t;

typedef union _pf1550_serv_common_request
{
    pf1550_serv_request_api_t requestApi;
    pf1550_serv_write_reg_request_t writeRegRequest;
    pf1550_serv_dump_reg_request_t dumpRegRequest;
    pf1550_serv_enable_regulator_request_t enableRegulatorRequest;
    pf1550_serv_set_regulator_output_request_t setRegulatorOutputRequest;
    pf1550_serv_get_regulator_output_request_t getRegulatorOutputRequest;
    pf1550_serv_is_regulator_enabled_request_t isRegulatorEnabledRequest;
    pf1550_serv_get_operating_status_request_t getOperatingStatusRequest;
    pf1550_serv_handle_irq_request_t handleIrqRequest;
} pf1550_serv_common_request_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void PF1550_SERV_ProcessWriteRegRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_write_reg_request_t *request);
static void PF1550_SERV_ProcessDumpRegRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_dump_reg_request_t *request);
static void PF1550_SERV_ProcessEnableRegultorRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_enable_regulator_request_t *request);
static void PF1550_SERV_ProcessSetRegulatorOutputRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_set_regulator_output_request_t *request);
static void PF1550_SERV_ProcessGetRegulatorOutputRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_get_regulator_output_request_t *request);
static void PF1550_SERV_ProcessIsRegulatorEnabledRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_is_regulator_enabled_request_t *request);
static void PF1550_SERV_ProcessGetOperatingStatusRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_get_operating_status_request_t *request);
static void PF1550_SERV_ProcessHandleIrqRequest(pf1550_serv_handle_t *handle);

/*******************************************************************************
 * Code
 ******************************************************************************/

void PF1550_SERV_HandleIRQ(pf1550_serv_handle_t *handle)
{
    pf1550_serv_handle_irq_request_t handleIrqRequest;
    BaseType_t reschedule;

    if (NULL != handle->PF1550_SERV_CallbackFunc)
    {
        /* Initialize PF1550 Service Request Structure. */
        handleIrqRequest.requestApi = kPF1550_SERV_RequestHandleIRQ;

        /* Send request to PF1550 Service Task and return immediately. */
        xQueueSendFromISR(handle->queue, (void *)&handleIrqRequest, &reschedule);
        portYIELD_FROM_ISR(reschedule);
    }
}

void PF1550_SERV_GetDefaultConfig(pf1550_serv_config_t *config)
{
    /* Get high level service default config. */
    config->PF1550_SERV_CallbackFunc = NULL;
    config->queueSize = 10U;
    config->outputWaitTicks = 1U;

    /* Get low level driver default config. */
    PF1550_GetDefaultConfig(&config->drvConfig);
}

void PF1550_SERV_Init(pf1550_serv_handle_t *handle, const pf1550_serv_config_t *config)
{
    /* Initialize underlying PF1550 driver configure structure. */
    handle->drvConfig = config->drvConfig;

    /* Register event notification function. */
    handle->PF1550_SERV_CallbackFunc = config->PF1550_SERV_CallbackFunc;

    /* Create event queue used in PMIC Service Task. */
    handle->queue = xQueueCreate(config->queueSize, sizeof(pf1550_serv_common_request_t));

    /* Initialize Regulator Output Wait Ticks Count. */
    handle->outputWaitTicks = config->outputWaitTicks;
}

bool PF1550_SERV_WriteReg(pf1550_serv_handle_t *handle, uint8_t reg, uint8_t val)
{
    pf1550_serv_write_reg_request_t writeRegRequest;
    bool result = false;

    /* Initialize PF1550 Service Request Structure. */
    writeRegRequest.requestApi = kPF1550_SERV_RequestApiWriteReg;
    writeRegRequest.semaphore = xSemaphoreCreateBinary();
    writeRegRequest.reg = reg;
    writeRegRequest.value = val;
    writeRegRequest.result = &result;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&writeRegRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(writeRegRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(writeRegRequest.semaphore);

    return result;
}

bool PF1550_SERV_DumpReg(pf1550_serv_handle_t *handle, uint8_t reg, uint8_t *buffer, uint8_t size)
{
    pf1550_serv_dump_reg_request_t dumpRegRequest;
    bool result = false;

    /* Initialize PF1550 Service Request Structure. */
    dumpRegRequest.requestApi = kPF1550_SERV_RequestApiDumpReg;
    dumpRegRequest.semaphore = xSemaphoreCreateBinary();
    dumpRegRequest.reg = reg;
    dumpRegRequest.buffer = buffer;
    dumpRegRequest.size = size;
    dumpRegRequest.result = &result;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&dumpRegRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(dumpRegRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(dumpRegRequest.semaphore);

    return result;
}

void PF1550_SERV_EnableRegulator(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                 pf1550_operating_status_t status, bool enable)
{
    pf1550_serv_enable_regulator_request_t enableRegulatorRequest;

    /* Initialize PF1550 Service Request Structure. */
    enableRegulatorRequest.requestApi = kPF1550_SERV_RequestApiEnableRegulator;
    enableRegulatorRequest.semaphore = xSemaphoreCreateBinary();
    enableRegulatorRequest.module = module;
    enableRegulatorRequest.status = status;
    enableRegulatorRequest.enable = enable;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&enableRegulatorRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(enableRegulatorRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(enableRegulatorRequest.semaphore);
}

bool PF1550_SERV_SetRegulatorOutput(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                    pf1550_operating_status_t status, uint32_t voltage)
{
    pf1550_serv_set_regulator_output_request_t setRegulatorOutputRequest;
    bool result = false;

    /* Initialize PF1550 Service Request Structure. */
    setRegulatorOutputRequest.requestApi = kPF1550_SERV_RequestApiSetRegulatorOutput;
    setRegulatorOutputRequest.semaphore = xSemaphoreCreateBinary();
    setRegulatorOutputRequest.module = module;
    setRegulatorOutputRequest.status = status;
    setRegulatorOutputRequest.voltage = voltage;
    setRegulatorOutputRequest.result = &result;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&setRegulatorOutputRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(setRegulatorOutputRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(setRegulatorOutputRequest.semaphore);

    if (true == result)
    {
        /* Wait until actual regulator output reach user's setting value. */
        vTaskDelay(handle->outputWaitTicks);
    }

    return result;
}

uint32_t PF1550_SERV_GetRegulatorOutput(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                        pf1550_operating_status_t status)
{
    pf1550_serv_get_regulator_output_request_t getRegulatorOutputRequest;
    uint32_t voltage = 0U;

    /* Initialize PF1550 Service Request Structure. */
    getRegulatorOutputRequest.requestApi = kPF1550_SERV_RequestApiGetRegulatorOutput;
    getRegulatorOutputRequest.semaphore = xSemaphoreCreateBinary();
    getRegulatorOutputRequest.module = module;
    getRegulatorOutputRequest.status = status;
    getRegulatorOutputRequest.voltage = &voltage;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&getRegulatorOutputRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(getRegulatorOutputRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(getRegulatorOutputRequest.semaphore);

    return voltage;
}

bool PF1550_SERV_IsRegulatorEnabled(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                    pf1550_operating_status_t status)
{
    volatile pf1550_serv_is_regulator_enabled_request_t isRegulatorEnabledRequest;
    bool enable = false;

    /* Initialize PF1550 Service Request Structure. */
    isRegulatorEnabledRequest.requestApi = kPF1550_SERV_RequestIsRegulatorEnabled;
    isRegulatorEnabledRequest.semaphore = xSemaphoreCreateBinary();
    isRegulatorEnabledRequest.module = module;
    isRegulatorEnabledRequest.status = status;
    isRegulatorEnabledRequest.enable = &enable;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&isRegulatorEnabledRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(isRegulatorEnabledRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(isRegulatorEnabledRequest.semaphore);

    return enable;
}

pf1550_operating_status_t PF1550_SERV_GetOperatingStatus(pf1550_serv_handle_t *handle)
{
    volatile pf1550_serv_get_operating_status_request_t getOperatingStatusRequest;
    pf1550_operating_status_t status = kPF1550_OperatingStatusSystemOff;

    /* Initialize PF1550 Service Request Structure. */
    getOperatingStatusRequest.requestApi = kPF1550_SERV_RequestGetOperatingStatus;
    getOperatingStatusRequest.semaphore = xSemaphoreCreateBinary();
    getOperatingStatusRequest.status = &status;

    /* Send request to PF1550 Service Task and return immediately. */
    xQueueSend(handle->queue, (void *)&getOperatingStatusRequest, 0U);

    /* Wait until request is processed. */
    xSemaphoreTake(getOperatingStatusRequest.semaphore, portMAX_DELAY);

    /* Free Semaphore that is useless. */
    vSemaphoreDelete(getOperatingStatusRequest.semaphore);

    return status;
}

void PF1550_SERV_Task(void *pvParameters)
{
    pf1550_serv_handle_t *handle = (pf1550_serv_handle_t *)pvParameters;
    pf1550_serv_common_request_t request;
    pf1550_serv_common_request_t *requestPtr = &request;

    /* PF1550 low level driver init. */
    PF1550_Init(&handle->drvHandle, &handle->drvConfig);

    for (;;)
    {
        /* Pending on PMIC Service Request. */
        xQueueReceive(handle->queue, &request, portMAX_DELAY);

        /* Process Service Request Here. */
        switch (request.requestApi)
        {
            case kPF1550_SERV_RequestApiWriteReg:
                /* Process Register Write Request. */
                PF1550_SERV_ProcessWriteRegRequest(handle, (pf1550_serv_write_reg_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_write_reg_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestApiDumpReg:
                /* Process Register Dump Request. */
                PF1550_SERV_ProcessDumpRegRequest(handle, (pf1550_serv_dump_reg_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_dump_reg_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestApiEnableRegulator:
                /* Process Regulator Enable Request. */
                PF1550_SERV_ProcessEnableRegultorRequest(handle, (pf1550_serv_enable_regulator_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_enable_regulator_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestApiSetRegulatorOutput:
                /* Process Set Regulator Output Request. */
                PF1550_SERV_ProcessSetRegulatorOutputRequest(handle, (pf1550_serv_set_regulator_output_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_set_regulator_output_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestApiGetRegulatorOutput:
                /* Process Get Regulator Output Request. */
                PF1550_SERV_ProcessGetRegulatorOutputRequest(handle, (pf1550_serv_get_regulator_output_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_get_regulator_output_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestIsRegulatorEnabled:
                /* Process Query Regulator Enable Request. */
                PF1550_SERV_ProcessIsRegulatorEnabledRequest(handle, (pf1550_serv_is_regulator_enabled_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_is_regulator_enabled_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestGetOperatingStatus:
                /* Process Get Operating Status Request. */
                PF1550_SERV_ProcessGetOperatingStatusRequest(handle, (pf1550_serv_get_operating_status_request_t *)&request);
                /* Unlock user application thread. */
                xSemaphoreGive(((pf1550_serv_get_operating_status_request_t *)requestPtr)->semaphore);
                break;

            case kPF1550_SERV_RequestHandleIRQ:
                /* Process HandleIRQ Request. */
                PF1550_SERV_ProcessHandleIrqRequest(handle);
                break;

            default:
                break;
        }
    }
}

static void PF1550_SERV_ProcessWriteRegRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_write_reg_request_t *request)
{
    *(request->result) = PF1550_WriteReg(&handle->drvHandle, request->reg,
                                         request->value);
}

static void PF1550_SERV_ProcessDumpRegRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_dump_reg_request_t *request)
{
    *(request->result) = PF1550_DumpReg(&handle->drvHandle, request->reg,
                                        request->buffer, request->size);
}

static void PF1550_SERV_ProcessEnableRegultorRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_enable_regulator_request_t *request)
{
    PF1550_EnableRegulator(&handle->drvHandle, request->module,
                           request->status, request->enable);
}

static void PF1550_SERV_ProcessSetRegulatorOutputRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_set_regulator_output_request_t *request)
{
    *(request->result) = PF1550_IsRegulatorSupportDvs(&handle->drvHandle, request->module);

    if (true == (*(request->result)))
    {
        PF1550_SetRegulatorOutputVoltage(&handle->drvHandle, request->module,
                                         request->status, request->voltage);
    }
}

static void PF1550_SERV_ProcessGetRegulatorOutputRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_get_regulator_output_request_t *request)
{
    *(request->voltage) = PF1550_GetRegulatorOutputVoltage(&handle->drvHandle,
                                                           request->module,
                                                           request->status);
}

static void PF1550_SERV_ProcessIsRegulatorEnabledRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_is_regulator_enabled_request_t *request)
{
    *(request->enable) = PF1550_IsRegulatorEnabled(&handle->drvHandle, request->module,
                                                   request->status);
}

static void PF1550_SERV_ProcessGetOperatingStatusRequest(pf1550_serv_handle_t *handle,
                        pf1550_serv_get_operating_status_request_t *request)
{
    *(request->status) = PF1550_GetOperatingStatus(&handle->drvHandle);
}

static void PF1550_SERV_ProcessHandleIrqRequest(pf1550_serv_handle_t *handle)
{
    uint32_t category;
    uint32_t source;

    category = PF1550_GetInterruptCategory(&handle->drvHandle);

    if (NULL != handle->PF1550_SERV_CallbackFunc)
    {
        if (kPF1550_IntCategoryCharger & category)
        {
            source = PF1550_GetInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryCharger);
            PF1550_ClearInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryCharger, source);
            handle->PF1550_SERV_CallbackFunc(kPF1550_IntCategoryCharger, source);
        }

        if (kPF1550_IntCategorySwitch & category)
        {
            source = PF1550_GetInterruptStatus(&handle->drvHandle, kPF1550_IntCategorySwitch);
            PF1550_ClearInterruptStatus(&handle->drvHandle, kPF1550_IntCategorySwitch, source);
            handle->PF1550_SERV_CallbackFunc(kPF1550_IntCategorySwitch, source);
        }

        if (kPF1550_IntCategoryLdo & category)
        {
            source = PF1550_GetInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryLdo);
            PF1550_ClearInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryLdo, source);
            handle->PF1550_SERV_CallbackFunc(kPF1550_IntCategoryLdo, source);
        }

        if (kPF1550_IntCategoryOnkey & category)
        {
            source = PF1550_GetInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryOnkey);
            PF1550_ClearInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryOnkey, source);
            handle->PF1550_SERV_CallbackFunc(kPF1550_IntCategoryOnkey, source);
        }

        if (kPF1550_IntCategoryTemp & category)
        {
            source = PF1550_GetInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryTemp);
            PF1550_ClearInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryTemp, source);
            handle->PF1550_SERV_CallbackFunc(kPF1550_IntCategoryTemp, source);
        }

        if (kPF1550_IntCategoryMisc & category)
        {
            source = PF1550_GetInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryMisc);
            PF1550_ClearInterruptStatus(&handle->drvHandle, kPF1550_IntCategoryMisc, source);
            handle->PF1550_SERV_CallbackFunc(kPF1550_IntCategoryMisc, source);
        }
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
