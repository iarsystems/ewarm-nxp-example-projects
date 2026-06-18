/* Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "app_flash.h"
#include "fsl_shell.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SHELL_Printf PRINTF
#define APP_BYTES_EACH_LINE          (16)

typedef enum
{
    APP_MemAccess8Bit  = 0U,
    APP_MemAccess16Bit = 1U,
    APP_MemAccess32Bit = 2U
} app_mem_access_unit_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* SHELL user send data callback */
void SHELL_SendDataCallback(uint8_t *buf, uint32_t len);

/* SHELL user receive data callback */
void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len);

static int32_t MemGet8Control (p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemGet16Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemGet32Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemSet8Control (p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemSet16Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemSet32Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemCmp8Control (p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemCmp16Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemCmp32Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemCpy8Control (p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemCpy16Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t MemCpy32Control(p_shell_context_t context, int32_t argc, char **argv);

static int32_t FlashEraseControl(p_shell_context_t context, int32_t argc, char **argv);
static int32_t FlashWriteControl(p_shell_context_t context, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const shell_command_context_t xMemGet8Command  = {"memget8",
                                                         "\r\n\"memget8 arg1 arg2\":  read memory in 8bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            memory address\r\n"
                                                         "    arg2: count           memory read count\r\n",
                                                         MemGet8Control, 2};

static const shell_command_context_t xMemGet16Command = {"memget16",
                                                         "\r\n\"memget16 arg1 arg2\": read memory in 16bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            memory address\r\n"
                                                         "    arg2: count           memory read count\r\n",
                                                         MemGet16Control, 2};

static const shell_command_context_t xMemGet32Command = {"memget32",
                                                         "\r\n\"memget32 arg1 arg2\": read memory in 32bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            memory address\r\n"
                                                         "    arg2: count           memory unit read count\r\n",
                                                         MemGet32Control, 2};

static const shell_command_context_t xMemSet8Command  = {"memset8",
                                                         "\r\n\"memset8 arg1 arg2 arg3\":  write memory in 8bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            memory address\r\n"
                                                         "    arg2: value           memory write value\r\n"
                                                         "    arg3: count           memory unit write count\r\n",
                                                         MemSet8Control, 3};

static const shell_command_context_t xMemSet16Command = {"memset16",
                                                         "\r\n\"memset16 arg1 arg2 arg3\": write memory in 16bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            memory address\r\n"
                                                         "    arg2: value           memory write value\r\n"
                                                         "    arg3: count           memory unit write count\r\n",
                                                         MemSet16Control, 3};

static const shell_command_context_t xMemSet32Command = {"memset32",
                                                         "\r\n\"memset32 arg1 arg2 arg3\": write memory in 32bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            memory address\r\n"
                                                         "    arg2: value           memory write value\r\n"
                                                         "    arg3: count           memory unit write count\r\n",
                                                         MemSet32Control, 3};

static const shell_command_context_t xMemCmp8Command  = {"memcmp8",
                                                         "\r\n\"memcmp8 arg1 arg2 arg3\":  compare memory in 8bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr1           memory address1\r\n"
                                                         "    arg2: addr2           memory address2\r\n"
                                                         "    arg3: count           memory unit compare count\r\n",
                                                         MemCmp8Control, 3};

static const shell_command_context_t xMemCmp16Command = {"memcmp16",
                                                         "\r\n\"memcmp16 arg1 arg2 arg3\": compare memory in 16bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr1           memory address1\r\n"
                                                         "    arg2: addr2           memory address2\r\n"
                                                         "    arg3: count           memory unit compare count\r\n",
                                                         MemCmp16Control, 3};

static const shell_command_context_t xMemCmp32Command = {"memcmp32",
                                                         "\r\n\"memcmp32 arg1 arg2 arg3\": compare memory in 32bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr1           memory address1\r\n"
                                                         "    arg2: addr2           memory address2\r\n"
                                                         "    arg3: count           memory unit compare count\r\n",
                                                         MemCmp32Control, 3};

static const shell_command_context_t xMemCpy8Command  = {"memcpy8",
                                                         "\r\n\"memcpy8 arg1 arg2 arg3\":  copy memory in 8bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: dst             destination memory address\r\n"
                                                         "    arg2: src             source memory address\r\n"
                                                         "    arg3: count           memory unit copy count\r\n",
                                                         MemCpy8Control, 3};

static const shell_command_context_t xMemCpy16Command = {"memcpy16",
                                                         "\r\n\"memcpy16 arg1 arg2 arg3\": copy memory in 16bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: dst             destination memory address\r\n"
                                                         "    arg2: src             source memory address\r\n"
                                                         "    arg3: count           memory unit copy count\r\n",
                                                         MemCpy16Control, 3};

static const shell_command_context_t xMemCpy32Command = {"memcpy32",
                                                         "\r\n\"memcpy32 arg1 arg2 arg3\": copy memory in 32bit unit\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: dst             destination memory address\r\n"
                                                         "    arg2: src             source memory address\r\n"
                                                         "    arg3: count           memory unit copy count\r\n",
                                                         MemCpy32Control, 3};

static const shell_command_context_t xFlashEraseCommand = {"flasherase",
                                                         "\r\n\"flasherase arg1 arg2\": erase QSPI flash\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: offset          flash memory offset in bytes\r\n"
                                                         "    arg2: bytes           flash memory size in bytes\r\n",
                                                         FlashEraseControl, 2};

static const shell_command_context_t xFlashWriteCommand = {"flashwrite",
                                                         "\r\n\"flashwrite arg1 arg2 arg3\": write QSPI flash\r\n"
                                                         " Usage:\r\n"
                                                         "    arg1: addr            source memory address\r\n"
                                                         "    arg2: offset          flash memory offset in bytes\r\n"
                                                         "    arg3: bytes           flash memory size in bytes\r\n",
                                                         FlashWriteControl, 3};

/*******************************************************************************
 * Code
 ******************************************************************************/

void SHELL_SendDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        PUTCHAR(*(buf++));
    }
}

void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        *(buf++) = GETCHAR();
    }
}

static int32_t MemGetControl(p_shell_context_t context, int32_t argc, char **argv, app_mem_access_unit_t unit)
{
    uint32_t addr = strtoul(argv[1], NULL, 0);
    uint32_t count = strtoul(argv[2], NULL, 0);
    uint8_t *p8;
    uint16_t *p16;
    uint32_t *p32;
    uint32_t pos;

    switch (unit)
    {
        case APP_MemAccess8Bit:
            p8 = (uint8_t *)addr;
            while (count)
            {
                PRINTF("0x%08x: ", (uint32_t)p8);
                for (pos = 0; (count > 0) && (pos < APP_BYTES_EACH_LINE); pos++)
                {
                    PRINTF("%02x ", *p8++);
                    count--;
                }
                PRINTF("\r\n");
            }
            break;

        case APP_MemAccess16Bit:
            p16 = (uint16_t *)(addr & 0xFFFFFFFEU);
            while (count)
            {
                PRINTF("0x%08x: ", (uint32_t)p16);
                for (pos = 0; (count > 0) && (pos < APP_BYTES_EACH_LINE); pos+=2)
                {
                    PRINTF("%04x ", *p16++);
                    count--; 
                }
                PRINTF("\r\n");
            }
            break;

        case APP_MemAccess32Bit:
            p32 = (uint32_t *)(addr & 0xFFFFFFFCU);
            while (count)
            {
                PRINTF("0x%08x: ", (uint32_t)p32);
                for (pos = 0; (count > 0) && (pos < APP_BYTES_EACH_LINE); pos+=4)
                {
                    PRINTF("%08x ", *p32++);
                    count--; 
                }
                PRINTF("\r\n");
            }
            break;

        default:
            break;
    }

    return 0;
}

static int32_t MemSetControl(p_shell_context_t context, int32_t argc, char **argv, app_mem_access_unit_t unit)
{
    uint32_t addr = strtoul(argv[1], NULL, 0);
    uint32_t value = strtoul(argv[2], NULL, 0);
    uint32_t count = strtoul(argv[3], NULL, 0);
    uint8_t *p8;
    uint16_t *p16;
    uint32_t *p32;

    switch (unit)
    {
        case APP_MemAccess8Bit:
            p8 = (uint8_t *)addr;
            while (count--)
            {
                *p8++ = value & 0xFFU;
            }
            break;

        case APP_MemAccess16Bit:
            p16 = (uint16_t *)(addr & 0xFFFFFFFEU);
            while (count--)
            {
                *p16++ = value & 0xFFFFU;
            }
            break;

        case APP_MemAccess32Bit:
            p32 = (uint32_t *)(addr & 0xFFFFFFFCU);
            while (count--)
            {
                *p32++ = value;
            }
            break;

        default:
            break;
    }

    return 0;
}

static int32_t MemCmpControl(p_shell_context_t context, int32_t argc, char **argv, app_mem_access_unit_t unit)
{
    uint32_t addr1 = strtoul(argv[1], NULL, 0);
    uint32_t addr2 = strtoul(argv[2], NULL, 0);
    uint32_t count = strtoul(argv[3], NULL, 0);
    uint8_t *p8[2];
    uint16_t *p16[2];
    uint32_t *p32[2];
    bool diff = false;

    switch (unit)
    {
        case APP_MemAccess8Bit:
            p8[0] = (uint8_t *)addr1;
            p8[1] = (uint8_t *)addr2;
            while (count--)
            {
                if (*(p8[0]) != *(p8[1]))
                {
                    PRINTF("DIFF at [0x%08x]:%02x and [0x%08x]:%02x\r\n", p8[0], *(p8[0]), p8[1], *(p8[1]));
                    diff = true;
                }
                p8[0]++;
                p8[1]++;
            }
            break;

        case APP_MemAccess16Bit:
            p16[0] = (uint16_t *)(addr1 & 0xFFFFFFFEU);
            p16[1] = (uint16_t *)(addr2 & 0xFFFFFFFEU);
            while (count--)
            {
                if (*(p16[0]) != *(p16[1]))
                {
                    PRINTF("DIFF at [0x%08x]:%04x and [0x%08x]:%04x\r\n", p16[0], *(p16[0]), p16[1], *(p16[1]));
                    diff = true;
                }
                p16[0]++;
                p16[1]++;
            }
            break;

        case APP_MemAccess32Bit:
            p32[0] = (uint32_t *)(addr1 & 0xFFFFFFFCU);
            p32[1] = (uint32_t *)(addr2 & 0xFFFFFFFCU);
            while (count--)
            {
                if (*(p32[0]) != *(p32[1]))
                {
                    PRINTF("DIFF at [0x%08x]:%08x and [0x%08x]:%08x\r\n", p32[0], *(p32[0]), p32[1], *(p32[1]));
                    diff = true;
                }
                p32[0]++;
                p32[1]++;
            }
            break;

        default:
            break;
    }

    if (!diff)
    {
        PRINTF("Same\r\n");
    }

    return 0;
}

static int32_t MemCpyControl(p_shell_context_t context, int32_t argc, char **argv, app_mem_access_unit_t unit)
{
    uint32_t dst = strtoul(argv[1], NULL, 0);
    uint32_t src = strtoul(argv[2], NULL, 0);
    uint32_t count = strtoul(argv[3], NULL, 0);
    uint8_t *p8Dst, *p8Src;
    uint16_t *p16Dst, *p16Src;
    uint32_t *p32Dst, *p32Src;

    switch (unit)
    {
        case APP_MemAccess8Bit:
            p8Dst = (uint8_t *)dst;
            p8Src = (uint8_t *)src;
            while (count--)
            {
                *p8Dst++ = *p8Src++;
            }
            break;

        case APP_MemAccess16Bit:
            p16Dst = (uint16_t *)(dst & 0xFFFFFFFEU);
            p16Src = (uint16_t *)(src & 0xFFFFFFFEU);
            while (count--)
            {
                *p16Dst++ = *p16Src++;
            }
            break;

        case APP_MemAccess32Bit:
            p32Dst = (uint32_t *)(dst & 0xFFFFFFFCU);
            p32Src = (uint32_t *)(src & 0xFFFFFFFCU);
            while (count--)
            {
                *p32Dst++ = *p32Src++;
            }
            break;

        default:
            break;
    }

    return 0;
}

static int32_t MemGet8Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemGetControl(context, argc, argv, APP_MemAccess8Bit);
}

static int32_t MemGet16Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemGetControl(context, argc, argv, APP_MemAccess16Bit);
}

static int32_t MemGet32Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemGetControl(context, argc, argv, APP_MemAccess32Bit);
}

static int32_t MemSet8Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemSetControl(context, argc, argv, APP_MemAccess8Bit);
}

static int32_t MemSet16Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemSetControl(context, argc, argv, APP_MemAccess16Bit);
}

static int32_t MemSet32Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemSetControl(context, argc, argv, APP_MemAccess32Bit);
}

static int32_t MemCmp8Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemCmpControl(context, argc, argv, APP_MemAccess8Bit);
}

static int32_t MemCmp16Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemCmpControl(context, argc, argv, APP_MemAccess16Bit);
}

static int32_t MemCmp32Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemCmpControl(context, argc, argv, APP_MemAccess32Bit);
}

static int32_t MemCpy8Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemCpyControl(context, argc, argv, APP_MemAccess8Bit);
}

static int32_t MemCpy16Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemCpyControl(context, argc, argv, APP_MemAccess16Bit);
}

static int32_t MemCpy32Control(p_shell_context_t context, int32_t argc, char **argv)
{
    return MemCpyControl(context, argc, argv, APP_MemAccess32Bit);
}

static int32_t FlashEraseControl(p_shell_context_t context, int32_t argc, char **argv)
{
    uint32_t offset = strtoul(argv[1], NULL, 0);
    uint32_t bytes = strtoul(argv[2], NULL, 0);

    return APP_EraseFlash(offset, bytes);
}

static int32_t FlashWriteControl(p_shell_context_t context, int32_t argc, char **argv)
{
    uint32_t addr = strtoul(argv[1], NULL, 0);
    uint32_t offset = strtoul(argv[2], NULL, 0);
    uint32_t bytes = strtoul(argv[3], NULL, 0);

    return APP_WriteFlash((uint32_t *)addr, offset, bytes);
}

/*! @brief Main function */
int main(void)
{
    shell_context_struct user_context;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrcDiv(kCLOCK_Qspi, kCLOCK_IpSrcSircAsync, 0, 0);

    APP_InitFlash();

    /* Init SHELL */
    SHELL_Init(&user_context, SHELL_SendDataCallback, SHELL_ReceiveDataCallback, SHELL_Printf, "SHELL>> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(&xMemGet8Command);
    SHELL_RegisterCommand(&xMemGet16Command);
    SHELL_RegisterCommand(&xMemGet32Command);
    SHELL_RegisterCommand(&xMemSet8Command);
    SHELL_RegisterCommand(&xMemSet16Command);
    SHELL_RegisterCommand(&xMemSet32Command);
    SHELL_RegisterCommand(&xMemCmp8Command);
    SHELL_RegisterCommand(&xMemCmp16Command);
    SHELL_RegisterCommand(&xMemCmp32Command);
    SHELL_RegisterCommand(&xMemCpy8Command);
    SHELL_RegisterCommand(&xMemCpy16Command);
    SHELL_RegisterCommand(&xMemCpy32Command);
    SHELL_RegisterCommand(&xFlashEraseCommand);
    SHELL_RegisterCommand(&xFlashWriteCommand);

    SHELL_Main(&user_context);

    while (1)
    {
    }
}
