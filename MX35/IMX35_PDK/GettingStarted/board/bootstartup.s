;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Start up code for iMX35 bootloader,
;; Copy 2K from NFC RAM Buffer to internal RAM
;; Init bootloader stack and jumps to bootloader
;;
;; Copyright 2008 IAR Systems. All rights reserved.
;;
;; $Revision: 7859 $
;;

        MODULE  ?bootloader

        SECTION .bootstartup:CODE:ROOT(2)

        EXTERN  bootload
        ARM                    ; Always ARM mode after reset

NFCRAM  DEFINE 0xBB000000      ; NFC RAM Start Address
INTRRAM DEFINE 0x10000000      ; Internal RAM Start Address
SIZE    DEFINE 0x1000          ; Size to copy 4K
STACK   DEFINE 0x1000FFFF      ; Stack For the Bootloader

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
