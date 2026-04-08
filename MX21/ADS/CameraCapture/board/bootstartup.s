;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Start up code for iMX31 bootloader,
;; Copy 2K from NFC RAM Buffer to internal RAM
;; Init bootloader stack and jumps to bootloader
;;
;; Copyright 2008 IAR Systems. All rights reserved.
;;
;; $Revision: 16 $
;;

        MODULE  ?bootloader

        SECTION .bootstartup:CODE:ROOT(2)

		    EXTERN	bootload
                ARM	; Always ARM mode after reset	

NFCRAM  DEFINE 0xDF003000      ; NFC RAM Start Address
INTRRAM DEFINE 0xFFFFE800      ; Internal RAM Start Address
SIZE    DEFINE 0x800           ; Size to copy 2K
STACK   DEFINE 0xFFFFFF00      ; Stack For the Bootloader
MODE_MSK DEFINE 0x1F            ; Bit mask for mode bits in CPSR


CP_DIS_MASK         DEFINE  0xFFFFEFFA
 // Disable Addr translation, D cache and enable I cache
        MRC         p15,0,R1,C1,C0,0
        LDR         R0,=CP_DIS_MASK      ;; 0xFFFFEFFA
        AND         R1,R1,R0
        ORR         R1,R1,#(1<<12)
        MCR         p15,0,R1,C1,C0,0


;copy bootloader in to internal RAM
        ldr    r0,=NFCRAM     ;
        ldr    r1,=INTRRAM    ;
        mov    r2,#0x0        ;

  __loop:
        ldr    r3,[r0,#+0]  ;
        str    r3,[r1,#+0]  ;
        adds   r0,r0,#+4    ;
        adds   r1,r1,#+4    ;
        adds   r2,r2,#+4    ; 
        
        cmp    r2,#SIZE     ;
        bcc    __loop       ;
;Set bootloader stack
         ldr     sp,=STACK;
;start bootloader
        ldr         r0,=bootload
        bx          r0


                END
