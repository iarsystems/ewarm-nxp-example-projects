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

/*
 *
 * Filename:	memtest.c
 *
 * Description:	General-purpose memory testing functions.
 *
 * Notes:		This software can be easily ported to systems with
 *				different data bus widths by redefining 'datum'.
 *
 * Copyright (c) 1998 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 */

#include "memtest.h"

/*
 *
 * Function:	memTestDataBus()
 *
 * Description:	Test the data bus wiring in a memory region by
 *				performing a walking 1's test at a fixed address
 *				within that region.  The address (and hence the
 *				memory region) is selected by the caller.
 *
 * Notes:
 *
 * Returns:		0 if the test succeeds.
 *				A non-zero result is the first pattern that failed.
 */
datum memTestDataBus(volatile datum * address)
{
	datum pattern;

	/* Perform a walking 1's test at the given address. */
	for (pattern = 1; pattern != 0; pattern <<= 1) {
		/* Write the test pattern. */
		*address = pattern;

		/* Read it back (immediately is okay for this test). */
		if (*address != pattern) {
			return (pattern);
		}
	}

	return (0);
}		/* memTestDataBus() */

/*
 *
 * Function:	memTestAddressBus()
 *
 * Description	 Test the address bus wiring in a memory region by
 *				performing a walking 1's test on the relevant bits
 *				of the address and checking for aliasing. This test
 *				will find single-bit address failures such as stuck
 *				-high, stuck-low, and shorted pins.  The base address
 *				and size of the region are selected by the caller.
 *
 * Notes:		For best results, the selected base address should
 *				have enough LSB 0's to guarantee single address bit
 *				changes.  For example, to test a 64-Kbyte region,
 *				select a base address on a 64-Kbyte boundary.  Also,
 *				select the region size as a power-of-two--if at all
 *				possible.
 *
 * Returns:		NULL if the test succeeds.
 *				A non-zero result is the first address at which an
 *				aliasing problem was uncovered.  By examining the
 *				contents of memory, it may be possible to gather
 *				additional information about the problem.
 */
datum *memTestAddressBus(volatile datum * baseAddress, unsigned long nBytes)
{
	unsigned long addressMask = (nBytes / sizeof(datum) - 1);
	unsigned long offset;
	unsigned long testOffset;

	datum pattern = (datum) 0xAAAAAAAA;
	datum antipattern = (datum) 0x55555555;

	/* Write the default pattern at each of the power-of-two offsets. */
	for (offset = 1; (offset & addressMask) != 0; offset <<= 1) {
		baseAddress[offset] = pattern;
	}

	/* Check for address bits stuck high. */
	testOffset = 0;
	baseAddress[testOffset] = antipattern;

	for (offset = 1; (offset & addressMask) != 0; offset <<= 1) {
		if (baseAddress[offset] != pattern) {
			return ((datum *) & baseAddress[offset]);
		}
	}

	baseAddress[testOffset] = pattern;

	/* Check for address bits stuck low or shorted. */
	for (testOffset = 1; (testOffset & addressMask) != 0; testOffset <<= 1) {
		baseAddress[testOffset] = antipattern;

		if (baseAddress[0] != pattern) {
			return ((datum *) & baseAddress[testOffset]);
		}

		for (offset = 1; (offset & addressMask) != 0; offset <<= 1) {
			if ((baseAddress[offset] != pattern) && (offset != testOffset)) {
				return ((datum *) & baseAddress[testOffset]);
			}
		}

		baseAddress[testOffset] = pattern;
	}

	return (NULL);
}		/* memTestAddressBus() */

/*
 *
 * Function:	memTestDevice()
 *
 * Description:	Test the integrity of a physical memory device by
 *				performing an increment/decrement test over the
 *				entire region.  In the process every storage bit
 *				in the device is tested as a zero and a one.  The
 *				base address and the size of the region are
 *				selected by the caller.
 *
 * Notes:
 *
 * Returns:		NULL if the test succeeds.
 *
 *				A non-zero result is the first address at which an
 *				incorrect value was read back.  By examining the
 *				contents of memory, it may be possible to gather
 *				additional information about the problem.
 */
datum *memTestDevice(volatile datum * baseAddress, unsigned long nBytes)
{
	unsigned long offset;
	unsigned long nWords = nBytes / sizeof(datum);

	datum pattern;
	datum antipattern;

	/* Fill memory with a known pattern. */
	for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++) {
		baseAddress[offset] = pattern;
	}

	/* Check each location and invert it for the second pass. */
	for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++) {
		if (baseAddress[offset] != pattern) {
			return ((datum *) & baseAddress[offset]);
		}

		antipattern = ~pattern;
		baseAddress[offset] = antipattern;
	}

	/* Check each location for the inverted pattern and zero it. */
	for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++) {
		antipattern = ~pattern;
		if (baseAddress[offset] != antipattern) {
			return ((datum *) & baseAddress[offset]);
		}
	}

	return (NULL);
}		/* memTestDevice() */
