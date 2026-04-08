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
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_HOST_H
#define _FSL_HOST_H

#include "fsl_common.h"
#include "board.h"
#if defined(FSL_FEATURE_SOC_SDHC_COUNT) && FSL_FEATURE_SOC_SDHC_COUNT > 0U
#include "fsl_sdhc.h"
#elif defined(FSL_FEATURE_SOC_SDIF_COUNT) && FSL_FEATURE_SOC_SDIF_COUNT > 0U
#include "fsl_sdif.h"
#endif

/*!
 * @addtogroup CARD
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* select host */
#if defined(FSL_FEATURE_SOC_SDHC_COUNT) && FSL_FEATURE_SOC_SDHC_COUNT > 0U
#define HOST_TYPE SDHC_Type
#define HOST_CONFIG sdhc_host_t
#define HOST_TRANSFER sdhc_transfer_t
#define HOST_COMMAND sdhc_command_t
#define HOST_DATA sdhc_data_t
#define CARD_DATA0_STATUS_MASK kSDHC_Data0LineLevelFlag
#define CARD_DATA0_NOT_BUSY kSDHC_Data0LineLevelFlag
#define HOST_BUS_WIDTH_TYPE sdhc_data_bus_width_t
#define HOST_CAPABILITY sdhc_capability_t

#define kHOST_DATABUSWIDTH1BIT kSDHC_DataBusWidth1Bit /*!< 1-bit mode */
#define kHOST_DATABUSWIDTH4BIT kSDHC_DataBusWidth4Bit /*!< 4-bit mode */
#define kHOST_DATABUSWIDTH8BIT kSDHC_DataBusWidth8Bit /*!< 8-bit mode */

/* function pointer define */
#define HOST_TRANSFER_FUNCTION sdhc_transfer_function_t
#define GET_HOST_CAPABILITY(base, capability) (SDHC_GetCapability(base, capability))
#define GET_HOST_STATUS(base) (SDHC_GetPresentStatusFlags(base))
#define HOST_SET_CARD_CLOCK(base, sourceClock_HZ, busClock_HZ) (SDHC_SetSdClock(base, sourceClock_HZ, busClock_HZ))
#define HOST_SET_CARD_BUS_WIDTH(base, busWidth) (SDHC_SetDataBusWidth(base, busWidth))
#define HOST_SEND_CARD_ACTIVE(base, timeout) (SDHC_SetCardActive(base, timeout))

/*! @brief SDHC host capability*/
enum _host_capability
{
    kHOST_SupportAdma = kSDHC_SupportAdmaFlag,
    kHOST_SupportHighSpeed = kSDHC_SupportHighSpeedFlag,
    kHOST_SupportDma = kSDHC_SupportDmaFlag,
    kHOST_SupportSuspendResume = kSDHC_SupportSuspendResumeFlag,
    kHOST_SupportV330 = kSDHC_SupportV330Flag,
#if defined FSL_FEATURE_SDHC_HAS_V300_SUPPORT && FSL_FEATURE_SDHC_HAS_V300_SUPPORT
    kHOST_SupportV300 = kSDHC_SupportV300Flag,
#endif
#if defined FSL_FEATURE_SDHC_HAS_V180_SUPPORT && FSL_FEATURE_SDHC_HAS_V180_SUPPORT
    kHOST_SupportV180 = kSDHC_SupportV180Flag,
#endif
    kHOST_Support4BitBusWidth = kSDHC_Support4BitFlag,
    kHOST_Support8BitBusWidth = kSDHC_Support8BitFlag,

};

/* Endian mode. */
#define SDHC_ENDIAN_MODE kSDHC_EndianModeLittle

/* DMA mode */
#define SDHC_DMA_MODE kSDHC_DmaModeAdma2

/* Read/write watermark level. The bigger value indicates DMA has higher read/write performance. */
#define SDHC_READ_WATERMARK_LEVEL (0x80U)
#define SDHC_WRITE_WATERMARK_LEVEL (0x80U)

/* ADMA table length united as word.
 *
 * SD card driver can't support ADMA1 transfer mode currently.
 * One ADMA2 table item occupy two words which can transfer maximum 0xFFFFU bytes one time.
 * The more data to be transferred in one time, the bigger value of SDHC_ADMA_TABLE_WORDS need to be set.
 */
#define SDHC_ADMA_TABLE_WORDS (8U)

#elif defined(FSL_FEATURE_SOC_SDIF_COUNT) && FSL_FEATURE_SOC_SDIF_COUNT > 0U
#define HOST_TYPE SDIF_Type
#define HOST_CONFIG sdif_host_t
#define HOST_TRANSFER sdif_transfer_t
#define HOST_COMMAND sdif_command_t
#define HOST_DATA sdif_data_t
#define CARD_DATA0_STATUS_MASK SDIF_STATUS_DATA_BUSY_MASK
#define CARD_DATA0_NOT_BUSY 0U
#define HOST_BUS_WIDTH_TYPE sdif_bus_width_t
#define HOST_CAPABILITY sdif_capability_t

#define kHOST_DATABUSWIDTH1BIT kSDIF_Bus1BitWidth /*!< 1-bit mode */
#define kHOST_DATABUSWIDTH4BIT kSDIF_Bus4BitWidth /*!< 4-bit mode */
#define kHOST_DATABUSWIDTH8BIT kSDIF_Bus8BitWidth /*!< 8-bit mode */

/* function pointer define */
#define HOST_TRANSFER_FUNCTION sdif_transfer_function_t
#define GET_HOST_CAPABILITY(base, capability) (SDIF_GetCapability(base, capability))
#define GET_HOST_STATUS(base) (SDIF_GetControllerStatus(base))
#define HOST_SET_CARD_CLOCK(base, sourceClock_HZ, busClock_HZ) (SDIF_SetCardClock(base, sourceClock_HZ, busClock_HZ))
#define HOST_SET_CARD_BUS_WIDTH(base, busWidth) (SDIF_SetCardBusWidth(base, busWidth))
#define HOST_SEND_CARD_ACTIVE(base, timeout) (SDIF_SendCardActive(base, timeout))

/*! @brief SDIF host capability*/
enum _host_capability
{
    kHOST_SupportHighSpeed = kSDIF_SupportHighSpeedFlag,
    kHOST_SupportDma = kSDIF_SupportDmaFlag,
    kHOST_SupportSuspendResume = kSDIF_SupportSuspendResumeFlag,
    kHOST_SupportV330 = kSDIF_SupportV330Flag,

    kHOST_Support4BitBusWidth = kSDIF_Support4BitFlag,
    kHOST_Support8BitBusWidth = 0U, /* mask the 8 bit here,user can change depend on your board */

};

/*! @brief DMA table length united as word
 * One dma table item occupy four words which can transfer maximum 2*8188 bytes in dual DMA mode
 * and 8188 bytes in chain mode
 * The more data to be transferred in one time, the bigger value of SDHC_ADMA_TABLE_WORDS need to be set.
 * user need check the DMA descriptor table lenght if bigger enough.
 */
#define SDIF_DMA_TABLE_WORDS (0x40U)

#endif

/*! @brief host Endian mode
* corresponding to driver define
*/
enum _host_endian_mode
{
    kHOST_EndianModeBig = 0U,         /*!< Big endian mode */
    kHOST_EndianModeHalfWordBig = 1U, /*!< Half word big endian mode */
    kHOST_EndianModeLittle = 2U,      /*!< Little endian mode */
};

#define EVENT_TIMEOUT_TRANSFER_COMPLETE (1000U)
#define EVENT_TIMEOUT_CARD_DETECT (~0U)

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDIF adaptor function
 * @{
 */

/*!
 * @brief Detect card insert, only need for SD cases.
 * @param hostBase the pointer to host base address
 * @retval kStatus_Success detect card insert
 * @retval kStatus_Fail card insert event fail
 */
status_t CardInsertDetect(HOST_TYPE *hostBase);

/*!
 * @brief Init host controller.
 * @param host the pointer to host structure in card structure.
 * @retval kStatus_Success host init success
 * @retval kStatus_Fail event fail
 */
status_t HOST_Init(void *host);

/*!
 * @brief Deinit host controller.
 * @param host the pointer to host structure in card structure.
 */
void HOST_Deinit(void *host);

/* @} */

#if defined(__cplusplus)
}
#endif

#endif
