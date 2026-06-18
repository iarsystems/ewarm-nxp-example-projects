;/*
; * File:	crt0.s
; * Purpose:	Lowest level routines for Vybrid
; *
; * Notes:
; *
; */

; SECTION IRQ_STACK:DATA:NOROOT(3)
; SECTION FIQ_STACK:DATA:NOROOT(3)
; SECTION CSTACK:DATA:NOROOT(3)

;IRQ_Stack_Size  EQU     0x00000400
;                AREA    IRQ_STACK, NOINIT, READWRITE, ALIGN=3
;IRQStack_Mem    SPACE   IRQ_Stack_Size
;__initial_irqsp
;
;FIQ_Stack_Size  EQU     0x00000400
;                AREA    FIQ_STACK, NOINIT, READWRITE, ALIGN=3
;FIQStack_Mem    SPACE   FIQ_Stack_Size
;__initial_fiqsp
;
;CStack_Size     EQU     0x00000400
;                AREA    CSTACK, NOINIT, READWRITE, ALIGN=3
;CStack_Mem      SPACE   CStack_Size
;__initial_csp


        AREA   Crt0, CODE, READONLY      ; name this block of code
		PRESERVE8
; SECTION .noinit : CODE

		IMPORT  ||Image$$ARM_LIB_STACK$$ZI$$Limit||
		IMPORT  ||Image$$IRQ_STACK$$ZI$$Limit||
		IMPORT  ||Image$$FIQ_STACK$$ZI$$Limit||

        EXPORT  _start
_start
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
        
; --------------------
; Mode, correspords to bits 0-5 in CPSR

MODE_MSK EQU 0x1F            ; Bit mask for mode bits in CPSR

USR_MODE EQU 0x10            ; User mode
FIQ_MODE EQU 0x11            ; Fast Interrupt Request mode
IRQ_MODE EQU 0x12            ; Interrupt Request mode
SVC_MODE EQU 0x13            ; Supervisor mode
ABT_MODE EQU 0x17            ; Abort mode
UND_MODE EQU 0x1B            ; Undefined Instruction mode
SYS_MODE EQU 0x1F            ; System mode


        MRS     r0, cpsr                ; Original PSR value

        ;; Set up the interrupt stack pointer.

        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #IRQ_MODE       ; Set IRQ mode bits
        MSR     cpsr_c, r0              ; Change the mode
        ;LDR     sp, =__initial_irqsp     ; End of IRQ_STACK
        LDR     sp, =||Image$$IRQ_STACK$$ZI$$Limit||     ; End of IRQ_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        ;; Set up the fast interrupt stack pointer.

        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #FIQ_MODE       ; Set FIR mode bits
        MSR     cpsr_c, r0              ; Change the mode
        ;LDR     sp, =__initial_fiqsp     ; End of FIQ_STACK
        LDR     sp, =||Image$$FIQ_STACK$$ZI$$Limit||     ; End of FIQ_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        ;; Set up the normal stack pointer.

        BIC     r0 ,r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0 ,r0, #SYS_MODE       ; Set System mode bits
        MSR     cpsr_c, r0              ; Change the mode
        ;LDR     sp, __initial_csp        ; End of CSTACK
        LDR     sp, =||Image$$ARM_LIB_STACK$$ZI$$Limit||       ; End of CSTACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        import start
        BL      start                  ; call the C code

__done
        B       __done

        END
