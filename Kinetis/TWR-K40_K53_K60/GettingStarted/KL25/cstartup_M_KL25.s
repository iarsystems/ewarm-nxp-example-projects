/**************************************************
 *
 * Copyright 2012 IAR Systems. All rights reserved.
 *
 * $Revision: 488 $
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

        SECTION .intvec:CODE:ROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)                   ; Top of Stack
        DCD     __iar_program_start           ; Reset Handler
        DCD     NMI_Handler                   ; NMI Handler
        DCD     HardFault_Handler             ; Hard Fault Handler
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     SVC_Handler                   ; SVCall Handler
        DCD     0                             ; Reserved
        DCD     0                             ; Reserved
        DCD     PendSV_Handler                ; PendSV Handler
        DCD     SysTick_Handler               ; SysTick Handler
        ; External Interrupts
        DCD     DMA0_IRQHandler               ; 0:  DMA Channel 0 transfer complete intertrupt
        DCD     DMA1_IRQHandler               ; 1:  DMA Channel 1 transfer complete intertrupt
        DCD     DMA2_IRQHandler               ; 2:  DMA Channel 2 transfer complete intertrupt
        DCD     DMA3_IRQHandler               ; 3:  DMA Channel 3 transfer complete intertrupt
        DCD     0                             ; 4:  Reserved  DMA Channel 5 transfer complete intertrupt
        DCD     FTFA_IRQHandler               ; 5:  FTFA
        DCD     PMU_IRQHandler                ; 6:  Low-voltage detect, low-voltage warning
        DCD     LLWU_IRQHandler               ; 7:  Low Leakage Wakeup
        DCD     I2C0_IRQHandler               ; 8:  IIC 0 interrupt
        DCD     I2C1_IRQHandler               ; 9:  IIC 1 intertrupt
        DCD     SPI0_IRQHandler               ;10:  SPI0 intertrupt
        DCD     SPI1_IRQHandler               ;11:  SPI1 intertrupt
        DCD     UART0_IRQHandler              ;12:  UART 0 status and error intertrupt
        DCD     UART1_IRQHandler              ;13:  UART 1 status and error intertrupt
        DCD     UART2_IRQHandler              ;14:  UART 2 status and error intertrupt
        DCD     ADC0_IRQHandler               ;15:  ADC 0 interrupt
        DCD     CMP0_IRQHandler               ;16:  CMP 0 interrupt
        DCD     TPM0_IRQHandler               ;17:  TPM 0 interrupt
        DCD     TPM1_IRQHandler               ;18:  TPM 1 interrupt
        DCD     TPM2_IRQHandler               ;19:  TPM 2 interrupt
        DCD     RTC_Alarm_IRQHandler          ;20:  RTC Alarm interrupt
        DCD     RTC_IRQHandler                ;21:  RTC Seconds interrupt
        DCD     PIT_IRQHandler                ;22:  PIT Single interrupt vector for all channels
        DCD     0                             ;23:  Reserved
        DCD     USB_OTG_IRQHandler            ;24:  USB OTG intertrupt
        DCD     DAC0_IRQHandler               ;25:  UART 0 status intertrupt
        DCD     TSI0_IRQHandler               ;26:  TSI 0 interrupt
        DCD     MCG_IRQHandler                ;27:  MCG intertrupt
        DCD     LPTMR0_IRQHandler             ;28:  LPTMR0 intertrupt
        DCD     0                             ;29:  Reserved
        DCD     PORTA_IRQHandler              ;30:  PORT A interrupt
        DCD     PORTD_IRQHandler              ;31:  PORT D interrupt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;

      PUBWEAK NMI_Handler
      PUBWEAK HardFault_Handler
      PUBWEAK SVC_Handler
      PUBWEAK PendSV_Handler
      PUBWEAK SysTick_Handler
      PUBWEAK DMA0_IRQHandler
      PUBWEAK DMA1_IRQHandler
      PUBWEAK DMA2_IRQHandler
      PUBWEAK DMA3_IRQHandler
      PUBWEAK FTFA_IRQHandler
      PUBWEAK PMU_IRQHandler
      PUBWEAK LLWU_IRQHandler
      PUBWEAK I2C0_IRQHandler
      PUBWEAK I2C1_IRQHandler
      PUBWEAK SPI0_IRQHandler
      PUBWEAK SPI1_IRQHandler
      PUBWEAK UART0_IRQHandler
      PUBWEAK UART1_IRQHandler
      PUBWEAK UART2_IRQHandler
      PUBWEAK ADC0_IRQHandler
      PUBWEAK CMP0_IRQHandler
      PUBWEAK TPM0_IRQHandler
      PUBWEAK TPM1_IRQHandler
      PUBWEAK TPM2_IRQHandler
      PUBWEAK RTC_Alarm_IRQHandler
      PUBWEAK RTC_IRQHandler
      PUBWEAK PIT_IRQHandler
      PUBWEAK USB_OTG_IRQHandler
      PUBWEAK DAC0_IRQHandler
      PUBWEAK TSI0_IRQHandler
      PUBWEAK MCG_IRQHandler
      PUBWEAK LPTMR0_IRQHandler
      PUBWEAK PORTA_IRQHandler
      PUBWEAK PORTD_IRQHandler

      SECTION .text:CODE:REORDER:NOROOT(1)
      THUMB
NMI_Handler
HardFault_Handler
SVC_Handler
PendSV_Handler
SysTick_Handler
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
FTFA_IRQHandler
PMU_IRQHandler
LLWU_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
RTC_Alarm_IRQHandler
RTC_IRQHandler
PIT_IRQHandler
USB_OTG_IRQHandler
DAC0_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
PORTA_IRQHandler
PORTD_IRQHandler
Default_Handler

        B Default_Handler
        END
