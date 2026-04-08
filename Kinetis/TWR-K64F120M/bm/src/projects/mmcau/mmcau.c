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
 * File:		cau_test.c
 * Purpose:		Test CAU and mmCAU library
 */

#include "common.h"
#include "cau_api.h"		/*CAU and mmCAU header file */

/* crypto */
#include "aes.h"
#include "des.h"
#include "des3.h"

/* hash */
#include "md5.h"
#include "sha1.h"
#include "sha256.h"

void main(void)
{
	unsigned int dummy_var[SHA256_RESULT_LENGTH / 4];

#if NXP_MMCAU
	printf("\nTesting mmCAU\n");
#elif NXP_CAU
	printf("\nTesting CAU\n");
#else
#	error "Declare CAU or mmCAU coprocessor"
#endif

	printf("\nIf no error message is shown, then everything's OK\n");
	printf("\nRunning crypto in HW\n");

	/* FSL: starting crypto and hash tests */

	/* FSL: AES128, AES192, AES256 test */
	aes_main();

	/* FSL: DES test */
	des_main();

	/* FSL: 3DES test */
	des3_main();

	/* FSL: MD5 test */
	md5_main();

	/* FSL: SHA1 test */
	sha1_main();

	/* FSL: SHA256 test */
	/* Only run if correct version of CAU */
	if (!cau_sha256_initialize_output(dummy_var))
		sha256_main();
	else
		printf("No support for SHA256\n");

	printf("\nTests done!\n");

	while (1) ;
}
