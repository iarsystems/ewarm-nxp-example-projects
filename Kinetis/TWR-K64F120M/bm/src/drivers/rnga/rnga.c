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
#include "rnga.h"

/*
 * \brief	init random number generation module
 * \author
 * \param	none
 * \return	the 32-bit random number
 * \todo
 * \warning
 */
void rnga_init (unsigned int seed)
{
	RNG_ER = seed;

	/*FSL: generate randomness*/
	RNG_CR = RNG_CR_GO_MASK;
}

/*
 * \brief	Wait for the generation of a random number
 * \author
 * \param	none
 * \return	the 32-bit random number
 * \todo
 * \warning
 */
unsigned int rnga_getnumber (void)
{
	/* FSL: check for FIFO level */
	while(!(RNG_SR & RNG_SR_OREG_LVL_MASK))
		;

	/* FSL: get random number */
	return RNG_OR;
}

/*
 * \brief	Stop randomness
 * \author
 * \param	none
 * \return	the 32-bit random number
 * \todo
 * \warning
 */
void rnga_stop (void)
{
	RNG_CR &= ~RNG_CR_GO_MASK;
}
