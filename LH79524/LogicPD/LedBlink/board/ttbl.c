/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : ttbl.c
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : May, 18 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "ttbl.h"

#pragma segment="MMU_TT"

#pragma location="MMU_TT"
#pragma data_alignment=16384
__no_init Int32U L1Table[L1_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse[L2_CP_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse1[L2_CP_ENTRIES_NUMB];

const TtSectionBlock_t TtSB[] =
{
  // L1
  L1_COARSES_PAGE_ENTRY(1,0,(Int32U)L2Coarse,0),          // 1MB Flash
  L1_SECTIONS_ENTRY(16,0x20000000,0x20000000,1,1,1),      // 16MB SDRAM
  L1_SECTIONS_ENTRY(16,0x21000000,0x21000000,1,0,0),      // 16MB SDRAM
  L1_SECTIONS_ENTRY(1 ,0x44000000,0x44000000,0,1,0),      // nCS1 Flash
  L1_SECTIONS_ENTRY(32,0x48000000,0x48000000,1,0,0),      // nCS2 Slow periphery
  L1_SECTIONS_ENTRY(32,0x4C000000,0x4C000000,1,0,0),      // nCS3 Fast periphery
  L1_COARSES_PAGE_ENTRY(1,0x60000000,(Int32U)L2Coarse1,1),// iRam
  L1_SECTIONS_ENTRY(1 ,0xFFF00000,0xFFF00000,1,0,0),      // SFR
  TSB_INVALID,
  // L2 1
  L2_CT_LARGE_PAGE_ENTRY(8,0x00000000,0x00000000,0,0,0,0,1,0),  // Flash 1/2
  L2_CT_LARGE_PAGE_ENTRY(8,0x00080000,0x00080000,0,0,0,0,0,0),  // Flash 1/2
  TSB_INVALID,
  // L2 2
  L2_CT_SMALL_PAGE_ENTRY(4,0x60000000,0x60000000,3,3,3,3,1,1),  // iRam
  TSB_INVALID
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Coarse,TableL2_Coarse},
  {L2Coarse1,TableL2_Coarse},
  TTB_INVALID
};

