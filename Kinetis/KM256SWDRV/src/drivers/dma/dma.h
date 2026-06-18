/******************************************************************************
 * (c) Copyright 2010-2016, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      dma.h
 * @version   1.0.4.0
 * @date      Feb-17-2016
 * @brief     Direct Memory Access (DMA) driver header file.
 ******************************************************************************/
#ifndef __DMA_H
#define __DMA_H

#if !defined(MCU_MKM34Z7)
  #error "The dma.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * Configuration structure definition                                         *
 ******************************************************************************/
typedef struct { uint32 CHCFG, SAR, DAR, DSR, DCR; } tDMA_CH;

/***************************************************************************//*!
 *//*! @addtogroup dma_request_sources
 * @{
 ******************************************************************************/
#define DMA_REQ_DISABLED    (uint8)0   ///< Channel disabled (default)
#define DMA_REQ_UART0_RX    (uint8)2   ///< UART0 Receive
#define DMA_REQ_UART0_TX    (uint8)3   ///< UART0 Transmit
#define DMA_REQ_UART1_RX    (uint8)4   ///< UART1 Receive
#define DMA_REQ_UART1_TX    (uint8)5   ///< UART1 Transmit
#define DMA_REQ_UART2_RX    (uint8)6   ///< UART2 Receive
#define DMA_REQ_UART2_TX    (uint8)7   ///< UART2 Transmit
#define DMA_REQ_UART3_RX    (uint8)8   ///< UART3 Receive
#define DMA_REQ_UART3_TX    (uint8)9   ///< UART3 Transmit
#define DMA_REQ_SPI0_RX     (uint8)16  ///< SPI0 Receive
#define DMA_REQ_SPI0_TX     (uint8)17  ///< SPI0 Transmit
#define DMA_REQ_SPI1_RX     (uint8)18  ///< SPI1 Receive
#define DMA_REQ_SPI1_TX     (uint8)19  ///< SPI1 Transmit
#define DMA_REQ_I2C0        (uint8)22  ///< I2C0
#define DMA_REQ_I2C1        (uint8)23  ///< I2C1
#define DMA_REQ_TMR_CH0     (uint8)24  ///< Quad Timer CH0 OFLAG
#define DMA_REQ_TMR_CH1     (uint8)25  ///< Quad Timer CH1 OFLAG
#define DMA_REQ_TMR_CH2     (uint8)26  ///< Quad Timer CH2 OFLAG
#define DMA_REQ_TMR_CH3     (uint8)27  ///< Quad Timer CH3 OLAG
#define DMA_REQ_XBAR0       (uint8)28  ///< XBAR DMA request 0
#define DMA_REQ_XBAR1       (uint8)29  ///< XBAR DMA request 1
#define DMA_REQ_XBAR2       (uint8)30  ///< XBAR DMA request 2
#define DMA_REQ_XBAR3       (uint8)31  ///< XBAR DMA request 3
#define DMA_REQ_AFE_CH0     (uint8)32  ///< AFE CH0
#define DMA_REQ_AFE_CH1     (uint8)33  ///< AFE CH1
#define DMA_REQ_AFE_CH2     (uint8)34  ///< AFE CH2
#define DMA_REQ_AFE_CH3     (uint8)35  ///< AFE CH3
#define DMA_REQ_PORTJ       (uint8)36  ///< Port J
#define DMA_REQ_PORTK       (uint8)37  ///< Port K
#define DMA_REQ_PORTL       (uint8)38  ///< Port L
#define DMA_REQ_PORTM       (uint8)39  ///< Port M
#define DMA_REQ_ADC         (uint8)40  ///< SAR ADC
#define DMA_REQ_CMP0        (uint8)42  ///< CMP0
#define DMA_REQ_CMP1        (uint8)43  ///< CMP1
#define DMA_REQ_CMP2        (uint8)44  ///< CMP2
#define DMA_REQ_MMAU        (uint8)47  ///< MMAU
#define DMA_REQ_PDB         (uint8)48  ///< PDB
#define DMA_REQ_PORTA       (uint8)49  ///< Port A
#define DMA_REQ_PORTB       (uint8)50  ///< Port B
#define DMA_REQ_PORTC       (uint8)51  ///< Port C
#define DMA_REQ_PORTD       (uint8)52  ///< Port D
#define DMA_REQ_PORTE       (uint8)53  ///< Port E
#define DMA_REQ_PORTF       (uint8)54  ///< Port F
#define DMA_REQ_PORTG       (uint8)55  ///< Port G
#define DMA_REQ_PORTH       (uint8)56  ///< Port H
#define DMA_REQ_PORTI       (uint8)57  ///< Port I
#define DMA_REQ_LPUART_RX   (uint8)58  ///< LPUART Receive
#define DMA_REQ_LPUART_TX   (uint8)59  ///< LPUART Transmit
#define DMA_REQ_ENABLED     (uint8)60  ///< Always enabled
/*! @} End of dma_request_sources                                             */

/***************************************************************************//*!
 *//*! @addtogroup dma_request_sizes
 * @{
 ******************************************************************************/
#define DMA_SIZE32BIT       (uint8)0   ///< 4-byte transfer (default)
#define DMA_SIZE8BIT        (uint8)1   ///< 1-byte transfer
#define DMA_SIZE16BIT       (uint8)2   ///< 2-byte transfer
/*! @} End of dma_request_sizes                                               */

/***************************************************************************//*!
 *//*! @addtogroup dma_chacr_attr
 * @{
 ******************************************************************************/
#define DMA_CHACR_RW_RW_RW  (uint8)0  ///< User nonsecure=Read/Write, User secure=Read/Write, Privileged secure=Read/Write (default)
#define DMA_CHACR_NO_RW_RW  (uint8)1  ///< User nonsecure=None, User secure=Read/Write, Privileged secure=Read/Write
#define DMA_CHACR_NO_NO_RW  (uint8)2  ///< User nonsecure=None, User secure=None, Privileged secure=Read/Write
/*! @} End of dma_chacr_attr                                                  */

/***************************************************************************//*!
 *//*! @addtogroup dma_umnsm_attr
 * @{
 ******************************************************************************/
#define DMA_UMNSM_EN_PRIV_OR_USER_SECURE_OR_NONSEC  (uint8)0  ///< Enable privileged or user, secure or nonsecure (default).
#define DMA_UMNSM_FR_PRIV_SECURE                    (uint8)1  ///< Forced to privileged secure.
#define DMA_UMNSM_FR_USER_SECURE                    (uint8)2  ///< Forced to user secure.
#define DMA_UMNSM_FR_USER_UNSEC                     (uint8)3  ///< Forced to user unsecure.
/*! @} End of dma_umnsm_attr                                                  */

/******************************************************************************
* DMA default configurations used by DMA_Init() function
*
*//*! @addtogroup dma_chan_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Selects and starts software triggered continuous memory to memory data
 * transfer.
 * @details Configures and starts DMA channel to operate in software trigger
 * continuous memory to memory data transfer.
 * @param   srcaddr   Source memory buffer address (uint32).
 * @param   dstaddr   Destination memory buffer address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_SWTRG_M2M_CN_CONFIG(srcaddr,dstaddr,nbytes)                    \
(tDMA_CH){                                                                    \
/* CHCFG */  CLR(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(0)), /* DMA_REQ_DISABLED */              \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)dstaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|CLR(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  CLR(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|CLR(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(DMA_SIZE8BIT))|         \
/* ...   */  SET(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(DMA_SIZE8BIT))|         \
/* ...   */  SET(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|CLR(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects software initiated continuous memory to peripheral data transfer.
 * @details Configures software initiated continuous memory to peripheral data
 * transfer.
 * @param   srcaddr   Source memory buffer address (uint32-regsize aligned)
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   regaddr   Destination peripheral register address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_SWTRG_M2P_CN_CONFIG(srcaddr,regsize,regaddr,nbytes)            \
(tDMA_CH){                                                                    \
/* CHCFG */  CLR(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(0)), /* DMA_REQ_DISABLED */              \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)regaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|CLR(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  CLR(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|CLR(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  SET(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|CLR(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects and starts hardware triggered continuous memory to memory data
 * transfer.
 * @details Configures and starts DMA channel to operate in hardware trigger
 * continuous memory to memory data transfer.
 * @param   reqsrc    Select one of the @ref dma_request_sources.
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   srcaddr   Source memory buffer address (uint32).
 * @param   dstaddr   Destination memory buffer address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_HWTRG_M2M_CN_CONFIG(reqsrc,regsize,srcaddr,dstaddr,nbytes)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)dstaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  CLR(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  SET(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects and starts hardware triggered cycle steal memory to memory data
 * transfer.
 * @details Configures and starts DMA channel to operate in hardware trigger
 * cycle steal memory to memory data transfer.
 * @param   reqsrc    Select one of the @ref dma_request_sources.

 * @param   regsize   Select @ref dma_request_sizes.
 * @param   srcaddr   Source memory buffer address (uint32).
 * @param   dstaddr   Destination memory buffer address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_HWTRG_M2M_CS_CONFIG(reqsrc,regsize,srcaddr,dstaddr,nbytes)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)dstaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  SET(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects hardware initiated cycle steal peripheral to memory data
 * transfer.
 * @details Configures hardware initiated cycle steal peripheral to memory data
 * transfer.
 * @param   reqsrc    Select one of the @ref dma_request_sources.
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   regaddr   Peripheral register address (uint32).
 * @param   dstaddr   Destination memory buffer address (uint32-regsize aligned).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_HWTRG_P2M_CS_CONFIG(reqsrc,regsize,regaddr,dstaddr,nbytes)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)regaddr,                                                 \
/* DAR   */  (uint32)dstaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  CLR(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  SET(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects hardware initiated cycle steal memory to peripheral data
 * transfer.
 * @details Configures hardware initiated cycle steal memory to peripheral data
 * transfer.
 * @param   reqsrc    Select one of the @ref dma_request_sources.
 * @param   srcaddr   Source memory buffer address (uint32-regsize aligned)
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   regaddr   Destination peripheral register address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_HWTRG_M2P_CS_CONFIG(reqsrc,srcaddr,regsize,regaddr,nbytes)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)regaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects hardware initiated cycle steal peripheral to peripheral data
 * transfer executed in a loop.
 * @details Configures hardware initiated cycle steal peripheral to peripheral
 * data transfer executed in a loop.
 * @param   reqsrc    Select one of the @ref dma_request_sources.
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   reg1addr  Source peripheral register address (uint32).
 * @param   reg2addr  Destination peripheral register address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 * @showinitializer
 ******************************************************************************/
#define DMA_CH_HWTRG_P2P_CS_LOOP_CONFIG(reqsrc,regsize,reg1addr,reg2addr,nbytes) \
(tDMA_CH){                                                                       \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|            \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                   \
/* SAR   */  (uint32)reg1addr,                                                   \
/* DAR   */  (uint32)reg2addr,                                                   \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),               \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                       \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))|    \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                     \
/* ...   */  CLR(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|                 \
/* ...   */  CLR(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|                 \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                       \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                       \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0))    \
}
/*! @} End of dma_chan_config                                                 */

/******************************************************************************
* DMA callback registered by DMA_Init() function
*
*//*! @addtogroup dma_ch_callback
* @{
*******************************************************************************/
/*! DMA_CH_CALLBACK type declaration                                          */
typedef enum
{
  CE_CALLBACK   =1,   ///< Configuration error (CE) interrupt
  DONE_CALLBACK =2,   ///< Transactions done (DONE) interrupt
} DMA_CH_CALLBACK_TYPE;

/*! @brief DMA_CH_CALLBACK function declaration                               */
typedef void (*DMA_CH_CALLBACK)(DMA_CH_CALLBACK_TYPE type);
/*! @} End of dma_ch_callback                                                 */

/******************************************************************************
* DMA function and macro definitions
*
*//*! @addtogroup dma_macro
* @{
*******************************************************************************/
#define DMA0_Ack              DMA_DSR_BCR0=DMA_DSR_BCR0
#define DMA1_Ack              DMA_DSR_BCR1=DMA_DSR_BCR1
#define DMA2_Ack              DMA_DSR_BCR2=DMA_DSR_BCR2
#define DMA3_Ack              DMA_DSR_BCR3=DMA_DSR_BCR3
/***************************************************************************//*!
 * @brief   DMA channel transfer complete acknowledge.
 * @details This macro acknowledges completed DMA channel transfer.
 * @param   ch     DMA0,DMA1,DMA2,DMA3.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define DMA_Ack(ch)           ch##_Ack

#define DMA0_IsCfgError       (DMA_DSR_BCR0&DMA_DSR_BCR_CE_MASK)
#define DMA1_IsCfgError       (DMA_DSR_BCR1&DMA_DSR_BCR_CE_MASK)
#define DMA2_IsCfgError       (DMA_DSR_BCR2&DMA_DSR_BCR_CE_MASK)
#define DMA3_IsCfgError       (DMA_DSR_BCR3&DMA_DSR_BCR_CE_MASK)
/***************************************************************************//*!
 * @brief   DMA channel configuration error check.
 * @details This macro checks DMA channel for configuration error.
 * @param   ch     DMA0,DMA1,DMA2,DMA3.
 * @return  true (non-zero) channel configuration error occurred (call @ref DMA_Ack
 *                          to clear error condition),
 * @return  false           no configuration error exists.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define DMA_IsCfgError(ch)    ch##_IsCfgError

#define DMA0_IsRdError        (DMA_DSR_BCR0&DMA_DSR_BCR_BES_MASK)
#define DMA1_IsRdError        (DMA_DSR_BCR1&DMA_DSR_BCR_BES_MASK)
#define DMA2_IsRdError        (DMA_DSR_BCR2&DMA_DSR_BCR_BES_MASK)
#define DMA3_IsRdError        (DMA_DSR_BCR3&DMA_DSR_BCR_BES_MASK)
/***************************************************************************//*!
 * @brief   DMA channel bus error occurred during the read portion of a transfer.
 * @details This macro checks DMA channel for bus error during the read portion
 *          of a transfer.
 * @param   ch     DMA0,DMA1,DMA2,DMA3.
 * @return  true (non-zero) bus error occurred during the read (call
 *                          @ref DMA_Ack to clear error condition),
 * @return  false           no bus error occurred.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define DMA_IsRdError(ch)     ch##_IsRdError

#define DMA0_IsWrError        (DMA_DSR_BCR0&DMA_DSR_BCR_BED_MASK)
#define DMA1_IsWrError        (DMA_DSR_BCR1&DMA_DSR_BCR_BED_MASK)
#define DMA2_IsWrError        (DMA_DSR_BCR2&DMA_DSR_BCR_BED_MASK)
#define DMA3_IsWrError        (DMA_DSR_BCR3&DMA_DSR_BCR_BED_MASK)
/***************************************************************************//*!
 * @brief   DMA channel bus error occurred during the write portion of a transfer.
 * @details This macro checks DMA channel for bus error during the write portion
 *          of a transfer.
 * @param   ch    DMA0,DMA1,DMA2,DMA3.
 * @return  true (non-zero) bus error occurred during the write (call
 *                          @ref DMA_Ack to clear error condition),
 * @return  false           no bus error occurred.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define DMA_IsWrError(ch)     ch##_IsWrError

#define DMA0_IsDone           (DMA_DSR_BCR0&DMA_DSR_BCR_DONE_MASK)
#define DMA1_IsDone           (DMA_DSR_BCR1&DMA_DSR_BCR_DONE_MASK)
#define DMA2_IsDone           (DMA_DSR_BCR2&DMA_DSR_BCR_DONE_MASK)
#define DMA3_IsDone           (DMA_DSR_BCR3&DMA_DSR_BCR_DONE_MASK)
/***************************************************************************//*!
 * @brief   DMA channel transfer complete check.
 * @details This macro checks DMA channel transfer complete.
 * @param   ch    DMA0,DMA1,DMA2,DMA3.
 * @return  true (non-zero) channel DMA transfer completed call @ref DMA_Ack
 *                          to acknowledge the DMA transfer),
 * @return  false           DMA channel transfer is not yet complete.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define DMA_IsDone(ch)        ch##_IsDone

/***************************************************************************//*!
 * @brief   Installs callback function for specified DMA channel.
 * @details This function installs callback function for specified DMA channel.
 * @param   ch        DMA0,DMA1,DMA2,DMA3.
 * @param   ip        Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref dma_ch_callback.
 * @note    Implemented as a function call. A channel callback function must be 
 *          installed before DMA channel is initialized using @ref DMA_Init or
 *          @ref DMA_AdvInit function.
 * @see     @ref DMA_Init, @ref DMA_AdvInit
 ******************************************************************************/
#define DMA_InstallCallback(ch,ip,callback)                                 \
                              DMA_InstallCallback##ch(ip,callback)

/***************************************************************************//*!
 * @brief   DMA channel initialization.
 * @details This function initializes specified DMA channel for data transfer.
 * @param   ch        DMA0,DMA1,DMA2,DMA3.
 * @param   dma       Structure containing the initialization parameters
 *                    @ref dma_chan_config
 * @note    Implemented as a function call. Initialize DMA channel after a channel
 *          callback function is installed using @ref DMA_InstallCallback function.
 * @see     @ref DMA_AdvInit, @ref DMA_InstallCallback
 ******************************************************************************/
#define DMA_Init(ch,dma)      ch##_Init(dma)

/***************************************************************************//*!
 * @brief   Advanced DMA channel initialization with access rights setting.
 * @details This function initializes specified DMA channel for data transfer.
 * It also sets the access control attributes needed to reference the channel's
 * transfer channel descriptor (TCDn) and defines the privileged/user and
 * secure/nonsecure attributes for the DMA channel as it executes.
 * @param   ch      DMA0,DMA1,DMA2,DMA3.
 * @param   dma     Structure containing the initialization parameters
 *                  @ref dma_chan_config
 * @param   chacr   Select one of the @ref dma_chacr_attr.
 * @param   umnsm   Select one of the @ref dma_umnsm_attr.
 * @note    Implemented as a function call. Initialize DMA channel after a channel
 *          callback function is installed using @ref DMA_InstallCallback function.
 * @see     @ref DMA_Init, @ref DMA_InstallCallback
 ******************************************************************************/
#define DMA_AdvInit(ch,dma,chacr,umnsm)     ch##_AdvInit(dma,chacr,umnsm)
/*! @} End of dma_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void DMA_InstallCallbackDMA0 (uint8 ip, DMA_CH_CALLBACK pCallback);
extern void DMA_InstallCallbackDMA1 (uint8 ip, DMA_CH_CALLBACK pCallback);
extern void DMA_InstallCallbackDMA2 (uint8 ip, DMA_CH_CALLBACK pCallback);
extern void DMA_InstallCallbackDMA3 (uint8 ip, DMA_CH_CALLBACK pCallback);

extern void DMA0_Init(tDMA_CH dma);
extern void DMA1_Init(tDMA_CH dma);
extern void DMA2_Init(tDMA_CH dma);
extern void DMA3_Init(tDMA_CH dma);

extern void DMA0_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
extern void DMA1_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
extern void DMA2_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
extern void DMA3_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void dma0_isr (void);
extern void dma1_isr (void);
extern void dma2_isr (void);
extern void dma3_isr (void);

#endif /* __DMA_H */
