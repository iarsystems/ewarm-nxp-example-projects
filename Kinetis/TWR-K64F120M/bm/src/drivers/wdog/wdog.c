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
#include "wdog.h"

/* Watchdog timer disable routine */
void wdog_disable(void)
{
	/* First unlock the watchdog so that we can write to registers */
	wdog_unlock();

	/* Clear the WDOGEN bit to disable the watchdog */
	WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

/*
 * Watchdog timer unlock routine. Writing 0xC520 followed by 0xD928
 * will unlock the write once registers in the WDOG so they are writable
 * within the WCT period.
 *
 * Parameters:
 * none
 */
void wdog_unlock(void)
{
	/*
	 * NOTE: DO NOT SINGLE STEP THROUGH THIS FUNCTION!!!
	 * There are timing requirements for the execution of the unlock. If
	 * you single step through the code you will cause the CPU to reset.
	 *
	 * This sequence must execute within 20 clock cycles, so disable
	 * interrupts will keep the code atomic and ensure the timing.
	 */
	DisableInterrupts;

	/* Write 0xC520 to the unlock register */
	WDOG_UNLOCK = 0xC520;

	/* Followed by 0xD928 to complete the unlock */
	WDOG_UNLOCK = 0xD928;

	/* Re-enable interrupts now that we are done */
	EnableInterrupts;
}
