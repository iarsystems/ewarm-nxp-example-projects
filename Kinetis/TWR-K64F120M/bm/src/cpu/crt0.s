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
 * File:	crt0.s
 * Purpose:	Lowest level routines for Kinetis.
 *
 * Notes:
 *
 */

;	AREA   Crt0, CODE, READONLY		; name this block of code
  SECTION CRT0_SECTION : CODE

	EXPORT  __startup

__startup
	MOV		r0,#0				; Initialize the GPRs
	MOV		r1,#0
	MOV		r2,#0
	MOV		r3,#0
	MOV		r4,#0
	MOV		r5,#0
	MOV		r6,#0
	MOV		r7,#0
	MOV		r8,#0
	MOV		r9,#0
	MOV		r10,#0
	MOV		r11,#0
	MOV		r12,#0
	CPSIE	i					; Unmask interrupts
	import start
	BL		start				; call the C code
__done
	B		__done

// hard fault handler wrapper in assembly
// it extract the location of stack frame and pass it
// to handler in C as pointer.
	EXPORT  __hard_fault_handler_asm
	IMPORT  hard_fault_handler_c

__hard_fault_handler_asm
	TST     LR, #4
	ITE     EQ
	MRSEQ   R0, MSP
	MRSNE   R0, PSP
	B       hard_fault_handler_c	; call the C code

	END
