/**********************************************************************
* $Id: SDRAM_Init.c 28 2014-01-27 14:45:13Z danielru $		SDRAM_Init.c		2011-06-02
*//**
* @file		SDRAM_Init.c
* @brief	This file contains functions used to initialize SDRAM
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
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
**********************************************************************/
#define BOARD_HITEX_LPC1800
#include "lpc18xx.h"
#include "lpc_types.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_timer.h"
#include "lpc18xx_cgu.h"

#ifdef BOARD_HITEX_LPC1800

/************************** PRIVATE DEFINITIONS *************************/
/* SDRAM Address Base for DYCS0*/
#define SDRAM_ADDR_BASE		0x28000000

/* SDRAM refresh time to 16 clock num */
#define EMC_SDRAM_REFRESH(freq,time)  \
  (((uint64_t)((uint64_t)time * freq)/16000000000ull)+1)

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************
 * @brief		Calculate EMC Clock from nano second
 * @param[in]	freq - frequency of EMC Clk
 * @param[in]	time - nano second
 * @return 		None
 **********************************************************************/
uint32_t NS2CLK(uint32_t freq,uint32_t time){
 return (((uint64_t)time*freq/1000000000));
}


/*********************************************************************
 * @brief		Init the EMC Controller to connect ex SDRAM
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SDRAM_Init () {
	uint32_t pclk, temp;
	uint64_t tmpclk;
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	/* Set up EMC pin */
	scu_pinmux(	1	,	0	,	2<<2	,	2	);	//A5
	scu_pinmux(	1	,	1	,	2<<2	,	2	);	//A6
	scu_pinmux(	1	,	2	,	2<<2	,	2	);	//A7
	scu_pinmux(	1	,	3	,	2<<2	,	3	);	//OE
	scu_pinmux(	1	,	5	,	2<<2	,	3	);	//CS0
	scu_pinmux(	1	,	6	,	2<<2	,	3	);	//WE
	scu_pinmux(	1	,	7	,	2<<2	,	3	);//D0
	scu_pinmux(	1	,	8	,	2<<2	,	3	);//D1
	scu_pinmux(	1	,	9	,	2<<2	,	3	);//D2
	scu_pinmux(	1	,	10	,	2<<2	,	3	);//D3
	scu_pinmux(	1	,	11	,	2<<2	,	3	);//D4
	scu_pinmux(	1	,	12	,	2<<2	,	3	);//D5
	scu_pinmux(	1	,	13	,	2<<2	,	3	);//D6
	scu_pinmux(	1	,	14	,	2<<2	,	3	);//D7
	scu_pinmux(	2	,	0	,	2<<2	,	2	);//A13
	scu_pinmux(	2	,	1	,	2<<2	,	2	);//A12
	scu_pinmux(	2	,	2	,	2<<2	,	2	);//A11
	scu_pinmux(	2	,	6	,	2<<2	,	2	);//A10
	scu_pinmux(	2	,	7	,	2<<2	,	3	);//A9
	scu_pinmux(	2	,	8	,	2<<2	,	3	);//A8
	scu_pinmux(	2	,	9	,	2<<2	,	3	);//A0
	scu_pinmux(	2	,	10	,	2<<2	,	3	);//A1
	scu_pinmux(	2	,	11	,	2<<2	,	3	);//A2
	scu_pinmux(	2	,	12	,	2<<2	,	3	);//A3
	scu_pinmux(	2	,	13	,	2<<2	,	3	);//A4
	scu_pinmux(	5	,	0	,	2<<2	,	2	);//D12
	scu_pinmux(	5	,	1	,	2<<2	,	2	);//D13
	scu_pinmux(	5	,	2	,	2<<2	,	2	);//D14
	scu_pinmux(	5	,	3	,	2<<2	,	2	);//D15
	scu_pinmux(	5	,	4	,	2<<2	,	2	);//D8
	scu_pinmux(	5	,	5	,	2<<2	,	2	);//D9
	scu_pinmux(	5	,	6	,	2<<2	,	2	);//D10
	scu_pinmux(	5	,	7	,	2<<2	,	2	);//D11
	scu_pinmux(	6	,	3	,	2<<2	,	3	);//CS1
	scu_pinmux(	6	,	4	,	2<<2	,	3	);//CAS
	scu_pinmux(	6	,	5	,	2<<2	,	3	);//RAS
	scu_pinmux(	6	,	7	,	2<<2	,	1	);//A15
	scu_pinmux(	6	,	8	,	2<<2	,	1	);//A14
	scu_pinmux(	6	,	9	,	2<<2	,	3	);//DYCS0
	scu_pinmux(	6	,	10	,	2<<2	,	3	);//DQMOUT1
	scu_pinmux(	6	,	11	,	2<<2	,	3	);//CKEOUT0
	scu_pinmux(	6	,	12	,	2<<2	,	3	);//DQMOUT0
	scu_pinmux(	10	,	4	,	2<<2	,	3	);
	scu_pinmux(	13	,	0	,	2<<2	,	2	);
	scu_pinmux(	13	,	2	,	2<<2	,	2	);
	scu_pinmux(	13	,	3	,	2<<2	,	2	);
	scu_pinmux(	13	,	4	,	2<<2	,	2	);
	scu_pinmux(	13	,	5	,	2<<2	,	2	);
	scu_pinmux(	13	,	6	,	2<<2	,	2	);
	scu_pinmux(	13	,	7	,	2<<2	,	2	);
	scu_pinmux(	13	,	8	,	2<<2	,	2	);
	scu_pinmux(	13	,	9	,	2<<2	,	2	);
	scu_pinmux(	13	,	12	,	2<<2	,	2	);
	scu_pinmux(	13	,	15	,	2<<2	,	2	);
	scu_pinmux(	13	,	16	,	2<<2	,	2	);
	scu_pinmux(	14	,	0	,	2<<2	,	3	);
	scu_pinmux(	14	,	1	,	2<<2	,	3	);
	scu_pinmux(	14	,	2	,	2<<2	,	3	);
	scu_pinmux(	14	,	3	,	2<<2	,	3	);
	scu_pinmux(	14	,	4	,	2<<2	,	3	);
	scu_pinmux(	14	,	5	,	2<<2	,	3	);
	scu_pinmux(	14	,	6	,	2<<2	,	3	);
	scu_pinmux(	14	,	7	,	2<<2	,	3	);
	scu_pinmux(	14	,	8	,	2<<2	,	3	);
	scu_pinmux(	14	,	9	,	2<<2	,	3	);
	scu_pinmux(	14	,	10	,	2<<2	,	3	);
	scu_pinmux(	14	,	11	,	2<<2	,	3	);
	scu_pinmux(	14	,	12	,	2<<2	,	3	);
	scu_pinmux(	14	,	13	,	2<<2	,	3	);

	/* Select EMC clock-out */
	LPC_SCU->SFSCLK_0 = 0 | (0x3<<2);

	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIMER0, TIM_TIMER_MODE,&TIM_ConfigStruct);

	LPC_EMC->CONTROL 	= 0x00000001;
	LPC_EMC->CONFIG  	= 0x00000000;
	LPC_EMC->DYNAMICCONFIG0    = 0<<14 | 1<<9 | 1<<7; /* 64Mb, 4Mx16, 4 banks, row=12, column=8 */

	pclk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE);

	LPC_EMC->DYNAMICRASCAS0    = 0x00000303; /* 1 RAS, 3 CAS latency */
	LPC_EMC->DYNAMICREADCONFIG = 0x00000001; /* Command delayed strategy, using EMCCLKDELAY */

	LPC_EMC->DYNAMICRP         = NS2CLK(pclk, 20);
	LPC_EMC->DYNAMICRAS        = NS2CLK(pclk, 42);
	LPC_EMC->DYNAMICSREX       = NS2CLK(pclk, 63);
	LPC_EMC->DYNAMICAPR        = 0x00000005;
	LPC_EMC->DYNAMICDAL        = 0x00000005;
	LPC_EMC->DYNAMICWR         = 2;
	LPC_EMC->DYNAMICRC         = NS2CLK(pclk, 63);
	LPC_EMC->DYNAMICRFC        = NS2CLK(pclk, 63);
	LPC_EMC->DYNAMICXSR        = NS2CLK(pclk, 63);
	LPC_EMC->DYNAMICRRD        = NS2CLK(pclk, 14);
	LPC_EMC->DYNAMICMRD        = 0x00000002;

	TIM_Waitus(100);						   /* wait 100ms */
	LPC_EMC->DYNAMICCONTROL    = 0x00000183; /* Issue NOP command */

	TIM_Waitus(200);						   /* wait 200ms */
	LPC_EMC->DYNAMICCONTROL    = 0x00000103; /* Issue PALL command */

	LPC_EMC->DYNAMICREFRESH    = EMC_SDRAM_REFRESH(pclk,70); /* ( n * 16 ) -> 32 clock cycles */

	//for(i = 0; i < 0x80; i++);	           /* wait 128 AHB clock cycles */
	TIM_Waitus(200);						   /* wait 200ms */

	tmpclk = (uint64_t)15625*(uint64_t)pclk/1000000000/16;
	LPC_EMC->DYNAMICREFRESH    = tmpclk; /* ( n * 16 ) -> 736 clock cycles -> 15.330uS at 48MHz <= 15.625uS ( 64ms / 4096 row ) */

	LPC_EMC->DYNAMICCONTROL    = 0x00000083; /* Issue MODE command */

	//Timing for 48/60/72MHZ Bus
	temp = *((volatile uint32_t *)(SDRAM_ADDR_BASE | (3<<4| 3)<<11)); /* 4 burst, 3 CAS latency */
	temp = temp;
	LPC_EMC->DYNAMICCONTROL    = 0x00000000; /* Issue NORMAL command */

	//[re]enable buffers
	LPC_EMC->DYNAMICCONFIG0    |= 1<<19;

}
#endif

#ifdef BOARD_NXP_EA

/************************** PRIVATE DEFINITIONS *************************/
/* SDRAM Address Base for DYCS0*/
#define SDRAM_ADDR_BASE		0x28000000

/* SDRAM refresh time to 16 clock num */
#define EMC_SDRAM_REFRESH(freq,time)  \
  (((uint64_t)((uint64_t)time * freq)/16000000000ull)+1)

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************
 * @brief		Calculate EMC Clock from nano second
 * @param[in]	freq - frequency of EMC Clk
 * @param[in]	time - nano second
 * @return 		None
 **********************************************************************/
uint32_t NS2CLK(uint32_t freq,uint32_t time){
 return (((uint64_t)time*freq/1000000000));
}


/*********************************************************************
 * @brief		Init the EMC Controller to connect ex SDRAM
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SDRAM_Init () {
	uint32_t pclk, i, temp;
	uint64_t tmpclk;
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	/* Set up EMC pin */
	scu_pinmux(	1	,	0	,	2<<2	,	2	);	//A5
	scu_pinmux(	1	,	1	,	2<<2	,	2	);	//A6
	scu_pinmux(	1	,	2	,	2<<2	,	2	);	//A7
	scu_pinmux(	1	,	3	,	2<<2	,	3	);	//OE
	scu_pinmux(	1	,	5	,	2<<2	,	3	);	//CS0
	scu_pinmux(	1	,	6	,	2<<2	,	3	);	//WE
	scu_pinmux(	1	,	7	,	2<<2	,	3	);//D0
	scu_pinmux(	1	,	8	,	2<<2	,	3	);//D1
	scu_pinmux(	1	,	9	,	2<<2	,	3	);//D2
	scu_pinmux(	1	,	10	,	2<<2	,	3	);//D3
	scu_pinmux(	1	,	11	,	2<<2	,	3	);//D4
	scu_pinmux(	1	,	12	,	2<<2	,	3	);//D5
	scu_pinmux(	1	,	13	,	2<<2	,	3	);//D6
	scu_pinmux(	1	,	14	,	2<<2	,	3	);//D7
	scu_pinmux(	2	,	0	,	2<<2	,	2	);//A13
	scu_pinmux(	2	,	1	,	2<<2	,	2	);//A12
	scu_pinmux(	2	,	2	,	2<<2	,	2	);//A11
	scu_pinmux(	2	,	6	,	2<<2	,	2	);//A10
	scu_pinmux(	2	,	7	,	2<<2	,	3	);//A9
	scu_pinmux(	2	,	8	,	2<<2	,	3	);//A8
	scu_pinmux(	2	,	9	,	2<<2	,	3	);//A0
	scu_pinmux(	2	,	10	,	2<<2	,	3	);//A1
	scu_pinmux(	2	,	11	,	2<<2	,	3	);//A2
	scu_pinmux(	2	,	12	,	2<<2	,	3	);//A3
	scu_pinmux(	2	,	13	,	2<<2	,	3	);//A4
	scu_pinmux(	5	,	0	,	2<<2	,	2	);//D12
	scu_pinmux(	5	,	1	,	2<<2	,	2	);//D13
	scu_pinmux(	5	,	2	,	2<<2	,	2	);//D14
	scu_pinmux(	5	,	3	,	2<<2	,	2	);//D15
	scu_pinmux(	5	,	4	,	2<<2	,	2	);//D8
	scu_pinmux(	5	,	5	,	2<<2	,	2	);//D9
	scu_pinmux(	5	,	6	,	2<<2	,	2	);//D10
	scu_pinmux(	5	,	7	,	2<<2	,	2	);//D11
	scu_pinmux(	6	,	3	,	2<<2	,	3	);//CS1
	scu_pinmux(	6	,	4	,	2<<2	,	3	);//CAS
	scu_pinmux(	6	,	5	,	2<<2	,	3	);//RAS
	scu_pinmux(	6	,	7	,	2<<2	,	1	);//A15
	scu_pinmux(	6	,	8	,	2<<2	,	1	);//A14
	scu_pinmux(	6	,	9	,	2<<2	,	3	);//DYCS0
	scu_pinmux(	6	,	10	,	2<<2	,	3	);//DQMOUT1
	scu_pinmux(	6	,	11	,	2<<2	,	3	);//CKEOUT0
	scu_pinmux(	6	,	12	,	2<<2	,	3	);//DQMOUT0
	scu_pinmux(	10	,	4	,	2<<2	,	3	);
	scu_pinmux(	13	,	0	,	2<<2	,	2	);
	scu_pinmux(	13	,	2	,	2<<2	,	2	);
	scu_pinmux(	13	,	3	,	2<<2	,	2	);
	scu_pinmux(	13	,	4	,	2<<2	,	2	);
	scu_pinmux(	13	,	5	,	2<<2	,	2	);
	scu_pinmux(	13	,	6	,	2<<2	,	2	);
	scu_pinmux(	13	,	7	,	2<<2	,	2	);
	scu_pinmux(	13	,	8	,	2<<2	,	2	);
	scu_pinmux(	13	,	9	,	2<<2	,	2	);
	scu_pinmux(	13	,	12	,	2<<2	,	2	);
	scu_pinmux(	13	,	15	,	2<<2	,	2	);
	scu_pinmux(	13	,	16	,	2<<2	,	2	);
	scu_pinmux(	14	,	0	,	2<<2	,	3	);
	scu_pinmux(	14	,	1	,	2<<2	,	3	);
	scu_pinmux(	14	,	2	,	2<<2	,	3	);
	scu_pinmux(	14	,	3	,	2<<2	,	3	);
	scu_pinmux(	14	,	4	,	2<<2	,	3	);
	scu_pinmux(	14	,	5	,	2<<2	,	3	);
	scu_pinmux(	14	,	6	,	2<<2	,	3	);
	scu_pinmux(	14	,	7	,	2<<2	,	3	);
	scu_pinmux(	14	,	8	,	2<<2	,	3	);
	scu_pinmux(	14	,	9	,	2<<2	,	3	);
	scu_pinmux(	14	,	10	,	2<<2	,	3	);
	scu_pinmux(	14	,	11	,	2<<2	,	3	);
	scu_pinmux(	14	,	12	,	2<<2	,	3	);
	scu_pinmux(	14	,	13	,	2<<2	,	3	);

	/* Select EMC clock-out */
	LPC_SCU->SFSCLK_0 = 0 | (0x3<<2);

	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIMER0, TIM_TIMER_MODE,&TIM_ConfigStruct);

	LPC_EMC->CONTROL 	= 0x00000001;
	LPC_EMC->CONFIG  	= 0x00000000;
	LPC_EMC->DYNAMICCONFIG0    = 1<<14 | 2<<9 | 2<<7; /* 128MB, 4Mx32, 4 banks, row=12, column=8 */

	pclk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE);

	LPC_EMC->DYNAMICRASCAS0    = 0x00000301; /* 1 RAS, 3 CAS latency */
	LPC_EMC->DYNAMICREADCONFIG = 0x00000001; /* Command delayed strategy, using EMCCLKDELAY */

	LPC_EMC->DYNAMICRP         = NS2CLK(pclk, 20);
	LPC_EMC->DYNAMICRAS        = NS2CLK(pclk, 42);
	LPC_EMC->DYNAMICSREX       = NS2CLK(pclk, 70);
	LPC_EMC->DYNAMICAPR        = 0x00000004;
	LPC_EMC->DYNAMICDAL        = 0x00000004;
	LPC_EMC->DYNAMICWR         = NS2CLK(pclk, 14);
	LPC_EMC->DYNAMICRC         = NS2CLK(pclk, 70);
	LPC_EMC->DYNAMICRFC        = NS2CLK(pclk, 70);
	LPC_EMC->DYNAMICXSR        = NS2CLK(pclk, 70);
	LPC_EMC->DYNAMICRRD        = NS2CLK(pclk, 14);
	LPC_EMC->DYNAMICMRD        = 0x00000001;

	TIM_Waitus(100);						   /* wait 100ms */
	LPC_EMC->DYNAMICCONTROL    = 0x00000183; /* Issue NOP command */

	TIM_Waitus(200);						   /* wait 200ms */
	LPC_EMC->DYNAMICCONTROL    = 0x00000103; /* Issue PALL command */

	LPC_EMC->DYNAMICREFRESH    = EMC_SDRAM_REFRESH(pclk,70); /* ( n * 16 ) -> 32 clock cycles */

	//for(i = 0; i < 0x80; i++);	           /* wait 128 AHB clock cycles */
	TIM_Waitus(200);						   /* wait 200ms */

	tmpclk = (uint64_t)15625*(uint64_t)pclk/1000000000/16;
	LPC_EMC->DYNAMICREFRESH    = tmpclk; /* ( n * 16 ) -> 736 clock cycles -> 15.330uS at 48MHz <= 15.625uS ( 64ms / 4096 row ) */

	LPC_EMC->DYNAMICCONTROL    = 0x00000083; /* Issue MODE command */

	//Timing for 48/60/72MHZ Bus
	temp = *((volatile uint32_t *)(SDRAM_ADDR_BASE | (3<<4| 2)<<12)); /* 4 burst, 3 CAS latency */
	LPC_EMC->DYNAMICCONTROL    = 0x00000000; /* Issue NORMAL command */

	//[re]enable buffers
	LPC_EMC->DYNAMICCONFIG0    |= 1<<19;

}
#endif
