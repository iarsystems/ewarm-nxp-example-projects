/****************************************************************************
 *   $Id: lpc13Axx_adc.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx ADC example
 *
 *   Description:
 *     This file contains ADC code header definition.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#ifndef __ADC_H 
#define __ADC_H

#define ADC_INTERRUPT_FLAG    1	/* 1 is interrupt driven, 0 is polling */
#define BURST_MODE_ENA        1 /* Burst mode works in interrupt driven mode only. */
#define TENBIT_MODE_ENA       0	/* 10-bit mode */
#define LOWPWR_MODE_ENA       0	/* Low-power mode */
#define ASYNC_MODE_ENA        0	/* Async Mode */
#define HW_TRIGGER_ENA				0
#define THR_SEL								0
#define ADC_DEBUG             1
#define ADC_DEBUG_CLEAR_CNT   (100) /* Number of samples until statistics are cleared */
#define ADC_DEBUG_UPDATE_CNT  (5000) /* Number of samples until statistics are cleared */

#define SEQ_A									0
#define SEQ_B									1

typedef struct {
	uint32_t int_mode;
	uint32_t adc_clock;
	uint32_t hwtrig_mode;
	uint32_t async_mode;
	uint32_t tenbit_mode;
	uint32_t lpwr_mode;
  uint32_t input_sel;
	uint32_t seq_sel;
	uint32_t channels;
	uint32_t thr_sel;
	uint32_t trigger;
	uint32_t trig_pol;
	uint32_t sync_bypass;
	uint32_t burst_mode;
	uint32_t single_step;
	uint32_t seq_ctrl_mode;
} ADC_Config;


/* Hardware trigger mode */
#define ADC0_PIN_TRIG0					0
#define ADC0_PIN_TRIG1					1
#define SCT0_OUT7_TRIG					2
#define SCT0_OUT9_TRIG					3
#define SCT1_OUT7_TRIG					4
#define SCT1_OUT9_TRIG					5
#define SCT2_OUT3_TRIG					6
#define SCT2_OUT4_TRIG					7
#define SCT3_OUT3_TRIG					8
#define SCT3_OUT4_TRIG					9
#define COMP0_OUT_TRIG					10
#define COMP1_OUT_TRIG					11
#define COMP2_OUT_TRIG					12
#define COMP3_OUT_TRIG					13
#define RESERVED_TRIG						14
#define ARM_TXEV_TRIG 					15

/* ADC control register */
#define ADC_ASYNC_MODE				(0x1<<8)
#define ADC_10BIT_MODE				(0x1<<9)
#define ADC_LPWR_MODE 				(0x1<<10)
#define ADC_CAL_MODE	 				(0x1<<30)
#define ADC_ENA				 				(0x1UL<<31)

/* ADC input Select register */
#define INPUTSEL_PIN					(0x0<<0)
#define INPUTSEL_CORE_VOL			(0x1<<0)
#define INPUTSEL_INTERNAL_VOL	(0x2<<0)
#define INPUTSEL_TEMP_VOL			(0x3<<0)

/* Only if the IOCON for ADC0 configured as ADC output. */
#define WRAPEN(n)							(0x2UL<<30)

/* ADC SEQx Control register */
#define CHANNELS(n)						((n)<<0)
#define TRIGGER(n)						((n)<<12)
#define TRIG_POL							(0x1<<18)
#define SYNC_BYPASS						(0x1<<19)
#define ADC_START							(0x1<<26)
#define ADC_BURST							(0x1<<27)
#define ADC_SINGLE_STEP				(0x1<<28)
#define ADC_LOW_PRIORITY			(0x1<<29)
#define ADC_MODE_EACH_CHN			(0x0<<30)
#define ADC_MODE_ENTIRE_SEQ		(0x1<<30)
#define ADC_SEQ_ENA						(0x1UL<<31)

/* ADC SEQx Glabal Data register */
#define RESULT_OFFSET					4
#define THCMP_RANGE_OFFSET		16
#define THCMP_CROSS_OFFSET		18
#define CHN_OFFSET						26
#define ADC_OVERRUN						(0x1<<30)
#define ADC_DATAVALID					(0x1UL<<31)

/* ADC Interrupt Enable register definition. */
#define SEQA_ENA							(0x1<<0)
#define SEQB_ENA							(0x1<<1)
#define OVR_ENA					  		(0x1<<2)
#define ADCCMP_ENA0(n)			 	((n)<<3)
#define ADCCMP_ENA1(n)			 	((n)<<5)
#define ADCCMP_ENA2(n)			 	((n)<<7)
#define ADCCMP_ENA3(n)			 	((n)<<9)
#define ADCCMP_ENA4(n)			 	((n)<<11)
#define ADCCMP_ENA5(n)			 	((n)<<13)
#define ADCCMP_ENA6(n)			 	((n)<<15)
#define ADCCMP_ENA7(n)			 	((n)<<17)
#define ADCCMP_ENA8(n)			 	((n)<<19)
#define ADCCMP_ENA9(n)			 	((n)<<21)
#define ADCCMP_ENA10(n)			 	((n)<<23)
#define ADCCMP_ENA11(n)			 	((n)<<25)

/* ADC Flags register definition. */
#define FLAGS_THCMP(n)				(0x1<<(n))
#define FLAGS_OVERRUN(n)			(0x1<<(n+12))
#define SEQA_INT							(0x1<<28)
#define SEQB_INT							(0x1<<29)
#define THCMP_INT							(0x1<<30)
#define OVERRUN_INT						(0x1UL<<31)

#define THR0_LOW_VALUE				0
#define THR0_HIGH_VALUE				0x3FF

#define THR1_LOW_VALUE				0x10
#define THR1_HIGH_VALUE				0x3F0

#define ADC_OFFSET            0x20
#define ADC_INDEX             4

#define ADC_NUM               12			/* for LPC13Axx */
#define ADC_CALIB_CLK         500000		/* set to 0.5Mhz */
#define ADC_CLK               2000000		/* set to 2.0Mhz - assuming CPU running at 72MHz */

extern void ADC_SEQHandler( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr );
extern void ADC_OVR_IRQHandler( LPC_ADC_TypeDef *ADCx );
extern void ADCCalibration( void );
extern void ADCInit( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr );
extern uint32_t ADCRead( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr, uint8_t channelNum );
extern uint32_t ADCReadGlobal( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr, uint8_t channelNum );
extern void ADCBurstRead( LPC_ADC_TypeDef *ADCx, ADC_Config *ADCSetupPtr );
#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
