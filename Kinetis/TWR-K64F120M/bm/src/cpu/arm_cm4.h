/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

/*
 * File:		arm_cm4.h
 * Purpose:		Definitions common to all ARM Cortex M4 processors
 *
 * Notes:
 */

#ifndef _CPU_ARM_CM4_H
#define _CPU_ARM_CM4_H

#include "common.h"

/*ARM Cortex M4 implementation for interrupt priority shift*/
#define ARM_INTERRUPT_LEVEL_BITS		4

// function prototypes for arm_cm4.c
void stop (void);
void wait (void);
void write_vtor (int);
void enable_irq (int);
void disable_irq (int);
void set_irq_priority (int, int);

/*!< Macro to enable all interrupts. */
#define EnableInterrupts asm(" CPSIE i");

/*!< Macro to disable all interrupts. */
#define DisableInterrupts asm(" CPSID i");

/* Misc. Defines */
#ifdef FALSE
#undef FALSE
#endif
#define FALSE	(0)

#ifdef TRUE
#undef TRUE
#endif
#define	TRUE	(1)

#ifdef NULL
#undef NULL
#endif
#define NULL	(0)

#ifdef ON
#undef ON
#endif
#define ON		(1)

#ifdef OFF
#undef OFF
#endif
#define OFF		(0)

/* The basic data types */
typedef unsigned char		uint8;		/*  8 bits */
typedef unsigned short int	uint16;		/* 16 bits */
typedef unsigned long int	uint32;		/* 32 bits */

typedef char				int8;		/*  8 bits */
typedef short int			int16;		/* 16 bits */
typedef int					int32;		/* 32 bits */

typedef volatile int8		vint8;		/*  8 bits */
typedef volatile int16		vint16;		/* 16 bits */
typedef volatile int32		vint32;		/* 32 bits */

typedef volatile uint8		vuint8;		/*  8 bits */
typedef volatile uint16		vuint16;	/* 16 bits */
typedef volatile uint32		vuint32;	/* 32 bits */

/* function prototype for main function */
void main(void);

#endif		/* _CPU_ARM_CM4_H */
