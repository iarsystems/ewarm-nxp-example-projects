;;
;; Copyright (c) 2009 ARM Ltd. All rights reserved.
;;


;  SECTION VECTORS_SECTION : CODE (2)



;ARM_PERIPHBASE           EQU        0x00A00000
;IC_INTERFACES_BASE_ADDR  EQU        ARM_PERIPHBASE+0x0100
;ICCIAR                   EQU        IC_INTERFACES_BASE_ADDR + 0x00c
;ICCEOIR                  EQU        IC_INTERFACES_BASE_ADDR + 0x010

ICCIAR                   EQU        0x40002000 + 0x10c
ICCEOIR                  EQU        0x40002000 + 0x110

NO_IRQ     EQU    0x80                     ;/* mask to disable IRQ */
NO_FIQ     EQU    0x40                     ;/* mask to disable FIQ */
NO_INT     EQU    (NO_IRQ | NO_FIQ) ;/*mask to disable IRQ and FIQ */
FIQ_MODE   EQU    0x11
IRQ_MODE   EQU    0x12
SYS_MODE   EQU    0x1F
	
  ;PUBLIC vector_table

  
 ;GE: not required by IAR?


  ;SECTION .intvec : CODE (2)
  ;CODE32
  AREA vectors_section, CODE, READONLY
  ENTRY

  EXPORT vector_table

vector_table
  LDR   pc, =reset_handler
  LDR   pc, =Undef_Routine
  LDR   pc, =SWI_Routine
  LDR   pc, =Prefetch_Routine
  LDR   pc, =Abort_Routine
  NOP   ; Reserved vector
  LDR   pc, =IRQ_Routine
  LDR   pc, =FIQ_Routine
END


  extern interrupt_handlers
  EXPORT reset_handler
  EXPORT Undef_Routine
  EXPORT SWI_Routine
  EXPORT Prefetch_Routine
  EXPORT Abort_Routine
  EXPORT IRQ_Routine
  EXPORT FIQ_Routine
  EXPORT SMI_Routine

literals
  LTORG

; Handlers
  ;IMPORT init_start
  IMPORT _start
reset_handler
  ;BL     init_start
  BL	  _start

literals2
  LTORG


default_exception_hdler
    stmfd   sp!, {lr}       
    ldmfd   sp!, {pc}   
undef_hdler
  DCD  default_exception_hdler
swi_hdler
  DCD  default_exception_hdler
prefetch_hdler
  DCD  default_exception_hdler
abort_hdler
  DCD  default_exception_hdler
smi_hdler
  DCD  default_exception_hdler

Undef_Routine
    stmfd   sp!, {r0,lr}           ; push our temp reg, R0, since C seems to feel free to bash it anyway
    ldr     lr,=undef_hdler        ; load address of pointer to undef handler function using LR temporarily
    ldr     r0,[lr]                ; load the undef handler function pointer into R0
    mov     lr,pc                  ; preserve PC in LR so that called code can return.
    bx      r0                     ; this way of doing the call allows Thumb code to be called
    ldmfd   sp!, {r0,lr}           ; then, pull back preserved R0.
    movs    pc,lr                  ; return from the interrupt to wherever LR points.

SWI_Routine
    stmfd   sp!, {r0-r1,lr}        ; push register onto SWI stack
    mrs     r0, spsr               ; get saved status register
    tst     r0, #0x20              ; check if call was in THUMB mode
    ldrneh  r0, [lr,#-2]           ; yes: load opcode half-word and
    bicne   r0, r0, #0xff00        ; yes: extract THUMB comment
    ldreq   r0, [lr,#-4]           ; no: load opcode word and
    biceq   r0, r0, #0xff000000    ; no: extract ARM comment
                                   ; now r0 has comment field
    ldr     lr,=swi_hdler          ; >= 16: pointer to standard SWI
    ldr     r1,[lr]                ; handler
    mov     lr,pc                  ; >= 16: set link register
    bx      r1                     ; >= 16: jump to standard SWI
                                   ; handler
    ldmfd   sp!, {r0-r1,pc}


Prefetch_Routine
    stmfd    sp!, {r0,lr}          ; see UNDEF_HLDR for commentary
                                   ; as this only differs in return mechanism
    ldr      lr,=prefetch_hdler
    ldr      r0,[lr]
    mov      lr,pc
    bx       r0
    ldmfd    sp!, {r0,lr}
    subs     pc,lr,#4

Abort_Routine
    stmfd    sp!, {r0-r7,lr}       ; see UNDEF_HLDR for commentary
                                   ; as this only differs in return mechanism
    ldr      lr,=abort_hdler
    ldr      r0,[lr]
    mov      lr,pc
    bx       r0
    mrc      p15,0,r0,c5,c0,0
    ldr      r1,=0x40f
    and      r0,r0,r1
    ldr      r2,=0x406
    cmp      r0,r2
    bne      NOT_IMPRECISE_ABORT
    ldmfd    sp!, {r0-r7,lr}
    subs     pc,lr,#8
NOT_IMPRECISE_ABORT
    ldmfd    sp!, {r0-r7,lr}
    subs     pc,lr,#4

IRQ_Routine
;/* IRQ entry {{{ */
    MOV     r13,r0              ;/* save r0 in r13_IRQ */
    SUB     r0,lr,#4            ;/* put return address in r0_SYS */
    MOV     lr,r1               ;/* save r1 in r14_IRQ (lr) */
    MRS     r1,spsr            ; /* put the SPSR in r1_SYS */

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ;/* SYSTEM, no IRQ, but FIQ enabled! */
    STMFD   sp!,{r0,r1}         ;/* save SPSR and PC on SYS stack */
    STMFD   sp!,{r2-r12,lr}  ;/* save all other regs on SYS stack */
    MOV     r0,sp               ;/* make the sp_SYS visible to IRQ mode */
    SUB     sp,sp,#(2*4)        ;/* make room for stacking (r0_SYS, r1_SYS) */

    MSR     cpsr_c,#(IRQ_MODE | NO_IRQ) ;/* IRQ mode, IRQ/FIQ disabled */
    STMFD   r0!,{r13,r14}       ;/* finish saving the context (r0_SYS,r1_SYS)*/

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ;/* SYSTEM mode, IRQ disabled */
;/* IRQ entry }}} */]

    ldr     r0,=ICCIAR      ; Interrupt Acknowledge register
    ldr     r3,[r0]         ; r0[12:10] = CPU_ID of software interrupts, r0[9:0] = INT_ID
    ldr     r2,=0x00000200
    and     r2,r3,r2        
    cmp     r2,#0           ; Check that INT_ID is not 1023 or 1022 (spurious interrupt)
    bne     END_IRQ_HLDR
    ldr     r2,=0x000001FF
    and     r0,r3,r2        ; Keep INT_ID at r0
    stmfd   sp!, {r3}       ; Save original ICCIAR value

    lsl     r2,r0,#2
    ldr     lr,=interrupt_handlers
    ldr     r2,[lr,r2]
 	MSR cpsr_c,#(SYS_MODE)  ; allow interrupt nesting
    mov     lr,pc
    bx      r2              ; Jump to Interrupt Service Routine
	MSR cpsr_c,#(SYS_MODE | NO_IRQ )  ; disable interrupts
    ldmfd   sp!, {r3}       ; Restore original ICCIAR value
    ldr     r0,=ICCEOIR
    str     r3,[r0]         ; Update End of Interrupt register with original value from ICCIAR
END_IRQ_HLDR

;/* IRQ exit {{{ */
    MSR     cpsr_c,#(SYS_MODE | NO_INT) ;/* SYSTEM mode, IRQ/FIQ disabled */
    MOV     r0,sp               ;/* make sp_SYS visible to IRQ mode */
    ADD     sp,sp,#(16*4)       ; /* fake unstacking 8 registers from sp_SYS */

    MSR     cpsr_c,#(IRQ_MODE | NO_INT) ;/* IRQ mode, both IRQ/FIQ disabled */
    MOV     sp,r0               ;/* copy sp_SYS to sp_IRQ */
;    LDR     r0,[sp,#(7*4)]      ;/* load the saved SPSR from the stack */
    LDR     r0,[sp,#(15*4)]      ;/* load the saved SPSR from the stack */
    MSR     spsr_cxsf,r0        ;/* copy it into spsr_IRQ */

    LDMFD   sp,{r0-r12,lr}^  ;/* unstack all saved USER/SYSTEM registers */
    NOP                         ;/* can't access banked reg immediately */
;    LDR     lr,[sp,#(6*4)]      ;/* load return address from the SYS stack */
    LDR     lr,[sp,#(14*4)]      ;/* load return address from the SYS stack */
    MOVS    pc,lr               ;/* return restoring CPSR from SPSR */
;/* IRQ exit }}} */


FIQ_Routine
    stmfd   sp!, {r0-r7}
    stmfd   sp!, {r8-r12,lr}

    ldr     r0,=ICCIAR      ; Interrupt Acknowledge register
    ldr     r3,[r0]         ; r0[12:10] = CPU_ID of software interrupts, r0[9:0] = INT_ID
    ldr     r2,=0x00001C00
    and     r1,r3,r2
    lsr     r1,r1,#10       ; Keep CPU_ID at r1
    ldr     r2,=0x00000200
    and     r2,r3,r2        
    cmp     r2,#0           ; Check that INT_ID is not 1023 or 1022 (spurious interrupt)
    bne     END_FIQ_HLDR
    ldr     r2,=0x000001FF
    and     r0,r3,r2        ; Keep INT_ID at r0
    stmfd   sp!, {r3}       ; Save original ICCIAR value

    lsl     r2,r0,#2
    ldr     lr,=interrupt_handlers
    ldr     r2,[lr,r2]
    mov     lr,pc
    bx      r2              ; Jump to Interrupt Service Routine
    ldmfd   sp!, {r3}       ; Restore original ICCIAR value
    ldr     r0,=ICCEOIR
    str     r3,[r0]         ; Update End of Interrupt register with original value from ICCIAR
END_FIQ_HLDR
    mrs     r2, cpsr
    bic     r0, r2, #0x100
    msr     cpsr_cxsf, r0
    ldmfd   sp!, {r8-r12,lr}
    ldmfd   sp!, {r0-r7}
    subs    pc,lr,#4

SMI_Routine
    stmfd   sp!, {r0-r1,lr}        ; push register onto SMI stack
    ldr     r1,=smi_hdler          ; >= 16: pointer to standard SMI
    ldr     r0,[r1]                ; handler
    mov     lr,pc                  ; >= 16: set link register
    bx      r0                     ; >= 16: jump to standard SMI
    ldmfd   sp!, {r0-r1,pc}

  END
