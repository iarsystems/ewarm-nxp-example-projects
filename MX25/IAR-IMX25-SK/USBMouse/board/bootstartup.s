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

        SECTION .intvec:CODE:NOROOT(2)
        SECTION .bootstartup:CODE:ROOT(2)

		    EXTERN	bootload
        EXTERN	__region_RAM_sram_start__
        PUBLIC __boot_entry
        PUBLIC __boot_entry_copy_end
                ARM	; Always ARM mode after reset	
__boot_entry
NFCRAM  DEFINE 0xBB000000       ; NFC RAM Start Address
SIZE    DEFINE 0x1000           ; Size to copy 4K

;copy bootloader in to internal RAM
        ldr    r0,=NFCRAM     ;
        ldr    r1,=SFB(.bootstartup);
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
__boot_entry_copy_end
        ldr         sp,=__region_RAM_sram_start__
;start bootloader
        ldr         r0,=bootload
        bx          r0


                END
