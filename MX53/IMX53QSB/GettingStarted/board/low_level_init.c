/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : low_level_init.c
 *    Description : low_level_init for iMX535 QSB
 *                  
 *                  
 *    History :
 *    1. Date        : 1.03.2011
 *       Author      : Stanimir Bonev
 *       Description : initial vestion
 *
 *    $Revision: 5880 $
 **************************************************************************/

/** include files **/
#include <NXP/iomcimx535.h>
#include "arm_comm.h"

/** local definitions **/
extern __arm void __vector(void);

/** default settings **/
#define ROM_SI_REV()	(*(volatile const unsigned int *)0x48)

#define CHIP_REV_1_0            0x10
#define CHIP_REV_1_1            0x11
#define CHIP_REV_2_0            0x20
#define CHIP_REV_2_5						0x25
#define CHIP_REV_3_0            0x30

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/*************************************************************************
 * Function Name: setup_dpll1_800
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init DPLL1 - 800 MHz  
 *
 *************************************************************************/
static void setup_dpll1_800 (void)
{
	DPLLC1_DP_CTL = 0x00001232; /* Set DPLL ON (set UPEN bit): BRMO=1 */
	DPLLC1_DP_CONFIG = 2; 			/* Enable auto-restart AREN bit */

	DPLLC1_DP_OP = ((8 << 4) + ((1 - 1)  << 0));
	DPLLC1_DP_HFS_OP = ((8 << 4) + ((1 - 1)  << 0));
	
	DPLLC1_DP_MFD = (3 - 1);
	DPLLC1_DP_HFS_MFD = (3 - 1);

	DPLLC1_DP_MFN = 1;
	DPLLC1_DP_HFS_MFN = 1;

	DPLLC1_DP_CTL = 0x00001232;

	while(!DPLLC1_DP_CTL_bit.LRF);
}

/*************************************************************************
 * Function Name: setup_dpll3_216
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init DPLL3 - 216 MHz  
 *
 *************************************************************************/
static void setup_dpll3_216 (void)
{
	DPLLC3_DP_CTL = 0x00001232; /* Set DPLL ON (set UPEN bit): BRMO=1 */
	DPLLC3_DP_CONFIG = 2; 			/* Enable auto-restart AREN bit */

	DPLLC3_DP_OP = ((6 << 4) + ((3 - 1)  << 0));
	DPLLC3_DP_HFS_OP = ((6 << 4) + ((3 - 1)  << 0));
	
	DPLLC3_DP_MFD = (4 - 1);
	DPLLC3_DP_HFS_MFD = (4 - 1);

	DPLLC3_DP_MFN = 3;
	DPLLC3_DP_HFS_MFN = 3;

	DPLLC1_DP_CTL = 0x00001232;

	while(!DPLLC3_DP_CTL_bit.LRF);
}

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: None
 *
 * Return: None
 *
 * Description: Low level initialization  
 *
 *************************************************************************/
int __low_level_init (void)
{
Int32U reg;
	/* init_l2cc */
	/* explicitly disable L2 cache */
	reg = __MRC(15, 0, 1, 0, 1);
	reg &= ~(1ul << 2);
	__MCR(15, 0, reg, 1, 0, 0);
	/* reconfigure L2 cache aux control reg */
	reg = 0xC4ul
			| (1ul << 24)		/* disable write allocate delay */
			| (1ul << 23)		/* disable write allocate combine */
			|	(1ul << 22)		/* disable write allocate */
			;
	
	if(CHIP_REV_2_0 > ROM_SI_REV())
	{
		reg |= (1ul << 25);		/*  disable write combine */
	}
	__MCR(15, 1, reg, 9, 0, 2);
	
	/* init_aips */
	/*
	 * Set all MPROTx to be non-bufferable, trusted for R/W,
	 * not forced to user-mode.
	 */
	AIPSTZ1_MPR1 = 0x77777777;
	AIPSTZ1_MPR2 = 0x77777777;
	AIPSTZ2_MPR1 = 0x77777777;
	AIPSTZ2_MPR2 = 0x77777777;
	/*
	 * Clear the on and off peripheral modules Supervisor Protect bit
	 * for SDMA to access them. Did not change the AIPS control registers
	 * (offset 0x20) access type
	 */

	/* init_clock */
	/* Switch ARM to step clock */
	CCM_CCSR = 4;
	setup_dpll1_800();			// 800 MHz
	setup_dpll3_216();			// 216 MHz
	/* Set the platform clock dividers */
	ARMPC_ICGC = 0x00000725;
	CCM_CACRR = 0;
	/* Switch ARM back to PLL 1 */
	CCM_CCSR = 0;

	/* Restore the default values in the Gate registers */
	CCM_CCGR0 = 0xFFFFFFFF;
	CCM_CCGR1 = 0xFFFFFFFF;
	CCM_CCGR2 = 0xFFFFFFFF;
	CCM_CCGR3 = 0xFFFFFFFF;
	CCM_CCGR4 = 0xFFFFFFFF;
	CCM_CCGR5 = 0xFFFFFFFF;
	CCM_CCGR6 = 0xFFFFFFFF;
	CCM_CCGR7 = 0xFFFFFFFF;

	CCM_CSCDR1_bit.uart_clk_podf = 1-1;
	CCM_CSCDR1_bit.uart_clk_pred = 5-1;

	/* make sure divider effective */
	while(CCM_CDHIPR);
	CCM_CCDR = 0;
	/* for cko - for ARM div by 8 */
	CCM_CCOSR = 0x000A00F0;

	return 1;
}
