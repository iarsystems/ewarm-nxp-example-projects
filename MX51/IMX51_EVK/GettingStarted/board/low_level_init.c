/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : low_level_init.c
 *    Description : low_level_init for iMX systems
 *                  
 *                  
 *    History :
 *    1. Date        : 1.03.2011
 *       Author      : Stanimir Bonev
 *       Description : initial version (for iMX535 QSB)
 *
 *    2. Date        : 15.03.2012
 *       Author      : Atanas Uzunov
 *       Description : ported to iMX51 EVK
 *
 *    $Revision: 5880 $
 **************************************************************************/

/** include files **/
#include <NXP/iomcimx515.h>
#include <stdint.h>
#include "arm_comm.h"

/** local definitions **/

/** default settings **/

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
  DPLLIP1_DP_CTL = 0x00001232; /* Set DPLL ON (set UPEN bit): BRMO=1 */
  DPLLIP1_DP_CONFIG = 2;       /* Enable auto-restart AREN bit */

  DPLLIP1_DP_OP = ((8 << 4) + ((1 - 1)  << 0));      /* MFI=8, PDF=1 */
  DPLLIP1_DP_HFS_OP = ((8 << 4) + ((1 - 1)  << 0));  /* HFS mode, MFI=8, PDF=1 */
  
  DPLLIP1_DP_MFD = 180 - 1;            /* MFD = 180 */
  DPLLIP1_DP_HFS_MFD = 180 - 1;        /* HFS mode, MFD = 180 */

  
  DPLLIP1_DP_MFN = 60;           /* MFN = 60 */
  DPLLIP1_DP_HFS_MFN = 60;       /* HFS mode, MFN = 60 */

  DPLLIP1_DP_CTL = 0x00001232;

  while(!DPLLIP1_DP_CTL_bit.LRF);
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
  DPLLIP3_DP_CTL = 0x00001232; /* Set DPLL ON (set UPEN bit): BRMO=1 */
  DPLLIP3_DP_CONFIG = 2;       /* Enable auto-restart AREN bit */

  DPLLIP3_DP_OP = ((6 << 4) + ((3 - 1)  << 0));
  DPLLIP3_DP_HFS_OP = ((6 << 4) + ((3 - 1)  << 0));
  
  DPLLIP3_DP_MFD = (4 - 1);
  DPLLIP3_DP_HFS_MFD = (4 - 1);

  DPLLIP3_DP_MFN = 3;
  DPLLIP3_DP_HFS_MFN = 3;

  DPLLIP3_DP_CTL = 0x00001232;

  while(!DPLLIP3_DP_CTL_bit.LRF);
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
uint32_t reg;
  /* Configure CKO2 debug output - ddr_clk_root */
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_3_bit.MUX_MODE = 5; // CLKO2  
  CCM_CCOSR_bit.cko2_div = 7; 
  CCM_CCOSR_bit.cko2_sel = 9;
  CCM_CCOSR_bit.cko2_en = 1;
  /* Configure CKO1 debug output - pll1_sw_clk */
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_9_bit.MUX_MODE = 4; // CLKO
  CCM_CCOSR_bit.cko1_div = 7;
  CCM_CCOSR_bit.cko1_sel = 1;
  CCM_CCOSR_bit.cko1_en = 1;  
 
  /* explicitly disable L2 cache */
  reg = __MRC(15, 0, 1, 0, 1);
  reg &= ~(1ul << 1);
  __MCR(15, 0, reg, 1, 0, 0);
  /* reconfigure L2 cache aux control reg */
  reg = 0xC4ul
      | (1ul << 24)   /* disable write allocate delay */
      | (1ul << 23)   /* disable write allocate combine */
      | (1ul << 22)   /* disable write allocate */
      ;
  
  __MCR(15, 1, reg, 9, 0, 2);

  /* init_clock */
  /* Switch ARM to step clock */
  CCM_CCSR = 4;
  setup_dpll1_800();
  setup_dpll3_216();
  /* Set the platform clock dividers */
  TIGERP_ICGC = 0x00000725;
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

  /* make sure divider effective */
  while(CCM_CDHIPR);  
  CCM_CCDR = 0;

  return 1;
}
