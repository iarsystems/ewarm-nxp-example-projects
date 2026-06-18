/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      osc32k.h
 * @version   1.0.2.0
 * @date      Jun-14-2014
 * @brief     32KHz RTC oscillator (OSC32K) driver header file.
 ******************************************************************************/
#ifndef __OSC32K_H
#define __OSC32K_H

#if !defined(MCU_MKM34Z7)
  #error "The osc32k.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
* OSC32K load capacitors definitions used by OSC32K_SetLoadCaps() function
*
*//*! @addtogroup osc32k_load_caps
* @{
*******************************************************************************/
#define OSC32K_SC16P (uint8)0x10 ///< Add 16 pF capacitor to the oscillator load
#define OSC32K_SC8P  (uint8)0x08 ///< Add 08 pF capacitor to the oscillator load
#define OSC32K_SC4P  (uint8)0x04 ///< Add 04 pF capacitor to the oscillator load
#define OSC32K_SC2P  (uint8)0x02 ///< Add 02 pF capacitor to the oscillator load
#define OSC32K_SC0P  (uint8)0x00 ///< No internal load capacitor applied
/*! @} End of osc32k_load_caps                                                */

/******************************************************************************
*OSC32K function and macro definitions
*
*//*! @addtogroup osc32k_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Disable 32KHz RTC oscillator.
 * @details This macro disables 32KHz RTC oscillator.
 * @note    Provided VBAT is powered by battery (uninterruptible) the 32KHz RTC
 *          oscillator will be disabled across POR.
 *          Implemented as an inline macro.
 * @see     @ref SIM_IsOSC32KEnabled, @ref OSC32K_Enable
 ******************************************************************************/
#define OSC32K_Disable()                                                      \
{                                                                             \
  IRTC_LockUnlock ();                                                         \
  (*((volatile uint8*)0x40050020))|= 0x0001;                                  \
}

/***************************************************************************//*!
 * @brief   Enable RTC 32KHz oscillator (enabled by default).
 * @details This macro enables 32KHz RTC oscillator (enabled by default).
 * @note    Provided VBAT is powered by battery (uninterruptible) the 32KHz RTC
 *          oscillator will be enabled across POR.
 *          Implemented as an inline macro.
 * @see     @ref SIM_IsOSC32KEnabled, @ref OSC32K_Disable
 ******************************************************************************/
#define OSC32K_Enable()                                                       \
{                                                                             \
  IRTC_LockUnlock ();                                                         \
  (*((volatile uint8*)0x40050020))&=~0x0001;                                  \
}

/***************************************************************************//*!
 * @brief   Select 32KHz RTC oscillator load capacitors.
 * @details This macro selects load capacitors for operation with 32KHz RTC
 *          oscillator.
 * @param   caps  Select one or more OR'ed @ref osc32k_load_caps.
 * @note    Provided VBAT is powered by battery (uninterruptible) the selection
 *          32KHz RTC oscillator load capacitors will be active across POR.
 *          Implemented as an inline macro.
 ******************************************************************************/
#define OSC32K_SetLoadCaps(caps)                                              \
{                                                                             \
  register uint8 mask = OSC32K_SC16P|OSC32K_SC8P|OSC32K_SC4P|OSC32K_SC2P;     \
  IRTC_LockUnlock ();                                                         \
  (*((volatile uint8*)0x40050020))=((*((volatile uint8*)0x40050020))&~mask)|  \
                                   (caps);                                    \
}
/*! @} End of osc32k_macro                                                    */

#endif /* __OSC32K_H */
