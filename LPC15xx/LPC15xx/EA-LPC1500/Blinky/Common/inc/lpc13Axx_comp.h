/*****************************************************************************
 *   comp.h:  Header file for NXP LPC8xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __COMP_H__
#define __COMP_H__

#define COMP_CH_NUM					4

#define COMP_VP							0
#define COMP_VM							1

// bit definitions, control register
#define CMP_VTEMP(d)        ((d) << 4)
#define CMP_TEMPSEN(d)      ((d) << 6)
#define CMP_ROSC0CTL        (1 << 8)
#define CMP_EXT_RESET       (1 << 9)
#define CMP_ROSC1CTL        (1 << 10)

// comparators
#define CMP_EN(d)           ((d) << 0)
#define CMP_OE              (1 << 2)
#define CMP_STAT            (1 << 3)
#define CMP_VM(d)           ((d) << 4)
#define CMP_VP(d)           ((d) << 8)
#define CMP_SYNC            (1 << 12)
#define CMP_HYS(d)          ((d) << 13)
#define CMP_INTPOL          (1 << 15)
#define CMP_INTTYPE         (1 << 16)
#define CMP_INTEDGE(d)      ((d) << 17)
#define CMP_INTFLAG         (1 << 19)
#define CMP_VLADEN(d)       ((d) << 20)
#define CMP_VLADREF         (1 << 22)
#define CMP_VSEL(d)         ((d) << 24)
#define CMP_DLY(d)          ((d) << 29)

// filters
#define CMP_FILTER_SAMPLE_MODE(d) ((d) << 0)
#define CMP_FILTER_CLKDIV(d)      ((d) << 2)

extern void COMP_Handler ( uint32_t channel_num );
extern void COMP_IOConfig( void );
extern void COMP_Init( void );
extern void COMP_SelectInput( uint32_t channel_num, uint32_t comp_channel, uint32_t input );
extern void COMP_LadderControl( uint32_t channel_num, uint32_t ladder_ctrl, uint32_t ref_sel, uint32_t ladder_val );
extern void COMP_SetOutput( uint32_t channel_num, uint32_t sync );
extern void COMP_SetInterrupt( uint32_t channel_num, uint32_t polarity, uint32_t int_type, uint32_t int_edge );
extern void COMP_SetHysteresis( uint32_t channel_num, uint32_t level );
extern void COMP_SetFilterDivider( uint32_t channel_num, uint32_t clkdiv );
extern uint32_t COMP_GetOutputStatus( uint32_t channel_num );

#endif  /* __COMP_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

