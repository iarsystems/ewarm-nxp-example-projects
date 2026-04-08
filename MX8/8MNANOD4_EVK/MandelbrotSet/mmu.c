/*******************************************************************************
                                Copyright 2020 IAR Systems. All rights reserved.
Function: i.MX8M Nano Applications Processor

Note: Evaluation Kit for the i.MX 8M Nano(4Kbyte granule)

History:
 No  Rev  Date       Name           Note
---+-----+----------+-------------+--------------------------------------------
000 00.00 2020/07/15 S.Tonoshita    New
*******************************************************************************/
#define __MMU_C__

/******************************************************************************/
#include <stdint.h>
#include <intrinsics.h>
#include "mmu.h"

/******************************************************************************/
#define L0_TRANSLATION_TABLE_SIZE   (512)               //
#define L1_TRANSLATION_TABLE_SIZE   (512)               //
#define L2_TRANSLATION_TABLE_SIZE   (512)               //

//
// Next-level attributes in stage 1 VMSAv8-64 Table descriptors
//
#define TLB_DESC_NSTABLE_MASK   (0x8000000000000000ull) //
#define TLB_DESC_NSTABLE_SHIFT  (63)                    //
#define TLB_DESC_APTABLE_MASK   (0x6000000000000000ull) //
#define TLB_DESC_APTABLE_SHIFT  (61)                    //
#define TLB_DESC_UXNTABLE_MASK  (0x1000000000000000ull) //
#define TLB_DESC_UXNTABLE_SHIFT (60)                    //
#define TLB_DESC_XNTABLE_MASK   (0x1000000000000000ull) //
#define TLB_DESC_XNTABLE_SHIFT  (60)                    //
#define TLB_DESC_PXNTABLE_MASK  (0x0800000000000000ull) //
#define TLB_DESC_PXNTABLE_SHIFT (59)                    //

//
// Attribute fields in stage 1 VMSAv8-64 Block and Page descriptors
//
#define TRANS_UNX_XN_SHIFT      (54)                    //
#define TRANS_UNX_XN_MASK       (0x0040000000000000ull) //
#define TRANS_PXN_SHIFT         (53)                    //
#define TRANS_PXN_MASK          (0x0020000000000000ull) //
#define TRANS_DBM_SHIFT         (51)                    //
#define TRANS_DBM_MASK          (0x0008000000000000ull) //

#define TRANS_NT_SHIFT          (16)                    //
#define TRANS_NT_MASK           (0x0000000000010000ull) //
#define TRANS_NG_SHIFT          (11)                    //
#define TRANS_NG_MASK           (0x0000000000000800ull) //
#define TRANS_AF_SHIFT          (10)                    //
#define TRANS_AF_MASK           (0x0000000000000400ull) //
#define TRANS_SH_SHIFT          (8)                     //
#define TRANS_SH_MASK           (0x0000000000000300ull) //
#define TRANS_AP_SHIFT          (6)                     //
#define TRANS_AP_MASK           (0x00000000000000C0ull) //
#define TRANS_NS_SHIFT          (5)                     //
#define TRANS_NS_MASK           (0x0000000000000020ull) //
#define TRANS_ATTRINDEX_SHIFT   (2)                     //
#define TRANS_ATTRINDEX_MASK    (0x000000000000001Cull) //

#define TRANS_ATTR_FAULT        (0ull << 0)             // Level 0/1/2
#define TRANS_ATTR_BLOCK        (1ull << 0)             // Level 1/2
#define TRANS_ATTR_TABLE        (3ull << 0)             // Level 0/1/2
#define TRANS_ATTR_PAGE         (3ull << 0)             // Level 3

/******************************************************************************/
#pragma data_alignment = 0x1000
#pragma location="TLB"
__root __no_init uint64_t l0_translation_table[L0_TRANSLATION_TABLE_SIZE];
#pragma data_alignment = 0x1000
#pragma location="TLB"
__root __no_init uint64_t l1_translation_table[L1_TRANSLATION_TABLE_SIZE];
#pragma data_alignment = 0x1000
#pragma location="TLB"
__root __no_init uint64_t l2_0_translation_table[L2_TRANSLATION_TABLE_SIZE];

/******************************************************************************/
void init_mmu( void )
{
    uint32_t i;
    uint64_t addr;

// No.0   V:0000_0000_0000_0000
// No.1   V:0000_0000_4000_0000
// ...
// No.510 V:0000_007F_8000_0000
// No 511 V:0000_007F_C000_0000

    for(i=0;i<L0_TRANSLATION_TABLE_SIZE;i++){           // Level0 all Invalid entry
        l0_translation_table[i] = 0x0ull;
    }

    // Table: 0x0-0x3FFF_FFFF
    l0_translation_table[0] = (uint64_t)l1_translation_table | TRANS_ATTR_TABLE;

    addr = 0x0;

    for(i=0;i<L1_TRANSLATION_TABLE_SIZE;i++){           // Level1 all Memory Block 2M  UXN=1 AF=1
        l1_translation_table[i] = (1ull << TRANS_UNX_XN_SHIFT) | (1ull << TRANS_AF_SHIFT) | TRANS_ATTR_BLOCK | addr;
        addr += 0x0000000000200000ull;                  // Next page address
    }

    // Table: 0x80_0000-9F_FFFF
    l1_translation_table[4] = (uint64_t)l2_0_translation_table | TRANS_ATTR_TABLE;

    addr = 0x800000;
    for(i=0;i<L2_TRANSLATION_TABLE_SIZE;i++){           // Level2
        l2_0_translation_table[i] =  (1ull << TRANS_AF_SHIFT) | (1ull << TRANS_AP_SHIFT) | (0ull << TRANS_ATTRINDEX_SHIFT) | TRANS_ATTR_PAGE | addr;
        addr += 0x0000000000001000ull;                  // Next page address
    }
}
