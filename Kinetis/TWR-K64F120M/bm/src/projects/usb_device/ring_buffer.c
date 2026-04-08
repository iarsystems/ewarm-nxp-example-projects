/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

#include "ring_buffer.h"

/* Pointers */
volatile uint8 *OUT_StartAddress;
volatile uint8 *OUT_EndAddress;
volatile uint8 *OUT_UsbPointer;
volatile uint8 *OUT_SciPointer;

/* Variables */
volatile uint8 gu8BufferMaxSize;
volatile uint8 gu8BufferOverFlow;

void Buffer_Init(uint8 * pu8BufferPointer, uint8 u8MaxSize)
{
	/* Buffer Initialization */
	OUT_EndAddress = pu8BufferPointer + u8MaxSize - 1;
	OUT_StartAddress = pu8BufferPointer;
	gu8BufferMaxSize = u8MaxSize;
	OUT_UsbPointer = pu8BufferPointer;
	OUT_SciPointer = pu8BufferPointer;
	gu8BufferOverFlow = 0;
}

uint8 Buffer_Request(uint8 * pu8DataPointer, uint16 u8RequestSize)
{
	uint8 u8FreeSpace;

	/* Check for OverFlow */

	if (gu8BufferOverFlow)
		return (NOT_ENOUGH_SPACE);

	/* Calculate Free Space */
	if (OUT_UsbPointer < OUT_SciPointer)
		u8FreeSpace = (uint8) (OUT_SciPointer - OUT_UsbPointer);

	else
		u8FreeSpace =
		    gu8BufferMaxSize - (OUT_UsbPointer - OUT_SciPointer);

	/* Validate requested size */
	if (u8FreeSpace < u8RequestSize)
		return (NOT_ENOUGH_SPACE);

	if (u8FreeSpace == u8RequestSize)
		gu8BufferOverFlow = 1;

	/* Buffer Copy */
	while (u8RequestSize--) {
		*OUT_UsbPointer = *pu8DataPointer;
		OUT_UsbPointer++;
		pu8DataPointer++;
		if (OUT_UsbPointer > OUT_EndAddress) {
			OUT_UsbPointer = OUT_StartAddress;
		}
	}
	return (OK);
}
