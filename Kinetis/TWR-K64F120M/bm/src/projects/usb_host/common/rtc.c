/*
 * Copyright (c) 2010 Freescale Semiconductor;
 * All Rights Reserved
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "psptypes.h"
#include "usb.h"
#include "rtc.h"
#include "host_common.h"

static int_16 _bsp_timer_sw_prescaller;
static int_16 _bsp_timer_sw_prescaller_cnt;

void TimerInit(void);
void Timer_ISR(void);
static void EnableTimerInterrupt(void);
static void DisableTimerInterrupt(void);

static TICK_STRUCT time_ticks;

uint_32 delay_count;
/*
 * Function Name	: TimerInit
 * Returned Value	:
 * Comments			: Initialize timer module
 */
void TimerInit(void)
{
	/* Enable LPT Module Clock */
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	/* Configure LPT */

	/* Enable LPT Interrupt in NVIC */
	NVICICPR2 |= (1 << 21);	/* Clear any pending interrupts on LPT */
	NVICISER2 |= (1 << 21);	/* Enable interrupts from LPT module */

#if 0
	RTCSC_RTIF = 0x01;		/* Clear previous RTC Interrupt */
	RTCMOD = 0x0B;			/* 1 ms Interrupt Generation */
	/*  Start RTC by Reseting Counter to 0 */
	RTCSC = 0x08;		/* Prescaler = 1000 Clock = 12MHz (external clock ) */
	//RTCSC = 0xA8; /* Prescaler = 1000 Clock = 12MHz (external clock ) */
#endif

	EnableTimerInterrupt();
}

/*
 * Function Name	: EnableTimerInterrupt
 * Returned Value	:
 * Comments			: Enable timer interrupt
 */
static void EnableTimerInterrupt(void)
{
	/* Enable Timer Interrupt */
	//RTCSC_RTIE = 0x01;
	return;
}

/*
 * Function Name	: DisableTimerInterrupt
 * Returned Value	:
 * Comments			: Disable timer interrupt.
 */
static void DisableTimerInterrupt(void)
{
	/* Disable Timer Interrupt */
	//RTCSC_RTIE = 0x00;

	return;
}

/*
 * Function Name	: Timer_ISR
 * Returned Value	:
 * Comments			: Timer interrupt service routine
 */
void Timer_ISR(void)
{
	if (delay_count > 0)
		delay_count--;
}

/*
 * Function Name	: time_delay
 * Returned Value	:
 * Comments			: Wait until interrupt of timer occur
 */
void time_delay(uint_32 delay)
{
	uint_32 u32Delay;
	uint_32 u32Delay2;

#if 0
	delay_count = delay;
	//RTCMOD = 0x0B; /* Reset RTCCNT */
	while(1) {
		if(0 == delay_count) {
			break;
		}
	}
#endif

	for (u32Delay = 0; u32Delay < delay; u32Delay++);
	for (u32Delay2 = 0; u32Delay2 < 0xFFFF; u32Delay2++);
}

/*
 * Function Name	: get_hw_ticks
 * Returned Value	: hardware tick per prescaler.
 * Comments			:
 */
uint_32 get_hw_ticks(void)
{
/*
	uint_32 ticks;

	ticks = (uint_16)RTCCNT;

	if ((RTCSC & RTCSC_RTIF_MASK) != 0) {
		ticks = (uint_16)RTCMOD + (uint_16)RTCCNT;
	}

	return ticks;
*/
	return (100);
}
