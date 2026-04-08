
#ifndef __DEVICE_REGISTERS_H__
#define __DEVICE_REGISTERS_H__

/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */

#if defined(CPU_MCIMX7U5_A7)

#include "MCIMX7U5_A7.h"

#elif defined(CPU_MCIMX7U5_M4)

#include "MCIMX7U5_M4.h"

#define __NVIC_PRIO_BITS               4            /*!< Number of Bits used for Priority Levels                               */
#define __FPU_PRESENT                  1            /*!< FPU present or not                                                    */
#define __MPU_PRESENT                  1            /*!< MPU present or not                                                    */

typedef enum {
/* -------------------  Cortex-M4 Processor Exceptions Numbers  ------------------- */
  Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                 */
  NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted           */
  HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                 */
  MemoryManagement_IRQn         = -12,              /*!<   4  Memory Management, MPU mismatch, including Access Violation
                                                         and No Match                                                          */
  BusFault_IRQn                 = -11,              /*!<   5  Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory
                                                         related Fault                                                         */
  UsageFault_IRQn               = -10,              /*!<   6  Usage Fault, i.e. Undef Instruction, Illegal State Transition    */
  SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                          */
  DebugMonitor_IRQn             =  -4,              /*!<  12  Debug Monitor                                                    */
  PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                              */
  SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                */
/* --------------------  MCIMX7U_M4 Specific Interrupt Numbers  ------------------- */
  DMA0_0_4_IRQn                 =   1,              /*!<   1  DMA0_0_4                                                         */
  DMA0_1_5_IRQn                 =   2,              /*!<   2  DMA0_1_5                                                         */
  DMA0_2_6_IRQn                 =   3,              /*!<   3  DMA0_2_6                                                         */
  DMA0_3_7_IRQn                 =   4,              /*!<   4  DMA0_3_7                                                         */
  DMA0_8_12_IRQn                =   5,              /*!<   5  DMA0_8_12                                                        */
  DMA0_9_13_IRQn                =   6,              /*!<   6  DMA0_9_13                                                        */
  DMA0_10_14_IRQn               =   7,              /*!<   7  DMA0_10_14                                                       */
  DMA0_11_15_IRQn               =   8,              /*!<   8  DMA0_11_15                                                       */
  DMA0_16_20_IRQn               =   9,              /*!<   9  DMA0_16_20                                                       */
  DMA0_17_21_IRQn               =  10,              /*!<  10  DMA0_17_21                                                       */
  DMA0_18_22_IRQn               =  11,              /*!<  11  DMA0_18_22                                                       */
  DMA0_19_23_IRQn               =  12,              /*!<  12  DMA0_19_23                                                       */
  DMA0_24_28_IRQn               =  13,              /*!<  13  DMA0_24_28                                                       */
  DMA0_25_29_IRQn               =  14,              /*!<  14  DMA0_25_29                                                       */
  DMA0_26_30_IRQn               =  15,              /*!<  15  DMA0_26_30                                                       */
  DMA0_27_31_IRQn               =  16,              /*!<  16  DMA0_27_31                                                       */
  DMA0_Error_IRQn               =  17,              /*!<  17  DMA0_Error                                                       */
  MCM0_IRQn                     =  18,              /*!<  18  MCM0                                                             */
  EWM_IRQn                      =  19,              /*!<  19  EWM                                                              */
  LLWU0_IRQn                    =  20,              /*!<  20  LLWU0                                                            */
  SIM_IRQn                      =  21,              /*!<  21  SIM                                                              */
  MU_A_IRQn                     =  22,              /*!<  22  MU_A                                                             */
  WDOG0_IRQn                    =  26,              /*!<  26  WDOG0                                                            */
  SCG0_IRQn                     =  27,              /*!<  27  SCG0                                                             */
  QSPI_IRQn                     =  28,              /*!<  28  QSPI                                                             */
  SNVS_IRQn                     =  31,              /*!<  31  SNVS                                                             */
  TRNG0_IRQn                    =  32,              /*!<  32  TRNG0                                                            */
  LPIT0_IRQn                    =  33,              /*!<  33  LPIT0                                                            */
  PMC0_IRQn                     =  34,              /*!<  34  PMC0                                                             */
  CMC0_IRQn                     =  35,              /*!<  35  CMC0                                                             */
  LPTMR0_IRQn                   =  36,              /*!<  36  LPTMR0                                                           */
  LPTMR1_IRQn                   =  37,              /*!<  37  LPTMR1                                                           */
  TPM0_IRQn                     =  38,              /*!<  38  TPM0                                                             */
  TPM1_IRQn                     =  39,              /*!<  39  TPM1                                                             */
  TPM2_IRQn                     =  40,              /*!<  40  TPM2                                                             */
  TPM3_IRQn                     =  41,              /*!<  41  TPM3                                                             */
  FLEXIO0_IRQn                  =  42,              /*!<  42  FLEXIO0                                                          */
  LPI2C0_IRQn                   =  43,              /*!<  43  LPI2C0                                                           */
  LPI2C1_IRQn                   =  44,              /*!<  44  LPI2C1                                                           */
  LPI2C2_IRQn                   =  45,              /*!<  45  LPI2C2                                                           */
  LPI2C3_IRQn                   =  46,              /*!<  46  LPI2C3                                                           */
  I2S0_IRQn                     =  47,              /*!<  47  I2S0                                                             */
  I2S1_IRQn                     =  48,              /*!<  48  I2S1                                                             */
  LPSPI0_IRQn                   =  49,              /*!<  49  LPSPI0                                                           */
  LPSPI1_IRQn                   =  50,              /*!<  50  LPSPI1                                                           */
  LPUART0_IRQn                  =  51,              /*!<  51  LPUART0                                                          */
  LPUART1_IRQn                  =  52,              /*!<  52  LPUART1                                                          */
  LPUART2_IRQn                  =  53,              /*!<  53  LPUART2                                                          */
  LPUART3_IRQn                  =  54,              /*!<  54  LPUART3                                                          */
  PCTLA_IRQn                    =  56,              /*!<  56  PCTLA                                                            */
  PCTLB_IRQn                    =  57,              /*!<  57  PCTLB                                                            */
  ADC0_IRQn                     =  58,              /*!<  58  ADC0                                                             */
  ADC1_IRQn                     =  59,              /*!<  59  ADC1                                                             */
  CMP0_IRQn                     =  60,              /*!<  60  CMP0                                                             */
  CMP1_IRQn                     =  61,              /*!<  61  CMP1                                                             */
  DAC0_IRQn                     =  62,              /*!<  62  DAC0                                                             */
  DAC1_IRQn                     =  63,              /*!<  63  DAC1                                                             */
  WDOG1_IRQn                    =  64,              /*!<  64  WDOG1                                                            */
  USB0_IRQn                     =  65,              /*!<  65  USB0                                                             */
  USB1_IRQn                     =  66,              /*!<  66  USB1                                                             */
  WDOG2_IRQn                    =  68,              /*!<  68  WDOG2                                                            */
  USBPHY_IRQn                   =  69,              /*!<  69  USBPHY                                                           */
  CMC1_IRQn                     =  70               /*!<  70  CMC1                                                             */
} IRQn_Type;

#include "system_MCIMX7U5_M4.h"
#include "core_cm4.h"

#else

#error "No valid CPU defined!"

#endif

#endif /* __DEVICE_REGISTERS_H__ */
