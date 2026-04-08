/******************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      LCDHDW.h
 * @version   1.0.2.0
 * @date      Jan-26-2015
 * @brief     LCD driver configuration file for LCD type GDH-1247WP
 *           (used on Freescale TWR-KM34Z75M board)
 ******************************************************************************/
#ifndef __LCDHWD_H
#define __LCDHWD_H

#define  DIM(x)                (sizeof(x)/sizeof(x[0]))

#define _LCD_CHARNUM          (9)      ///< number of LCD segment
#define _LCDTYPE              (1)      ///< 1 byte to complete one character

#define  CharacterPlace(LCDn)   *((unsigned char *)&LCD_WF3TO0 + LCDn)

/***************************************************************************//*!
 * @brief   Turns the LCD symbol ON
 * @details Turns the LCD symbol ON, by enabling the proper FP and BP pins
 * @param   LCDn    MCU LCDxx pin number (0..64) used as FrontPlane for segment
 * @param   bplane  MCU BackPlane (0..8) used by segment
 * @note    Implemented as an inline macro.
 * @see     @ref SymbolOFF, @ref SymbolOFF
 ******************************************************************************/
#define SymbolON(LCDn, bplane)                                                 \
{                                                                              \
  *((unsigned char *)&LCD_WF3TO0 + LCDn) |= (1<<(bplane));                     \
}

/***************************************************************************//*!
 * @brief   Turns the LCD symbol ON
 * @details Turns the LCD symbol ON, by enabling the proper FP and BP pins
 * @param   LCDn    MCU LCDxx pin number (0..64) used as FrontPlane for segment
 * @param   bplane  MCU BackPlane (0..8) used by segment
 * @note    Implemented as an inline macro.
 * @see     @ref SymbolON
 ******************************************************************************/
#define SymbolOFF(LCDn, bplane)                                                \
{                                                                              \
  *((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(1<<(bplane));                    \
}

/***************************************************************************//*!
 * @brief   Turns the LCD symbol ON
 * @details Turns the LCD symbol ON, by enabling the proper FP and BP pins
 * @param   LCDn    MCU LCDxx pin number (0..64) used as FrontPlane for segment
 * @param   mask (0x0..0xf) select BackPlanes used by segments
 * @note    Implemented as an inline macro.
 * @see     @ref SegmentsOFF
 ******************************************************************************/
#define SegmentsON(LCDn, mask)                                                 \
{                                                                              \
  *((unsigned char *)&LCD_WF3TO0 + LCDn) |= (mask);                            \
}

/***************************************************************************//*!
 * @brief   Turns the LCD symbol ON
 * @details Turns the LCD symbol ON, by enabling the proper FP and BP pins
 * @param   LCDn    MCU LCDxx pin number (0..64) used as FrontPlane for segment
 * @param   mask (0x0..0xf) select BackPlanes used by segments
 * @note    Implemented as an inline macro.
 * @see     @ref SegmentsON
 ******************************************************************************/
#define SegmentsOFF(LCDn, mask)                                                \
{                                                                              \
  *((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(mask);                           \
}

/* LCD Segment characters connected to MCU LCDn pin                           */
#define   Char0     CharacterPlace(37)
#define   Char1     CharacterPlace(35)
#define   Char2     CharacterPlace(33)
#define   Char3     CharacterPlace(50)
#define   Char4     CharacterPlace(30)
#define   Char5     CharacterPlace(45)
#define   Char6     CharacterPlace(24)
#define   Char7     CharacterPlace(26)
#define   Char8     CharacterPlace(28)

/* Special symbols ON                                                         */

#define _LCD_CHAR_V_S3_ON     SymbolON(23,0)   ///< Segment ON

#define _LCD_CHAR_V_S6_ON     SymbolON(23,5)   ///< Segment ON

#define _LCD_CHAR_V_S7_ON     SymbolON(25,1)   ///< Segment ON

#define _LCD_CHAR_V_S9_ON     SymbolON(23,4)   ///< Segment ON

#define _LCD_CHAR_V_S11_ON    SymbolON(25,4)   ///< Segment ON

#define _LCD_CHAR_V_S12_ON    SymbolON(25,2)   ///< Segment ON

#define _LCD_CHAR_V_S13_ON    SymbolON(25,3)   ///< Segment ON

#define _LCD_DOT_P3_ON        SymbolON(44,1)   ///< Segment ON

#define _LCD_SYM_T1_ON        SymbolON(59,2    ///< Segment ON

#define _LCD_SYM_T2_ON        SymbolON(59,3)   ///< Segment ON

#define _LCD_SYM_T3_ON        SymbolON(59,4)   ///< Segment ON

#define _LCD_SYM_T4_ON        SymbolON(59,5)   ///< Segment ON

/* Special symbols OFF                                                        */

#define _LCD_CHAR_V_S3_OFF    SymbolOFF(23,0)  ///< Segment OFF

#define _LCD_CHAR_V_S6_OFF    SymbolOFF(23,5)  ///< Segment OFF

#define _LCD_CHAR_V_S7_OFF    SymbolOFF(25,1)  ///< Segment OFF

#define _LCD_CHAR_V_S9_OFF    SymbolOFF(23,4)  ///< Segment OFF

#define _LCD_CHAR_V_S11_OFF   SymbolOFF(25,4)  ///< Segment OFF

#define _LCD_CHAR_V_S12_OFF   SymbolOFF(25,2)  ///< Segment OFF

#define _LCD_CHAR_V_S13_OFF   SymbolOFF(25,3)  ///< Segment OFF

#define _LCD_DOT_P3_OFF       SymbolOFF(44,1)  ///< Segment OFF

#define _LCD_SYM_T1_OFF       SymbolOFF(59,2)  ///< Segment OFF

#define _LCD_SYM_T2_OFF       SymbolOFF(59,3)  ///< Segment OFF

#define _LCD_SYM_T3_OFF       SymbolOFF(59,4)  ///< Segment OFF

#define _LCD_SYM_T4_OFF       SymbolOFF(59,5)  ///< Segment OFF

/* Segments ON                                                                */
#define Segment38_ON    SegmentsON ( 38,0x0f )
#define Segment37_ON    SegmentsON ( 37,0x0f )
#define Segment36_ON    SegmentsON ( 36,0x0f )
#define Segment35_ON    SegmentsON ( 35,0x0f )
#define Segment34_ON    SegmentsON ( 34,0x0f )
#define Segment33_ON    SegmentsON ( 33,0x0f )
#define Segment32_ON    SegmentsON ( 32,0x0f )
#define Segment31_ON    SegmentsON ( 31,0x0f )
#define Segment30_ON    SegmentsON ( 30,0x0f )
#define Segment29_ON    SegmentsON ( 29,0x0f )
#define Segment28_ON    SegmentsON ( 28,0x0f )
#define Segment27_ON    SegmentsON ( 27,0x0f )
#define Segment26_ON    SegmentsON ( 26,0x0f )
#define Segment25_ON    SegmentsON ( 25,0x0f )
#define Segment24_ON    SegmentsON ( 24,0x0f )
#define Segment23_ON    SegmentsON ( 23,0x0f )
#define Segment22_ON    SegmentsON ( 22,0x0f )
#define Segment21_ON    SegmentsON ( 21,0x0f )
#define Segment20_ON    SegmentsON ( 20,0x0f )
#define Segment19_ON    SegmentsON ( 19,0x0f )
#define Segment18_ON    SegmentsON ( 18,0x0f )
#define Segment17_ON    SegmentsON ( 17,0x0f )

/* Segments OFF                                                               */
#define Segment38_OFF    SegmentsOFF(38,0x0f )
#define Segment37_OFF    SegmentsOFF(37,0x0f )
#define Segment36_OFF    SegmentsOFF(36,0x0f )
#define Segment35_OFF    SegmentsOFF(35,0x0f )
#define Segment34_OFF    SegmentsOFF(34,0x0f )
#define Segment33_OFF    SegmentsOFF(33,0x0f )
#define Segment32_OFF    SegmentsOFF(32,0x0f )
#define Segment31_OFF    SegmentsOFF(31,0x0f )
#define Segment30_OFF    SegmentsOFF(30,0x0f )
#define Segment29_OFF    SegmentsOFF(29,0x0f )
#define Segment28_OFF    SegmentsOFF(28,0x0f )
#define Segment27_OFF    SegmentsOFF(27,0x0f )
#define Segment26_OFF    SegmentsOFF(26,0x0f )
#define Segment25_OFF    SegmentsOFF(25,0x0f )
#define Segment24_OFF    SegmentsOFF(24,0x0f )
#define Segment23_OFF    SegmentsOFF(23,0x0f )
#define Segment22_OFF    SegmentsOFF(22,0x0f )
#define Segment21_OFF    SegmentsOFF(21,0x0f )
#define Segment20_OFF    SegmentsOFF(20,0x0f )
#define Segment19_OFF    SegmentsOFF(19,0x0f )
#define Segment18_OFF    SegmentsOFF(18,0x0f )
#define Segment17_OFF    SegmentsOFF(17,0x0f )

/* Map segment to COM mask - LINE0                                            */
#define  L0SEGA   0x02
#define  L0SEGB   0x08
#define  L0SEGC   0x40
#define  L0SEGD   0x80
#define  L0SEGE   0x20
#define  L0SEGF   0x04
#define  L0SEGG   0x10

#endif /* __LCDHWD_H */
