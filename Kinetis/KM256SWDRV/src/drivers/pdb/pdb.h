/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      pdb.h
 * @version   1.0.2.0
 * @date      Apr-19-2015
 * @brief     Programmable Delay Block (PDB) driver header file.
 ******************************************************************************/
#ifndef __PDB_H
#define __PDB_H

#if !defined(MCU_MKM34Z7)
  #error "The pdb.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * Configuration structure definitions                                        *
 ******************************************************************************/
typedef struct { uint32 SC, MOD, IDLY;  } tPDB;
typedef struct { uint32 C1;   } tPDB_TRG;
typedef struct { uint32 POEN; } tPDB_PO;

/******************************************************************************
* PDB input trigger option selection
*
*//*! @addtogroup pdb_triggers
* @{
*******************************************************************************/
#define PDBTRG_EXTRG    (uint32)0x0  ///< External trigger pin input (PDB0_EXTRG)
#define PDBTRG_CMP0     (uint32)0x1  ///< comparator 0
#define PDBTRG_CMP1     (uint32)0x2  ///< comparator 1
#define PDBTRG_CMP2     (uint32)0x3  ///< comparator 2
#define PDBTRG_PIT0CH0  (uint32)0x4  ///< PIT0 channel 0 output
#define PDBTRG_PIT0CH1  (uint32)0x5  ///< PIT0 channel 1 output
#define PDBTRG_PIT1CH0  (uint32)0x6  ///< PIT1 channel 0 output
#define PDBTRG_PIT1CH1  (uint32)0x7  ///< PIT1 channel 1 output
#define PDBTRG_LPTMR    (uint32)0xB  ///< LPTMR output
#define PDBTRG_RTCALARM (uint32)0xC  ///< RTC alarm
#define PDBTRG_RTCOUT   (uint32)0xD  ///< RTC clock output
#define PDBTRG_XBAROUT  (uint32)0xE  ///< XBAR output 39
#define PDBTRG_SW       (uint32)0xF  ///< SW trigger
/*! @} End of pdb_triggers                                                    */

/******************************************************************************
* PDB load mode selection
*
*//*! @addtogroup pdb_ldmod_list
* @{
*******************************************************************************/
#define LDMOD_IMM       (uint32)0x0  ///< The internal registers are loaded from their buffers immediately.
#define LDMOD_ONMOD     (uint32)0x1  ///< The internal registers are loaded from their buffers when the PDB counter reaches the MOD register value.
#define LDMOD_ONTRG     (uint32)0x2  ///< The internal registers are loaded from their buffers when a trigger input event is detected.
#define LDMOD_ONBOTH    (uint32)0x3  ///< The internal registers are loaded from their buffers when either the PDB counter reaches the MOD register value or a trigger input event is detected.
/*! @} End of pdb_ldmod_list                                                  */

/******************************************************************************
* PDB prescaler divider definition
*
*//*! @addtogroup pdb_prescaler_list
* @{
*******************************************************************************/
#define PDBCLK_DIV1     (uint32)0x0  ///< peripheral clock divided by 1
#define PDBCLK_DIV2     (uint32)0x1  ///< peripheral clock divided by 2
#define PDBCLK_DIV4     (uint32)0x2  ///< peripheral clock divided by 4
#define PDBCLK_DIV8     (uint32)0x3  ///< peripheral clock divided by 8
#define PDBCLK_DIV16    (uint32)0x4  ///< peripheral clock divided by 16
#define PDBCLK_DIV32    (uint32)0x5  ///< peripheral clock divided by 32
#define PDBCLK_DIV64    (uint32)0x6  ///< peripheral clock divided by 64
#define PDBCLK_DIV128   (uint32)0x7  ///< peripheral clock divided by 128
/*! @} End of pdb_prescaler_list                                              */

/******************************************************************************
* PDB multiplication factor for prescaler definition
*
*//*! @addtogroup pdb_multfactor_list
* @{
*******************************************************************************/
#define PDB_MULT1       (uint32)0x0  ///< multiplication factor 1
#define PDB_MULT10      (uint32)0x1  ///< multiplication factor 10
#define PDB_MULT20      (uint32)0x2  ///< multiplication factor 20
#define PDB_MULT40      (uint32)0x3  ///< multiplication factor 40
/*! @} End of pdb_multfactor_list                                             */

/******************************************************************************
* PDB configurations used by PDB_init() function
*
*//*! @addtogroup pdb_module_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Default PDB configuration - PDB disabled.
 * @details Default PDB configuration - PDB disabled.
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_DEFAULT_CONFIG                                             \
(tPDB){                                                                       \
/* SC   */   SET(PDB_SC_LDMOD(0))|CLR(PDB_SC_PDBEIE_MASK)|                    \
/* .... */   CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                  \
/* .... */   SET(PDB_SC_PRESCALER(0))|SET(PDB_SC_TRGSEL(0))|                  \
/* .... */   CLR(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                   \
/* .... */   SET(PDB_SC_MULT(0))|CLR(PDB_SC_CONT_MASK)|CLR(PDB_SC_LDOK_MASK), \
/* MOD  */   0l,                                                              \
/* IDLY */   0l,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with SW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, SW trigger is enabled, interrupt is disabled, DMA is disabled,
 * PDB is enabled, one-shot mode is configured.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_SWTRG_ONESHOT_CONFIG(presc,mult,modulo,idly)               \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(15))|               \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|CLR(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with SW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, SW trigger is enabled, interrupt is disabled, DMA is disabled,
 * PDB is enabled, continuous mode is configured.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_SWTRG_CONT_CONFIG(presc,mult,modulo,idly)                  \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(15))|               \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|SET(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with SW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, SW trigger is enabled, interrupt is enabled, DMA is disabled,
 * PDB is enabled, one-shot mode is configured.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_SWTRG_ONESHOT_IRQ_CONFIG(presc,mult,modulo,idly)           \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(15))|               \
/* .... */ SET(PDB_SC_PDBEN_MASK)|SET(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|CLR(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with SW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, SW trigger is enabled, interrupt is enabled, DMA is disabled,
 * PDB is enabled, continuous mode is configured.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_SWTRG_CONT_IRQ_CONFIG(presc,mult,modulo,idly)              \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(15))|               \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|SET(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with SW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, SW trigger is enabled, interrupt is disabled, DMA is enabled,
 * PDB is enabled, one-shot mode is configured.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_SWTRG_ONESHOT_DMA_CONFIG(presc,mult,modulo,idly)           \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|SET(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(15))|               \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|CLR(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with SW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, SW trigger is enabled, interrupt is disabled, DMA is enabled,
 * PDB is enabled, continuous mode is configured.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_SWTRG_CONT_DMA_CONFIG(presc,mult,modulo,idly)              \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|SET(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(15))|               \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|SET(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with HW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, HW trigger is enabled, interrupt is disabled, DMA is disabled,
 * PDB is enabled, one-shot mode is configured.
 * @param trgs    Select one of the @ref pdb_triggers.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_HWTRG_ONESHOT_CONFIG(trgs,presc,mult,modulo,idly)          \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(trgs))|             \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|CLR(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with HW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, HW trigger is enabled, interrupt is disabled, DMA is disabled,
 * PDB is enabled, continuous mode is configured.
 * @param trgs    Select one of the @ref pdb_triggers.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_HWTRG_CONT_CONFIG(trgs,presc,mult,modulo,idly)             \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(trgs))|             \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|SET(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with HW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, HW trigger is enabled, interrupt is enabled, DMA is disabled,
 * PDB is enabled, one-shot mode is configured.
 * @param trgs    Select one of the @ref pdb_triggers.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_HWTRG_ONESHOT_IRQ_CONFIG(trgs,presc,mult,modulo,idly)      \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(trgs))|             \
/* .... */ SET(PDB_SC_PDBEN_MASK)|SET(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|CLR(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with HW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, HW trigger is enabled, interrupt is enabled, DMA is disabled,
 * PDB is enabled, continuous mode is configured.
 * @param trgs    Select one of the @ref pdb_triggers.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_HWTRG_CONT_IRQ_CONFIG(trgs,presc,mult,modulo,idly)         \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|CLR(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(trgs))|             \
/* .... */ SET(PDB_SC_PDBEN_MASK)|SET(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|SET(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with HW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, HW trigger is enabled, interrupt is disabled, DMA is enabled,
 * PDB is enabled, one-shot mode is configured.
 * @param trgs    Select one of the @ref pdb_triggers.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_HWTRG_ONESHOT_DMA_CONFIG(trgs,presc,mult,modulo,idly)      \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|SET(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(trgs))|             \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|CLR(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}

/***************************************************************************//*!
 * @brief PBD init with HW trigger.
 * @details  Delay registers are updated when the counter reaches the MOD
 * register value, HW trigger is enabled, interrupt is disabled, DMA is enabled,
 * PDB is enabled, continuous mode is configured.
 * @param trgs    Select one of the @ref pdb_triggers.
 * @param presc   Select one of the @ref pdb_prescaler_list.
 * @param mult    Select one of the @ref pdb_multfactor_list.
 * @param modulo  PDB modulus (0 - 65535).
 * @param idly    PDB interrupt delay (0 - 65535).
 * @showinitializer
 ******************************************************************************/
#define PDB_MODULE_HWTRG_CONT_DMA_CONFIG(trgs,presc,mult,modulo,idly)         \
(tPDB){                                                                       \
/* SC   */ SET(PDB_SC_LDMOD(1))|CLR(PDB_SC_PDBEIE_MASK)|                      \
/* .... */ CLR(PDB_SC_SWTRIG_MASK)|SET(PDB_SC_DMAEN_MASK)|                    \
/* .... */ SET(PDB_SC_PRESCALER(presc))|SET(PDB_SC_TRGSEL(trgs))|             \
/* .... */ SET(PDB_SC_PDBEN_MASK)|CLR(PDB_SC_PDBIE_MASK)|                     \
/* .... */ SET(PDB_SC_MULT(mult))|SET(PDB_SC_CONT_MASK)|SET(PDB_SC_LDOK_MASK),\
/* MOD  */ modulo,                                                            \
/* IDLY */ idly,                                                              \
}
/*! @} End of pdb_module_config                                              */

/******************************************************************************
* PDB channel + pretrigger configurations used by PDB_PtrgInit() function
* for ADC module)
*
*//*! @addtogroup pdb_ptrg_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Default PDB channel/pre-trigger configuration.
 * @details This structure sets default PDB channel/pre-trigger configuration.
 * @showinitializer
 ******************************************************************************/
#define PDB_PTRG_DEFAULT_CONFIG                                               \
(tPDB_TRG){                                                                   \
/* C1 */CLR(PDB_C1_BB_MASK)|CLR(PDB_C1_TOS_MASK)|CLR(PDB_C1_EN_MASK)          \
}

/***************************************************************************//*!
 * @brief   Back to back mode with delay enabled PDB channel configuration.
 * @details This structure enabled back to back mode with delay.
 * @showinitializer
 ******************************************************************************/
#define PDB_PTRG_ON_B2BDELAYED_CONFIG                                         \
(tPDB_TRG){                                                                   \
/* C1 */SET(PDB_C1_BB_MASK)|SET(PDB_C1_TOS_MASK)|SET(PDB_C1_EN_MASK)          \
}

/***************************************************************************//*!
 * @brief   Back to back mode without delay (bypassed) PDB channel configuration.
 * @details This structure enables back to back mode without delay (bypassed).
 * @showinitializer
 ******************************************************************************/
#define PDB_PTRG_ON_B2BBYPASSED_CONFIG                                        \
(tPDB_TRG){                                                                   \
/* C1 */SET(PDB_C1_BB_MASK)|CLR(PDB_C1_TOS_MASK)|SET(PDB_C1_EN_MASK)          \
}

/***************************************************************************//*!
 * @brief   Delay PDB channel configuration.
 * @details This structure sets delayed PDB channel configuration.
 * @showinitializer
 ******************************************************************************/
#define PDB_PTRG_ON_DELAYED_CONFIG                                            \
(tPDB_TRG){                                                                   \
/* C1 */CLR(PDB_C1_BB_MASK)|SET(PDB_C1_TOS_MASK)|SET(PDB_C1_EN_MASK)          \
}

/***************************************************************************//*!
 * @brief   Bypassed (without delay) PDB channel configuration.
 * @details This structure enables bypassed (without delay) PDB channel
 * configuration.
 * @showinitializer
 ******************************************************************************/
#define PDB_PTRG_ON_BYPASSED_CONFIG                                           \
(tPDB_TRG){                                                                   \
/* C1 */CLR(PDB_C1_BB_MASK)|CLR(PDB_C1_TOS_MASK)|SET(PDB_C1_EN_MASK)          \
}
/*! @} End of pdb_ptrg_config                                                 */

/******************************************************************************
* PDB pulse-out configurations used by PDB_POUT_Init() function.
*
*//*! @addtogroup pdb_pout_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   PDB pulse-out disabled configuration.
 * @details This structure disables PDB pulse-out function.
 * @showinitializer
 ******************************************************************************/
#define PDB_POUT_DISABLE_CONFIG                                               \
(tPDB_PO){                                                                    \
/* POEN */CLR(PDB_POEN_POEN_MASK),                                            \
}

/***************************************************************************//*!
 * @brief   PDB pulse-out enable configuration.
 * @details This structure enables PDB pulse-out function.
 * @showinitializer
 ******************************************************************************/
#define PDB_POUT_ENABLE_CONFIG                                                \
(tPDB_PO){                                                                    \
/* POEN */SET(PDB_POEN_POEN_MASK),                                            \
}
/*! @} End of pdb_pout_config                                                 */

/******************************************************************************
* PDB callback registered by PDB_Init() function
*
*//*! @addtogroup pdb_callback
* @{
*******************************************************************************/
/*! @brief PDB_CALLBACK type declaration                                      */
typedef enum
{
  PDBDLY_CALLBACK    = 1, ///< PDB interrupt
  CH0SEQERR_CALLBACK = 2, ///< PDB channel 0 sequence error interrupt
} PDB_CALLBACK_TYPE;

/*! PDB_CALLBACK function declaration                                         */
typedef void (*PDB_CALLBACK)(PDB_CALLBACK_TYPE type);
/*! @} End of pdb_callback                                                    */

/******************************************************************************
* PDB function and macro definitions
*
*//*! @addtogroup pdb_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Sets the load mode for internal registers.
 * @details This macro sets the load mode for internal registers after @ref
 *          PDB_SetLDOK macro is called.
 * @param   mode    Select one of the @ref pdb_ldmod_list.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_SetLDOK
 ******************************************************************************/
#define PDB_SetLDMode(mode)       iobfiw(PDB0_SC,PDB_SC_LDMOD_SHIFT,2,(mode))

/***************************************************************************//*!
 * @brief   Enables the sequence error interrupt generation.
 * @details This macro enables the sequence error interrupt. The sequence error
 *          occurs when a new pre-trigger asserts when there is a active lock in
 *          the PDB channel.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_ErrIsrDisable
 ******************************************************************************/
#define PDB_ErrIsrEnable()        iosetw(PDB0_SC,PDB_SC_PDBEIE_MASK)

/***************************************************************************//*!
 * @brief   Enables sequence error interrupt generation.
 * @details This macro disables the sequence error interrupt. The sequence error
 *          occurs when a new pre-trigger asserts when there is a active lock in
 *          the PDB channel.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_ErrIsrEnable
 ******************************************************************************/
#define PDB_ErrIsrDisable()       ioclrw(PDB0_SC,PDB_SC_PDBEIE_MASK)

/***************************************************************************//*!
 * @brief   Generates SW trigger if SW trigger is selected and PDB
 *          is enabled.
 * @details This macro generates software trigger if the @ref PDBTRG_SW is
 *          selected as the PDB trigger source.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PtrgInit
 ******************************************************************************/
#define PDB_SWTrg()               iosetw(PDB0_SC,PDB_SC_SWTRIG_MASK)

/***************************************************************************//*!
 * @brief   Enables DMA request generation.
 * @details This macro enables the DMA request generation when the PDB counter
            reaches the value set by @ref PDB_SetIdelay.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_SetIdelay, @ref PDB_DmaDisable
 ******************************************************************************/
#define PDB_DmaEnable()           iosetw(PDB0_SC,PDB_SC_DMAEN_MASK)

/***************************************************************************//*!
 * @brief   Disables DMA request generation.
 * @details This macro disables the DMA request generation when the PDB counter
            reaches the value set by @ref PDB_SetIdelay.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_DmaEnable
 ******************************************************************************/
#define PDB_DmaDisable()          ioclrw(PDB0_SC,PDB_SC_DMAEN_MASK)

/***************************************************************************//*!
 * @brief   Enables the PDB module.
 * @details This macro enables the PDB module.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_Disable
 ******************************************************************************/
#define PDB_Enable()              iosetw(PDB0_SC,PDB_SC_PDBEN_MASK)

/***************************************************************************//*!
 * @brief   Disables the PDB module.
 * @details This macro disables the PDB module. When the PDB is disabled the
 *          counter is off.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_Enable
 ******************************************************************************/
#define PDB_Disable()             ioclrw(PDB0_SC,PDB_SC_PDBEN_MASK)

/***************************************************************************//*!
 * @brief   Enables interrupt generation.
 * @details This macro enables an interrupt generation when the PDB counter
            reaches the value set by @ref PDB_SetIdelay.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_SetIdelay, @ref PDB_IsrDisable
 ******************************************************************************/
#define PDB_IsrEnable()           iosetw(PDB0_SC,PDB_SC_PDBIE_MASK)

/***************************************************************************//*!
 * @brief   Disables interrupt generation.
 * @details This macro disables an interrupt generation when the PDB counter
            reaches the value set by @ref PDB_SetIdelay.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_IsrEnable
 ******************************************************************************/
#define PDB_IsrDisable()          ioclrw(PDB0_SC,PDB_SC_PDBIE_MASK)

/***************************************************************************//*!
 * @brief   Enables one shot mode of PDB.
 * @details This macro enables the one shot mode of the PDB. When the PDB
            counter reaches value set by PDB_SetModulo the Modulo register is
 *          cleared to zero and is waiting to a new trigger.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_ContModeEnable
 ******************************************************************************/
#define PDB_OneshotModeEnable()   ioclrw(PDB0_SC,PDB_SC_CONT_MASK)

/***************************************************************************//*!
 * @brief   Enables continuous mode of PDB.
 * @details This macro enables the continuous mode of the PDB. When the PDB
            counter reaches value set by PDB_SetModulo the counter begins
 *          a new counting.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_OneshotModeEnable
 ******************************************************************************/
#define PDB_ContModeEnable()      iosetw(PDB0_SC,PDB_SC_CONT_MASK)

/***************************************************************************//*!
 * @brief   Enables the loading buffered register by theirs buffered value
 *          according to LDMOD selection.
 * @details This macro updates internal registers of MOD, IDLY, CHnDLYm, POyDLY
 *          with the values written to their buffers.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define PDB_SetLDOK()             iosetw(PDB0_SC,PDB_SC_LDOK_MASK)

/***************************************************************************//*!
 * @brief   Sets the modulo register.
 * @details This macro specifies the period of the counter.
 * @param   modulo 16-bit modulo value.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_OneshotModeEnable, @ref PDB_ContModeEnable
 ******************************************************************************/
#define PDB_SetModulo(modulo)     iobfiw(PDB0_MOD,PDB_MOD_MOD_SHIFT,16,(modulo))

/***************************************************************************//*!
 * @brief   Sets the delay register.
 * @details This macro specifies the delay value to schedule the PDB interrupt
 *          or the DMA request.
 * @param   val    16-bit delay trigger value.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_DmaEnable, @ref PDB_IsrEnable
 ******************************************************************************/
#define PDB_SetIdelay(val)        iobfiw(PDB0_IDLY,PDB_IDLY_IDLY_SHIFT,16,(val))

/***************************************************************************//*!
 * @brief   Enables the pulse output.
 * @details This macro enables the pulse output.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PoutDisable, @ref PDB_PoutSetDelay
 ******************************************************************************/
#define PDB_PoutEnable()          iobfiw(PDB0_POEN,PDB_POEN_POEN_SHIFT,1,1)

/***************************************************************************//*!
 * @brief   Disables specified the pulse output.
 * @details This macro disables the pulse output.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PoutEnable, @ref PDB_PoutSetDelay
 ******************************************************************************/
#define PDB_PoutDisable()         iobfiw(PDB0_POEN,PDB_POEN_POEN_SHIFT,1,0)

#define PDBCH0TRG0_B2BModeEnable()      iobfiw(PDB0_CH0C1,16,1,1)
#define PDBCH0TRG1_B2BModeEnable()      iobfiw(PDB0_CH0C1,17,1,1)
#define PDBCH0TRG2_B2BModeEnable()      iobfiw(PDB0_CH0C1,18,1,1)
#define PDBCH0TRG3_B2BModeEnable()      iobfiw(PDB0_CH0C1,19,1,1)
/***************************************************************************//*!
 * @brief   Enables back-to-back mode for specified PDB channel pre-trigger.
 * @details This macro enables the back-to-back mode for specified PDB channel
 *          pre-trigger. If this mode is enabled the ADC conversion complete or
 *          XBAR_OUT35 (for pre-trigger 0), XBAR_OUT36 (for pre-trigger 1),
 *          XBAR_OUT37 (for pre-trigger 2), XBAR_OUT38 (for pre-trigger 3) is
 *          used as an acknowledgement for the next trigger. Which source is
 *          used can be selected with the @ref SIM_SelAdcTrg function.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @note    Implemented as an inline macro.
 * @see     @ref SIM_SelAdcTrg, @ref PDB_PtrgB2BModeDisable
 ******************************************************************************/
#define PDB_PtrgB2BModeEnable(trg)      PDBCH0##trg##_B2BModeEnable()

#define PDBCH0TRG0_B2BModeDisable()     iobfiw(PDB0_CH0C1,16,1,0)
#define PDBCH0TRG1_B2BModeDisable()     iobfiw(PDB0_CH0C1,17,1,0)
#define PDBCH0TRG2_B2BModeDisable()     iobfiw(PDB0_CH0C1,18,1,0)
#define PDBCH0TRG3_B2BModeDisable()     iobfiw(PDB0_CH0C1,19,1,0)
/***************************************************************************//*!
 * @brief   Disables back-to-back mode for specified PDB channel pre-trigger.
 * @details This macro disables the back-to-back mode for specified PDB channel
 *          pre-trigger.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @note    Implemented as an inline macro.
 * @see     @ref SIM_SelAdcTrg, @ref PDB_PtrgB2BModeEnable
 ******************************************************************************/
#define PDB_PtrgB2BModeDisable(trg)     PDBCH0##trg##_B2BModeDisable()

#define PDBCH0TRG0_BypassModeEnable()   iobfiw(PDB0_CH0C1,8,1,0)
#define PDBCH0TRG1_BypassModeEnable()   iobfiw(PDB0_CH0C1,9,1,0)
#define PDBCH0TRG2_BypassModeEnable()   iobfiw(PDB0_CH0C1,10,1,0)
#define PDBCH0TRG3_BypassModeEnable()   iobfiw(PDB0_CH0C1,11,1,0)
/***************************************************************************//*!
 * @brief   Enables bypass mode for specified PDB channel pre-trigger.
 * @details This macro enables the bypass mode for a specified PDB channel
 *          pre-trigger. When the bypass mode is selected the pre-trigger asserts
 *          one peripheral clock cycle after a rising edge is detected on the
 *          selected trigger input source or software trigger is selected and
 *          @ref PDB_SWTrg is called.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PtrgBypassModeDisable
 ******************************************************************************/
#define PDB_PtrgBypassModeEnable(trg)   PDBCH0##trg##_BypassModeEnable()

#define PDBCH0TRG0_BypassModeDisable()  iobfiw(PDB0_CH0C1,8,1,1)
#define PDBCH0TRG1_BypassModeDisable()  iobfiw(PDB0_CH0C1,9,1,1)
#define PDBCH0TRG2_BypassModeDisable()  iobfiw(PDB0_CH0C1,10,1,1)
#define PDBCH0TRG3_BypassModeDisable()  iobfiw(PDB0_CH0C1,11,1,1)
/***************************************************************************//*!
 * @brief   Disables bypass mode for specified PDB channel pre-trigger.
 * @details This macro disables the bypass mode for a specified PDB channel
 *          pre-trigger. When the bypass mode is disabled the pre-trigger asserts
 *          when the counter reaches the channel delay register plus one
 *          peripheral clock cycle after a rising edge is detected on the
 *          selected trigger input source or software trigger is selected and
 *          @ref PDB_SWTrg is called.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PtrgBypassModeEnable
 ******************************************************************************/
#define PDB_PtrgBypassModeDisable(trg)  PDBCH0##trg##_BypassModeDisable()

#define PDBCH0TRG0_Enable()         iobfiw(PDB0_CH0C1,0,1,1)
#define PDBCH0TRG1_Enable()         iobfiw(PDB0_CH0C1,1,1,1)
#define PDBCH0TRG2_Enable()         iobfiw(PDB0_CH0C1,2,1,1)
#define PDBCH0TRG3_Enable()         iobfiw(PDB0_CH0C1,3,1,1)
/***************************************************************************//*!
 * @brief   Enables specified channel pre-trigger.
 * @details This macro enables specified pre-trigger.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PtrgDisable, @ref PDB_PtrgInit
 ******************************************************************************/
#define PDB_PtrgEnable(trg)         PDBCH0##trg##_Enable()

#define PDBCH0TRG0_Disable()        iobfiw(PDB0_CH0C1,0,1,0)
#define PDBCH0TRG1_Disable()        iobfiw(PDB0_CH0C1,1,1,0)
#define PDBCH0TRG2_Disable()        iobfiw(PDB0_CH0C1,2,1,0)
#define PDBCH0TRG3_Disable()        iobfiw(PDB0_CH0C1,3,1,0)
/***************************************************************************//*!
 * @brief   Disables specified channel pre-trigger.
 * @details This macro disables specified pre-trigger.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PtrgEnable, @ref PDB_PtrgSetDelay, @ref PDB_PtrgInit
 ******************************************************************************/
#define PDB_PtrgDisable(trg)        PDBCH0##trg##_Disable()

#define PDBCH0TRG0_SetDelay(val)    iobfiw(PDB0_CH0DLY0,PDB_DLY_DLY_SHIFT,16,(val))
#define PDBCH0TRG1_SetDelay(val)    iobfiw(PDB0_CH0DLY1,PDB_DLY_DLY_SHIFT,16,(val))
#define PDBCH0TRG2_SetDelay(val)    iobfiw(PDB0_CH0DLY2,PDB_DLY_DLY_SHIFT,16,(val))
#define PDBCH0TRG3_SetDelay(val)    iobfiw(PDB0_CH0DLY3,PDB_DLY_DLY_SHIFT,16,(val))
/***************************************************************************//*!
 * @brief   Sets trigger delay value.
 * @details This macro sets trigger delay value. The pre-trigger asserts when
 * the counter is equal to this value.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @param   val    16-bit delay value.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PtrgEnable, @ref PDB_PtrgDisable, @ref PDB_PtrgInit
 ******************************************************************************/
#define PDB_PtrgSetDelay(trg,val)   PDBCH0##trg##_SetDelay(val)

#define PDBPOUT0DLY1_SetDelay(val)  iobfiw(PDB0_PO0DLY,PDB_PODLY_DLY1_SHIFT,16,(val))
#define PDBPOUT0DLY2_SetDelay(val)  iobfiw(PDB0_PO0DLY,PDB_PODLY_DLY2_SHIFT,16,(val))
/***************************************************************************//*!
 * @brief   Sets specified pulseout delay value.
 * @details This macro sets specified pulseout delay value. Pulse-out goes high
 *          when the PDB counter is equal to the DLY1 value and goes low when
 *          is equal to the DLY2 value.
 * @param   dly    DLY1,DLY2.
 * @param   val    16-bit delay value.
 * @note    Implemented as an inline macro.
 * @see     @ref PDB_PoutEnable, @ref PDB_PoutInit
 ******************************************************************************/
#define PDB_PoutSetDelay(dly,val)   PDBPOUT0##dly##_SetDelay(val)

/***************************************************************************//*!
 * @brief   PDB pre-trigger initialization.
 * @details This macro initializes specified pre-trigger. It also initializes
 *          pre-trigger delay value.
 * @param   trg    TRG0,TRG1,TRG2,TRG3.
 * @param   cfg    Select one of the @ref pdb_ptrg_config.
 * @param   val    16-bit delay value.
 * @note    Implemented as a function call.
 * @warning @ref PDB_PtrgInit function should be called before PDB module
 *          initialization in order to allow the delay registers to be updated
 *          from their buffers correctly.
 * @see     @ref PDB_PtrgEnable, @ref PDB_PtrgSetDelay
 ******************************************************************************/
#define PDB_PtrgInit(trg,cfg,val)   PDBCH0##trg##_Init(cfg,val)

/***************************************************************************//*!
 * @brief   PDB pulse out module initialization.
 * @details This macro initializes pulse output. It also initializes value1
 *          (pulse out goes high) and value2(pulse out goes low).
 * @param   cfg    Select one of the @ref pdb_pout_config.
 * @param   val1   16-bit delay value (pulse out goes high).
 * @param   val2   16-bit delay value (pulse out goes low again).
 * @note    Implemented as a function call.
 * @warning @ref PDB_PoutInit function should be called before PDB module
 *          initialization in order to allow the delay registers to be updated
 *          from their buffers correctly.
 * @see     @ref PDB_PoutEnable, @ref PDB_PoutSetDelay
 ******************************************************************************/
#define PDB_PoutInit(cfg,val1,val2) PDBPOUT0_Init (cfg,val1,val2)

/***************************************************************************//*!
 * @brief   PDB module initialization.
 * @details This macro initializes PDB module. It also installs callback
 *          function for interrupt vector associated with PDB
 *          module: vector 21(PDB0).
 * @param   pdb       Select one of the @ref pdb_module_config.
 * @param   ip        Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref pdb_callback.
 * @note    Implemented as a function call.
 ******************************************************************************/
#define PDB_Init(pdb,ip,callback)   PDB_Init(pdb,ip,callback)
/*! @} End of pdb_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void PDB_Init (tPDB pdb, uint8 ip, PDB_CALLBACK pCallback);
extern void PDBCH0TRG0_Init (tPDB_TRG trg, uint16 val);
extern void PDBCH0TRG1_Init (tPDB_TRG trg, uint16 val);
extern void PDBCH0TRG2_Init (tPDB_TRG trg, uint16 val);
extern void PDBCH0TRG3_Init (tPDB_TRG trg, uint16 val);
extern void PDBPOUT0_Init (tPDB_PO pout, uint16 val1, uint16 val2);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void pdb_isr (void);

#endif /* __PDB_H */
