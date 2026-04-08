; ---------------------------------------------------------------------------------------
;  @file:    startup_MCIMX7U5_M4.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MCIMX7U5_M4
;  @version: 2.0
;  @date:    2016-7-19
;  @build:   b160722
; ---------------------------------------------------------------------------------------
;
; Copyright (c) 1997 - 2016 , Freescale Semiconductor, Inc.
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;
; o Redistributions of source code must retain the above copyright notice, this list
;   of conditions and the following disclaimer.
;
; o Redistributions in binary form must reproduce the above copyright notice, this
;   list of conditions and the following disclaimer in the documentation and/or
;   other materials provided with the distribution.
;
; o Neither the name of Freescale Semiconductor, Inc. nor the names of its
;   contributors may be used to endorse or promote products derived from this
;   software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
; ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

        EXTERN  __cmain
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

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     CTI0_IRQHandler                               ;Cross Trigger Interface for CM4
        DCD     DMA0_0_4_IRQHandler                           ;DMA Channel 0, 4 Transfer Complete
        DCD     DMA0_1_5_IRQHandler                           ;DMA Channel 1, 5 Transfer Complete
        DCD     DMA0_2_6_IRQHandler                           ;DMA Channel 2, 6 Transfer Complete
        DCD     DMA0_3_7_IRQHandler                           ;DMA Channel 3, 7 Transfer Complete
        DCD     DMA0_8_12_IRQHandler                          ;DMA Channel 8, 12 Transfer Complete
        DCD     DMA0_9_13_IRQHandler                          ;DMA Channel 9, 13 Transfer Complete
        DCD     DMA0_10_14_IRQHandler                         ;DMA Channel 10, 14 Transfer Complete
        DCD     DMA0_11_15_IRQHandler                         ;DMA Channel 11, 15 Transfer Complete
        DCD     DMA0_16_20_IRQHandler                         ;DMA Channel 16, 20 Transfer Complete
        DCD     DMA0_17_21_IRQHandler                         ;DMA Channel 17, 21 Transfer Complete
        DCD     DMA0_18_22_IRQHandler                         ;DMA Channel 18, 22 Transfer Complete
        DCD     DMA0_19_23_IRQHandler                         ;DMA Channel 19, 23 Transfer Complete
        DCD     DMA0_24_28_IRQHandler                         ;DMA Channel 24, 28 Transfer Complete
        DCD     DMA0_25_29_IRQHandler                         ;DMA Channel 25, 29 Transfer Complete
        DCD     DMA0_26_30_IRQHandler                         ;DMA Channel 26, 30 Transfer Complete
        DCD     DMA0_27_31_IRQHandler                         ;DMA Channel 27, 31 Transfer Complete
        DCD     DMA0_Error_IRQHandler                         ;DMA Error Interrupt - All Channels
        DCD     MCM0_IRQHandler                               ;MCM Interrupt
        DCD     EWM_IRQHandler                                ;External Watchdog Monitor Interrupt
        DCD     LLWU0_IRQHandler                              ;Low Leakage Wake Up
        DCD     SIM_IRQHandler                                ;System Integation Module
        DCD     MU_A_IRQHandler                               ;Messaging Unit - Side A
        DCD     Reserved39_IRQHandler                         ;Secured JTAG Controller
        DCD     Software1_IRQHandler                          ;Software Interrupt
        DCD     Software2_IRQHandler                          ;Software Interrupt
        DCD     WDOG0_IRQHandler                              ;Watchdog Interrupt
        DCD     SCG0_IRQHandler                               ;System Clock Generator for M4 domain
        DCD     QSPI_IRQHandler                               ;Quad Serial Peripheral Interface
        DCD     LTC_IRQHandler                                ;Low Power Trusted Cryptography
        DCD     Reserved46_IRQHandler                         ;Reserved
        DCD     SNVS_IRQHandler                               ;Secure Non-Volatile Storage Consolidated Interrupt
        DCD     TRNG0_IRQHandler                              ;Random Number Generator
        DCD     LPIT0_IRQHandler                              ;Low Power Periodic Interrupt Timer
        DCD     PMC0_IRQHandler                               ;Power Management  Control interrupts for M4 domain
        DCD     CMC0_IRQHandler                               ;Core Mode Controller interrupts for M4 domain
        DCD     LPTMR0_IRQHandler                             ;Low Power Timer
        DCD     LPTMR1_IRQHandler                             ;Low Power Timer
        DCD     TPM0_IRQHandler                               ;Timer PWM module
        DCD     TPM1_IRQHandler                               ;Timer PWM module
        DCD     TPM2_IRQHandler                               ;Timer PWM module
        DCD     TPM3_IRQHandler                               ;Timer PWM module
        DCD     FLEXIO0_IRQHandler                            ;Flexible IO
        DCD     LPI2C0_IRQHandler                             ;Inter-integrated circuit 0
        DCD     LPI2C1_IRQHandler                             ;Inter-integrated circuit 1
        DCD     LPI2C2_IRQHandler                             ;Inter-integrated circuit 2
        DCD     LPI2C3_IRQHandler                             ;Inter-integrated circuit 3
        DCD     SAI0_IRQHandler                               ;Serial Audio Interface
        DCD     SAI1_IRQHandler                               ;Serial Audio Interface 1
        DCD     LPSPI0_IRQHandler                             ;Low Power Serial Peripheral Interface
        DCD     LPSPI1_IRQHandler                             ;Low Power Serial Peripheral Interface
        DCD     LPUART0_IRQHandler                            ;Low Power UART
        DCD     LPUART1_IRQHandler                            ;Low Power UART
        DCD     LPUART2_IRQHandler                            ;Low Power UART
        DCD     LPUART3_IRQHandler                            ;Low Power UART
        DCD     DPM_IRQHandler                                ;Dynamic Process Monitor
        DCD     PCTLA_IRQHandler                              ;Port A pin interrupt
        DCD     PCTLB_IRQHandler                              ;Port B pin interrupt
        DCD     ADC0_IRQHandler                               ;Analog to Digital Convertor
        DCD     ADC1_IRQHandler                               ;Analog to Digital Convertor
        DCD     CMP0_IRQHandler                               ;Comparator
        DCD     CMP1_IRQHandler                               ;Comparator
        DCD     DAC0_IRQHandler                               ;Digital to Analog Convertor
        DCD     DAC1_IRQHandler                               ;Digital to Analog Convertor
        DCD     WDOG1_IRQHandler                              ;Watchdog Interrupt from A7 subsystem
        DCD     USB0_IRQHandler                               ;USB 0 Interrupt from A7 subsystem
        DCD     USB1_IRQHandler                               ;USB 1 Interrupt from A7 subsystem
        DCD     Reserved83_IRQHandler                         ;
        DCD     WDOG2_IRQHandler                              ;Watchdog Interrupt from A7 subsystem
        DCD     USBPHY_IRQHandler                             ;USB PHY (used in conjunction with USBOTG0)
        DCD     CMC1_IRQHandler                               ;A7 resets
        DCD     DefaultISR                                    ;87
        DCD     DefaultISR                                    ;88
        DCD     DefaultISR                                    ;89
        DCD     DefaultISR                                    ;90
        DCD     DefaultISR                                    ;91
        DCD     DefaultISR                                    ;92
        DCD     DefaultISR                                    ;93
        DCD     DefaultISR                                    ;94
        DCD     DefaultISR                                    ;95
        DCD     DefaultISR                                    ;96
        DCD     DefaultISR                                    ;97
        DCD     DefaultISR                                    ;98
        DCD     DefaultISR                                    ;99
        DCD     DefaultISR                                    ;100
        DCD     DefaultISR                                    ;101
        DCD     DefaultISR                                    ;102
        DCD     DefaultISR                                    ;103
        DCD     DefaultISR                                    ;104
        DCD     DefaultISR                                    ;105
        DCD     DefaultISR                                    ;106
        DCD     DefaultISR                                    ;107
        DCD     DefaultISR                                    ;108
        DCD     DefaultISR                                    ;109
        DCD     DefaultISR                                    ;110
        DCD     DefaultISR                                    ;111
        DCD     DefaultISR                                    ;112
        DCD     DefaultISR                                    ;113
        DCD     DefaultISR                                    ;114
        DCD     DefaultISR                                    ;115
        DCD     DefaultISR                                    ;116
        DCD     DefaultISR                                    ;117
        DCD     DefaultISR                                    ;118
        DCD     DefaultISR                                    ;119
        DCD     DefaultISR                                    ;120
        DCD     DefaultISR                                    ;121
        DCD     DefaultISR                                    ;122
        DCD     DefaultISR                                    ;123
        DCD     DefaultISR                                    ;124
        DCD     DefaultISR                                    ;125
        DCD     DefaultISR                                    ;126
        DCD     DefaultISR                                    ;127
        DCD     DefaultISR                                    ;128
        DCD     DefaultISR                                    ;129
        DCD     DefaultISR                                    ;130
        DCD     DefaultISR                                    ;131
        DCD     DefaultISR                                    ;132
        DCD     DefaultISR                                    ;133
        DCD     DefaultISR                                    ;134
        DCD     DefaultISR                                    ;135
        DCD     DefaultISR                                    ;136
        DCD     DefaultISR                                    ;137
        DCD     DefaultISR                                    ;138
        DCD     DefaultISR                                    ;139
        DCD     DefaultISR                                    ;140
        DCD     DefaultISR                                    ;141
        DCD     DefaultISR                                    ;142
        DCD     DefaultISR                                    ;143
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
        CPSID   I               ; Mask interrupts
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R0, =SystemInit
        BLX     R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =__cmain
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK CTI0_IRQHandler
        PUBWEAK DMA0_0_4_IRQHandler
        PUBWEAK DMA0_0_4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_0_4_IRQHandler
        LDR     R0, =DMA0_0_4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_1_5_IRQHandler
        PUBWEAK DMA0_1_5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_1_5_IRQHandler
        LDR     R0, =DMA0_1_5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_2_6_IRQHandler
        PUBWEAK DMA0_2_6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_2_6_IRQHandler
        LDR     R0, =DMA0_2_6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_3_7_IRQHandler
        PUBWEAK DMA0_3_7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_3_7_IRQHandler
        LDR     R0, =DMA0_3_7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_8_12_IRQHandler
        PUBWEAK DMA0_8_12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_8_12_IRQHandler
        LDR     R0, =DMA0_8_12_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_9_13_IRQHandler
        PUBWEAK DMA0_9_13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_9_13_IRQHandler
        LDR     R0, =DMA0_9_13_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_10_14_IRQHandler
        PUBWEAK DMA0_10_14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_10_14_IRQHandler
        LDR     R0, =DMA0_10_14_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_11_15_IRQHandler
        PUBWEAK DMA0_11_15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_11_15_IRQHandler
        LDR     R0, =DMA0_11_15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_16_20_IRQHandler
        PUBWEAK DMA0_16_20_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_16_20_IRQHandler
        LDR     R0, =DMA0_16_20_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_17_21_IRQHandler
        PUBWEAK DMA0_17_21_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_17_21_IRQHandler
        LDR     R0, =DMA0_17_21_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_18_22_IRQHandler
        PUBWEAK DMA0_18_22_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_18_22_IRQHandler
        LDR     R0, =DMA0_18_22_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_19_23_IRQHandler
        PUBWEAK DMA0_19_23_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_19_23_IRQHandler
        LDR     R0, =DMA0_19_23_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_24_28_IRQHandler
        PUBWEAK DMA0_24_28_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_24_28_IRQHandler
        LDR     R0, =DMA0_24_28_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_25_29_IRQHandler
        PUBWEAK DMA0_25_29_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_25_29_IRQHandler
        LDR     R0, =DMA0_25_29_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_26_30_IRQHandler
        PUBWEAK DMA0_26_30_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_26_30_IRQHandler
        LDR     R0, =DMA0_26_30_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_27_31_IRQHandler
        PUBWEAK DMA0_27_31_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_27_31_IRQHandler
        LDR     R0, =DMA0_27_31_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_Error_IRQHandler
        PUBWEAK DMA0_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_Error_IRQHandler
        LDR     R0, =DMA0_Error_DriverIRQHandler
        BX      R0

        PUBWEAK MCM0_IRQHandler
        PUBWEAK EWM_IRQHandler
        PUBWEAK LLWU0_IRQHandler
        PUBWEAK SIM_IRQHandler
        PUBWEAK MU_A_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK Software1_IRQHandler
        PUBWEAK Software2_IRQHandler
        PUBWEAK WDOG0_IRQHandler
        PUBWEAK SCG0_IRQHandler
        PUBWEAK QSPI_IRQHandler
        PUBWEAK QSPI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
QSPI_IRQHandler
        LDR     R0, =QSPI_DriverIRQHandler
        BX      R0

        PUBWEAK LTC_IRQHandler
        PUBWEAK Reserved46_IRQHandler
        PUBWEAK SNVS_IRQHandler
        PUBWEAK TRNG0_IRQHandler
        PUBWEAK LPIT0_IRQHandler
        PUBWEAK PMC0_IRQHandler
        PUBWEAK CMC0_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK LPTMR1_IRQHandler
        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK TPM3_IRQHandler
        PUBWEAK FLEXIO0_IRQHandler
        PUBWEAK FLEXIO0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO0_IRQHandler
        LDR     R0, =FLEXIO0_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C0_IRQHandler
        PUBWEAK LPI2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C0_IRQHandler
        LDR     R0, =LPI2C0_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C1_IRQHandler
        LDR     R0, =LPI2C1_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C2_IRQHandler
        LDR     R0, =LPI2C2_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C3_IRQHandler
        PUBWEAK LPI2C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C3_IRQHandler
        LDR     R0, =LPI2C3_DriverIRQHandler
        BX      R0

        PUBWEAK SAI0_IRQHandler
        PUBWEAK SAI1_IRQHandler
        PUBWEAK LPSPI0_IRQHandler
        PUBWEAK LPSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI0_IRQHandler
        LDR     R0, =LPSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI1_IRQHandler
        LDR     R0, =LPSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART3_IRQHandler
        PUBWEAK LPUART3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART3_IRQHandler
        LDR     R0, =LPUART3_DriverIRQHandler
        BX      R0

        PUBWEAK DPM_IRQHandler
        PUBWEAK PCTLA_IRQHandler
        PUBWEAK PCTLB_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK DAC1_IRQHandler
        PUBWEAK WDOG1_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK USB1_IRQHandler
        PUBWEAK Reserved83_IRQHandler
        PUBWEAK WDOG2_IRQHandler
        PUBWEAK USBPHY_IRQHandler
        PUBWEAK CMC1_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
CTI0_IRQHandler
DMA0_0_4_DriverIRQHandler
DMA0_1_5_DriverIRQHandler
DMA0_2_6_DriverIRQHandler
DMA0_3_7_DriverIRQHandler
DMA0_8_12_DriverIRQHandler
DMA0_9_13_DriverIRQHandler
DMA0_10_14_DriverIRQHandler
DMA0_11_15_DriverIRQHandler
DMA0_16_20_DriverIRQHandler
DMA0_17_21_DriverIRQHandler
DMA0_18_22_DriverIRQHandler
DMA0_19_23_DriverIRQHandler
DMA0_24_28_DriverIRQHandler
DMA0_25_29_DriverIRQHandler
DMA0_26_30_DriverIRQHandler
DMA0_27_31_DriverIRQHandler
DMA0_Error_DriverIRQHandler
MCM0_IRQHandler
EWM_IRQHandler
LLWU0_IRQHandler
SIM_IRQHandler
MU_A_IRQHandler
Reserved39_IRQHandler
Software1_IRQHandler
Software2_IRQHandler
WDOG0_IRQHandler
SCG0_IRQHandler
QSPI_DriverIRQHandler
LTC_IRQHandler
Reserved46_IRQHandler
SNVS_IRQHandler
TRNG0_IRQHandler
LPIT0_IRQHandler
PMC0_IRQHandler
CMC0_IRQHandler
LPTMR0_IRQHandler
LPTMR1_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
TPM3_IRQHandler
FLEXIO0_DriverIRQHandler
LPI2C0_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPI2C2_DriverIRQHandler
LPI2C3_DriverIRQHandler
SAI0_IRQHandler
SAI1_IRQHandler
LPSPI0_DriverIRQHandler
LPSPI1_DriverIRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
LPUART3_DriverIRQHandler
DPM_IRQHandler
PCTLA_IRQHandler
PCTLB_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
WDOG1_IRQHandler
USB0_IRQHandler
USB1_IRQHandler
Reserved83_IRQHandler
WDOG2_IRQHandler
USBPHY_IRQHandler
CMC1_IRQHandler
DefaultISR
        B DefaultISR

        END
