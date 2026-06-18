/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : ttbl.c
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *        The physical memory map is:
 *                                                                      Instruction cache
 *                                                                        Data cache
 *                                                                          User Access
 *                                                                            Supervisor Access
*                                                                               Read Only
 *  Segment                     Description     Size                    I D U S R
 *  0x0000_0000 - 0x0000_3FFF   BROM            16KB                    * - * * * coarses 1M
 *  0x0000 4000 – 0x0040 3FFF   Reserved        4 Mbyte                 - - - - -
 *  0x0040 4000 – 0x0040_8FFF   BROM            20Kbyte                 * - * * * coarses 1M
 *  0x0040_9000 – 0x0FFF_FFFF   Reserved        252Mbyte - 36kB         - - - - -
 *  0x1000 0000 – 0x1FFF_FFFF   Reserved        256 Mbytes              - - - - -
 *  0x2000 0000 – 0x2FFF_FFFF   Reserved        256 Mbytes              - - - - -
 *  0x3000 0000 – 0x3FFF_FFFF   Reserved        256 Mbytes              - - - - -
 *  0x4000 0000 – 0x43EF_FFFF   Reserved        63 Mbytes               - - - - -
 *  0x43F0_0000 – 0x43F0_3FFF   AIPS A          16 Kbytes               - - - * - Entry 257
 *  0x43F0_4000 – 0x43F0_7FFF   MAX             16 Kbytes               - - - * -
 *  0x43F0_8000 – 0x43F0_BFFF   CLKCTL          16 Kbytes               - - - * -
 *  0x43F0_C000 – 0x43F0_FFFF   ETB             16 Kbytes               - - - * -
 *  0x43F1_0000 – 0x43F1_3FFF   ETB Memory      16 Kbytes               - - - * -
 *  0x43F1_4000 – 0x43F1_7FFF   AAPE            16 Kbytes               - - - * -
 *  0x43F1_8000 – 0x43F7_FFFF   Reserved        416 Kbytes              - - - - -
 *  0x43F8_0000 – 0x43F8_3FFF   I2C-1           16 Kbytes               - - - * -
 *  0x43F8_4000 – 0x43F8_7FFF   I2C-3           16 Kbytes               - - - * -
 *  0x43F8_8000 – 0x43F8_BFFF   CAN-1           16 Kbytes               - - - * -
 *  0x43F8_C000 – 0x43F8_C000   CAN-2           16 Kbytes               - - - * -
 *  0x43F9_0000 – 0x43F9_3FFF   UART-1          16 Kbytes               - - - * -
 *  0x43F9_4000 – 0x43F9_7FFF   UART-2          16 Kbytes               - - - * -
 *  0x43F9_8000 – 0x43F9_BFFF   I2C-2           16 Kbytes               - - - * -
 *  0x43F9_C000 – 0x43F9_FFFF   1-Wire          16 Kbytes               - - - * -
 *  0x43FA_0000 – 0x43FA_3FFF   ATA             16 Kbytes               - - - * -
 *  0x43FA_4000 – 0x43FA_7FFF   CSPI-1          16 Kbytes               - - - * -
 *  0x43FA_8000 - 0x43FA_BFFF   KPP             16 Kbytes               - - - * -
 *  0x43FA_C000 - 0x43FA_FFFF   IOMUXC          16 Kbytes               - - - * -
 *  0x43FB_0000 - 0x43FB_3FFF   AUDMUX          16 Kbytes               - - - * -
 *  0x43FB_4000 - 0x43FB_7FFF   Reserved        16 Kbytes               - - - - -
 *  0x5000_0000 - 0x5000_3FFF   SPBA            16 Kbytes               - - - * -
 *  0x5000_4000 - 0x5000_7FFF   CSPI-3          16 Kbytes               - - - * -
 *  0x5000_8000 - 0x5000_BFFF   UART-4          16 Kbytes               - - - * -
 *  0x5000_C000 - 0x5000_FFFF   UART-3          16 Kbytes               - - - * -
 *  0x5001_0000 - 0x5001_3FFF   CSPI-2          16 Kbytes               - - - * -
 *  0x5001_4000 - 0x5001_7FFF   SSI-2           16 Kbytes               - - - * -
 *  0x5001_8000 - 0x5001_BFFF   ESAI            16 Kbytes               - - - * -
 *  0x5001_C000 - 0x5001_FFFF   Reserved        16 Kbytes               - - - - -
 *  0x5002_0000 - 0x5002_3FFF   ATA             16 Kbytes               - - - * -
 *  0x5002_0000 - 0x5002_3FFF   SIM-1           16 Kbytes               - - - * -
 *  0x5002_8000 - 0x5002_BFFF   SIM-2           16 Kbytes               - - - * -
 *  0x5002_C000 - 0x5002_FFFF   UART-5          16 Kbytes               - - - * -
 *  0x5003_0000 - 0x5003_3FFF   TSC             16 Kbytes               - - - * -
 *  0x5003_4000 - 0x5003_7FFF   SSI-1           16 Kbytes               - - - * -
 *  0x5003_8000 - 0x5003_BFFF   FEC             16 Kbytes               - - - * -
 *  0x5003_C000 - 0x5003_FFFF   SPBA            16 Kbytes               - - - * -
 *  0x5004_0000 - 0x51FF_FFFF   AIPS B          32 mbytes - 256 Kbytes  - - - * -
 *  0x5200_0000 - 0x53EF_FFFF   Reserved        31 Mbytes               - - - - -
 *  0x53F0_0000 - 0x53F0_3FFF   AIPS B          16 Kbytes               - - - * -
 *  0x53F0_4000 - 0x53F7_FFFF   Reserved        496 Kbytes              - - - - -
 *  0x53F8_0000 - 0x53F8_3FFF   CCM             16 Kbytes               - - - * -
 *  0x53F8_4000 - 0x53F8_7FFF   GPT-4           16 Kbytes               - - - * -
 *  0x53F8_8000 - 0x53F8_BFFF   GPT-3           16 Kbytes               - - - * -
 *  0x53F8_C000 - 0x53F8_FFFF   GPT-2           16 Kbytes               - - - * -
 *  0x53F9_0000 - 0x53F9_3FFF   GPT-1           16 Kbytes               - - - * -
 *  0x53F9_4000 - 0x53F9_7FFF   EPIT-1          16 Kbytes               - - - * -
 *  0x53F9_4000 - 0x53F9_7FFF   EPIT-2          16 Kbytes               - - - * -
 *  0x53F9_C000 - 0x53F9_FFFF   GPIO-4          16 Kbytes               - - - * -
 *  0x53FA_0000 - 0x53FA_3FFF   PWM-2           16 Kbytes               - - - * -
 *  0x53FA_4000 - 0x53FA_7FFF   GPIO-3          16 Kbytes               - - - * -
 *  0x53FA_8000 - 0x53FA_BFFF   PWM-3           16 Kbytes               - - - * -
 *  0x53FA_C000 - 0x53FA_FFFF   SCC             16 Kbytes               - - - * -
 *  0x53FB_0000 - 0x53FB_3FFF   RNGB            16 Kbytes               - - - * -
 *  0x53FB_4000 - 0x53FB_7FFF   eSDHC-1         16 Kbytes               - - - * -
 *  0x53FB_8000 - 0x53FB_BFFF   eSDHC-2         16 Kbytes               - - - * -
 *  0x53FB_C000 - 0x53FB_FFFF   LCDC            16 Kbytes               - - - * -
 *  0x53FC_0000 - 0x53FC_3FFF   SLCDC           16 Kbytes               - - - * -
 *  0x53FC_4000 - 0x53FC_7FFF   Reserved        16 Kbytes               - - - - -
 *  0x53FC_8000 - 0x53FC_BFFF   PWM-4           16 Kbytes               - - - * -
 *  0x53FC_C000 - 0x53FC_FFFF   GPIO-1          16 Kbytes               - - - * -
 *  0x53FD_0000 - 0x53FD_3FFF   GPIO-2          16 Kbytes               - - - * -
 *  0x53FD_4000 - 0x53FD_7FFF   SDMA            16 Kbytes               - - - * -
 *  0x53FD_8000 - 0x53FD_BFFF   Reserved        16 Kbytes               - - - - -
 *  0x53FD_C000 - 0x53FD_FFFF   WDT             16 Kbytes               - - - * -
 *  0x53FE_0000 - 0x53FE_3FFF   PWM-1           16 Kbytes               - - - * -
 *  0x53FE_4000 - 0x53FE_BFFF   Reserved        32 Kbytes               - - - - -
 *  0x53FE_C000 - 0x53FE_FFFF   RTICv3          16 Kbytes               - - - * -
 *  0x53FF_0000 - 0x53FF_3FFF   IIM             16 Kbytes               - - - * -
 *  0x53FF_4000 - 0x53FF_7FFF   USB             16 Kbytes               - - - * -
 *  0x53FF_8000 - 0x53FF_BFFF   CSI             16 Kbytes               - - - * -
 *  0x53FF_C000 - 0x53FF_FFFF   DryIce          16 Kbytes               - - - * -
 *  0x5400_0000 - 0x5FFF_FFFF   Reserved        192 Mbytes              - - - - -
 *  0x6000_0000 - 0x67FF_FFFF   ROMPATCH        128 Mbytes              - - - * - entry 128MB
 *  0x6800_0000 - 0x6FFF_FFFF   ASIC            128 Mbytes              - - - * -
 *  0x7000_0000 - 0x77FF_FFFF   Reserved        128 Mbytes              - - - * -
 *  0x7800_0000 - 0x7BFF_FFFF   SRAM alias      64 Mbytes               * * * * -
 *  0x7C00_0000 - 0x7FFF_FFFF   SRAM alias      64 Mbytes               * - * * -
 *  0x8000_0000 - 0x87FF_FFFF   SDRAM bank 0    128 Mbytes              * * * * -
 *  0x8800_0000 - 0x8FFF_FFFF   SDRAM bank 0    128 Mbytes              * - * * -
 *  0x9000_0000 - 0x97FF_FFFF   SDRAM bank 1    128 Mbytes              * * * * -
 *  0x9800_0000 - 0x9FFF_FFFF   SDRAM bank 1    128 Mbytes              * - * * -
 *  0xA000_0000 - 0xA3FF_FFFF   WEIM CS0        64 Mbytes               * * * * *
 *  0xA400_0000 - 0xA7FF_FFFF   WEIM CS0        64 Mbytes               * - * * *
 *  0xA800_0000 - 0xAFFF_FFFF   WEIM CS1        128 Mbytes              * * * * *
 *  0xB000_0000 - 0xB1FF_FFFF   WEIM CS2        32 Mbytes               * * * * -
 *  0xB200_0000 - 0xB3FF_FFFF   WEIM CS3        32 Mbytes               * * * * -
 *  0xB400_0000 - 0xB5FF_FFFF   WEIM CS4        32 Mbytes               * * * * *
 *  0xB600_0000 - 0xB7FF_FFFF   Reserved        32 Mbytes               * - * * -
 *  0xB800_0000 - 0xB800_0FFF   Reserved        4 Kbytes                - - - - - coarses 1M
 *  0xB800_1000 - 0xB800_1FFF   SDRAM           4 Kbytes                - - - * -
 *  0xB800_2000 - 0xB800_2FFF   WEIM            4 Kbytes                - - - * -
 *  0xB800_3000 - 0xB800_3FFF   M3IF            4 Kbytes                - - - * -
 *  0xB800_4000 - 0xB800_4FFF   EMI             4 Kbytes                - - - * -
 *  0xB800_5000 - 0xBAFF_FFFF   Reserved        32 Mbytes - 20 Kbytes   - - - - -
 *  0xBB00_0000 - 0xBB00_0FFF   NAND main       4 Kbytes                - - * * -
 *  0xBB00_1000 - 0xBB00_11FF   NAND spare      512 bytes               - - * * -
 *  0xBB00_1200 - 0xBB00_1DFF   Reserved        3 Kbytes                - - - - -
 *  0xBB00_1E00 - 0xBB00_1FFFF  NAND            512 bytes               - - - * -
 *  0xBB01_2000 - 0xBFFF_FFFF   Reserved        96 Mbytes - 8 Kbytes    - - - - -
 *  0xC000_0000 - 0xFFFF_FFFF   Reserved        1024 Mbytes             - - - - -
 *
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

#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine1[L2_FP_ENTRIES_NUMB];

extern __arm void __vector(void);

const TtSectionBlock_t TtSB[] =
{
  // L1
  //   1 MB coarses table 1  0x0000 0000
  L1_COARSES_PAGE_ENTRY(  1,0x00000000,(Int32U)L2Coarse1  ,  0    ),
  //   1 MB coarses table 2  0x0040 0000
  L1_COARSES_PAGE_ENTRY(  1,0x00400000,(Int32U)L2Coarse2  ,  0    ),
  // 257 MB RW section section no cached (SFR)
  L1_SECTIONS_ENTRY(    257,0x43F00000,0x43F00000         ,1,0,0,0),
  // 256 MB RW section section no cached (SFR)
  L1_SECTIONS_ENTRY(    256,0x60000000,0x60000000         ,1,0,0,0),
  // 64 MB RW section section cached (SRAM 128KB alias 64MB)
  L1_SECTIONS_ENTRY(     64,0x78000000,0x78000000         ,3,0,1,1),
  // 64 MB RW section section no cached (SRAM 128KB alias 64MB)
  L1_SECTIONS_ENTRY(     64,0x7C000000,0x7C000000         ,3,0,0,0),
  // 16 MB RW section section cached SDRAM
  L1_SECTIONS_ENTRY(    16, 0x80000000,0x80000000         ,3,0,1,1),
  // 16 MB RW section section no cached SDRAM
  L1_SECTIONS_ENTRY(    16,0x81000000,0x81000000          ,3,0,0,0),
  // 128 MB RW section section cached DCS1
  L1_SECTIONS_ENTRY(    128,0x90000000,0x90000000         ,3,0,1,1),
  // 128 MB RW section section no cached DCS1
  L1_SECTIONS_ENTRY(    128,0x98000000,0x98000000         ,3,0,0,0),
  // 64 MB RD section section cached WEIM CS0
  L1_SECTIONS_ENTRY(    64,0xA0000000,0xA0000000          ,3,0,1,0),
  // 64 MB RD section section no cached WEIM CS0
  L1_SECTIONS_ENTRY(    64,0xA4000000,0xA4000000          ,3,0,0,0),
  // 128 MB RD section section cached WEIM CS1
  L1_SECTIONS_ENTRY(   128,0xA8000000,0xA8000000          ,3,0,1,0),
  // 32 MB RW section section cached WEIM CS2
  L1_SECTIONS_ENTRY(   32,0xB0000000,0xB0000000           ,3,0,1,1),
  // 32 MB RW section section cached WEIM CS3
  L1_SECTIONS_ENTRY(   32,0xB2000000,0xB2000000           ,3,0,1,1),
  // 32 MB RW section section cached WEIM CS4
  L1_SECTIONS_ENTRY(   32,0xB4000000,0xB4000000           ,3,0,1,1),
  //   1 MB coarses table 3  0xB800_0000
  L1_COARSES_PAGE_ENTRY(  1,0xB8000000,(Int32U)L2Coarse3  ,  0    ),
  //   1 MB Fine table 1 0xBB00_0000
  L1_FINE_PAGE_ENTRY(  1,0xBB000000,(Int32U)L2Fine1       ,  0    ),
  TSB_INVALID,

// L2 coarses table1
  // 16KB 4*4 small pages R0 section cached (SDRAM)
  L2_CT_SMALL_PAGE_ENTRY( 4,0x00000000,(Int32U)__vector   ,0,0,0,0,1,1),
  TSB_INVALID,
  // L2 coarses table2
  // 20KB 5*4 small pages R0 section cached (BROM)
  L2_CT_SMALL_PAGE_ENTRY( 5,0x00404000,0x00404000         ,0,0,0,0,1,1),
  TSB_INVALID,
  // L2 coarses table3
  // 16KB 4*4 small page RW section no cached SDRAM, WEIM, M3IF, EMI
  L2_CT_SMALL_PAGE_ENTRY( 4,0xB8001000,0xB8001000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 fine table1
  // 5KB  5 tiny pages RW section no cached NAND
  L2_FT_TINY_PAGE_ENTRY(  5,0xBB000000,0xBB000000         ,3       ,0,0),
  // 1KB  5 tiny pages RW section no cached NAND
  L2_FT_TINY_PAGE_ENTRY(  1,0xBB001C00,0xBB001C00         ,3       ,0,0),
  TSB_INVALID,
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Coarse1,TableL2_Coarse},
  {L2Coarse2,TableL2_Coarse},
  {L2Coarse3,TableL2_Coarse},
  {L2Fine1  ,TableL2_Fine  },
  TTB_INVALID
};
