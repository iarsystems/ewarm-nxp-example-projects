;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level bootloader
;; initialization.
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 9233 $
;;

        MODULE  ?boot_cstartup

        ;; Forward declaration of sections.
;
; The module in this file are included in the libraries, and may be
; replaced by any user-defined modules that define the PUBLIC symbol
; __iar_program_start or a user defined start symbol.
;
; To override the cstartup defined in the library, simply add your
; modified version to the workbench project.

        SECTION BOOT_STACK:DATA:NOROOT(3)

        SECTION .boot_intvec:CODE:ROOT(2)

        ARM	; Always ARM mode after reset	

; --------------------
; Mode, correspords to bits 0-5 in CPSR

MODE_MSK DEFINE 0x1F            ; Bit mask for mode bits in CPSR
SVC_MODE DEFINE 0x13            ; Supervisor mode

        PUBLIC __boot_entry
        EXTERN  bootloader

        ARM
__boot_entry

; Execution starts here.
; After a reset, the mode is ARM, Supervisor, interrupts disabled.

; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.
        mrs         r0,cpsr               ; Original PSR value
        bic         r0,r0,#MODE_MSK       ; Clear the mode bits
        orr         r0,r0,#SVC_MODE       ; Set Supervisor mode bits
        msr         cpsr_c,r0             ; Change the mode
        ldr         sp,=SFE(BOOT_STACK)   ; End of SVC_STACK

; Add more initialization here
NFCRAM      DEFINE 0xD8000000      ; NFC RAM Start Address
NFCRAM_END  DEFINE 0xD80007FF      ; NFC RAM End Address
INTRRAM     DEFINE 0xFFFFC400      ; Internal RAM Start Address

;copy bootloader in to internal RAM
        ldr         r0,=NFCRAM     ;
        ldr         r1,=INTRRAM    ;
        ldr         r2,=NFCRAM_END ;

  __loop:
        ldm         r0!,{R3,R4,R5,R6,R7,R8,R9,R10};
        stm         r1!,{R3,R4,R5,R6,R7,R8,R9,R10};
        cmp         r2,r0;
        bhi         __loop;

        ldr         r0,=bootloader
        bx          r0

        b           .
                END
