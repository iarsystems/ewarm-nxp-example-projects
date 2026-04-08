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
#include "sha256.h"
#include "crypto_common.h"

/*
 * Note: FIPS 180-2 says SHA-1 and SHA-256 outputs are integers
 * ...the resulting 256-bit message digest of the message, M, is H0 | H1 |
 * H2 | H3 | H4 | H5 | H6 | H7
 */

/* SHA256 online: http://www.xorbin.com/tools/sha256-hash-calculator */

/* FSL: testcases */
HashSHA256Test sha256_test[] = {
	{(unsigned char *)"",
	 {0xe3b0c442, 0x98fc1c14, 0x9afbf4c8, 0x996fb924, 0x27ae41e4,
	  0x649b934c, 0xa495991b, 0x7852b855}},
	{(unsigned char *)"abc",
	 {0xba7816bf, 0x8f01cfea, 0x414140de, 0x5dae2223, 0xb00361a3,
	  0x96177a9c, 0xb410ff61, 0xf20015ad}},
	{(unsigned char *)
	 "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	 {0x248d6a61, 0xd20638b8, 0xe5c02693, 0x0c3e6039, 0xa33ce459,
	  0x64ff2167, 0xf6ecedd4, 0x19db06c1}},
	{(unsigned char *)
	 "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno"
	 "ijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
	 {0xcf5b16a7, 0x78af8380, 0x036ce59e, 0x7b049237, 0x0b249b11,
	  0xe8f07a51, 0xafac4503, 0x7afee9d1}},
	{(unsigned char *)
	 "This is exactly 64 bytes long, not counting the terminating byte",
	 {0xab64eff7, 0xe88e2e46, 0x165e29f2, 0xbce41826, 0xbd4c7b35,
	  0x52f6b382, 0xa9e7d3af, 0x47c245f8}},
	{(unsigned char *)
	 "For this sample, this 63-byte string will be used as input data",
	 {0xf08a78cb, 0xbaee082b, 0x052ae070, 0x8f32fa1e, 0x50c5c421,
	  0xaa772ba5, 0xdbb406a2, 0xea6be342}},
	{(unsigned char *)
	 "And this textual data, astonishing as it may appear, is exactly "
	 "128 bytes in length, as are both SHA-384 and SHA-512 block sizes",
	 {0x0ab80334, 0x4830f920, 0x89494fb6, 0x35ad00d7, 0x6164ad6e,
	  0x57012b23, 0x7722df0d, 0x7ad26896}},
	{(unsigned char *)
	 "By hashing data that is one byte less than a multiple of a hash "
	 "block length (like this 127-byte string), bugs may be revealed.",
	 {0xe4326d04, 0x59653d7d, 0x3514674d, 0x713e74dc, 0x3df11ed4,
	  0xd30b4013, 0xfd327fdb, 0x9e394c26}},
	{(unsigned char *)"The quick brown fox jumps over the lazy dog",
	 {0xd7a8fbb3, 0x07d78094, 0x69ca9abc, 0xb0082e4f, 0x8d5651e4,
	  0x6d3cdb76, 0x2d02d0bf, 0x37c9e592}},
	{(unsigned char *)
	 "This test tries to use the n-block utility from the hash library "
	 "and as a matter of fact we're trying to get only 128 characters",
	 {0x2ce675bd, 0x3b70e104, 0xd696d1b2, 0x5bf3d42b, 0x2b45cd77,
	  0x6d4f590f, 0x210f12c4, 0x4bf473d5}},
};

/*FSL: new test cases*/
unsigned int sha256_result1[] = {
	0xf8c21c3f, 0x5f3f5198, 0xb22f97d3, 0x0e33e341,
	0x000f6cbc, 0x64e79716, 0xcaf2c249, 0xf77bd8be
};
unsigned int sha256_result2[] = {
	0xe3b0c442, 0x98fc1c14, 0x9afbf4c8, 0x996fb924,
	0x27ae41e4, 0x649b934c, 0xa495991b, 0x7852b855
};

/* sha256_main: performs SHA256 test using CAU */
void sha256_main(void)
{
	unsigned int sha256_result[SHA256_RESULT_LENGTH / 4];
	unsigned char *sha256_padded;
	unsigned int i, blocks, length;
	unsigned char *p, *q, pad[CRYPTO_BLOCK_LENGTH];

	/* Testing cau_sha256_update() */
	for (i = 0; i < SHA256_TEST_CASES; i++) {
		/* get string length */
		length = strlen((char const *)sha256_test[i].input);
		/* calculate padding */
		if ((sha256_padded = padding_start(sha256_test[i].input,
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

		/* Hash with SHA256 everything */
		cau_sha256_update(sha256_padded, blocks, sha256_result);

		if (compare_arrays((unsigned char *)sha256_result,
						   (unsigned char *)sha256_test[i].output,
						   SHA256_RESULT_LENGTH))
			printf("Error during SHA256 testcase %d\n", i);

		/* release allocated memory */
		padding_end(sha256_padded);
	}

	/* 2nd Test */
	p = (unsigned char *)"This test tries to use the n-block utility from the hash library";	/* 64 bytes */
	q = (unsigned char *)" and as a matter of fact we're trying to get only 128 characters, anyway we need to add 64 more bytes to finally get 192 bytes!!";	/* 128 bytes */
	/* sha256_result1 is the output */

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

	/* FSL: Initializes the SHA256 state variables */
	cau_sha256_initialize_output(sha256_result);	/* init state variables */

	/*
	 * FSL: Updates SHA256 state variables for one or more input message blocks
	 * note state variables is [IN/OUT] for each nth block, then 512-bit
	 * blocks can be non-contiguous
	 */
	cau_sha256_hash_n(&p[0], 1, sha256_result);		/* 1 block */
	cau_sha256_hash_n(&q[0], 2, sha256_result);		/* 2 blocks */
	cau_sha256_hash_n(&pad[0], 1, sha256_result);	/* 1 block */

	/* FSL: check if output matches with the precalculated result */
	if (compare_arrays((unsigned char *)sha256_result,
					   (unsigned char *)sha256_result1,
					   SHA256_RESULT_LENGTH))
		printf("Error during SHA256 testcase using cau_sha256_hash_n()\n");

	/* 3rd test */

	/* using an empty string as test! */

	/* sha256_result2 is the output */

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

	/* FSL: Initializes the SHA256 state variables */
	cau_sha256_initialize_output(sha256_result);	/* init state variables */

	/*
	 * FSL: Performs SHA256 hash algorithm for a single input message block
	 * note cau_sha256_initialize_output must be called prior to
	 * cau_sha256_hash, which is different than cau_sha256_update(...)
	 */
	cau_sha256_hash(&pad[0], sha256_result);

	/* FSL: check if output matches with the precalculated result */
	if (compare_arrays((unsigned char *)sha256_result,
					   (unsigned char *)sha256_result2,
					   SHA256_RESULT_LENGTH))
		printf("Error during SHA256 testcase using cau_sha256_hash()\n");

	/* FSL: we are done! */
	printf("SHA256 tests completed\n");
}
