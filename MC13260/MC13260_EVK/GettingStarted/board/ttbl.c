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
 *    1. Date        : July, 22 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *        The physical memory map is:
 *                                                                      Instruction cache
 *                                                                        Data cache
 *                                                                          User Access
 *                                                                            Supervisor Access
*                                                                               Read Only
 *  Segment                     Description     Size                    I D U S R
 *  0x0000_0000 - 0x0000_3FFF   ROM             16KB                    * - * * * coarses 1M
 *  0x0000 4000 – 0x0040 3FFF   Reserved        4 Mbyte                 - - - - -
 *  0x0040 4000 – 0x0040_5FFF   ROM             8Kbyte                  * - * * * coarses 1M
 *  0x0040_6000 – 0x007F_FFFF   Reserved        <4 Mbyte                - - - - -
 *  0x0080 0000 – 0x0089_FFFF   System RAM      640 Kbytes              * * * * -
 *  0x008A 0000 – 0x00FF_FFFF   Reserved        7552 Kbytes             - - - - -
 *  0x0100 0000 – 0x0100_3FFF   AIPS            16 Kbytes               - - - * -
 *  0x0100_4000 – 0x0100_7FFF   MAX             16 Kbytes               - - - * -
 *  0x0100_8000 – 0x0100_BFFF   CLKCTL          16 Kbytes               - - - * -
 *  0x0100_C000 – 0x0100_FFFF   ETB             16 Kbytes               - - - * -
 *  0x0101_0000 – 0x0101_3FFF   ETB Memory      16 Kbytes               - - - * -
 *  0x0101_4000 – 0x0101_7FFF   ETM             16 Kbytes               - - - * -
 *  0x0101_8000 – 0x0101_BFFF   AAPE            16 Kbytes               - - - * -
 *  0x0101_C000 – 0x0107_FFFF   Reserved        400 Kbytes              - - - - -
 *  0x0108_0000 – 0x0108_3FFF   Modem           16 Kbytes               - - - * -
 *  0x0108_4000 – 0x0108_7FFF   GPADC           16 Kbytes               - - - * -
 *  0x0108_8000 – 0x0108_BFFF   SLCDC           16 Kbytes               - - - * -
 *  0x0108_C000 – 0x0108_FFFF   SRC             16 Kbytes               - - - * -
 *  0x0109_0000 – 0x0109_3FFF   HDMA            16 Kbytes               - - - * -
 *  0x0109_4000 – 0x0109_7FFF   CCM             16 Kbytes               - - - * -
 *  0x0109_8000 – 0x0109_BFFF   ASM             16 Kbytes               - - - * -
 *  0x0109_C000 – 0x0109_FFFF   DPLL            16 Kbytes               - - - * -
 *  0x010A_0000 – 0x010A_3FFF   WDOG            16 Kbytes               - - - * -
 *  0x010A_4000 – 0x010A_7FFF   RTC             16 Kbytes               - - - * -
 *  0x010A_8000 - 0x010A_BFFF   DSM             16 Kbytes               - - - * -
 *  0x010A_C000 - 0x010A_FFFF   KPP             16 Kbytes               - - - * -
 *  0x010B_0000 - 0x010B_3FFF   CSPI            16 Kbytes               - - - * -
 *  0x010B_4000 - 0x010B_7FFF   SSI             16 Kbytes               - - - * -
 *  0x010B_8000 - 0x010B_BFFF   UART1           16 Kbytes               - - - * -
 *  0x010B_C000 - 0x010B_FFFF   UART2           16 Kbytes               - - - * -
 *  0x010C_0000 - 0x010C_3FFF   USB_FS          16 Kbytes               - - - * -
 *  0x010C_4000 - 0x010C_7FFF   USB RAM         16 Kbytes               - - - * -
 *  0x010C_8000 - 0x010C_BFFF   I2C             16 Kbytes               - - - * -
 *  0x010C_C000 - 0x010C_FFFF   IOMUX           16 Kbytes               - - - * -
 *  0x010D_0000 - 0x010D_3FFF   GPT             16 Kbytes               - - - * -
 *  0x010D_4000 - 0x010D_7FFF   TMR             16 Kbytes               - - - * -
 *  0x010D_8000 - 0x010D_BFFF   EPIT            16 Kbytes               - - - * -
 *  0x010D_C000 - 0x010D_FFFF   GPIO            16 Kbytes               - - - * -
 *  0x010E_0000 - 0x010E_3FFF   RNGB            16 Kbytes               - - - * -
 *  0x010E_4000 - 0x010E_7FFF   ROMCP           16 Kbytes               - - - * -
 *  0x010E_8000 - 0x010E_BFFF   PWM1            16 Kbytes               - - - * -
 *  0x010E_C000 - 0x010E_FFFF   PWM2            16 Kbytes               - - - * -
 *  0x010F_0000 - 0x010F_3FFF   TPRC1           16 Kbytes               - - - * -
 *  0x010F_4000 - 0x010F_7FFF   TSM             16 Kbytes               - - - * -
 *  0x010F_8000 - 0x010F_BFFF   IIM             16 Kbytes               - - - * -
 *  0x010F_C000 - 0x010F_FFFF   TPRC2           16 Kbytes               - - - * -
 *  0x0110_0000 - 0x017F_FFFF   en0 TPRC3       7 Mbytes                - - - * -
 *  0x0180_0000 - 0x0187_FFFF   ASIC            512 Kbytes              - - - * -
 *  0x0188_0000 - 0x01FF_FFFF   Reserved        7.5 Mbytes              - - - - -
 *  0x0200_0000 - 0x027F_FFFF   RFDI            8 Mbytes                - - - * - 
 *  0x0280_0000 - 0x02FF_FFFF   Voice CODEC     8 Mbytes                - - - * -
 *  0x0300_0000 - 0xFFFF_FFFF   Reserved        <4 Gbytes               - - - - -
 *
 *  Domain 0 - 01 Client
 *  Domain 1 - 11 Manager
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "ttbl.h"

#pragma segment="MMU_TT_L1"
#pragma segment="MMU_TT_L2"

/* 
  Be careful!!! L1Table[] array memory (16384 bytes) is reserved in the
  linker (.icf) file. It overlaps SPI flash boot-header which is linked and 
  used by the flash loader but is not used in the application code. Do not use 
  this file in other applications without specifying the array capacity!
*/

#pragma location="MMU_TT_L1"
#pragma data_alignment=16384
__no_init Int32U L1Table[/*L1_ENTRIES_NUMB*/];

#pragma location="MMU_TT_L2"
#pragma data_alignment=4096
__no_init Int32U L2Fine1[L2_FP_ENTRIES_NUMB];

extern __arm void __intvec_start(void);
extern __arm void __vector(void);

const TtSectionBlock_t TtSB[] =
{
 // L1 table
  //  1 MB coarses table 1  0x0000 0000
  L1_FINE_PAGE_ENTRY(     1,0x00000000,(Int32U)L2Fine1   ,  0    ),
  //  7 MB RW section entry
  L1_SECTIONS_ENTRY(      7,0x00100000,0x00100000        ,1,0,0,0),
  //  8 MB RW section entry cached (System RAM 640KB up to 2MB + 6MB gap)
  L1_SECTIONS_ENTRY(      8,0x00800000,0x00800000        ,3,0,1,1),
  // 16 MB RW section entry no cached (SFR)
  L1_SECTIONS_ENTRY(     16,0x01000000,0x01000000        ,1,0,0,0),
  // 16 MB RW section entry no cached (SFR)
  L1_SECTIONS_ENTRY(     16,0x02000000,0x02000000        ,1,0,0,0),     
  TSB_INVALID,

// L2 fine table1
  // 1KB 1 tiny page RW section cached (RAM)
  L2_FT_TINY_PAGE_ENTRY(  1,0x00000000,(Int32U)__vector  ,3  ,1,1),
  TSB_INVALID,
  
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1}, 
  {L2Fine1  ,TableL2_Fine  },
  TTB_INVALID
};
