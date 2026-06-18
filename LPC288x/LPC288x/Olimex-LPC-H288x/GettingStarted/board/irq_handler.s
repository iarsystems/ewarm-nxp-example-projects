;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; IRQ nested handler
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 28 $
;;

I_BIT     DEFINE  (1<<7)

INT_PRIOMASK0_REG DEFINE  0x80300000
;---------------------------------------------------------------
; ?IRQHandler
;---------------------------------------------------------------
    MODULE  ?IRQHandler

      SECTION .text:CODE:NOROOT(2)

    PUBLIC  ?IRQHandler
    PUBLIC  __IRQHandlerEnd
    CODE32
?IRQHandler
                sub     lr,lr,#4                ; Update the link register
                ; save context
                stmfd   sp!,{r0-r2,r4,lr}
                mrs     r0,spsr
                ldr     r4,=INT_PRIOMASK0_REG
                ldr     r2,[r4,#0x100]          ; get interrupt index and base address of the interrupt's table
                ldr     r1,[r4]                 ; get priority mask interrupt level
                stmfd   sp!,{r0,r1}
                ldmia   r2,{r0,r1}
                str     r1,[r4]                 ; update priority mask interrupt level
                msr     cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
                stmfd   sp!,{lr}                ; Save the link register.
                ldr     lr,=__IRQHandlerEnd
                bx      r0
__IRQHandlerEnd:
                ldmfd   sp!,{lr}                ; Restore the link register.
                msr     cpsr_c,#0xD2 | I_BIT    ; Switch to IRQ mode and disable IRQ
                ldmfd   sp!,{r0,r1}             ; Restore the saved spsr_mode into r0 and INT_VECTOR0 into r2
                str     r1,[r4]                 ; Restore INT_VECTOR0
                msr     spsr_cxsf,r0            ; Restore spsr_mode
                ldmfd   sp!,{r0-r2,r4,pc}^      ; Return to the instruction following...
  END
