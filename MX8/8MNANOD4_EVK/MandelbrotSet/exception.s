;*******************************************************************************
;                               Copyright 2020 IAR Systems. All rights reserved.
;Function: Cortex-A53(AARCH64) Exception
;
;Note:
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

;*******************************************************************************
        SECTION .ErrorException:CODE:ROOT(7)
        B   Error_Handler_A64

;*******************************************************************************
        SECTION .FIQException:CODE:ROOT(7)
        B   FIQ_Handler_A64

;*******************************************************************************
        SECTION .IRQException:CODE:ROOT(7)
        B   IRQ_Handler_A64

;*******************************************************************************
        SECTION .SynchronousException:CODE:ROOT(7)
        B   Synchronous_Handler_A64

        SECTION .text:CODE:NOROOT:REORDER(2)

        PUBWEAK  Error_Handler_A64
        PUBWEAK  FIQ_Handler_A64
        PUBWEAK  IRQ_Handler_A64
        PUBWEAK  Synchronous_Handler_A64

Error_Handler_A64:
IRQ_Handler_A64:
FIQ_Handler_A64:
Synchronous_Handler_A64:
__default_handler:
        B .

        END
