;/*****************************************************************************
;*   startup.s:  Startup file for Philips LPC29xx Family Microprocessors
;*
;*   Copyright(C) 2007, NXP Semiconductor
;*   All rights reserved.
;*
;*   History
;*   2007.09.01  ver 1.00    Prelimnary version, first Release
;*
;*****************************************************************************/

	PRESERVE8

; ARM - CP15 c1 Control Register
DTCM_enabled    	EQU 1<<2		; Data TCM
ITCM_enabled    	EQU 1<<12		; Instruction TCM
WB_enabled      	EQU 1<<3		; Write Buffer

;define the stack size
SVC_STACK_LEGTH		EQU	256
FIQ_STACK_LEGTH		EQU	256
IRQ_STACK_LEGTH		EQU	256
ABT_STACK_LEGTH		EQU	64
UND_STACK_LEGTH		EQU	64

I_Bit				EQU	0x80
F_Bit				EQU	0x40

USR32Mode			EQU	0x10
SVC32Mode			EQU	0x13
SYS32Mode			EQU	0x1F
IRQ32Mode			EQU	0x12
FIQ32Mode			EQU	0x11
ABT32Mode			EQU	0x17
UDF32Mode			EQU	0x1B

;The imported labels
    IMPORT  IRQ_Exception	; IRQ interrupt handler
    IMPORT  FIQ_Exception	; Fast interrupt exceptions handler
	IMPORT	IRQ_VectorTable
	IMPORT	FIQ_VectorTable 
    IMPORT  __main			; The entry point to the main function
	IMPORT	VectorRemap		
    IMPORT  TargetResetInit	; initialize the target board
	IMPORT  RGU_Status
	IMPORT  RGU_SRC_Status
    IMPORT  SWI_Handler		; SWI handler from swi_handler.s
       
;The exported labels
    EXPORT  bottom_of_heap
    EXPORT  StackUsr
    EXPORT  Reset
    EXPORT __user_initial_stackheap
    
    CODE32

    AREA    Reset,CODE,READONLY
    ENTRY

;interrupt vectors
ResetStart
	LDR     PC, ResetAddr
	LDR     PC, UndefinedAddr
	LDR     PC, SWI_Addr
	LDR     PC, PrefetchAddr
	LDR     PC, DataAbortAddr
	LDR     PC, ReservedAddr
	LDR     PC, IRQ_Addr
	LDR     PC, FIQ_Addr

ResetAddr		DCD     ResetInit
UndefinedAddr	DCD     Undefined_Handler
SWI_Addr		DCD     SWI_Handler
PrefetchAddr	DCD     Prefetch_Handler
DataAbortAddr	DCD     DataAbort_Handler
ReservedAddr	DCD     Reserved_Handler
IRQ_Addr		DCD     IRQ_Exception
FIQ_Addr		DCD     FIQ_Exception

Undefined_Handler
	B	Undefined_Handler

Reserved_Handler					
	B	Reserved_Handler

Prefetch_Handler
	B	Prefetch_Handler

DataAbort_Handler
	B	DataAbort_Handler

;*****************************************************************************
;*  Initialize the stacks                                                    *
;*  Function : void InitStack(void)                                          *
;*      Parameters                                                           *
;*          input  : None                                                    * 
;*          output : None                                                    *
;*****************************************************************************
InitStack    
	MOV     R0, LR
;Build the SVC stack
	MSR     CPSR_c, #SVC32Mode		
	LDR     SP, StackSvc	
;Build the IRQ stack
	MSR     CPSR_c, #IRQ32Mode
	LDR     SP, StackIrq
;Build the FIQ stack
	MSR     CPSR_c, #FIQ32Mode
	LDR     SP, StackFiq
;Build the DATAABORT stack
	MSR     CPSR_c, #ABT32Mode
	LDR     SP, StackAbt
;Build the UDF stack
	MSR     CPSR_c, #UDF32Mode
	LDR     SP, StackUnd
;Build the SYS stack
	MSR     CPSR_c, #SYS32Mode
	LDR     SP, =StackUsr
	BX      R0

;******************************************************************************
;*  Reset Entry                                                               *
;*  Function : void ResetInit(void)                                           *
;*      Parameters                                                            *
;*          input  : None                                                     * 
;*          output : None                                                     *
;******************************************************************************
ResetInit
	; Set global core configurations
	MRC     p15, 0, r4, c1, c0, 0   ; Read CP15
	ORR     r4, r4, #DTCM_enabled	; Enable Data TCM
	; At power up, the interrupt vector is mapped to addr. 0 already,
	; enabling instruction TCM will wipe out the mirror of the
	; vector table. A remap will be performed there after. */
	ORR     r4, r4, #ITCM_enabled	; Enable Instruction TCM
	ORR     r4, r4, #WB_enabled      ; Enable Write Buffer
	MCR     p15, 0, r4, c1, c0, 0   ; Write CP15

	BL      InitStack		;Initialize the stack
	BL		VectorRemap
	; below two lines are for RGU test only
	BL		RGU_Status
	BL		RGU_SRC_Status
	BL      TargetResetInit	;Initialize the target board
					;Jump to the entry point of C program
	B       __main

;******************************************************************************
;*  Initialize the function library stacks and heaps, can't be                * 
;*  deleted                                                                   *
;*  Function : void __user_initial_stackheap(void)                            *
;*      Parameters                                                            *
;*          input  : None                                                     * 
;*          output : None                                                     *
;******************************************************************************
__user_initial_stackheap    
	LDR     r0, =bottom_of_heap
	BX      lr
            
StackSvc	DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4
StackIrq	DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq	DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt	DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd	DCD     UndStackSpace + (UND_STACK_LEGTH - 1)* 4
    
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
SvcStackSpace	SPACE   SVC_STACK_LEGTH * 4  ;Stack spaces for ADMIN Mode
IrqStackSpace	SPACE   IRQ_STACK_LEGTH * 4  ;Stack spaces for IRQ Mode
FiqStackSpace	SPACE   FIQ_STACK_LEGTH * 4  ;Stack spaces for FIQ Mode
AbtStackSpace	SPACE   ABT_STACK_LEGTH * 4  ;Stack spaces for ABORT Mode
UndStackSpace	SPACE   UND_STACK_LEGTH * 4  ;Stack spaces for UNDEF Mode

        AREA    Heap, DATA, NOINIT
bottom_of_heap	SPACE   1

        AREA    Stacks, DATA, NOINIT
StackUsr    	SPACE   1

        END

;/*****************************************************************************
;**                            End Of File
;*****************************************************************************/
