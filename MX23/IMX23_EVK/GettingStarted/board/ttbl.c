/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : ttbl.c
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : May 2011
 *       Author      : Stoyan Choynev
 *       Description : iMX23 EVK translation tables
 *
 *        The physical memory map is:
 *                                                                      Instruction cache
 *                                                                        Data cache
 *                                                                          User Access
 *                                                                            Supervisor Access
 *                                                                               Read Only
 *  Segment                     Description     Size                    I D U S R
 *  0x0000_0000 - 0x0000_7FFF   OCRAM           32KB                    * * * * - 
 *  0x0000 8000 – 0x3FFF_FFFF   Reserved OCRAM  1GB - 32KB              - - - - -
 *                              alias
 *  0x4000 0000 – 0x5FFF_FFFF   External Memory 512MB                   * * * * -
 *  0x6000 0000 – 0x7FFF_FFFF   Reserved        512MB                   - - - - -
 *  0x8000_0000 – 0x8000_1FFF   ICOLL           8KB                     - - - * - 
 *  0x8000_2000 – 0x8000_3FFF   Reserved        8KB                     - - - - - 
 *  0x8000_4000 – 0x8000_5FFF   APBHDMA         8KB                     - - - * - 
 *  0x8000_6000 – 0x8000_7FFF   Reserved        8KB                     - - - - - 
 *  0x8000_8000 – 0x8000_9FFF   ECC8            8KB                     - - - * - 
 *  0x8000_A000 – 0x8000_BFFF   BCH             8KB                     - - - * - 
 *  0x8000_C000 – 0x8000_DFFF   GPMI            8KB                     - - - * - 
 *  0x8000_E000 – 0x8000_FFFF   Reserved        8KB                     - - - - - 
 *  0x8001_0000 – 0x8001_1FFF   SSP1            8KB                     - - - * - 
 *  0x8001_2000 – 0x8001_3FFF   Reserved        8KB                     - - - - - 
 *  0x8001_4000 – 0x8001_5FFF   ETM             8KB                     - - - * - 
 *  0x8001_4000 – 0x8001_5FFF   Reserved        8KB                     - - - - - 
 *  0x8001_8000 – 0x8001_9FFF   PINCTRL         8KB                     - - - * - 
 *  0x8001_A000 – 0x8001_BFFF   Reserved        8KB                     - - - - - 
 *  0x8001_C000 – 0x8001_DFFF   DIGCTL          8KB                     - - - * - 
 *  0x8001_E000 – 0x8001_FFFF   Reserved        8KB                     - - - - - 
 *  0x8002_0000 – 0x8002_1FFF   EMI             8KB                     - - - * - 
 *  0x8002_2000 – 0x8002_3FFF   Reserved        8KB                     - - - - - 
 *  0x8002_4000 – 0x8002_5FFF   APBXDMA         8KB                     - - - * - 
 *  0x8002_6000 – 0x8002_7FFF   Reserved        8KB                     - - - - - 
 *  0x8002_8000 – 0x8002_9FFF   DCP             8KB                     - - - * - 
 *  0x8002_A000 – 0x8002_BFFF   PXP             8KB                     - - - * - 
 *  0x8002_C000 – 0x8002_DFFF   OCOTP           8KB                     - - - * - 
 *  0x8002_E000 – 0x8002_FFFF   AXI             8KB                     - - - * - 
 *  0x8003_0000 – 0x8003_1FFF   LCDIF           8KB                     - - - * - 
 *  0x8003_2000 – 0x8003_3FFF   Reserved        8KB                     - - - - - 
 *  0x8003_4000 – 0x8003_5FFF   SSP2            8KB                     - - - * - 
 *  0x8003_6000 – 0x8003_7FFF   Reserved        8KB                     - - - - - 
 *  0x8003_8000 – 0x8003_9FFF   TVENC           8KB                     - - - * - 
 *  0x8003_A000 – 0x8003_FFFF   Reserved        3*8KB                   - - - - - 
 *  0x8004_0000 – 0x8004_1FFF   CLKCTRL         8KB                     - - - * - 
 *  0x8004_2000 – 0x8004_3FFF   SAIF1           8KB                     - - - * - 
 *  0x8004_4000 – 0x8004_5FFF   PWR             8KB                     - - - * - 
 *  0x8004_6000 – 0x8004_7FFF   SAIF2           8KB                     - - - * - 
 *  0x8004_8000 – 0x8004_9FFF   AUDIOOUT        8KB                     - - - * - 
 *  0x8004_A000 – 0x8004_BFFF   Reserved        8KB                     - - - - - 
 *  0x8004_C000 – 0x8004_DFFF   AUDIOIN         8KB                     - - - * - 
 *  0x8004_E000 – 0x8004_FFFF   Reserved        8KB                     - - - - - 
 *  0x8005_0000 – 0x8005_1FFF   LRADC           8KB                     - - - * - 
 *  0x8005_2000 – 0x8005_3FFF   Reserved        8KB                     - - - - - 
 *  0x8005_4000 – 0x8005_5FFF   SPDIF           8KB                     - - - * - 
 *  0x8005_6000 – 0x8005_7FFF   Reserved        8KB                     - - - - - 
 *  0x8005_8000 – 0x8005_9FFF   I2C             8KB                     - - - * - 
 *  0x8005_A000 – 0x8005_BFFF   Reserved        8KB                     - - - - - 
 *  0x8005_C000 – 0x8005_DFFF   RTC             8KB                     - - - * - 
 *  0x8005_E000 – 0x8006_3FFF   Reserved        3*8KB                   - - - - - 
 *  0x8006_4000 – 0x8006_5FFF   PWM             8KB                     - - - * - 
 *  0x8006_6000 – 0x8006_7FFF   Reserved        8KB                     - - - - - 
 *  0x8006_8000 – 0x8006_9FFF   TIMROT          8KB                     - - - * - 
 *  0x8006_A000 – 0x8006_BFFF   Reserved        8KB                     - - - - - 
 *  0x8006_C000 – 0x8006_DFFF   APPUART1        8KB                     - - - * - 
 *  0x8006_E000 – 0x8006_FFFF   APPUART2        8KB                     - - - * - 
 *  0x8007_0000 – 0x8007_1FFF   DBGUART         8KB                     - - - * - 
 *  0x8007_2000 – 0x8007_BFFF   Reserved        5*8KB                   - - - - - 
 *  0x8007_C000 – 0x8007_DFFF   USBPHY          8KB                     - - - * - 
 *  0x8007_E000 – 0x8007_FFFF   Reserved        8KB                     - - - - - 
 *  0x8008_0000 – 0x800B_FFFF   USB             256KB                   - - - * - 
 *  0x800C_0000 – 0x800D_FFFF   Reserved        128KB                   - - - - - 
 *  0x800E_0000 – 0x800F_FFFF   DRAM            64KB                    - - - * - 
 *  0x8010_0000 – 0xBFFF_FFFF   Reserved                                - - - - - 
 *  0xC000_0000 – 0xC000_FFFF   OCROM           64KB                    * - * * * 
 *  0xC001_0000 – 0xFFFF_FFFF   Reserved                                - - - - -
 *                              OCROM alias 
 *  Domain 0 - 01 Client
 *  Domain 1 - 11 Manager
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "ttbl.h"

#pragma segment="MMU_TT"

#pragma location="MMU_TT"
#pragma data_alignment=16384
__no_init Int32U L1Table[L1_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse1[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse2[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse3[L2_CP_ENTRIES_NUMB];
#pragma data_alignment=2048
__no_init Int32U L2Coarse4[L2_CP_ENTRIES_NUMB];

extern __arm void __vector(void);

const TtSectionBlock_t TtSB[] =
{
  // L1
  //1 MB coarses table 1  OCRAM
  L1_COARSES_PAGE_ENTRY(  1,0x00000000,(Int32U)L2Coarse1  ,  0    ),
  //64MB RW section section cached DDR
  L1_SECTIONS_ENTRY(    64, 0x40000000,0x40000000         ,3,0,1,1),
  //64MB RW section section no cached DDR
  L1_SECTIONS_ENTRY(    64, 0x44000000,0x44000000         ,3,0,0,0),
  //1 MB coarses table 2  0x80000000 SFR
  L1_COARSES_PAGE_ENTRY(  1,0x80000000,(Int32U)L2Coarse2  ,  0    ),
  //1 MB coarses table 3  0xC0000000 CROM
  L1_COARSES_PAGE_ENTRY(  1,0xC0000000,(Int32U)L2Coarse3  ,  0    ),
  //1 MB coarses table 4  vectors remap
  L1_COARSES_PAGE_ENTRY(  1,0xFFF00000,(Int32U)L2Coarse4  ,  0    ),
  TSB_INVALID,
  // L2 coarses table1
  // 32KB 8 small pages OCRAM alias cached
  L2_CT_SMALL_PAGE_ENTRY( 8,0x00008000, 0x00008000        ,3,3,3,3,1,1),
  // 32KB 8 small pages OCRAM alias no cached
  L2_CT_SMALL_PAGE_ENTRY( 8,0x00010000, 0x00010000        ,3,3,3,3,0,0),
  TSB_INVALID,
  // L2 coarses table2
  // 8KB 2 small pages ICOLL SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80000000, 0x80000000        ,2,2,2,2,0,0),
  // 8KB 2 small pages APBHDMA SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80004000, 0x80004000        ,2,2,2,2,0,0),
  // 24KB 6 small pages ECC8,BCH, GPMI SFR
  L2_CT_SMALL_PAGE_ENTRY( 6,0x80008000, 0x80008000        ,2,2,2,2,0,0),
  // 8KB 2 small pages SSP1 SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80010000, 0x80010000        ,2,2,2,2,0,0),
  // 8KB 2 small pages ETM, SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80014000, 0x80014000        ,2,2,2,2,0,0),
  // 8KB 2 small pages PINCTRL SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80018000, 0x80018000        ,2,2,2,2,0,0),
  // 8KB 2 small pages DIGCTL SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x8001C000, 0x8001C000        ,2,2,2,2,0,0),
  // 8KB 2 small pages EMI SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80020000, 0x80020000        ,2,2,2,2,0,0),
  // 8KB 2 small pages APBXDMA SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80024000, 0x80024000        ,2,2,2,2,0,0),
  // 40KB 10 small pages DCP, PXP, OCOTP, AXI, LCDIF SFR
  L2_CT_SMALL_PAGE_ENTRY(10,0x80028000, 0x80028000        ,2,2,2,2,0,0),
  // 8KB 2 small pages SSP2 SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80034000, 0x80034000        ,2,2,2,2,0,0),
  // 8KB 2 small pages TVENC SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80038000, 0x80038000        ,2,2,2,2,0,0),
  // 40KB 10 small pages CLKCTRL, SAIF1, PWR, SAIF2, AUDIOOUT SFR
  L2_CT_SMALL_PAGE_ENTRY(10,0x80040000, 0x80040000        ,2,2,2,2,0,0),
  // 8KB 2 small pages AUDIOOUT SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x8004C000, 0x8004C000        ,2,2,2,2,0,0),
  // 8KB 2 small pages LRADC SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80050000, 0x80050000        ,2,2,2,2,0,0),
  // 8KB 2 small pages SPDIF SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80054000, 0x80054000        ,2,2,2,2,0,0),
  // 8KB 2 small pages I2C SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80058000, 0x80058000        ,2,2,2,2,0,0),
  // 8KB 2 small pages RTC SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x8005C000, 0x8005C000        ,2,2,2,2,0,0),
  // 8KB 2 small pages PWM SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80064000, 0x80064000        ,2,2,2,2,0,0),
  // 8KB 2 small pages TIMROT SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80068000, 0x80068000        ,2,2,2,2,0,0),
  // 24KB 6 small pages APPUART1, APPUART2, DBGUART SFR
  L2_CT_SMALL_PAGE_ENTRY( 6,0x8006C000, 0x8006C000        ,2,2,2,2,0,0),
  // 8KB 2 small pages USBPHY SFR
  L2_CT_SMALL_PAGE_ENTRY( 2,0x8007C000, 0x8007C000        ,2,2,2,2,0,0),
  // 256KB 64 small pages USB SFR
  L2_CT_SMALL_PAGE_ENTRY(64,0x80080000, 0x80080000        ,2,2,2,2,0,0),
  // 64KB 16 small pages DRAM SFR
  L2_CT_SMALL_PAGE_ENTRY(16,0x800E0000, 0x800E0000        ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 coarses table3
  // 64KB 1 large pages OCROM section cached 
  L2_CT_LARGE_PAGE_ENTRY( 1,0xC0000000,0xC0000000         ,0,0,0,0,1,1),
  TSB_INVALID,
  // L2 coarses table4
  // 4KB 1 small page vectors remap
  L2_CT_SMALL_PAGE_ENTRY( 1,0xFFFF0000,(Int32U)__vector   ,2,2,2,2,1,1),
  TSB_INVALID,
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Coarse1,TableL2_Coarse},
  {L2Coarse2,TableL2_Coarse},
  {L2Coarse3,TableL2_Coarse},
  {L2Coarse4,TableL2_Coarse},
  TTB_INVALID
};
