/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      lpuart.h
 * @version   1.0.5.1
 * @date      Nov-23-2015
 * @brief     Low-Power Universal Asynchronous Receiver/Transmitter (LPUART)
 *            driver header file.
 ******************************************************************************/
#ifndef __LPUART_H
#define __LPUART_H

#if !defined(MCU_MKM34Z7)
  #error "The lpuart.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 BAUD, STAT, CTRL, MATCH; } tLPUART;

/******************************************************************************
* LPUART flag definition
*
*//*! @addtogroup lpuart_flags
* @{
*******************************************************************************/
/* LPUART Flags                                                               */
#define LPUART_MA2F     (uint32)(1u << 14)   ///< match 2 error flag
#define LPUART_MA1F     (uint32)(1u << 15)   ///< match 1 error flag
#define LPUART_PF       (uint32)(1u << 16)   ///< parity error flag
#define LPUART_FE       (uint32)(1u << 17)   ///< framing error flag
#define LPUART_NF       (uint32)(1u << 18)   ///< noise flag
#define LPUART_OR       (uint32)(1u << 19)   ///< receiver overrun flag
#define LPUART_IDLE     (uint32)(1u << 20)   ///< idle line flag
#define LPUART_RXEDGIF  (uint32)(1u << 30)   ///< RX pin active edge interrupt flag
#define LPUART_LBKDIF   (uint32)(1u << 31)   ///< LIN break detect interrupt flag
#define LPUART_ALL      (uint32)0xC01FC000u  ///< all flags
/*! @} End of lpuart_flags                                                    */

#define LPUART_CALC_SBR_OSR(brate,clk,osr)                                    \
                       (uint16)((double)clk/(((double)osr+1.0)*(double)brate))
#define LPUART_CALC_SBR(brate,clk)           LPUART_CALC_SBR_OSR(brate,clk,15)

/******************************************************************************
* LPUART Single-wire TxD pin direction
*
*//*! @addtogroup lpuart_pindir
* @{
*******************************************************************************/
#define LPUART_PIN_IN         0 ///< LPUART_TX pin is input in single-wire mode
#define LPUART_PIN_OUT        1 ///< LPUART_TX pin is output in single-wire mode
/*! @} End of lpuart_pindir                                                   */

/******************************************************************************
* LPUART Match address enable bits.
*
*//*! @addtogroup lpuart_matchen
* @{
*******************************************************************************/
#define LPUART_MAEN1     0x80000000 ///< Auto addr/data match enabled MATCH[MA1]
#define LPUART_MAEN2     0x40000000 ///< Auto addr/data match enabled MATCH[MA2]
/*! @} End of lpuart_matchen                                                  */

/******************************************************************************
* LPUART default configurations used by LPUART_init() function
*
*//*! @addtogroup lpuart_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Configures LPUART to operate in polling mode. Receive and transmit
 *          data is not inverted.
 * @details Configures LPUART to operate in polling mode. Receive and transmit
 *          data is not inverted.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_POLLMODE_CONFIG(brate,clk)                              \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ CLR(LPUART_BAUD_TDMAE_MASK)|CLR(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|CLR(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ CLR(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}
/***************************************************************************//*!
 * @brief   Configures LPUART to operate in polling mode. Receive data is not
 *          inverted. Transmit data is inverted.
 * @details Configures LPUART to operate in polling mode. Receive data is not
 *          inverted. Transmit data is inverted.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_POLLMODE_TXINV_CONFIG(brate,clk)                        \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ CLR(LPUART_BAUD_TDMAE_MASK)|CLR(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|SET(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ CLR(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}

/***************************************************************************//*!
 * @brief   Configures LPUART to operate in interrupt mode. Receive and transmit
 *          data is not inverted.
 * @details Configures LPUART to operate in interrupt mode. Receive and transmit
 *          data is not inverted.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_INTRMODE_CONFIG(brate,clk)                              \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ CLR(LPUART_BAUD_TDMAE_MASK)|CLR(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|CLR(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}

/***************************************************************************//*!
 * @brief   Configures LPUART to operate in interrupt mode. Receive data is not
 *          inverted. Transmit data is inverted.
 * @details Configures LPUART to operate in interrupt mode. Receive data is not
 *          inverted. Transmit data is inverted.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_INTRMODE_TXINV_CONFIG(brate,clk)                        \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ CLR(LPUART_BAUD_TDMAE_MASK)|CLR(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|SET(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}

/***************************************************************************//*!
 * @brief   Configures LPUART to operate in receive DMA mode. Receive data
 *          register full flag will cause DMA request.
 * @details Configures LPUART to operate in receive DMA mode. Receive data
 *          register full flag will cause DMA request.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_RDRF_DMA_CONFIG(brate,clk)                              \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ CLR(LPUART_BAUD_TDMAE_MASK)|SET(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|CLR(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}

/***************************************************************************//*!
 * @brief   Configures LPUART to operate in transmit DMA mode. Transmit data
 *          register empty flag will cause DMA request.
 * @details Configures LPUART to operate in transmit DMA mode. Transmit data
 *          register empty flag will cause DMA request.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_TDRE_DMA_CONFIG(brate,clk)                              \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ SET(LPUART_BAUD_TDMAE_MASK)|CLR(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|CLR(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ SET(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ CLR(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}

/***************************************************************************//*!
 * @brief   Configures LPUART to operate in DMA mode. Receive data register
 *          full flag and transmit data register empty will cause DMA requests.
 * @details Configures LPUART to operate in DMA mode. Receive data register
 *          full flag and transmit data register empty will cause DMA requests.
 * @param   brate     Baud rate.
 * @param   clk       Module clock in Hz.
 * @showinitializer
 ******************************************************************************/
#define LPUART_MODULE_RDRF_TDRE_DMA_CONFIG(brate,clk)                         \
(tLPUART){                                                                    \
/* BAUD  */ CLR(LPUART_BAUD_MAEN1_MASK)|CLR(LPUART_BAUD_MAEN2_MASK)|          \
/* ..... */ CLR(LPUART_BAUD_M10_MASK)|((15<<24)&0x1f000000)|                  \
/* ..... */ SET(LPUART_BAUD_TDMAE_MASK)|SET(LPUART_BAUD_RDMAE_MASK)|          \
/* ..... */ SET(LPUART_BAUD_MATCFG(0x0))|CLR(LPUART_BAUD_BOTHEDGE_MASK)|      \
/* ..... */ CLR(LPUART_BAUD_RESYNCDIS_MASK)|CLR(LPUART_BAUD_LBKDIE_MASK)|     \
/* ..... */ CLR(LPUART_BAUD_RXEDGIE_MASK)|CLR(LPUART_BAUD_SBNS_MASK)|         \
/* ..... */ (LPUART_CALC_SBR(brate,clk)&0x1FFF),                              \
/* STAT  */ CLR(LPUART_STAT_MSBF_MASK)|CLR(LPUART_STAT_RXINV_MASK)|           \
/* ..... */ CLR(LPUART_STAT_RWUID_MASK)|CLR(LPUART_STAT_BRK13_MASK)|          \
/* ..... */ CLR(LPUART_STAT_LBKDE_MASK),                                      \
/* CTRL  */ CLR(LPUART_CTRL_R8T9_MASK)|CLR(LPUART_CTRL_R9T8_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_TXDIR_MASK)|CLR(LPUART_CTRL_TXINV_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_ORIE_MASK)|CLR(LPUART_CTRL_NEIE_MASK)|            \
/* ..... */ CLR(LPUART_CTRL_FEIE_MASK)|CLR(LPUART_CTRL_PEIE_MASK)|            \
/* ..... */ SET(LPUART_CTRL_TIE_MASK)|CLR(LPUART_CTRL_TCIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_RIE_MASK)|CLR(LPUART_CTRL_ILIE_MASK)|             \
/* ..... */ SET(LPUART_CTRL_TE_MASK)|SET(LPUART_CTRL_RE_MASK)|                \
/* ..... */ CLR(LPUART_CTRL_RWU_MASK)|CLR(LPUART_CTRL_SBK_MASK)|              \
/* ..... */ CLR(LPUART_CTRL_MA1IE_MASK)|CLR(LPUART_CTRL_MA2IE_MASK)|          \
/* ..... */ SET(LPUART_CTRL_IDLECFG(0x0))|CLR(LPUART_CTRL_LOOPS_MASK)|        \
/* ..... */ CLR(LPUART_CTRL_DOZEEN_MASK)|CLR(LPUART_CTRL_RSRC_MASK)|          \
/* ..... */ CLR(LPUART_CTRL_M_MASK)|CLR(LPUART_CTRL_WAKE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_ILT_MASK)|CLR(LPUART_CTRL_PE_MASK)|               \
/* ..... */ CLR(LPUART_CTRL_PT_MASK),                                         \
/* MATCH */ SET(LPUART_MATCH_MA2(0x0))|SET(LPUART_MATCH_MA1(0x0))             \
}
/*! @} End of lpuart_config                                                   */

/******************************************************************************
* LPUART callback registered by LPUART_Init() function
*
*//*! @addtogroup lpuart_callback
* @{
*******************************************************************************/
/*! @brief LPUART_CALLBACK source declaration                                 */
typedef enum
{
  LPUART0_CALLBACK=1    ///< LPUART0 module
} LPUART_CALLBACK_SRC;

/*! @brief LPUART_CALLBACK type declaration                                   */
typedef enum
{
  LPUART_TX_CALLBACK=1, ///< Transmit Data Register Empty Flag (TDRE),
                        ///  Transmit Complete (TC)
  LPUART_RX_CALLBACK=2, ///< Receive Data Register Full (RDRF)
  LPUART_ER_CALLBACK=4, ///< Receiver Overrun (OR), Framing Error (FE),
                        ///  Noise Flag (NF), Parity Error Flag (PF)
  LPUART_EDG_CALLBACK=8 ///< RxD Input Active Edge Interrupt Flag (RXEDGIF)
} LPUART_CALLBACK_TYPE;

/*! LPUART_CALLBACK function declaration                                      */
typedef void (*LPUART_CALLBACK)(LPUART_CALLBACK_SRC module,
                                LPUART_CALLBACK_TYPE type,
                                int32 status);
/*! @} End of lpuart_callback                                                 */

/******************************************************************************
* LPUART function and macro definitions
*
*//*! @addtogroup lpuart_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Disables transmitter interrupt or DMA transfer.
 * @details This macro disables transmitter interrupt or DMA transfer for
 *          specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxIsrEnable, @ref LPUART_TxCmplIsrDisable,
 *          @ref LPUART_TxCmplIsrEnable
 ******************************************************************************/
#define LPUART_TxIsrDisable(module)                                           \
                               ioclrw (module##_CTRL,LPUART_CTRL_TIE_MASK)

/***************************************************************************//*!
 * @brief   Enables transmitter interrupt or DMA transfer.
 * @details This macro enables transmitter interrupt or DMA transfer for
 *          specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxIsrDisable, @ref LPUART_TxCmplIsrDisable,
 *          @ref LPUART_TxCmplIsrEnable
 ******************************************************************************/
#define LPUART_TxIsrEnable(module)                                            \
                               iosetw (module##_CTRL,LPUART_CTRL_TIE_MASK)

/***************************************************************************//*!
 * @brief   Disables transmission complete interrupt.
 * @details This macro disables transmission complete interrupt request for
 *          specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxIsrEnable, @ref LPUART_TxCmplIsrDisable,
 *          @ref LPUART_TxCmplIsrEnable
 ******************************************************************************/
#define LPUART_TxCmplIsrDisable(module)                                       \
                               ioclrw (module##_CTRL,LPUART_CTRL_TCIE_MASK)

/***************************************************************************//*!
 * @brief   Enables transmission complete interrupt.
 * @details This macro enables transmission complete interrupt request for
 *          specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxCmplIsrDisable, @ref LPUART_TxIsrDisable,
 *          @ref LPUART_TxIsrEnable
 ******************************************************************************/
#define LPUART_TxCmplIsrEnable(module)                                        \
                               iosetw (module##_CTRL,LPUART_CTRL_TCIE_MASK)

/***************************************************************************//*!
 * @brief   Disables receiver interrupt.
 * @details This macro disables transmitter interrupt or DMA transfer for
 *          specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxIsrEnable
 ******************************************************************************/
#define LPUART_RxIsrDisable(module)                                           \
                               ioclrw (module##_CTRL,LPUART_CTRL_RIE_MASK)

/***************************************************************************//*!
 * @brief   Enables receiver full interrupt or DMA transfer.
 * @details This macro enables receiver full interrupt or DMA transfer for
 *          specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxIsrDisable
 ******************************************************************************/
#define LPUART_RxIsrEnable(module)                                            \
                               iosetw (module##_CTRL,LPUART_CTRL_RIE_MASK)

/***************************************************************************//*!
 * @brief   Enables RxD input active edge interrupt.
 * @details This macro enables RxD input active edge interrupt for specified
 *          module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_EdgIsrDisable, @ref LPUART_EdgIsrClr,
 *          @ref LPUART_EdgIsrClrEnable
 ******************************************************************************/
#define LPUART_EdgIsrEnable(module)                                           \
                               iosetw (module##_BAUD,LPUART_BAUD_RXEDGIE_MASK)

/***************************************************************************//*!
 * @brief   Clears RxD input active edge interrupt flag.
 * @details This macro clears RxD input active edge interrupt flag for specified
 *          module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_EdgIsrDisable, @ref LPUART_EdgIsrEnable,
 *          @ref LPUART_EdgIsrClrEnable
 ******************************************************************************/
#define LPUART_EdgIsrClr(module)                                              \
                               iosetw (module##_STAT,LPUART_STAT_RXEDGIF_MASK)

/***************************************************************************//*!
 * @brief   Clears RxD input active edge interrupt flag and enables RxD input
 *          active edge interrupt.
 * @details This macro clears RxD input active edge interrupt flag and it also
 *          enables RxD input active edge interrupt for specified module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_EdgIsrDisable, @ref LPUART_EdgIsrEnable, @ref LPUART_EdgIsrClr
 ******************************************************************************/
#define LPUART_EdgIsrClrEnable(module)                                         \
                               iosetw (module##_STAT,LPUART_STAT_RXEDGIF_MASK);\
                               iosetw (module##_BAUD,LPUART_BAUD_RXEDGIE_MASK)

/***************************************************************************//*!
 * @brief   Disables RxD input active edge interrupt.
 * @details This macro disables RxD input active edge interrupt for specified
 *          module.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_EdgIsrEnable, @ref LPUART_EdgIsrClrEnable
 ******************************************************************************/
#define LPUART_EdgIsrDisable(module)                                           \
                               ioclrw (module##_BAUD,LPUART_BAUD_RXEDGIE_MASK)

/***************************************************************************//*!
 * @brief   Disables hardware parity on LPUART.
 * @details This macro disables hardware parity generation and checking.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_SetEvenParity, @ref LPUART_SetOddParity,
 *          @ref LPUART_ParityEnable
 ******************************************************************************/
#define LPUART_ParityDisable(module)                                           \
                               ioclrw (module##_CTRL,LPUART_CTRL_PE_MASK)

/***************************************************************************//*!
 * @brief   Enables hardware parity on LPUART.
 * @details This macro enables hardware parity generation and checking.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_ParityDisable, @ref LPUART_SetOddParity
 *          @ref LPUART_SetEvenParity
 ******************************************************************************/
#define LPUART_ParityEnable(module)   iosetw (module##_CTRL,LPUART_CTRL_PE_MASK)

/***************************************************************************//*!
 * @brief   Enables odd parity.
 * @details This macro selects odd parity when the HW parity is enabled.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_ParityDisable, @ref LPUART_ParityEnable
 *          @ref LPUART_SetEvenParity
 ******************************************************************************/
#define LPUART_SetOddParity(module)   iosetw (module##_CTRL,LPUART_CTRL_PT_MASK)

/***************************************************************************//*!
 * @brief   Enables even parity.
 * @details This macro selects even parity when the HW parity is enabled
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_ParityDisable, @ref LPUART_ParityEnable
 *          @ref LPUART_SetOddParity
 ******************************************************************************/
#define LPUART_SetEvenParity(module)  ioclrw (module##_CTRL,LPUART_CTRL_PT_MASK)

/***************************************************************************//*!
 * @brief   Enables 9-bit mode.
 * @details This macro enables receiver and transmitter to use 9-bit data
 *          characters.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_Set8BitMode
 ******************************************************************************/
#define LPUART_Set9BitMode(module)    iosetw (module##_CTRL,LPUART_CTRL_M_MASK)

/***************************************************************************//*!
 * @brief   Enables 8-bit mode.
 * @details This macro enables receiver and transmitter to use 8-bit data
 *          characters.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_Set9BitMode
 ******************************************************************************/
#define LPUART_Set8BitMode(module)    ioclrw (module##_CTRL,LPUART_CTRL_M_MASK)

/***************************************************************************//*!
 * @brief   Disables receiver.
 * @details This macro disables the LPUART receiver.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxEnable, @ref LPUART_TxEnable
 *          @ref LPUART_TxDisable
 ******************************************************************************/
#define LPUART_RxDisable(module)      ioclrw (module##_CTRL,LPUART_CTRL_RE_MASK)

/***************************************************************************//*!
 * @brief   Enables receiver.
 * @details This macro enables the LPUART receiver.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxDisable, @ref LPUART_TxEnable
 *          @ref LPUART_TxDisable
 ******************************************************************************/
#define LPUART_RxEnable(module)       iosetw (module##_CTRL,LPUART_CTRL_RE_MASK)

/***************************************************************************//*!
 * @brief   Disables transmitter.
 * @details This macro disables the LPUART transmitter.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxEnable, @ref LPUART_RxDisable
 *          @ref LPUART_RxEnable
 ******************************************************************************/
#define LPUART_TxDisable(module)      ioclrw (module##_CTRL,LPUART_CTRL_TE_MASK)

/***************************************************************************//*!
 * @brief   Enables transmitter.
 * @details This macro enables the LPUART transmitter.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxDisable, @ref LPUART_RxDisable, @ref LPUART_RxEnable
 ******************************************************************************/
#define LPUART_TxEnable(module)       iosetw (module##_CTRL,LPUART_CTRL_TE_MASK)

/***************************************************************************//*!
 * @brief   Disables MSB bit first.
 * @details This macro disables MSB bit first. The LSB (bit0) is the 1st bit
 *          that is transmitted following the start bit.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_MSBFirstEnable
 ******************************************************************************/
#define LPUART_MSBFirstDisable(module)                                        \
                                 ioclrw (module##_STAT,LPUART_STAT_MSBF_MASK)

/***************************************************************************//*!
 * @brief   Enables MSB bit first.
 * @details This macro enables MSB bit first. The MSB (bit 9,8,7 or 6) is the
 *          1st bit transmitted following the start bit.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_MSBFirstDisable
 ******************************************************************************/
#define LPUART_MSBFirstEnable(module)                                         \
                               iosetw (module##_STAT,LPUART_STAT_MSBF_MASK)

/***************************************************************************//*!
 * @brief   Disables receive data inversion.
 * @details This macro disables receive data inversion.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxInvEnable
 ******************************************************************************/
#define LPUART_RxInvDisable(module)                                           \
                               ioclrw (module##_STAT,LPUART_STAT_RXINV_MASK)

/***************************************************************************//*!
 * @brief   Enables receive data inversion.
 * @details This macro enables receive data inversion.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxInvDisable
 ******************************************************************************/
#define LPUART_RxInvEnable(module)                                            \
                               iosetw (module##_STAT,LPUART_STAT_RXINV_MASK)

/***************************************************************************//*!
 * @brief   Disables receive wake up idle detect.
 * @details This macro disables receive wake up idle detect.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxWakeupIdleEnable
 ******************************************************************************/
#define LPUART_RxWakeupIdleDisable(module)                                    \
                               ioclrw (module##_STAT,LPUART_STAT_RWUID_MASK)

/***************************************************************************//*!
 * @brief   Enables receive wake up idle detect.
 * @details This macro enables receive wake up idle detect.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxWakeupIdleDisable
 ******************************************************************************/
#define LPUART_RxWakeupIdleEnable(module)                                     \
                               iosetw (module##_STAT,LPUART_STAT_RWUID_MASK)

/***************************************************************************//*!
 * @brief   Enables long Break character generation.
 * @details This macro enables long Break character generation. The Break
 *          character with length of 13 bit times is transmitted.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_LongBreakCharDisable
 ******************************************************************************/
#define LPUART_LongBreakCharEnable(module)                                    \
                               iosetw (module##_STAT,LPUART_STAT_BRK13_MASK)

/***************************************************************************//*!
 * @brief   Disables long Break character generation.
 * @details This macro disables long Break character generation. The Break
 *          character with length of 10 bit times is transmitted.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_LongBreakCharEnable
 ******************************************************************************/
#define LPUART_LongBreakCharDisable(module)                                   \
                               ioclrw (module##_STAT,LPUART_STAT_BRK13_MASK)

/***************************************************************************//*!
 * @brief   Disables LIN Break Detection.
 * @details This macro disables a longer break character detection length.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_LINBreakEnable
 ******************************************************************************/
#define LPUART_LINBreakDisable(module)                                        \
                               ioclrw (module##_STAT,LPUART_STAT_LBKDE_MASK)

/***************************************************************************//*!
 * @brief   Enables LIN Break Detection.
 * @details This macro enables a longer break character detection length.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_LINBreakDisable
 ******************************************************************************/
#define LPUART_LINBreakEnable(module)                                         \
                               iosetw (module##_STAT,LPUART_STAT_LBKDE_MASK)

/***************************************************************************//*!
 * @brief   Reads and returns transmitter state.
 * @details This macro returns transmitter state of the specified module.
 * @param   module  LPUART0.
 * @return  true (non-zero) transmitter idle,
 * @return  false           transmitter active.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_RxFull, @ref LPUART_TxComplete, @ref LPUART_ClrFlags
 ******************************************************************************/
#define LPUART_TxIdle(module)        (module##_STAT & LPUART_STAT_TDRE_MASK)

/***************************************************************************//*!
 * @brief   Reads and returns receiver state.
 * @details This macro returns receiver state of the specified module.
 * @param   module  LPUART0.
 * @return  true (non-zero) character ready,
 * @return  false           receiver busy.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxIdle, @ref LPUART_TxComplete, @ref LPUART_ClrFlags
 ******************************************************************************/
#define LPUART_RxFull(module)        (module##_STAT & LPUART_STAT_RDRF_MASK)

/***************************************************************************//*!
 * @brief   Returns transfer complete state.
 * @details This macro returns transfer complete state.
 * @param   module  LPUART0.
 * @return  true (non-zero) character transmit complete,
 * @return  false           character transmit non-complete.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxIdle, @ref LPUART_RxFull, @ref LPUART_ClrFlags
 ******************************************************************************/
#define LPUART_TxComplete(module)    (module##_STAT & LPUART_STAT_TC_MASK)

/***************************************************************************//*!
 * @brief   Clears LPUART flags.
 * @details This macro clears LPUART flags.
 * @param   module  LPUART0.
 * @param   mask    Select one of the @ref lpuart_flags.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_TxIdle, @ref LPUART_RxFull, @ref LPUART_TxComplete
 ******************************************************************************/
#define LPUART_ClrFlags(module, mask)  module##_STAT |= (uint32)(mask&0xC01FC000)

/***************************************************************************//*!
 * @brief   Reads and returns received character.
 * @details This macro reads character received by the specified module.
 * @param   module  LPUART0.
 * @return  @ref int8   received character.
 * @note    Implemented as an inline macro.
 * @warning Doesn't wait for a new byte (until RDRF is set). Insert
 *          <c>while(!LPUART_RxFull(LPUART?));</c> statement prior this macro
 *          to ensure new byte is returned.
 * @see     @ref LPUART_PutChar, @ref LPUART_PutStr, @ref LPUART_Rd,
 *          @ref LPUART_GetData, @ref LPUART_PutData, @ref LPUART_Wr
 ******************************************************************************/
#define LPUART_GetChar(module)        (uint8)(module##_DATA&0xFF)

/***************************************************************************//*!
 * @brief   Sends character.
 * @details This macro sends character to the specified module.
 * @param   module  LPUART0.
 * @param   c       Character.
 * @note    Implemented as an inline macro.
 * @warning Doesn't wait until character is sent. Insert
 *          <c>while(!LPUART_TxIdle(LPUART?));</c> statement prior this macro to
 *          ensure transmitter is idle and able to send new character.
 * @see     @ref LPUART_GetChar, @ref LPUART_PutStr, @ref LPUART_Rd,
 *          @ref LPUART_GetChar, @ref LPUART_PutData, @ref LPUART_Wr
 ******************************************************************************/
#define LPUART_PutChar(module,c)      module##_DATA = (uint32)(c)

/***************************************************************************//*!
 * @brief   Reads and returns 10-bits data.
 * @details This macro reads character received by the specified module.
 * @param   module  LPUART0.
 * @return  @ref int16  received 16 bit word.
 * @note    Implemented as an inline macro.
 * @warning Doesn't wait for a new byte (until RDRF is set). Insert
 *          <c>while(!LPUART_RxFull(LPUART?));</c> statement prior this macro
 *          to ensure new byte is returned.
 * @see     @ref LPUART_PutChar, @ref LPUART_PutStr, @ref LPUART_Rd,
 *          @ref LPUART_GetChar, @ref LPUART_PutData, @ref LPUART_Wr
 ******************************************************************************/
#define LPUART_GetData(module)        (uint16)(module##_DATA&0x3FF)

/***************************************************************************//*!
 * @brief   Sends data.
 * @details This macro sends data to the specified module.
 * @param   module  LPUART0.
 * @param   c       10-bit data to be sent.
 * @note    Implemented as an inline macro.
 * @warning FRETSC (Frame Error / Transmit Special Character) bit is affected.
 * @see     @ref LPUART_PutChar, @ref LPUART_PutStr, @ref LPUART_Rd,
 *          @ref LPUART_GetChar, @ref LPUART_GetData, @ref LPUART_Wr
 ******************************************************************************/
#define LPUART_PutData(module,c)      module##_DATA = (uint32)(c)

/***************************************************************************//*!
 * @brief   Sends string.
 * @details This macro sends string to the specified module.
 * @param   module  LPUART0.
 * @param   str     String terminated by null character.
 * @note    Implemented as an inline macro.
 * @warning Doesn't terminate until NULL character is read.
 * @see     @ref LPUART_GetChar, @ref LPUART_PutChar, @ref LPUART_Rd,
 *          @ref LPUART_GetData, @ref LPUART_PutData, @ref LPUART_Wr
 ******************************************************************************/
#define LPUART_PutStr(module,str)     {                                       \
                                        register uint16 __tmp=0;              \
                                        while(str[__tmp] != 0)                \
                                        {                                     \
                                          while (!LPUART_TxIdle(module));     \
                                          LPUART_PutChar(module,str[__tmp]);  \
                                          __tmp++;                            \
                                        }                                     \
                                        while (!LPUART_TxComplete(module));   \
                                      }

/***************************************************************************//*!
 * @brief   Reads <c>count</c> bytes and stores them in a <c>buffer</c>.
 * @details This macro reads <c>count</c> bytes from specified module and
 *          stores them in <c>buffer</c>. Returns when <c>count</c> bytes have
 *          been read.
 * @param   module  LPUART0.
 * @param   buffer  @ref uint8 [] specifies the variable in which to store the
 *                  data that was read from the selected module.
 * @param   count   Specifies the number of bytes to read from the selected
 *                    module.
 * @note    Implemented as an inline macro.
 * @warning Doesn't terminate until <c>count</c> bytes are read.
 * @see     @ref LPUART_GetChar, @ref LPUART_PutChar, @ref LPUART_PutStr,
 *          @ref LPUART_GetData, @ref LPUART_PutData, @ref LPUART_Wr
 ******************************************************************************/
#define LPUART_Rd(module,buffer,count)                                        \
{                                                                             \
  register uint16 __tmp=0;                                                    \
  while (__tmp < count)                                                       \
  {                                                                           \
    while (!LPUART_RxFull(module));   /* wait until new byte is available  */ \
    buffer[__tmp++]=module##_DATA;    /* read and store new byte           */ \
  }                                                                           \
}

/***************************************************************************//*!
 * @brief   Writes <c>count</c> bytes from <c>buffer</c>.
 * @details This macro writes <c>count</c> bytes from <c>buffer</c> to the
 *          specified module. Returns when <c>count</c> bytes are written.
 * @param   module  LPUART0.
 * @param   buffer  @ref uint8 [] specifies the variable from which to read
 *                  the data that will be written to the selected module.
 * @param   count   Specifies the number of bytes to write to the selected
 *                  module.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_GetChar, @ref LPUART_PutChar, @ref LPUART_PutStr,
 *          @ref LPUART_GetData, @ref LPUART_PutData, @ref LPUART_Rd
 ******************************************************************************/
#define LPUART_Wr(module,buffer,count)                                        \
{                                                                             \
  register uint16 __tmp=0;                                                    \
  while (__tmp < count)                                                       \
  {                                                                           \
    while(!LPUART_TxIdle(module));  /* wait until transmitter is idle     */  \
    module##_DATA = buffer[__tmp++];/* write new byte                     */  \
  }                                                                           \
  while(!LPUART_TxIdle(module));    /* wait until last character is sent  */  \
}

/***************************************************************************//*!
 * @brief   Enables/disables internal loop mode.
 * @details This macro enables/disables internal loop mode for the specified
 *          module. When loop mode is set, the RxD pin is disconnected from the
 *          LPUART and the transmitter output is internally connected to the
 *          receiver input. The transmitter and the receiver must be enabled to
 *          use the loop function.
 * @param   module  LPUART0.
 * @param   ctrl    TRUE (loopback mode enabled, receiver input is internally
 *                        connected to transmitter output)\n
 *                  FALSE (normal operation).
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define LPUART_LoopModeCtrl(module,ctrl)                                      \
{                                                                             \
  if (ctrl==TRUE)                                                             \
  {                                                                           \
    iosetw(module##_CTRL,LPUART_CTRL_LOOPS_MASK);                             \
    ioclrw(module##_CTRL, LPUART_CTRL_RSRC_MASK);                             \
  }                                                                           \
  else                                                                        \
    ioclrw(module##_CTRL,LPUART_CTRL_LOOPS_MASK);                             \
}

/***************************************************************************//*!
 * @brief   Enables single-wire mode.
 * @details This macro enables the single-wire mode, TX pin acts as input or
 *          output.
 * @param   module  LPUART0.
 * @param   dir     Select one of the @ref lpuart_pindir.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_SingleWireDisable
 ******************************************************************************/
#define LPUART_SingleWireEnable(module,dir)                                   \
{                                                                             \
  module##_CTRL |= (LPUART_CTRL_LOOPS_MASK|LPUART_CTRL_RSRC_MASK);            \
  iobfiw(module##_CTRL,LPUART_CTRL_TXDIR_SHIFT,1,dir);                        \
}

/***************************************************************************//*!
 * @brief   Disables single-wire mode.
 * @details This macro disables the single-wire mode, TX/RX pins working normally.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_SingleWireEnable
 ******************************************************************************/
#define LPUART_SingleWireDisable(module)                                      \
{                                                                             \
  module##_CTRL&=~(LPUART_CTRL_LOOPS_MASK|LPUART_CTRL_RSRC_MASK|              \
                   LPUART_CTRL_TXDIR_MASK);                                   \
}

/***************************************************************************//*!
 * @brief   Disables match address mode.
 * @details This macro disables match address mode.
 * @param   module  LPUART0.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_MatchAddrConfig
 ******************************************************************************/
#define LPUART_MatchAddrDisable(module)                                       \
{                                                                             \
  module##_BAUD &= ~(LPUART_BAUD_MAEN1_MASK|LPUART_BAUD_MAEN2_MASK);          \
  module##_MATCH = 0x00;                                                      \
}

/***************************************************************************//*!
 * @brief   Enables match address mode.
 * @details This macro enables match address mode, sets the match addresses.
 * @param   module  LPUART0.
 * @param   ma      Select one of the @ref lpuart_matchen.
 * @param   addr1   Address of first match.
 * @param   addr2   Address of second match.
 * @note    Implemented as an inline macro.
 * @see     @ref LPUART_MatchAddrDisable
 ******************************************************************************/
#define LPUART_MatchAddrConfig(module,ma,addr1,addr2)                         \
{                                                                             \
  module##_BAUD = (ma)|(module##_BAUD&0x3FFFFFFF);                            \
  iobfiw(module##_MATCH,LPUART_MATCH_MA1_SHIFT,10,addr1);                     \
  iobfiw(module##_MATCH,LPUART_MATCH_MA2_SHIFT,10,addr2);                     \
}

/***************************************************************************//*!
 * @brief   Installs callback function for LPUART interrupt vector 43.
 * @details This function installs callback function for LPUART interrupt
 *          vector 43.
 * @param   ip        Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref lpuart_callback.
 * @note    Implemented as a function call.
 * @see     @ref LPUART_Init
 ******************************************************************************/
#define LPUART_InstallCallback(ip,callback) LPUART0_InstallCallback(ip,callback)

/***************************************************************************//*!
 * @brief   LPUART initialization.
 * @details This function initializes Low Power UART (LPUART) block.
 * @param   module  LPUART0.
 * @param   cfg     Select one of the @ref lpuart_config.
 * @note    Implemented as function call.
 * @see     @ref LPUART_InstallCallback
 ******************************************************************************/
#define LPUART_Init(module,cfg)   module##_Init(cfg)
/*! @} End of lpuart_macro                                                    */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void LPUART0_Init (tLPUART sci);
extern void LPUART0_InstallCallback (uint8 ip, LPUART_CALLBACK pCallback);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void lpuart_isr (void);

#endif /* __LPUART_H */
