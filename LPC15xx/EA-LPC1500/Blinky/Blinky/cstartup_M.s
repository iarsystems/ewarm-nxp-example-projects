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
__vector_table_0x1c
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; PendSV Handler
        DCD     SysTick_Handler             ; SysTick Handler


        ; External Interrupts
                DCD     WDT_IRQHandler            ; Watchdog timer
                DCD     BOD_IRQHandler            ; Brown Out Detect
                DCD     FLASH_IRQHandler          ; NVMC Flash Controller
                DCD     EE_IRQHandler             ; NVMC EE Controller
                DCD     DMA_IRQHandler            ; DMA Controller
                DCD     GINT0_IRQHandler                         
                DCD     GINT1_IRQHandler          ; PIO0 (0:7)   
                DCD     PIN_INT0_IRQHandler       ; All GPIO pin can be routed to PIN_INTx
                DCD     PIN_INT1_IRQHandler          
                DCD     PIN_INT2_IRQHandler                       
                DCD     PIN_INT3_IRQHandler                         
                DCD     PIN_INT4_IRQHandler                        
                DCD     PIN_INT5_IRQHandler
                DCD     PIN_INT6_IRQHandler
                DCD     PIN_INT7_IRQHandler                       
                DCD     RIT_IRQHandler            ; RIT Timer
                DCD     SCT0_IRQHandler           ; SCT Timer0
                DCD     SCT1_IRQHandler           ; SCT Timer1
                DCD     SCT2_IRQHandler           ; SCT Timer2
                DCD     SCT3_IRQHandler           ; SCT Timer3
                
                DCD     MRT_IRQHandler            ; MRT timer
                DCD     UART0_IRQHandler          ; MIN UART0
                DCD     UART1_IRQHandler          ; MIN UART1
                DCD     UART2_IRQHandler          ; MIN UART2
                DCD     I2C_IRQHandler            ; BI2C
                DCD     SPI0_IRQHandler           ; LSPI0 
                DCD     SPI1_IRQHandler           ; LSPI1 
                DCD     CAN_IRQHandler            ; CAN 
                DCD     USB_IRQHandler            ; USB IRQ
                DCD     USB_FIQHandler            ; USB FIQ
                DCD     USBWakeup_IRQHandler      ; USB wake up
                DCD     ADC0_SEQA_IRQHandler      ; ADC0 SEQA
                DCD     ADC0_SEQB_IRQHandler      ; ADC0 SEQB
                DCD     ADC0_THCMP_IRQHandler     ; ADC0 THCMP
                DCD     ADC0_OVR_IRQHandler       ; ADC0 OVR
                DCD     ADC1_SEQA_IRQHandler      ; ADC1 SEQA
                DCD     ADC1_SEQB_IRQHandler      ; ADC1 SEQB
                DCD     ADC1_THCMP_IRQHandler     ; ADC1 THCMP
                DCD     ADC1_OVR_IRQHandler       ; ADC1 OVR
                DCD     DAC_IRQHandler            ; D/A Converter
                
                DCD     COMP0_IRQHandler          ; Comparator 0
                DCD     COMP1_IRQHandler          ; Comparator 1
                DCD     COMP2_IRQHandler          ; Comparator 2
                DCD     COMP3_IRQHandler          ; Comparator 3
                DCD     QEI_IRQHandler            ; QEI 
                DCD     RTC_ALARM_IRQHandler      ; RTC Alarm 
                DCD     RTC_WAKE_IRQHandler       ; RTC Wake
		DCD     EZH_ARCH_B0_IRQHandler	  ; EZH_ARCH_B
        


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        SECTION .text:CODE:REORDER:NOROOT(1)

        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler
        
                 PUBWEAK  WDT_IRQHandler            
                PUBWEAK  BOD_IRQHandler            
                PUBWEAK  FLASH_IRQHandler          
                PUBWEAK  EE_IRQHandler             
                PUBWEAK  DMA_IRQHandler            
                PUBWEAK  GINT0_IRQHandler          
                PUBWEAK  GINT1_IRQHandler          
                PUBWEAK  PIN_INT0_IRQHandler       
                PUBWEAK  PIN_INT1_IRQHandler       
                PUBWEAK  PIN_INT2_IRQHandler       
                PUBWEAK  PIN_INT3_IRQHandler       
                PUBWEAK  PIN_INT4_IRQHandler       
                PUBWEAK  PIN_INT5_IRQHandler       
                PUBWEAK  PIN_INT6_IRQHandler      
                PUBWEAK  PIN_INT7_IRQHandler       
                PUBWEAK  RIT_IRQHandler            
                PUBWEAK  SCT0_IRQHandler           
                PUBWEAK  SCT1_IRQHandler           
                PUBWEAK  SCT2_IRQHandler           
                PUBWEAK  SCT3_IRQHandler           

                PUBWEAK  MRT_IRQHandler            
                PUBWEAK  UART0_IRQHandler          
                PUBWEAK  UART1_IRQHandler          
                PUBWEAK  UART2_IRQHandler          
                PUBWEAK  I2C_IRQHandler            
                PUBWEAK  SPI0_IRQHandler           
                PUBWEAK  SPI1_IRQHandler           
                PUBWEAK  CAN_IRQHandler            
                PUBWEAK  USB_IRQHandler            
                PUBWEAK  USB_FIQHandler            
                PUBWEAK  USBWakeup_IRQHandler      
                PUBWEAK  ADC0_SEQA_IRQHandler      
                PUBWEAK  ADC0_SEQB_IRQHandler      
                PUBWEAK  ADC0_THCMP_IRQHandler     
                PUBWEAK  ADC0_OVR_IRQHandler       
                PUBWEAK  ADC1_SEQA_IRQHandler      
                PUBWEAK  ADC1_SEQB_IRQHandler      
                PUBWEAK  ADC1_THCMP_IRQHandler     
                PUBWEAK  ADC1_OVR_IRQHandler       
                PUBWEAK  DAC_IRQHandler             

                PUBWEAK  COMP0_IRQHandler           
                PUBWEAK  COMP1_IRQHandler           
                PUBWEAK  COMP2_IRQHandler           
                PUBWEAK  COMP3_IRQHandler           
                PUBWEAK  QEI_IRQHandler             
                PUBWEAK  RTC_ALARM_IRQHandler       
                PUBWEAK  RTC_WAKE_IRQHandler        
		PUBWEAK  EZH_ARCH_B0_IRQHandler            
        
        
        


NMI_Handler:
HardFault_Handler:
SVC_Handler:
DebugMon_Handler:
PendSV_Handler:
SysTick_Handler:

WDT_IRQHandler:            
BOD_IRQHandler:            
FLASH_IRQHandler:           
EE_IRQHandler:              
DMA_IRQHandler:             
GINT0_IRQHandler:           
GINT1_IRQHandler:           
PIN_INT0_IRQHandler:        
PIN_INT1_IRQHandler:        
PIN_INT2_IRQHandler:        
PIN_INT3_IRQHandler:        
PIN_INT4_IRQHandler:        
PIN_INT5_IRQHandler:        
PIN_INT6_IRQHandler:       
PIN_INT7_IRQHandler:        
RIT_IRQHandler:             
SCT0_IRQHandler:            
SCT1_IRQHandler:            
SCT2_IRQHandler:            
SCT3_IRQHandler:            

MRT_IRQHandler:             
UART0_IRQHandler:           
UART1_IRQHandler:           
UART2_IRQHandler:           
I2C_IRQHandler:             
SPI0_IRQHandler:            
SPI1_IRQHandler:            
CAN_IRQHandler:             
USB_IRQHandler:             
USB_FIQHandler:             
USBWakeup_IRQHandler:       
ADC0_SEQA_IRQHandler:       
ADC0_SEQB_IRQHandler:       
ADC0_THCMP_IRQHandler:      
ADC0_OVR_IRQHandler:        
ADC1_SEQA_IRQHandler:       
ADC1_SEQB_IRQHandler:       
ADC1_THCMP_IRQHandler:      
ADC1_OVR_IRQHandler:        
DAC_IRQHandler:              

COMP0_IRQHandler:            
COMP1_IRQHandler:            
COMP2_IRQHandler:            
COMP3_IRQHandler:            
QEI_IRQHandler:              
RTC_ALARM_IRQHandler:        
RTC_WAKE_IRQHandler:         
EZH_ARCH_B0_IRQHandler:         
Default_Handler:
        B Default_Handler

        END
