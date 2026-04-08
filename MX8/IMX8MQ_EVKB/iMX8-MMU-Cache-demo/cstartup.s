;*******************************************************************************
;                               Copyright 2020 IAR Systems. All rights reserved.
;Function: Cortex-A53(LP64 AARCH64) Startup
;
;Note: LP64(32-bit int,64-bit long pointer)
;
;History:
; +------------- No(000 .. 999)
; |   +--------- Version
; |   |    +---- New,Change,Add,delete
; v   v    v
; No  Ver  Category    Date      Name       Description
;---+-----+--------+----------+-------------+-----------------------------------
;000 00.00 New      2020/07/15 S.Tonoshita  New development
;*******************************************************************************

        SECTION CORE0_CSTACK:DATA:NOROOT(4)
        SECTION CORE1_CSTACK:DATA:NOROOT(4)
        SECTION CORE2_CSTACK:DATA:NOROOT(4)
        SECTION CORE3_CSTACK:DATA:NOROOT(4)
        EXTERN __evector
        EXTERN __cmain
        EXTERN core1_main
        EXTERN core2_main
        EXTERN core3_main
        EXTERN invalidate_udcaches
        EXTERN zero_block
        EXTERN get_mpidr
        EXTERN init_mmu
        EXTERN l0_translation_table


        SECTION .reset:CODE:NOROOT(3)
        CODE
;*******************************************************************************
    SECTION .reset:CODE:NOROOT(2)

;*******************************************************************************
    PUBLIC __iar_program_start
__iar_program_start:
        MRS         x0, SCTLR_EL3                               ; Read SCTLR_EL3
        MOV         x1, XZR                                     ; Clear x1
        ORR         x1, x1, #(0x1 << 12)                        ; Disable instruction cache
        ORR         x1, x1, #(0x1 << 2)                         ; Disable data cache
        ORR         x1, x1, #(0x1 << 0)                         ; Disable MMU
        BIC         x0, x0, x1                                  ; Clear
        MSR         SCTLR_EL3, x0                               ; Write SCTLR_EL3
        ISB                                                     ; Instruction re-fetching
        LDR         x0, evector                                 ; Read exception vector address
        MSR         VBAR_EL3, x0                                ; Write Vector Base Address Register(VBAR_EL3)

        DSB         ISHST                                       ;
        TLBI        ALLE3                                       ; TLB invalidate All,EL3
        DSB         ISH                                         ;
        ISB                                                     ; Instruction Synchronization Barrier

; Set up memory attributes(See Cortex-A53 TRM)
; These equate to:
; 0 -> 0b11101110 = 0x000000EE = Normal memory,Outer Write-Back Non-transient(Outer Read-Allocate policy)
;                              = Normal memory,Inner Write-Back Non-transient(Outer Read-Allocate policy)
; 1 -> 0b00000100 = 0x00000400 = Device-nGnRE
;
        MOV         x0, #0x04ee                                 ; Set memory attributes
        MSR         MAIR_EL3, x0                                ; Write MAIR_EL3
;
        BL          init_mmu                                    ; VMSAv8-64 translation table setup.
        LDR         x0,l0_tlb                                   ; Read translation table address.
        MSR         TTBR0_EL3, x0                               ; Write translation table address.
        LDR         x0,tcr_el3_init                             ; Read TCR_EL3 data
        MSR         TCR_EL3, x0                                 ; Write TCR_EL3 data
        ISB                                                     ; Re-fetch(Instruction Synchronization Barrier)

        MRS         x0, SCTLR_EL3                               ; Read SCTLR_EL3
        ORR         x0, x0, #(0x1 << 0)                         ; Enable MMU
        MSR         SCTLR_EL3, x0                               ; Write SCTLR_EL3
        ISB                                                     ; Re-fetch(Instruction Synchronization Barrier)

        BL          invalidate_udcaches                         ; Invalidate udcaches

        BL          get_mpidr                                   ; Read Core No
        CMP         x0,1                                        ; Core1?
        B.EQ        start_0001                                  ; Core1
        CMP         x0,2                                        ; Core2?
        B.EQ        start_0002                                  ; Core2
        CMP         x0,3                                        ; Core3?
        B.EQ        start_0003                                  ; Core3
        LDR         x0, core0_stack_end                         ; Load the end-address of the EL3 stack
        MOV         sp, x0                                      ; Initialization of the EL3 stack
start:
        B           __cmain                                     ; Core0 Start

start_0001:                                                     ; Core1
        LDR         x0, core1_stack_end                         ; Load the end-address of the EL3 stack
        MOV         sp, x0                                      ; Initialization of the EL3 stack
        B           core1_main                                  ;

start_0002:                                                     ; Core2
        LDR         x0, core2_stack_end                         ; Load the end-address of the EL3 stack
        MOV         sp, x0                                      ; Initialization of the EL3 stack
        B           core2_main                                  ;

start_0003:                                                     ; Core2
        LDR         x0, core3_stack_end                         ; Load the end-address of the EL3 stack
        MOV         sp, x0                                      ; Initialization of the EL3 stack
        B           core3_main                                  ;

        SECTION .reset:CODE:NOROOT(3)
        DATA64
evector:
        DC64        __evector                                   ; Vector top address
core0_stack_end:
        DC64        SFE(CORE0_CSTACK)
core1_stack_end:
        DC64        SFE(CORE1_CSTACK)
core2_stack_end:
        DC64        SFE(CORE2_CSTACK)
core3_stack_end:
        DC64        SFE(CORE3_CSTACK)
l0_tlb:
        DC64        l0_translation_table
tcr_el3_init:
        DC64        0x0002051f
        END
