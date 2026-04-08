/**********************************************************************
* $Id: Qei_Velo.c 28 2014-01-27 14:45:13Z danielru $		Qei_Velo.c			2011-06-02
*//**
* @file		Qei_Velo.c
* @brief	This example used to test QEI driver in Quadrature mode with
* 			velocity calculation (RPM)
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
#include "lpc18xx_qei.h"
#include "lpc18xx_cgu.h"
#include "lpc18xx_timer.h"
#include "debug_frmwrk.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_libcfg.h"
#include "lpc18xx_evrt.h"
#include "lpc18xx_scu.h"


/* Example group ----------------------------------------------------------- */
/** @defgroup Qei_Velo	Qei_Velo
 * @ingroup QEI_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS *************************/
/** Define hardware */
#define BOARD_NXP_EA

/** In case of using QEI virtual signal, this macro below must be set to 1 */
#define VIRTUAL_QEI_SIGNAL	1

/** Signal Mode setting:
 * - When = 0, PhA and PhB function as quadrature encoder inputs.
 * - When = 1, PhA functions as the direction signal and PhB functions
 * as the clock signal
 */
#define SIGNAL_MODE 		0

/** Capture Mode setting:
 * - When = 0, only PhA edges are counted (2X).
 * - When = 1, BOTH PhA and PhB edges are counted (4X), increasing
 * resolution but decreasing range
 */
#define CAP_MODE 			1

/** Velocity capture period definition (in microsecond) */
#define CAP_PERIOD			250000UL

/** Delay time to Read Velocity Accumulator and display (in microsecond)*/
#define DISP_TIME			3000000UL

/** Max velocity capture times calculated */
#define MAX_CAP_TIMES		(DISP_TIME/CAP_PERIOD)

#define ENC_RES	 			2048UL	/**< Encoder resolution (PPR) */


/* In case of using Virtual QEI signal, these following macros must be defined */
#ifdef VIRTUAL_QEI_SIGNAL
/* Max velocity */
#define MAX_VEL  			600UL 	/**< Max velocity (RPM) */
#if CAP_MODE
#define COUNT_MODE			4
#else
#define COUNT_MODE			2
#endif

// GPIO port used for virtual QEI signal
#define VIRTUAL_QEI_GPIO_PORT	(4)
/* Pin assigned to Phase A */
#define PHASE_A_PIN			(1<<0)
/* Pin assigned to Phase B */
#define PHASE_B_PIN			(1<<1)

#endif

#define NUMSAMPLE_T0_REVERSE_DIR		(10)

/************************** PRIVATE VARIABLES *************************/
#ifdef VIRTUAL_QEI_SIGNAL
/** Phase Counter:
 * - 0: Phase A = 1, Phase B = 0
 * - 1: Phase A = 1, Phase B = 1
 * - 2: Phase A = 0, Phase B = 1
 * - 3: Phase A = 0, Phase B = 0
 */
__IO uint8_t PhaseCnt;

uint8_t revDir;
uint32_t sampleCnt;
#endif /* VIRTUAL_QEI_SIGNAL */

/** Velocity Accumulator */
__IO uint64_t VeloAcc;
/** Times of Velocity capture */
__IO uint32_t VeloCapCnt;
/** Flag indicates Times of Velocity capture is enough to read out */
__IO FlagStatus VeloAccFlag;


/************************** PRIVATE FUNCTIONS *************************/
#ifdef VIRTUAL_QEI_SIGNAL
void VirtualQEISignal_Init(void);
void TIMER0_IRQHandler(void);
#endif
void EVRT_IRQHandler(void);
void QEI_IRQHandler(void);


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
#ifdef VIRTUAL_QEI_SIGNAL
/*********************************************************************//**
 * @brief		Timer 0 interrupt handler. This sub-routine will set/clear
 * 				two Phase A-B output pin to their phase state
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TIMER0_IRQHandler(void)
{
	if (TIM_GetIntStatus(LPC_TIMER0,TIM_MR0_INT))
	{
		// Set/Clear phase A/B pin corresponding to their state
		switch (PhaseCnt)
		{
			case 0:
				GPIO_SetValue(VIRTUAL_QEI_GPIO_PORT,PHASE_A_PIN);
				GPIO_ClearValue(VIRTUAL_QEI_GPIO_PORT,PHASE_B_PIN);
				break;

			case 1:
				GPIO_SetValue(VIRTUAL_QEI_GPIO_PORT, PHASE_A_PIN | PHASE_B_PIN);
				break;

			case 2:
				GPIO_SetValue(VIRTUAL_QEI_GPIO_PORT, PHASE_B_PIN);
				GPIO_ClearValue(VIRTUAL_QEI_GPIO_PORT, PHASE_A_PIN);
				break;

			case 3:
				GPIO_ClearValue(VIRTUAL_QEI_GPIO_PORT, PHASE_A_PIN | PHASE_B_PIN);
				break;

			default:
				break;
		}

		if (revDir == 0)
		{
			// update PhaseCnt
			PhaseCnt = (PhaseCnt + 1) & 0x03;
		}
		else
		{
			PhaseCnt = (PhaseCnt - 1) & 0x03;
		}


		// Clear Timer 0 match interrupt pending
		TIM_ClearIntPending(LPC_TIMER0,TIM_MR0_INT);
	}
}
#endif
/*********************************************************************//**
 * @brief		QEI interrupt handler. This sub-routine will update current
 * 				value of captured velocity in to velocity accumulate.
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void QEI_IRQHandler(void)
{
	// Check whether if velocity timer overflow
	if (QEI_GetIntStatus(QEI_0, QEI_INTFLAG_TIM_Int) == SET)
	{
		if (VeloAccFlag == RESET)
		{
			// Get current velocity captured and update to accumulate
			VeloAcc += QEI_GetVelocityCap(QEI_0);

			// Update Velocity capture times
			VeloAccFlag = ((VeloCapCnt++) >= MAX_CAP_TIMES) ? SET : RESET;
		}
		// Reset Interrupt flag pending
		QEI_IntClear(QEI_0, QEI_INTFLAG_TIM_Int);
	}

	// Check whether if direction change occurred
	if (QEI_GetIntStatus(QEI_0, QEI_INTFLAG_DIR_Int) == SET)
	{
		// Print direction status
		_DBG("Direction has changed: ");

		_DBG_((QEI_GetStatus(QEI_0, QEI_STATUS_DIR) == SET) ? "1" : "0");

		// Reset Interrupt flag pending
		QEI_IntClear(QEI_0, QEI_INTFLAG_DIR_Int);
	}
}
/*********************************************************************//**
 * @brief		EVRT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER,EVRT_SRC_QEI))
	{
		QEI_IRQHandler();
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_QEI);
	}
}
/*-------------------------PRIVATE FUNCTIONS------------------------------*/
#ifdef VIRTUAL_QEI_SIGNAL
/*********************************************************************//**
 * @brief		Initializes signal supplying for QEI peripheral by using timer
 * 			match interrupt output, that will generate two virtual signal on
 * 			Phase-A and Phase-B. These two clock are 90 degrees out of phase.
 * 			In this case, a 'virtual encoder' that has these following parameter:
 * 			- Encoder type			: Quadrature encoder
 * 			- Max velocity			: MAX_VEL (Round Per Minute)
 * 			- Encoder Resolution	: ENC_RES (Pulse Per Round)
 * 			The calculated frequency is: Freq = (MAX_VEL * ENC_RES * COUNT_MODE) / 60 (Hz)
 * 			The timer therefore should be set to tick every cycle T = 1/Freq (second)
 * Figure:
 *           |-----|     |-----|
 * Phase A --|     |-----|     |-----
 *              |-----|     |-----|
 * Phase B -----|     |-----|     |--
 *
 *           |--|--|--|--|--|--|--|--
 *            T  T  T  T  T  T  T
 *
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void VirtualQEISignal_Init(void)
{
	uint32_t pclk;
	TIM_TIMERCFG_Type TimerConfig;
	TIM_MATCHCFG_Type TimerMatchConfig;

	_DBG_("Initializing Virtual QEI signal...");

	// Initialize timer 0, Prescale value in tick value option with tick value = 1
	TimerConfig.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TimerConfig.PrescaleValue	= 1;
	TIM_Init(LPC_TIMER0, TIM_TIMER_MODE,&TimerConfig);

	// Get actual peripheral clock of timer 0
	pclk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE);

	pclk = pclk / ((MAX_VEL * ENC_RES * COUNT_MODE) / 60 );

	// Set match for match channel 0
	TimerMatchConfig.MatchChannel = 0;
	TimerMatchConfig.MatchValue = pclk;
	TimerMatchConfig.IntOnMatch = ENABLE;
	TimerMatchConfig.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
	TimerMatchConfig.ResetOnMatch = ENABLE;
	TimerMatchConfig.StopOnMatch = DISABLE;
	TIM_ConfigMatch(LPC_TIMER0, &TimerMatchConfig);

	// Select GPIO function for PHA, PHB pins
#ifdef	BOARD_NXP_EA
	scu_pinmux(0x8 ,0 , MD_PDN, FUNC0); 	// P8.0 : GPIO4_0
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : GPIO4_1
#endif

	// Reconfigures GPIO for pin used as Phase A and Phase B output
	GPIO_SetDir(VIRTUAL_QEI_GPIO_PORT, PHASE_A_PIN | PHASE_B_PIN, 1);

	// Set default State after initializing
	GPIO_ClearValue(VIRTUAL_QEI_GPIO_PORT, PHASE_A_PIN | PHASE_B_PIN);

	// Reset Phase Counter
	PhaseCnt = 0;

	/* preemption = 1, sub-priority = 2 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x02<<3)|0x01));

	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);

	// To start timer 0
	TIM_Cmd(LPC_TIMER0,ENABLE);
}
#endif /* VIRTUAL_QEI_SIGNAL */

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main QEI program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	QEI_CFG_Type QEIConfig;
	QEI_RELOADCFG_Type ReloadConfig;
	uint32_t rpm, averageVelo;

	SystemInit();
	CGU_Init();

	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	revDir = 0;
	sampleCnt = 0;

	_DBG_("Hello QEI ...");
	_DBG("Speed will be sampled every each ");
	_DBD32(CAP_PERIOD);
	_DBG_(" us");
	_DBG_("This value will be accumulated to display as RPM after every each");
	_DBD32(DISP_TIME);
	_DBG_(" us");

	// Prepare IO Pins for QEI
#ifdef	BOARD_NXP_EA
	scu_pinmux(0xA ,1 , MD_PDN, FUNC1); 	// PA.1 : QEI_IDX
	scu_pinmux(0xA ,2 , MD_PDN, FUNC1); 	// PA.2 : QEI_PHB
	scu_pinmux(0xA ,3 , MD_PDN, FUNC1); 	// PA.3 : QEI_PHA
#endif

	/* Initialize QEI configuration structure to default value */
#if CAP_MODE
	QEIConfig.CaptureMode = QEI_CAPMODE_4X;
#else
	QEIConfig.CaptureMode = QEI_CAPMODE_2X;
#endif

	QEIConfig.DirectionInvert = QEI_DIRINV_NONE;
	QEIConfig.InvertIndex = QEI_INVINX_NONE;

#if SIGNAL_MODE
	QEIConfig.SignalMode = QEI_SIGNALMODE_CLKDIR;
#else
	QEIConfig.SignalMode = QEI_SIGNALMODE_QUAD;
#endif

	/* Initialize QEI peripheral with given configuration structure */
	QEI_Init(QEI_0, &QEIConfig);

	// Set timer reload value for  QEI that used to set velocity capture period
	ReloadConfig.ReloadOption = QEI_TIMERRELOAD_USVAL;
	ReloadConfig.ReloadValue = CAP_PERIOD;
	QEI_SetTimerReload(QEI_0, &ReloadConfig);

	// Reset VeloAccFlag
	VeloAccFlag = RESET;

	// Reset value of Acc and Acc count to default
	VeloAcc = 0;
	VeloCapCnt = 0;

	// Enable interrupt for velocity Timer overflow for capture velocity into Acc */
	QEI_IntCmd(QEI_0, QEI_INTFLAG_TIM_Int, ENABLE);

	// Enable interrupt for direction change */
	QEI_IntCmd(QEI_0, QEI_INTFLAG_DIR_Int, ENABLE);

	//////////////////Configure EVRT/////////////////////
	// Initiate EVRT
	EVRT_Init(LPC_EVENTROUTER);
	// Configure interrupt signal from QEI to EVRT
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_QEI,EVRT_SRC_ACTIVE_HIGH_LEVEL);
	// Enable interrupt signal from QEI to EVRT
	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_QEI,ENABLE);

#ifdef VIRTUAL_QEI_SIGNAL
	_DBG("Init the Virtual Signal");_DBG_("");

	// This used for generating virtual QEI signal
	VirtualQEISignal_Init();
#endif

    /* Disable EVRT interrupt */
    NVIC_DisableIRQ(EVENTROUTER_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x01<<3)|0x01));
    /* Enable QEI interrupt */
	NVIC_ClearPendingIRQ(EVENTROUTER_IRQn);
    NVIC_EnableIRQ(EVENTROUTER_IRQn);

	_DBG("Start the loop");_DBG_("");
	// Main loop
	while (1)
	{
		// Check VeloAccFlag continuously
		if (VeloAccFlag == SET)
		{
			_DBG("Checked the flag");_DBG_("");

			// Get Acc
			averageVelo = (uint32_t)(VeloAcc / VeloCapCnt);

			rpm = QEI_CalculateRPM(QEI_0, averageVelo, ENC_RES);

			// Disp the result
			_DBG("Sampling Speed: ");

			_DBD32(rpm);

			_DBG_(" RPM");

			// Reset VeloAccFlag
			VeloAccFlag = RESET;

			// Reset value of Acc and Acc count to default
			VeloAcc = 0;
			VeloCapCnt = 0;

			if(sampleCnt++ >= NUMSAMPLE_T0_REVERSE_DIR)
			{
				sampleCnt = 0;
				revDir = (revDir == 0)? 1 : 0;
			}
		}
	}
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif
/*
 * @}
 */
