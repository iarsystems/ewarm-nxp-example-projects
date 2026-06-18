/*
 * File:	crt0.s
 * Purpose:	Lowest level routines for Kinetis.
 *
 * Notes:
 *
 */

;         AREA   Crt0, CODE, READONLY      ; name this block of code


    SECTION CSTACK:DATA:NOROOT(3)

  	SECTION .startup : CODE
  	EXPORT  __startup
    import __iar_program_start;

__startup
        MOV     r0,#0                   ; Initialize the GPRs
	MOV     r1,#0
	MOV     r2,#0
	MOV     r3,#0
	MOV     r4,#0
	MOV     r5,#0
	MOV     r6,#0
	MOV     r7,#0
	MOV     r8,#0
	MOV     r9,#0
	MOV     r10,#0
	MOV     r11,#0
	MOV     r12,#0

        ldr r0, =sfe(CSTACK)
        ; set stack for SVC mode
        mov sp, r0

        B       __iar_program_start
        END
