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

#ifndef __USB_REGULATOR__
#define __USB_REGULATOR__

#include "common.h"

/* Defines */
#define USB_REG_SET_ENABLE			FLAG_SET(SIM_SOPT1CFG_URWE_SHIFT,SIM_SOPT1CFG); \
									FLAG_SET(SIM_SOPT1_USBREGEN_SHIFT,SIM_SOPT1)

#define USB_REG_CLEAR_ENABLE		FLAG_SET(SIM_SOPT1CFG_URWE_SHIFT,SIM_SOPT1CFG); \
									FLAG_CLR(SIM_SOPT1_USBREGEN_SHIFT,SIM_SOPT1)

/* Regulator in standby during Stop, VLPS, LLS or VLLS */
#define USB_REG_SET_STDBY_STOP		FLAG_SET(SIM_SOPT1CFG_USSWE_SHIFT,SIM_SOPT1CFG); \
                                  FLAG_SET(SIM_SOPT1_USBSSTBY_SHIFT,SIM_SOPT1)
#define USB_REG_CLEAR_STDBY_STOP	FLAG_SET(SIM_SOPT1CFG_USSWE_SHIFT,SIM_SOPT1CFG); \
                                  FLAG_CLR(SIM_SOPT1_USBSSTBY_SHIFT,SIM_SOPT1)

/* Regulator in standby during VLPR or VLPW */
#define USB_REG_SET_STDBY_VLPx		FLAG_SET(SIM_SOPT1CFG_UVSWE_SHIFT,SIM_SOPT1CFG); \
									FLAG_SET(SIM_SOPT1_USBVSTBY_SHIFT,SIM_SOPT1)
#define USB_REG_CLEAR_STDBY_VLPx	FLAG_SET(SIM_SOPT1CFG_UVSWE_SHIFT,SIM_SOPT1CFG); \
									FLAG_CLR(SIM_SOPT1_USBVSTBY_SHIFT,SIM_SOPT1)

/* Prototypes */
void USBReg_Testing(void);

#endif		/* __USB_REGULATOR__ */
