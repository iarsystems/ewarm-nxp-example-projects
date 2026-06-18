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

#include "common.h"
#include "crypto_common.h"

unsigned char parityBits[128] = {
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0
};

/*
 * padding_start: add padding to hashing array
 *	NOTE: requires heap to get memory at runtime
 * Parameters:
 *	[in] *input:	hashing array for padding
 *	[in,out] *input_length: get hashing array length, returns new length
 *	[in] endianess: endianess to use. MD5 little endian, SHA1 and
 *					SHA256 use big endian
 * Returns:
 *	new padding array address. NULL if error.
 */
unsigned char *padding_start(unsigned char *input,
							 unsigned int *input_length,
							 unsigned char endianess)
{
	unsigned char *padding_array;
	signed char padding_length;
	unsigned int temp_length;
	unsigned int bits_length;
	unsigned int final_length;

	temp_length = *input_length % CRYPTO_BLOCK_LENGTH;

	/*
	 * get padding length: padding special case when 448 mod 512
	 * working with bytes rather than bits
	 */
	if (!((padding_length = CRYPTO_MAX_PADDING_LENGTH -
		(temp_length % CRYPTO_BLOCK_LENGTH)) > 0)) {
		padding_length = CRYPTO_BLOCK_LENGTH -
						 (temp_length % CRYPTO_MAX_PADDING_LENGTH);
	}

	padding_length += temp_length / CRYPTO_BLOCK_LENGTH;
	temp_length = *input_length;

	/* reserve necessary memory */
	final_length = temp_length + padding_length +
				   CRYPTO_ARRAY_LENGTH;	/* bits length */
	if ((padding_array = (unsigned char *)malloc(final_length)) == NULL)
		return (unsigned char *)NULL;	/* not enough mem */

	/* copy original data to new padding array */
	memcpy((void *)padding_array, (void *)input, temp_length);

	/* add padding */
	padding_array[temp_length++] = 0x80;	/* first bit enabled */
	while ((--padding_length != 0))
		padding_array[temp_length++] = 0;	/* clear the rest */

	/* add length depending on endianess: get number of bits */
	bits_length = *input_length << 3;
	*input_length = final_length;

	if (endianess == CRYPTO_LITTLE_ENDIAN) {
		padding_array[temp_length++] = bits_length & 0xff;
		padding_array[temp_length++] = bits_length >> 8 & 0xff;
		padding_array[temp_length++] = bits_length >> 16 & 0xff;
		padding_array[temp_length++] = bits_length >> 24 & 0xff;
		padding_array[temp_length++] = 0;
		padding_array[temp_length++] = 0;
		padding_array[temp_length++] = 0;
		padding_array[temp_length] = 0;
	} else {
		/* CRYPTO_BIG_ENDIAN */
		padding_array[temp_length++] = 0;
		padding_array[temp_length++] = 0;
		padding_array[temp_length++] = 0;
		padding_array[temp_length++] = 0;
		padding_array[temp_length++] = bits_length >> 24 & 0xff;
		padding_array[temp_length++] = bits_length >> 16 & 0xff;
		padding_array[temp_length++] = bits_length >> 8 & 0xff;
		padding_array[temp_length] = bits_length & 0xff;
	}

	return padding_array;
}

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
							 unsigned length)
{
	unsigned i;

	for (i = 0; i < length; i++) {
		if (first[i] != second[i])
			return (unsigned char)-1;
	}
	return 0;
}
