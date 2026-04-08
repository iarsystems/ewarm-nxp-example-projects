/***********************************************************************
 * $Id: ea3250_ddr_setup.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: DDR setup and calibration code
 *
 * Description:
 *   This file contains DDR setup code used with the system startup
 *   code. This code is generic enough to work across multiple
 *   platforms, but may require tweaking for DDR geometry, timing, and
 *   size.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only  
 * which provides customers with programming information regarding the  
 * products. This software is supplied "AS IS" without any warranties.  
 * NXP Semiconductors assumes no responsibility or liability for the 
 * use of the software, conveys no license or title under any patent, 
 * copyright, or mask work right to the product. NXP Semiconductors 
 * reserves the right to make changes in the software without 
 * notification. NXP Semiconductors also make no representation or 
 * warranty that such application will be suitable for the specified 
 * use without further testing or modification. 
 **********************************************************************/

#include "lpc_arm922t_cp15_driver.h"
#include "ea3250_startup.h"
#include "ea3250_board.h"
#include "lpc32xx_emc.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_gpio_driver.h"

#define DEBUG_CAPTURE
#ifdef DEBUG_CAPTURE
typedef struct {
	UNS_32 dqs_pass[32];
} DDRTEST_ST;
DDRTEST_ST ddrst;
#endif

/***********************************************************************
 * Startup code private data
 **********************************************************************/

/* Structures and types used for memory tests */
typedef void (*pfrvi)(UNS_32 *);
typedef int (*pfrii)(UNS_32 *);
struct _memtests
{
	pfrvi testsetup;
	pfrii testcheck;
};

/* Calibration sensitivity table */
static const UNS_8 dqs2calsen[32] = {
	7, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
};

/***********************************************************************
 * Private functions
 **********************************************************************/

/***********************************************************************
 *
 * Function: *various*
 *
 * Purpose: Memory pattern and check tests
 *
 * Processing:
 *     A collection of small memory pattern generator and test functions
 *     that are used for DDR calibration.
 *
 * Parameters:
 *     base : Base address for test or check
 *
 * Outputs: None
 *
 * Returns:
 *     Check tests returns TRUE if passed, otherwise FALSE. Pattern
 *     generators return nothing.
 *
 * Notes: None
 *
 **********************************************************************/
static void walking0bitsetup(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		*base = ~(1 << i);
		base++;
	}
}
static int walking0bitcheck(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (*base != ~(1 << i))
		{
			return FALSE;
		}

		base++;
	}

	return TRUE;
}
static void walking1bitsetup(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		*base = (1 << i);
		base++;
	}
}
static int walking1bitcheck(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (*base != (1 << i))
		{
			return FALSE;
		}

		base++;
	}

	return TRUE;
}
static void invaddrsetup(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		*base = ~((UNS_32) base);
		base++;
	}
}
static int invaddrcheck(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (*base != ~((UNS_32) base))
		{
			return FALSE;
		}

		base++;
	}

	return TRUE;
}
static void noninvaddrsetup(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		*base = ((UNS_32) base);
		base++;
	}
}
static int noninvaddrcheck(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (*base != ((UNS_32) base))
		{
			return FALSE;
		}

		base++;
	}

	return TRUE;
}
static void aa55setup(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		*base = 0x55aa55aa;
		base++;
	}
}
static int aa55check(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (*base != 0x55aa55aa)
		{
			return FALSE;
		}

		base++;
	}

	return TRUE;
}
static void _55aasetup(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		*base = 0x55aa55aa;
		base++;
	}
}
static int _55aacheck(UNS_32 *base)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (*base != 0x55aa55aa)
		{
			return FALSE;
		}

		base++;
	}

	return TRUE;
}
static struct _memtests testvecs[] = {
	{walking0bitsetup, walking0bitcheck},
	{walking1bitsetup, walking1bitcheck},
	{invaddrsetup, invaddrcheck},
	{noninvaddrsetup, noninvaddrcheck},
	{aa55setup, aa55check},
	{_55aasetup, _55aacheck},
	{NULL, NULL},
};

/***********************************************************************
 *
 * Function: dqsin_ddr_mod
 *
 * Purpose: Adjusts non-calibrated DQSIN delay and cal sensitivity
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     ddl : New DQSIN delay value
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void dqsin_ddr_mod(UNS_32 ddl)
{
	UNS_32 tmp;

	/* Adjust calibration sensitivity with DQS delay */
	tmp = CLKPWR->clkpwr_sdramclk_ctrl & ~(CLKPWR_SDRCLK_SENS_FACT(7) |
		CLKPWR_SDRCLK_DQS_DLY(0x1F));
	CLKPWR->clkpwr_sdramclk_ctrl = tmp | CLKPWR_SDRCLK_DQS_DLY(ddl) |
		CLKPWR_SDRCLK_SENS_FACT(dqs2calsen[ddl]);
}

/***********************************************************************
 *
 * Function: ddr_memtst
 *
 * Purpose: Performs various quick integrity tests of the DDR memory
 *
 * Processing:
 *     This function simply performs a series of small memory tests
 *     on subsections of the passed memory range. The intended use of
 *     this function is to test DDR at different DQSIN delay values
 *     to find an optimal value for the current device/system's
 *     process, voltage, and temperature.
 *
 * Parameters:
 *     seed  : Test seed value
 *     start : Starting range for test (ie, start of DDR memory)
 *     size  : Size of test range in bytes (ie, 64MB)
 *
 * Outputs: None
 *
 * Returns: TRUE if integrity tests passed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
int ddr_memtst(UNS_32 seed, UNS_32 start, UNS_32 size)
{
	int testnum;
	UNS_32 inc, *base = (UNS_32 *) start;

	/* Offset test areas so we don't accidently get the results
	   from a previous test */
	base += (seed * 0x4000) + (seed * 4);
	inc = size / sizeof(UNS_32);
	inc = inc / 256; /* 256 test sections over test range */

	/* The DDR test is performed on a number of sections. Sections are
	   small areas of DDR memory seperated by untested areas. The
	   sections tested are spread out over the entire range of the
	   device. Testing the entire DDR would take a long time, so this
	   is a good alternative. */
	while ((UNS_32) base < ((start + size) - (32 * sizeof(UNS_32))))
	{
		/* Loop through each test */
		testnum = 0;
		while (testvecs[testnum].testsetup != NULL)
		{
			testvecs[testnum].testsetup(base);
			if (testvecs[testnum].testcheck(base) == FALSE)
			{
				/* Test failed */
				return FALSE;
			}

			testnum++;
		}

		base += inc;
	}

	/* Test passed */
	return TRUE;
}

/***********************************************************************
 *
 * Function: find_ddr_dqsin_delay
 *
 * Purpose: Determines optimal DQSIN delay for the device/system
 *
 * Processing:
 *     This function goes through a range of DQSIN delay values and
 *     performs memory tests with the DQSIN delay values. A working
 *     range of DQSIN delay values is generated and the optimal range
 *     is selected for the device/system.
 *
 * Parameters:
 *     start : Starting range for test (ie, start of DDR memory)
 *     size  : Size of test range in bytes (ie, 64MB)
 *
 * Outputs: None
 *
 * Returns: TRUE if integrity tests passed, otherwise FALSE
 *
 * Notes: Calibration must be disabled for this function.
 *
 **********************************************************************/
int find_ddr_dqsin_delay(UNS_32 start, UNS_32 size)
{
	UNS_32 tmp, dqsindly, dqsstart = 0xFF, dqsend = 0xFF;
	int ppass = 0, pass = FALSE;

	/* Most devices fail DDR in the DQSIN range of 2 or less to about
	   20+ or more, so the test will start with a DQSIN delay value of
	   1 up to 30 (max - 1) */
	dqsindly = 1;

	/* At this point, DDR is initialized and opeational with the
	   exception of the DQSIN delay value and calibrarion sensitivity.
	   While adjusting the DQSIN delay between a range of values,
	   perform spot checks on uncached DDR memory to determine if DDR
	   is working with specific DQSIN delay values. By mapping out a
	   range of working values, we can determine the optimal DQSIN
	   delay value and calibration sensitivity. */
	while (dqsindly < 31)
	{
		/* Modify the DQSIN delay and appropriate calibration sensitivity
		   before running the test */
		dqsin_ddr_mod(dqsindly);

		/* Perform some memory write-read checks of uncached DDR memory
		   to determine if the values seem to work */
		if (ddr_memtst(dqsindly, start, size) == TRUE)
		{
#ifdef DEBUG_CAPTURE
			ddrst.dqs_pass[dqsindly] = 1;
#endif
			/* Test passed */
			if (dqsstart == 0xFF)
			{
				dqsstart = dqsindly;
			}

			ppass = 1;
		}
		else
		{
#ifdef DEBUG_CAPTURE
			ddrst.dqs_pass[dqsindly] = 0;
#endif

			/* Test failed */
			if (ppass == 1)
			{
				dqsend = dqsindly - 1;
				pass = TRUE;
				ppass = 0;
			}
		}

		/* Try next value */
		dqsindly++;
	}

	/* If the test passed, the we can use the average of the min and 
	   max values to get an optimal DQSIN delay */
	if (pass == TRUE)
	{
		dqsindly = (dqsstart + dqsend) / 2;
	}
	else
	{
		/* A working value couldn't be found, just pick something safe
		   so the system doesn't become unstable */
		dqsindly = 0xF;
	}

	/* Set calibration sensitivity based on nominal delay */
	dqsin_ddr_mod(dqsindly);

	return pass;
}

/***********************************************************************
 *
 * Function: ea3250_ddr_adjust_timing
 *
 * Purpose: Sets up DDR timing to the current clock speed
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     clk          : DDR interface clock rate
 *     board_memcfg : Either DDR_MOBILE or DDR_STANDARD
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_ddr_adjust_timing(UNS_32 clk, UNS_32 board_memcfg)
{
	if (board_memcfg == DDR_MOBILE) 
	{
		/* Setup percharge command delay */
		EMC->emcdynamictrp = EMC_DYN_PRE_CMD_PER(1 + (clk / DDR_LP_TRP_DELAY));

		/* Setup Dynamic Memory Active to Precharge Command period */
		EMC->emcdynamictras = EMC_DYN_ACTPRE_CMD_PER(1 +
			(clk / DDR_LP_TRAS_DELAY));

		/* Dynamic Memory Self-refresh Exit Time */
		EMC->emcdynamictsrex = EMC_DYN_SELF_RFSH_EXIT(1 + (clk / DDR_LP_TSREX_TIME));

		/* Dynamic Memory Self-refresh Exit Time */
		EMC->emcdynamictwr = EMC_DYN_WR_RECOVERT_TIME(1 +
			(clk / DDR_LP_TWR_TIME));

		/* Dynamic Memory Active To Active Command Period */
		EMC->emcdynamictrc = EMC_DYN_ACT2CMD_PER(7);

		/* Dynamic Memory Auto-refresh Period */
		EMC->emcdynamictrfc = EMC_DYN_AUTOREFRESH_PER(1 +
			(clk / DDR_LP_TRFC_TIME));

		/* Dynamic Memory Active To Active Command Period */
		EMC->emcdynamictxsr = EMC_DYN_EXIT_SRFSH_TIME(1 +
			(clk / DDR_LP_TXSNR_TIME));

		/* Dynamic Memory Active Bank A to Active Bank B Time */
		EMC->emcdynamictrrd = EMC_DYN_BANKA2BANKB_LAT(1 +
			(clk / DDR_LP_TRRD_TIME));

		/* Dynamic Memory Load Mode Register To Active Command Time */
		EMC->emcdynamictmrd = EMC_DYN_LM2ACT_CMD_TIME(DDR_LP_TMRD_TIME - 1);

		/* Dynamic Memory Last Data In to Read Command Time */
		EMC->emcdynamictcdlr =
			EMC_DYN_LASTDIN_CMD_TIME(1 + (clk / DDR_LP_TCDLR_TIME));
	}
	else
	{
		/* Setup percharge command delay */
		EMC->emcdynamictrp = EMC_DYN_PRE_CMD_PER(1 + (clk / DDR_STD_TRP_DELAY));

		/* Setup Dynamic Memory Active to Precharge Command period */
		EMC->emcdynamictras = EMC_DYN_ACTPRE_CMD_PER(1 +
			(clk / DDR_STD_TRAS_DELAY));

		/* Dynamic Memory Self-refresh Exit Time */
		EMC->emcdynamictsrex = EMC_DYN_SELF_RFSH_EXIT(DDR_STD_TSREX_TIME);

		/* Dynamic Memory Self-refresh Exit Time */
		EMC->emcdynamictwr = EMC_DYN_WR_RECOVERT_TIME(1 +
			(clk / DDR_STD_TWR_TIME));

		/* Dynamic Memory Active To Active Command Period */
		EMC->emcdynamictrc = EMC_DYN_ACT2CMD_PER(7);

		/* Dynamic Memory Auto-refresh Period */
		EMC->emcdynamictrfc = EMC_DYN_AUTOREFRESH_PER(1 +
			(clk / DDR_STD_TRFC_TIME));

		/* Dynamic Memory Active To Active Command Period */
		EMC->emcdynamictxsr = EMC_DYN_EXIT_SRFSH_TIME(1 +
			(clk / DDR_STD_TXSNR_TIME));

		/* Dynamic Memory Active Bank A to Active Bank B Time */
		EMC->emcdynamictrrd = EMC_DYN_BANKA2BANKB_LAT(1 +
			(clk / DDR_STD_TRRD_TIME));

		/* Dynamic Memory Load Mode Register To Active Command Time */
		EMC->emcdynamictmrd = EMC_DYN_LM2ACT_CMD_TIME(1 +
			(clk / DDR_STD_TMRD_TIME));

		/* Dynamic Memory Last Data In to Read Command Time */
		EMC->emcdynamictcdlr =
			EMC_DYN_LASTDIN_CMD_TIME(1 + (clk / DDR_STD_TCDLR_TIME));
	}

	/* Note : Refresh timing needs to be performed outside this
	   function after this function exits. Something similar to the
	   following can be used:
           EMC->emcdynamicrefresh =
               EMC_DYN_REFRESH_IVAL(clk / DDR_RFSH_INTERVAL);
    */
}

/***********************************************************************
 *
 * Function: ea3250_ddr_if_init
 *
 * Purpose: Sets up DDR interface and initial calibration
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     board_memcfg : Either DDR_MOBILE or DDR_STANDARD
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_ddr_if_init(UNS_32 board_memcfg)
{
	/* Reset DDR interface */
	CLKPWR->clkpwr_sdramclk_ctrl &= ~CLKPWR_SDRCLK_USE_DDR;
	CLKPWR->clkpwr_sdramclk_ctrl |= CLKPWR_SDRCLK_SW_DDR_RESET;
	
	/* Enables DDR clocks, (15*0.25nS) HCLK delay
	   Tests of HCLK delay have shown that voltage and frequency are
	   not driving factors for it's timing. Over testing, a working
	   value of 3 to 27 (average) appears to work and not effect other
	   operation. For this value, a hardcoded value of 15 is used. */
    CLKPWR->clkpwr_sdramclk_ctrl = CLKPWR_SDRCLK_USE_DDR;
    CLKPWR->clkpwr_sdramclk_ctrl = CLKPWR_SDRCLK_USE_DDR |
		CLKPWR_SDRCLK_HCLK_DLY(15);

	/* Enable calibration logic with low calibration sensitivity and
	   a guessed first DQSIN delay value */
	CLKPWR->clkpwr_sdramclk_ctrl |= CLKPWR_SDRCLK_USE_CAL |
		CLKPWR_SDRCLK_SENS_FACT(7) | CLKPWR_SDRCLK_DQS_DLY(0xF);

	/* Force a calibration cycle now and wait at least 1 PCLK for the
	   results */
	CLKPWR->clkpwr_sdramclk_ctrl |= CLKPWR_SDRCLK_DO_CAL;
	timer_wait_ms(TIMER_CNTR0, 1);
	CLKPWR->clkpwr_sdramclk_ctrl &= ~CLKPWR_SDRCLK_DO_CAL;
	timer_wait_ms(TIMER_CNTR0, 1);

	/* Save the hardware measured value of the ring oscillator and
	   computed delay */
	CLKPWR->clkpwr_ddr_lap_nom = CLKPWR->clkpwr_ddr_lap_count;

	/* Enable automatic RTC tick calibration */
	CLKPWR->clkpwr_sdramclk_ctrl |= CLKPWR_SDRCLK_CAL_ON_RTC;

	/* Keep calibration off for now until the DQS tests are done */
	CLKPWR->clkpwr_sdramclk_ctrl &= ~CLKPWR_SDRCLK_USE_CAL;

	/* Enable normal power mode, little-endian mode, start DDR I/F
	   clocks, disable clock enables and self-refresh mode */
	EMC->emccontrol = EMC_DYN_SDRAM_CTRL_EN;
	EMC->emcconfig = 0;

	/* DDR type specific configuration */
	if (board_memcfg == DDR_MOBILE) 
	{
		EMC->emcdynamicconfig0 = DDR_STD_SY_CFG0NP;

		/* Setup CAS and RAS latencies (assumes 133MHz clock), 3 CAS
		   cycles, 2 RAS cycles */
		EMC->emcdynamicrascas0 = DDR_LP_SY_RASCAS_SETUP;

		/* Setup DDR read strategy */
		EMC->emcdynamicreadconfig = (EMC_SDR_CLK_NODLY_CMD_DEL |
		    EMC_SDR_READCAP_POS_POL | EMC_DDR_CLK_NODLY_CMD_DEL);

		/* Mobile DDR gets high slew rates */
		CLKPWR->clkpwr_sdramclk_ctrl &= ~(CLKPWR_SDRCLK_FASTSLEW_CLK |
			CLKPWR_SDRCLK_FASTSLEW | CLKPWR_SDRCLK_FASTSLEW_DAT);

	}
	else
	{
		EMC->emcdynamicconfig0 = DDR_STD_SY_CFG0;

		/* Setup CAS and RAS latencies (assumes 133MHz clock), 2.5 CAS
		   cycles, 2 RAS cycles */
		EMC->emcdynamicrascas0 = DDR_LP_SY_RASCAS_SETUP;

		/* Setup DDR read strategy */
		EMC->emcdynamicreadconfig = (EMC_SDR_CLK_NODLY_CMD_DEL |
		    EMC_SDR_READCAP_POS_POL | EMC_DDR_CLK_NODLY_CMD_DEL |
			EMC_DDR_READCAP_POS_POL);

		/* Standard DDR gets low slew rates */
		CLKPWR->clkpwr_sdramclk_ctrl |= (CLKPWR_SDRCLK_FASTSLEW_CLK |
			CLKPWR_SDRCLK_FASTSLEW | CLKPWR_SDRCLK_FASTSLEW_DAT);
	}
}

/***********************************************************************
 *
 * Function: ea3250_ddr_init
 *
 * Purpose: Setup and calibrate DDR
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     clk          : DDR interface clock rate
 *     board_memcfg : Either DDR_MOBILE or DDR_STANDARD
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void ea3250_ddr_init(UNS_32 clk, UNS_32 board_memcfg)
{
	volatile UNS_32 tmp, tmp32;
	volatile UNS_16 tmp16;

	/* 16-bit interface for DDR memory */
	EMC->emcstatic_regs[0].emcstaticconfig =
		EMC->emcstatic_regs[1].emcstaticconfig = EXTMEM_CFG16;

	/* Setup DDR interface */
	ea3250_ddr_if_init(board_memcfg);

	/* Setup interface timing */
	ea3250_ddr_adjust_timing(clk, board_memcfg);

	/* Issue NOP with Clock enables active */
	tmp = (EMC_DYN_CLK_ALWAYS_ON | EMC_DYN_CLKEN_ALWAYS_ON);
	EMC->emcdynamiccontrol = (tmp | EMC_DYN_NOP_MODE);
	timer_wait_ms(TIMER_CNTR0, 100);

	/* Issue a precharge all command */
	EMC->emcdynamiccontrol = (tmp | EMC_DYN_PALL_MODE);

	/* Fast dynamic refresh */
	EMC->emcdynamicrefresh = EMC_DYN_REFRESH_IVAL(32);
	timer_wait_ms(TIMER_CNTR0, 100);

	if (board_memcfg == DDR_MOBILE) 
	{
		/* Compute nominal refresh period of DDR for current clock */
		EMC->emcdynamicrefresh =
			EMC_DYN_REFRESH_IVAL(clk / DDR_LP_RFSH_INTERVAL);

		/* Issue load mode command and issue extended mode word */
		EMC->emcdynamiccontrol = (tmp | EMC_DYN_CMD_MODE);
		tmp16 = * (volatile UNS_16 *) (EMC_DYCS0_BASE + DDR_LP_MODE_EXTENDED);

		/* Issue load mode command and normal mode word */
		EMC->emcdynamiccontrol = (tmp | EMC_DYN_CMD_MODE);
		tmp16 = * (volatile UNS_16 *) (EMC_DYCS0_BASE + DDR_LP_MODE_NORMAL);
	}
	else
	{
		/* Compute nominal refresh period of DDR for current clock */
		EMC->emcdynamicrefresh =
			EMC_DYN_REFRESH_IVAL(clk / DDR_STD_RFSH_INTERVAL);

		/* Issue load mode command and issue extended mode word */
		EMC->emcdynamiccontrol = (tmp | EMC_DYN_CMD_MODE);
		tmp16 = * (volatile UNS_16 *) (EMC_DYCS0_BASE + DDR_STD_MODE_EXTENDED);

		/* Issue load mode command and normal mode word */
		EMC->emcdynamiccontrol = (tmp | EMC_DYN_CMD_MODE);
		tmp16 = * (volatile UNS_16 *) (EMC_DYCS0_BASE + DDR_STD_MODE_NORMAL);
	}

	/* Normal DDR mode */
	EMC->emcdynamiccontrol = EMC_DYN_NORMAL_MODE;

	/* Manual calibration */
	CLKPWR->clkpwr_sdramclk_ctrl |= CLKPWR_SDRCLK_DO_CAL;
	CLKPWR->clkpwr_sdramclk_ctrl &= ~CLKPWR_SDRCLK_DO_CAL;
	timer_wait_ms(TIMER_CNTR0, 1);

	/* Find optimal DQSin delay and calibration sensitivity, tested
	   over DRAM bank 0 with a size of 32MBytes */
	find_ddr_dqsin_delay(EMC_DYCS0_BASE, (64 * 1024 * 1024));

	/* Enable automatic calibration */                     
	CLKPWR->clkpwr_sdramclk_ctrl |= CLKPWR_SDRCLK_USE_CAL;
}
