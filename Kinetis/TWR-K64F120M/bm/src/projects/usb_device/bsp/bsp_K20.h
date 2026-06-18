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

#ifndef __bsp_K20_H
#define __bsp_K20_H

#include "common.h"
#include "Settings.h"

enum usb_clock {
	MCGPLL0,
	MCGFLL,
	CLKIN,
	IRC48M
};

/* Select Clock source */
#define USB_CLOCK		IRC48M

/* USB Fractional Divider value for 48MHz input */
/* USB Clock = PLL x (FRAC +1) / (DIV+1) */
/* USB Clock = 120MHz x (1+1) / (4+1) = 48 MHz */
#define USB_FRAC		1
#define USB_DIV			SIM_CLKDIV2_USBDIV(4)

void vfnInitUSBClock(uint8 u8ClkOption);

#endif		/* __bsp_K20_H */
