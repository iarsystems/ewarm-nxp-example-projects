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

#ifndef _CRYPTO_COMMON_H_
#define _CRYPTO_COMMON_H_

#define padding_end						free

#define CRYPTO_LITTLE_ENDIAN			0
#define CRYPTO_BIG_ENDIAN				1

#define CRYPTO_ARRAY_LENGTH				8
#define CRYPTO_MAX_PADDING_LENGTH		56
#define CRYPTO_BLOCK_LENGTH				64

/*
 * padding_start: add padding to hashing array
 *	NOTE: requires heap to get memory at runtime
 * Parameters:
 *	[in] *input:    hashing array for padding
 *	[in,out] *input_length: get hashing array length, returns new length
 *	[in] endianess: endianess to use. MD5 little endian,
 *		 SHA1 and SHA256 use big endian
 * Returns:
 *	new padding array address. NULL if error.
 */
unsigned char *padding_start(unsigned char *input,
							 unsigned int *input_length,
							 unsigned char endianess);

/*
 * compare_arrays: compares 2 arrays
 * Parameters:
 *	[in] *first: 1st array
 *	[in] *second: 2nd array
 *	[in] length: arrays length
 * Returns:
 *	0 if OK otherwise error
 */
unsigned char compare_arrays(unsigned char *first,
							 unsigned char *second,
							 unsigned length);

#endif
