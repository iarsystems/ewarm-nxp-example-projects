/******************************************************************************
 * (c) Copyright 2010-2016, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      arm_cm0.h
 * @version   1.0.4.2
 * @date      Mar-27-2016
 * @brief     Header file.
 ******************************************************************************/
#ifndef __ARM_CM0_H
#define __ARM_CM0_H

#if !defined(MCU_MKM34Z7)
  #error "The arm_cm0.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/*ARM Cortex M0 implementation for interrupt priority shift                   */
#define ARM_INTERRUPT_LEVEL_BITS    6

/******************************************************************************
* Priority level definition
*
*//*! @addtogroup cm0plus_prilvl
* @{
*******************************************************************************/
#define PRI_LVL0 (uint8)0x00    ///< Priority level 0 (highest priority)
#define PRI_LVL1 (uint8)0x01    ///< Priority level 1
#define PRI_LVL2 (uint8)0x02    ///< Priority level 2
#define PRI_LVL3 (uint8)0x03    ///< Priority level 3 (lowest priority)
/*! @} End of cm0plus_prilvl                                                  */

/******************************************************************************
* SVC callback registered by SVC_InstallCallback() function
*
*//*! @addtogroup svc_callback
* @{
*******************************************************************************/
/*! @brief SVC_CALLBACK function declaration                                  */
typedef void (*SVC_CALLBACK)(uint8 imm);
/*! @} End of svc_callback                                                    */

/***************************************************************************//*!
 * @brief   Configures the ARM system control register for STOP (deep sleep) mode.
 * @details This macro configures the ARM system control register for STOP
 *          (deep sleep) mode and then executes the WFI instruction to enter the
 *          mode.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define stop()      SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK; __asm volatile("WFI")

/***************************************************************************//*!
 * @brief   Configures the ARM system control register for WAIT (sleep) mode.
 * @details This macro configures the ARM system control register for WAIT (sleep)
 *          mode and then executes the WFI instruction to enter the mode.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define wait()      SCB_SCR &=~SCB_SCR_SLEEPDEEP_MASK; __asm volatile("WFI")

/***************************************************************************//*!
 * @brief   Reads CONTROL register.
 * @details This inline function reads CONTROL register.
 * @return  uint32   32-bit value of the CONTROL register.
 * @note    Implemented as an inline function.
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline uint32 GetCONTROL(void)
  {
    register uint32 value;
    __asm volatile("MRS %[Rd],CONTROL":[Rd]"=r"(value));
    return value;
  }
#elif defined(__CC_ARM)
  static inline uint32 GetCONTROL(void)
  {
    register uint32 value;
    __asm volatile("MRS value,CONTROL");
    return value;
  }
#endif

/***************************************************************************//*!
 * @brief   Writes CONTROL register.
 * @details This inline function writes <c>value</c> to the CONTROL register.
 * @param   value  32-bit value of the CONTROL register.
 * @note    Implemented as an inline function.
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline void SetCONTROL(register uint32 value)
  {
    __asm volatile("MSR CONTROL,%[Rs]":[Rs]"+r"(value));
  }
#elif defined(__CC_ARM)
  static inline void SetCONTROL(register uint32 value)
  {
    __asm volatile("MSR CONTROL,value");
  }
#endif

/******************************************************************************
* Cortex-M0+ function and macro definitions
*
*//*! @addtogroup cm0plus_macros
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   No operation.
 * @details This inline function performs no operation and is not guaranteed to
 *          be time consuming. The processor might remove it from the pipeline
 *          before it reaches the execution stage.
 * @note    Implemented as an inline function.
 ******************************************************************************/
#define NOP() __asm volatile("nop")

/***************************************************************************//*!
 * @brief   Returns state of Interrupt Program Status (IPSR) register.
 * @details This inline function returns state of Interrupt Program Status (IPSR)
 *          register.
 * @return  uint32   32-bit value of the IPSR register.
 * @note    Implemented as an inline function.
 * @see     @ref SetInterruptMask, @ref EnableInterrupts, @ref DisableInterrupts,
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, @ref GetIPSR,
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline uint32 GetIPSR (void)
  {
    register uint32 value;
    __asm volatile("MRS %[Rd],IPSR":[Rd]"=r"(value));
    return value;
  }
#elif defined(__CC_ARM)
  static inline uint32 GetIPSR (void)
  {
    register uint32 value;
    __asm volatile("MRS value,IPSR");
    return value;
  }
#endif

/***************************************************************************//*!
 * @brief   Returns state of global interrupt disable bit from Priority Mask
 *          Register (PRIMASK).
 * @details This inline function returns state of global interrupt disable bit
 *          from Priority Mask Register (PRIMASK).
 * @return  true (non-zero) all interrupts disabled,
 * @return  false           all interrupts enabled.
 * @note    Implemented as an inline function.
 * @see     @ref SetInterruptMask, @ref EnableInterrupts, @ref DisableInterrupts,
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, @ref GetIPSR,
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline uint32 GetInterruptMask (void)
  {
    register uint32 mask;
    __asm volatile("MRS %[Rd],PRIMASK":[Rd]"=r"(mask));
    return mask;
  }
#elif defined(__CC_ARM)
  static inline uint32 GetInterruptMask (void)
  {
    register uint32 mask;
    __asm volatile("MRS mask,PRIMASK");
    return mask;
  }
#endif

/***************************************************************************//*!
 * @brief   Controls state of global interrupt disable bit in Priority Mask
 *          Register (PRIMASK).
 * @details This inline function controls state of global interrupt disable bit
 *          in Priority Mask Register (PRIMASK).
 * @param   mask    TRUE (all interrupts disabled)\n
 *                  FALSE (all interrupts enabled).
 * @note    Implemented as an inline function.
 * @see     @ref GetInterruptMask, @ref EnableInterrupts, @ref DisableInterrupts,
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, @ref GetIPSR,
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline void SetInterruptMask (register uint32 mask)
  {
    __asm volatile("MSR PRIMASK,%[Rs]":[Rs]"+r"(mask));
  }
#elif defined(__CC_ARM)
  static inline void SetInterruptMask (register uint32 mask)
  {
    __asm volatile("MSR PRIMASK,mask");
  }
#endif

/***************************************************************************//*!
 * @brief   Enables all interrupts.
 * @details This macro enables all interrupts by clearing disable bit in Priority
 *          Mask Register (PRIMASK).
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref DisableInterrupts,
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, @ref GetIPSR,
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr
 ******************************************************************************/
#define EnableInterrupts()    __asm volatile("CPSIE i")

/***************************************************************************//*!
 * @brief   Disables all interrupts.
 * @details This macro disables all interrupts by setting disable bit in Priority
 *          Mask Register (PRIMASK).
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, @ref GetIPSR,
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr
 ******************************************************************************/
#define DisableInterrupts()   __asm volatile("CPSID i")

/***************************************************************************//*!
 * @brief   Executes system reset.
 * @details This macro executes system reset.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#if defined(__ICCARM__)
  #pragma diag_suppress=Pe111
#endif
#define SystemReset()         {                                               \
                                SCB_AIRCR = SCB_AIRCR_VECTKEY(0x5FA)|         \
                                            SCB_AIRCR_SYSRESETREQ_MASK;       \
                                while(1);                                     \
                              }

#if defined(__ICCARM__)
  #pragma diag_suppress=Pe175
#endif
#define IRQ(x)              ((x)-16)
#define IPR_INDEX(x)        (uint16)(IRQ(x)>>2)
#define IPR_SHIFT(x)        (uint16)(((IRQ(x)%4)<<3)+ARM_INTERRUPT_LEVEL_BITS)
/***************************************************************************//*!
 * @brief   Enables interrupt request and sets its priority.
 * @details This macro enables interrupt request and sets its priority.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0_SPI1,INT_PDB0,
 *                INT_PMC,INT_TMR0,INT_TMR1,INT_TMR2,INT_TMR3,INT_PIT0_PIT1,
 *                INT_LLWU,INT_FTFA,INT_CMP0_CMP1_CMP2,INT_LCD,INT_ADC0,INT_PTx,
 *                INT_RNGA,INT_UART0_UART1_UART2_UART3,INT_MMAU,INT_AFE_CH0,
 *                INT_AFE_CH1,INT_AFE_CH2,INT_AFE_CH3,INT_RTC,INT_I2C0_I2C1,
 *                INT_LPUART0,INT_MCG,INT_WDOG_EWM,INT_LPTMR,INT_XBAR.
 * @param   ip    Select one of the @ref cm0plus_prilvl.
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref DisableInterrupts, @ref NVIC_ClrIsr, @ref NVIC_CallIsr,
 *          @ref GetIPSR, @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr,
 *          @ref FAULT_SetIsr
 ******************************************************************************/
#if defined ( __CC_ARM)
  #pragma diag_suppress 111
#endif
#define NVIC_SetIsr(src,ip) {                                                 \
                              NVIC_ICPR_REG(NVIC_BASE_PTR) |=                 \
                              (uint32)(1u<<IRQ(src));                         \
                              NVIC_ISER_REG(NVIC_BASE_PTR) |=                 \
                              (uint32)(1u<<IRQ(src));                         \
                              NVIC_IP_REG(NVIC_BASE_PTR,IPR_INDEX(src)) |=    \
                              (uint32)((uint32)(ip)<<IPR_SHIFT(src));         \
                            }

/***************************************************************************//*!
 * @brief   Disables interrupt request.
 * @details This macro disables interrupt request.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0_SPI1,INT_PDB0,
 *                INT_PMC,INT_TMR0,INT_TMR1,INT_TMR2,INT_TMR3,INT_PIT0_PIT1,
 *                INT_LLWU,INT_FTFA,INT_CMP0_CMP1_CMP2,INT_LCD,INT_ADC0,INT_PTx,
 *                INT_RNGA,INT_UART0_UART1_UART2_UART3,INT_MMAU,INT_AFE_CH0,
 *                INT_AFE_CH1,INT_AFE_CH2,INT_AFE_CH3,INT_RTC,INT_I2C0_I2C1,
 *                INT_LPUART0,INT_MCG,INT_WDOG_EWM,INT_LPTMR,INT_XBAR.
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_CallIsr,
 *          @ref GetIPSR, @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr,
 *          @ref FAULT_SetIsr
 ******************************************************************************/
#define NVIC_ClrIsr(src)  NVIC_ICER_REG(NVIC_BASE_PTR)=(uint32)(1u<<IRQ(src))

/***************************************************************************//*!
 * @brief   Asserts interrupt request.
 * @details This macro asserts interrupt request.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0_SPI1,INT_PDB0,
 *                INT_PMC,INT_TMR0,INT_TMR1,INT_TMR2,INT_TMR3,INT_PIT0_PIT1,
 *                INT_LLWU,INT_FTFA,INT_CMP0_CMP1_CMP2,INT_LCD,INT_ADC0,INT_PTx,
 *                INT_RNGA,INT_UART0_UART1_UART2_UART3,INT_MMAU,INT_AFE_CH0,
 *                INT_AFE_CH1,INT_AFE_CH2,INT_AFE_CH3,INT_RTC,INT_I2C0_I2C1,
 *                INT_LPUART0,INT_MCG,INT_WDOG_EWM,INT_LPTMR,INT_XBAR.
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr,
 *          @ref GetIPSR, @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr,
 *          @ref FAULT_SetIsr
 ******************************************************************************/
#define NVIC_CallIsr(src) NVIC_ISPR_REG(NVIC_BASE_PTR)=(uint32)(1u<<IRQ(src))

/***************************************************************************//*!
 * @brief   Deasserts pending bits for interrupt request.
 * @details This macro deasserts pending bits for interrupt request.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0_SPI1,INT_PDB0,
 *                INT_PMC,INT_TMR0,INT_TMR1,INT_TMR2,INT_TMR3,INT_PIT0_PIT1,
 *                INT_LLWU,INT_FTFA,INT_CMP0_CMP1_CMP2,INT_LCD,INT_ADC0,INT_PTx,
 *                INT_RNGA,INT_UART0_UART1_UART2_UART3,INT_MMAU,INT_AFE_CH0,
 *                INT_AFE_CH1,INT_AFE_CH2,INT_AFE_CH3,INT_RTC,INT_I2C0_I2C1,
 *                INT_LPUART0,INT_MCG,INT_WDOG_EWM,INT_LPTMR,INT_XBAR.
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr,
 *          @ref GetIPSR, @ref NVIC_CallIsr, @ref NVIC_GetPendingIsr,
 *          @ref FAULT_SetIsr
 ******************************************************************************/
#define NVIC_ClrPendingIsr(src) {                                             \
                                  NVIC_ICPR_REG(NVIC_BASE_PTR) =              \
                                  (uint32)(1u<<IRQ(src));                     \
                                }

/***************************************************************************//*!
 * @brief   Returns pending interrupt state.
 * @details This macro reads and returns pending interrupt state.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0_SPI1,INT_PDB0,
 *                INT_PMC,INT_TMR0,INT_TMR1,INT_TMR2,INT_TMR3,INT_PIT0_PIT1,
 *                INT_LLWU,INT_FTFA,INT_CMP0_CMP1_CMP2,INT_LCD,INT_ADC0,INT_PTx,
 *                INT_RNGA,INT_UART0_UART1_UART2_UART3,INT_MMAU,INT_AFE_CH0,
 *                INT_AFE_CH1,INT_AFE_CH2,INT_AFE_CH3,INT_RTC,INT_I2C0_I2C1,
 *                INT_LPUART0,INT_MCG,INT_WDOG_EWM,INT_LPTMR,INT_XBAR.
 * @return  true (non-zero) interrupt is pending,
 * @return  false           interrupt is not pending.
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr,
 *          @ref GetIPSR, @ref NVIC_CallIsr, @ref NVIC_ClrPendingIsr,
 *          @ref FAULT_SetIsr
 ******************************************************************************/
#define NVIC_GetPendingIsr(src) (NVIC_ISPR&((uint32)(1u<<(IRQ(src))))>>(IRQ(src)))

/***************************************************************************//*!
 * @brief   Sets the Vector Table Offset Register (VTOR).
 * @details This macro sets the Vector Table Offset Register (VTOR).
 * @param   offset  32-bit vector table base offset value. This value must be a
 *                  multiple of 0x100.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define NVIC_SetVectorTable(offset)   SCB_VTOR=((uint32)offset)

/***************************************************************************//*!
 * @brief   Sets lower priority of the fault exceptions.
 * @details This macro sets lower priority of the selected fault exceptions.
 *          Fault exceptions have always the highest priority after reset.
 * @param   src   Select exception: INT_SVCall, INT_PendableSrvReq, INT_SysTick.
 * @param   ip    Select one of the @ref cm0plus_prilvl.
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts,
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr,
 *          @ref GetIPSR, @ref NVIC_CallIsr, @ref NVIC_ClrPendingIsr,
 *          @ref NVIC_GetPendingIsr
 ******************************************************************************/
#define FAULT_SetIsr(src,ip)                                                  \
{                                                                             \
 if (src==INT_SVCall        )                                                 \
   SCB_SHPR2|=(uint32)((ip)<<(24+ARM_INTERRUPT_LEVEL_BITS));                  \
 if (src==INT_PendableSrvReq)                                                 \
   SCB_SHPR3|=(uint32)((ip)<<(16+ARM_INTERRUPT_LEVEL_BITS));                  \
 if (src==INT_SysTick       )                                                 \
   SCB_SHPR3|=(uint32)((ip)<<(24+ARM_INTERRUPT_LEVEL_BITS));                  \
}

/***************************************************************************//*!
 * @brief   Returns current mode of execution.
 * @details This macro reads and returns current mode of execution.
 * @return  true (non-zero) unprivileged (user) mode of execution,
 * @return  false           privileged (supervisor) mode of execution.
 * @note    Implemented as an inline macro.
 * @see     @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#define GetMode()            (GetCONTROL() & 0x00000001l)

/***************************************************************************//*!
 * @brief   Enters unprivileged (user) mode of execution.
 * @details This macro enters unprivileged (user) mode of execution.
 *          It can be called from software running in privileged (supervisor) mode.
 * @note    Implemented as an inline macro.
 * @see     @ref GetMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#define UserMode()            SetCONTROL(GetCONTROL() |  0x00000001l)

/***************************************************************************//*!
 * @brief   Enters privileged (supervisor) mode of execution.
 * @details This macro enters privileged (supervisor) mode of execution.
 *          It can be called from software running in handler or privileged
 *          (supervisor) modes.
 * @note    Implemented as an inline macro.
 * @warning Calling it from unprivileged (user) mode of execution causes not
 *          entering into privileged (supervisor) mode of execution.
 *          Call @ref SVC_Call macro to switch from privileged to user mode.
 * @see     @ref GetMode, @ref UserMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#define SpvrMode()            SetCONTROL(GetCONTROL() & ~0x00000001l)

/***************************************************************************//*!
 * @brief   Supervisor Call (SVC).
 * @details This macro performs Supervisor Call (SVC). It passes to the service
 *          routine an identifier (8-bit immediate value). Service routine
 *          always executes in handler mode.
 * @param   imm   8-bit identifier.
 * @note    Implemented as an inline macro. If required, the 8-bit identifier
 *          passed by this inline function can be retrieved by callback function
 *          installed by @ref SVC_InstallCallback.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  #define SVC_Call(imm) __asm volatile("SVC %0"::"I"(imm))
#elif defined(__CC_ARM)
  #define SVC_Call(imm) __asm volatile("SVC " #imm)
#endif

/***************************************************************************//*!
 * @brief   Sets top of Process Stack Pointer (PSP).
 * @details This macro sets top of Process Stack Pointer (PSP).
 * @param   value 32-bit value of the top of Process Stack Pointer.
 * @note    Implemented as an inline function.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline void SetPSP (register uint32 value)
  {
    __asm volatile("MSR PSP,%[Rs]":[Rs]"+r"(value));
  }
#elif defined(__CC_ARM)
  static inline void SetPSP (register uint32 value)
  {
    __asm volatile("MSR PSP,value");
  }
#endif

/***************************************************************************//*!
 * @brief   Gets Process Stack Pointer (PSP).
 * @details This macro returns Process Stack Pointer (PSP).
 * @return  uint32   32-bit value of the PSP register.
 * @note    Implemented as an inline function.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline uint32 GetPSP (void)
  {
    register uint32 value;
    __asm volatile("MRS %[Rd],PSP":[Rd]"=r"(value));
    return value;
  }
#elif defined(__CC_ARM)
  static inline uint32 GetPSP (void)
  {
    register uint32 value;
    __asm volatile("MRS value,PSP");
    return value;
  }
#endif

/***************************************************************************//*!
 * @brief   Sets top of Main Stack Pointer (MSP).
 * @details This macro sets top of Main Stack Pointer (MSP).
 * @param   value 32-bit value of the top of Main Stack Pointer.
 * @note    Implemented as an inline function.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline void SetMSP (register uint32 value)
  {
    __asm volatile("MSR MSP,%[Rs]":[Rs]"+r"(value));
  }
#elif defined(__CC_ARM)
  static inline void SetMSP (register uint32 value)
  {
    __asm volatile("MSR MSP,value");
  }
#endif

/***************************************************************************//*!
 * @brief   Gets Main Stack Pointer (MSP).
 * @details This macro returns Main Stack Pointer (MSP).
 * @return  uint32   32-bit value of the MSP register.
 * @note    Implemented as an inline function.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#if defined(__ICCARM__) || defined(__GNUC__)
  static inline uint32 GetMSP (void)
  {
    register uint32 value;
    __asm volatile("MRS %[Rd],MSP":[Rd]"=r"(value));
    return value;
  }
#elif defined(__CC_ARM)
  static inline uint32 GetMSP (void)
  {
    register uint32 value;
    __asm volatile("MRS value,MSP");
    return value;
  }
#endif

/***************************************************************************//*!
 * @brief   Switches stack pointer to Process Stack Pointer (PSP).
 * @details This macro switches stack pointer to Process Stack Pointer (PSP).
 * @note    Implemented as an inline macro.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#define SelPSP()              SetCONTROL(GetCONTROL() |  0x00000002l)

/***************************************************************************//*!
 * @brief   Switches stack pointer back to Main Stack Pointer (MSP).
 * @details This macro switches stack pointer back to Main Stack Pointer (MSP).
 * @note    Implemented as an inline macro.
 * @see     @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP, @ref SetPSP,
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP,
 *          @ref SVC_InstallCallback
 ******************************************************************************/
#define SelMSP()              SetCONTROL(GetCONTROL() & ~0x00000002l)

/***************************************************************************//*!
 * @brief   Installs callback function for Supervisor Call (SVC) interrupt
 *          vector 11.
 * @details This function installs callback function for Supervisor Call (SVC)
 *          interrupt vector 11.
 * @param   ip    Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref svc_callback.
 * @note    Implemented as a function call.
 * @see     @ref GetMode, @ref UserMode, @ref SpvrMode, @ref SVC_Call, @ref SelPSP,
 *          @ref SelMSP, @ref SetPSP, @ref SetMSP, @ref GetPSP, @ref GetMSP
 ******************************************************************************/
#define SVC_InstallCallback(ip,callback)  SVC_InstallCallback(ip,callback)
/*! @} End of cm0plus_macros                                                  */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void main (void);
extern void SVC_InstallCallback (uint8 ip, SVC_CALLBACK pCallback);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void svcall_isr (void);

#endif  /* __ARM_CM0_H */
