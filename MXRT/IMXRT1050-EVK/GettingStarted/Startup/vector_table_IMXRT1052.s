/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2008-2017 IAR Systems AB.
 *
 * $Revision: 112610 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol __iar_program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY, which
; is where to find the SP start value.
; If vector table is not located at address 0, the user has to initialize the  NVIC vector
; table register (VTOR) before using interrupts.
;
; Cortex-M version
;

        MODULE  ?vector_table

        AAPCS INTERWORK, VFP_COMPATIBLE, RWPI_COMPATIBLE
        PRESERVE8


          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI EndCommon cfiCommon0
        
        
        
        
        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA

__iar_init$$done:               ; The vector table is not needed
                                ; until after copy initialization is done

__vector_table
        DCD     sfe(CSTACK)
        DCD     __iar_program_start

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        DCD     DMA0_DMA16_IRQHandler                         ;DMA channel 0/16 transfer complete
        DCD     DMA1_DMA17_IRQHandler                         ;DMA channel 1/17 transfer complete
        DCD     DMA2_DMA18_IRQHandler                         ;DMA channel 2/18 transfer complete
        DCD     DMA3_DMA19_IRQHandler                         ;DMA channel 3/19 transfer complete
        DCD     DMA4_DMA20_IRQHandler                         ;DMA channel 4/20 transfer complete
        DCD     DMA5_DMA21_IRQHandler                         ;DMA channel 5/21 transfer complete
        DCD     DMA6_DMA22_IRQHandler                         ;DMA channel 6/22 transfer complete
        DCD     DMA7_DMA23_IRQHandler                         ;DMA channel 7/23 transfer complete
        DCD     DMA8_DMA24_IRQHandler                         ;DMA channel 8/24 transfer complete
        DCD     DMA9_DMA25_IRQHandler                         ;DMA channel 9/25 transfer complete
        DCD     DMA10_DMA26_IRQHandler                        ;DMA channel 10/26 transfer complete
        DCD     DMA11_DMA27_IRQHandler                        ;DMA channel 11/27 transfer complete
        DCD     DMA12_DMA28_IRQHandler                        ;DMA channel 12/28 transfer complete
        DCD     DMA13_DMA29_IRQHandler                        ;DMA channel 13/29 transfer complete
        DCD     DMA14_DMA30_IRQHandler                        ;DMA channel 14/30 transfer complete
        DCD     DMA15_DMA31_IRQHandler                        ;DMA channel 15/31 transfer complete
        DCD     DMA_ERROR_IRQHandler                          ;DMA error interrupt channels 0-15 / 16-31
        DCD     CTI0_ERROR_IRQHandler                         ;CTI0_Error
        DCD     CTI1_ERROR_IRQHandler                         ;CTI1_Error
        DCD     CORE_IRQHandler                               ;CorePlatform exception IRQ
        DCD     LPUART1_IRQHandler                            ;LPUART1 TX interrupt and RX interrupt
        DCD     LPUART2_IRQHandler                            ;LPUART2 TX interrupt and RX interrupt
        DCD     LPUART3_IRQHandler                            ;LPUART3 TX interrupt and RX interrupt
        DCD     LPUART4_IRQHandler                            ;LPUART4 TX interrupt and RX interrupt
        DCD     LPUART5_IRQHandler                            ;LPUART5 TX interrupt and RX interrupt
        DCD     LPUART6_IRQHandler                            ;LPUART6 TX interrupt and RX interrupt
        DCD     LPUART7_IRQHandler                            ;LPUART7 TX interrupt and RX interrupt
        DCD     LPUART8_IRQHandler                            ;LPUART8 TX interrupt and RX interrupt
        DCD     LPI2C1_IRQHandler                             ;LPI2C1 interrupt
        DCD     LPI2C2_IRQHandler                             ;LPI2C2 interrupt
        DCD     LPI2C3_IRQHandler                             ;LPI2C3 interrupt
        DCD     LPI2C4_IRQHandler                             ;LPI2C4 interrupt
        DCD     LPSPI1_IRQHandler                             ;LPSPI1 single interrupt vector for all sources
        DCD     LPSPI2_IRQHandler                             ;LPSPI2 single interrupt vector for all sources
        DCD     LPSPI3_IRQHandler                             ;LPSPI3 single interrupt vector for all sources
        DCD     LPSPI4_IRQHandler                             ;LPSPI4  single interrupt vector for all sources
        DCD     CAN1_IRQHandler                               ;CAN1 interrupt
        DCD     CAN2_IRQHandler                               ;CAN2 interrupt
        DCD     FLEXRAM_IRQHandler                            ;FlexRAM address out of range Or access hit IRQ
        DCD     KPP_IRQHandler                                ;Keypad nterrupt
        DCD     TSC_DIG_IRQHandler                            ;TSC interrupt
        DCD     GPR_IRQ_IRQHandler                            ;GPR interrupt
        DCD     LCDIF_IRQHandler                              ;LCDIF interrupt
        DCD     CSI_IRQHandler                                ;CSI interrupt
        DCD     PXP_IRQHandler                                ;PXP interrupt
        DCD     WDOG2_IRQHandler                              ;WDOG2 interrupt
        DCD     SNVS_HP_WRAPPER_IRQHandler                    ;SRTC Consolidated Interrupt. Non TZ
        DCD     SNVS_HP_WRAPPER_TZ_IRQHandler                 ;SRTC Security Interrupt. TZ
        DCD     SNVS_LP_WRAPPER_IRQHandler                    ;ON-OFF button press shorter than 5 secs (pulse event)
        DCD     CSU_IRQHandler                                ;CSU interrupt
        DCD     DCP_IRQHandler                                ;DCP_IRQ interrupt
        DCD     DCP_VMI_IRQHandler                            ;DCP_VMI_IRQ interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     TRNG_IRQHandler                               ;TRNG interrupt
        DCD     SJC_IRQHandler                                ;SJC interrupt
        DCD     BEE_IRQHandler                                ;BEE interrupt
        DCD     SAI1_IRQHandler                               ;SAI1 interrupt
        DCD     SAI2_IRQHandler                               ;SAI1 interrupt
        DCD     SAI3_RX_IRQHandler                            ;SAI3 interrupt
        DCD     SAI3_TX_IRQHandler                            ;SAI3 interrupt
        DCD     SPDIF_IRQHandler                              ;SPDIF interrupt
        DCD     ANATOP_EVENT0_IRQHandler                      ;ANATOP interrupt
        DCD     ANATOP_EVENT1_IRQHandler                      ;ANATOP interrupt
        DCD     ANATOP_TAMP_LOW_HIGH_IRQHandler               ;ANATOP interrupt
        DCD     ANATOP_TEMP_PANIC_IRQHandler                  ;ANATOP interrupt
        DCD     USB_PHY1_IRQHandler                           ;USBPHY (UTMI0), Interrupt
        DCD     USB_PHY2_IRQHandler                           ;USBPHY (UTMI0), Interrupt
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     ADC2_IRQHandler                               ;ADC2 interrupt
        DCD     DCDC_IRQHandler                               ;DCDC interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     GPIO1_INT0_IRQHandler                         ;Active HIGH Interrupt from INT0 from GPIO
        DCD     GPIO1_INT1_IRQHandler                         ;Active HIGH Interrupt from INT1 from GPIO
        DCD     GPIO1_INT2_IRQHandler                         ;Active HIGH Interrupt from INT2 from GPIO
        DCD     GPIO1_INT3_IRQHandler                         ;Active HIGH Interrupt from INT3 from GPIO
        DCD     GPIO1_INT4_IRQHandler                         ;Active HIGH Interrupt from INT4 from GPIO
        DCD     GPIO1_INT5_IRQHandler                         ;Active HIGH Interrupt from INT5 from GPIO
        DCD     GPIO1_INT6_IRQHandler                         ;Active HIGH Interrupt from INT6 from GPIO
        DCD     GPIO1_INT7_IRQHandler                         ;Active HIGH Interrupt from INT7 from GPIO
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
        DCD     FLEXIO1_IRQHandler                            ;FLEXIO1 interrupt
        DCD     FLEXIO2_IRQHandler                            ;FLEXIO2 interrupt
        DCD     WDOG1_IRQHandler                              ;WDOG1 interrupt
        DCD     RTWDOG_IRQHandler                             ;RTWDOG interrupt
        DCD     EWM_IRQHandler                                ;EWM interrupt
        DCD     CCM_1_IRQHandler                              ;CCM IRQ1 interrupt
        DCD     CCM_2_IRQHandler                              ;CCM IRQ2 interrupt
        DCD     GPC_IRQHandler                                ;GPC interrupt
        DCD     SRC_IRQHandler                                ;SRC interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     GPT1_IRQHandler                               ;GPT1 interrupt
        DCD     GPT2_IRQHandler                               ;GPT2 interrupt
        DCD     PWM1_0_IRQHandler                             ;PWM1 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM1_1_IRQHandler                             ;PWM1 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM1_2_IRQHandler                             ;PWM1 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM1_3_IRQHandler                             ;PWM1 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM1_FAULT_IRQHandler                         ;PWM1 fault or reload error interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     FLEXSPI_IRQHandler                            ;FlexSPI0 interrupt
        DCD     SEMC_IRQHandler                               ;Reserved interrupt
        DCD     USDHC1_IRQHandler                             ;USDHC1 interrupt
        DCD     USDHC2_IRQHandler                             ;USDHC2 interrupt
        DCD     USB_OTG2_IRQHandler                           ;USBO2 USB OTG2
        DCD     USB_OTG1_IRQHandler                           ;USBO2 USB OTG1
        DCD     ENET_IRQHandler                               ;ENET interrupt
        DCD     ENET_1588_Timer_IRQHandler                    ;ENET_1588_Timer interrupt
        DCD     XBAR1_IRQ_0_1_IRQHandler                      ;XBAR1 interrupt
        DCD     XBAR1_IRQ_2_3_IRQHandler                      ;XBAR1 interrupt
        DCD     ADC_ETC_IRQ0_IRQHandler                       ;ADCETC IRQ0 interrupt
        DCD     ADC_ETC_IRQ1_IRQHandler                       ;ADCETC IRQ1 interrupt
        DCD     ADC_ETC_IRQ2_IRQHandler                       ;ADCETC IRQ2 interrupt
        DCD     ADC_ETC_ERROR_IRQ_IRQHandler                  ;ADCETC Error IRQ interrupt
        DCD     PIT_IRQHandler                                ;PIT interrupt
        DCD     ACMP1_IRQHandler                              ;ACMP interrupt
        DCD     ACMP2_IRQHandler                              ;ACMP interrupt
        DCD     ACMP3_IRQHandler                              ;ACMP interrupt
        DCD     ACMP4_IRQHandler                              ;ACMP interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     ENC1_IRQHandler                               ;ENC1 interrupt
        DCD     ENC2_IRQHandler                               ;ENC2 interrupt
        DCD     ENC3_IRQHandler                               ;ENC3 interrupt
        DCD     ENC4_IRQHandler                               ;ENC4 interrupt
        DCD     TMR1_IRQHandler                               ;TMR1 interrupt
        DCD     TMR2_IRQHandler                               ;TMR2 interrupt
        DCD     TMR3_IRQHandler                               ;TMR3 interrupt
        DCD     TMR4_IRQHandler                               ;TMR4 interrupt
        DCD     PWM2_0_IRQHandler                             ;PWM2 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM2_1_IRQHandler                             ;PWM2 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM2_2_IRQHandler                             ;PWM2 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM2_3_IRQHandler                             ;PWM2 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM2_FAULT_IRQHandler                         ;PWM2 fault or reload error interrupt
        DCD     PWM3_0_IRQHandler                             ;PWM3 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM3_1_IRQHandler                             ;PWM3 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM3_2_IRQHandler                             ;PWM3 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM3_3_IRQHandler                             ;PWM3 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM3_FAULT_IRQHandler                         ;PWM3 fault or reload error interrupt
        DCD     PWM4_0_IRQHandler                             ;PWM4 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM4_1_IRQHandler                             ;PWM4 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM4_2_IRQHandler                             ;PWM4 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM4_3_IRQHandler                             ;PWM4 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM4_FAULT_IRQHandler                         ;PWM4 fault or reload error interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     Default_Handler                               ;Reserved interrupt
        DCD     SJC_ARM_DEBUG_IRQHandler                      ;SJC ARM debug interrupt
        DCD     NMI_WAKEUP_IRQHandler                         ;NMI wake up

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;

        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK MemManage_Handler
        PUBWEAK BusFault_Handler
        PUBWEAK UsageFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler

        PUBWEAK DMA0_DMA16_IRQHandler
        PUBWEAK DMA1_DMA17_IRQHandler
        PUBWEAK DMA2_DMA18_IRQHandler
        PUBWEAK DMA3_DMA19_IRQHandler
        PUBWEAK DMA4_DMA20_IRQHandler
        PUBWEAK DMA5_DMA21_IRQHandler
        PUBWEAK DMA6_DMA22_IRQHandler
        PUBWEAK DMA7_DMA23_IRQHandler
        PUBWEAK DMA8_DMA24_IRQHandler
        PUBWEAK DMA9_DMA25_IRQHandler
        PUBWEAK DMA10_DMA26_IRQHandler
        PUBWEAK DMA11_DMA27_IRQHandler
        PUBWEAK DMA12_DMA28_IRQHandler
        PUBWEAK DMA13_DMA29_IRQHandler
        PUBWEAK DMA14_DMA30_IRQHandler
        PUBWEAK DMA15_DMA31_IRQHandler
        PUBWEAK DMA_ERROR_IRQHandler
        PUBWEAK CTI0_ERROR_IRQHandler
        PUBWEAK CTI1_ERROR_IRQHandler
        PUBWEAK CORE_IRQHandler
        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART3_IRQHandler
        PUBWEAK LPUART4_IRQHandler
        PUBWEAK LPUART5_IRQHandler
        PUBWEAK LPUART6_IRQHandler
        PUBWEAK LPUART7_IRQHandler
        PUBWEAK LPUART8_IRQHandler
        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C3_IRQHandler
        PUBWEAK LPI2C4_IRQHandler
        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI2_IRQHandler
        PUBWEAK LPSPI3_IRQHandler
        PUBWEAK LPSPI4_IRQHandler
        PUBWEAK CAN1_IRQHandler
        PUBWEAK CAN2_IRQHandler
        PUBWEAK FLEXRAM_IRQHandler
        PUBWEAK KPP_IRQHandler
        PUBWEAK TSC_DIG_IRQHandler
        PUBWEAK GPR_IRQ_IRQHandler
        PUBWEAK LCDIF_IRQHandler
        PUBWEAK CSI_IRQHandler
        PUBWEAK PXP_IRQHandler
        PUBWEAK WDOG2_IRQHandler
        PUBWEAK SNVS_HP_WRAPPER_IRQHandler
        PUBWEAK SNVS_HP_WRAPPER_TZ_IRQHandler
        PUBWEAK SNVS_LP_WRAPPER_IRQHandler
        PUBWEAK CSU_IRQHandler
        PUBWEAK DCP_IRQHandler
        PUBWEAK DCP_VMI_IRQHandler
        PUBWEAK TRNG_IRQHandler
        PUBWEAK SJC_IRQHandler
        PUBWEAK BEE_IRQHandler
        PUBWEAK SAI1_IRQHandler
        PUBWEAK SAI2_IRQHandler
        PUBWEAK SAI3_RX_IRQHandler
        PUBWEAK SAI3_TX_IRQHandler
        PUBWEAK SPDIF_IRQHandler
        PUBWEAK ANATOP_EVENT0_IRQHandler
        PUBWEAK ANATOP_EVENT1_IRQHandler
        PUBWEAK ANATOP_TAMP_LOW_HIGH_IRQHandler
        PUBWEAK ANATOP_TEMP_PANIC_IRQHandler
        PUBWEAK USB_PHY1_IRQHandler
        PUBWEAK USB_PHY2_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK ADC2_IRQHandler
        PUBWEAK DCDC_IRQHandler
        PUBWEAK GPIO1_INT0_IRQHandler
        PUBWEAK GPIO1_INT1_IRQHandler
        PUBWEAK GPIO1_INT2_IRQHandler
        PUBWEAK GPIO1_INT3_IRQHandler
        PUBWEAK GPIO1_INT4_IRQHandler
        PUBWEAK GPIO1_INT5_IRQHandler
        PUBWEAK GPIO1_INT6_IRQHandler
        PUBWEAK GPIO1_INT7_IRQHandler
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
        PUBWEAK FLEXIO1_IRQHandler
        PUBWEAK FLEXIO2_IRQHandler
        PUBWEAK WDOG1_IRQHandler
        PUBWEAK RTWDOG_IRQHandler
        PUBWEAK EWM_IRQHandler
        PUBWEAK CCM_1_IRQHandler
        PUBWEAK CCM_2_IRQHandler
        PUBWEAK GPC_IRQHandler
        PUBWEAK SRC_IRQHandler
        PUBWEAK GPT1_IRQHandler
        PUBWEAK GPT2_IRQHandler
        PUBWEAK PWM1_0_IRQHandler
        PUBWEAK PWM1_1_IRQHandler
        PUBWEAK PWM1_2_IRQHandler
        PUBWEAK PWM1_3_IRQHandler
        PUBWEAK PWM1_FAULT_IRQHandler
        PUBWEAK FLEXSPI_IRQHandler
        PUBWEAK SEMC_IRQHandler
        PUBWEAK USDHC1_IRQHandler
        PUBWEAK USDHC2_IRQHandler
        PUBWEAK USB_OTG2_IRQHandler
        PUBWEAK USB_OTG1_IRQHandler
        PUBWEAK ENET_IRQHandler
        PUBWEAK ENET_1588_Timer_IRQHandler
        PUBWEAK XBAR1_IRQ_0_1_IRQHandler
        PUBWEAK XBAR1_IRQ_2_3_IRQHandler
        PUBWEAK ADC_ETC_IRQ0_IRQHandler
        PUBWEAK ADC_ETC_IRQ1_IRQHandler
        PUBWEAK ADC_ETC_IRQ2_IRQHandler
        PUBWEAK ADC_ETC_ERROR_IRQ_IRQHandler
        PUBWEAK PIT_IRQHandler
        PUBWEAK ACMP1_IRQHandler
        PUBWEAK ACMP2_IRQHandler
        PUBWEAK ACMP3_IRQHandler
        PUBWEAK ACMP4_IRQHandler
        PUBWEAK ENC1_IRQHandler
        PUBWEAK ENC2_IRQHandler
        PUBWEAK ENC3_IRQHandler
        PUBWEAK ENC4_IRQHandler
        PUBWEAK TMR1_IRQHandler
        PUBWEAK TMR2_IRQHandler
        PUBWEAK TMR3_IRQHandler
        PUBWEAK TMR4_IRQHandler
        PUBWEAK PWM2_0_IRQHandler
        PUBWEAK PWM2_1_IRQHandler
        PUBWEAK PWM2_2_IRQHandler
        PUBWEAK PWM2_3_IRQHandler
        PUBWEAK PWM2_FAULT_IRQHandler
        PUBWEAK PWM3_0_IRQHandler
        PUBWEAK PWM3_1_IRQHandler
        PUBWEAK PWM3_2_IRQHandler
        PUBWEAK PWM3_3_IRQHandler
        PUBWEAK PWM3_FAULT_IRQHandler
        PUBWEAK PWM4_0_IRQHandler
        PUBWEAK PWM4_1_IRQHandler
        PUBWEAK PWM4_2_IRQHandler
        PUBWEAK PWM4_3_IRQHandler
        PUBWEAK PWM4_FAULT_IRQHandler
        PUBWEAK SJC_ARM_DEBUG_IRQHandler
        PUBWEAK NMI_WAKEUP_IRQHandler

        SECTION .text:CODE:REORDER:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __default_handler
        THUMB

NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler

DMA0_DMA16_IRQHandler
DMA1_DMA17_IRQHandler
DMA2_DMA18_IRQHandler
DMA3_DMA19_IRQHandler
DMA4_DMA20_IRQHandler
DMA5_DMA21_IRQHandler
DMA6_DMA22_IRQHandler
DMA7_DMA23_IRQHandler
DMA8_DMA24_IRQHandler
DMA9_DMA25_IRQHandler
DMA10_DMA26_IRQHandler
DMA11_DMA27_IRQHandler
DMA12_DMA28_IRQHandler
DMA13_DMA29_IRQHandler
DMA14_DMA30_IRQHandler
DMA15_DMA31_IRQHandler
DMA_ERROR_IRQHandler
CTI0_ERROR_IRQHandler
CTI1_ERROR_IRQHandler
CORE_IRQHandler
LPUART1_IRQHandler
LPUART2_IRQHandler
LPUART3_IRQHandler
LPUART4_IRQHandler
LPUART5_IRQHandler
LPUART6_IRQHandler
LPUART7_IRQHandler
LPUART8_IRQHandler
LPI2C1_IRQHandler
LPI2C2_IRQHandler
LPI2C3_IRQHandler
LPI2C4_IRQHandler
LPSPI1_IRQHandler
LPSPI2_IRQHandler
LPSPI3_IRQHandler
LPSPI4_IRQHandler
CAN1_IRQHandler
CAN2_IRQHandler
FLEXRAM_IRQHandler
KPP_IRQHandler
TSC_DIG_IRQHandler
GPR_IRQ_IRQHandler
LCDIF_IRQHandler
CSI_IRQHandler
PXP_IRQHandler
WDOG2_IRQHandler
SNVS_HP_WRAPPER_IRQHandler
SNVS_HP_WRAPPER_TZ_IRQHandler
SNVS_LP_WRAPPER_IRQHandler
CSU_IRQHandler
DCP_IRQHandler
DCP_VMI_IRQHandler
TRNG_IRQHandler
SJC_IRQHandler
BEE_IRQHandler
SAI1_IRQHandler
SAI2_IRQHandler
SAI3_RX_IRQHandler
SAI3_TX_IRQHandler
SPDIF_IRQHandler
ANATOP_EVENT0_IRQHandler
ANATOP_EVENT1_IRQHandler
ANATOP_TAMP_LOW_HIGH_IRQHandler
ANATOP_TEMP_PANIC_IRQHandler
USB_PHY1_IRQHandler
USB_PHY2_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
DCDC_IRQHandler
GPIO1_INT0_IRQHandler
GPIO1_INT1_IRQHandler
GPIO1_INT2_IRQHandler
GPIO1_INT3_IRQHandler
GPIO1_INT4_IRQHandler
GPIO1_INT5_IRQHandler
GPIO1_INT6_IRQHandler
GPIO1_INT7_IRQHandler
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
FLEXIO1_IRQHandler
FLEXIO2_IRQHandler
WDOG1_IRQHandler
RTWDOG_IRQHandler
EWM_IRQHandler
CCM_1_IRQHandler
CCM_2_IRQHandler
GPC_IRQHandler
SRC_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
FLEXSPI_IRQHandler
SEMC_IRQHandler
USDHC1_IRQHandler
USDHC2_IRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
ENET_IRQHandler
ENET_1588_Timer_IRQHandler
XBAR1_IRQ_0_1_IRQHandler
XBAR1_IRQ_2_3_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
PIT_IRQHandler
ACMP1_IRQHandler
ACMP2_IRQHandler
ACMP3_IRQHandler
ACMP4_IRQHandler
ENC1_IRQHandler
ENC2_IRQHandler
ENC3_IRQHandler
ENC4_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
TMR4_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM2_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
PWM4_FAULT_IRQHandler
SJC_ARM_DEBUG_IRQHandler
NMI_WAKEUP_IRQHandler

Default_Handler
__default_handler
        CALL_GRAPH_ROOT __default_handler, "interrupt"
        NOCALL __default_handler
        B __default_handler
          CFI EndBlock cfiBlock0
          
        END
