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
 *  0x0000 0000 – 0x0000 3FFF   BROM            16KB                    * - * * *
 *  0x0000 4000 – 0x0040 3FFF   Reserved        4 Mbyte                 - - - - -
 *  0x0040 4000 – 0x0040 5FFF   BROM            8Kbyte                  * - * * *
 *  0x0040 6000 – 0x007F FFFF   BROM (Hole)     3MB + 1000 Kbyte        - - - - -
 *  0x0080 0000 – 0x0FFF FFFF   Reserved        248 Mbyte               - - - - -
 *  0x1000 0000 – 0x1001 FFFF   AIPI1           128 Kbyte               - - - * -
 *  0x1002 0000 – 0x1003 FFFF   AIPI2           128 Kbyte               - - - * -
 *  0x1004 0000 – 0x1004 0FFF   AITC            4 Kbyte                 - - - * -
 *  0x1004 1000 – 0x1004 1FFF   Reserved        4 Kbyte                 - - - - -
 *  0x1004 2000 – 0x7FFF FFFF   Reserved        1791 Mbyte + 760 Kbyte  - - - - -
 *  0x8000 0000 – 0x8000 0FFF   CSI             4 Kbyte                 - - - * -
 *  0x8000 1000 – 0x8000 1FFF   ATA             4 Kbyte                 - - - * -
 *  0x8000 2000 – 0x9FFF FFFF   Reserved        512 Mbyte - 8 Kbyte     - - - - -
 *  0xA000 0000 - 0xA3FF FFFF   DDRRAM (CSD0)   64 Mbyte                * * * * -
 *  0xA400 0000 - 0xA7FF FFFF   DDRRAM (CSD0)   64 Mbyte                - - * * -
 *  0xA800 0000 - 0xAFFF FFFF   Reserved        128 Mbyte               - - - - -
 *  0xB000 0000 – 0xBFFF FFFF   Reserved (CSD1) 256 Mbyte               - - - - -
 *  0xC000 0000 - 0xC1FF FFFF   NOR             32  Mbyte               * - * * *
 *  0xC200 0000 – 0xC7FF FFFF   Reserved (CS0)  96  Mbyte               - - - - -
 *  0xC800 0000 – 0xCFFF FFFF   Reserved (CS1)  128 Mbyte               - - - - -
 *  0xD000 0000 – 0xD1FF FFFF   Reserved (CS2)  32 Mbyte                - - - - -
 *  0xD200 0000 – 0xD3FF FFFF   Reserved (CS3)  32 Mbyte                - - - - -
 *  0xD400 0000 – 0xD5FF FFFF   CPLD (CS4)      32 Mbyte                - - - * -
 *  0xD600 0000 - 0xD6FF FFFF   PSRAM (CS5)     16 Mbyte                * * * * -
 *  0xD700 0000 – 0xD7FF FFFF   Reserved (CS5)  16 Mbyte                - - - - -
 *  0xD800 0000 – 0xD800 FFFF   NANDFC          4 Kbyte                 - - - * -
 *  0xD800 1000 – 0xD800 1FFF   SDRAMC          4 Kbyte                 - - - * -
 *  0xD800 2000 – 0xD800 2FFF   EIM             4 Kbyte                 - - - * -
 *  0xD800 3000 – 0xD800 3FFF   M3IF            4 Kbyte                 - - - * -
 *  0xD800 4000 – 0xD800 4FFF   PCMCIA          4 Kbyte                 - - - * -
 *  0xD800 5000 – 0xDBFF FFFF   Reserved        64 M - 20 Kbyte         - - - - -
 *  0xDC00 0000 – 0xDFFF FFFF   PCMCIA Memory   64 Mbyte                - - * * -
 *  0xE000 0000 – 0xFFEF FFFF   Reserved        511 Mbyte               - - - - -
 *  0xFFF0 0000 – 0xFFFF 4BFF   VRAM Not Use    979 Kbyte               - - - - -
 *  0xFFFF 4C00 – 0xFFFF FFFF   45 Kbyte VRAM   45 Kbyte                * * * * -
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
#pragma data_alignment=2048
__no_init Int32U L2Coarse4[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse5[L2_CP_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine1[L2_FP_ENTRIES_NUMB];

const TtSectionBlock_t TtSB[] =
{
  // L1
  //   1 MB coarses table 1 0x0000 0000
  L1_COARSES_PAGE_ENTRY(  1,0x00000000,(Int32U)L2Coarse1  ,  0    ),
  //   1 MB coarses table 2 0x0040 0000
  L1_COARSES_PAGE_ENTRY(  1,0x00400000,(Int32U)L2Coarse2  ,  0    ),
  //   1 MB coarses table 3 0x1000 0000
  L1_COARSES_PAGE_ENTRY(  1,0x10000000,(Int32U)L2Coarse3  ,  0    ),
  //   1 MB coarses table 4 0x8000 0000
  L1_COARSES_PAGE_ENTRY(  1,0x80000000,(Int32U)L2Coarse4  ,  0    ),
  // 64 MB RW section cached (DDRRAM)
  L1_SECTIONS_ENTRY(     64,0xA0000000,0xA0000000         ,3,0,1,1),
  // 64 MB RW section no cached (DDRRAM)
  L1_SECTIONS_ENTRY(     64,0xA4000000,0xA4000000         ,3,0,0,0),
  // 32 MB RO section cached (NOR)
  L1_SECTIONS_ENTRY(     32,0xC0000000,0xC0000000         ,3,0,1,0),
  // 32 MB RW section no cached (CPLD)
  L1_SECTIONS_ENTRY(     32,0xD4000000,0xD4000000         ,3,0,0,0),
  // 8 MB RW section cached (PSRAM)
  L1_SECTIONS_ENTRY(      8,0xD6000000,0xD6000000         ,3,0,1,1),
  // 8 MB RW section no cached (PSRAM)
  L1_SECTIONS_ENTRY(      8,0xD6800000,0xD6800000         ,3,0,0,0),
  //   1 MB coarses table 5 0xD800 0000
  L1_COARSES_PAGE_ENTRY(  1,0xD8000000,(Int32U)L2Coarse5  ,  0    ),
  // 64 MB RW section no cached (PCMCIA)
  L1_SECTIONS_ENTRY(     16,0xDC000000,0xDC000000         ,3,0,0,0),
  //   1 MB Fine table 1 0xFFF0 0000
  L1_FINE_PAGE_ENTRY(  1,0xFFF00000,(Int32U)L2Fine1       ,  0    ),
  TSB_INVALID,
  // L2 coarses table1
  // 16KB 4*4 small pages R0 section cached (BROM)
  L2_CT_SMALL_PAGE_ENTRY( 4,0x00000000,0x00000000         ,0,0,0,0,1,1),
  TSB_INVALID,
  // L2 coarses table2
  // 8KB 2*4 small pages R0 section cached (BROM)
  L2_CT_SMALL_PAGE_ENTRY( 2,0x00404000,0x00404000         ,0,0,0,0,1,1),
  TSB_INVALID,
  // L2 coarses table3
  // 256KB 4 large pages RW section no cached AIPI1,AIPI2
  L2_CT_LARGE_PAGE_ENTRY( 4,0x10000000,0x10000000         ,2,2,2,2,0,0),
  // 4KB 1 small page RW section no cached AITC
  L2_CT_SMALL_PAGE_ENTRY( 1,0x10040000,0x10040000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 coarses table4
  // 8KB 2 small pages RW section no cached CSI,ATA
  L2_CT_SMALL_PAGE_ENTRY( 2,0x80000000,0x80000000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 coarses table5
  // 20KB 5 large pages RW section no cached NANDFC,SDRAMC,EIM,M3IF,PCMCIA
  L2_CT_SMALL_PAGE_ENTRY( 5,0xD8000000,0xD8000000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 fine table1
  // 45KB 45 tiny pages RW section cached VRAM
  L2_FT_TINY_PAGE_ENTRY( 45,0xFFFF4C00,0xFFFF4C00         ,3       ,1,1),
  TSB_INVALID,
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Coarse1,TableL2_Coarse},
  {L2Coarse2,TableL2_Coarse},
  {L2Coarse3,TableL2_Coarse},
  {L2Coarse4,TableL2_Coarse},
  {L2Coarse5,TableL2_Coarse},
  {L2Fine1  ,TableL2_Fine  },
  TTB_INVALID
};
