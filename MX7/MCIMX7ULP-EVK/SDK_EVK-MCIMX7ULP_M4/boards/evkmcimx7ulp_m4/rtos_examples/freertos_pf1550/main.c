/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#include "fsl_debug_console.h"
#include "fsl_lpi2c_freertos.h"
#include "fsl_pf1550_serv.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PF1550_LPI2C_BASE                    LPI2C3
#define DEMO_PF1550_LPI2C_IRQ                     LPI2C3_IRQn
#define DEMO_PF1550_LPI2C_PRIO                    (5U)
#define DEMO_PF1550_LPI2C_CLKSRC_FREQ             CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define DEMO_PF1550_LPI2C_BAUDRATE                (400000U)
#define DEMO_PF1550_LPI2C_SCL_GPIO                GPIOB
#define DEMO_PF1550_LPI2C_SCL_PIN                 (12U)
#define DEMO_PF1550_LPI2C_SDA_GPIO                GPIOB
#define DEMO_PF1550_LPI2C_SDA_PIN                 (13U)
#define DEMO_PF1550_LPI2C_DELAY                   (100U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
bool I2C_SendFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize);
bool I2C_ReceiveFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static pf1550_serv_handle_t pf1550ServHandle;
static lpi2c_rtos_handle_t lpi2c_rtos_handle;
static uint8_t buffer[256U];

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_I2C_ReleaseBusDelay(void)
{
    uint32_t i = 0U;
    for (i = 0U; i < DEMO_PF1550_LPI2C_DELAY; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0U;
    gpio_pin_config_t pin_config;

    /* Initialize PTB12/PTB13 as GPIO */
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    IOMUXC_SetPinMux(IOMUXC_PTB12_GPIOB12, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB13_GPIOB13, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB12_GPIOB12, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTB13_GPIOB13, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);
    GPIO_PinInit(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, &pin_config);
    GPIO_PinInit(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (i = 0U; i < 9U; i++)
    {
        GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 0U);
        BOARD_I2C_ReleaseBusDelay();

        GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 1U);
        BOARD_I2C_ReleaseBusDelay();

        GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 1U);
        BOARD_I2C_ReleaseBusDelay();
        BOARD_I2C_ReleaseBusDelay();
    }

    /* Send stop */
    GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 1U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_WritePinOutput(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 1U);
    BOARD_I2C_ReleaseBusDelay();
}

static uint32_t DEMO_GetDemoToRun(void)
{
    uint8_t demoSel;

    PRINTF("\r\n-------------- PF1550 on board PMIC RTOS driver example --------------\r\n");
    PRINTF("\r\nPlease select the PMIC example you want to run:\r\n");
    PRINTF("[1]. Setting Regulator Output Voltage\r\n");
    PRINTF("[2]. Dumping Regulator Output Voltage\r\n");
    PRINTF("[3]. Dumping Selected Register Content\r\n");

    while(1)
    {
        demoSel = GETCHAR();
        if ((demoSel >= '1') && (demoSel <= '3'))
        {
            break;
        }
    }

    return demoSel;
}

static uint32_t DEMO_GetInputNumWithEcho(uint32_t length, bool allowZero)
{
    uint8_t ch;
    uint8_t digBuffer[10U];
    uint8_t i, j;
    uint8_t digCount = 0U;
    uint32_t temp1, temp2;
    uint32_t result = 0U;
    bool getFirstDig = false;

    assert(length <= (sizeof(digBuffer) / sizeof(digBuffer[0U])));

    /* Get user input and echo it back to terminal. */
    for (;;)
    {
        ch = GETCHAR();
        if ((('0' <= ch) && ('9' >= ch)) && (digCount < length))
        {
            if (false == getFirstDig)
            {
                if (allowZero || (('0' < ch) && ('9' >= ch)))
                {
                    getFirstDig = true;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                if ('0' == digBuffer[0U])
                {
                    continue;
                }
            }
            PUTCHAR(ch);
            digBuffer[digCount] = ch;
            digCount++;
        }

        if ('\r' == ch)
        {
            break;
        }
    }

    /* Reconstruct user input number. */
    temp1 = digCount - 1;
    for (i = 0; i < digCount; i++)
    {
        temp2 = digBuffer[i] - '0';
        for (j = 0U; j < temp1; j++)
        {
            temp2 *= 10U;
        }
        temp1--;
        result += temp2;
    }

    return result;
}

static void DEMO_SetRegulatorOutput(void)
{
    uint8_t regulatorSel;
    uint32_t outputVoltage = 0;
    pf1550_module_t module;

    for (;;)
    {
        PRINTF("\f");
        PRINTF("\r\n-------------- Setting Regulator Output Voltage  --------------\r\n");
        PRINTF("Please select which regulator to set:\r\n");
        PRINTF("[a]. Buck Switch 1\r\n");
        PRINTF("[b]. Buck Switch 2\r\n");
        PRINTF("[c]. Buck Switch 3\r\n");
        PRINTF("[d]. LDO 1\r\n");
        PRINTF("[e]. LDO 2\r\n");
        PRINTF("[f]. LDO 3\r\n");
        PRINTF("[g]. Back to Home Page\r\n");

        for (;;)
        {
            regulatorSel = GETCHAR();
            if ((regulatorSel >= 'a') && (regulatorSel <= 'g'))
            {
                break;
            }
        }

        if ('g' != regulatorSel)
        {
            switch (regulatorSel)
            {
                case 'a':
                    if (PF1550_IsRegulatorSupportDvs(&pf1550ServHandle.drvHandle,
                                                     kPF1550_ModuleSwitch1))
                    {
                        module = kPF1550_ModuleSwitch1;
                    }
                    else
                    {
                        PRINTF("\f");
                        PRINTF("%s does not support DVS!!!\r\n", "Switch1");
                        PRINTF("Press Any Key to Continue...\r\n");
                        GETCHAR();
                        PRINTF("\f");
                        continue;
                    }
                    break;

                case 'b':
                    if (PF1550_IsRegulatorSupportDvs(&pf1550ServHandle.drvHandle,
                                                     kPF1550_ModuleSwitch2))
                    {
                        module = kPF1550_ModuleSwitch2;
                    }
                    else
                    {
                        PRINTF("\f");
                        PRINTF("%s does not support DVS!!!\r\n", "Switch2");
                        PRINTF("Press Any Key to Continue...\r\n");
                        GETCHAR();
                        PRINTF("\f");
                        continue;
                    }
                    break;

                case 'c':
                    PRINTF("\f");
                    PRINTF("%s does not support DVS!!!\r\n", "Switch3");
                    PRINTF("Press Any Key to Continue...\r\n");
                    GETCHAR();
                    PRINTF("\f");
                    continue;
                    break;

                case 'd':
                    module = kPF1550_ModuleLdo1;
                    break;

                case 'e':
                    module = kPF1550_ModuleLdo2;
                    break;

                case 'f':
                    module = kPF1550_ModuleLdo3;
                    break;

                default:
                    PRINTF("\f");
                    PRINTF("Unsupported regulator is selected!!!\r\n");
                    PRINTF("Press Any Key to Continue...\r\n");
                    GETCHAR();
                    PRINTF("\f");
                    continue;
                    break;
            }

            PRINTF("\f");
            PRINTF("Please input the regulator output(mV):");
            outputVoltage = DEMO_GetInputNumWithEcho(4U, false);
            PRINTF("\r\n");

            PRINTF("User desired regulator output:%dmV\r\n", outputVoltage);
            PRINTF("Please confirm the desired output will not damage to the hardware!!!\r\n");
            PRINTF("Press any key to update regulator output...\r\n");
            GETCHAR();

            /* Convert user input unit to uV. */
            outputVoltage *= 1000U;
        }
        else
        {
            break;
        }

        PF1550_SERV_SetRegulatorOutput(&pf1550ServHandle, module, PF1550_GetOperatingStatus(&pf1550ServHandle.drvHandle),
                                       outputVoltage);
        outputVoltage =
            PF1550_SERV_GetRegulatorOutput(&pf1550ServHandle, module, PF1550_GetOperatingStatus(&pf1550ServHandle.drvHandle));

        /* Convert actual output unit to mV. */
        outputVoltage /= 1000U;
        PRINTF("Actual Regulator Output:%dmV\r\n", outputVoltage);
        PRINTF("Press Any Key to Continue...\r\n");
        GETCHAR();
        PRINTF("\f");
    }
}

static void DEMO_PrintRegulatorInfo(const char *regulatorName,
                 pf1550_module_t module)
{
    pf1550_operating_status_t status;

    PRINTF("[Name:%s]", regulatorName);
    status = PF1550_SERV_GetOperatingStatus(&pf1550ServHandle);
    switch (status)
    {
        case kPF1550_OperatingStatusCoreOff:
            PRINTF("[Status:CoreOff]");
            break;

        case kPF1550_OperatingStatusShipMode:
            PRINTF("[Status:ShipMode]");
            break;

        case kPF1550_OperatingStatusRegsDisable:
            PRINTF("[Status:RegsDisable]");
            break;

        case kPF1550_OperatingStatusRun:
            PRINTF("[Status:Run]");
            break;

        case kPF1550_OperatingStatusStandby:
            PRINTF("[Status:Standby]");
            break;

        case kPF1550_OperatingStatusSleep:
            PRINTF("[Status:Sleep]");
            break;

        default:
            break;
    }

    PRINTF("[Enable:%s][Voltage:%dmV]\r\n",
           PF1550_SERV_IsRegulatorEnabled(&pf1550ServHandle, module, status) ? "Yes" : "No",
           PF1550_SERV_GetRegulatorOutput(&pf1550ServHandle, module, status) / 1000U);
}

static void DEMO_DumpRegulatorOutput(void)
{
    /* Scroll to a new page. */
    PRINTF("\f");
    PRINTF("\r\n-------------- Dumping PF1550 Regulator Output --------------\r\n");
    PRINTF("All the Regulator Output Parameters are listed here:\r\r\n\n");

    /* Print All Regulator Output Voltages. */
    DEMO_PrintRegulatorInfo("Switch1",
         kPF1550_ModuleSwitch1);
    DEMO_PrintRegulatorInfo("Switch2",
         kPF1550_ModuleSwitch2);
    DEMO_PrintRegulatorInfo("Switch3",
         kPF1550_ModuleSwitch3);
    DEMO_PrintRegulatorInfo(" LDO1  ",
         kPF1550_ModuleLdo1);
    DEMO_PrintRegulatorInfo(" LDO2  ",
         kPF1550_ModuleLdo2);
    DEMO_PrintRegulatorInfo(" LDO3  ",
         kPF1550_ModuleLdo3);
    DEMO_PrintRegulatorInfo(" Vsnvs ",
         kPF1550_ModuleVsnvs);
    PRINTF("\r\n");
}

static void DEMO_DumpRegisterContent(void)
{
    uint32_t address;
    uint8_t number;
    uint8_t i;

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("\f");
        PRINTF("\r\n-------------- Dumping PF1550 Register Content --------------\r\n\r\n");
        PRINTF("Please select the first address to dump(0~255):");
        address = DEMO_GetInputNumWithEcho(3U, true);
        if (255U >= address)
        {
            break;
        }
    }
    PRINTF("\r\n");

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("\f");
        PRINTF("Please select the number of registers to dump(1~256):");
        number = DEMO_GetInputNumWithEcho(3U, false);
        if ((256U > (address + number)) && (0U != number))
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Read register from PF1550 */
    PF1550_SERV_DumpReg(&pf1550ServHandle, address, buffer, number);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    for (i = 0; i < number; i++)
    {
        PRINTF("[0x%x] = 0x%x\r\n", address + i, buffer[i]);
    }
    PRINTF("\r\n");
}

/*! @brief User Application task */
void User_Application_Task(void *pvParameters)
{
    uint32_t demoSelection;

    for (;;)
    {
        demoSelection = DEMO_GetDemoToRun();

        switch (demoSelection)
        {
            /* Setting Regulator Output Voltage: */
            case '1':
                DEMO_SetRegulatorOutput();
                break;

            /* Dumping all Regulator Output Voltage: */
            case '2':
                DEMO_DumpRegulatorOutput();
                PRINTF("Press Any Key to Home Page...");
                GETCHAR();
                break;

            /* Dumping Selected Register Content: */
            case '3':
                DEMO_DumpRegisterContent();
                PRINTF("Press Any Key to Home Page...");
                GETCHAR();
                break;

            default:
                break;
        }

        /* Scroll to a new page. */
        PRINTF("\f");
    }
}

/*! @brief Main function */
int main(void)
{
    lpi2c_master_config_t lpi2cMasterConfig;
    pf1550_serv_config_t pf1550Config;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    /* Initialize MPU */
    /* Make sure outstanding transfers are done. */
    __DMB();
    /* Disable the MPU. */
    MPU->CTRL = 0U;
    /* Select Region 0 to configure. */
    MPU->RNR  = 0U;
    /* Set base address of Region 0 to the base of DDR memory region(1GB). */
    MPU->RBAR = 0x60000000U;
    /* Region 0 setting:
     * 1) Enable Instruction Access;
     * 2) Full Data Access Permission;
     * 3) Outer and inner Non-Cacheable;
     * 4) Region is Sharable;
     * 5) All Sub-Region Enable;
     * 6) MPU Protection Region size = 1GB;
     * 7) Enable Region 0.
     */
    MPU->RASR = 0x030C003BU;

    /* Enable Privileged default memory map and the MPU. */
    MPU->CTRL = MPU_CTRL_ENABLE_Msk |
                MPU_CTRL_PRIVDEFENA_Msk;
    /* Memory barriers to ensure subsequence data & instruction
     * transfers using updated MPU settings.
     */
    __DSB();
    __ISB();

    CLOCK_EnableClock(kCLOCK_PctlB);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSircAsync);

    /*
     * lpi2cMasterConfig.debugEnable = false;
     * lpi2cMasterConfig.ignoreAck = false;
     * lpi2cMasterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cMasterConfig.baudRate_Hz = 100000U;
     * lpi2cMasterConfig.busIdleTimeout_ns = 0;
     * lpi2cMasterConfig.pinLowTimeout_ns = 0;
     * lpi2cMasterConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cMasterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cMasterConfig);
    lpi2cMasterConfig.baudRate_Hz = DEMO_PF1550_LPI2C_BAUDRATE;
    /*  Set LPI2C Master Interrupt Priority. */
    NVIC_SetPriority(DEMO_PF1550_LPI2C_IRQ, DEMO_PF1550_LPI2C_PRIO);
    /* Initialize LPI2C RTOS driver. */
    LPI2C_RTOS_Init(&lpi2c_rtos_handle, DEMO_PF1550_LPI2C_BASE, &lpi2cMasterConfig,
                    DEMO_PF1550_LPI2C_CLKSRC_FREQ);

    /* Initialize PMIC Local Service. */
    PF1550_SERV_GetDefaultConfig(&pf1550Config);
    pf1550Config.drvConfig.I2C_SendFunc = I2C_SendFunc;
    pf1550Config.drvConfig.I2C_ReceiveFunc = I2C_ReceiveFunc;
    PF1550_SERV_Init(&pf1550ServHandle, &pf1550Config);

    /* Create User Application Task. */
    xTaskCreate(User_Application_Task, "User Application Task",
                configMINIMAL_STACK_SIZE * 2U, NULL, tskIDLE_PRIORITY + 1U,
                NULL);
    /* Create Local PMIC Service Task. */
    xTaskCreate(PF1550_SERV_Task, "PMIC Service Task",
                configMINIMAL_STACK_SIZE * 2U, (void *)&pf1550ServHandle,
                tskIDLE_PRIORITY + 3U, NULL);

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for(;;)
    {
    }
}

bool I2C_SendFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize)
{
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data = txBuff;
    masterXfer.dataSize = txBuffSize;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    /* Calling I2C Transfer API to start send. */
    return (kStatus_Success == LPI2C_RTOS_Transfer(&lpi2c_rtos_handle,
                                                   &masterXfer)) ? true : false;
}

bool I2C_ReceiveFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data = rxBuff;
    masterXfer.dataSize = rxBuffSize;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    /* Calling I2C Transfer API to start receive. */
    return (kStatus_Success == LPI2C_RTOS_Transfer(&lpi2c_rtos_handle,
                                                   &masterXfer)) ? true : false;
}
