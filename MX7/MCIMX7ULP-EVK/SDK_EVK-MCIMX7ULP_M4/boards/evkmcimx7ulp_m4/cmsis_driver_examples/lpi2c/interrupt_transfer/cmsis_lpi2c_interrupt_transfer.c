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

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_cmsis.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER Driver_I2C0
#define EXAMPLE_I2C_SLAVE Driver_I2C3
#define EXAMPLE_I2C_MASTER_BASE (LPI2C0_BASE)
#define EXAMPLE_I2C_SLAVE_BASE (LPI2C3_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c0))
#define LPI2C_SLAVE_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c3))

#define I2C_MASTER_IRQ LPI2C0_IRQn
#define I2C_SLAVE_IRQ LPI2C3_IRQn
#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_DATA_LENGTH (32) /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
uint8_t g_master_buff[I2C_DATA_LENGTH];

volatile bool g_slaveCompleted = false;
volatile bool g_masterCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t LPI2C0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpi2c0);
}
uint32_t LPI2C3_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpi2c3);
}

static void lpi2c_slave_callback(uint32_t event)
{
    switch (event)
    {
        /* The master has sent a stop transition on the bus */
        case ARM_I2C_EVENT_TRANSFER_DONE:
            g_slaveCompleted = true;
            break;

        default:
            break;
    }
}
static void lpi2c_master_callback(uint32_t event)
{
    switch (event)
    {
        /* The master has sent a stop transition on the bus */
        case ARM_I2C_EVENT_TRANSFER_DONE:
            g_masterCompleted = true;
            break;

        /* master arbitration lost */
        case ARM_I2C_EVENT_ARBITRATION_LOST:
            g_masterCompleted = true;
            break;

        default:
            break;
    }
}

void test_interrupt_transfer(void)
{
    uint32_t i = 0;

    PRINTF("\r\ntest interrupt transfer\r\n");

    memset(g_slave_buff, 0, sizeof(g_slave_buff));
    /* Set up i2c master to send data to slave */
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    /* Display the data the master will send */
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
    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    EXAMPLE_I2C_SLAVE.SlaveReceive(g_slave_buff, I2C_DATA_LENGTH);
    /* Start transmitting I2C transfers on the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.MasterTransmit(I2C_MASTER_SLAVE_ADDR_7BIT, g_master_buff, I2C_DATA_LENGTH, false);

    /* Wait for the transfer to complete. */
    while (!g_slaveCompleted)
    {
    }

    g_slaveCompleted = false;

    /* Display the slave data */
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

    memset(g_master_buff, 0, sizeof(g_master_buff));
    /* Set up i2c slave to send data to master*/
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = i;
    }

    /* Display the data the slave will send */
    PRINTF("slave will send data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }

    PRINTF("\r\n\r\n");

    /* Start transmitting I2C transfers on the LPI2C slave peripheral */
    EXAMPLE_I2C_SLAVE.SlaveTransmit(g_slave_buff, I2C_DATA_LENGTH);
    /* Start accepting I2C transfers on the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.MasterReceive(I2C_MASTER_SLAVE_ADDR_7BIT, g_master_buff, I2C_DATA_LENGTH, false);

    /* Wait for the transfer to complete. */
    while (!g_masterCompleted)
    {
    }

    g_masterCompleted = false;
    /* Display the master data */
    PRINTF("\r\nmaster received data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    /*Init BOARD*/
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcFircAsync);

    /*set I2C master and I2C slave priority*/
    NVIC_SetPriority(I2C_SLAVE_IRQ, 0);
    NVIC_SetPriority(I2C_MASTER_IRQ, 1);

    /* Initialize the LPI2C slave peripheral */
    EXAMPLE_I2C_SLAVE.PowerControl(ARM_POWER_FULL);
    EXAMPLE_I2C_SLAVE.Initialize(lpi2c_slave_callback);

    /* Change the slave address */
    EXAMPLE_I2C_SLAVE.Control(ARM_I2C_OWN_ADDRESS, I2C_MASTER_SLAVE_ADDR_7BIT);

    /* Initialize the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.PowerControl(ARM_POWER_FULL);
    EXAMPLE_I2C_MASTER.Initialize(lpi2c_master_callback);

    /* Change the default baudrate configuration */
    EXAMPLE_I2C_MASTER.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);

    /*test interrupt transfer*/
    test_interrupt_transfer();

    /* Hang at the end */
    while (1)
    {
    }
}
