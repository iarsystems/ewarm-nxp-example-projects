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
 *    1. Date        : May, 14 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *        The physical memory map is:
 *                                                                      Instruction cache
 *                                                                        Data cache
 *                                                                          User Access
 *                                                                            Supervisor Access
 *                                                                              Read Only
 *  Segment                     Description     Size                    I D U S R
 *  0x0000 0000 – 0x0000 3FFF   BROM            16KB                    * - * * *
 *  0x0000 4000 – 0x0040 3FFF   Reserved        4 Mbyte                 - - - - -
 *  0x0040 0000 – 0x0040 5FFF   BROM            8Kbyte                  * - * * *
 *  0x0040 6000 – 0x007F FFFF   BROM (Hole)     3MB + 1000 Kbyte        - - - - -
 *  0x0080 0000 – 0x0FFF FFFF   Reserved        248 Mbyte               - - - - -
 *  0x1000 0000 – 0x1001 FFFF   AIPI1           128 Kbyte               - - - * -
 *  0x1002 0000 – 0x1003 FFFF   AIPI2           128 Kbyte               - - - * -
 *  0x1004 0000 – 0x1004 0FFF   AITC            4 Kbyte                 - - - * -
 *  0x1004 1000 – 0x1004 1FFF   Reserved        4 Kbyte                 - - - - -
 *  0x1004 2000 – 0x7FFF FFFF   Reserved        1791 Mbyte + 760 Kbyte  - - - - -
 *  0x8000 0000 – 0x8000 0FFF   CSI             4 Kbyte                 - - - * -
 *  0x8000 1000 – 0x9FFF FFFF   Reserved        512 Mbyte - 4 Kbyte     - - - - -
 *  0xA000 0000 - 0xA000 0FFF   BMI             4   kbyte               - - - * -
 *  0xA000 1000 - 0xBFFF FFFF   Reserved        512 Mbyte - 4 Kbyte     - - - - -
 *  0xC000 0000 - 0xC1FF FFFF   SDRAM(CSD0)     32  Mbyte               * * * * -
 *  0xC200 0000 - 0xC3FF FFFF   SDRAM(CSD0)     32  Mbyte               - - * * -
 *  0xC400 0000 – 0xC7FF FFFF   Reserved (CSD1) 64  Mbyte               - - - - -
 *  0xC800 0000 – 0xC9FF FFFF   NOR (CS0)       32  Mbyte               * * * * -
 *  0xCA00 0000 - 0xCBFF FFFF   Reserved (CS0)  32  Mbyte               - - - - -
 *  0xCC00 0000 – 0xCC00 03FF   Ether (CS1)     1   Kbyte               - - - * -
 *  0xCC00 1000 - 0xCC1F FFFF   Reserved (CS1)  2   Mbyte - 1Kbyte      - - - - -
 *  0xCC20 0000 – 0xCC20 03FF   DUART (CS1)     1   Kbyte               - - - * -
 *  0xCC20 1000 - 0xCC3F FFFF   Reserved (CS1)  2   Mbyte - 1Kbyte      - - - - -
 *  0xCC40 0000 – 0xCC40 03FF   Rev.  (CS1)     1   Kbyte               - - - * -
 *  0xCC40 1000 - 0xCC7F FFFF   Reserved (CS1)  4   Mbyte - 1Kbyte      - - - - -
 *  0xCC80 0000 – 0xCC80_03FF   I/O   (CS1)     1   Kbyte               - - - * -
 *  0xCC80 1000 - 0xCFFF FFFF   Reserved (CS1)  56  Mbyte - 1Kbyte      - - - - -
 *  0xD000 0000 – 0xD0FF FFFF   Reserved (CS2)  16  Mbyte               - - - - -
 *  0xD100 0000 – 0xD1FF FFFF   Reserved (CS3)  16  Mbyte               - - - - -
 *  0xD200 0000 – 0xD2FF FFFF   Reserved (CS4)  16  Mbyte               - - - - -
 *  0xD300 0000 – 0xD3FF FFFF   Reserved (CS5)  16  Mbyte               - - - - -
 *  0xD400 0000 – 0xD7FF FFFF   PCMCIA Reserved 64  Mbyte               - - - - -
 *  0xD800 0000 – 0xDEFF FFFF   Reserved        112 Mbyte               - - - - -
 *  0xDF00 0000 – 0xDF00 0FFF   SDRAMC          4   Kbyte               - - - * -
 *  0xDF00 1000 – 0xDF00 1FFF   EIM             4   Kbyte               - - - * -
 *  0xDF00 2000 – 0xDF00 2FFF   PCMCIA          4   Kbyte               - - - * -
 *  0xDF00 3000 – 0xDF00 3FFF   NANDFC          4   Kbyte               - - - * -
 *  0xDF00 4000 – 0xDFFF FFFF   Reserved        16  Mbyte - 16Kbyte     - - - - -
 *  0xE000 0000 – 0xFFFF E7FF   Reserved        16  Mbyte - 1 Kbyte     - - - - -
 *  0xFFFF E800 – 0xFFFF FFFF   6 Kbyte VRAM    6   Kbyte               * * * * -
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
#pragma data_alignment=2048
__no_init Int32U L2Coarse6[L2_CP_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine1[L2_FP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine2[L2_FP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine3[L2_FP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine4[L2_FP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine5[L2_FP_ENTRIES_NUMB];

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
  //   1 MB coarses table 5 0xA0000000
  L1_COARSES_PAGE_ENTRY(  1,0xA0000000,(Int32U)L2Coarse5  ,  0    ),
  // 32 MB RW section cached (SDRAM)
  L1_SECTIONS_ENTRY(     32,0xC0000000,0xC0000000         ,3,0,1,1),
  // 32 MB RW section no cached (SDRAM)
  L1_SECTIONS_ENTRY(     32,0xC2000000,0xC2000000         ,3,0,0,0),
  // 32 MB RO section cached NOR (CS0)
  L1_SECTIONS_ENTRY(     32,0xC8000000,0xC8000000         ,3,0,1,0),
  //   1 MB Fine table 1 0xCC00 0000
  L1_FINE_PAGE_ENTRY(  1,0xCC000000,(Int32U)L2Fine1       ,  0    ),
  //   1 MB Fine table 2 0xCC20 0000
  L1_FINE_PAGE_ENTRY(  1,0xCC200000,(Int32U)L2Fine2       ,  0    ),
  //   1 MB Fine table 3 0xCC40 0000
  L1_FINE_PAGE_ENTRY(  1,0xCC400000,(Int32U)L2Fine3       ,  0    ),
  //   1 MB Fine table 4 0xCC80 0000
  L1_FINE_PAGE_ENTRY(  1,0xCC800000,(Int32U)L2Fine4       ,  0    ),
  //   1 MB coarses table 6 0xDF00 0000
  L1_COARSES_PAGE_ENTRY(  1,0xDF000000,(Int32U)L2Coarse6  ,  0    ),
  //   1 MB Fine table 5 0xFFE0 0000
  L1_FINE_PAGE_ENTRY(     1,0xFFF00000,(Int32U)L2Fine5    ,  0    ),
  TSB_INVALID,

  // L2 coarses table1
  // 16KB 4*4 small pages R0 section cached (BROM)
  L2_CT_SMALL_PAGE_ENTRY( 4,0x00000000,0x00000000         ,0,0,0,0,1,0),
  TSB_INVALID,
  // L2 coarses table2
  // 8KB 2*4 small pages R0 section cached (BROM)
  L2_CT_SMALL_PAGE_ENTRY( 2,0x00404000,0x00404000         ,0,0,0,0,1,0),
  TSB_INVALID,
  // L2 coarses table3
  // 256KB 4 large pages RW section no cached AIPI1,AIPI2
  L2_CT_LARGE_PAGE_ENTRY( 4,0x10000000,0x10000000         ,2,2,2,2,0,0),
  // 4KB 1 small page RW section no cached AITC
  L2_CT_SMALL_PAGE_ENTRY( 1,0x10040000,0x10040000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 coarses table4
  // 4KB 1 small pages RW section no cached CSI
  L2_CT_SMALL_PAGE_ENTRY( 1,0x80000000,0x80000000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 coarses table5
  // 4KB 1 small pages RW section no cached BMI
  L2_CT_SMALL_PAGE_ENTRY( 1,0xA0000000,0xA0000000         ,2,2,2,2,0,0),
  TSB_INVALID,
  // L2 coarses table6
  // 16KB 4 small pages RW section no cached SDRAMC, EIM, PCMCIA, NANDFC
  L2_CT_SMALL_PAGE_ENTRY( 4,0xDF000000,0xDF000000         ,2,2,2,2,0,0),
  TSB_INVALID,

  // L2 fine table1
  // 1KB 1 tiny pages RW section no cached Ether (CS1)
  L2_FT_TINY_PAGE_ENTRY( 1,0xCC000000,0xCC000000         ,2       ,0,0),
  TSB_INVALID,

  // L2 fine table2
  // 1KB 1 tiny pages RW section no cached DUART (CS1)
  L2_FT_TINY_PAGE_ENTRY( 1,0xCC200000,0xCC200000         ,2       ,0,0),
  TSB_INVALID,

  // L2 fine table3
  // 1KB 1 tiny pages RW section no cached Rev. (CS1)
  L2_FT_TINY_PAGE_ENTRY( 1,0xCC400000,0xCC400000         ,2       ,0,0),
  TSB_INVALID,

  // L2 fine table4
  // 1KB 1 tiny pages RW section no cached I/O (CS1)
  L2_FT_TINY_PAGE_ENTRY( 1,0xCC800000,0xCC800000         ,2       ,0,0),
  TSB_INVALID,

  // L2 fine table4
  // 6KB 6 tiny pages RW section  cached VRAM
  L2_FT_TINY_PAGE_ENTRY( 6,0xFFFFE800,0xFFFFE800         ,3       ,1,1),
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
  {L2Coarse6,TableL2_Coarse},
  {L2Fine1  ,TableL2_Fine  },
  {L2Fine2  ,TableL2_Fine  },
  {L2Fine3  ,TableL2_Fine  },
  {L2Fine4  ,TableL2_Fine  },
  {L2Fine5  ,TableL2_Fine  },
  TTB_INVALID
};
