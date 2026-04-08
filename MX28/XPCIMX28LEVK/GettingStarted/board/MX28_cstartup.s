;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 16 $
;;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)

;
; The module in this file are included in the libraries, and may be
; replaced by any user-defined modules that define the PUBLIC symbol
; __iar_program_start or a user defined start symbol.
;
; To override the cstartup defined in the library, simply add your
; modified version to the workbench project.

        SECTION .intvec:CODE:NOROOT(2)

        PUBLIC  __vector
        PUBLIC  __iar_program_start

		    EXTERN	undef_handler, swi_handler, prefetch_handler
		    EXTERN	data_handler, IRQ_Handler, FIQ_Handler
                ARM	; Always ARM mode after reset	
__vector:
		ldr	pc,[pc,#24]	; Absolute jump can reach 4 GByte
;		b ?cstartup	; Relative branch allows remap, limited to 32 MByte

__undef_handler:
    ldr	pc,[pc,#24]	; Branch to undef_handler
__swi_handler:
		ldr	pc,[pc,#24]	; Branch to swi_handler
__prefetch_handler:
		ldr	pc,[pc,#24]	; Branch to prefetch_handler
__data_handler
		ldr	pc,[pc,#24]	; Branch to data_handler
		  dc32 0xFFFFFFFF
__irq_handler:
		ldr	pc,[pc,#24]	; Branch to irq_handler
__fiq_handler:
		ldr	pc,[pc,#24]	; Branch to fiq_handler

		; Constant table entries (for ldr pc) will be placed at 0x20
      dc32	__iar_program_start
      dc32	__undef_handler
      dc32	__swi_handler
      dc32	__prefetch_handler
      dc32	__data_handler
		  dc32 0xFFFFFFFF
      dc32	IRQ_Handler
      dc32	FIQ_Handler

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


CP_DIS_MASK         DEFINE  0xFFFFEFFA

        SECTION .text:CODE:NOROOT(2)

        EXTERN  ?main
        REQUIRE __vector

        ARM

__iar_program_start:
?cstartup:

; Execution starts here.
; After a reset, the mode is ARM, Supervisor, interrupts disabled.

 // Disable Addr translation, D cache and enable I cache
        MRC         p15,0,R1,C1,C0,0
        LDR         R0,=CP_DIS_MASK      ;; 0xFFFFEFFA
        AND         R1,R1,R0
        ORR         R1,R1,#(1<<12)
        MCR         p15,0,R1,C1,C0,0

; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.
        mrs     r0,cpsr                             ; Original PSR value
        bic     r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr     r0,r0,#SVC_MODE                     ; Set Supervisor mode bits
        msr     cpsr_c,r0                           ; Change the mode
        ldr     sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

        bic     r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr     r0,r0,#ABT_MODE                     ; Set Abort mode bits
        msr     cpsr_c,r0                           ; Change the mode
        ldr     sp,=SFE(ABT_STACK)                  ; End of ABT_STACK

        bic     r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr     r0,r0,#UND_MODE                     ; Set Undefined mode bits
        msr     cpsr_c,r0                           ; Change the mode
        ldr     sp,=SFE(UND_STACK)                  ; End of UND_STACK

        bic     r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr     r0,r0,#FIQ_MODE                     ; Set FIR mode bits
        msr     cpsr_c,r0                           ; Change the mode
        ldr     sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK

        bic     r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr     r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
        msr     cpsr_c,r0                           ; Change the mode
        ldr     sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK

        bic     r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr     r0,r0,#SYS_MODE                     ; Set System mode bits
        msr     cpsr_c,r0                           ; Change the mode
        ldr     sp,=SFE(CSTACK)                     ; End of CSTACK

#ifdef __ARMVFP__
; Enable the VFP coprocessor.
        mov         r0, #BASE_ARD_EIM                   ; Set EN bit in VFP
        fmxr        fpexc, r0                           ; FPEXC, clear others.

; Disable underflow exceptions by setting flush to zero mode.
; For full IEEE 754 underflow compliance this code should be removed
; and the appropriate exception handler installed.
        mov         r0, #0x01000000		                  ; Set FZ bit in VFP
        fmxr        fpscr, r0                           ; FPSCR, clear others.
#endif

; Add more initialization here

; Continue to ?main for more IAR specific system startup
        ldr         r0,=?main
        bx          r0


                END
