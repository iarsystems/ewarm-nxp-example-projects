/*
 * File:	crt0.s
 * Purpose:	Lowest level routines for Vybrid
 *
 * Notes:
 *
 */

 SECTION IRQ_STACK:DATA:NOROOT(3)
 SECTION FIQ_STACK:DATA:NOROOT(3)
 SECTION CSTACK:DATA:NOROOT(3)

;         AREA   Crt0, CODE, READONLY      ; name this block of code

 SECTION .noinit : CODE

        EXPORT  __iar_program_start
        EXTERN  ?main
        EXTWEAK __iar_init_vfp
        EXTWEAK __iar_init_core

__iar_program_start

	ldr    r1,=0x0
	mov    r2,  r1
	mov    r3,  r1
	mov    r4,  r1
	mov    r5,  r1
	mov    r6,  r1
	mov    r7,  r1
	mov    r8,  r1
	mov    r9,  r1
	mov    r10, r1
	mov    r11, r1
	mov    r12, r1

  ; Disable MMU
  MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
  BIC   r0, r0, #0x1
  MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data

; --------------------
; Mode, correspords to bits 0-5 in CPSR

MODE_MSK DEFINE 0x1F            ; Bit mask for mode bits in CPSR

USR_MODE DEFINE 0x10            ; User mode
FIQ_MODE DEFINE 0x11            ; Fast Interrupt Request mode
IRQ_MODE DEFINE 0x12            ; Interrupt Request mode
SVC_MODE DEFINE 0x13            ; Supervisor mode
ABT_MODE DEFINE 0x17            ; Abort mode
UND_MODE DEFINE 0x1B            ; Undefined Instruction mode
SYS_MODE DEFINE 0x1F            ; System mode


        MRS     r0, cpsr                ; Original PSR value

        ;; Set up the interrupt stack pointer.

        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #IRQ_MODE       ; Set IRQ mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(IRQ_STACK)     ; End of IRQ_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        ;; Set up the fast interrupt stack pointer.

        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #FIQ_MODE       ; Set FIR mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(FIQ_STACK)     ; End of FIQ_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        ;; Set up the normal stack pointer.

        BIC     r0 ,r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0 ,r0, #SYS_MODE       ; Set System mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(CSTACK)        ; End of CSTACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        BL      __iar_init_vfp
        BL      __iar_init_core

        ldr     r0,=?main                 ; call the C code
        bx      r0

        END
