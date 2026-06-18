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
 * File:	start.c
 * Purpose:	Kinetis start up routines.
 *
 * Notes:
 */

#include "start.h"
#include "common.h"
#include "wdog.h"
#include "rcm.h"
#include "sysinit.h"

/*
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then
 * branches to the main process.
 */
void start(void)
{
	/* Disable the watchdog timer */
	wdog_disable();

	/* Copy any vector or data sections that need to be in RAM */
	common_startup();

	/* Perform processor initialization */
	sysinit();

	printf("\n\n");

	/* Determine the last cause(s) of reset */
	outSRS();

	/* Determine specific Kinetis device and revision */
	cpu_identify();

	/* Jump to main process */
	main();

	/* No actions to perform after this so wait forever */
	while (1) ;
}

/*
 * \brief   Kinetis Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 * - Kinetis family
 * - package
 * - die revision
 * - P-flash size
 * - Ram size
 */
void cpu_identify(void)
{
	char tmpbuf[64] = {0};
	char serID[4] = {'K', 'L', 'W', 'V'};
	int pinID[16] = {
					 0, 0, 32, 0,
					 48, 64, 80, 121,
					 100, 121, 144, -1,	// -1 customer WCSP
					 0, 0, 256, 0
					};
	int flash[16] = {
					 0, 0, 0, 32,
					 0, 64, 0, 128,
					 0, 256, 0, 512,
					 0, 1024, 0, 1024
					};
	int eerom[16] = {
					 16384, 8192, 4096, 2048,
					 1024, 512, 256, 128,
					 64, 32, 0, 0,
					 0, 0, 0, 0
	};
	int ramsize[16] = {
					   0, 8, 0, 16,
					   24, 32, 48, 64,
					   96, 128, 0, 256,
					   0, 0, 0, 0
	};
	int series = 0xFF, pin = 0xFF, die = 0xFF;
	int idx = 0;
	int tmp[4] = {0};

	/* Determine the Kinetis family */
	series = (SIM_SDID >> SIM_SDID_SERIESID_SHIFT) & 0xF;
	pin = (SIM_SDID >> SIM_SDID_PINID_SHIFT) & 0xF;
	die = (SIM_SDID >> SIM_SDID_DIEID_SHIFT) & 0x1F;

	tmp[0] = (SIM_SDID >> SIM_SDID_FAMILYID_SHIFT) & 0xF;
	tmp[1] = (SIM_SDID >> SIM_SDID_SUBFAMID_SHIFT) & 0xF;
	tmp[2] = (SIM_SDID >> SIM_SDID_REVID_SHIFT) & 0xF;

	if (die == 6) {
		tmp[1] = tmp[1] + 2;
	}
	sprintf(tmpbuf, "\nKinetis %c%d%d - %d pin  Silicon rev %d",
			serID[series],
			tmp[0],
			tmp[1],
			pinID[pin],
			tmp[2]);
	printf("%s - Build date: %s %s\n", tmpbuf, __DATE__, __TIME__);

	/* Determine the flash revision */
	flash_identify();

	idx = sprintf(tmpbuf, "\n - P-Flash: %d KBytes\n",
				  flash[(SIM_FCFG1 >> SIM_FCFG1_PFSIZE_SHIFT) & 0xF]);

	if ((SIM_FCFG1 >> SIM_FCFG1_NVMSIZE_SHIFT) & 0xF) {
	  idx += sprintf(&tmpbuf[idx], " - Flexmem: %d KBytes\n",
					   flash[(SIM_FCFG1 >> SIM_FCFG1_NVMSIZE_SHIFT) & 0xF]);
	}

	if ((SIM_FCFG1 >> SIM_FCFG1_EESIZE_SHIFT) & 0xF) {
		int romsz = eerom[(SIM_FCFG1 >> SIM_FCFG1_EESIZE_SHIFT) & 0xF];

		if (romsz > 0) {
			romsz /= 1024;
			idx += sprintf(&tmpbuf[idx], " - EEROM: %d KBytes\n",
						   romsz);
		}
	}

	sprintf(&tmpbuf[idx], " - RAM: %d KBytes\n\n",
			ramsize[(SIM_SOPT1 >> SIM_SOPT1_RAMSIZE_SHIFT) & 0xF]);
	printf("%s\n", tmpbuf);
}

/*
 * \brief   flash Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific flash parameters and flash version ID for
 * the current device. These parameters are obtained using a special
 * flash command call "read resource." The first four bytes returned
 * are the flash parameter revision, and the second four bytes are
 * the flash version ID.
 */
void flash_identify(void)
{
	uint8_t tmp[11] = {0};

	/* Get the flash parameter version and flash version ID */

	/*
	 * Write the flash FCCOB registers with the values for a
	 * read resource command
	 */
	FTFE_FCCOB0 = 0x03;
	FTFE_FCCOB1 = 0x00;
	FTFE_FCCOB2 = 0x00;
	FTFE_FCCOB3 = 0x08;
	FTFE_FCCOB4 = 0x01;

	/* All required FCCOBx registers are written, so launch the command */
	FTFE_FSTAT = FTFE_FSTAT_CCIF_MASK;

	/* Wait for the command to complete */
	while (!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK)) ;

	tmp[3] = FTFE_FCCOB4;
	tmp[4] = FTFE_FCCOB5;
	tmp[5] = FTFE_FCCOB6;
	tmp[6] = FTFE_FCCOB7;
	tmp[7] = FTFE_FCCOB8;
	tmp[8] = FTFE_FCCOB9;
	tmp[9] = FTFE_FCCOBA;
	tmp[10] = FTFE_FCCOBB;

	printf("Flash parameter version %d.%d.%d.%d\n",
		   tmp[3], tmp[4], tmp[5], tmp[6]);
	printf("Flash version ID %d.%d.%d.%d\n",
		   tmp[7], tmp[8], tmp[9], tmp[10]);
}
