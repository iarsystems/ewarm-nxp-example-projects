/***********************************************************************
 *
 *  STARTUP.S:  Startup file for NXP LPC23xx device series
 *
 ***********************************************************************
 *  The STARTUP.S code is executed after CPU Reset. This file may be
 *  translated with the following SET symbols. In uVision these SET
 *  symbols are entered under Options - ASM - Set.
 *
 *  REMAP: when set the startup code initializes the register MEMMAP
 *  which overwrites the settings of the CPU configuration pins. The
 *  startup and interrupt vectors are remapped from:
 *     0x00000000  default setting (not remapped)
 *     0x80000000  when EXTMEM_MODE is used
 *     0x40000000  when RAM_MODE is used
 *
 *  EXTMEM_MODE: when set the device is configured for code execution
 *  from external memory starting at address 0x80000000. The startup
 *  vectors are located to 0x80000000.
 *
 *  RAM_MODE: when set the device is configured for code execution
 *  from on-chip RAM starting at address 0x40000000. The startup
 *  vectors are located to 0x40000000.
 ***********************************************************************/

/* Map Preprocessor definitions to assembler definitions/symbols */
EXTMEM_MODE     DEFINE  0
RAM_MODE        DEFINE  MAPMODEFLAG 	/* RAM_MODE activate */
REMAP           DEFINE  MAPMODEFLAG 	/* REMMAPPING activate */
VECTREMAPPED    DEFINE  0

    IF (RAM_MODE)
	#message "RAM_MODE enabled"
    ELSE
	#message "ROM_MODE enabled"
    ENDIF

    IF (REMAP)
	#message "REMMAPPING enabled"
    ELSE
	#message "REMMAPPING disable"
    ENDIF

    IF (VECTREMAPPED)
	#message "Vectors at start of RAM"
    ELSE
	#message "Vectors at start of CODE"
    ENDIF

/* From swi_handler patch */
SWI_IRQ_DIS     DEFINE	0
SWI_IRQ_EN      DEFINE	1
SWI_FIQ_DIS     DEFINE	2
SWI_FIQ_EN      DEFINE	3
SWI_GET_CPSR    DEFINE	4
SWI_IRQ_REST    DEFINE	5
SWI_FIQ_REST    DEFINE	6

I_Bit           DEFINE  0x80
F_Bit           DEFINE  0x40
T_Bit           DEFINE  0x20

/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */
Mode_USR        DEFINE  0x10
Mode_FIQ        DEFINE  0x11
Mode_IRQ        DEFINE  0x12
Mode_SVC        DEFINE  0x13
Mode_ABT        DEFINE  0x17
Mode_UND        DEFINE  0x1B
Mode_SYS        DEFINE  0x1F

/* -------------------- System Control Block (SCB) Module Definitions -------------------- */
SCB_BASE        EQU     0xE01FC000      /* SCB Base Address */
PLLCON_OFS      EQU     0x80            /* PLL Control Offset */
PLLCFG_OFS      EQU     0x84            /* PLL Configuration Offset	*/
PLLSTAT_OFS     EQU     0x88            /* PLL Status Offset */
PLLFEED_OFS     EQU     0x8C            /* PLL Feed Offset */
CCLKCFG_OFS     EQU     0x104           /* CPU Clock Divider Reg Offset */
USBCLKCFG_OFS   EQU     0x108           /* USB Clock Divider Reg Offset	*/
CLKSRCSEL_OFS   EQU     0x10C           /* Clock Source Select Reg Offset */
SCS_OFS         EQU     0x1A0           /* System Control and Status Reg Offset */
PCLKSEL0_OFS    EQU     0x1A8           /* Peripheral Clock Select Reg 0 Offset	*/
PCLKSEL1_OFS    EQU     0x1AC           /* Peripheral Clock Select Reg 1 Offset	*/
PCON_OFS        EQU     0x0C0           /* Power Mode Control Reg Offset */
PCONP_OFS       EQU     0x0C4           /* Power Control for Periphs Reg Offset */

/* -------------------- Constants -------------------- */
OSCRANGE        EQU     (1<<4)          /* Oscillator Range Select */
OSCEN           EQU     (1<<5)          /* Main oscillator Enable */
OSCSTAT         EQU     (1<<6)          /* Main Oscillator Status */
PLLCON_PLLE     EQU     (1<<0)          /* PLL Enable */
PLLCON_PLLC     EQU     (1<<1)          /* PLL Connect */
PLLSTAT_M       EQU     (0x7FFF<<0)     /* PLL M Value */
PLLSTAT_N       EQU     (0xFF<<16)      /* PLL N Value */
PLLSTAT_PLOCK   EQU     (1<<26)         /* PLL Lock Status */

/* -------------------- Memory Accelerator Module (MAM) Definitions -------------------- */
MAM_BASE        EQU	0xE01FC000      /* MAM Base Address */
MAMCR_OFS       EQU     0x00            /* MAM Control Offset */
MAMTIM_OFS      EQU     0x04            /* MAM Timing Offset */

MAM_SETUP       EQU     1
MAMCR_Val       EQU     0x00000002
MAMTIM_Val      EQU     0x00000004

CLOCK_SETUP     EQU     1
SCS_Val         EQU     0x00000020
CLKSRCSEL_Val   EQU     0x00000001
PLLCFG_Val      EQU     0x0000000B
CCLKCFG_Val     EQU     0x00000005
USBCLKCFG_Val   EQU     0x00000005
PCLKSEL0_Val    EQU     0x00000000
PCLKSEL1_Val    EQU     0x00000000
PCONP_Val       EQU     0x04280FDE

/* -------------------- Memory Mapping Definitions -------------------- */
MEMMAP          EQU     0xE01FC040      /* Memory Mapping Control */

/* -------------------- Vector Interrupt Controller Definitions -------------------- */
VIC_base_addr   EQU     0xFFFFF000
VIC_vect_offs   EQU     0xF00

/* Starupt Code must be linked first at Address at which it expects to run */
    IF      (EXTMEM_MODE)
CODE_BASE   DEFINE  0x80000000
    ELSEIF  (RAM_MODE)
CODE_BASE   DEFINE  0x40000000
    ELSE
CODE_BASE   DEFINE  0x00000000
    ENDIF

/* -------------------- Code Start Here -------------------- */

        ;;;;;;;;;;;;;;;;;;;;;;;
        ;;  MODULE CSTARTUP  ;;
        ;;;;;;;;;;;;;;;;;;;;;;;
        PROGRAM ?cstartup

        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)

        ;;;;;;;;;;;;;;;;;;;;;;;
        ;;  SECTION .intvec  ;;
        ;;;;;;;;;;;;;;;;;;;;;;;
        SECTION .intvec:CODE(2)

        PUBLIC  __vector
        PUBLIC  __vector_0x14

        ARM

__iar_init$$done:
__vector:
        LDR     PC, Reset_Addr
        LDR     PC, Undef_Addr
        LDR     PC, SWI_Addr
        LDR     PC, PAbt_Addr
        LDR     PC, DAbt_Addr
__vector_0x14:
        NOP     /* Reserved Vector */
        LDR     PC, IRQ_Wrapper_Addr
        LDR     PC, FIQ_Addr

Undef_Handler:      B       Undef_Handler
PAbt_Handler:       B       PAbt_Handler
DAbt_Handler:       B       DAbt_Handler
FIQ_Handler:        B       FIQ_Handler

        DATA

Reset_Addr:         DCD     Reset_Handler
Undef_Addr:         DCD     Undef_Handler
SWI_Addr:           DCD     SoftwareInterrupt      /* in swi_handler.s */
PAbt_Addr:          DCD     PAbt_Handler
DAbt_Addr:          DCD     DAbt_Handler
                    DCD     0                      /* Reserved Address */
IRQ_Wrapper_Addr:   DCD     __IRQ_Wrapper
FIQ_Addr:           DCD     FIQ_Handler

        LTORG

        ;;;;;;;;;;;;;;;;;;;;;
        ;;  SECTION .text  ;;
        ;;;;;;;;;;;;;;;;;;;;;
        SECTION .text:CODE:NOROOT(2)

        PUBLIC  __iar_program_start
        EXTERN  ?main
        REQUIRE __vector

        ARM

__iar_program_start:
Reset_Handler:

/* Setup Clock */
    IF (CLOCK_SETUP)
    LDR     R0, =SCB_BASE
    MOV     R1, #0xAA
    MOV     R2, #0x55

/* Configure and Enable PLL */
    LDR     R3, =SCS_Val          /* Enable main oscillator */
    STR     R3, [R0, #SCS_OFS]

    IF (SCS_Val & OSCEN)
OSC_Loop:
    LDR     R3, [R0, #SCS_OFS]    /* Wait for main osc stabilize */
    ANDS    R3, R3,  #OSCSTAT
    BEQ     OSC_Loop
    ENDIF

    LDR     R3, =CLKSRCSEL_Val    /* Select PLL source clock */
    STR     R3, [R0, #CLKSRCSEL_OFS]
    LDR     R3, =PLLCFG_Val
    STR     R3, [R0, #PLLCFG_OFS]
    STR     R1, [R0, #PLLFEED_OFS]
    STR     R2, [R0, #PLLFEED_OFS]
    MOV     R3, #PLLCON_PLLE
    STR     R3, [R0, #PLLCON_OFS]
    STR     R1, [R0, #PLLFEED_OFS]
    STR     R2, [R0, #PLLFEED_OFS]

/* Wait until PLL Locked */
PLL_Loop:
    LDR     R3, [R0, #PLLSTAT_OFS]
    ANDS    R3, R3, #PLLSTAT_PLOCK
    BEQ     PLL_Loop

M_N_Lock:
    LDR     R3, [R0, #PLLSTAT_OFS]
    LDR     R4, =(PLLSTAT_M | PLLSTAT_N)
    AND     R3, R3, R4
    LDR     R4, =PLLCFG_Val
    EORS    R3, R3, R4
    BNE     M_N_Lock

/* Setup CPU clock divider */
    MOV     R3, #CCLKCFG_Val
    STR     R3, [R0, #CCLKCFG_OFS]

/* Setup USB clock divider */
    LDR     R3, =USBCLKCFG_Val
    STR     R3, [R0, #USBCLKCFG_OFS]

/* Setup Peripheral Clock */
    LDR     R3, =PCLKSEL0_Val
    STR     R3, [R0, #PCLKSEL0_OFS]
    LDR     R3, =PCLKSEL1_Val
    STR     R3, [R0, #PCLKSEL1_OFS]

/* Switch to PLL Clock */
    MOV     R3, #(PLLCON_PLLE | PLLCON_PLLC)
    STR     R3, [R0, #PLLCON_OFS]
    STR     R1, [R0, #PLLFEED_OFS]
    STR     R2, [R0, #PLLFEED_OFS]
    ENDIF   /* CLOCK_SETUP */

/* Setup MAM */
    IF (MAM_SETUP)
    LDR     R0, =MAM_BASE
    MOV     R1, #MAMTIM_Val
    STR     R1, [R0, #MAMTIM_OFS]
    MOV     R1, #MAMCR_Val
    STR     R1, [R0, #MAMCR_OFS]
    ENDIF

/* Memory Mapping (when Interrupt Vectors are in RAM) */
    IF      (REMAP)
    LDR     R0, =MEMMAP
        IF      (EXTMEM_MODE)
        MOV     R1, #3
        ELSEIF  (RAM_MODE) || (VECTREMAPPED)
        #message "MEMMAP to 2 on init"
        MOV     R1, #2
        ELSE
        MOV     R1, #1
        ENDIF   /* EXTMEM_MODE */
    STR     R1, [R0]
    ENDIF   /* REMAP */

/* Setup Stack for each mode */

/* Enter Undefined Instruction Mode and set its Stack Pointer */
    MSR     CPSR_c, #Mode_UND|I_Bit|F_Bit
    LDR     SP, =SFE(UND_STACK)

/* Enter Abort Mode and set its Stack Pointer */
    MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit
    LDR     SP, =SFE(ABT_STACK)

/* Enter FIQ Mode and set its Stack Pointer  */
    MSR     CPSR_c, #Mode_FIQ|I_Bit|F_Bit
    LDR     SP, =SFE(FIQ_STACK)

/* Enter IRQ Mode and set its Stack Pointer */
    MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit
    LDR     SP, =SFE(IRQ_STACK)

/* Enter Supervisor Mode and set its Stack Pointer */
    MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit
    LDR     SP, =SFE(SVC_STACK)

/* Enter User Mode and set its Stack Pointer */
    MSR     CPSR_c, #Mode_USR   /* Interrupts enabled */
    LDR     SP, =SFE(CSTACK)

/* Enter the C Run-time library */
    B       ?main

    LTORG

/* ---------- __IRQ_Wrapper ---------- */

__IRQ_Wrapper:
/* Manage Exception Entry  */
/* Adjust and save LR_irq in IRQ stack  */
    sub     lr, lr, #4
    stmfd   sp!, {lr}

/* Save SPSR need to be saved for nested interrupt */
    mrs     r14, SPSR
    stmfd   sp!, {r14}

/* Save and r0 in IRQ stack  */
    stmfd   sp!, {r0}

/* Write in the IVR to support Protect Mode  */
/* No effect in Normal Mode  */
/* De-assert the NIRQ and clear the source in Protect Mode */
/* R14 = LR */
    ldr     r14, =VIC_base_addr
    ldr     r0 , [r14, #VIC_vect_offs]

/* Enable Interrupt and Switch in Supervisor Mode */
    msr     CPSR_c, #Mode_SVC

/* Save scratch/used registers and LR in User Stack */
    stmfd   sp!, { r1-r12, r14 }

/* Branch to the routine pointed by the VIC-Vector-Address  */
    mov     r14, pc
    bx      r0

/* Restore scratch/used registers and LR from User Stack*/
    ldmia   sp!, { r1-r12, r14 }

/* Disable Interrupt and switch back in IRQ mode */
    msr     CPSR_c, #I_Bit | Mode_IRQ

/* Restore SPSR_irq and r0 from IRQ stack */
    ldmia   sp!, {r0}

/* Restore SPSR_irq and r0 from IRQ stack */
    ldmia   sp!, {r14}
    msr     SPSR_cxsf, r14

/* Restore adjusted LR_irq from IRQ stack directly in the PC */
    ldmia   sp!, {pc}^

    LTORG

/* ---------- source from swi_handler ----------- */

/*********************************************************************
*  SWI interrupt handler
*  Function :  SoftwareInterrupt(SWI_Number)
*              See below "SwiFunction" table
*  Parameters:    None
*  input  :       SWI_Number (extracted from SWI command automaticly)
*  output :       states for some SWIs - see below
**********************************************************************/

MAX_SWI SET  (((SwiTableEnd-SwiTableStart)/4)-1)

SoftwareInterrupt:
SWI_HandlerMT:
	STMFD   sp!, {r4, lr}          /* store regs. */
	MRS     r4, spsr
	TST     r4, #T_Bit             /* test for thumb */
	LDRNEH  r4, [lr, #-2]          /* NE->thumb - get swi instruction code */
	BICNE   r4, r4, #0xff00        /* NE->thumb - clear top 8 bits leaving swi "comment field"=number */
	LDREQ   r4, [lr, #-4]          /* EQ->arm - get swi instruction code */
	BICEQ   r4, r4, #0xff000000    /* EQ->arm - clear top 8 bits leaving swi "comment field"=number */
	CMP     r4, #MAX_SWI           /* range-check */
	LDRLS   pc, [pc, r4, LSL #2]   /* jump to routine if <= MAX (LS) */
SWIOutOfRange:
	B       SWIOutOfRange

/* Jump-Table */
SwiTableStart:
	DCD     IRQDisable	/* 0 */
	DCD     IRQEnable	/* 1 */
	DCD     FIQDisable	/* 2 */
	DCD     FIQEnable	/* 3 */
	DCD     CPSRget		/* 4 */
	DCD     IRQRestore	/* 5 */
	DCD     FIQRestore	/* 6 */
SwiTableEnd:

IRQDisable:
	MRS     r0, SPSR                        /* Get SPSR = return value */
	ORR     r4, r0, #I_Bit                  /* I_Bit set */
	MSR     SPSR_c, r4                      /* Set SPSR */
	B       EndofSWI

IRQEnable:
	MRS     r0, SPSR                        /* Get SPSR = return value */
	BIC     r4, r0, #I_Bit                  /* I_Bit clear */
	MSR     SPSR_c, r4                      /* Set SPSR */
	B       EndofSWI

FIQDisable:
	MRS     r0, SPSR
	ORR     r4, r0, #F_Bit
	AND     r0, r0, #F_Bit
	MSR     SPSR_c, r4
	B       EndofSWI

FIQEnable:
	MRS     r0, SPSR
	BIC     r4, r0, #F_Bit
	AND     r0, r0, #F_Bit
	MSR     SPSR_c, r4
	B       EndofSWI

CPSRget:
	MRS     r0, SPSR                        /* Get SPSR */
	B       EndofSWI

IRQRestore:
	MRS     r4, SPSR                        /* Get SPSR */
	AND     r0, r0, #I_Bit
	TST     r0, #I_Bit                      /* Test input for I_Bit */
	BICEQ   r4, r4, #I_Bit
	ORRNE   r4, r4, #I_Bit
	MSR     SPSR_c, r4
	B       EndofSWI

FIQRestore:
	MRS     r4, SPSR                        /* Get SPSR */
	AND     r0, r0, #F_Bit
	TST     r0, #F_Bit                      /* Test input for F_Bit */
	BICEQ   r4, r4, #F_Bit
	ORRNE   r4, r4, #F_Bit
	MSR     SPSR_c, r4
	B       EndofSWI

EndofSWI:
	LDMFD   sp!, {r4,pc}^

/**********************************************************************
 *  Call SWI to restore IRQ
 *  Function : void IntEnable(uint32_t)
 *  Parameters:      None
 *  input  :         newstate
 *                   if I_bit in newstate cleared->IRQ on  -> clear I_BIT
 *                   if I_bit in newstate set    ->IRQ off -> set I_Bit
 *  output :         None
 **********************************************************************/
IntRestore:
        SWI     SWI_IRQ_REST
        BX      lr

/**********************************************************************
 *  Call SWI to restore FIQ
 *  Function : void IntEnable(uint32_t)
 *  Parameters:      None
 *  input  :         newstate
 *                   if F_bit in newstate cleared->FIQ on  -> clear F_BIT
 *                   if F_bit in newstate set    ->FIQ off -> set F_Bit
 *  output :         None
 **********************************************************************/
FiqRestore:
        SWI     SWI_FIQ_REST
        BX      lr

/**********************************************************************
 *  Call SWI to read IRQ/FIQ-status
 *  Function : uint32_t IntEnable(void)
 *  Parameters:      None
 *  input  :         None
 *  output :         CPSR (SPSR_SVC)
 **********************************************************************/
IntGetCPSR:
        SWI     SWI_GET_CPSR
        BX      lr

/**********************************************************************
 *  Call SWI to enable IRQ
 *  Function : uint32_t IntEnable(void)
 *  Parameters:      None
 *  input  :         None
 *  output :         previous status
 *                   I_Bit clear if IRQs were enabled
 *                   I_Bit set   if IRQs were disabled
 **********************************************************************/
IntEnable:
        SWI     SWI_IRQ_EN
        BX      lr

/**********************************************************************
 *  Call SWI to disable IRQ
 *  Function : uint32_t IntDisable(void)
 *  Parameters     : None
 *  input          : None
 *  output :         previous status
 *                   I_Bit clear if IRQs were enabled
 *                   I_Bit set   if IRQs were disabled
 **********************************************************************/
IntDisable:
        SWI     SWI_IRQ_DIS
        BX      lr

/**********************************************************************
 *  Call SWI to enable FIQ
 *  Function : uint32_t FiqEnable(void)
 *  Parameters:      None
 *  input  :         None
 *  output :         previous status
 *                   F_Bit clear if FIQs were enabled
 *                   F_Bit set   if FIQs were disabled
 **********************************************************************/
FiqEnable:
        SWI     SWI_FIQ_EN
        BX      lr

/**********************************************************************
 *  Call SWI to disable FIQ
 *  Function : uint32_t FiqDisable(void)
 *  Parameters     : None
 *  input          : None
 *  output :         previous status
 *                   F_Bit clear if FIQs were enabled
 *                   F_Bit set   if FIQs were disabled
 **********************************************************************/
FiqDisable:
        SWI     SWI_FIQ_DIS
        BX      lr

        LTORG

        END
