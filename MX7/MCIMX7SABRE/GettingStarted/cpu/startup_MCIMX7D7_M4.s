; ---------------------------------------------------------------------------------------
;  @file:    startup_MCIMX7D7_M4.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MCIMX7D7_M4
;  @version: 1.0
;  @date:    2016-4-13
;  @build:   b170615
; ---------------------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2017 NXP
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;
; 1. Redistributions of source code must retain the above copyright notice, this list
;   of conditions and the following disclaimer.
;
; 2. Redistributions in binary form must reproduce the above copyright notice, this
;   list of conditions and the following disclaimer in the documentation and/or
;   other materials provided with the distribution.
;
; 3. Neither the name of the copyright holder nor the names of its
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

        EXTERN  __iar_program_start
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     __iar_program_start

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
        DCD     GPR_IRQ_IRQHandler                            ;GPR Interrupt. Used to notify cores on exception condition while boot.
        DCD     DAP_IRQHandler                                ;DAP Interrupt
        DCD     SDMA_IRQHandler                               ;AND of all 48 SDMA interrupts (events) from all the channels
        DCD     DBGMON_IRQHandler                             ;DBGMON Sync Interrupt
        DCD     SNVS_IRQHandler                               ;ON-OFF button press shorter than 5 seconds (pulse event)
        DCD     LCDIF_IRQHandler                              ;LCDIF Sync Interrupt
        DCD     SIM2_IRQHandler                               ;SIM2 Interrupt
        DCD     CSI_IRQHandler                                ;CSI Interrupt
        DCD     PXP0_IRQHandler                               ;PXP Interrupt 0
        DCD     Reserved25_IRQHandler                         ;Reserved
        DCD     WDOG3_IRQHandler                              ;Watchdog Timer reset
        DCD     HS0_IRQHandler                                ;HS Interrupt Request 0
        DCD     APBHDMA_IRQHandler                            ;GPMI operation channel 0 description complete interrupt
        DCD     EIM_IRQHandler                                ;EIM Interrupt
        DCD     BCH_IRQHandler                                ;BCH operation complete interrupt
        DCD     GPMI_IRQHandler                               ;GPMI operation TIMEOUT ERROR interrupt
        DCD     UART6_IRQHandler                              ;UART-6 ORed interrupt
        DCD     FTM1_IRQHandler                               ;Flex Timer1 Fault / Counter / Channel interrupt
        DCD     FTM2_IRQHandler                               ;Flex Timer2 Fault / Counter / Channel interrupt
        DCD     SNVS_Consolidated_IRQHandler                  ;SRTC Consolidated Interrupt. Non TZ.
        DCD     SNVS_Security_IRQHandler                      ;SRTC Security Interrupt. TZ.
        DCD     CSU_IRQHandler                                ;CSU Interrupt Request. Indicates to the processor that one or more alarm inputs were asserted.
        DCD     USDHC1_IRQHandler                             ;uSDHC1 Enhanced SDHC Interrupt Request
        DCD     USDHC2_IRQHandler                             ;uSDHC2 Enhanced SDHC Interrupt Request
        DCD     USDHC3_IRQHandler                             ;uSDHC3 Enhanced SDHC Interrupt Request
        DCD     MIPI_CSI_IRQHandler                           ;MIPI CSI interrupt
        DCD     UART1_IRQHandler                              ;UART-1 ORed interrupt
        DCD     UART2_IRQHandler                              ;UART-2 ORed interrupt
        DCD     UART3_IRQHandler                              ;UART-3 ORed interrupt
        DCD     UART4_IRQHandler                              ;UART-4 ORed interrupt
        DCD     UART5_IRQHandler                              ;UART-5 ORed interrupt
        DCD     eCSPI1_IRQHandler                             ;eCSPI1 interrupt request line to the core.
        DCD     eCSPI2_IRQHandler                             ;eCSPI2 interrupt request line to the core.
        DCD     eCSPI3_IRQHandler                             ;eCSPI3 interrupt request line to the core.
        DCD     eCSPI4_IRQHandler                             ;eCSPI4 interrupt request line to the core.
        DCD     I2C1_IRQHandler                               ;I2C-1 Interrupt
        DCD     I2C2_IRQHandler                               ;I2C-2 Interrupt
        DCD     I2C3_IRQHandler                               ;I2C-3 Interrupt
        DCD     I2C4_IRQHandler                               ;I2C-4 Interrupt
        DCD     RDC_IRQHandler                                ;RDC interrupt
        DCD     USB_HOST_IRQHandler                           ;USB HOST Interrupt
        DCD     MIPI_DSI_IRQHandler                           ;DSI Interrupt
        DCD     USB_OTG2_CORE_IRQHandler                      ;USB OTG2 core interrupt
        DCD     USB_OTG1_CORE_IRQHandler                      ;USB OTG1 core interrupt
        DCD     USB_OTG1_WAKEUP_IRQHandler                    ;USB OTG1 wake up Interrupt
        DCD     USB_OTG2_WAKEUP_IRQHandler                    ;USB OTG2 wake up Interrupt
        DCD     PXP1_IRQHandler                               ;PXP Interrupt 1
        DCD     SCTR_IRQ0_IRQHandler                          ;ISO7816IP Interrupt 0
        DCD     SCTR_IRQ1_IRQHandler                          ;ISO7816IP Interrupt 1
        DCD     TEMPMON_LOW_IRQHandler                        ;TempSensor (Temperature low alarm).
        DCD     SAI3_IRQHandler                               ;SAI3 Receive / Transmit Interrupt
        DCD     PMU_IRQHandler                                ;Brown-out event on either analog regulators.
        DCD     GPT4_IRQHandler                               ;OR of GPT Rollover interrupt line, Input Capture 1 and 2 lines, Output Compare 1, 2, and 3 Interrupt lines
        DCD     GPT3_IRQHandler                               ;OR of GPT Rollover interrupt line, Input Capture 1 and 2 lines, Output Compare 1, 2, and 3 Interrupt lines
        DCD     GPT2_IRQHandler                               ;OR of GPT Rollover interrupt line, Input Capture 1 and 2 lines, Output Compare 1, 2, and 3 Interrupt lines
        DCD     GPT1_IRQHandler                               ;OR of GPT Rollover interrupt line, Input Capture 1 and 2 lines, Output Compare 1, 2, and 3 Interrupt lines
        DCD     GPIO1_INT7_IRQHandler                         ;Active HIGH Interrupt from INT7 from GPIO
        DCD     GPIO1_INT6_IRQHandler                         ;Active HIGH Interrupt from INT6 from GPIO
        DCD     GPIO1_INT5_IRQHandler                         ;Active HIGH Interrupt from INT5 from GPIO
        DCD     GPIO1_INT4_IRQHandler                         ;Active HIGH Interrupt from INT4 from GPIO
        DCD     GPIO1_INT3_IRQHandler                         ;Active HIGH Interrupt from INT3 from GPIO
        DCD     GPIO1_INT2_IRQHandler                         ;Active HIGH Interrupt from INT2 from GPIO
        DCD     GPIO1_INT1_IRQHandler                         ;Active HIGH Interrupt from INT1 from GPIO
        DCD     GPIO1_INT0_IRQHandler                         ;Active HIGH Interrupt from INT0 from GPIO
        DCD     GPIO1_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO1 signal 0 throughout 15
        DCD     GPIO1_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO1 signal 16 throughout 31
        DCD     GPIO2_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO2 signal 0 throughout 15
        DCD     GPIO2_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO2 signal 16 throughout 31
        DCD     GPIO3_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO3 signal 0 throughout 15
        DCD     GPIO3_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO3 signal 16 throughout 31
        DCD     GPIO4_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO4 signal 0 throughout 15
        DCD     GPIO4_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO4 signal 16 throughout 31
        DCD     GPIO5_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO5 signal 0 throughout 15
        DCD     GPIO5_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO5 signal 16 throughout 31
        DCD     GPIO6_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO6 signal 0 throughout 15
        DCD     GPIO6_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO6 signal 16 throughout 31
        DCD     GPIO7_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO7 signal 0 throughout 15
        DCD     GPIO7_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO7 signal 16 throughout 31
        DCD     WDOG1_IRQHandler                              ;Watchdog Timer reset
        DCD     WDOG2_IRQHandler                              ;Watchdog Timer reset
        DCD     KPP_IRQHandler                                ;Keypad Interrupt
        DCD     PWM1_IRQHandler                               ;Cumulative interrupt line. OR of Rollover Interrupt line, Compare Interrupt line and FIFO Waterlevel crossing interrupt line.
        DCD     PWM2_IRQHandler                               ;Cumulative interrupt line. OR of Rollover Interrupt line, Compare Interrupt line and FIFO Waterlevel crossing interrupt line.
        DCD     PWM3_IRQHandler                               ;Cumulative interrupt line. OR of Rollover Interrupt line, Compare Interrupt line and FIFO Waterlevel crossing interrupt line.
        DCD     PWM4_IRQHandler                               ;Cumulative interrupt line. OR of Rollover Interrupt line, Compare Interrupt line and FIFO Waterlevel crossing interrupt line.
        DCD     CCM1_IRQHandler                               ;CCM, Interrupt Request 1
        DCD     CCM2_IRQHandler                               ;CCM, Interrupt Request 2
        DCD     GPC_IRQHandler                                ;GPC Interrupt Request 1
        DCD     MU_A7_IRQHandler                              ;Interrupt to A7
        DCD     SRC_IRQHandler                                ;SRC interrupt request
        DCD     SIM1_IRQHandler                               ;SIM1 Interrupt
        DCD     RTIC_IRQHandler                               ;RTIC Interrupt
        DCD     CPU_PerformanceUnit_IRQHandler                ;Performance Unit Interrupts from Cheetah (interrnally: PMUIRQ[n]
        DCD     CPU_CTI_Trigger_IRQHandler                    ;CTI trigger outputs (internal: nCTIIRQ[n]
        DCD     SRC_Combined_IRQHandler                       ;Combined CPU wdog interrupts (4x) out of SRC.
        DCD     SAI1_IRQHandler                               ;SAI1 Receive / Transmit Interrupt
        DCD     SAI2_IRQHandler                               ;SAI2 Receive / Transmit Interrupt
        DCD     MU_M4_IRQHandler                              ;Interrupt to M4
        DCD     ADC1_IRQHandler                               ;ADC-1 Interrupt
        DCD     ADC2_IRQHandler                               ;ADC-2 Interrupt
        DCD     ENET2_MAC0_Rx_Tx_Done1_IRQHandler             ;MAC 0 Receive / Transmit Frame / Buffer Done
        DCD     ENET2_MAC0_Rx_Tx_Done2_IRQHandler             ;MAC 0 Receive / Transmit Frame / Buffer Done
        DCD     ENET2_IRQHandler                              ;MAC 0 IRQ
        DCD     ENET2_1588_IRQHandler                         ;MAC 0 1588 Timer Interrupt - synchronous
        DCD     TPR_IRQ_IRQHandler                            ;TPR IRQ
        DCD     CAAM_Job_Ring0_IRQHandler                     ;CAAM interrupt queue for JQ
        DCD     CAAM_Job_Ring1_IRQHandler                     ;CAAM interrupt queue for JQ
        DCD     QSPI_IRQHandler                               ;QSPI Interrupt
        DCD     TZASC1_IRQHandler                             ;TZASC (PL380) interrupt
        DCD     WDOG4_IRQHandler                              ;Watchdog Timer reset
        DCD     CAN1_IRQHandler                               ;FlexCAN1 Interrupt
        DCD     CAN2_IRQHandler                               ;FlexCAN2 Interrupt
        DCD     PERFMON1_IRQHandler                           ;General Interrupt
        DCD     PERFMON2_IRQHandler                           ;General Interrupt
        DCD     CAAM_Job_Ring2_IRQHandler                     ;CAAM interrupt queue for JQ
        DCD     CAAM_ERROR_IRQHandler                         ;Recoverable error interrupt
        DCD     HS1_IRQHandler                                ;HS Interrupt Request 1
        DCD     EPDC_IRQHandler                               ;EPDC Interrupt
        DCD     ENET1_MAC0_Rx_Tx_Done1_IRQHandler             ;MAC 0 Receive / Trasmit Frame / Buffer Done
        DCD     ENET1_MAC0_Rx_Tx_Done2_IRQHandler             ;MAC 0 Receive / Trasmit Frame / Buffer Done
        DCD     ENET1_IRQHandler                              ;MAC 0 IRQ
        DCD     ENET1_1588_IRQHandler                         ;MAC 0 1588 Timer Interrupt – synchronous
        DCD     PCIE_CTRL_IRQ0_IRQHandler                     ;Coming from GLUE logic, of set / reset FF, driven by PCIE signals.
        DCD     PCIE_CTRL_IRQ1_IRQHandler                     ;Coming from GLUE logic, of set / reset FF, driven by PCIE signals.
        DCD     PCIE_CTRL_IRQ2_IRQHandler                     ;Coming from GLUE logic, of set / reset FF, driven by PCIE signals.
        DCD     PCIE_CTRL_IRQ3_IRQHandler                     ;Coming from GLUE logic, of set / reset FF, driven by PCIE signals.
        DCD     UART7_IRQHandler                              ;UART-7 ORed interrupt
        DCD     PCIE_CTRL_IRQHandler                          ;Channels [63:32] interrupts requests
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

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

        PUBWEAK GPR_IRQ_IRQHandler
        PUBWEAK DAP_IRQHandler
        PUBWEAK SDMA_IRQHandler
        PUBWEAK SDMA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SDMA_IRQHandler
        LDR     R0, =SDMA_DriverIRQHandler
        BX      R0

        PUBWEAK DBGMON_IRQHandler
        PUBWEAK SNVS_IRQHandler
        PUBWEAK LCDIF_IRQHandler
        PUBWEAK SIM2_IRQHandler
        PUBWEAK CSI_IRQHandler
        PUBWEAK PXP0_IRQHandler
        PUBWEAK Reserved25_IRQHandler
        PUBWEAK WDOG3_IRQHandler
        PUBWEAK HS0_IRQHandler
        PUBWEAK APBHDMA_IRQHandler
        PUBWEAK APBHDMA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
APBHDMA_IRQHandler
        LDR     R0, =APBHDMA_DriverIRQHandler
        BX      R0

        PUBWEAK EIM_IRQHandler
        PUBWEAK BCH_IRQHandler
        PUBWEAK GPMI_IRQHandler
        PUBWEAK UART6_IRQHandler
        PUBWEAK UART6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART6_IRQHandler
        LDR     R0, =UART6_DriverIRQHandler
        BX      R0

        PUBWEAK FTM1_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK SNVS_Consolidated_IRQHandler
        PUBWEAK SNVS_Security_IRQHandler
        PUBWEAK CSU_IRQHandler
        PUBWEAK USDHC1_IRQHandler
        PUBWEAK USDHC1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC1_IRQHandler
        LDR     R0, =USDHC1_DriverIRQHandler
        BX      R0

        PUBWEAK USDHC2_IRQHandler
        PUBWEAK USDHC2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC2_IRQHandler
        LDR     R0, =USDHC2_DriverIRQHandler
        BX      R0

        PUBWEAK USDHC3_IRQHandler
        PUBWEAK USDHC3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC3_IRQHandler
        LDR     R0, =USDHC3_DriverIRQHandler
        BX      R0

        PUBWEAK MIPI_CSI_IRQHandler
        PUBWEAK UART1_IRQHandler
        PUBWEAK UART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_IRQHandler
        LDR     R0, =UART1_DriverIRQHandler
        BX      R0

        PUBWEAK UART2_IRQHandler
        PUBWEAK UART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART2_IRQHandler
        LDR     R0, =UART2_DriverIRQHandler
        BX      R0

        PUBWEAK UART3_IRQHandler
        PUBWEAK UART3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART3_IRQHandler
        LDR     R0, =UART3_DriverIRQHandler
        BX      R0

        PUBWEAK UART4_IRQHandler
        PUBWEAK UART4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART4_IRQHandler
        LDR     R0, =UART4_DriverIRQHandler
        BX      R0

        PUBWEAK UART5_IRQHandler
        PUBWEAK UART5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART5_IRQHandler
        LDR     R0, =UART5_DriverIRQHandler
        BX      R0

        PUBWEAK eCSPI1_IRQHandler
        PUBWEAK eCSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
eCSPI1_IRQHandler
        LDR     R0, =eCSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK eCSPI2_IRQHandler
        PUBWEAK eCSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
eCSPI2_IRQHandler
        LDR     R0, =eCSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK eCSPI3_IRQHandler
        PUBWEAK eCSPI3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
eCSPI3_IRQHandler
        LDR     R0, =eCSPI3_DriverIRQHandler
        BX      R0

        PUBWEAK eCSPI4_IRQHandler
        PUBWEAK eCSPI4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
eCSPI4_IRQHandler
        LDR     R0, =eCSPI4_DriverIRQHandler
        BX      R0

        PUBWEAK I2C1_IRQHandler
        PUBWEAK I2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C1_IRQHandler
        LDR     R0, =I2C1_DriverIRQHandler
        BX      R0

        PUBWEAK I2C2_IRQHandler
        PUBWEAK I2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C2_IRQHandler
        LDR     R0, =I2C2_DriverIRQHandler
        BX      R0

        PUBWEAK I2C3_IRQHandler
        PUBWEAK I2C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C3_IRQHandler
        LDR     R0, =I2C3_DriverIRQHandler
        BX      R0

        PUBWEAK I2C4_IRQHandler
        PUBWEAK I2C4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C4_IRQHandler
        LDR     R0, =I2C4_DriverIRQHandler
        BX      R0

        PUBWEAK RDC_IRQHandler
        PUBWEAK USB_HOST_IRQHandler
        PUBWEAK MIPI_DSI_IRQHandler
        PUBWEAK USB_OTG2_CORE_IRQHandler
        PUBWEAK USB_OTG1_CORE_IRQHandler
        PUBWEAK USB_OTG1_WAKEUP_IRQHandler
        PUBWEAK USB_OTG2_WAKEUP_IRQHandler
        PUBWEAK PXP1_IRQHandler
        PUBWEAK SCTR_IRQ0_IRQHandler
        PUBWEAK SCTR_IRQ1_IRQHandler
        PUBWEAK TEMPMON_LOW_IRQHandler
        PUBWEAK SAI3_IRQHandler
        PUBWEAK PMU_IRQHandler
        PUBWEAK GPT4_IRQHandler
        PUBWEAK GPT3_IRQHandler
        PUBWEAK GPT2_IRQHandler
        PUBWEAK GPT1_IRQHandler
        PUBWEAK GPIO1_INT7_IRQHandler
        PUBWEAK GPIO1_INT6_IRQHandler
        PUBWEAK GPIO1_INT5_IRQHandler
        PUBWEAK GPIO1_INT4_IRQHandler
        PUBWEAK GPIO1_INT3_IRQHandler
        PUBWEAK GPIO1_INT2_IRQHandler
        PUBWEAK GPIO1_INT1_IRQHandler
        PUBWEAK GPIO1_INT0_IRQHandler
        PUBWEAK GPIO1_Combined_0_15_IRQHandler
        PUBWEAK GPIO1_Combined_16_31_IRQHandler
        PUBWEAK GPIO2_Combined_0_15_IRQHandler
        PUBWEAK GPIO2_Combined_16_31_IRQHandler
        PUBWEAK GPIO3_Combined_0_15_IRQHandler
        PUBWEAK GPIO3_Combined_16_31_IRQHandler
        PUBWEAK GPIO4_Combined_0_15_IRQHandler
        PUBWEAK GPIO4_Combined_16_31_IRQHandler
        PUBWEAK GPIO5_Combined_0_15_IRQHandler
        PUBWEAK GPIO5_Combined_16_31_IRQHandler
        PUBWEAK GPIO6_Combined_0_15_IRQHandler
        PUBWEAK GPIO6_Combined_16_31_IRQHandler
        PUBWEAK GPIO7_Combined_0_15_IRQHandler
        PUBWEAK GPIO7_Combined_16_31_IRQHandler
        PUBWEAK WDOG1_IRQHandler
        PUBWEAK WDOG2_IRQHandler
        PUBWEAK KPP_IRQHandler
        PUBWEAK PWM1_IRQHandler
        PUBWEAK PWM2_IRQHandler
        PUBWEAK PWM3_IRQHandler
        PUBWEAK PWM4_IRQHandler
        PUBWEAK CCM1_IRQHandler
        PUBWEAK CCM2_IRQHandler
        PUBWEAK GPC_IRQHandler
        PUBWEAK MU_A7_IRQHandler
        PUBWEAK SRC_IRQHandler
        PUBWEAK SIM1_IRQHandler
        PUBWEAK RTIC_IRQHandler
        PUBWEAK CPU_PerformanceUnit_IRQHandler
        PUBWEAK CPU_CTI_Trigger_IRQHandler
        PUBWEAK SRC_Combined_IRQHandler
        PUBWEAK SAI1_IRQHandler
        PUBWEAK SAI2_IRQHandler
        PUBWEAK MU_M4_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK ADC2_IRQHandler
        PUBWEAK ENET2_MAC0_Rx_Tx_Done1_IRQHandler
        PUBWEAK ENET2_MAC0_Rx_Tx_Done1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET2_MAC0_Rx_Tx_Done1_IRQHandler
        LDR     R0, =ENET2_MAC0_Rx_Tx_Done1_DriverIRQHandler
        BX      R0

        PUBWEAK ENET2_MAC0_Rx_Tx_Done2_IRQHandler
        PUBWEAK ENET2_MAC0_Rx_Tx_Done2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET2_MAC0_Rx_Tx_Done2_IRQHandler
        LDR     R0, =ENET2_MAC0_Rx_Tx_Done2_DriverIRQHandler
        BX      R0

        PUBWEAK ENET2_IRQHandler
        PUBWEAK ENET2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET2_IRQHandler
        LDR     R0, =ENET2_DriverIRQHandler
        BX      R0

        PUBWEAK ENET2_1588_IRQHandler
        PUBWEAK ENET2_1588_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET2_1588_IRQHandler
        LDR     R0, =ENET2_1588_DriverIRQHandler
        BX      R0

        PUBWEAK TPR_IRQ_IRQHandler
        PUBWEAK CAAM_Job_Ring0_IRQHandler
        PUBWEAK CAAM_Job_Ring1_IRQHandler
        PUBWEAK QSPI_IRQHandler
        PUBWEAK QSPI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
QSPI_IRQHandler
        LDR     R0, =QSPI_DriverIRQHandler
        BX      R0

        PUBWEAK TZASC1_IRQHandler
        PUBWEAK WDOG4_IRQHandler
        PUBWEAK CAN1_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN2_IRQHandler
        PUBWEAK CAN2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN2_IRQHandler
        LDR     R0, =CAN2_DriverIRQHandler
        BX      R0

        PUBWEAK PERFMON1_IRQHandler
        PUBWEAK PERFMON2_IRQHandler
        PUBWEAK CAAM_Job_Ring2_IRQHandler
        PUBWEAK CAAM_ERROR_IRQHandler
        PUBWEAK HS1_IRQHandler
        PUBWEAK EPDC_IRQHandler
        PUBWEAK ENET1_MAC0_Rx_Tx_Done1_IRQHandler
        PUBWEAK ENET1_MAC0_Rx_Tx_Done1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET1_MAC0_Rx_Tx_Done1_IRQHandler
        LDR     R0, =ENET1_MAC0_Rx_Tx_Done1_DriverIRQHandler
        BX      R0

        PUBWEAK ENET1_MAC0_Rx_Tx_Done2_IRQHandler
        PUBWEAK ENET1_MAC0_Rx_Tx_Done2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET1_MAC0_Rx_Tx_Done2_IRQHandler
        LDR     R0, =ENET1_MAC0_Rx_Tx_Done2_DriverIRQHandler
        BX      R0

        PUBWEAK ENET1_IRQHandler
        PUBWEAK ENET1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET1_IRQHandler
        LDR     R0, =ENET1_DriverIRQHandler
        BX      R0

        PUBWEAK ENET1_1588_IRQHandler
        PUBWEAK ENET1_1588_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET1_1588_IRQHandler
        LDR     R0, =ENET1_1588_DriverIRQHandler
        BX      R0

        PUBWEAK PCIE_CTRL_IRQ0_IRQHandler
        PUBWEAK PCIE_CTRL_IRQ1_IRQHandler
        PUBWEAK PCIE_CTRL_IRQ2_IRQHandler
        PUBWEAK PCIE_CTRL_IRQ3_IRQHandler
        PUBWEAK UART7_IRQHandler
        PUBWEAK UART7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART7_IRQHandler
        LDR     R0, =UART7_DriverIRQHandler
        BX      R0

        PUBWEAK PCIE_CTRL_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
GPR_IRQ_IRQHandler
DAP_IRQHandler
SDMA_DriverIRQHandler
DBGMON_IRQHandler
SNVS_IRQHandler
LCDIF_IRQHandler
SIM2_IRQHandler
CSI_IRQHandler
PXP0_IRQHandler
Reserved25_IRQHandler
WDOG3_IRQHandler
HS0_IRQHandler
APBHDMA_DriverIRQHandler
EIM_IRQHandler
BCH_IRQHandler
GPMI_IRQHandler
UART6_DriverIRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
SNVS_Consolidated_IRQHandler
SNVS_Security_IRQHandler
CSU_IRQHandler
USDHC1_DriverIRQHandler
USDHC2_DriverIRQHandler
USDHC3_DriverIRQHandler
MIPI_CSI_IRQHandler
UART1_DriverIRQHandler
UART2_DriverIRQHandler
UART3_DriverIRQHandler
UART4_DriverIRQHandler
UART5_DriverIRQHandler
eCSPI1_DriverIRQHandler
eCSPI2_DriverIRQHandler
eCSPI3_DriverIRQHandler
eCSPI4_DriverIRQHandler
I2C1_DriverIRQHandler
I2C2_DriverIRQHandler
I2C3_DriverIRQHandler
I2C4_DriverIRQHandler
RDC_IRQHandler
USB_HOST_IRQHandler
MIPI_DSI_IRQHandler
USB_OTG2_CORE_IRQHandler
USB_OTG1_CORE_IRQHandler
USB_OTG1_WAKEUP_IRQHandler
USB_OTG2_WAKEUP_IRQHandler
PXP1_IRQHandler
SCTR_IRQ0_IRQHandler
SCTR_IRQ1_IRQHandler
TEMPMON_LOW_IRQHandler
SAI3_IRQHandler
PMU_IRQHandler
GPT4_IRQHandler
GPT3_IRQHandler
GPT2_IRQHandler
GPT1_IRQHandler
GPIO1_INT7_IRQHandler
GPIO1_INT6_IRQHandler
GPIO1_INT5_IRQHandler
GPIO1_INT4_IRQHandler
GPIO1_INT3_IRQHandler
GPIO1_INT2_IRQHandler
GPIO1_INT1_IRQHandler
GPIO1_INT0_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO2_Combined_16_31_IRQHandler
GPIO3_Combined_0_15_IRQHandler
GPIO3_Combined_16_31_IRQHandler
GPIO4_Combined_0_15_IRQHandler
GPIO4_Combined_16_31_IRQHandler
GPIO5_Combined_0_15_IRQHandler
GPIO5_Combined_16_31_IRQHandler
GPIO6_Combined_0_15_IRQHandler
GPIO6_Combined_16_31_IRQHandler
GPIO7_Combined_0_15_IRQHandler
GPIO7_Combined_16_31_IRQHandler
WDOG1_IRQHandler
WDOG2_IRQHandler
KPP_IRQHandler
PWM1_IRQHandler
PWM2_IRQHandler
PWM3_IRQHandler
PWM4_IRQHandler
CCM1_IRQHandler
CCM2_IRQHandler
GPC_IRQHandler
MU_A7_IRQHandler
SRC_IRQHandler
SIM1_IRQHandler
RTIC_IRQHandler
CPU_PerformanceUnit_IRQHandler
CPU_CTI_Trigger_IRQHandler
SRC_Combined_IRQHandler
SAI1_IRQHandler
SAI2_IRQHandler
MU_M4_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
ENET2_MAC0_Rx_Tx_Done1_DriverIRQHandler
ENET2_MAC0_Rx_Tx_Done2_DriverIRQHandler
ENET2_DriverIRQHandler
ENET2_1588_DriverIRQHandler
TPR_IRQ_IRQHandler
CAAM_Job_Ring0_IRQHandler
CAAM_Job_Ring1_IRQHandler
QSPI_DriverIRQHandler
TZASC1_IRQHandler
WDOG4_IRQHandler
CAN1_DriverIRQHandler
CAN2_DriverIRQHandler
PERFMON1_IRQHandler
PERFMON2_IRQHandler
CAAM_Job_Ring2_IRQHandler
CAAM_ERROR_IRQHandler
HS1_IRQHandler
EPDC_IRQHandler
ENET1_MAC0_Rx_Tx_Done1_DriverIRQHandler
ENET1_MAC0_Rx_Tx_Done2_DriverIRQHandler
ENET1_DriverIRQHandler
ENET1_1588_DriverIRQHandler
PCIE_CTRL_IRQ0_IRQHandler
PCIE_CTRL_IRQ1_IRQHandler
PCIE_CTRL_IRQ2_IRQHandler
PCIE_CTRL_IRQ3_IRQHandler
UART7_DriverIRQHandler
PCIE_CTRL_IRQHandler
DefaultISR
        B DefaultISR

        END
