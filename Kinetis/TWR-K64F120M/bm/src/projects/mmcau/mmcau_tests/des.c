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

#include "stdlib.h"
#include "common.h"
#include "cau_api.h"
#include "des.h"
#include "crypto_common.h"

extern unsigned char parityBits[128];

/* 8-byte multiple */
unsigned char des_test[] =
		"Once upon a midnight dreary, while I pondered weak and weary,"
		"Over many a quaint and curious volume of forgotten lore,"
		"While I nodded, nearly napping, suddenly there came a tapping,"
		"As of some one gently rapping, rapping at my chamber door"
		"Tis some visitor, I muttered, tapping at my chamber door"
		"Only this, and nothing more.";

/* FSL: taken from http://www.tero.co.uk/des/test.php */
unsigned char des_encrypted[] = {
	 0xfa, 0xfe, 0xcd, 0xd1, 0x13, 0x6e, 0x68, 0xf2,
	 0x12, 0xf7, 0x7a, 0xed, 0x26, 0x7e, 0x03, 0x36,
	 0x56, 0xa8, 0xeb, 0xa1, 0xa3, 0xdf, 0x88, 0x48,
	 0x31, 0x8e, 0x41, 0x8b, 0x16, 0x0c, 0x3f, 0x2d,
	 0x54, 0xc5, 0x98, 0x67, 0x2e, 0x47, 0xd1, 0x2c,
	 0xc8, 0x54, 0x29, 0xb4, 0x73, 0xc1, 0xa1, 0x70,
	 0x31, 0x73, 0xc3, 0xb7, 0xbe, 0xd6, 0xc2, 0xb5,
	 0x71, 0x84, 0xab, 0xe6, 0x62, 0x3a, 0xb0, 0x6f,
	 0x9d, 0xd9, 0x9a, 0xd3, 0x28, 0xd5, 0xfc, 0x61,
	 0xed, 0x17, 0x09, 0x48, 0xdf, 0x0e, 0xdf, 0xeb,
	 0x29, 0xd0, 0x22, 0x0f, 0x33, 0x49, 0xec, 0x22,
	 0x6f, 0x53, 0x8e, 0x7d, 0xf2, 0xbd, 0xbe, 0x8b,
	 0x88, 0xbd, 0x9c, 0xd8, 0x5c, 0x1b, 0x3e, 0x1a,
	 0xc4, 0xf7, 0xf5, 0xe6, 0xf7, 0x8e, 0xd4, 0xa3,
	 0xbf, 0x36, 0xc3, 0x79, 0x2f, 0xd6, 0x2a, 0xff,
	 0xa3, 0xa9, 0x78, 0x72, 0x1e, 0xf4, 0x36, 0x81,
	 0xe3, 0x9e, 0xe8, 0x03, 0x8a, 0x5c, 0x94, 0xf2,
	 0xe4, 0x15, 0xbf, 0x6b, 0x1d, 0x85, 0x44, 0x31,
	 0x03, 0x1d, 0x45, 0xba, 0xf2, 0xc0, 0x6e, 0x4c,
	 0x4f, 0x03, 0xb6, 0xea, 0x35, 0x04, 0xde, 0x38,
	 0xc9, 0x0e, 0x19, 0x34, 0xfa, 0x19, 0xc5, 0xa0,
	 0x67, 0x54, 0x1f, 0x00, 0xe7, 0x61, 0xbd, 0xba,
	 0x5e, 0xeb, 0xe7, 0x2f, 0x1d, 0x54, 0x3d, 0x26,
	 0x11, 0x0f, 0x22, 0x4c, 0x1e, 0xb5, 0x3b, 0xaa,
	 0xab, 0x38, 0x48, 0x6e, 0xbf, 0x85, 0xcb, 0x6b,
	 0x89, 0xda, 0xfd, 0xf3, 0x41, 0x26, 0x1f, 0xfc,
	 0x1a, 0x2e, 0x77, 0xcc, 0x04, 0xe1, 0x53, 0x9f,
	 0x59, 0x36, 0xa9, 0x27, 0xca, 0x70, 0x83, 0x26,
	 0xcc, 0xe6, 0x0a, 0xf7, 0xe7, 0x2c, 0x7c, 0x31,
	 0xaa, 0x2e, 0xa7, 0xd9, 0x41, 0x39, 0x8d, 0xb3,
	 0x17, 0x84, 0x2f, 0x27, 0xcf, 0x12, 0x81, 0x8f,
	 0xb8, 0xf3, 0x05, 0xed, 0xca, 0x30, 0x73, 0xa4,
	 0x5e, 0xe9, 0xd7, 0x54, 0x26, 0xa2, 0x29, 0x68,
	 0xee, 0x2b, 0x30, 0x94, 0xc2, 0x35, 0x92, 0xc6,
	 0x4d, 0xd1, 0x4c, 0x80, 0xfb, 0x44, 0xc0, 0x3e,
	 0xc2, 0x5e, 0xa9, 0xe0, 0xe5, 0xfb, 0x62, 0x30,
	 0x0c, 0x0b, 0x95, 0xd5, 0x16, 0xbb, 0x5d, 0xf0,
	 0x08, 0x9c, 0x42, 0x84, 0xe3, 0x8d, 0x48, 0xd9,
	 0x5c, 0xd8, 0x44, 0x61, 0xc1, 0x07, 0xe2, 0x75,
	 0x68, 0x75, 0xf9, 0xbc, 0x4d, 0x56, 0x67, 0x98
};

/*
 * after encrypt/decrypt we must have original input:
 * symmetric crypto algorithm DES in CBC mode
 */
void des_main(void)
{
	/* Testing DES support */
	unsigned int length;

	/* 8 bytes key: "password" */
	/* Note: parity is fixed inside DES crypto function */
	unsigned char key[DES_KEY_LENGTH] = {
		0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64
	};

	/* initialization vector: 8 bytes: "mysecret" */
	unsigned char ive[DES_BLOCK_LENGTH] = {
		0x6d, 0x79, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74
	};

	/* result holding the example max length array */
	unsigned char output[sizeof(des_test) - 1];

	//for(i=0;i<DES_TEST_CASES;i++)
	//{
	length = strlen((const char *)des_test);

	/* encrypt */
	if (encryptDES(key, des_test, output, length, ive) != 0)
		printf("Error during encryption\n\r");

	/* compare encrypted info */
	if (compare_arrays(output, des_encrypted, length) != 0)
		printf("DES encryption error\n\r");

	/* decrypt */
	if (decryptDES(key, output, output, length, ive) != 0)
		printf("Error during decryption\n\r");

	/* compare decrypted info */
	if (compare_arrays(output, des_test, length) != 0)
		printf("DES decryption error\n\r");
	//}

	printf("DES test completed\n");
}

/*
 * encryptDES: DES encryption function
 *
 * Parameters:
 *	[in] key: key of 8 bytes
 *	[in] input_data: pointer to in data
 *	[out] output_data: pointer to out data
 *	[in] data_length: number of bytes of input data.
 *		 Must be divisible by 8 (DES block)
 *	[in] init_vector: init_vector to use during xor
 * Return:
 *	0 if OK, otherwise error
 */
unsigned char encryptDES(unsigned char *key,
						 unsigned char *input_data,
						 unsigned char *output_data,
						 unsigned short data_length,
						 unsigned char *init_vector)
{
	unsigned char i;
	unsigned short blocks;
	unsigned char temp_block[DES_BLOCK_LENGTH];
	unsigned char temp_iv[DES_BLOCK_LENGTH];
	unsigned int parity;

	/* validate NULL for key, input_data, output_data or init_vector */
	if ((key == NULL) || (input_data == NULL) ||
		(output_data == NULL) || (init_vector == NULL))
		return (unsigned char)-1;	/*wrong pointer */

	/* validate data length */
	if (data_length % DES_BLOCK_LENGTH)
		return (unsigned char)-1;	/*wrong length */

	/*
	 * fix parity key if needed: keep in mind LSB of
	 * each byte is only used for parity
	 */
	for (i = 0; i < DES_KEY_LENGTH; i++)
		key[i] = ((key[i] & 0xFE) | parityBits[key[i] >> 1]);

	/* FSL: optional -- check parity */

	parity = cau_des_chk_parity(key);

	if (parity != NULL)
		return (unsigned char)-1;	/* wrong parity */

	/* execute DES in CBC mode */
	/* http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation */

	/* get number of blocks */
	blocks = data_length / DES_BLOCK_LENGTH;

	/* copy init vector to temp storage */
	memcpy((void *)temp_iv, (void *)init_vector, DES_BLOCK_LENGTH);

	do {
		/* copy to temp storage */
		memcpy((void *)temp_block, (void *)input_data,
		       DES_BLOCK_LENGTH);

		/* xor for CBC */
		for (i = 0; i < DES_BLOCK_LENGTH; i++)
			temp_block[i] ^= temp_iv[i];

		/* FSL: single DES round */
		cau_des_encrypt(temp_block, key, output_data);

		/* update init_vector for next DES round */
		memcpy((void *)temp_iv, (void *)output_data, DES_BLOCK_LENGTH);

		/* adjust pointers for next DES round */
		input_data += DES_BLOCK_LENGTH;
		output_data += DES_BLOCK_LENGTH;

	} while (--blocks);

	return 0;		/*OK */
}

/*
 * decryptDES: DES decryption function
 *
 * Parameters:
 *	[in] key: key of 8 bytes
 *	[in] input_data: pointer to in data
 *	[out] output_data: pointer to out data
 *	[in] data_length: number of bytes of input data.
 *		 Must be divisible by 8 (DES block)
 *	[in] init_vector: init_vector to use during xor
 * Return:
 *	0 if OK, otherwise error
 */
unsigned char decryptDES(unsigned char *key,
						 unsigned char *input_data,
						 unsigned char *output_data,
						 unsigned short data_length,
						 unsigned char *init_vector)
{
	unsigned char i;
	unsigned short blocks;
	unsigned char temp_block[DES_BLOCK_LENGTH];
	unsigned char temp_iv[DES_BLOCK_LENGTH];

	/* validate NULL for key, input_data, output_data or init_vector */
	if ((key == NULL) || (input_data == NULL) ||
		(output_data == NULL) || (init_vector == NULL))
		return (unsigned char)-1;	/*wrong pointer */

	/* validate data length */
	if (data_length % DES_BLOCK_LENGTH)
		return (unsigned char)-1;	/*wrong length */

	/*
	 * fix parity key if needed: keep in mind LSB of
	 * each byte is only used for parity
	 */
	for (i = 0; i < DES_KEY_LENGTH; i++)
		key[i] = ((key[i] & 0xFE) | parityBits[key[i] >> 1]);

	/* FSL: optional -- check parity */
	cau_des_chk_parity(key);

	/* execute DES in CBC mode */
	/* http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation */

	/* get number of blocks */
	blocks = data_length / DES_BLOCK_LENGTH;

	/* copy init vector to temp storage */
	memcpy((void *)temp_iv, (void *)init_vector, DES_BLOCK_LENGTH);

	do {
		/* copy to temp storage */
		memcpy((void *)temp_block, (void *)input_data,
		       DES_BLOCK_LENGTH);

		/* FSL: single DES round */
		cau_des_decrypt(input_data, key, output_data);

		/* xor for CBC */
		for (i = 0; i < DES_BLOCK_LENGTH; i++)
			output_data[i] ^= temp_iv[i];

		/* update init_vector for next DES round */
		memcpy((void *)temp_iv, (void *)temp_block, DES_BLOCK_LENGTH);

		/* adjust pointers for next DES round */
		input_data += DES_BLOCK_LENGTH;
		output_data += DES_BLOCK_LENGTH;

	} while (--blocks);

	return 0;		/* OK */
}
