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


//
// System Control Register
//
SCTLR_EL1_UCI       EQU     (1 << 26)
SCTLR_ELx_EE        EQU     (1 << 25)
SCTLR_EL1_E0E       EQU     (1 << 24)
SCTLR_ELx_WXN       EQU     (1 << 19)
SCTLR_EL1_nTWE      EQU     (1 << 18)
SCTLR_EL1_nTWI      EQU     (1 << 16)
SCTLR_EL1_UCT       EQU     (1 << 15)
SCTLR_EL1_DZE       EQU     (1 << 14)
SCTLR_ELx_I         EQU     (1 << 12)
SCTLR_EL1_UMA       EQU     (1 << 9)
SCTLR_EL1_SED       EQU     (1 << 8)
SCTLR_EL1_ITD       EQU     (1 << 7)
SCTLR_EL1_THEE      EQU     (1 << 6)
SCTLR_EL1_CP15BEN   EQU     (1 << 5)
SCTLR_EL1_SA0       EQU     (1 << 4)
SCTLR_ELx_SA        EQU     (1 << 3)
SCTLR_ELx_C         EQU     (1 << 2)
SCTLR_ELx_A         EQU     (1 << 1)
SCTLR_ELx_M         EQU     (1 << 0)


        SECTION .text:CODE:NOROOT(2)

        PUBLIC  enable_caches_el1                               ;
        PUBLIC  disable_caches_el1                              ;
        PUBLIC  enable_caches_el2                               ;
        PUBLIC  disable_caches_el2                              ;
        PUBLIC  enable_caches_el3                               ;
        PUBLIC  disable_caches_el3                              ;
        PUBLIC  invalidate_udcaches                             ;
        PUBLIC  zero_block                                      ;
        PUBLIC  get_midr                                        ;
        PUBLIC  get_mpidr                                       ;
        PUBLIC  set_ttbr0_el1                                   ;
        PUBLIC  set_ttbr1_el1                                   ;
        PUBLIC  get_tcr_el1                                     ;
        PUBLIC  set_tcr_el1                                     ;

;*******************************************************************************
;1.Function: Enable of Cache
;
;2.Note: void enable_caches_el1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
enable_caches_el1:
        mrs     x0, SCTLR_EL1                                   ;
        ORR     x0, x0, #SCTLR_ELx_I                            ;
        orr     x0, x0, #SCTLR_ELx_C                            ;
        msr     SCTLR_EL1, x0                                   ;
        isb
        ret

;*******************************************************************************
;1.Function: Disable of Cache
;
;2.Note: void disable_caches_el1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
disable_caches_el1:
        mrs     x0, SCTLR_EL1                                   ;
        AND     x0, x0, #~SCTLR_ELx_I                           ;
        AND     x0, x0, #~SCTLR_ELx_C                           ;
        msr     SCTLR_EL1, x0                                   ;
        isb
        ret

;*******************************************************************************
;1.Function: Enable of Cache
;
;2.Note: void enable_caches_el1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
enable_caches_el2:
        mrs     x0, SCTLR_EL2                                   ;
        ORR     x0, x0, #SCTLR_ELx_I                            ;
        orr     x0, x0, #SCTLR_ELx_C                            ;
        msr     SCTLR_EL2, x0                                   ;
        isb
        ret

;*******************************************************************************
;1.Function: Disable of Cache
;
;2.Note: void disable_caches_el1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
disable_caches_el2:
        mrs     x0, SCTLR_EL2                                   ;
        AND     x0, x0, #~SCTLR_ELx_I                           ;
        AND     x0, x0, #~SCTLR_ELx_C                           ;
        msr     SCTLR_EL2, x0                                   ;
        isb
        ret

;*******************************************************************************
;1.Function: Enable of Cache
;
;2.Note: void enable_caches_el3(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
enable_caches_el3:
        mrs     x0, SCTLR_EL3                                   ;
        ORR     x0, x0, #SCTLR_ELx_I                            ;
        orr     x0, x0, #SCTLR_ELx_C                            ;
        msr     SCTLR_EL3, x0                                   ;
        isb
        ret

;*******************************************************************************
;1.Function: Disable of Cache
;
;2.Note: void disable_caches_el3(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
disable_caches_el3:
        MRS     x0, SCTLR_EL3                                   ;
        AND     x0, x0, #~SCTLR_ELx_I                           ;
        AND     x0, x0, #~SCTLR_ELx_C                           ;
        MSR     SCTLR_EL3, x0                                   ;
        ISB
        RET

;*******************************************************************************
;1.Function: invalidate_udcaches
;
;2.Note: void Invalidate_udcaches(void)
;        ARM Cortex-A Series Programmer's Guide for ARMv8-A(Version 1.0)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
invalidate_udcaches:
        dmb     ish                                             ; ensure all prior inner-shareable accesses have been observed
        mrs     x0, CLIDR_EL1                                   ;
        and     w3, w0, #0x07000000                             ; get 2 x level of coherence
        lsr     w3, w3, #23                                     ;
        cbz     w3, finished                                    ;
        mov     w10, #0                                         ; w10 = 2 x cache level
        mov     w8, #1                                          ; w8 = constant 0b1
loop_level:
        add     w2, w10, w10, lsr #1                            ; calculate 3 x cache level
        lsr     w1, w0, w2                                      ; extract 3-bit cache type for this level
        and     w1, w1, #0x7                                    ;
        cmp     w1, #2                                          ;
        b.lt    next_level                                      ; no data or unified cache at this level
        msr     CSSELR_EL1, x10                                 ; select this cache level
        isb                                                     ; synchronize change of csselr
        mrs     x1, CCSIDR_EL1                                  ; read ccsidr
        and     w2, w1, #7                                      ; w2 = log2(linelen)-4
        add     w2, w2, #4                                      ; w2 = log2(linelen)
        ubfx    w4, w1, #3, #10                                 ; w4 = max way number, right aligned
        clz     w5, w4                                          ; w5 = 32-log2(ways), bit position of way in dc operand
        lsl     w9, w4, w5                                      ; w9 = max way number, aligned to position in dc operand
        lsl     w16, w8, w5                                     ; w16 = amount to decrement way number per iteration
loop_way:
        ubfx    w7, w1, #13, #15                                ; w7 = max set number, right aligned
        lsl     w7, w7, w2                                      ; w7 = max set number, aligned to position in dc operand
        lsl     w17, w8, w2                                     ; w17 = amount to decrement set number per iteration
loop_set:
        orr     w11, w10, w9                                    ; w11 = combine way number and cache number ...
        orr     w11, w11, w7                                    ; ...and set number for dc operand
        dc      isw, x11                                        ; do data cache invalidate by set and way
        subs    w7, w7, w17                                     ; decrement set number
        b.ge    loop_set                                        ;
        subs    x9, x9, x16                                     ; decrement way number
        b.ge    loop_way                                        ;
next_level:
        add     w10, w10, #2                                    ; increment 2 x cache level
        cmp     w3, w10                                         ;
        b.gt    loop_level                                      ;
        dsb     sy                                              ; ensure completion of previous cache maintenance operation
        isb                                                     ;
finished:
        ret

;*******************************************************************************
;1.Function: Zero fill a block of memory
;
;2.Note: void zero_block(void *blockPtr,unsigned int nBytes)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/  blockPtr                   base address of block to fill
;I/  nBytes                     block size, in bytes

;4.Return:pointer to just filled block, NULL if nBytes is incompatible with block fill size
;*******************************************************************************
zero_block:
        ubfx    x2, x1, #0, #4                                  ;
        cbnz    x2, incompatible                                ;

        mov     x3, x2                                          ;
        add     x0, x0, x1                                      ;
loop0:
        subs    x1, x1, #16                                     ;
        stp     x2, x3, [x0, #-16]!                             ;
        b.ne    loop0                                           ;
        ret                                                     ;

incompatible:
        mov x0,#0                                               ;
        ret                                                     ;


;*******************************************************************************
;1.Function: Read Main ID Register(MIDR_EL1)
;
;2.Note: void get_midr(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      0x410FD034
;*******************************************************************************
get_midr:
        MRS     x0, MIDR_EL1
        RET

;*******************************************************************************
;1.Function: Read Multiprocessor Affinity Register(MPIDR_EL1)
;
;2.Note: void get_mpidr(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      Core ID no(0:Core0,1:Core1,2:Core2,3:Core3)
;*******************************************************************************
get_mpidr:
        MRS     x0, MPIDR_EL1
        AND     x0,x0, #0xff
        RET

;*******************************************************************************
;1.Function: Write Translation Table Base Register0(EL1) TTBR0_EL1
;
;2.Note: void set_ttbr0_el1(uint64_t)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ addr                        TLB base address
;4.Return:
;*******************************************************************************
set_ttbr0_el1:
        MSR     TTBR0_EL1,x0
        RET

;*******************************************************************************
;1.Function: Write Translation Table Base Register1(EL1) TTBR1_EL1
;
;2.Note: void set_ttbr1_el1(uint64_t)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ addr                        TLB base address
;4.Return:
;*******************************************************************************
set_ttbr1_el1:
        MSR     TTBR1_EL1,x0
        RET

;*******************************************************************************
;1.Function: Write Translation Control Register(EL1) TCR_EL1
;
;2.Note: void set_ttbr1_el1(uint64_t)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I
;4.Return:                      TCR_EL1 val
;*******************************************************************************
get_tcr_el1:
        MRS     x0,TCR_EL1
        RET

;*******************************************************************************
;1.Function: Write Translation Control Register(EL1) TCR_EL1
;
;2.Note: void set_ttbr1_el1(uint64_t)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ addr                        TCR_EL1 val
;4.Return:
;*******************************************************************************
set_tcr_el1:
        MSR     TCR_EL1,x0
        RET

        END

