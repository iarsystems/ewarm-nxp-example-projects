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

#ifndef _DES_H_
#define _DES_H_

#define DES_BLOCK_LENGTH			8
#define DES_KEY_LENGTH				8

//#define DES_TEST_CASES			(sizeof(des_test)/sizeof(des_test[0]))

/*
 * after encrypt/decrypt we must have original input:
 * symmetric crypto algorithm DES in CBC mode
 */
void des_main(void);

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
						 unsigned char *init_vector);

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
						 unsigned char *init_vector);

#endif
