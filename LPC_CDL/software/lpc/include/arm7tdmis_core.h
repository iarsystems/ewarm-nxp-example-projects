#ifndef ARM7TDMIS_CORE_H_
#define ARM7TDMIS_CORE_H_

#include "arm7tdmis_types.h"

/*****************************************************************************************************
 *                                 Register Abstraction Layer                                        *
 *****************************************************************************************************/

/***********************************************************************
 * ARM Hard Vector Address Locations
 **********************************************************************/

#define ARM_RESET_VEC   0x00 /* ARM reset vector address */
#define ARM_UNDEF_VEC   0x04 /* ARM undefined vector address */
#define ARM_SWI_VEC     0x08 /* ARM software interrupt vector address */
#define ARM_PABORT_VEC  0x0C /* ARM prefetch abort vector address */
#define ARM_DABORT_VEC  0x10 /* ARM data abort vector address */
#define ARM_RSVD_VEC    0x14 /* Reserved */
#define ARM_IRQ_VEC     0x18 /* ARM IRQ vector address */
#define ARM_FIQ_VEC     0x1C /* ARM FIQ vector address */

/***********************************************************************
 * ARM Current and Saved Processor Status Register Bits (xPSR)
 **********************************************************************/
/* ARM Condition Code Flag Bits (xPSR bits [31:27]) */
#define ARM_CCFLG_N     0x80000000 /* ARM negative flag */
#define ARM_CCFLG_Z     0x40000000 /* ARM zero flag */
#define ARM_CCFLG_C     0x20000000 /* ARM carry flag */
#define ARM_CCFLG_V     0x10000000 /* ARM overflow flag */
#define ARM_CCFLG_Q     0x08000000 /* ARM 'Q' flag */

/* ARM Interrupt Disable Bits (xPSR bits [7:6]) */
#define ARM_IRQ         0x80 /* ARM IRQ mode */
#define ARM_FIQ         0x40 /* ARM FIQ mode */

/* ARM Thumb State Bit (xPSR bit [5]) */
#define ARM_THUMB       0x20 /* ARM thumb mode */

/* ARM Processor Mode Values (xPSR bits [4:0])
 * Use ARM_MODE macro and constants to test ARM mode
 * Example, where tmp has xPSR value:
 * if (ARM_MODE(tmp, ARM_MODE_IRQ))
 *     statement; */

/* ARM mode selection macro */
#define ARM_MODE(m,n)   (((m) & _BITMASK(5)) == (n))
#define ARM_MODE_USR    0x10 /* ARM user mode */
#define ARM_MODE_FIQ    0x11 /* ARM FIQ mode */
#define ARM_MODE_IRQ    0x12 /* ARM IRQ mode */
#define ARM_MODE_SVC    0x13 /* ARM service mode */
#define ARM_MODE_ABT    0x17 /* ARM abort mode */
#define ARM_MODE_UND    0x1B /* ARM undefined mode */
#define ARM_MODE_SYS    0x1F /* ARM system mode */


/*****************************************************************************************************
 *                                 Hardware Abstraction Layer                                        *
 *****************************************************************************************************/

#if defined (__ICCARM__)
  #define __enable_irq    __enable_interrupt           
  #define __disable_irq   __disable_interrupt       
#endif

#if defined (__GNUC__)
  #define __ASM     asm

#elif defined (__CC_ARM)
    #define __ASM     __asm

#elif defined (__ICCARM__)
    #define __ASM     __asm

#endif


/* ARM7STDMI-S core interrupt function */
#ifdef __GNUC__

/* Disable global IRQ */
static __inline U32 DisableIRQ(void)
{
  register U32 ret, tmp;

  asm volatile
  (
    "MRS %0, CPSR" "\n\t"
    "ORR %1, %0, #0x80" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
  );

  return ret;
}

/* Disable global FIQ */
static __inline U32 DisableFIQ(void)
{
  register U32 ret, tmp;

  asm volatile
  (
    "MRS %0, CPSR" "\n\t"
    "ORR %1, %0, #0x40" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
  );

  return ret;
}

/* Disable both global IRQ and FIQ */
static __inline U32 Disable_IRQ_FIQ(void)
{
  register U32 ret, tmp;

  asm volatile
  (
    "MRS %0, CPSR" "\n\t"
    "ORR %1, %0, #0xC0" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
  );

  return ret;
}

/* Enable global IRQ */
static __inline U32 EnableIRQ(void)
{
  register U32 ret, tmp;

  asm volatile
  (
    "MRS %0, CPSR" "\n\t"
    "BIC %1, %0, #0x80" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
  );

  return ret;
}

/* Enable global FIQ */
static __inline U32 EnableFIQ(void)
{
  register U32 ret, tmp;

  asm volatile
  (
    "MRS %0, CPSR" "\n\t"
    "BIC %1, %0, #0x40" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
  );

  return ret;
}

/* Enable both global IRQ and FIQ */
static __inline U32 Enable_IRQ_FIQ(void)
{
  register U32 ret, tmp;

  asm volatile
  (
    "MRS %0, CPSR" "\n\t"
    "BIC %1, %0, #0xC0" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
  );

  return ret;
}

static __inline U32 Disable_IRQ_FIQ_MASK(U32 mask)
{
  register U32 ret, tmp;

  asm volatile
  (
    "BIC %1, %2, #0x2F" "\n\t"
    "MRS %0, CPSR" "\n\t"
    "ORR %1, %0, %1" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
        : "r"(mask)
      );

  
  return ret;
}

static __inline U32 Enable_IRQ_FIQ_MASK(U32 mask)
{
  register U32 ret, tmp;

  asm volatile
  (
    "BIC %1, %2, #0x2F" "\n\t"
    "MRS %0, CPSR" "\n\t"
    "BIC %1, %0, %1" "\n\t"
    "MSR CPSR_c, %1" "\n\t"
  : "=r"(ret), "=r"(tmp)
        : "r"(mask)
      );

  return ret;
}

static __inline U32 RestoreException(U32 old)
{
  register U32 ret, tmp, tmp2;

  asm volatile
  (
    "BIC %1, %3, #0x2F" "\n\t"
    "MRS %0, CPSR" "\n\t"
    "BIC %2, %0, #0xC0" "\n\t"
    "ORR %2, %2, %1" "\n\t"
    "MSR CPSR_c, %2" "\n\t"
  : "=r"(ret), "=r"(tmp), "=r"(tmp2)
        : "r"(old)
      );

  return ret;
}

#endif /* __GNUC__ */


#endif /*ARM7TDMIS_CORE_H_*/
