;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 28 $
;;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)
        SECTION BTLD_STACK:DATA:NOROOT(3)
        SECTION CODE_END:DATA

;
; The module in this file are included in the libraries, and may be
; replaced by any user-defined modules that define the PUBLIC symbol
; __iar_program_start or a user defined start symbol.
;
; To override the cstartup defined in the library, simply add your
; modified version to the workbench project.



        SECTION .bootheader:CODE:ROOT(2)

        PUBLIC  __iar_program_start
        ARM	  ; Always ARM mode after reset	

; This initial handler is only for reset, a real application will
; replace this exception handler in memory with a more capable one
;__vector
    B		__iar_program_start  ; Reset
	DCD		0x41676d69		       ;image magic number 
	DCD		0,0,0,0,0
image_type
	DCD		0x0000000A		       ;Plain text no CRC check

sizeOfPermanentCode	
  DCD 	sfe(CODE_END)-0x11029000 ;
	DCD 	0,0
bootparameter	
	DCD 	0
	DCD 	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

        PUBLIC  ?cstartup
        EXTERN  ?main
        EXTERN  bootloader
        EXTERN  test_sdram_init
        
        SECTION .text:CODE:NOROOT(2)
        ARM

__iar_program_start:
?cstartup:

;
; Add initialization needed before setup of stackpointers here.
;
MMU_DISABLE_MASK   DEFINE    0xFFFFEFFA
MMU_ENABLE_MASK    DEFINE    0x00001005 
; MMU table address present in bootROM
BROM_MMU_BASE_ADDR	DEFINE   0x1201C000 

    ; Ensure the MMU is disabled
    MRC   p15, 0, r1, c1, c0, 0
    LDR   r2,=MMU_DISABLE_MASK
    AND   r1, r1, r2
    MCR   p15, 0, r1, c1, c0, 0

    ; Invalidate TLBs
    MOV   r1,#0
    MCR   p15, 0, r1, c8, c7, 0

    ; Invalidate both caches
    MCR   p15, 0, r1, c7, c7, 0

    ; Enable instruction cache
    MRC   p15, 0, r1, c1, c0, 0
    LDR   r2,=0x1000
    ORR   r1, r1, r2
    MCR   p15, 0, r1, c1, c0, 0
;
; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.
;
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
I_MASK   DEFINE 0x080
F_MASK   DEFINE 0x040

        mrs         r0,cpsr                             ; Original PSR value
        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#(SVC_MODE | I_MASK | F_MASK); Set Supervisor mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

        ; Initial ea3131 board
        BL    test_sdram_init

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#(UND_MODE |I_MASK | F_MASK)  ; Set Undefined mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(UND_STACK)                  ; End of UND_MODE

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#(ABT_MODE | I_MASK | F_MASK) ; Set Data abort mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(ABT_STACK)                  ; End of ABT_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#(FIQ_MODE | I_MASK | F_MASK) ; Set FIR mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#(IRQ_MODE | I_MASK | F_MASK) ; Set IRQ mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#(SYS_MODE | I_MASK | F_MASK) ; Set System mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(CSTACK)                     ; End of CSTACK

#ifdef __ARMVFP__
        ;; Enable the VFP coprocessor.

        MOV     r0, #0x40000000         ; Set EN bit in VFP
        FMXR    fpexc, r0               ; FPEXC, clear others.

;
; Disable underflow exceptions by setting flush to zero mode.
; For full IEEE 754 underflow compliance this code should be removed
; and the appropriate exception handler installed.
;

        MOV     r0, #0x01000000         ; Set FZ bit in VFP
        FMXR    fpscr, r0               ; FPSCR, clear others.
#endif

    ; Setup the Domain Access Control as all Manager
    ; Make all domains open, user can impose restrictions
    MVN   r1, #0
    MCR   p15, 0, r1, c3, c0, 0
    
    ; Setup Translation Table Base
    LDR   r3, =BROM_MMU_BASE_ADDR
    MCR   p15, 0, r3, c2, c0, 0

    ; Enable the MMU with instruction and data caches enabled
    MRC   p15, 0, r1, c1, c0, 0
    LDR   r2,=MMU_ENABLE_MASK
    ORR   r1, r1, r2
    MCR   p15, 0, r1, c1, c0, 0

; Continue to ?main for C-level initialization.

    LDR     r0, =?main

    ; Setup jump to run out of virtual memory
    ADD     r0,r0,#0x100000 
    BX      r0
      
   SECTION CODE_END:DATA(9)
   DATA

CodeEnd:

    END


