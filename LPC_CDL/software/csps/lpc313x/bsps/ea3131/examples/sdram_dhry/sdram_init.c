/***********************************************************************
 * $Id: sdram_init.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: NXP val3150 board startup code
 *
 * Description:
 *     This file contains startup code used with the NXP val3150 board.
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
#include "ea3131_board.h"
#include "lpc313x_mpmc.h"
#include "lpc313x_sysreg.h"
#include "lpc313x_cgu_driver.h"
#include "lpc313x_timer_driver.h"
#include "lpc313x_i2c.h"
#include "lpc315x_analog.h"

/***********************************************************************
 * Startup code private data
 **********************************************************************/
#define TEST_MPMC_CLK    (90000000ull)
//#define TEST_CAS_3     1

#define pI2C1Regs                   ((I2C_REGS_T*) I2C1_BASE)
#define I2C_CLK_DIV_12MHZ           (60)


void analog_reg_init(void)
{
  pI2C1Regs->ckh = I2C_CLK_DIV_12MHZ;
  pI2C1Regs->ckl = I2C_CLK_DIV_12MHZ;

  pI2C1Regs->ctl = I2C_CTL_SOFT_RESET;  //Issue soft reset of the block
}

void analog_reg_read(UNS_32 reg_addr, UNS_32 * pReg_value)
{
  UNS_32   status = 0;

  // Write dummy value to avoid problems when no analog die is connected in asic simulation
  *pReg_value = 0;

  pI2C1Regs->fifo = AD_I2C_WRITE_ADR;                  // Send write address of the AD I2C slave with the start condition
  pI2C1Regs->fifo = ((reg_addr >> 8) & 0xFF);   // Send the high byte of the register address
  pI2C1Regs->fifo = (reg_addr & 0xFF);          // Send the low byte of the register address
  pI2C1Regs->fifo = AD_I2C_READ_ADR;                   // Send read address of the AD I2C slave with the (re)start condition
  pI2C1Regs->fifo = 0;                                 // Dummy 1
  pI2C1Regs->fifo = 0;                                 // Dummy 2
  pI2C1Regs->fifo = 0;                                 // Dummy 3
  pI2C1Regs->fifo = I2C_TXFF_STOP_CND;      // Dummy 4 with the stop condition

  while ((status & (I2C_STS_NAI | I2C_STS_TDI)) == 0)
  {
    status = pI2C1Regs->sts;
  }

  if (status & I2C_STS_TDI)
  {
    // Clear the TDI status
    pI2C1Regs->sts = I2C_STS_TDI;

    // Read out the register value
    *pReg_value = (((pI2C1Regs->fifo) & 0xFF) << 24);
    *pReg_value |= (((pI2C1Regs->fifo) & 0xFF) << 16);
    *pReg_value |= (((pI2C1Regs->fifo) & 0xFF) << 8);
    *pReg_value |= ((pI2C1Regs->fifo) & 0xFF);
  }
}

void analog_reg_write(UNS_32 reg_addr, UNS_32 reg_value)
{
  UNS_32   status = 0;

  pI2C1Regs->fifo = AD_I2C_WRITE_ADR;                  // Send write address of the AD I2C slave with the start condition
  pI2C1Regs->fifo = ((reg_addr >> 8) & 0xFF);   // Send the high byte of the register address
  pI2C1Regs->fifo = (reg_addr & 0xFF);          // Send the low byte of the register address
  pI2C1Regs->fifo = (reg_value >> 24);             // Send MSB of the register value
  pI2C1Regs->fifo = ((reg_value >> 16) & 0xFF);    // Send Byte 2
  pI2C1Regs->fifo = ((reg_value >> 8) & 0xFF);     // Sent Byte1
  pI2C1Regs->fifo = ((reg_value & 0xFF)            // Send LSB of the register value
                     | I2C_TXFF_STOP_CND);   // ... with the stop condition

  while ((status & (I2C_STS_NAI | I2C_STS_TDI)) == 0)
  {
    status = pI2C1Regs->sts;
  }

  if (status & I2C_STS_TDI)
  {
    // Clear the TDI status
    pI2C1Regs->sts = I2C_STS_TDI;
  }
}

/***********************************************************************
 * Private functions
 **********************************************************************/
static inline void timer_udelay(TIMER_REGS_T* pTimerRegs, UNS_32 usec)
{
  UNS_32 clkdlycnt;

  /* Determine the value to exceed before the count reaches the desired
     delay time */
  clkdlycnt = 12 * usec;

  /* Reset timer */
  pTimerRegs->control = 0;
  pTimerRegs->load = clkdlycnt;

  /* Enable the timer in free running mode*/
  pTimerRegs->control = TM_CTRL_ENABLE;

  /* Loop until terminal count matches or exceeds computed delay count */
  while (pTimerRegs->value <= clkdlycnt);

  /* Stop timer */
  pTimerRegs->control = 0;
}

/***********************************************************************
 *
 * Function: clock_setup
 *
 * Purpose: Setup system clocking
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void clock_setup(void)
{
  UNS_32 clkin_freq[CGU_FIN_SELECT_MAX];
  CGU_HPLL_SETUP_T pll =
  {
    CGU_FIN_SELECT_FFAST, //fin_select;
    770,		//ndec;
    8191,		//mdec;
    98,			//pdec;
    0,			//selr;
    16,			//seli;
    8,			//selp;
    0,			//mode;
    180000000	//freq; 
  };

  /* init clock frequencies */
  clkin_freq[0] = XTAL_IN; /* CGU_FIN_SELECT_FFAST */
  clkin_freq[1] = 0; /*	CGU_FIN_SELECT_XT_DAI_BCK0 */
  clkin_freq[2] = 0; /*	CGU_FIN_SELECT_XT_DAI_WS0 */
  clkin_freq[3] = 0; /*	CGU_FIN_SELECT_XT_DAI_BCK1 */
  clkin_freq[4] = 0; /*	CGU_FIN_SELECT_XT_DAI_WS1 */
  clkin_freq[5] = 0; /*	CGU_FIN_SELECT_HPPLL0     */
  clkin_freq[6] = 0; /*	CGU_FIN_SELECT_HPPLL1     */

  /* init CGU driver */
  cgu_init(clkin_freq);
  cgu_reset_all_clks();
  /* set HPLL1 - main PLL to default speed */
  cgu_hpll_config(CGU_HPLL1_ID, &pll);
  /* re-use the default clock tree distribution structure */
  cgu_init_clks(&g_cgu_default_clks);
}

/***********************************************************************
 *
 * Function: test_sdram_init
 *
 * Purpose: Setup SDRAM
 *
 * Processing:
 *     ***** Micron Initialization Sequence from their data sheet
 *           for the Micron MT48LC32M16A2 32M x 16 SDRAM chip:
 *
 *     Initialization
 *
 *     SDRAMs must be powered up and initialized in a
 *     predefined manner. Operational procedures other than
 *     those specified may result in undefined operation. Once
 *     power is applied to VDD and VDDQ (simultaneously) and
 *     the clock is stable (stable clock is defined as a signal
 *     cycling within timing constraints specified for the clock
 *     pin), the SDRAM requires a 100µs delay prior to issuing
 *     any command other than a COMMAND INHIBIT or NOP.
 *
 *     Starting at some point during this 100µs period and
 *     continuing at least through the end of this period,
 *     COMMAND INHIBIT or NOP commands should be applied.
 *     Once the 100µs delay has been satisfied with at least
 *     one COMMAND INHIBIT or NOP command having been applied,
 *     a PRECHARGE command should be applied. All banks must
 *     then be precharged, thereby placing the device in the
 *     all banks idle state.
 *
 *     Once in the idle state, two AUTO REFRESH cycles
 *     must be performed. After the AUTO REFRESH cycles are
 *     complete, the SDRAM is ready for mode register programming.
 *
 *     Because the mode register will power up in an
 *     unknown state, it should be loaded prior to applying any
 *     operational command.
 *
 *     *****  The JEDEC recommendation for initializing SDRAM is:
 *
 *      APPLY POWER (Vdd/Vddq equally, and CLK is stable)
 *      Wait 200uS
 *      PRECHARGE all
 *      8 AUTO REFRESH COMMANDS
 *      LOAD MODE REGISTER
 *      SDRAM is ready for operation
 *
 *     *****  The Micron SDRAM parts will work fine with the JEDEC sequence,
 *      but also allow for a quicker init sequence of:
 *
 *      APPLY POWER (Vdd/Vddq equally, and CLK is stable)
 *      Wait at least 100uS (during which time start applying and
 *         continue applying NOP or COMMAND INHIBIT)
 *		PRECHARGE all
 *		2 AUTO REFRESH COMMANDS (min requirement, more than 2 is also ok)
 *		LOAD MODE REGISTER
 *		SDRAM is ready for operation
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes:
 *     IF MPMC CLOCK SPEED IS FIXED REPLACE 'TEST_MPMC_CLK' WITH DECIMAL
 *     VALUE OF THE CLOCK. SO THAT THE COMPUTATION IS DONE AT COMPILE
 *     TIME. THIS WILL REDUCE THE CODE FOOTPRINT DRASTICALLY or ELSE
 *     LOT OF RUNTIME-LIBRARY CODE WILL BE ADDED TO THE IMAGE DUE TO
 *     64BIT CALCULATIONS DONE BY THE SDRAM TIMING MACROS.
 *
 **********************************************************************/
void test_sdram_init(void)
{
  UNS_32 tmp;
  UNS_32 reg_val;

  /* configure the LCD pins in EBI memory mode. */
  SYS_REGS->mux_lcd_ebi_sel = 1;

  /* init clocks */
  clock_setup();
  /* enable EBI clock */
  cgu_clk_en_dis(CGU_SB_EBI_CLK_ID, TRUE);

  /* Enable SYSCLK_O clock */
  cgu_clk_en_dis(CGU_SB_SYSCLK_O_ID, TRUE);

  /* Enable I2C system clock */
  cgu_clk_en_dis(CGU_SB_I2C1_PCLK_ID, TRUE);
  /* enable 1.8v rail */
  analog_reg_init();
  analog_reg_read(AD_REG_OTGDCLIC_RW, &reg_val);
  reg_val |= _BIT(16);
  analog_reg_write(AD_REG_OTGDCLIC_RW, reg_val);

  cgu_clk_en_dis(CGU_SB_TIMER0_PCLK_ID, TRUE);

  /* set the EBI pins in high speed mode for 1.8V operation */
  SYS_REGS->eshctrl_sup4 = 0;
  SYS_REGS->eshctrl_sup8 = 0;

  /* enable MPMC controller clocks */
  cgu_clk_en_dis(CGU_SB_MPMC_CFG_CLK_ID, TRUE);
  cgu_clk_en_dis(CGU_SB_MPMC_CFG_CLK2_ID, TRUE);
  cgu_clk_en_dis(CGU_SB_MPMC_CFG_CLK3_ID, TRUE);

  /* enable External Memory controller */
  MPMC->control = MPMC_CTL_ENABLE;
  /* Force HCLK to MPMC_CLK to 1:1 ratio */
  MPMC->config = MPMC_CFG_SDCCLK_1_1;
  /* set MPMC delay gates appropriately based on trace lengths between
  SDRAM and the chip. Also based on the delay startergy used for SDRAM. */
  SYS_REGS->mpmp_delaymodes = EA3131_MPMC_DELAY;

  timer_udelay(TIMER_CNTR0, 100);


  /* Set command delay startergy */
  MPMC->sdram_rdcfg = MPMC_SDRAMC_RDCFG_CMDDELAY_STG;

  /* configure "device config register" nSDCE0 for proper width
     SDRAM */
  MPMC->sdram[0].config = SDRAMC_16HP_32Mx16_4B_R13_C10;

#ifdef TEST_CAS_3
  MPMC->sdram[0].rascas = MPMC_SDRAMC_RASCAS_RAS3 | MPMC_SDRAMC_RASCAS_CAS3;
#else
  MPMC->sdram[0].rascas = MPMC_SDRAMC_RASCAS_RAS2 | MPMC_SDRAMC_RASCAS_CAS2;
#endif

  /* min 20ns program 1 so that atleast 2 HCLKs are used */
  MPMC->sdram_rp   = MPMC_SDRAMC_TRP(EA3131_SDRAM_TRP, TEST_MPMC_CLK);
  MPMC->sdram_ras  = MPMC_SDRAMC_TRAS(EA3131_SDRAM_TRAS, TEST_MPMC_CLK);
  MPMC->sdram_srex = MPMC_SDRAMC_TSREX(EA3131_SDRAM_TREX, TEST_MPMC_CLK);
  MPMC->sdram_apr  = EA3131_SDRAM_TARP;
  MPMC->sdram_dal  = MPMC_SDRAMC_TDAL(EA3131_SDRAM_TDAL, TEST_MPMC_CLK);
  MPMC->sdram_wr   = MPMC_SDRAMC_TWR(EA3131_SDRAM_TWR, TEST_MPMC_CLK);
  MPMC->sdram_rc   = MPMC_SDRAMC_TRC(EA3131_SDRAM_TRC, TEST_MPMC_CLK);
  MPMC->sdram_rfc  = MPMC_SDRAMC_TRFC(EA3131_SDRAM_TRFC, TEST_MPMC_CLK);
  MPMC->sdram_xsr  = MPMC_SDRAMC_TXSR(EA3131_SDRAM_TXSR, TEST_MPMC_CLK);
  MPMC->sdram_rrd  = MPMC_SDRAMC_TRRD(EA3131_SDRAM_TRRD, TEST_MPMC_CLK);
  MPMC->sdram_mrd  = MPMC_SDRAMC_TMRD(EA3131_SDRAM_TMRD, TEST_MPMC_CLK);

  timer_udelay(TIMER_CNTR0, 100);

  /* issue continuous NOP commands (INIT & MRS set) */
  MPMC->sdram_ctrl = MPMC_SDRAMC_CTL_CE | MPMC_SDRAMC_CTL_CS |
                     MPMC_SDRAMC_CTL_NOP_CMD;

  /* load ~200us delay value to timer1 */
  timer_udelay(TIMER_CNTR0, 200);

  /* issue a "pre-charge all" command */
  MPMC->sdram_ctrl = MPMC_SDRAMC_CTL_CE | MPMC_SDRAMC_CTL_CS |
                     MPMC_SDRAMC_CTL_PALL_CMD;

  /*******************************************************************
  * Minimum refresh pulse interval (tRFC) for MT48LC32M16A2=80nsec,
  * 100nsec provides more than adequate interval.
  ******************************************************************/
  MPMC->sdram_ref = MPMC_SDRAMC_REFRESH(EA3131_SDRAM_REFRESH,
                                        TEST_MPMC_CLK);

  /* load ~250us delay value to timer1 */
  timer_udelay(TIMER_CNTR0, 250);

  /*******************************************************************
  *  Recommended refresh interval for normal operation of the Micron
  * MT48LC16LFFG = 7.8125usec (128KHz rate).
  * ((HCLK / 128000) - 1) = refresh counter interval rate, (subtract
  * one for safety margin).
  ******************************************************************/
  MPMC->sdram_ref = MPMC_SDRAMC_REFRESH(EA3131_SDRAM_OPER_REFRESH,
                                        TEST_MPMC_CLK);

  /* select mode register update mode */
  MPMC->sdram_ctrl = MPMC_SDRAMC_CTL_CE | MPMC_SDRAMC_CTL_CS |
                     MPMC_SDRAMC_CTL_MODE_CMD;

  /*******************************************************************
  * Program the SDRAM internal mode registers on bank nSDCE0
  * and reconfigure the SDRAM chips.  Bus speeds up to 90MHz
  * requires use of a CAS latency = 2.
  * To get correct value on address bus CAS cycle, requires a shift
  * by 13 for 16bit mode
  ******************************************************************/
#ifdef TEST_CAS_3
  tmp = *((volatile UNS_32 *)(EXT_SDRAM_BASE | _SBF(13, 0x33)));
#else
  tmp = *((volatile UNS_32 *)(EXT_SDRAM_BASE | _SBF(13, 0x23)));
#endif

  MPMC->sdram[0].config = SDRAMC_16HP_32Mx16_4B_R13_C10;

#ifdef TEST_CAS_3
  MPMC->sdram[0].rascas = MPMC_SDRAMC_RASCAS_RAS3 | MPMC_SDRAMC_RASCAS_CAS3;
#else
  MPMC->sdram[0].rascas = MPMC_SDRAMC_RASCAS_RAS2 | MPMC_SDRAMC_RASCAS_CAS2;
#endif

  /* select normal operating mode */
  MPMC->sdram_ctrl = MPMC_SDRAMC_CTL_CE | MPMC_SDRAMC_CTL_CS |
                     MPMC_SDRAMC_CTL_NORMAL_CMD;

  MPMC->sdram[0].config |= MPMC_SDRAMC_CFG_BUF_EN;

  MPMC->sdram_ctrl = MPMC_SDRAMC_CTL_NORMAL_CMD | MPMC_SDRAMC_CTL_CS;
}

