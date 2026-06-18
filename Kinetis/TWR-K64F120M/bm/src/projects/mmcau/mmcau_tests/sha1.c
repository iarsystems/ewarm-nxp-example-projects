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
#include "sha1.h"
#include "crypto_common.h"

/*
 * Note: FIPS 180-2 says SHA-1 and SHA-256 outputs are integers
 * . . . the resulting 160-bit message digest of the message, M, is H0 |
 * H1 | H2 | H3 | H4
 */

/* SHA1 online: http://www.functions-online.com/sha1.html */

/* FSL: testcases */
HashSHA1Test sha1_test[] = {
	{(unsigned char *)"",
	 {0xda39a3ee, 0x5e6b4b0d, 0x3255bfef, 0x95601890, 0xafd80709}},
	{(unsigned char *)"a",
	 {0x86f7e437, 0xfaa5a7fc, 0xe15d1ddc, 0xb9eaeaea, 0x377667b8}},
	{(unsigned char *)"abc",
	 {0xa9993e36, 0x4706816a, 0xba3e2571, 0x7850c26c, 0x9cd0d89d}},
	{(unsigned char *)"message digest",
	 {0xc12252ce, 0xda8be899, 0x4d5fa029, 0x0a47231c, 0x1d16aae3}},
	{(unsigned char *)"abcdefghijklmnopqrstuvwxyz",
	 {0x32d10c7b, 0x8cf96570, 0xca04ce37, 0xf2a19d84, 0x240d3a89}},
	{(unsigned char *)
	 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
	 {0x761c457b, 0xf73b14d2, 0x7e9e9265, 0xc46f4b4d, 0xda11f940}},
	{(unsigned char *)
	 "1234567890123456789012345678901234567890"
	 "1234567890123456789012345678901234567890",
	 {0x50abf570, 0x6a150990, 0xa08b2c5e, 0xa40fa0e5, 0x85554732}},
};

/* FSL: new test cases */
unsigned int sha1_result1[] = {
	0xb7a1f498, 0xdaf49dce, 0x8fb714b4,
	0xc27fe1a1, 0xee33150e
};
unsigned int sha1_result2[] = {
	0xda39a3ee, 0x5e6b4b0d, 0x3255bfef,
	0x95601890, 0xafd80709
};

/* sha1_main: performs SHA1 test using CAU */
void sha1_main(void)
{
	unsigned int sha1_result[SHA1_RESULT_LENGTH / 4];
	unsigned char *sha1_padded;
	unsigned int i, blocks, length;
	unsigned char *p, *q, pad[CRYPTO_BLOCK_LENGTH];

	/* Testing cau_sha1_update() */
	for (i = 0; i < SHA1_TEST_CASES; i++) {
		/* get string length */
		length = strlen((char const *)sha1_test[i].input);
		/* calculate padding */
		if ((sha1_padded = padding_start(sha1_test[i].input,
										 &length,
										 CRYPTO_BIG_ENDIAN)) == NULL) {
			printf("Out of memory\n");
			break;
		}

		/*
		 * calculate number of 512-bit blocks present in the message
		 * multiple of CRYPTO_BLOCK_LENGTH bytes alway because of padding
		 */
		blocks = length / CRYPTO_BLOCK_LENGTH;

		/* Hash with SHA1 everything */
		cau_sha1_update(sha1_padded, blocks, sha1_result);

		if (compare_arrays((unsigned char *)sha1_result,
						   (unsigned char *)sha1_test[i].output,
						   SHA1_RESULT_LENGTH))
			printf("Error during SHA1 testcase %d\n", i);

		/* release allocated memory */
		padding_end(sha1_padded);
	}

	/* 2nd Test */
	p = (unsigned char *)"This test tries to use the n-block utility from the hash library";	/* 64 bytes */
	q = (unsigned char *)" and as a matter of fact we're trying to get only 128 characters, anyway we need to add 64 more bytes to finally get 192 bytes!!";	/* 128 bytes */
	/* sha1_result1 is the output */

	/* calculating length in bits */
	length = 192 << 3;
	/* add padding manually */
	pad[0] = 0x80;		/* set bit */
	for (i = 1; i < 56; i++)
		pad[i] = 0;		/* clear the rest */
	/* add length */
	pad[56] = 0;
	pad[57] = 0;
	pad[58] = 0;
	pad[59] = 0;
	pad[60] = length >> 24 & 0xff;
	pad[61] = length >> 16 & 0xff;
	pad[62] = length >> 8 & 0xff;
	pad[63] = length >> 0 & 0xff;

	/* FSL: Initializes the SHA1 state variables */
	cau_sha1_initialize_output(sha1_result);	//init state variables

	/*
	 * FSL: Updates SHA1 state variables for one or more input message blocks
	 * note state variables is [IN/OUT] for each nth block, then 512-bit
	 * blocks can be non-contiguous
	 */
	cau_sha1_hash_n(&p[0], 1, sha1_result);		/* 1 block */
	cau_sha1_hash_n(&q[0], 2, sha1_result);		/* 2 blocks */
	cau_sha1_hash_n(&pad[0], 1, sha1_result);	/* 1 block */

	/* FSL: check if output matches with the precalculated result */
	if (compare_arrays((unsigned char *)sha1_result,
					   (unsigned char *)sha1_result1,
					   SHA1_RESULT_LENGTH))
		printf("Error during SHA1 testcase using cau_sha1_hash_n()\n");

	/*3rd test */

	/* using an empty string as test! */

	/* sha1_result2 is the output */

	/* calculating length in bits */
	length = 0 << 3;
	/* add padding manually */
	pad[0] = 0x80;		/* set bit */
	for (i = 1; i < 56; i++)
		pad[i] = 0;		/* clear the rest */
	/* add length */
	pad[56] = 0;
	pad[57] = 0;
	pad[58] = 0;
	pad[59] = 0;
	pad[60] = length >> 24 & 0xff;
	pad[61] = length >> 16 & 0xff;
	pad[62] = length >> 8 & 0xff;
	pad[63] = length >> 0 & 0xff;

	/* FSL: Initializes the SHA1 state variables */
	cau_sha1_initialize_output(sha1_result);	/* init state variables */

	/*
	 * FSL: Performs SHA1 hash algorithm for a single input message block
	 * note cau_sha1_initialize_output must be called prior to cau_sha1_hash,
	 * which is different than cau_sha1_update(...)
	 */
	cau_sha1_hash(&pad[0], sha1_result);

	/*FSL: check if output matches with the precalculated result */
	if (compare_arrays((unsigned char *)sha1_result,
					   (unsigned char *)sha1_result2,
					   SHA1_RESULT_LENGTH))
		printf("Error during SHA1 testcase using cau_sha1_hash()\n");

	/* FSL: we are done! */
	printf("SHA1 tests completed\n");
}
