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
#include "des3.h"
#include "crypto_common.h"

extern unsigned char parityBits[128];

/* 8-byte multiple */
unsigned char des3_test[] =
	"And the raven, never flitting, still is sitting, still is sitting"
	"On the pallid bust of Pallas just above my chamber door;"
	"And his eyes have all the seeming of a demons that is dreaming,"
	"And the lamp-light oer him streaming throws his shadow on the floor;"
	"And my soul from out that shadow that lies floating on the floor"
	"Shall be lifted - nevermore!";

/* FSL: taken from http://www.tero.co.uk/des/test.php */
unsigned char des3_encrypted[] = {
	0xb9, 0x30, 0x3f, 0xda, 0x4d, 0x82, 0xc5, 0xe7,
	0x27, 0x6b, 0x91, 0xf2, 0x84, 0x15, 0xf9, 0x74,
	0xd0, 0xe5, 0x5f, 0x4b, 0x95, 0xe1, 0x35, 0xac,
	0x5d, 0xbe, 0xa8, 0x83, 0x6d, 0xd5, 0x71, 0x0d,
	0xde, 0x86, 0x71, 0x61, 0x58, 0x23, 0xe8, 0x4a,
	0x3c, 0x99, 0x96, 0x98, 0x00, 0x1a, 0xe7, 0x42,
	0xd4, 0x72, 0xf2, 0xf9, 0xc5, 0x50, 0xa9, 0x7f,
	0xe7, 0xab, 0xee, 0x5a, 0x3b, 0x5b, 0x43, 0x67,
	0x59, 0x30, 0xec, 0x37, 0xe5, 0x2f, 0xc6, 0x5c,
	0x45, 0x02, 0x55, 0xe6, 0x75, 0x11, 0x7c, 0x9d,
	0x05, 0xa9, 0xe2, 0x97, 0xc8, 0xe3, 0x6a, 0x65,
	0x76, 0xce, 0xc3, 0x56, 0xea, 0xd4, 0x30, 0x0d,
	0xe4, 0x4c, 0x9a, 0xee, 0x9c, 0x5e, 0x48, 0xa3,
	0x85, 0x85, 0x5f, 0x59, 0x3b, 0x34, 0xee, 0x32,
	0x54, 0xd9, 0x91, 0x9f, 0x97, 0x7c, 0xce, 0x1d,
	0x9e, 0xe8, 0xb8, 0x86, 0xbc, 0x3a, 0x1e, 0x52,
	0xe1, 0x35, 0xe3, 0x92, 0xa2, 0xa3, 0xa6, 0xbd,
	0x3f, 0x66, 0x76, 0xf3, 0x47, 0x37, 0x61, 0xb8,
	0x12, 0x35, 0xa1, 0x7e, 0xb3, 0xd1, 0x79, 0xfd,
	0xb3, 0x69, 0xbc, 0x68, 0x4c, 0xd2, 0x24, 0xdc,
	0x04, 0xae, 0x82, 0x0e, 0x54, 0x4b, 0xcf, 0xa8,
	0x40, 0xde, 0x72, 0x4c, 0x23, 0xfc, 0xc9, 0x86,
	0x78, 0xc3, 0x50, 0xf0, 0x1d, 0x62, 0x61, 0x7d,
	0xbb, 0xc9, 0x83, 0x6f, 0x21, 0x3f, 0x6f, 0x2a,
	0xb1, 0x7e, 0x9f, 0x24, 0xee, 0xe7, 0x5b, 0x1d,
	0xca, 0xb5, 0xd0, 0x31, 0xdc, 0xc5, 0x95, 0xcf,
	0x8e, 0x5c, 0xf0, 0xbf, 0xb2, 0x15, 0xfb, 0x68,
	0x65, 0xc7, 0x5d, 0x77, 0x17, 0xc2, 0xf2, 0xd7,
	0x19, 0x29, 0xcb, 0x49, 0x62, 0xf3, 0xa1, 0xd0,
	0x18, 0xc1, 0xed, 0x77, 0x9f, 0x4d, 0x28, 0xce,
	0x66, 0x62, 0x7d, 0x56, 0xe7, 0x11, 0x5f, 0xf1,
	0x7a, 0xca, 0xf4, 0x2d, 0xeb, 0x6d, 0xca, 0x47,
	0xf3, 0x39, 0xfc, 0x2e, 0x81, 0x73, 0x70, 0xe1,
	0x4c, 0x37, 0xf8, 0xda, 0x91, 0x79, 0x7c, 0x4b,
	0xa0, 0xb4, 0x0e, 0x1e, 0xcf, 0x7b, 0xb1, 0xba,
	0x9c, 0xe8, 0x8b, 0xcf, 0xe0, 0x7b, 0x07, 0x13,
	0x17, 0x0d, 0x55, 0xbf, 0x3d, 0x24, 0x6e, 0xfe,
	0x61, 0x68, 0x41, 0xaf, 0x23, 0xf4, 0xd6, 0xd7,
	0xf3, 0xab, 0x78, 0x9c, 0xbf, 0xd3, 0xde, 0x37,
	0xe1, 0x0c, 0x02, 0x88, 0xa5, 0xd4, 0x43, 0xf9,
	0x08, 0x2d, 0x53, 0xce, 0x2e, 0x43, 0xdc, 0x25,
	0x26, 0xe1, 0x39, 0x17, 0x60, 0x10, 0x2d, 0x07,
	0xa5, 0x9e, 0x1c, 0xa6, 0x90, 0x46, 0x68, 0x14
};

/*
 * after encrypt/decrypt we must have original input:
 * symmetric crypto algorithm
 * 3DES in CBC mode
 */
void des3_main(void)
{
	/* Testing 3DES support */
	unsigned int length;

	/*
	 * 24 bytes key: "verynicepassword12345678"
	 * Note: parity is fixed inside DES crypto function
	 */
	unsigned char key[DES3_KEY_LENGTH] = {
		0x76, 0x65, 0x72, 0x79, 0x6e, 0x69, 0x63, 0x65,
		0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64,
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
	};

	/* initialization vector: 8 bytes: "mysecret" */
	unsigned char ive[DES3_BLOCK_LENGTH] = {
		0x6d, 0x79, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74
	};

	/* result holding the example max length array */
	unsigned char output[sizeof(des3_test) - 1];

	//for(i = 0; i < DES3_TEST_CASES; i++)
	//{
	length = strlen((const char *)des3_test);

	/* encrypt */
	if (encrypt3DES(key, des3_test, output, length, ive) != 0)
		printf("Error during encryption\n\r");

	/* compare encrypted info */
	if (compare_arrays(output, des3_encrypted, length) != 0)
		printf("DES encryption error\n\r");

	/* decrypt */
	if (decrypt3DES(key, output, output, length, ive) != 0)
		printf("Error during decryption\n\r");

	/* compare decrypted info */
	if (compare_arrays(output, des3_test, length) != 0)
		printf("3DES algorithm is NOT working as expected\n\r");
	//}

	printf("3DES test completed\n");
}

/*
 * encrypt3DES: 3DES encryption function
 *
 * Parameters:
 *	[in] key: key of 24 bytes
 *	[in] input_data: pointer to in data
 *	[out] output_data: pointer to out data
 *	[in] data_length: number of bytes of input data.
 *		 Must be divisible by 8 (3DES block)
 *	[in] init_vector: init_vector to use during xor
 * Return:
 *	0 if OK, otherwise error
 *
 */
unsigned char encrypt3DES(unsigned char *key,
						  unsigned char *input_data,
						  unsigned char *output_data,
						  unsigned short data_length,
						  unsigned char *init_vector)
{
	unsigned char i;
	unsigned short blocks;
	unsigned char temp_block[DES3_BLOCK_LENGTH];
	unsigned char temp_iv[DES3_BLOCK_LENGTH];

	/* validate NULL for key, input_data, output_data or init_vector */
	if ((key == NULL) || (input_data == NULL) || (output_data == NULL)
	    || (init_vector == NULL))
		return (unsigned char)-1;	/*wrong pointer */

	/* validate data length */
	if (data_length % DES3_BLOCK_LENGTH)
		return (unsigned char)-1;	/*wrong length */

	/*
	 * fix parity key if needed: keep in mind LSB of each
	 * byte is only used for parity
	 */
	for (i = 0; i < DES3_KEY_LENGTH; i++)
		key[i] = ((key[i] & 0xFE) | parityBits[key[i] >> 1]);

	/* FSL: optional -- check parity */
#if 1
	if ((cau_des_chk_parity(key) != NULL) ||
	    (cau_des_chk_parity(key + DES3_BLOCK_LENGTH) != NULL) ||
	    (cau_des_chk_parity(key + (2 * DES3_BLOCK_LENGTH)) != NULL))
		return (unsigned char)-1;	/*wrong parity */
#endif

	/* execute 3DES in CBC mode */
	/* http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation */

	/* get number of blocks */
	blocks = data_length / DES3_BLOCK_LENGTH;

	/* copy init vector to temp storage */
	memcpy((void *)temp_iv, (void *)init_vector, DES3_BLOCK_LENGTH);

	do {
		/* copy to temp storage */
		memcpy((void *)temp_block, (void *)input_data, DES3_BLOCK_LENGTH);

		/* xor for CBC */
		for (i = 0; i < DES3_BLOCK_LENGTH; i++)
			temp_block[i] ^= temp_iv[i];

		/* FSL: 1st DES round */
		cau_des_encrypt(temp_block, key, output_data);

		/* FSL: 2nd DES round */
		/* adjust key to 2nd part */
		cau_des_decrypt(output_data, key + DES3_BLOCK_LENGTH,
				output_data);

		/* FSL: 3rd DES round */
		/* adjust key to 3rd part */
		cau_des_encrypt(output_data,
						key + (2 * DES3_BLOCK_LENGTH),
						output_data);

		/* update init_vector for next 3DES round */
		memcpy((void *)temp_iv, (void *)output_data, DES3_BLOCK_LENGTH);

		/* adjust pointers for next 3DES round */
		input_data += DES3_BLOCK_LENGTH;
		output_data += DES3_BLOCK_LENGTH;

	} while (--blocks);

	return 0;		/* OK */
}

/*
 * decrypt3DES: 3DES decryption function
 *
 * Parameters:
 *	[in] key: key of 24 bytes
 *	[in] input_data: pointer to in data
 *	[out] output_data: pointer to out data
 *	[in] data_length: number of bytes of input data.
 *		 Must be divisible by 8 (3DES block)
 *	[in] init_vector: init_vector to use during xor
 * Return:
 *	0 if OK, otherwise error
 */
unsigned char decrypt3DES(unsigned char *key,
						  unsigned char *input_data,
						  unsigned char *output_data,
						  unsigned short data_length,
						  unsigned char *init_vector)
{
	unsigned char i;
	unsigned short blocks;
	unsigned char temp_block[DES3_BLOCK_LENGTH];
	unsigned char temp_iv[DES3_BLOCK_LENGTH];

	/* validate NULL for key, input_data, output_data or init_vector */
	if ((key == NULL) || (input_data == NULL) ||
		(output_data == NULL) || (init_vector == NULL))
		return (unsigned char)-1;	/*wrong pointer */

	/* validate data length */
	if (data_length % DES3_BLOCK_LENGTH)
		return (unsigned char)-1;	/*wrong length */

	/*
	 * fix parity key if needed: keep in mind LSB of each
	 * byte is only used for parity
	 */
	for (i = 0; i < DES3_KEY_LENGTH; i++)
		key[i] = ((key[i] & 0xFE) | parityBits[key[i] >> 1]);

	/* FSL: optional -- check parity */
#if 1
	if ((cau_des_chk_parity(key) != NULL) ||
	    (cau_des_chk_parity(key + DES3_BLOCK_LENGTH) != NULL) ||
	    (cau_des_chk_parity(key + (2 * DES3_BLOCK_LENGTH)) != NULL))
		return (unsigned char)-1;	/*wrong parity */
#endif

	/* execute 3DES in CBC mode */
	/* http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation */

	/* get number of blocks */
	blocks = data_length / DES3_BLOCK_LENGTH;

	/* copy init vector to temp storage */
	memcpy((void *)temp_iv, (void *)init_vector, DES3_BLOCK_LENGTH);

	do {
		/*copy to temp storage */
		memcpy((void *)temp_block, (void *)input_data, DES3_BLOCK_LENGTH);

		/* FSL: 1st DES round */
		cau_des_decrypt(input_data,
						key + (2 * DES3_BLOCK_LENGTH),
						output_data);

		/* FSL: 2nd DES round */
		/* adjust key to 2nd part */
		cau_des_encrypt(output_data,
						key + DES3_BLOCK_LENGTH,
						output_data);

		/* FSL: 3rd DES round */
		/* adjust key to 3rd part */
		cau_des_decrypt(output_data, key, output_data);

		/* xor for CBC */
		for (i = 0; i < DES3_BLOCK_LENGTH; i++)
			output_data[i] ^= temp_iv[i];

		/* update init_vector for next 3DES round */
		memcpy((void *)temp_iv, (void *)temp_block, DES3_BLOCK_LENGTH);

		/* adjust pointers for next 3DES round */
		input_data += DES3_BLOCK_LENGTH;
		output_data += DES3_BLOCK_LENGTH;

	} while (--blocks);

	return 0;		/* OK */
}
