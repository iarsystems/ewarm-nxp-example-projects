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

#ifndef _FSL_PF1550_SERV_H_
#define _FSL_PF1550_SERV_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "fsl_pf1550.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _pf1550_serv_handle
{
    /* Handle of underlying PF1550 bare-bone driver. */
    pf1550_handle_t drvHandle;
    /* PF1550 bare-bone driver initialize configuration structure. */
    pf1550_config_t drvConfig;
    /* PF1550 event notification callback function. */
    void (*PF1550_SERV_CallbackFunc)(pf1550_interrupt_category_t category,
                                     uint32_t source);
    /* Queue used to serialize PMIC request. */
    QueueHandle_t queue;
    /* Wait N ticks to make sure the actual output is updated. */
    uint32_t outputWaitTicks;
} pf1550_serv_handle_t;

/*! @brief PF1550 Configuration Structure definition. */
typedef struct _pf1550_serv_config
{
    /* PF1550 bare-bone driver initialize configuration structure. */
    pf1550_config_t drvConfig;
    /* PF1550 event notification callback function. */
    void (*PF1550_SERV_CallbackFunc)(pf1550_interrupt_category_t category,
                                     uint32_t source);
    /* PF1550 Service request queue size. */
    uint32_t queueSize;
    /* Wait N ticks to make sure the actual output is updated. */
    uint32_t outputWaitTicks;
} pf1550_serv_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Gets the default configuration structure.
 *
 * This function initializes the PF1550 configuration structure to default values. The default
 * values are as follows.
 *   pf1550Config->drvConfig.I2C_SendFunc              = NULL;
 *   pf1550Config->drvConfig.I2C_ReceiveFunc           = NULL;
 *   pf1550Config->drvConfig.threshold                 = kPF1550_UnderVoltDetThres_Rising3V0Falling2V9;
 *   pf1550Config->drvConfig.delay                     = kPF1550_PowerUpDelay_2ms;
 *   pf1550Config->drvConfig.shutDownLdoInCurrentFault = false;
 *   pf1550Config->drvConfig.slaveAddress              = PF1550_DEFAULT_I2C_ADDR;
 *   pf1550Config->PF1550_SERV_CallbackFunc            = NULL;
 *   pf1550Config->queueSize                           = 10U;
 *   pf1550Config->outputWaitTicks                     = 1U;
 *
 * @param config Pointer to the PF1550 Service configuration structure.
 */
void PF1550_SERV_GetDefaultConfig(pf1550_serv_config_t *config);

/*!
 * @brief Initializes a PF1550 instance.
 *
 * This function initializes the PF1550 module with user-defined settings.
 * This example shows how to set up the pf1550_config_t parameters and how
 * to call the PF1550_Init function by passing in these parameters.
 * @code
 *   pf1550_serv_config_t pf1550Config;
 *   pf1550Config.drvConfig.I2C_SendFunc              = APP_I2C_SendFunc;
 *   pf1550Config.drvConfig.I2C_ReceiveFunc           = APP_I2C_ReceiveFunc;
 *   pf1550Config.drvConfig.threshold                 = kPF1550_UnderVoltDetThres_Rising3V1Falling3V0;
 *   pf1550Config.drvConfig.delay                     = kPF1550_PowerUpDelay_1024ms;
 *   pf1550Config.drvConfig.shutDownLdoInCurrentFault = true;
 *   pf1550Config.drvConfig.slaveAddress              = PF1550_DEFAULT_I2C_ADDR;
 *   pf1550Config.PF1550_SERV_CallbackFunc            = PF1550_SERV_CallbackFunc;
 *   pf1550Config.queueSize                           = 10U;
 *   pf1550Config.outputWaitTicks                     = 1U
 *   PF1550_SERV_Init(&pf1550Handle, &pf1550Config);
 * @endcode
 *
 * @param handle PF1550 Service Handle.
 * @param config Pointer to the user-defined configuration structure.
 */
void PF1550_SERV_Init(pf1550_serv_handle_t *handle, const pf1550_serv_config_t *config);

/*!
 * @brief Write the value to register of PF1550.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @param reg variable store address of register.
 * @param val variable store value which is written to PF1550.
 * @return true if success or false if error.
 */
bool PF1550_SERV_WriteReg(pf1550_serv_handle_t *handle, uint8_t reg, uint8_t val);

/*!
 * @brief Dump the register content in PF1550.
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @param reg The address of the first register to dump.
 * @param buffer The buffer to store the dumped content.
 * @param size The count of registers to dump.
 * @return true if success or false if error.
 */
bool PF1550_SERV_DumpReg(pf1550_serv_handle_t *handle, uint8_t reg, uint8_t *buffer, uint8_t size);

/*!
 * @brief Enable/Disable the output of selected regulator.
 *
 * This function is used to enable/disable the output of selected regulator
 * enumerated in @ref pf1550_module_t under selected operating status
 * enumerated in @ref pf1550_operating_status_t. The output state of
 * the regulators will change automatically once the the operating status
 * changed.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @param module Sub-modules in PF1550 device.
 * @param status The operating status of PF1550 to set.
 * @param enable Set true to enable regulator, set false to disable regulator.
 */
void PF1550_SERV_EnableRegulator(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                 pf1550_operating_status_t status, bool enable);

/*!
 * @brief Set the output voltage of selected regulator.
 *
 * This function is used to set the output voltage of selected regulator
 * enumerated in @ref pf1550_module_t under selected operating status
 * enumerated in @ref pf1550_operating_status_t. The output state of
 * the regulators will change automatically once the the operating status
 * changed. All the available output voltages are listed in
 * @ref pf1550_regulator_output_voltage_t.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @param module Sub-modules in PF1550 device.
 * @param status The operating status of PF1550 to set.
 * @param voltage The output voltage in uV to set.
 */
bool PF1550_SERV_SetRegulatorOutput(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                    pf1550_operating_status_t status, uint32_t voltage);

/*!
 * @brief Query the output voltage of selected regulator.
 *
 * This function is used to query the output voltage of selected regulator
 * enumerated in @ref pf1550_module_t under selected operating status
 * enumerated in @ref pf1550_operating_status_t. The output state of
 * the regulators will change automatically once the the operating status
 * changed. All the available output voltages are listed in
 * @ref pf1550_regulator_output_voltage_t.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @param module Sub-modules in PF1550 device.
 * @param status The operating status of PF1550 to set.
 * @return voltage The output voltage in uV of selected regulator.
 */
uint32_t PF1550_SERV_GetRegulatorOutput(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                        pf1550_operating_status_t status);

/*!
 * @brief Query the output state of selected regulator.
 *
 * This function is used to query the output state of selected regulator
 * enumerated in @ref pf1550_module_t under selected operating status
 * enumerated in @ref pf1550_operating_status_t. The output state of
 * the regulators will change automatically once the the operating status
 * changed.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @param module Sub-modules in PF1550 device.
 * @param status The operating status of PF1550 to set.
 * @return true if regulator is enabled, false if regulator is disabled.
 */
bool PF1550_SERV_IsRegulatorEnabled(pf1550_serv_handle_t *handle, pf1550_module_t module,
                                    pf1550_operating_status_t status);

/*!
 * @brief Query Current Operating Status of PF1550.
 *
 * This function is used to query the current operating status of PF1550.
 * All the available operating status for PF1550 are listed in
 * @ref pf1550_operating_status_t.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 * @return Current PF1550 operating status.
 */
pf1550_operating_status_t PF1550_SERV_GetOperatingStatus(pf1550_serv_handle_t *handle);

/*!
 * @brief PF1550 Service Task.
 *
 * This function is used to service the PMIC control request from other
 * thread. This function should be registered to FreeRTOS and called by
 * RTOS Kernel. User need to provide the priority and stack size to the
 * task for the PMIC service task.
 *
 * @param pvParameters Pointer to a valid PF1550 service instance structure.
 */
void PF1550_SERV_Task(void *pvParameters);

/*!
 * @brief PF1550 IRQ Handler function.
 *
 * This function is used to provide the default PF1550 ISR.
 * all the events asserted by the PF1550 will be cleared and passed to
 * user application by Callback function registered in PF1550_SERV_Init()
 * function.
 *
 * @param handle Pointer to a valid PF1550 service instance structure.
 */
void PF1550_SERV_HandleIRQ(pf1550_serv_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_PF1550_SERV_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
