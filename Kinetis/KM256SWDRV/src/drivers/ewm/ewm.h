/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      ewm.h
 * @version   1.0.0.0
 * @date      Jan-16-2015
 * @brief     External Watchdog Monitor (EWM) driver header file.
 ******************************************************************************/
#ifndef __EWM_H
#define __EWM_H

#if !defined(MCU_MKM34Z7)
  #error "The ewm.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * Configuration structure definitions                                        *
 ******************************************************************************/
typedef struct { uint8 CTRL, CMPL, CMPH, CLKCTRL, CLKPRESCALER; } tEWM;

/******************************************************************************
* Low power clock sources for running EWM counter used by the @ref EWM_Init
* function. This clock is used when LPO (SIM_OSC32K_SRC4) clock source is
* selected by the @ref SIM_SelOsc32kClk function.
*
*//*! @addtogroup ewm_clocks
* @{
*******************************************************************************/
#define LPO_CLK0      (uint8)(0)     ///< LPO
#define LPO_CLK1      (uint8)(1)     ///< MCGIRCLK
#define LPO_CLK2      (uint8)(2)     ///< OSC32KCLK
#define LPO_CLK3      (uint8)(3)     ///< ERCLK32K
/*! @} End of ewm_clocks                                                      */

/******************************************************************************
* EWM default module configurations used by EWM_init() function
*
*//*! @addtogroup ewm_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Selects EWM no inversed input.
 * @details The EWM input is enabled, no input assert inverted (default assert
 * state is 0).
 * @param clksrc  Select one of the @ref ewm_clocks.
 * @param presc   Clock source prescaler (0-256), prescaled clock frequency
 * is equal to frequency of the clksrc/(1+presc). The divided clock must be no
 * more than half of the bus clock frequency.
 * @param per_hi  Upper watchdog window timeout in number of clock cycles.
 * @param per_lo  Lower watchdog window timeout in number of clock cycles.
 * @showinitializer
 *****************************************************************************/
#define EWM_MODULE_IN_EN_CONFIG(clksrc,presc,per_lo,per_hi)                   \
(tEWM){                                                                       \
/* CTRL         */ SET(EWM_CTRL_INTEN_MASK)|SET(EWM_CTRL_INEN_MASK)|          \
/* ............ */ CLR(EWM_CTRL_ASSIN_MASK)|SET(EWM_CTRL_EWMEN_MASK),         \
/* CMPL         */ per_lo,                                                    \
/* CMPH         */ per_hi,                                                    \
/* CLKCTRL      */ clksrc,                                                    \
/* CLKPRESCALER */ presc,                                                     \
}

/***************************************************************************//*!
 * @brief Selects EWM inverted input.
 * @details The EWM input is enabled, input assert inversion (default assert
 * state is 1).
 * @param clksrc  Select one of the @ref ewm_clocks.
 * @param presc   Clock source prescaler (0-256), prescaled clock frequency
 * is equal to frequency of the clksrc/(1+presc). The divided clock must be no
 * more than half of the bus clock frequency.
 * @param per_hi  Upper watchdog window timeout in number of clock cycles.
 * @param per_lo  Lower watchdog window timeout in number of clock cycles.
 * @showinitializer
 *****************************************************************************/
#define EWM_MODULE_IN_EN_INVERT_CONFIG(clksrc,presc,per_lo,per_hi)            \
(tEWM){                                                                       \
/* CTRL         */ SET(EWM_CTRL_INTEN_MASK)|SET(EWM_CTRL_INEN_MASK)|          \
/* ............ */ SET(EWM_CTRL_ASSIN_MASK)|SET(EWM_CTRL_EWMEN_MASK),         \
/* CMPL         */ per_lo,                                                    \
/* CMPH         */ per_hi,                                                    \
/* CLKCTRL      */ clksrc,                                                    \
/* CLKPRESCALER */ presc,                                                     \
}

/***************************************************************************//*!
 * @brief Selects EWM input disabled.
 * @details The EWM input is disabled.
 * @param clksrc  Select one of the @ref ewm_clocks.
 * @param presc   Clock source prescaler (0-256), prescaled clock frequency
 * is equal to frequency of the clksrc/(1+presc). The divided clock must be no
 * more than half of the bus clock frequency.
 * @param per_hi  Upper watchdog window timeout in number of clock cycles.
 * @param per_lo  Lower watchdog window timeout in number of clock cycles.
 * @showinitializer
 *****************************************************************************/
#define EWM_MODULE_IN_DI_CONFIG(clksrc,presc,per_lo,per_hi)                   \
(tEWM){                                                                       \
/* CTRL         */ SET(EWM_CTRL_INTEN_MASK)|CLR(EWM_CTRL_INEN_MASK)|          \
/* ............ */ CLR(EWM_CTRL_ASSIN_MASK)|SET(EWM_CTRL_EWMEN_MASK),         \
/* CMPL         */ per_lo,                                                    \
/* CMPH         */ per_hi,                                                    \
/* CLKCTRL      */ clksrc,                                                    \
/* CLKPRESCALER */ presc,                                                     \
}
/*! @} End of ewm_config                                                      */

/******************************************************************************
* EWM callback registered by EWM_Init() function
*
*//*! @addtogroup ewm_callback
* @{
*******************************************************************************/
typedef void (*EWM_CALLBACK)(void);
/*! @} End of ewm_callback                                                    */

/******************************************************************************
* EWM function and macro definitions
*
*//*! @addtogroup ewm_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Refreshes the ewm counter.
 * @details This macro refreshes the EWM counter.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define EWM_WriteRefreshSeq()         {  EWM_SERV=0xB4; EWM_SERV=0x2C; }

/***************************************************************************//*!
 * @brief   External Watchdog Module initialization.
 * @details This function initializes External Watchdog Monitor.
 * @param   cfg  Select one of the following @ref ewm_config.
 * @param   ip        Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref ewm_callback.
 * @note    Implemented as a function call.
 * @warning Call this function only once after a CPU reset. Calling this
 * function more than once generates a bus transfer error.
*******************************************************************************/
#define EWM_Init(cfg,ip,callback)     EWM_Init(cfg,ip,callback)
/*! @} End of ewm_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void EWM_Init (tEWM cfg, uint8 ip, EWM_CALLBACK pCallback);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void ewm_isr (void);

#endif /* __EWM_H */
