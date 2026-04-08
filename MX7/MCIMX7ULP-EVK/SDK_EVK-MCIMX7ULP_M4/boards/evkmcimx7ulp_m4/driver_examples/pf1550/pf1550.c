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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_lpi2c.h"
#include "fsl_pf1550.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PF1550_INTB_GPIO         GPIOB
#define DEMO_PF1550_INTB_PORT         PORTB
#define DEMO_PF1550_INTB_PIN          (7U)
#define DEMO_PF1550_INTB_HANDLER      PCTLB_IRQHandler
#define DEMO_PF1550_PORT_IRQ          PCTLB_IRQn
#define DEMO_PF1550_LPI2C             LPI2C3
#define DEMO_PF1550_LPI2C_CLKSRC_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define DEMO_PF1550_LPI2C_BAUDRATE    (100000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static bool I2C_SendFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize);
static bool I2C_ReceiveFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static pf1550_handle_t pf1550Handle;
static lpi2c_master_handle_t lpi2cHandle;
static volatile bool pf1550IntFlag;
static volatile bool lpi2cIntFlag;
static uint8_t buffer[256U];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief PF1550 INTB Pad ISR function */
void DEMO_PF1550_INTB_HANDLER(void)
{
    /* Disable level-triggered PF1550 INTB Pad Interrupt here.
       The interrupt will be enabled once current events have
       been solved in the main loop. */
    PORT_SetPinInterruptConfig(DEMO_PF1550_INTB_PORT, DEMO_PF1550_INTB_PIN,
                               kPORT_InterruptOrDMADisabled);
    /* Inform main loop PF1550 INTB Pad Interrupt is asserted. */
    pf1550IntFlag = true;
}

static void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        lpi2cIntFlag = true;
    }
}

static uint32_t DEMO_GetDemoToRun(void)
{
    uint8_t demoSel;

    PRINTF("\r\n-------------- PF1550 on board PMIC driver example --------------\r\n");
    PRINTF("\r\nPlease select the PMIC example you want to run:\r\n");
    PRINTF("[1]. Setting Regulator Output Voltage\r\n");
    PRINTF("[2]. Processing ONKEY Press Event \r\n");
    PRINTF("[3]. Dumping Regulator Output Voltage\r\n");
    PRINTF("[4]. Dumping Selected Register Content\r\n");

    while (1)
    {
        demoSel = GETCHAR();
        if ((demoSel >= '1') && (demoSel <= '4'))
        {
            break;
        }
    }

    return demoSel;
}

static uint32_t DEMO_GetInputNumWithEcho(uint32_t length, bool allowZero)
{
    uint8_t ch;
    uint8_t digBuffer[10U] = {0U};
    uint8_t i, j;
    uint8_t digCount = 0U;
    uint32_t temp1 = 0U, temp2 = 0U;
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
        else if ((0x7FU == ch) && (0U != digCount))
        {
            digBuffer[digCount] = 0x0U;
            digCount--;
            PUTCHAR(0x7FU);
        }
        else
        {
            if ('\r' == ch)
            {
                break;
            }
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
                    if (PF1550_IsRegulatorSupportDvs(&pf1550Handle,
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
                    if (PF1550_IsRegulatorSupportDvs(&pf1550Handle,
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

        PF1550_SetRegulatorOutputVoltage(&pf1550Handle, module,
               PF1550_GetOperatingStatus(&pf1550Handle),
               outputVoltage);
        outputVoltage = PF1550_GetRegulatorOutputVoltage(&pf1550Handle,
                               module,
                               PF1550_GetOperatingStatus(&pf1550Handle));

        /* Convert actual output unit to mV. */
        outputVoltage /= 1000U;
        PRINTF("Actual Regulator Output:%dmV\r\n", outputVoltage);
        PRINTF("Press Any Key to Continue...\r\n");
        GETCHAR();
        PRINTF("\f");
    }
}

static void DEMO_ProcessOnkey(void)
{
    uint32_t category;
    uint32_t status;
    bool backToHomePage = false;

    PRINTF("\f");
    PRINTF("\r\n-------------- Process PF1550 ONKEY Event --------------\r\n");
    PRINTF("Press ONKEY to trigger ONKEY Event.\r\n");
    PRINTF("Hold ONKEY 8s to back to Home Page.\r\n\r\n");

    /* Enable ONKEY Press Interrupts. */
    pf1550IntFlag = false;
    PF1550_ClearInterruptStatus(&pf1550Handle, kPF1550_IntCategoryOnkey, 0xFF);
    PF1550_EnableInterrupts(&pf1550Handle, kPF1550_IntCategoryOnkey,
           kPF1550_IntSrcOnkeyPush |
           kPF1550_IntSrcOnkeyPress_1s |
           kPF1550_IntSrcOnkeyPress_2s |
           kPF1550_IntSrcOnkeyPress_3s |
           kPF1550_IntSrcOnkeyPress_4s |
           kPF1550_IntSrcOnkeyPress_8s);
    PORT_ClearPinsInterruptFlags(DEMO_PF1550_INTB_PORT, 1U << DEMO_PF1550_INTB_PIN);
    PORT_SetPinInterruptConfig(DEMO_PF1550_INTB_PORT, DEMO_PF1550_INTB_PIN,
                               kPORT_InterruptLogicZero);
    EnableIRQ(DEMO_PF1550_PORT_IRQ);

    for (;;)
    {
        /* PF1550 Interrupt  */
        if (pf1550IntFlag)
        {
            category = PF1550_GetInterruptCategory(&pf1550Handle);
            if (category & kPF1550_IntCategoryOnkey)
            {
                /* Process ONKEY event here. */
                status = PF1550_GetInterruptStatus(&pf1550Handle,
                                         kPF1550_IntCategoryOnkey);
                PF1550_ClearInterruptStatus(&pf1550Handle, kPF1550_IntCategoryOnkey, status);
                PORT_ClearPinsInterruptFlags(DEMO_PF1550_INTB_PORT, 1U << DEMO_PF1550_INTB_PIN);

                if (status & kPF1550_IntSrcOnkeyPress_8s)
                {
                    PRINTF("ONKEY is Pressed %d seconds!\r\n", 8);
                    backToHomePage = true;
                }
                if (status & kPF1550_IntSrcOnkeyPress_4s)
                {
                    PRINTF("ONKEY is Pressed %d seconds!\r\n", 4);
                }
                if (status & kPF1550_IntSrcOnkeyPress_3s)
                {
                    PRINTF("ONKEY is Pressed %d seconds!\r\n", 3);
                }
                if (status & kPF1550_IntSrcOnkeyPress_2s)
                {
                    PRINTF("ONKEY is Pressed %d seconds!\r\n", 2);
                }
                if (status & kPF1550_IntSrcOnkeyPress_1s)
                {
                    PRINTF("ONKEY is Pressed %d seconds!\r\n", 1);
                }
                if (status & kPF1550_IntSrcOnkeyPush)
                {
                    PRINTF("ONKEY is Pushed!\r\n");
                }
            }
            pf1550IntFlag = false;
            if (backToHomePage)
            {
                break;
            }
            else
            {
                PORT_SetPinInterruptConfig(DEMO_PF1550_INTB_PORT, DEMO_PF1550_INTB_PIN,
                                           kPORT_InterruptLogicZero);
            }
        }
    }
}

static void DEMO_PrintRegulatorInfo(const char *regulatorName,
                 pf1550_module_t module)
{
    pf1550_operating_status_t status;

    PRINTF("[Name:%s]", regulatorName);
    status = PF1550_GetOperatingStatus(&pf1550Handle);
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
        PF1550_IsRegulatorEnabled(&pf1550Handle,
               module, status) ? "Yes" : "No",
        PF1550_GetRegulatorOutputVoltage(&pf1550Handle, module,
               status) / 1000U);
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
    PF1550_DumpReg(&pf1550Handle, address, buffer, number);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    for (i = 0; i < number; i++)
    {
        PRINTF("[0x%x] = 0x%x\r\n", address + i, buffer[i]);
    }
    PRINTF("\r\n");
}

/*! @brief Main function */
int main(void)
{
    gpio_pin_config_t gpioPinConfig;
    lpi2c_master_config_t masterConfig;
    pf1550_config_t pf1550Config;
    pf1550_onkey_attribute_t pf1550OnkeyAttribute;
    uint32_t demoSelection;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_PctlB);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSircAsync);

    /* Init GPIO Interrupt used by PMIC. */
    gpioPinConfig.pinDirection = kGPIO_DigitalInput;
    gpioPinConfig.outputLogic = 0x0U;
    GPIO_PinInit(DEMO_PF1550_INTB_GPIO, DEMO_PF1550_INTB_PIN, &gpioPinConfig);

    /* Init LPI2C used by PF1550. */
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
    masterConfig.baudRate_Hz = DEMO_PF1550_LPI2C_BAUDRATE;
    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(DEMO_PF1550_LPI2C, &masterConfig, DEMO_PF1550_LPI2C_CLKSRC_FREQ);
    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(DEMO_PF1550_LPI2C, &lpi2cHandle,
                                     lpi2c_master_callback, NULL);

    /* Init PF1550 Component. */
    PF1550_GetDefaultConfig(&pf1550Config);
    pf1550Config.I2C_SendFunc = I2C_SendFunc;
    pf1550Config.I2C_ReceiveFunc = I2C_ReceiveFunc;
    PF1550_Init(&pf1550Handle, &pf1550Config);

    pf1550OnkeyAttribute.debounce =
          kPF1550_OnkeyDebounce_Falling31ms25Rising31ms25;
    pf1550OnkeyAttribute.enableTurnOffSystemViaOnkey = false;
    pf1550OnkeyAttribute.pushTime = kPF1550_OnkeyPushResetTime_16s;
    PF1550_SetOnKeyPadAttibute(&pf1550Handle, &pf1550OnkeyAttribute);

    /* Print the initial banner. */
    for (;;)
    {
        demoSelection = DEMO_GetDemoToRun();

        switch (demoSelection)
        {
            /* Setting Regulator Output Voltage: */
            case '1':
                DEMO_SetRegulatorOutput();
                break;

            /* Processing ONKEY Interrupt Event: */
            case '2':
                DEMO_ProcessOnkey();
                break;

            /* Dumping all Regulator Output Voltage: */
            case '3':
                DEMO_DumpRegulatorOutput();
                PRINTF("Press Any Key to Home Page...");
                GETCHAR();
                break;

            /* Dumping Selected Register Content: */
            case '4':
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

static bool I2C_SendFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize)
{
    status_t reVal = kStatus_Fail;
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data = txBuff;
    masterXfer.dataSize = txBuffSize;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    lpi2cIntFlag = false;
    reVal = LPI2C_MasterTransferNonBlocking(DEMO_PF1550_LPI2C, &lpi2cHandle, &masterXfer);
    if (kStatus_Success != reVal)
    {
        return false;
    }

    while(false == lpi2cIntFlag)
    {
    }

    return true;
}

static bool I2C_ReceiveFunc(uint8_t deviceAddress, uint32_t subAddress,
                uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    status_t reVal = kStatus_Fail;
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data = rxBuff;
    masterXfer.dataSize = rxBuffSize;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    lpi2cIntFlag = false;
    reVal = LPI2C_MasterTransferNonBlocking(DEMO_PF1550_LPI2C, &lpi2cHandle, &masterXfer);
    if (kStatus_Success != reVal)
    {
        return false;
    }

    while(false == lpi2cIntFlag)
    {
    }

    return true;
}
