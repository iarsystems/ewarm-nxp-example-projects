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

#ifndef __PMC_H__
#define __PMC_H__

/* LVD initialization code */
/*
   LVWV if LVD high range selected
   2.62
   2.72
   2.82
   2.92
   LVDW if LVD low range selected
   1.74
   1.84
   1.94
   2.04
*/
/*
 * 00 Low trip point selected (V LVD = V LVDL )
 * 01 High trip point selected (V LVD = V LVDH )
 */
#define VLVDH		1
#define VLVDL		0
#define LVDRESETEN	1
#define LVDRESETDIS	0
#define LVDINTEN	1
#define LVDINTDIS	0
/*
 * 00 Low trip point selected (V LVW = V LVW1 )
 * 01 Mid 1 trip point selected (V LVW = V LVW2 )
 * 10 Mid 2 trip point selected (V LVW = V LVW3 )
 * 11 High trip point selected (V LVW = V LVW4 )
 */
#define VLVW4		3
#define VLVW3		2
#define VLVW2		1
#define VLVW1		0
#define LVWINTEN	1
#define LVWINTDIS	0

/* function prototypes */
/* LVD initialization code */
void LVD_Initalize(unsigned char lvd_select,
				   unsigned char lvd_reset_enable,
				   unsigned char lvd_int_enable,
				   unsigned char lvw_select,
				   unsigned char lvw_int_enable);
void LVD_Init(void);
/*
 * LVD interrupt service routine - If entering into this interrupt
 * from stop you are in pbe mode.
 */
void pmc_lvd_isr(void);

#endif		/* __PMC_H__ */
