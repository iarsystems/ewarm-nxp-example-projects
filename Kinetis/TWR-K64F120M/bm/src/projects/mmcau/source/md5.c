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
#include "cau_api.h"
#include "md5.h"
#include "crypto_common.h"

/*
 * RFC-1321 specifically says MD5 outputs bytes in "little endian" order:
 *  The message digest produced as output is A, B, C, D. That is, we begin
 *  with the low-order byte of A, and end with the high-order byte of D.
 */

/* MD5 online: http://www.functions-online.com/md5.html */

/* FSL: testcases */
HashMD5Test md5_test[] = {
	{(unsigned char *)"",
	 {0xd4, 0x1d, 0x8c, 0xd9,
	  0x8f, 0x00, 0xb2, 0x04,
	  0xe9, 0x80, 0x09, 0x98,
	  0xec, 0xf8, 0x42, 0x7e}},
	{(unsigned char *)"a",
	 {0x0c, 0xc1, 0x75, 0xb9,
	  0xc0, 0xf1, 0xb6, 0xa8,
	  0x31, 0xc3, 0x99, 0xe2,
	  0x69, 0x77, 0x26, 0x61}},
	{(unsigned char *)"abc",
	 {0x90, 0x01, 0x50, 0x98,
	  0x3c, 0xd2, 0x4f, 0xb0,
	  0xd6, 0x96, 0x3f, 0x7d,
	  0x28, 0xe1, 0x7f, 0x72}},
	{(unsigned char *)"message digest",
	 {0xf9, 0x6b, 0x69, 0x7d,
	  0x7c, 0xb7, 0x93, 0x8d,
	  0x52, 0x5a, 0x2f, 0x31,
	  0xaa, 0xf1, 0x61, 0xd0}},
	{(unsigned char *)"abcdefghijklmnopqrstuvwxyz",
	 {0xc3, 0xfc, 0xd3, 0xd7,
	  0x61, 0x92, 0xe4, 0x00,
	  0x7d, 0xfb, 0x49, 0x6c,
	  0xca, 0x67, 0xe1, 0x3b}},
	{(unsigned char *)
	 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
	 {0xd1, 0x74, 0xab, 0x98,
	  0xd2, 0x77, 0xd9, 0xf5,
	  0xa5, 0x61, 0x1c, 0x2c,
	  0x9f, 0x41, 0x9d, 0x9f}},
	{(unsigned char *)
	 "1234567890123456789012345678901234567890"
	 "1234567890123456789012345678901234567890",
	 {0x57, 0xed, 0xf4, 0xa2,
	  0x2b, 0xe3, 0xc9, 0x55,
	  0xac, 0x49, 0xda, 0x2e,
	  0x21, 0x07, 0xb6, 0x7a}},
};

/* FSL: new test cases */
unsigned char md5_result1[] = {
	0xc3, 0x1f, 0x5e, 0x69,
	0xdd, 0x2e, 0x5c, 0xde,
	0x38, 0x53, 0x22, 0x0e,
	0x60, 0x1c, 0x01, 0xf1
};
unsigned char md5_result2[] = {
	0xd4, 0x1d, 0x8c, 0xd9,
	0x8f, 0x00, 0xb2, 0x04,
	0xe9, 0x80, 0x09, 0x98,
	0xec, 0xf8, 0x42, 0x7e
};

/* md5_main: performs MD5 test using CAU */
void md5_main(void)
{
	unsigned char md5_result[MD5_RESULT_LENGTH];
	unsigned char *md5_padded;
	unsigned int i, blocks, length;

	unsigned char *p, *q, pad[CRYPTO_BLOCK_LENGTH];

	/* Testing cau_md5_update() */
	for (i = 0; i < MD5_TEST_CASES; i++) {
		/* get string length */
		length = strlen((char const *)md5_test[i].input);
		/* calculate padding */
		if ((md5_padded = padding_start(md5_test[i].input,
										&length,
										CRYPTO_LITTLE_ENDIAN)) == NULL) {
			printf("Out of memory\n");
			break;
		}

		/*
		 * calculate number of 512-bit blocks present in the message
		 * multiple of CRYPTO_BLOCK_LENGTH bytes alway because of padding
		 */
		blocks = length / CRYPTO_BLOCK_LENGTH;

		/* Hash with MD5 everything */
		cau_md5_update(md5_padded, blocks, md5_result);

		if (compare_arrays(md5_result,
						   md5_test[i].output,
						   MD5_RESULT_LENGTH))
			printf("Error during MD5 testcase %d\n", i);

		/* release allocated memory */
		padding_end(md5_padded);
	}

	/* 2nd Test */
	p = (unsigned char *)"This test tries to use the n-block utility from the hash library";	/* 64 bytes */
	q = (unsigned char *)" and as a matter of fact we're trying to get only 128 characters, anyway we need to add 64 more bytes to finally get 192 bytes!!";	/* 128 bytes */
	/* md5_result1 is the output */

	/* calculating length in bits */
	length = 192 << 3;
	/* add padding manually */
	pad[0] = 0x80;		/* set bit */
	for (i = 1; i < 56; i++)
		pad[i] = 0;		/* clear the rest * /

	/* add length */
	pad[56] = length >> 0 & 0xff;
	pad[57] = length >> 8 & 0xff;
	pad[58] = length >> 16 & 0xff;
	pad[59] = length >> 24 & 0xff;
	pad[60] = 0;
	pad[61] = 0;
	pad[62] = 0;
	pad[63] = 0;

	/* FSL: Initializes the MD5 state variables */
	cau_md5_initialize_output(md5_result);	/*init state variables */

	/*
	 * FSL: Updates MD5 state variables for one or more input message blocks
	 * note state variables is [IN/OUT] for each nth block, then 512-bit
	 * blocks can be non-contiguous
	 */
	cau_md5_hash_n(&p[0], 1, md5_result);	/* 1 block */
	cau_md5_hash_n(&q[0], 2, md5_result);	/* 2 blocks */
	cau_md5_hash_n(&pad[0], 1, md5_result);	/* 1 block */

	/* FSL: check if output matches with the precalculated result */
	if (compare_arrays(md5_result, md5_result1, MD5_RESULT_LENGTH))
		printf("Error during MD5 testcase using cau_md5_hash_n()\n");

	/* 3rd test */

	/* using an empty string as test! */

	/* md5_result2 is the output */

	/* calculating length in bits */
	length = 0 << 3;
	/* add padding manually */
	pad[0] = 0x80;		/* set bit */
	for (i = 1; i < 56; i++)
		pad[i] = 0;	/* clear the rest */
	/* add length */
	pad[56] = length >> 0 & 0xff;
	pad[57] = length >> 8 & 0xff;
	pad[58] = length >> 16 & 0xff;
	pad[59] = length >> 24 & 0xff;
	pad[60] = 0;
	pad[61] = 0;
	pad[62] = 0;
	pad[63] = 0;

	/* FSL: Initializes the MD5 state variables */
	cau_md5_initialize_output(md5_result);	/*init state variables */

	/*
	 * FSL: Performs MD5 hash algorithm for a single input message block
	 * note cau_md5_initialize_output must be called prior to cau_md5_hash,
	 * which is different than cau_md5_update(...)
	 */
	cau_md5_hash(&pad[0], md5_result);

	/* FSL: check if output matches with the precalculated result */
	if (compare_arrays(md5_result, md5_result2, MD5_RESULT_LENGTH))
		printf("Error during MD5 testcase using cau_md5_hash()\n");

	/* FSL: we are done! */
	printf("MD5 tests completed\n");
}
