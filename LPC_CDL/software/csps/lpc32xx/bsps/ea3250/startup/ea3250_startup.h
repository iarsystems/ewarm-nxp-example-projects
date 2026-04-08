/***********************************************************************
 * $Id: ea3250_startup.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Embedded Artists LPC3250 board startup code
 *
 * Description:
 *     This package contains the startup functions that initialize the
 *     Embedded Artists LPC3250 board into a known state.
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

#ifndef EA3250_STARTUP_H
#define EA3250_STARTUP_H

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************
 * DDR/SDRAM Mobile/standard selection constants
 *
 * These defines are an Or'ed value of (GPI11 >> 27) | (GPI05 >> 5)
 *
 **********************************************************************/
#define DDR_STD    0x0
#define SDR_STD    0x1
#define DDR_MOBILE 0x2
#define SDR_MOBILE 0x3


/***********************************************************************
 * DDR SDRAM initialization constants
 *
 * DDR memory type  : Micron MT46V32M16BN-6
 * Size             : 512Mbits (64MBytes)
 * Geometry         : 8MBits x 16 bits x 4 banks (11 cols, 13 rows)
 * Refresh interval : 7.8uS
 * CAS latency      : 2 clocks (at 133MHz)
 * Minimum RAS      : 40nS (6 clocks) (5 clocks @ 104MHz is ok)
 **********************************************************************/

/* DDR refresh interval (1 / tREFI) */
#define DDR_STD_RFSH_INTERVAL 128205
#define DDR_LP_RFSH_INTERVAL 128205

/* Configuration ID for the SDRAM Controller */
#define DDR_STD_SY_CFG0 ((0x31 << 7) | EMC_DYN_DEV_LP_DDR_SDRAM)
#define DDR_STD_SY_CFG0NP ((0x31 << 7) | EMC_DYN_DEV_LP_DDR_SDRAM) /* Normal power */

/* RAS and CAS latencies setup (133MHz max), RAS is hardcoded for a
   >=40nS period @104MHz, but can be better adjusted based on the
   clock speed fetched at runtime */
#define DDR_STD_SY_RASCAS_SETUP (EMC_SET_CAS_IN_HALF_CYCLES(5) | \
	EMC_SET_RAS_IN_CYCLES(2))
#define DDR_LP_SY_RASCAS_SETUP (EMC_SET_CAS_IN_HALF_CYCLES(6) | \
	EMC_SET_RAS_IN_CYCLES(2))

/* (tRP) Precharge command period (1 / t) */
#define DDR_STD_TRP_DELAY 66666666
#define DDR_LP_TRP_DELAY 44444444 // 22.5nS

/* (tRAS) Dynamic Memory Active to Precharge Command period (1 / t) */
#define DDR_STD_TRAS_DELAY 66666666
#define DDR_LP_TRAS_DELAY 22222222 // 45nS

/* (tSREX) Dynamic Memory Self-refresh Exit Time (clocks) */
#define DDR_STD_TSREX_TIME 8
#define DDR_LP_TSREX_TIME 8333333 // 120nS

/* (tWR) Dynamic Memory Write Recovery Time (1 / t) */
#define DDR_STD_TWR_TIME 66666666
#define DDR_LP_TWR_TIME 66666666 // 15nS

/* (tRC) Dynamic Memory Active To Active Command Period (1 / t) */
#define DDR_STD_TRC_TIME 18181818
#define DDR_LP_TRC_TIME 13333333 // 75nS

/* (tRFC) Dynamic Memory Auto-refresh Period (1 / t) */
#define DDR_STD_TRFC_TIME 14285714
#define DDR_LP_TRFC_TIME 14285714 // 70nS

/* (tXSNR) Dynamic Memory Exit Self-refresh (1 / t) */
#define DDR_STD_TXSNR_TIME 4952381
#define DDR_LP_TXSNR_TIME 8333333 // 120nS

/* (tRRD) Dynamic Memory Active Bank A to Active Bank B Time (1 / t) */
#define DDR_STD_TRRD_TIME 100000000
#define DDR_LP_TRRD_TIME 66666666 // 15nS

/* (tMRD) Dynamic Memory Load Mode Register To Active Command
   Time (1 / t) */
#define DDR_STD_TMRD_TIME 100000000
#define DDR_LP_TMRD_TIME 2 // 2 clocks

/* (tCDLR) Dynamic Memory Last Data In to Read Command Time (1 / t) */
#define DDR_STD_TCDLR_TIME 75000000
#define DDR_LP_TCDLR_TIME 75000000

/* Extended and normal mode word write fields */
#define DDR_STD_MODE_WORD_EXT  (0x1 << 24)
#define DDR_STD_MODE_WORD_NORM (0x0 << 24)

/* Mode word used for load extended mode command with DLL on and
   reduced drive strength */
#define DDR_STD_MODE_EXTENDED DDR_STD_MODE_WORD_EXT

/* Mode word used for load mode command.
   Burst length: 4 (assumed)     (0x2 << 0)
   Burst type  : Sequential      (0x0 << 3)
   CAS latency : 2               (0x2 << 4)
   Op mode     : Normal (DLL)    (0x2 << 7)
   Op mode     : Normal          (0x0 << 7)
   mode def    : Extended mode   (0x1 << 13) */
#define DDR_STD_MODE_WORD ((0x2 << 0) | (0x0 << 3) | (0x2 << 4))
#define DDR_STD_MODE_NORMAL 0x30800
#define DDR_STD_MODE_DLL_RESET (0x2 << 7)

/* Mode word used for load mode command (low power) */
#define DDR_LP_MODE_WORD ((0x2 << 0) | (0x0 << 3) | (0x2 << 4))
#define DDR_LP_MODE_NORMAL 0x18800
#define DDR_LP_MODE_EXTENDED (0x1 << 25)



/***********************************************************************
 * Static memory initialization constants
 *
 * This is a asynchronous device that doesn't require strong timing
 * requirements.
 *
 * Static memory type  : K6R4008V1C-JC12
 * Size                : 512KBytes per device x4 devices
 * Number of devices   : 4 (configurable and byte addressable)
 * Geometry            : 4 device x8 bits/device x 2^19 addr per device
 * Chip setup          : SRAM on nCS0 and nCS1 (selectable)
 * nCS to output       : 12nS
 * nOE to output       : 6nS
 * nOE deass to hi-z   : 6nS
 * nCE deass to hi-z   : 12nS
 **********************************************************************/

/* Time from nOE to data out ( 1 / t) */
#define EXTMEM_OE_TO_OUT 50000000

/* Time from deassertion of nOE to data high-z ( 1 / t) */
#define EXTMEM_NO_OE_TO_HIZ 83333333

/* Minimum write enable hold time before sampling (1 / t) */
#define EXTMEM_WE_HOLD_TIME 83333333

/* Memory configuration */
#define EXTMEM_CFG16 (EMC_STC_MEMWIDTH_16 | EMC_STC_BLS_EN_BIT)
#define EXTMEM_CFG32 (EMC_STC_MEMWIDTH_32 | EMC_STC_BLS_EN_BIT)

/***********************************************************************
 * Startup code support functions
 **********************************************************************/

/* Main startup code entry point, called from reset entry code */
void ea3250_init(void);

void ea3250_ddr_init(UNS_32 clk, UNS_32 board_memcfg);

/* Flush data cache (and invalidate) */
void dcache_flush(BOOL_32 inval);

#ifdef __cplusplus
}
#endif

#endif /* EA3250_STARTUP_H */
