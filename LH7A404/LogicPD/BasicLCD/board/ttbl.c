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
 *  Notes: The Translation table is correct only for:
 *          Flash - 16MB (2 x 4MB x 16bit         devices)
 *          SDRAM - 64MB (2 x 4MB x 4Bank x 16Bit devices)
 *      and
 *        SDCSC1:SROMLL  = 0
 *        SDCSC1:2KPAGE  = 0
 *        SDCSC1:SROM512 = 0
 *
 *        The physical memory map is:
 *          (before the translation)
 *      0x00000000
 *                  |-----------|
 *                  |           |
 *                  | FLASH 16MB|
 *      0x00FFFFFF  |           |
 *                  |-----------|
 *                  ~           ~
 *      0xC0000000  |           |
 *                  |-----------|
 *      0xC03FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xC1000000  |           |
 *                  |-----------|
 *      0xC13FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xC4000000  |           |
 *                  |-----------|
 *      0xC43FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xC5000000  |           |
 *                  |-----------|
 *      0xC53FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xC8000000  |           |
 *                  |-----------|
 *      0xC83FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xC9000000  |           |
 *                  |-----------|
 *      0xC93FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xCC000000  |           |
 *                  |-----------|
 *      0xCC3FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xCD000000  |           |
 *                  |-----------|
 *      0xCD3FFFFF  | SDRAM 4MB |
 *                  |-----------|
 *                  ~           ~
 *      0xFFFFFFFF  |           |
 *                  |-----------|
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
__no_init Int32U L2Fine[L2_FP_ENTRIES_NUMB];

const TtSectionBlock_t TtSB[] =
{
  // L1
  L1_SECTIONS_ENTRY( 8,0x00000000,0x00000000,0,1,0),           // 8MB Flash
  L1_SECTIONS_ENTRY( 8,0x00800000,0x00800000,0,0,0),           // 8MB Flash
  L1_SECTIONS_ENTRY(64,0x60000000,0x60000000,1,0,0),           // nCS6 - CPLD Slow periphery
  L1_SECTIONS_ENTRY(64,0x70000000,0x70000000,1,0,0),           // nCS7 - CPLD Fast periphery
  L1_SECTIONS_ENTRY( 1,0x80000000,0x80000000,1,0,0),           // SFR
  L1_FINE_PAGE_ENTRY(1,0xB0000000,(Int32U)L2Fine,1),           // eSRAM 80KB
  // 16MB SDRAM not cached
  L1_SECTIONS_ENTRY( 4,0xC0000000,0xC0000000,1,0,0),           // 4MB segment
  L1_SECTIONS_ENTRY( 4,0xC0400000,0xC1000000,1,0,0),           // 4MB segment
  L1_SECTIONS_ENTRY( 4,0xC0800000,0xC4000000,1,0,0),           // 4MB segment
  L1_SECTIONS_ENTRY( 4,0xC0C00000,0xC5000000,1,0,0),           // 4MB segment
  // 16MB SDRAM cached
  L1_SECTIONS_ENTRY( 4,0xC1000000,0xC8000000,1,1,1),           // 4Ì segment
  L1_SECTIONS_ENTRY( 4,0xC1400000,0xC9000000,1,1,1),           // 4Ì segment
  L1_SECTIONS_ENTRY( 4,0xC1800000,0xCC000000,1,1,1),           // 4Ì segment
  L1_SECTIONS_ENTRY( 4,0xC1C00000,0xCD000000,1,1,1),           // 4Ì segment

  TSB_INVALID,
  // L2 1
  L2_FT_TINY_PAGE_ENTRY(80,0xB0000000,0xB0000000,3,1,1),       // iRam
  TSB_INVALID
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Fine,TableL2_Fine},
  TTB_INVALID
};
