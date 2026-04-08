/*
 * @brief LPC11xx ROM API declarations and functions
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __ROMAPI_11U6X_H_
#define __ROMAPI_11U6X_H_

#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ROMAPI_11U6X CHIP: LPC11U6X ROM API declarations and functions
 * @ingroup CHIP_11U6X_Drivers
 *
 * @{
 */

/**
 * @brief LPC11U6X High level ROM API structure
 */
typedef struct {
	const uint32_t usbdApiBase;				/*!< USBD API function table base address */
	const uint32_t reserved0;				/*!< Reserved */
	const uint32_t reserved1;				/*!< Reserved */
	const uint32_t pwrApiBase;				/*!< Power API function table base address */
	const uint32_t divApiBase;				/*!< Divider API function table base address */
	const uint32_t i2cApiBase;				/*!< I2C driver API function table base address */
	const uint32_t dmaApiBase;				/*!< DMA driver API function table base address */
	const uint32_t reserved2;				/*!< Reserved */
	const uint32_t reserved3;				/*!< Reserved */
	const uint32_t uartApiBase;				/*!< UART driver API function table base address */
	const uint32_t reserved4;				/*!< Reserved */
	const uint32_t usartApiBase;			/*!< USART driver API function table base address */
} LPC_ROM_API_T;

/**
 * @brief LPC11U6X IAP_ENTRY API function type
 */
typedef void (*IAP_ENTRY_T)(unsigned int[], unsigned int[]);

static INLINE void iap_entry(unsigned int cmd_param[], unsigned int status_result[])
{
	((IAP_ENTRY_T) IAP_ENTRY_LOCATION)(cmd_param, status_result);
}

/** @defgroup ROMAPI_DIV_11U6X ROM divider API declarations
 * The ROM-based 32-bit integer division routines are available for all LPC11U6x.
 * These routines are performance optimized and reduce application code space. As
 * part of chip library these routines overload “/” and “%” operators in C.
 * @{
 */

/**
 * @brief Structure containing signed integer divider return data.
 */
typedef struct {
	int quot;			/*!< Quotient */
	int rem;			/*!< Reminder */
} IDIV_RETURN_T;

/**
 * @brief Structure containing unsigned integer divider return data.
 */
typedef struct {
	unsigned quot;		/*!< Quotient */
	unsigned rem;		/*!< Reminder */
} UIDIV_RETURN_T;

/**
 * @brief ROM divider API Structure.
 */
typedef struct {
	int (*sidiv)(int numerator, int denominator);							/*!< Signed integer division */
	unsigned (*uidiv)(unsigned numerator, unsigned denominator);			/*!< Unsigned integer division */
	IDIV_RETURN_T (*sidivmod)(int numerator, int denominator);				/*!< Signed integer division with remainder */
	UIDIV_RETURN_T (*uidivmod)(unsigned numerator, unsigned denominator);	/*!< Unsigned integer division with remainder */
} LPC_ROM_DIV_API_T;

#if 0
/* Redirector of signed 32 bit integer divider to ROM routine */
static INLINE int __aeabi_idiv(int numerator, int denominator)
{
	LPC_ROM_DIV_API_T const *pROMDiv = (LPC_ROM_DIV_API_T *) LPC_ROM_API->divApiBase;
	return pROMDiv->sidiv(numerator, denominator);
}

/* Redirector of unsigned 32 bit integer divider to ROM routine */
static INLINE unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator)
{
	LPC_ROM_DIV_API_T const *pROMDiv = (LPC_ROM_DIV_API_T *) LPC_ROM_API->divApiBase;
	return pROMDiv->uidiv(numerator, denominator);
}

/* Redirector of signed 32 bit integer divider with reminder to ROM routine */
static INLINE  IDIV_RETURN_T __aeabi_idivmod(int numerator, int denominator)
{
	LPC_ROM_DIV_API_T const *pROMDiv = (LPC_ROM_DIV_API_T *) LPC_ROM_API->divApiBase;
	return pROMDiv->sidivmod(numerator, denominator);
}

/* Redirector of unsigned 32 bit integer divider with reminder to ROM routine */
static INLINE UIDIV_RETURN_T __aeabi_uidivmod(unsigned numerator, unsigned denominator)
{
	LPC_ROM_DIV_API_T const *pROMDiv = (LPC_ROM_DIV_API_T *) LPC_ROM_API->divApiBase;
	return pROMDiv->uidivmod(numerator, denominator);
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ROMAPI_11U6X_H_ */
