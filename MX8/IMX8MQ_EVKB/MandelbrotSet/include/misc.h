/*******************************************************************************
                                Copyright 2020 IAR Systems. All rights reserved.
Function: Cortex-A53(AARCH64)

Note:

History:
 +-------------- No(000 .. 999)
 |    +--------- Version
 |    |   +----- New,Change,Add,delete
 v    v   v
 No  Ver  Category    Date      Name        Description
---+-----+--------+----------+-------------+------------------------------------
000 00.00 New      2020/07/15 S.Tonoshita   New development
*******************************************************************************/
#ifndef __MISC_H__
#define __MISC_H__

/******************************************************************************/
#ifndef __MISC_C__
    extern void enable_caches_el1(void);
    extern void disable_caches_el1(void);
    extern void enable_caches_el2(void);
    extern void disable_caches_el2(void);
    extern void enable_caches_el3(void);
    extern void disable_caches_el3(void);
    extern void invalidate_udcaches(void);
    extern int zero_block(void *blockPtr,unsigned int nBytes);
    extern uint32_t get_midr(void);
    extern uint64_t get_mpidr(void);
    extern void set_ttbr0_el1(uint64_t addr);
    extern void set_ttbr1_el1(uint64_t addr);
    extern uint64_t get_tcr_el1(void);
    extern void set_tcr_el1(uint64_t addr);
#endif  // __MISC_C__

#endif  // __MISC_H__
