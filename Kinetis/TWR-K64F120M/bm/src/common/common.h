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
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/* Debug prints ON (#define) or OFF (#undef) */
#define DEBUG
#define DEBUG_PRINT

/* Include the generic CPU header file */
#include "arm_cm4.h"

/* Include the platform specific header file */
#if (defined(TOWER))
#	include "tower.h"
#else
#	error "No valid platform defined"
#endif

/* Include the cpu specific header file */
#if(defined(MCU_MK64FN1M0VCM12))
#	if(defined(DRYICE))
#		include "MK63F12WS.h"
#	else
#		include "MK64F12.h"
#	endif
#else
#	error "No valid CPU defined"
#endif

/* Include any toolchain specfic header files */
#if (defined(__MWERKS__))
#	include "mwerks.h"
#elif (defined(__DCC__))
#	include "build/wrs/diab.h"
#elif (defined(__ghs__))
#	include "build/ghs/ghs.h"
#elif (defined(__GNUC__))
#	include "build/gnu/gnu.h"
#elif (defined(IAR))
#else
#	warning "No toolchain specific header included"
#endif

/* Include common utilities */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"

#if (defined(IAR))
#	include "intrinsics.h"
#endif

#endif		/* _COMMON_H_ */
