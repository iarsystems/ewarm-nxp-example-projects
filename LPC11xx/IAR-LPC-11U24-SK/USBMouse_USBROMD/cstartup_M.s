/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2009 IAR Systems. All rights reserved.
 *
 * $Revision: 28 $
 *
 **************************************************/

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
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start

        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        DATA
__vector_table
        DCD     sfe(CSTACK)                 ; Top of Stack
        DCD     __iar_program_start         ; Reset Handler
        DCD     NMI_Handler                 ; NMI Handler
        DCD     HardFault_Handler           ; Hard Fault Handler
        DCD     MemManage_Handler           ; MPU Fault Handler
        DCD     BusFault_Handler            ; Bus Fault Handler
        DCD     UsageFault_Handler          ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     DebugMon_Handler            ; Debug Monitor Handler
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; PendSV Ha dler
        DCD     SysTick_Handler             ; SysTick Handler
        ; External Interrupts
        DCD     PIN_INT0_IRQHandler         ; All GPIO pin can be routed to FLEX_INTx
        DCD     PIN_INT1_IRQHandler
        DCD     PIN_INT2_IRQHandler
        DCD     PIN_INT3_IRQHandler
        DCD     PIN_INT4_IRQHandler
        DCD     PIN_INT5_IRQHandler
        DCD     PIN_INT6_IRQHandler
        DCD     PIN_INT7_IRQHandler
        DCD     GINT0_IRQHandler
        DCD     GINT1_IRQHandler            ; PIO0 (0:7)
        DCD     Reserved_IRQHandler         ; Reserved
        DCD     Reserved_IRQHandler
        DCD     Reserved_IRQHandler
        DCD     Reserved_IRQHandler
        DCD     SSP1_IRQHandler             ; SSP1
        DCD     I2C_IRQHandler              ; I2C
        DCD     CT16B0_IRQHandler           ; 16-bit Timer0
        DCD     CT16B1_IRQHandler           ; 16-bit Timer1
        DCD     CT32B0_IRQHandler           ; 32-bit Timer0
        DCD     CT32B1_IRQHandler           ; 32-bit Timer1
        DCD     SSP0_IRQHandler             ; SSP0
        DCD     USART_IRQHandler            ; USART
        DCD     USB_IRQHandler              ; USB IRQ
        DCD     USB_FIQHandler              ; USB FIQ
        DCD     ADC_IRQHandler              ; A/D Converter
        DCD     WWDT_IRQHandler             ; Watchdog timer
        DCD     BOD_IRQHandler              ; Brown Out Detect
        DCD     FLASH_IRQHandler            ; IP2111 Flash Memory Controller
        DCD     Reserved_IRQHandler         ; Reserved
        DCD     Reserved_IRQHandler         ; Reserved
        DCD     USB_WAKEUP_IRQHandler       ; USB wake up
        DCD     Reserved_IRQHandler         ; Reserved


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        SECTION .text:CODE:REORDER:NOROOT(1)
        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK MemManage_Handler
        PUBWEAK BusFault_Handler
        PUBWEAK UsageFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler
        PUBWEAK PIN_INT0_IRQHandler
        PUBWEAK PIN_INT1_IRQHandler
        PUBWEAK PIN_INT2_IRQHandler
        PUBWEAK PIN_INT3_IRQHandler
        PUBWEAK PIN_INT4_IRQHandler
        PUBWEAK PIN_INT5_IRQHandler
        PUBWEAK PIN_INT6_IRQHandler
        PUBWEAK PIN_INT7_IRQHandler
        PUBWEAK GINT0_IRQHandler
        PUBWEAK GINT1_IRQHandler
        PUBWEAK SSP1_IRQHandler
        PUBWEAK I2C_IRQHandler
        PUBWEAK CT16B0_IRQHandler
        PUBWEAK CT16B1_IRQHandler
        PUBWEAK CT32B0_IRQHandler
        PUBWEAK CT32B1_IRQHandler
        PUBWEAK SSP0_IRQHandler
        PUBWEAK USART_IRQHandler
        PUBWEAK USB_IRQHandler
        PUBWEAK USB_FIQHandler
        PUBWEAK ADC_IRQHandler
        PUBWEAK WWDT_IRQHandler
        PUBWEAK BOD_IRQHandler
        PUBWEAK FLASH_IRQHandler
        PUBWEAK USB_WAKEUP_IRQHandler
        PUBWEAK Reserved_IRQHandler

NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
PIN_INT0_IRQHandler
PIN_INT1_IRQHandler
PIN_INT2_IRQHandler
PIN_INT3_IRQHandler
PIN_INT4_IRQHandler
PIN_INT5_IRQHandler
PIN_INT6_IRQHandler
PIN_INT7_IRQHandler
GINT0_IRQHandler
GINT1_IRQHandler
SSP1_IRQHandler
I2C_IRQHandler
CT16B0_IRQHandler
CT16B1_IRQHandler
CT32B0_IRQHandler
CT32B1_IRQHandler
SSP0_IRQHandler
USART_IRQHandler
USB_IRQHandler
USB_FIQHandler
ADC_IRQHandler
WWDT_IRQHandler
BOD_IRQHandler
FLASH_IRQHandler
USB_WAKEUP_IRQHandler
Reserved_IRQHandler
Default_Handler:
        B Default_Handler

        SECTION .crp:CODE:ROOT(2)
        DATA
/* Code Read Protection
NO_ISP  0x4E697370 -  Prevents sampling of pin PIO0_1 for entering ISP mode
CRP1    0x12345678 - Write to RAM command cannot access RAM below 0x10000300.
                   - Copy RAM to flash command can not write to Sector 0.
                   - Erase command can erase Sector 0 only when all sectors
                     are selected for erase.
                   - Compare command is disabled.
                   - Read Memory command is disabled.
CRP2    0x87654321 - Read Memory is disabled.
                   - Write to RAM is disabled.
                   - "Go" command is disabled.
                   - Copy RAM to flash is disabled.
                   - Compare is disabled.
CRP3    0x43218765 - Access to chip via the SWD pins is disabled. ISP entry
                     by pulling PIO0_1 LOW is disabled if a valid user code is
                     present in flash sector 0.
Caution: If CRP3 is selected, no future factory testing can be
performed on the device.
*/
  DCD 0xFFFFFFFF

        END
