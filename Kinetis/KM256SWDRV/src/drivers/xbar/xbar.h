/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      xbar.h
 * @version   1.0.5.0
 * @date      Apr-20-2015
 * @brief     Inter-Peripheral Crossbar Switch (XBAR) driver header file.
 ******************************************************************************/
#ifndef __XBAR_H
#define __XBAR_H

#if !defined(MCU_MKM34Z7)
  #error "The xbar.h module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * Configuration structure definition                                         *
 ******************************************************************************/
typedef struct { uint16 CTRL;  } tXBAR;

/******************************************************************************
* XBAR input channel definition used by XBAR_SetMux() function
*
*//*! @addtogroup xbar_inputs
* @{
*******************************************************************************/
#define XBAR_LOGIC1     (uint8)0      ///< Logic 1 (VDD)
#define XBAR_LOGIC0     (uint8)1      ///< Logic 0 (VSS)
#define XBAR_AFEMCLK    (uint8)2      ///< AFE Master Clock Output
#define XBAR_AFEMDAT0   (uint8)3      ///< AFE Modulator 0 Data Output
#define XBAR_LPTMR      (uint8)4      ///< LPTimer Output
#define XBAR_CLKOUT     (uint8)5      ///< Clock Output
#define XBAR_TMR0       (uint8)6      ///< Quad Timer channel 0 output
#define XBAR_TMR1       (uint8)7      ///< Quad Timer channel 1 output
#define XBAR_TMR2       (uint8)8      ///< Quad Timer channel 2 output
#define XBAR_TMR3       (uint8)9      ///< Quad Timer channel 3 output
#define XBAR_RTCCLKOUT  (uint8)10     ///< RTC Clock Output
#define XBAR_CMP0OUT    (uint8)11     ///< CMP0 Output
#define XBAR_CMP1OUT    (uint8)12     ///< CMP1 Output
#define XBAR_IRTCALARM  (uint8)13     ///< iRTC Alarm Output
#define XBAR_UARTMODTX  (uint8)14     ///< UART TX Output (after modulation)
#define XBAR_EWMOUT     (uint8)15     ///< EWM Output (/EWM_OUT)
#define XBAR_PIT0TIF0   (uint8)16     ///< PIT 0 TIF0
#define XBAR_IN0        (uint8)17     ///< XBAR Input pin 0
#define XBAR_IN1        (uint8)18     ///< XBAR Input pin 1
#define XBAR_IN2        (uint8)19     ///< XBAR Input pin 2
#define XBAR_IN3        (uint8)20     ///< XBAR Input pin 3
#define XBAR_IN4        (uint8)21     ///< XBAR Input pin 4
#define XBAR_IN5        (uint8)22     ///< XBAR Input pin 5
#define XBAR_IN6        (uint8)23     ///< XBAR Input pin 6
#define XBAR_IN7        (uint8)24     ///< XBAR Input pin 7
#define XBAR_IN8        (uint8)25     ///< XBAR Input pin 8
#define XBAR_ADCALLCOC  (uint8)26     ///< OR'ed conversion complete flag for all SAR ADC channels
#define XBAR_AFEALLCOC  (uint8)27     ///< OR'ed conversion complete flag for all AFE channels
#define XBAR_AFE0COC    (uint8)28     ///< AFE channel 0 conversion complete flag
#define XBAR_AFE1COC    (uint8)29     ///< AFE channel 1 conversion complete flag
#define XBAR_AFE2COC    (uint8)30     ///< AFE channel 2 conversion complete flag
#define XBAR_AFE3COC    (uint8)31     ///< AFE channel 3 conversion complete flag
#define XBAR_DMADONE    (uint8)32     ///< DMA Done Signal
#define XBAR_IN9        (uint8)33     ///< XBAR Input pin 9
#define XBAR_IN10       (uint8)34     ///< XBAR Input pin 10
#define XBAR_CMP2OUT    (uint8)35     ///< CMP2 Output
#define XBAR_PIT0TIF1   (uint8)36     ///< PIT 0 TIF1
#define XBAR_PIT1TIF0   (uint8)37     ///< PIT 1 TIF0
#define XBAR_PIT1TIF1   (uint8)38     ///< PIT 1 TIF1
#define XBAR_AFEMDAT1   (uint8)39     ///< AFE modulator 1 data output
#define XBAR_AFEMDAT2   (uint8)40     ///< AFE modulator 2 data output
#define XBAR_AFEMDAT3   (uint8)41     ///< AFE modulator 3 data output
#define XBAR_ADCCOCCHA  (uint8)42     ///< SAR ADC COCO A
#define XBAR_ADCCOCCHB  (uint8)43     ///< SAR ADC COCO B
#define XBAR_ADCCOCCHC  (uint8)44     ///< SAR ADC COCO C
#define XBAR_ADCCOCCHD  (uint8)45     ///< SAR ADC COCO D
#define XBAR_PDBPTRG0   (uint8)46     ///< PDB0 Pre-trigger0
#define XBAR_PDBPTRG1   (uint8)47     ///< PDB0 Pre-trigger1
#define XBAR_PDBPTRG2   (uint8)48     ///< PDB0 Pre-trigger2
#define XBAR_PDBPTRG3   (uint8)49     ///< PDB0 Pre-trigger3
#define XBAR_PDBTRG     (uint8)50     ///< PDB0 CH0 Trigger
#define XBAR_PDBPOUT0   (uint8)51     ///< PDB0 Pulse-Out 0
/*! @} End of xbar_inputs                                                     */

/******************************************************************************
* XBAR output channel definition used by XBAR_SetMux() function
*
*//*! @addtogroup xbar_outputs
* @{
*******************************************************************************/
#define XBAR_DMA0       (int64)(1ll<< 0)  ///< XBAR DMA request or Interrupt 0
#define XBAR_DMA1       (int64)(1ll<< 1)  ///< XBAR DMA request or Interrupt 1
#define XBAR_DMA2       (int64)(1ll<< 2)  ///< XBAR DMA request or Interrupt 2
#define XBAR_DMA3       (int64)(1ll<< 3)  ///< XBAR DMA request or Interrupt 3
#define XBAR_CMP2INP    (int64)(1ll<< 4)  ///< CMP2 Sample Window input
#define XBAR_TMR0SEC    (int64)(1ll<< 5)  ///< Quad Timer channel 0 secondary input
#define XBAR_TMR1SEC    (int64)(1ll<< 6)  ///< Quad Timer channel 1 secondary input
#define XBAR_TMR2SEC    (int64)(1ll<< 7)  ///< Quad Timer channel 2 secondary input
#define XBAR_TMR3SEC    (int64)(1ll<< 8)  ///< Quad Timer channel 3 secondary input
#define XBAR_TMRPRI1    (int64)(1ll<< 9)  ///< Quad Timer primary clock input 1
#define XBAR_TMRPRI2    (int64)(1ll<<10)  ///< Quad Timer primary clock input 2
#define XBAR_CMP0INP    (int64)(1ll<<11)  ///< CMP0 Sample Window input
#define XBAR_CMP1INP    (int64)(1ll<<12)  ///< CMP1 Sample Window input
#define XBAR_UARTRXINP  (int64)(1ll<<13)  ///< UART Rx Input
#define XBAR_UARTTXMOD  (int64)(1ll<<14)  ///< UART Tx Modulation Output
#define XBAR_ADCTRGCHA  (int64)(1ll<<15)  ///< ADC trigger select A pulse
#define XBAR_ADCTRGCHB  (int64)(1ll<<16)  ///< ADC trigger select B pulse
#define XBAR_OUT0       (int64)(1ll<<17)  ///< XBAR Output pin 0
#define XBAR_OUT1       (int64)(1ll<<18)  ///< XBAR Output pin 1
#define XBAR_OUT2       (int64)(1ll<<19)  ///< XBAR Output pin 2
#define XBAR_OUT3       (int64)(1ll<<20)  ///< XBAR Output pin 3
#define XBAR_OUT4       (int64)(1ll<<21)  ///< XBAR Output pin 4
#define XBAR_OUT5       (int64)(1ll<<22)  ///< XBAR Output pin 5
#define XBAR_OUT6       (int64)(1ll<<23)  ///< XBAR Output pin 6
#define XBAR_OUT7       (int64)(1ll<<24)  ///< XBAR Output pin 7
#define XBAR_OUT8       (int64)(1ll<<25)  ///< XBAR Output pin 8
#define XBAR_ADCTRGCHC  (int64)(1ll<<26)  ///< ADC trigger select C pulse
#define XBAR_ADCTRGCHD  (int64)(1ll<<27)  ///< ADC trigger select D pulse
#define XBAR_AFE0TRG    (int64)(1ll<<28)  ///< AFE Channel 0 Trigger
#define XBAR_AFE1TRG    (int64)(1ll<<29)  ///< AFE Channel 1 Trigger
#define XBAR_AFE2TRG    (int64)(1ll<<30)  ///< AFE Channel 2 Trigger
#define XBAR_AFE3TRG    (int64)(1ll<<31)  ///< AFE Channel 3 Trigger
#define XBAR_EWMIN      (int64)(1ll<<32)  ///< EWM input (EWM_IN)
#define XBAR_OUT9       (int64)(1ll<<33)  ///< XBAR Output pin 9
#define XBAR_OUT10      (int64)(1ll<<34)  ///< XBAR Output pin 10
#define XBAR_PDBPTRG0A  (int64)(1ll<<35)  ///< PDB0 Pre-trigger0 B2B Ack
#define XBAR_PDBPTRG1A  (int64)(1ll<<36)  ///< PDB0 Pre-trigger1 B2B Ack
#define XBAR_PDBPTRG2A  (int64)(1ll<<37)  ///< PDB0 Pre-trigger2 B2B Ack
#define XBAR_PDBPTRG3A  (int64)(1ll<<38)  ///< PDB0 Pre-trigger3 B2B Ack
#define XBAR_PDBXBTRG   (int64)(1ll<<39)  ///< PDB0 Trigger Input14
#define XBAR_AFE0MDAT   (int64)(1ll<<40)  ///< AFE channel 0 external modulator data input
#define XBAR_AFE1MDAT   (int64)(1ll<<41)  ///< AFE channel 1 external modulator data input
#define XBAR_AFE2MDAT   (int64)(1ll<<42)  ///< AFE channel 2 external modulator data input
#define XBAR_AFE3MDAT   (int64)(1ll<<43)  ///< AFE channel 3 external modulator data input
/*! @} End of xbar_outputs                                                    */

/******************************************************************************
* XBAR default configurations used by XBAR_Init() function
*
*//*! @addtogroup xbar_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Edge detection on the respective XBAR_DMAx output channel disabled.
 *          Interrupt and DMA disabled on the peripheral level.
 * @details Edge detection on the respective XBAR_DMAx output channel disabled.
 *          Interrupt and DMA disabled on the peripheral level.
 * @showinitializer
 ******************************************************************************/
#define XBAR_MODULE_NO_EDGE_DETECT_CONFIG                                     \
(tXBAR){                                                                      \
/* CTRL   */  SET(XBAR_CTRL0_EDGE0(0x00))|CLR(XBAR_CTRL0_IEN0_MASK)|          \
/* .....  */  CLR(XBAR_CTRL0_DEN0_MASK),                                      \
}
/***************************************************************************//*!
 * @brief   Enabled rising edge detection on the respective XBAR_DMAx output
 *          channel. Interrupt enabled but DMA disabled on the peripheral level.
 * @details Enabled rising edge detection on the respective XBAR_DMAx output
 *          channel. Interrupt enabled but DMA disabled on the peripheral level.
 * @showinitializer
 ******************************************************************************/
#define XBAR_MODULE_POSEDGE_DETECT_CONFIG                                     \
(tXBAR){                                                                      \
/* CTRL   */  SET(XBAR_CTRL0_EDGE0(0x01))|SET(XBAR_CTRL0_IEN0_MASK)|          \
/* .....  */  CLR(XBAR_CTRL0_DEN0_MASK),                                      \
}
/***************************************************************************//*!
 * @brief   Enabled falling edge detection on the respective XBAR_DMAx output
 *          channel. Interrupt enabled but DMA disabled on the peripheral level.
 * @details Enabled falling edge detection on the respective XBAR_DMAx output
 *          channel. Interrupt enabled but DMA disabled on the peripheral level.
 * @showinitializer
 ******************************************************************************/
#define XBAR_MODULE_NEGEDGE_DETECT_CONFIG                                     \
(tXBAR){                                                                      \
/* CTRL   */  SET(XBAR_CTRL0_EDGE0(0x02))|SET(XBAR_CTRL0_IEN0_MASK)|          \
/* .....  */  CLR(XBAR_CTRL0_DEN0_MASK),                                      \
}
/***************************************************************************//*!
 * @brief   Enabled rising and falling edge detection on the respective
 *          XBAR_DMAx output channel. Interrupt enabled but DMA disabled on the
 *          peripheral level.
 * @details Enabled rising and falling edge detection on the respective
 *          XBAR_DMAx output channel. Interrupt enabled but DMA disabled on the
 *          peripheral level.
 * @showinitializer
 ******************************************************************************/
#define XBAR_MODULE_ANYEDGE_DETECT_CONFIG                                     \
(tXBAR){                                                                      \
/* CTRL   */  SET(XBAR_CTRL0_EDGE0(0x03))|SET(XBAR_CTRL0_IEN0_MASK)|          \
/* .....  */  CLR(XBAR_CTRL0_DEN0_MASK),                                      \
}
/*! @} End of xbar_config                                                     */

/******************************************************************************
* XBAR callback function's types
*
*//*! @addtogroup xbar_callback
* @{
*******************************************************************************/
/*! @brief XBAR_CALLBACK type declaration                                     */
typedef enum
{
    XBAR_DMA0_CALLBACK      =  0,  ///< XBAR_DMA0 interrupt
    XBAR_DMA1_CALLBACK      =  1,  ///< XBAR_DMA1 interrupt
    XBAR_DMA2_CALLBACK      =  2,  ///< XBAR_DMA2 interrupt
    XBAR_DMA3_CALLBACK      =  3   ///< XBAR_DMA3 interrupt
} XBAR_CALLBACK_SRC;
/*! @} End of xbar_callback                                                   */

/******************************************************************************
* XBAR callback registered by XBAR_Init() function
*
*//*! @addtogroup xbar_callback
* @{
*******************************************************************************/
/*! @brief XBAR_CALLBACK function declaration                                 */
typedef void (*XBAR_CALLBACK)(XBAR_CALLBACK_SRC src);
/*! @} End of xbar_callback                                                   */

/******************************************************************************
* XBAR function and macro definitions
*
*//*! @addtogroup xbar_macro
* @{
*******************************************************************************/
#define _XBAR_Path(inp,mask)                                                   \
{                                                                              \
  if (mask & XBAR_DMA0      ) iobfih (XBAR_SEL0 ,XBAR_SEL0_SEL0_SHIFT  ,6,inp);\
  if (mask & XBAR_DMA1      ) iobfih (XBAR_SEL0 ,XBAR_SEL0_SEL1_SHIFT  ,6,inp);\
  if (mask & XBAR_DMA2      ) iobfih (XBAR_SEL1 ,XBAR_SEL1_SEL2_SHIFT  ,6,inp);\
  if (mask & XBAR_DMA3      ) iobfih (XBAR_SEL1 ,XBAR_SEL1_SEL3_SHIFT  ,6,inp);\
  if (mask & XBAR_CMP2INP   ) iobfih (XBAR_SEL2 ,XBAR_SEL2_SEL4_SHIFT  ,6,inp);\
  if (mask & XBAR_TMR0SEC   ) iobfih (XBAR_SEL2 ,XBAR_SEL2_SEL5_SHIFT  ,6,inp);\
  if (mask & XBAR_TMR1SEC   ) iobfih (XBAR_SEL3 ,XBAR_SEL3_SEL6_SHIFT  ,6,inp);\
  if (mask & XBAR_TMR2SEC   ) iobfih (XBAR_SEL3 ,XBAR_SEL3_SEL7_SHIFT  ,6,inp);\
  if (mask & XBAR_TMR3SEC   ) iobfih (XBAR_SEL4 ,XBAR_SEL4_SEL8_SHIFT  ,6,inp);\
  if (mask & XBAR_TMRPRI1   ) iobfih (XBAR_SEL4 ,XBAR_SEL4_SEL9_SHIFT  ,6,inp);\
  if (mask & XBAR_TMRPRI2   ) iobfih (XBAR_SEL5 ,XBAR_SEL5_SEL10_SHIFT ,6,inp);\
  if (mask & XBAR_CMP0INP   ) iobfih (XBAR_SEL5 ,XBAR_SEL5_SEL11_SHIFT ,6,inp);\
  if (mask & XBAR_CMP1INP   ) iobfih (XBAR_SEL6 ,XBAR_SEL6_SEL12_SHIFT ,6,inp);\
  if (mask & XBAR_UARTRXINP ) iobfih (XBAR_SEL6 ,XBAR_SEL6_SEL13_SHIFT ,6,inp);\
  if (mask & XBAR_UARTTXMOD ) iobfih (XBAR_SEL7 ,XBAR_SEL7_SEL14_SHIFT ,6,inp);\
  if (mask & XBAR_ADCTRGCHA ) iobfih (XBAR_SEL7 ,XBAR_SEL7_SEL15_SHIFT ,6,inp);\
  if (mask & XBAR_ADCTRGCHB ) iobfih (XBAR_SEL8 ,XBAR_SEL8_SEL16_SHIFT ,6,inp);\
  if (mask & XBAR_OUT0      ) iobfih (XBAR_SEL8 ,XBAR_SEL8_SEL17_SHIFT ,6,inp);\
  if (mask & XBAR_OUT1      ) iobfih (XBAR_SEL9 ,XBAR_SEL9_SEL18_SHIFT ,6,inp);\
  if (mask & XBAR_OUT2      ) iobfih (XBAR_SEL9 ,XBAR_SEL9_SEL19_SHIFT ,6,inp);\
  if (mask & XBAR_OUT3      ) iobfih (XBAR_SEL10,XBAR_SEL10_SEL20_SHIFT,6,inp);\
  if (mask & XBAR_OUT4      ) iobfih (XBAR_SEL10,XBAR_SEL10_SEL21_SHIFT,6,inp);\
  if (mask & XBAR_OUT5      ) iobfih (XBAR_SEL11,XBAR_SEL11_SEL22_SHIFT,6,inp);\
  if (mask & XBAR_OUT6      ) iobfih (XBAR_SEL11,XBAR_SEL11_SEL23_SHIFT,6,inp);\
  if (mask & XBAR_OUT7      ) iobfih (XBAR_SEL12,XBAR_SEL12_SEL24_SHIFT,6,inp);\
  if (mask & XBAR_OUT8      ) iobfih (XBAR_SEL12,XBAR_SEL12_SEL25_SHIFT,6,inp);\
  if (mask & XBAR_ADCTRGCHC ) iobfih (XBAR_SEL13,XBAR_SEL13_SEL26_SHIFT,6,inp);\
  if (mask & XBAR_ADCTRGCHD ) iobfih (XBAR_SEL13,XBAR_SEL13_SEL27_SHIFT,6,inp);\
  if (mask & XBAR_AFE0TRG   ) iobfih (XBAR_SEL14,XBAR_SEL14_SEL28_SHIFT,6,inp);\
  if (mask & XBAR_AFE1TRG   ) iobfih (XBAR_SEL14,XBAR_SEL14_SEL29_SHIFT,6,inp);\
  if (mask & XBAR_AFE2TRG   ) iobfih (XBAR_SEL15,XBAR_SEL15_SEL30_SHIFT,6,inp);\
  if (mask & XBAR_AFE3TRG   ) iobfih (XBAR_SEL15,XBAR_SEL15_SEL31_SHIFT,6,inp);\
  if (mask & XBAR_EWMIN     ) iobfih (XBAR_SEL16,XBAR_SEL16_SEL32_SHIFT,6,inp);\
  if (mask & XBAR_OUT9      ) iobfih (XBAR_SEL16,XBAR_SEL16_SEL33_SHIFT,6,inp);\
  if (mask & XBAR_OUT10     ) iobfih (XBAR_SEL17,XBAR_SEL17_SEL34_SHIFT,6,inp);\
  if (mask & XBAR_PDBPTRG0A ) iobfih (XBAR_SEL17,XBAR_SEL17_SEL35_SHIFT,6,inp);\
  if (mask & XBAR_PDBPTRG1A ) iobfih (XBAR_SEL18,XBAR_SEL18_SEL36_SHIFT,6,inp);\
  if (mask & XBAR_PDBPTRG2A ) iobfih (XBAR_SEL18,XBAR_SEL18_SEL37_SHIFT,6,inp);\
  if (mask & XBAR_PDBPTRG3A ) iobfih (XBAR_SEL19,XBAR_SEL19_SEL38_SHIFT,6,inp);\
  if (mask & XBAR_PDBXBTRG  ) iobfih (XBAR_SEL19,XBAR_SEL19_SEL39_SHIFT,6,inp);\
  if (mask & XBAR_AFE0MDAT  ) iobfih (XBAR_SEL20,XBAR_SEL20_SEL40_SHIFT,6,inp);\
  if (mask & XBAR_AFE1MDAT  ) iobfih (XBAR_SEL20,XBAR_SEL20_SEL41_SHIFT,6,inp);\
  if (mask & XBAR_AFE2MDAT  ) iobfih (XBAR_SEL21,XBAR_SEL21_SEL42_SHIFT,6,inp);\
  if (mask & XBAR_AFE3MDAT  ) iobfih (XBAR_SEL21,XBAR_SEL21_SEL43_SHIFT,6,inp);\
}

/***************************************************************************//*!
 * @brief   Sets XBAR output channels.
 * @details This macro routes one of the following input channels @ref xbar_inputs
 *          to one or more the following @ref xbar_outputs.
 * @param   inp      Select one of the @ref xbar_inputs.
 * @param   outmask  Select one or more OR'ed @ref xbar_outputs.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define XBAR_Path(inp,outmask)  _XBAR_Path(inp,(outmask))

/***************************************************************************//*!
 * @brief   Enables DMA for XBAR_DMA output.
 * @details This macro enables the DMA function for XBAR_DMA output.
 *          Macros sets DEN (DMA enable bit) and clears IEN (interrupt
 *          enable bit) for selected outputs.
 * @param   outmask  Select one or more OR'ed XBAR_DMA @ref xbar_outputs.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define XBAR_EnableDMA(outmask)                                               \
{                                                                             \
  if (outmask & XBAR_DMA0 ){                                                  \
                            ioclrh (XBAR_CTRL0,XBAR_CTRL0_IEN0_MASK);         \
                            ioseth (XBAR_CTRL0,XBAR_CTRL0_DEN0_MASK);         \
                           }                                                  \
  if (outmask & XBAR_DMA1 ){                                                  \
                            ioclrh (XBAR_CTRL0,XBAR_CTRL0_IEN1_MASK);         \
                            ioseth (XBAR_CTRL0,XBAR_CTRL0_DEN1_MASK);         \
                           }                                                  \
  if (outmask & XBAR_DMA2 ){                                                  \
                            ioclrh (XBAR_CTRL1,XBAR_CTRL1_IEN2_MASK);         \
                            ioseth (XBAR_CTRL1,XBAR_CTRL1_DEN2_MASK);         \
                           }                                                  \
  if (outmask & XBAR_DMA3 ){                                                  \
                            ioclrh (XBAR_CTRL1,XBAR_CTRL1_IEN3_MASK);         \
                            ioseth (XBAR_CTRL1,XBAR_CTRL1_DEN3_MASK);         \
                           }                                                  \
}

/***************************************************************************//*!
 * @brief   XBAR initialization.
 * @details This function initializes Inter-Peripheral Crossbar Switch (XBAR)
 *          block. It also installs callback function for interrupt vector 47.
 * @param   cfg0     Select one of the @ref xbar_config for CH0.
 * @param   cfg1     Select one of the @ref xbar_config for CH1.
 * @param   cfg2     Select one of the @ref xbar_config for CH2.
 * @param   cfg3     Select one of the @ref xbar_config for CH3.
 * @param   ip       Select one of the @ref cm0plus_prilvl.
 * @param   callback Pointer to the @ref xbar_callback.
 * @note    Implemented as a function call.
 ******************************************************************************/
#define XBAR_Init(cfg0,cfg1,cfg2,cfg3,ip,callback)                            \
                      XBAR_Init(cfg0,cfg1,cfg2,cfg3,ip,callback)
/*! @} End of xbar_macro                                                      */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void XBAR_Init (tXBAR out0, tXBAR out1, tXBAR out2, tXBAR out3, uint16 ip,
                       XBAR_CALLBACK pCallback);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void xbar_isr (void);

#endif /* __XBAR_H */
