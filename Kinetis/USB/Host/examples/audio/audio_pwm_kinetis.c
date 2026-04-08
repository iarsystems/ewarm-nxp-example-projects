/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
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
 *
 **************************************************************************//*!
 *
 * @file audio_pwm_kinetis.c$
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief   This file configures Pulse Width Modulation (PWM) for Timer
 *          Implementation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <string.h>
#include "derivative.h"     /* include peripheral declarations */
#include "types.h"          /* Contains User Defined Data Types */
#include "audio_pwm.h"

void pwm_init(void);
volatile uint32_t *duty;

/******************************************************************************
* Function Name    : TimerInit
* Returned Value   :
* Comments         : Initialize timer module
*
******************************************************************************/
void pwm_init(void)
{
	duty = &(FTM0_C0V);
	/* FTM0_CH0 enable on PTA3 ****/
	PORTC_PCR1 = PORT_PCR_MUX(0x4);
	
	/* Enable clock for FTM */
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	
	/* Set FTM mode */
	FTM0_MODE = (FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK);
	FTM0_MODE &= ~FTM_MODE_FTMEN_MASK;
	FTM0_SC = 0;
	FTM0_CNTIN = 0x00;
	
	/* Set FTM modular */
	FTM0_MOD = 0xFF;
	FTM0_QDCTRL &= ~(FTM_QDCTRL_QUADEN_MASK);
	FTM0_COMBINE &= ~(FTM_COMBINE_DECAPEN0_MASK \
			|FTM_COMBINE_DECAPEN1_MASK \
			|FTM_COMBINE_DECAPEN2_MASK \
			|FTM_COMBINE_DECAPEN3_MASK \
			|FTM_COMBINE_COMBINE0_MASK \
			|FTM_COMBINE_COMBINE1_MASK \
			|FTM_COMBINE_COMBINE2_MASK \
			|FTM_COMBINE_COMBINE3_MASK);
	FTM0_C0SC = 0x28;
	
	/* Enable FTM */
	FTM0_SC = 0x08;
}
