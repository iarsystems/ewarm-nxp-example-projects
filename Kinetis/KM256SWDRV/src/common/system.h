/******************************************************************************
 * (c) Copyright 2010-2016, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      system.h
 * @version   1.0.4.0
 * @date      Mar-27-2016
 * @brief     Kinetis-M system definitions header file.
 ******************************************************************************/
#ifndef __SYSTEM_H
#define __SYSTEM_H

#if !defined(MCU_MKM34Z7)
  #error "The system.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/* function prototypes for weakly defined interrupt service routines; if both */
/* a non-weakly and weekly defined interrupt service routines exist then all  */
/* calls will be resolved to the non-weakly interrupt service routine         */
extern void default_isr  (void);
extern void hardfault_isr(void);
extern void nmi_isr      (void);
extern void pendsv_isr   (void);
extern void slcd_isr     (void);
extern void mcg_isr      (void);

/* import prototype of the startup function                                   */
#if defined(__ICCARM__)
  extern  void __iar_program_start (void);
#elif defined(__GNUC__)
  extern  void __gcc_program_start (void);
#elif defined(__CC_ARM)
  extern  void __uv_program_start  (void);
#endif

/******************************************************************************/
/**** ARM core cortex M0+ System handler Vectors ******************************/
/******************************************************************************/
//                                   Address       Vector IRQ Source module   Source description
#define VECTOR_000 (void (*)(void))0x20005ff8
#if defined(__ICCARM__)           // 0x0000_0000 0 -      ARM core    Initial Stack Pointer
  #define VECTOR_001 __iar_program_start
#elif defined(__GNUC__)           // 0x0000_0004 1 -      ARM core    Initial Program Counter
  #define VECTOR_001 __gcc_program_start
#elif defined(__CC_ARM)           // 0x0000_0004 1 -      ARM core    Initial Program Counter
  #define VECTOR_001 __uv_program_start
#endif
#define VECTOR_002  nmi_isr       // 0x0000_0008 2 -      ARM core    Non-Maskable Interrupt (NMI)
#define VECTOR_003  hardfault_isr // 0x0000_000C 3 -      ARM core    Hard Fault
#define VECTOR_004  default_isr   // 0x0000_0010 4
#define VECTOR_005  default_isr   // 0x0000_0014 5
#define VECTOR_006  default_isr   // 0x0000_0018 6
#define VECTOR_007  default_isr   // 0x0000_001C 7
#define VECTOR_008  default_isr   // 0x0000_0020 8
#define VECTOR_009  default_isr   // 0x0000_0024 9
#define VECTOR_010  default_isr   // 0x0000_0028 10
#define VECTOR_011  svcall_isr    // 0x0000_002C 11 -     ARM core    Supervisor call (SVCall)
#define VECTOR_012  default_isr   // 0x0000_0030 12
#define VECTOR_013  default_isr   // 0x0000_0034 13
#define VECTOR_014  pendsv_isr    // 0x0000_0038 14 -     ARM core    Pendable request for system service (PendableSrvReq)
#define VECTOR_015  syst_isr      // 0x0000_003C 15 -     ARM core    System tick timer (SysTick)

/******************************************************************************/
/**** Non-core On Platform Vectors ********************************************/
/******************************************************************************/
//                                   Address       Vector IRQ Source module   Source description
#define VECTOR_016  dma0_isr      // 0x0000_0040 16   0   DMA0        DMA Channel 0 transfer complete
#define VECTOR_017  dma1_isr      // 0x0000_0044 17   1   DMA1        DMA Channel 1 transfer complete
#define VECTOR_018  dma2_isr      // 0x0000_0048 18   2   DMA2        DMA Channel 2 transfer complete
#define VECTOR_019  dma3_isr      // 0x0000_004C 19   3   DMA3        DMA Channel 3 transfer complete
#define VECTOR_020  spix_isr      // 0x0000_0050 20   4   SPI0,1      OR'ed Interrupt SPI0 and SPI1
#define VECTOR_021  pdb_isr       // 0x0000_0054 21   5   PDB0        OR'ed Interrupt of PDB0
#define VECTOR_022  pmc_isr       // 0x0000_0058 22   6   PMC         Low-voltage detect, low-voltage warning
#define VECTOR_023  tmr0_isr      // 0x0000_005C 23   7   TMR0        Quad Timer Counter 0
#define VECTOR_024  tmr1_isr      // 0x0000_0060 24   8   TMR1        Quad Timer Counter 1
#define VECTOR_025  tmr2_isr      // 0x0000_0064 25   9   TMR2        Quad Timer Counter 2
#define VECTOR_026  tmr3_isr      // 0x0000_0068 26   10  TMR3        Quad Timer Counter 3
#define VECTOR_027  pitx_isr      // 0x0000_006C 27   11  PIT0,1      OR'ed Interrupt of PIT0 and PIT1
#define VECTOR_028  llwu_isr      // 0x0000_0070 28   12  LLWU        Low Leakage Wakeup
#define VECTOR_029  ftfa_isr      // 0x0000_0074 29   13  FLASH       OR'ed interrupt for Flash
#define VECTOR_030  cmpx_isr      // 0x0000_0078 30   14  ACMP0,1,2   OR'ed Interrupt of 3 ACMP
#define VECTOR_031  slcd_isr      // 0x0000_007C 31   15  SLCD        OR'ed Interrupt of SLCD
#define VECTOR_032  adc_isr       // 0x0000_0080 32   16  ADC         OR'ed Interrupt from SAR
#define VECTOR_033  portx_isr     // 0x0000_0084 33   17  PORTx       OR'ed Interrupt from all GPIO (PTx) ports
#define VECTOR_034  rnga_isr      // 0x0000_0088 34   18  RNGA        OR'ed Interrupt from RNGA
#define VECTOR_035  uartx_isr     // 0x0000_008C 35   19  UART0,1,2,3 OR'ed Interrupt from all UARTs
#define VECTOR_036  mmau_isr      // 0x0000_0090 36   20  MMAU        OR'ed Interrupt from Memory Mapped Arithmetic Unit
#define VECTOR_037  afe0_isr      // 0x0000_0094 37   21  AFE0        AFE Channel 0 OR'ed Interrupt
#define VECTOR_038  afe1_isr      // 0x0000_0098 38   22  AFE1        AFE Channel 1 OR'ed Interrupt
#define VECTOR_039  afe2_isr      // 0x0000_009C 39   23  AFE2        AFE Channel 2 OR'ed Interrupt
#define VECTOR_040  afe3_isr      // 0x0000_00A0 40   24  AFE3        AFE Channel 3 OR'ed Interrupt
#define VECTOR_041  irtc_isr      // 0x0000_00A4 41   25  iRTC        OR'ed I2C interrupt
#define VECTOR_042  i2cx_isr      // 0x0000_00A8 42   26  I2C0,1      OR'ed interrupt for I2C0 and I2C1
#define VECTOR_043  lpuart_isr    // 0x0000_00AC 43   27  LPUART      OR'ed Interrupt of LPUART
#define VECTOR_044  mcg_isr       // 0x0000_00B0 44   28  MCG         MCG Loss of Clock, Loss of Lock
#define VECTOR_045  wdogewm_isr   // 0x0000_00B4 45   29  WDOG/EWM    OR'ed WDOG Interrupt and External Watchdog Monitor
#define VECTOR_046  lptmr_isr     // 0x0000_00B8 46   30  LPTMR       OR'ed LPTIMER interrupt
#define VECTOR_047  xbar_isr      // 0x0000_00BC 47   31  XBAR        Peripheral XBAR OR'ed Interrupt

/******************************************************************************/
/**** Boot options default parameters - can be overloaded in appconfig.h ******/
/******************************************************************************/
/** Flash Security FTFA_FSEC[1-0]: ********************************************/
/*  2 - MCU security status is unsecure (default - shipping condition)        */
/*  3 - MCU security status is secure                                         */
#ifdef BOOT_FLASH_SECURE
  #define MCU_SECURITY   3
#else
  #define MCU_SECURITY   2
#endif

/** Low-power boot FTFA_FOPT[0]: **********************************************/
/*  0 - Core and system clock divider set to divide by 8                      */
/*  1 - Core and system clock divider set to divide by 1 (default)            */
#ifdef BOOT_DIV_CLK_BY_8
  #define BOOT_LPBOOT    0
#else
  #define BOOT_LPBOOT    1
#endif

/** NMI_B pin enable FTFA_FOPT[2]: ********************************************/
/*  0 - NMI_B pin is disabled. NMI functionality is disabled                  */
/*  1 - NMI_B pin is enabled. NMI functionality is enabled (default)          */
#ifdef BOOT_NMI_DISABLE
  #define BOOT_NMI_EN    0
#else
  #define BOOT_NMI_EN    1
#endif

/** Execution Mode FTFA_FOPT[3]: **********************************************/
/*  0 - Execution Mode is RUN Mode (default)                                  */
/*  1 - VLPR execution mode enetered by startup                               */
#ifdef BOOT_SWVLPR_MODE
  #define BOOT_EXE_MODE  1
#else
  #define BOOT_EXE_MODE  0
#endif

/** Clock source used by Flash in RUN boot mode FTFA_FOPT[5]: *****************/
/*  0 - Externally supplied clock used by Flash                               */
/*  1 - Internal clock source used by Flash (default)                         */
#ifdef BOOT_EXT_CLK
  #define BOOT_CLK_SRC   0
#else
  #define BOOT_CLK_SRC   1
#endif

#define CONFIG_1  (long)0xffffffff
#define CONFIG_2  (long)0xffffffff
#define CONFIG_3  (long)0xffffffff
#define CONFIG_4  (long)((long)0xffffd2fc             |                       \
                        ((long)MCU_SECURITY   <<  0)  |                       \
                        ((long)BOOT_LPBOOT    <<  8)  |                       \
                        ((long)BOOT_NMI_EN    << 10)  |                       \
                        ((long)BOOT_EXE_MODE  << 11)  |                       \
                        ((long)BOOT_CLK_SRC   << 13))
#endif /* __SYSTEM_H */
