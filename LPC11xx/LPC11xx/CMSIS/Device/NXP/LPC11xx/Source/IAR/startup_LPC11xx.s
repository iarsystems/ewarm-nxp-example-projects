;/**************************************************************************//**
; * @file     startup_<Device>.s
; * @brief    CMSIS Cortex-M# Core Device Startup File for
; *           for the NXP LPC11xx Device Series
; * @version  V3.01
; * @date     06. March 2012
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD     WAKEUP_IRQHandler         ; 16+ 0: Wakeup PIO0.0
        DCD     WAKEUP_IRQHandler         ; 16+ 1: Wakeup PIO0.1
        DCD     WAKEUP_IRQHandler         ; 16+ 2: Wakeup PIO0.2
        DCD     WAKEUP_IRQHandler         ; 16+ 3: Wakeup PIO0.3
        DCD     WAKEUP_IRQHandler         ; 16+ 4: Wakeup PIO0.4
        DCD     WAKEUP_IRQHandler         ; 16+ 5: Wakeup PIO0.5
        DCD     WAKEUP_IRQHandler         ; 16+ 6: Wakeup PIO0.6
        DCD     WAKEUP_IRQHandler         ; 16+ 7: Wakeup PIO0.7
        DCD     WAKEUP_IRQHandler         ; 16+ 8: Wakeup PIO0.8
        DCD     WAKEUP_IRQHandler         ; 16+ 9: Wakeup PIO0.9
        DCD     WAKEUP_IRQHandler         ; 16+10: Wakeup PIO0.10
        DCD     WAKEUP_IRQHandler         ; 16+11: Wakeup PIO0.11
        DCD     WAKEUP_IRQHandler         ; 16+12: Wakeup PIO1.0
        DCD     0                         ; 16+13: Reserved
        DCD     SSP1_IRQHandler           ; 16+14: SSP1
        DCD     I2C_IRQHandler            ; 16+15: I2C
        DCD     TIMER16_0_IRQHandler      ; 16+16: 16-bit Counter-Timer 0
        DCD     TIMER16_1_IRQHandler      ; 16+17: 16-bit Counter-Timer 1
        DCD     TIMER32_0_IRQHandler      ; 16+18: 32-bit Counter-Timer 0
        DCD     TIMER32_1_IRQHandler      ; 16+19: 32-bit Counter-Timer 1
        DCD     SSP0_IRQHandler           ; 16+20: SSP0
        DCD     UART_IRQHandler           ; 16+21: UART
        DCD     0                         ; 16+22: Reserved
        DCD     0                         ; 16+23: Reserved
        DCD     ADC_IRQHandler            ; 16+24: A/D Converter
        DCD     WDT_IRQHandler            ; 16+25: Watchdog Timer
        DCD     BOD_IRQHandler            ; 16+26: Brown Out Detect
        DCD     0                         ; 16+27: Reserved
        DCD     PIOINT3_IRQHandler        ; 16+28: PIO INT3
        DCD     PIOINT2_IRQHandler        ; 16+29: PIO INT2
        DCD     PIOINT1_IRQHandler        ; 16+30: PIO INT1
        DCD     PIOINT0_IRQHandler        ; 16+31: PIO INT0
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK WAKEUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WAKEUP_IRQHandler
        B WAKEUP_IRQHandler

        PUBWEAK SSP1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP1_IRQHandler
        B SSP1_IRQHandler

        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C_IRQHandler
        B I2C_IRQHandler

        PUBWEAK TIMER16_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER16_0_IRQHandler
        B TIMER16_0_IRQHandler

        PUBWEAK TIMER16_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER16_1_IRQHandler
        B TIMER16_1_IRQHandler

        PUBWEAK TIMER32_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER32_0_IRQHandler
        B TIMER32_0_IRQHandler

        PUBWEAK TIMER32_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER32_1_IRQHandler
        B TIMER32_1_IRQHandler

        PUBWEAK SSP0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP0_IRQHandler
        B SSP0_IRQHandler

        PUBWEAK UART_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART_IRQHandler
        B UART_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK WDT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_IRQHandler
        B WDT_IRQHandler

        PUBWEAK BOD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BOD_IRQHandler
        B BOD_IRQHandler

        PUBWEAK PIOINT3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PIOINT3_IRQHandler
        B PIOINT3_IRQHandler

        PUBWEAK PIOINT2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PIOINT2_IRQHandler
        B PIOINT2_IRQHandler

        PUBWEAK PIOINT1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PIOINT1_IRQHandler
        B PIOINT1_IRQHandler

        PUBWEAK PIOINT0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PIOINT0_IRQHandler
        B PIOINT0_IRQHandler
        
#ifndef SRAM
        SECTION .crp:CODE:ROOT(2)
        DATA
/* Code Read Protection
CRP1    0x12345678 - Write to RAM command can not access RAM below 0x10000200.
                   - Read Memory command: disabled.
                   - Copy RAM to Flash command: cannot write to Sector 0.
                   - "Go" command: disabled.
                   - Erase sector(s) command: can erase any individual sector except 
                   	 sector 0 only, or can erase all sectors at once.
                   - Compare command: disabled
CRP2    0x87654321 - Write to RAM command: disabled.
                   - Copy RAM to Flash: disabled.
                   - Erase command: only allows erase of all sectors.
CRP3    0x43218765 - Access to chip via the SWD pins is disabled. ISP entry
                     by pulling PIO0_1 LOW is disabled if a valid user code is
                     present in flash sector 0.
Caution: If CRP3 is selected, no future factory testing can be
performed on the device.
*/
	DCD	0xFFFFFFFF
#endif
        END
