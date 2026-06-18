/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      llwu.h
 * @version   1.0.6.0
 * @date      Jun-11-2015
 * @brief     Low-Leakage Wakeup Unit (LLWU) driver header file.
 ******************************************************************************/
#ifndef __LLWU_H
#define __LLWU_H

#if !defined(MCU_MKM34Z7)
  #error "The llwu.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * Configuration structure definition                                         *
 ******************************************************************************/
typedef struct { uint8 PE1,PE2,PE3,PE4,PE5,PE6,ME,FILT1,FILT2; } tLLWU;

/******************************************************************************
 * module constant and macro definitions - not in documentation               *
 ******************************************************************************/
#define LLWU_ANYEDGE(ptx,pem,nem)   ((((ptx)&(pem))?1:0)|(((ptx)&(nem))?2:0))
#define LLWU_PERIPH(per,pem,shift)  (((per)&(pem))?(1<<shift):0)

/******************************************************************************
*//*! @addtogroup llwu_flags
* @{
*******************************************************************************/
#define LLWU_PTG6     (uint32)(1u <<  0)    ///< PTG6/LLWU_P0 pin
#define LLWU_PTG2     (uint32)(1u <<  1)    ///< PTG2/LLWU_P1 pin
#define LLWU_PTG1     (uint32)(1u <<  2)    ///< PTG1/LLWU_P2 pin
#define LLWU_PTF6     (uint32)(1u <<  3)    ///< PTF6/LLWU_P3 pin
#define LLWU_PTF0     (uint32)(1u <<  4)    ///< PTF0/LLWU_P4 pin
#define LLWU_PTE6     (uint32)(1u <<  5)    ///< PTE6/LLWU_P5 pin
#define LLWU_PTE5     (uint32)(1u <<  6)    ///< PTE5/LLWU_P6 pin
#define LLWU_PTD7     (uint32)(1u <<  7)    ///< PTD7/LLWU_P7 pin
#define LLWU_PTD6     (uint32)(1u <<  8)    ///< PTD6/LLWU_P8 pin
#define LLWU_PTD4     (uint32)(1u <<  9)    ///< PTD4/LLWU_P9 pin
#define LLWU_PTD2     (uint32)(1u << 10)    ///< PTD2/LLWU_P10 pin
#define LLWU_PTD0     (uint32)(1u << 11)    ///< PTD0/LLWU_P11 pin
#define LLWU_PTC5     (uint32)(1u << 12)    ///< PTC5/LLWU_P12 pin
#define LLWU_PTC3     (uint32)(1u << 13)    ///< PTC3/LLWU_P13 pin
#define LLWU_PTA6     (uint32)(1u << 14)    ///< PTA6/LLWU_P14 pin
#define LLWU_PTA4     (uint32)(1u << 15)    ///< PTA4/LLWU_P15 pin
#define LLWU_PTA0     (uint32)(1u << 16)    ///< PTA0/LLWU_P16 pin
#define LLWU_PTB1     (uint32)(1u << 17)    ///< PTB1/LLWU_P17 pin
#define LLWU_PTJ6     (uint32)(1u << 18)    ///< PTJ6/LLWU_P18 pin
#define LLWU_PTK3     (uint32)(1u << 19)    ///< PTK3/LLWU_P19 pin
#define LLWU_PTF3     (uint32)(1u << 20)    ///< PTF3/LLWU_P20 pin
#define LLWU_PTI0     (uint32)(1u << 21)    ///< PTI0/LLWU_P21 pin
#define LLWU_PTI2     (uint32)(1u << 22)    ///< PTI2/LLWU_P22 pin
#define LLWU_PTL5     (uint32)(1u << 23)    ///< PTL5/LLWU_P23 pin

#define LLWU_LPTMR    (uint32)(1u << 24)    ///< LPTMR0
#define LLWU_IRTC_ALM (uint32)(1u << 25)    ///< IRTC Alarm
#define LLWU_CMP0     (uint32)(1u << 26)    ///< CMP0
#define LLWU_IRTC_IRQ (uint32)(1u << 27)    ///< IRTC Interrupt
#define LLWU_CMP1     (uint32)(1u << 28)    ///< CMP1
#define LLWU_CMP2     (uint32)(1u << 29)    ///< CMP2

#define LLWU_FILT1F   (uint32)(1u << 30)    ///< LLWU_FILT1F
#define LLWU_FILT2F   (uint32)(1u << 31)    ///< LLWU_FILT2F
#define LLWU_ALL      (uint32)0xffffffff    ///< All sources
/*! @} End of llwu_flags                                                      */

/******************************************************************************
* LLWU default configuration used by LLWU_init() function
*
*//*! @addtogroup llwu_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Configures LLWU module with all wakeup sources disabled.
 * @details This structure configures LLWU module with all wakeup sources
 *          disabled. Use it as a template for making your specific
 *          configuration.
 * @showinitializer
 ******************************************************************************/
#define LLWU_MODULE_DISABLE_ALL_CONFIG                                        \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (0))|SET(LLWU_PE1_WUPE1 (0))  |                \
/* ..... */ SET(LLWU_PE1_WUPE2 (0))|SET(LLWU_PE1_WUPE3 (0)),                  \
/* PE2   */ SET(LLWU_PE2_WUPE4 (0))|SET(LLWU_PE2_WUPE5 (0))  |                \
/* ..... */ SET(LLWU_PE2_WUPE6 (0))|SET(LLWU_PE2_WUPE7 (0)),                  \
/* PE3   */ SET(LLWU_PE3_WUPE8 (0))|SET(LLWU_PE3_WUPE9 (0))  |                \
/* ..... */ SET(LLWU_PE3_WUPE10(0))|SET(LLWU_PE3_WUPE11(0)),                  \
/* PE4   */ SET(LLWU_PE4_WUPE12(0))|SET(LLWU_PE4_WUPE13(0))  |                \
/* ..... */ SET(LLWU_PE4_WUPE14(0))|SET(LLWU_PE4_WUPE15(0)),                  \
/* PE5   */ SET(LLWU_PE5_WUPE16(0))|SET(LLWU_PE5_WUPE17(0))  |                \
/* ..... */ SET(LLWU_PE5_WUPE18(0))|SET(LLWU_PE5_WUPE19(0)),                  \
/* PE6   */ SET(LLWU_PE6_WUPE20(0))|SET(LLWU_PE6_WUPE21(0))  |                \
/* ..... */ SET(LLWU_PE6_WUPE22(0))|SET(LLWU_PE6_WUPE23(0)),                  \
/* ME    */ CLR(LLWU_ME_WUME0_MASK)|CLR(LLWU_ME_WUME1_MASK)  |                \
/* ..... */ CLR(LLWU_ME_WUME2_MASK)|CLR(LLWU_ME_WUME3_MASK)  |                \
/* ..... */ CLR(LLWU_ME_WUME4_MASK)|CLR(LLWU_ME_WUME5_MASK),                  \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected posedge wakeup source(s)
 *          enabled.
 * @details This structure configures LLWU module with user selected posedge,
 *          wakeup source(s) enabled.
 * @param pe_mask   Select one or more OR'ed posedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 ******************************************************************************/
#define LLWU_MODULE_POSEDGE_ONLY_DETECT_CONFIG(pe_mask)                       \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,pe_mask,0))),          \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,pe_mask,0))),          \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,pe_mask,0))),          \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,pe_mask,0))),          \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,pe_mask,0))),          \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,pe_mask,0))),          \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,0,LLWU_ME_WUME0_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,0,LLWU_ME_WUME1_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,0,LLWU_ME_WUME2_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,0,LLWU_ME_WUME3_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,0,LLWU_ME_WUME4_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,0,LLWU_ME_WUME5_SHIFT)),            \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected negedge wakeup source(s)
 *          enabled.
 * @details This structure configures LLWU module with user selected negedge,
 *          wakeup source(s) enabled.
 * @param ne_mask   Select one or more OR'ed negedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 ******************************************************************************/
#define LLWU_MODULE_NEGEDGE_ONLY_DETECT_CONFIG(ne_mask)                       \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,0,ne_mask))),          \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,0,ne_mask))),          \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,0,ne_mask))),          \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,0,ne_mask))),          \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,0,ne_mask))),          \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,0,ne_mask))),          \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,0,LLWU_ME_WUME0_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,0,LLWU_ME_WUME1_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,0,LLWU_ME_WUME2_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,0,LLWU_ME_WUME3_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,0,LLWU_ME_WUME4_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,0,LLWU_ME_WUME5_SHIFT)),            \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected posedge, negedge and
 *          any edge wakeup source(s) enabled.
 * @details This structure configures LLWU module with user selected posedge,
 *          negedge and any edge wakeup source(s) enabled.
 * @param pe_mask   Select one or more OR'ed posedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 * @param ne_mask   Select one or more OR'ed negedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 * @note    In order to setup external wakeup on any edge list the same source
 *          in both pe_mask and ne_mask parameters.
 ******************************************************************************/
#define LLWU_MODULE_ANYEDGE_ONLY_DETECT_CONFIG(pe_mask,ne_mask)               \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,pe_mask,ne_mask))),    \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,pe_mask,ne_mask))),    \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,pe_mask,ne_mask))),    \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,pe_mask,ne_mask))),    \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,pe_mask,ne_mask))),    \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,pe_mask,ne_mask))),    \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,0,LLWU_ME_WUME0_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,0,LLWU_ME_WUME1_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,0,LLWU_ME_WUME2_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,0,LLWU_ME_WUME3_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,0,LLWU_ME_WUME4_SHIFT))|            \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,0,LLWU_ME_WUME5_SHIFT)),            \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected peripheral wakeup
 *          source(s) enabled.
 * @details This structure configures LLWU module with user selected user
 *          selected peripheral wakeup source(s) enabled.
 * @param per_mask  Select one or more OR'ed internal peripheral wakeup sources:
 *                  LLWU_LPTMR,LLWU_IRTC_ALM,LLWU_CMP0,LLWU_IRTC_IRQ,LLWU_CMP1,
 *                  LLWU_CMP2.
 ******************************************************************************/
#define LLWU_MODULE_PERIPH_ONLY_DETECT_CONFIG(per_mask)                       \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,0,0)))|                \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,0,0)))|                \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,0,0)))|                \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,0,0))),                \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,0,0)))|                \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,0,0)))|                \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,0,0)))|                \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,0,0))),                \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,0,0)))|                \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,0,0)))|                \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,0,0)))|                \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,0,0))),                \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,0,0)))|                \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,0,0)))|                \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,0,0)))|                \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,0,0))),                \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,0,0)))|                \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,0,0)))|                \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,0,0)))|                \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,0,0))),                \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,0,0)))|                \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,0,0)))|                \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,0,0)))|                \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,0,0))),                \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,per_mask,LLWU_ME_WUME0_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,per_mask,LLWU_ME_WUME1_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,per_mask,LLWU_ME_WUME2_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,per_mask,LLWU_ME_WUME3_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,per_mask,LLWU_ME_WUME4_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,per_mask,LLWU_ME_WUME5_SHIFT)),     \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected posedge and peripheral
 *          wakeup source(s) enabled.
 * @details This structure configures LLWU module with user selected posedge and
 *          peripheral wakeup source(s) enabled.
 * @param pe_mask   Select one or more OR'ed posedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 * @param per_mask  Select one or more OR'ed internal peripheral wakeup sources:
 *                  LLWU_LPTMR,LLWU_IRTC_ALM,LLWU_CMP0,LLWU_IRTC_IRQ,LLWU_CMP1,
 *                  LLWU_CMP2.
 ******************************************************************************/
#define LLWU_MODULE_POSEDGE_PERIPH_DETECT_CONFIG(pe_mask,per_mask)            \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,pe_mask,0))),          \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,pe_mask,0))),          \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,pe_mask,0))),          \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,pe_mask,0))),          \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,pe_mask,0))),          \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,pe_mask,0)))|          \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,pe_mask,0))),          \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,per_mask,LLWU_ME_WUME0_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,per_mask,LLWU_ME_WUME1_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,per_mask,LLWU_ME_WUME2_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,per_mask,LLWU_ME_WUME3_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,per_mask,LLWU_ME_WUME4_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,per_mask,LLWU_ME_WUME5_SHIFT)),     \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected negedge and peripheral
 *          wakeup source(s) enabled.
 * @details This structure configures LLWU module with user selected negedge and
 *          peripheral wakeup source(s) enabled.
 * @param ne_mask   Select one or more OR'ed negedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 * @param per_mask  Select one or more OR'ed internal peripheral wakeup sources:
 *                  LLWU_LPTMR,LLWU_IRTC_ALM,LLWU_CMP0,LLWU_IRTC_IRQ,LLWU_CMP1,
 *                  LLWU_CMP2.
 ******************************************************************************/
#define LLWU_MODULE_NEGEDGE_PERIPH_DETECT_CONFIG(ne_mask,per_mask)            \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,0,ne_mask))),          \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,0,ne_mask))),          \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,0,ne_mask))),          \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,0,ne_mask))),          \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,0,ne_mask))),          \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,0,ne_mask)))|          \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,0,ne_mask))),          \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,per_mask,LLWU_ME_WUME0_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,per_mask,LLWU_ME_WUME1_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,per_mask,LLWU_ME_WUME2_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,per_mask,LLWU_ME_WUME3_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,per_mask,LLWU_ME_WUME4_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,per_mask,LLWU_ME_WUME5_SHIFT)),     \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}

/***************************************************************************//*!
 * @brief   Configures LLWU module with user selected posedge, negedge, any edge
 *          and peripheral wakeup source(s) enabled.
 * @details This structure configures LLWU module with user selected posedge,
 *          negedge, any edge and peripheral wakeup source(s) enabled.
 * @param pe_mask   Select one or more OR'ed posedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 * @param ne_mask   Select one or more OR'ed negedge external wakeup sources:
 *                  LLWU_PTG6,LLWU_PTG2,LLWU_PTG1,LLWU_PTF6,LLWU_PTF0,LLWU_PTE6,
 *                  LLWU_PTE5,LLWU_PTD7,LLWU_PTD6,LLWU_PTD4,LLWU_PTD2,LLWU_PTD0,
 *                  LLWU_PTC5,LLWU_PTC3,LLWU_PTA6,LLWU_PTA4,LLWU_PTA0,LLWU_PTB1,
 *                  LLWU_PTJ6,LLWU_PTK3,LLWU_PTF3,LLWU_PTI0,LLWU_PTI2,LLWU_PTL5.
 * @param per_mask  Select one or more OR'ed internal peripheral wakeup sources:
 *                  LLWU_LPTMR,LLWU_IRTC_ALM,LLWU_CMP0,LLWU_IRTC_IRQ,LLWU_CMP1,
 *                  LLWU_CMP2.
 * @note    In order to setup external wakeup on any edge list the same source
 *          in both pe_mask and ne_mask parameters.
 ******************************************************************************/
#define LLWU_MODULE_ANYEDGE_PERIPH_DETECT_CONFIG(pe_mask,ne_mask,per_mask)    \
(tLLWU){                                                                      \
/* PE1   */ SET(LLWU_PE1_WUPE0 (LLWU_ANYEDGE(LLWU_PTG6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE1_WUPE1 (LLWU_ANYEDGE(LLWU_PTG2,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE1_WUPE2 (LLWU_ANYEDGE(LLWU_PTG1,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE1_WUPE3 (LLWU_ANYEDGE(LLWU_PTF6,pe_mask,ne_mask))),    \
/* PE2   */ SET(LLWU_PE2_WUPE4 (LLWU_ANYEDGE(LLWU_PTF0,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE2_WUPE5 (LLWU_ANYEDGE(LLWU_PTE6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE2_WUPE6 (LLWU_ANYEDGE(LLWU_PTE5,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE2_WUPE7 (LLWU_ANYEDGE(LLWU_PTD7,pe_mask,ne_mask))),    \
/* PE3   */ SET(LLWU_PE3_WUPE8 (LLWU_ANYEDGE(LLWU_PTD6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE3_WUPE9 (LLWU_ANYEDGE(LLWU_PTD4,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE3_WUPE10(LLWU_ANYEDGE(LLWU_PTD2,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE3_WUPE11(LLWU_ANYEDGE(LLWU_PTD0,pe_mask,ne_mask))),    \
/* PE4   */ SET(LLWU_PE4_WUPE12(LLWU_ANYEDGE(LLWU_PTC5,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE4_WUPE13(LLWU_ANYEDGE(LLWU_PTC3,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE4_WUPE14(LLWU_ANYEDGE(LLWU_PTA6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE4_WUPE15(LLWU_ANYEDGE(LLWU_PTA4,pe_mask,ne_mask))),    \
/* PE5   */ SET(LLWU_PE5_WUPE16(LLWU_ANYEDGE(LLWU_PTA0,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE5_WUPE17(LLWU_ANYEDGE(LLWU_PTB1,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE5_WUPE18(LLWU_ANYEDGE(LLWU_PTJ6,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE5_WUPE19(LLWU_ANYEDGE(LLWU_PTK3,pe_mask,ne_mask))),    \
/* PE6   */ SET(LLWU_PE6_WUPE20(LLWU_ANYEDGE(LLWU_PTF3,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE6_WUPE21(LLWU_ANYEDGE(LLWU_PTI0,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE6_WUPE22(LLWU_ANYEDGE(LLWU_PTI2,pe_mask,ne_mask)))|    \
/* ..... */ SET(LLWU_PE6_WUPE23(LLWU_ANYEDGE(LLWU_PTL5,pe_mask,ne_mask))),    \
/* ME    */ SET(LLWU_PERIPH(LLWU_LPTMR   ,per_mask,LLWU_ME_WUME0_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_ALM,per_mask,LLWU_ME_WUME1_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP0    ,per_mask,LLWU_ME_WUME2_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_IRTC_IRQ,per_mask,LLWU_ME_WUME3_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP1    ,per_mask,LLWU_ME_WUME4_SHIFT))|     \
/* ..... */ SET(LLWU_PERIPH(LLWU_CMP2    ,per_mask,LLWU_ME_WUME5_SHIFT)),     \
/* FILT1 */ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),              \
/* FILT2 */ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0))               \
}
/*! @} End of llwu_config                                                     */

/******************************************************************************
* LLWU callback registered by LLWU_Init() function
*
*//*! @addtogroup llwu_callback
* @{
*******************************************************************************/
/*! @brief LLWU_CALLBACK function declaration.
    @details This callback is called by the LLWU module upon recovery from VLLS
             modes. The service routine shall restore I/O and release latch
             states by the @ref PMC_SetACKISO macro.
    @param   flags  Mask formed of the @ref llwu_flags.
*/
typedef void (*LLWU_CALLBACK)(uint32 flags);
/*! @} End of llwu_callback                                                   */

/******************************************************************************
* LLWU function and macro definitions
*
*//*! @addtogroup llwu_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns flag(s) indicating which internal wakeup source caused
 *          the MCU to exit VLLS mode.
 * @details This macro returns flag(s) indicating which internal wakeup source
 *          caused the MCU to exit VLLS mode.
 * @return  @ref uint32 Mask formed of the @ref llwu_flags.
 * @note    Implemented as an inline macro. Flags associated with on-chip
 *          peripherals, external input pins, filtered and unfiltered, must be
 *          cleared by software prior to entry to VLLSx mode.
 * @see     @ref LLWU_ClrFlags()
 ******************************************************************************/
#if defined(__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
static inline uint32 LLWU_GetFlags (void)
{
  return (((uint32)(LLWU_FILT2&0x80)<<23) | ((uint32)(LLWU_FILT1&0x80)<<22) |
          ((uint32)(LLWU_MF5  &0x3f)<<24) | ((uint32) LLWU_PF3        <<16) |
          ((uint32) LLWU_PF2        << 8) |  (uint32) LLWU_PF1);
}
#if defined(__ICCARM__)
  #pragma diag_default=Pa082
#endif

/***************************************************************************//*!
 * @brief   Clears wakeup flag(s) indicated by an input mask.
 * @details This macro clears wakeup flag(s) indicated by an input mask.
 * @param   mask  Select one or more OR'ed @ref llwu_flags.
 * @note    Implemented as an inline macro.
 * @see     @ref LLWU_GetFlags
 ******************************************************************************/
#define LLWU_ClrFlags(mask)                                                   \
{                                                                             \
  LLWU_PF1    = (uint8)(((mask)>> 0)&0xff);                                   \
  LLWU_PF2    = (uint8)(((mask)>> 8)&0xff);                                   \
  LLWU_PF3    = (uint8)(((mask)>>16)&0xff);                                   \
  LLWU_FILT1 |= (uint8)(((mask)>>22)&0x80);                                   \
  LLWU_FILT2 |= (uint8)(((mask)>>23)&0x80);                                   \
}

/***************************************************************************//*!
 * @brief   LLWU initialization.
 * @details This function initializes LLWU module. Wakeup sources shall be
 *          configured by the user in configuration structure @ref llwu_config.
 *          Function also installs callback function for interrupt vector 28
 *          (LLWU).
 * @param   cfg       Select one of the @ref llwu_config.
 * @param   ip        Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref llwu_callback.
 * @note    Implemented as a function call.
 * @warning After enabling an external pin filter or changing the source pin,
 * wait at least 5 LPO clock cycles before entering VLLSx mode to allow the
 * filter to initialize.
 ******************************************************************************/
#define LLWU_Init(cfg,ip,callback)      LLWU_Init(cfg,ip,callback)
/*! @} End of llwu_macro                                                      */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void LLWU_Init (tLLWU llwu, uint8 ip, LLWU_CALLBACK pCallback);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void llwu_isr (void);

#endif /* __LLWU_H */
