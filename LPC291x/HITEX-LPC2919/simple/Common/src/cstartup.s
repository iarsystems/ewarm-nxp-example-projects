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
	EXTERN	data_handler, irq_handler, fiq_handler
       	EXTERN	TargetResetInit
                    
        ARM	; Always ARM mode after reset	
        
__vector:
		ldr	pc,[pc,#24]	; Absolute jump can reach 4 GByte
;		ldr	b,?cstartup	; Relative branch allows remap, limited to 32 MByte
__undef_handler:
    ldr	pc,[pc,#24]	; Branch to undef_handler
__swi_handler:
		ldr	pc,[pc,#24]	; Branch to swi_handler
__prefetch_handler:
		ldr	pc,[pc,#24]	; Branch to prefetch_handler
__data_handler
		ldr	pc,[pc,#24]	; Branch to data_handler
		dc32 0xFFFFFFFF              
__irq_handler
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
      dc32	irq_handler
      dc32	__fiq_handler

        SECTION .text:CODE:NOROOT(2)

        EXTERN  ?main
        REQUIRE __vector
;        PUBLIC  ?cstartup

        ARM

__iar_program_start:
?cstartup:

;
; Add initialization needed before setup of stackpointers here.
;

; ARM - CP15 c1 Control Register
DTCM_enabled    	EQU 1<<2		; Data TCM
ITCM_enabled    	EQU 1<<12		; Instruction TCM
WB_enabled      	EQU 1<<3		; Write Buffer

	; Set global core configurations
	MRC     p15, 0, r4, c1, c0, 0   ; Read CP15
	ORR     r4, r4, #DTCM_enabled	; Enable Data TCM
	; At power up, the interrupt vector is mapped to addr. 0 already,
	; enabling instruction TCM will wipe out the mirror of the
	; vector table. A remap will be performed there after. */
	ORR     r4, r4, #ITCM_enabled	; Enable Instruction TCM
	ORR     r4, r4, #WB_enabled      ; Enable Write Buffer
	MCR     p15, 0, r4, c1, c0, 0   ; Write CP15


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


        mrs         r0,cpsr                             ; Original PSR value
        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#SVC_MODE                     ; Set Supervisor mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#UND_MODE                     ; Set Undefined mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(UND_STACK)                  ; End of UND_MODE

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#ABT_MODE                     ; Set Data abort mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(ABT_STACK)                  ; End of ABT_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#FIQ_MODE                     ; Set FIR mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK

        bic         r0,r0,#MODE_MSK                     ; Clear the mode bits
        orr         r0,r0,#SYS_MODE                     ; Set System mode bits
        msr         cpsr_c,r0                           ; Change the mode
        ldr         sp,=SFE(CSTACK)                     ; End of CSTACK

;
; Add more initialization here
;

	BL	VectorRemap     ; Need to copy the vector table to addr 0x0
	BL      TargetResetInit	;Initialize the target board
				;Jump to the entry point of C program

; Continue to ?main for C-level initialization.

        LDR     r0, =?main
        BX      r0
 
 
; Remap of the vector table down to 0x0. The TCM will be at address 0x0
; when it is enabled. We need to put the vector table in there after reset.
; if we boot from flash we copy the flash contents, if in debug we copy iSRAM

VectorRemap
        
VECTOR_RAM_SRC		EQU		0x80000000
VECTOR_FLASH_SRC	EQU		0x20000000
VECTOR_DST		EQU		0x00000000

        STMFD   sp!, {r0, r1, r4-r11}	; save registers

#ifdef  REMAP_RAM
	LDR     r0, =VECTOR_RAM_SRC
#else
	LDR     r0, =VECTOR_FLASH_SRC	; r0 = pointer to source block
#endif

	LDR     r1, =VECTOR_DST			; r1 = pointer to destination block      
	LDMIA   r0!, {r4-r11}			; remap first 16 words from 0x20000000
	STMIA   r1!, {r4-r11}			; to address 0x00000000 for interrupt
	LDMIA   r0!, {r4-r11}			; exception handler
	STMIA   r1!, {r4-r11}  
	
      LDMFD   sp!, {r0, r1, r4-r11}	        ; restore registers
      BX LR
    END
