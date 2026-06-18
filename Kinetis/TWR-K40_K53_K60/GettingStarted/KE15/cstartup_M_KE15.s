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
        DCD     sfe(CSTACK)               		; Top of Stack
        DCD     __iar_program_start       		; Reset Handler
        DCD     NMI_Handler               		; NMI Handler
        DCD     HardFault_Handler         		; Hard Fault Handler
        DCD     MemManage_Handler         		; MPU Fault Handler
        DCD     BusFault_Handler          		; Bus Fault Handler
        DCD     UsageFault_Handler        		; Usage Fault Handler
        DCD     0                         		; Reserved
        DCD     0                         		; Reserved
        DCD     0                         		; Reserved
        DCD     0                         		; Reserved
        DCD     SVC_Handler               		; SVCall Handler
        DCD     DebugMon_Handler          		; Debug Monitor Handler
        DCD     0                         		; Reserved
        DCD     PendSV_Handler            		; PendSV Handler
        DCD     SysTick_Handler           		; SysTick Handler
        ; External Interrupts
        DCD     DMA0_IRQHandler           		; 0:  DMA Channel 0 transfer complete intertrupt
        DCD     DMA1_IRQHandler           		; 1:  DMA Channel 1 transfer complete intertrupt
        DCD     DMA2_IRQHandler           		; 2:  DMA Channel 2 transfer complete intertrupt
        DCD     DMA3_IRQHandler           		; 3:  DMA Channel 3 transfer complete intertrupt
        DCD     DMA4_IRQHandler           		; 4:  DMA Channel 4 transfer complete intertrupt
        DCD     DMA5_IRQHandler           		; 5:  DMA Channel 5 transfer complete intertrupt
        DCD     DMA6_IRQHandler           		; 6:  DMA Channel 6 transfer complete intertrupt
        DCD     DMA7_IRQHandler           		; 7:  DMA Channel 7 transfer complete intertrupt
        DCD     DMA8_IRQHandler           		; 8:  DMA Channel 8 transfer complete intertrupt
        DCD     DMA9_IRQHandler           		; 9:  DMA Channel 9 transfer complete intertrupt
        DCD     DMA10_IRQHandler          		;10:  DMA Channel 10 transfer complete intertrupt
        DCD     DMA11_IRQHandler          		;11:  DMA Channel 11 transfer complete intertrupt
        DCD     DMA12_IRQHandler          		;12:  DMA Channel 12 transfer complete intertrupt
        DCD     DMA13_IRQHandler          		;13:  DMA Channel 13 transfer complete intertrupt
        DCD     iEVENT0_IRQHandler        		;14:  iEVENT channel 0 intertrupt
        DCD     iEVENT1_IRQHandler        		;15:  iEVENT channel 1 intertrupt
        DCD     INT_DMA_Error_IRQHandler  		;16:  DMA Error Interrupt
        DCD     0                         		;17:  Reserved
        DCD     FTMRA_IRQHandler      				;18:  FTMRA Interrupt
        DCD     Read_Collision_IRQHandler 		;19:  Read Collision intertrupt
        DCD     LVD_LVW_IRQHandler        		;20:  Low Voltage Detect, Low Voltage Warning intertrupt
        DCD     ICS_IRQHandler          		 	;21:  ICS intertrupt
        DCD     Watchdog_IRQHandler          	;22:  Watchdog intertrupt
        DCD     ADC0_IRQHandler           		;23:  ADC 0 interrupt
        DCD     ADC2_IRQHandler           		;24:  ADC 2 interrupt
        DCD     SPI0_IRQHandler      					;25:  SPI0 intertrupt
        DCD     SPI1_IRQHandler      					;26:  SPI1 intertrupt
        DCD     I2C0_IRQHandler      					;27:  IIC 0 interrupt
        DCD     I2C1_IRQHandler          			;28:  IIC 1 intertrupt
        DCD     UART0_RX_TX_IRQHandler      	;29:  UART 0 status intertrupt
        DCD     UART0_ERR_IRQHandler          ;30:  UART 0 error intertrupt
        DCD     UART1_RX_TX_IRQHandler      	;31:  UART 1 status intertrupt
        DCD     UART1_ERR_IRQHandler          ;32:  UART 1 error intertrupt
        DCD     UART2_RX_TX_IRQHandler      	;33:  UART 2 status intertrupt
        DCD     UART2_ERR_IRQHandler          ;34:  UART 2 error intertrupt
        DCD     UART3_RX_TX_IRQHandler      	;35:  UART 3 status intertrupt
        DCD     UART3_ERR_IRQHandler          ;36:  UART 3 error intertrupt
        DCD     ADC1_IRQHandler           		;37:  ADC 1 interrupt
        DCD     ADC3_IRQHandler           		;38:  ADC 3 interrupt
        DCD     SWI_IRQHandler           			;39:  Software interrupt
        DCD     CMP0_IRQHandler           		;40:  CMP 0 interrupt
        DCD     CMP1_IRQHandler           		;41:  CMP 1 interrupt
        DCD     CMP2_IRQHandler           		;42:  CMP 2 interrupt
        DCD     CMP3_IRQHandler           		;43:  CMP 3 interrupt
        DCD     FTM0_IRQHandler           		;44:  FTM 0 interrupt
        DCD     FTM1_IRQHandler           		;45:  FTM 1 interrupt
        DCD     FTM2_IRQHandler           		;46:  FTM 2 interrupt
        DCD     FTM3_IRQHandler           		;47:  FTM 3 interrupt
        DCD     CMT_IRQHandler           			;48:  CMT interrupt
        DCD     PDB0_IRQHandler           		;49:  PDB 0 interrupt
        DCD     PDB1_IRQHandler           		;50:  PDB 1 interrupt
        DCD     PDB2_IRQHandler           		;51:  PDB 2 interrupt
        DCD     PDB3_IRQHandler           		;52:  PDB 3 interrupt
        DCD     PIT0_IRQHandler           		;53:  PIT 0 interrupt
        DCD     PIT1_IRQHandler           		;54:  PIT 1 interrupt
        DCD     PIT2_IRQHandler           		;55:  PIT 2 interrupt
        DCD     PIT3_IRQHandler           		;56:  PIT 3 interrupt
        DCD     RTC_IRQHandler           			;57:  RTC interrupt
        DCD     TSI0_IRQHandler           		;58:  TSI 0 interrupt
        DCD     PORTA_IRQHandler           		;59:  PORT A interrupt
        DCD     PORTB_IRQHandler           		;60:  PORT B interrupt
        DCD     PORTC_IRQHandler           		;61:  PORT C interrupt
        DCD     PORTD_IRQHandler           		;62:  PORT D interrupt
        DCD     PORTE_IRQHandler           		;63:  PORT E interrupt


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
      PUBWEAK DMA0_IRQHandler
      PUBWEAK DMA1_IRQHandler
      PUBWEAK DMA2_IRQHandler
      PUBWEAK DMA3_IRQHandler
      PUBWEAK DMA4_IRQHandler
      PUBWEAK DMA5_IRQHandler
      PUBWEAK DMA6_IRQHandler
      PUBWEAK DMA7_IRQHandler
      PUBWEAK DMA8_IRQHandler
      PUBWEAK DMA9_IRQHandler
      PUBWEAK DMA10_IRQHandler
      PUBWEAK DMA11_IRQHandler
      PUBWEAK DMA12_IRQHandler
      PUBWEAK DMA13_IRQHandler
      PUBWEAK iEVENT0_IRQHandler
      PUBWEAK iEVENT1_IRQHandler
      PUBWEAK INT_DMA_Error_IRQHandler
      PUBWEAK FTMRA_IRQHandler
      PUBWEAK Read_Collision_IRQHandler
      PUBWEAK LVD_LVW_IRQHandler
      PUBWEAK ICS_IRQHandler
      PUBWEAK Watchdog_IRQHandler
      PUBWEAK ADC0_IRQHandler
      PUBWEAK ADC2_IRQHandler
      PUBWEAK SPI0_IRQHandler
      PUBWEAK SPI1_IRQHandler
      PUBWEAK I2C0_IRQHandler
      PUBWEAK I2C1_IRQHandler
      PUBWEAK UART0_RX_TX_IRQHandler
      PUBWEAK UART0_ERR_IRQHandler
      PUBWEAK UART1_RX_TX_IRQHandler
      PUBWEAK UART1_ERR_IRQHandler
      PUBWEAK UART2_RX_TX_IRQHandler
      PUBWEAK UART2_ERR_IRQHandler
      PUBWEAK UART3_RX_TX_IRQHandler
      PUBWEAK UART3_ERR_IRQHandler
      PUBWEAK ADC1_IRQHandler
      PUBWEAK ADC3_IRQHandler
      PUBWEAK SWI_IRQHandler
      PUBWEAK CMP0_IRQHandler
      PUBWEAK CMP1_IRQHandler
      PUBWEAK CMP2_IRQHandler
      PUBWEAK CMP3_IRQHandler
      PUBWEAK FTM0_IRQHandler
      PUBWEAK FTM1_IRQHandler
      PUBWEAK FTM2_IRQHandler
      PUBWEAK FTM3_IRQHandler
      PUBWEAK CMT_IRQHandler
      PUBWEAK PDB0_IRQHandler
      PUBWEAK PDB1_IRQHandler
      PUBWEAK PDB2_IRQHandler
      PUBWEAK PDB3_IRQHandler
      PUBWEAK PIT0_IRQHandler
      PUBWEAK PIT1_IRQHandler
      PUBWEAK PIT2_IRQHandler
      PUBWEAK PIT3_IRQHandler
      PUBWEAK RTC_IRQHandler
      PUBWEAK TSI0_IRQHandler
      PUBWEAK PORTA_IRQHandler
      PUBWEAK PORTB_IRQHandler
      PUBWEAK PORTC_IRQHandler
      PUBWEAK PORTD_IRQHandler
      PUBWEAK PORTE_IRQHandler

      SECTION .text:CODE:REORDER:NOROOT(1)
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
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
iEVENT0_IRQHandler
iEVENT1_IRQHandler
INT_DMA_Error_IRQHandler
FTMRA_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
ICS_IRQHandler
Watchdog_IRQHandler
ADC0_IRQHandler
ADC2_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
UART0_RX_TX_IRQHandler
UART0_ERR_IRQHandler
UART1_RX_TX_IRQHandler
UART1_ERR_IRQHandler
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler
ADC1_IRQHandler
ADC3_IRQHandler
SWI_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
CMP3_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
FTM3_IRQHandler
CMT_IRQHandler
PDB0_IRQHandler
PDB1_IRQHandler
PDB2_IRQHandler
PDB3_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
RTC_IRQHandler
TSI0_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
Default_Handler

        B Default_Handler
        END
