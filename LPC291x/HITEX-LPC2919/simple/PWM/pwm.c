/*****************************************************************************
 *   pwm.c:  PWM module file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "pwm.h"

volatile DWORD counter_overflow = 0;
volatile DWORD transfer_done = 0;
volatile DWORD update_done = 0;
volatile DWORD match_counter0 = 0;

volatile DWORD PWMFlag = 0;

/******************************************************************************
** Function name:		PWM0Handler
**
** Descriptions:		PWM interrupt handler
**						
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM0Handler (void)
{
  DWORD regVal;

  regVal = PWM0_INT_STATUS;
  if ( regVal & CO_INT )
  {
	counter_overflow++;	
  }
  if ( regVal & UD_INT )
  {
	update_done++;
	PWMFlag |= UD_INT;
  }
  if ( regVal & TD_INT )
  {
	transfer_done++;
	PWMFlag |= TD_INT;
  }
  PWM0_INT_CLR_STATUS |= regVal;
  return;
}

/******************************************************************************
** Function name:		PWM0Handler
**
** Descriptions:		PWM capture Match interrupt handler
**						
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM0CapMatchHandler (void)
{
  DWORD regVal;

  /* check match 0 on PWM0 only */
  regVal = PWM0_INTM_STATUS;
  if ( regVal & (1<<0) )
  {
	match_counter0++;
  }
  PWM0_INTM_CLR_STATUS |= regVal;
  return;
}

/******************************************************************************
** Function name:	PWM_Init
**
** Descriptions:	PWM initialization, reset counter, all latches are 
**					enabled, interrupt on PWMMR0, install PWM interrupt 
**					to the VIC table.
**
** parameters:		Duty cycle
** Returned value:	None
** 
******************************************************************************/
void PWM0_Init( DWORD cycle )
{
  /* set GPIOs as PWM MATCH0~5 pins on PWM0 */
  SFSP0_2 = (0x01<<2)|(0x02<<0);	/* Digital no PU and PD, func. 2. */
  SFSP0_3 = (0x01<<2)|(0x02<<0);
  SFSP0_4 = (0x01<<2)|(0x02<<0);
  SFSP0_5 = (0x01<<2)|(0x02<<0);
  SFSP0_6 = (0x01<<2)|(0x02<<0);
  SFSP0_7 = (0x01<<2)|(0x02<<0);
  
  PWM0_Stop();

  /* PWM0 Output period is (PRD+1)*(PRSC+1) system clock cycles */
  PWM0_PRSC = 1;			/* prescale */
  PWM0_PRD = PWM_CYCLE;		/* period */ 		
  PWM0_Set( PWM_CYCLE, 0 );	/* offset set to 0 */

  /* Trap polarity: active low, disable traps on PWM0~5 */
  PWM0_TRPCTLS &= ~( PWM_MATCH0_5|(0x01<<16) );	

  /* Cotinuous mode, sync out inactive */
  PWM0_MODECTL &= ~(RUN_ONCE|SYNC_OUT_ENA);
  /* Transfer trigger and sync source are external */	
  PWM0_MODECTL |= (SYNC_SEL|TRANS_ENA_SEL);

  PWM0_CTRLS |= PWM_MATCH0_5;			/* PWM0~5 output are HIGH level, reset value */
  PWM0_CTRLS &= ~(PWM_MATCH0_5<<16);	/* PWM0~5 carrier burst disabled, reset value */ 

  PWM0_MODECTL |= UPD_ENA;		/* Update shadow registers one more time */

  /* Only PWM0 is used thus far. Enable PWM0 interrupt. */
  install_irq( PWM0_INT, (void *)PWM0Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( PWM0_INT, ACTIVE_HIGH, HIGHEST_PRIORITY );

#if 0
  /* Enable PWM capture/match interrupt. */ 
  install_irq( PWM0_CAP_INT, (void *)PWM0CapMatchHandler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( PWM0_CAP_INT, ACTIVE_HIGH, HIGHEST_PRIORITY );
  PWM0_INTM_SET_ENABLE = PWM_MATCH0_5;	/* Enable MATCH ACTIVE on match0~5  */
#endif

  /* Below line move to main(), or interrupt will occur immediately. */
//  PWM0_INT_SET_ENABLE = UD_INT | TD_INT;
  return;
}

/******************************************************************************
** Function name:		PWM_Set
**
** Descriptions:		PWM cycle setup
**
** parameters:			PWM cycle, and offset
** Returned value:		None
** 
******************************************************************************/
void PWM0_Set( DWORD cycle, DWORD offset )
{
  PWM0_MATCHA0 = 0x0;		/* set PWM cycle ACTIVE */
  PWM0_MATCHA1 = 0x0;
  PWM0_MATCHA2 = 0x0;
  PWM0_MATCHA3 = 0x0;
  PWM0_MATCHA4 = 0x0;
  PWM0_MATCHA5 = 0x0;
  	
  PWM0_MATCHDA0 = cycle;	/* set PWM cycle DEACTIVE */
  PWM0_MATCHDA1 = cycle * 5/6 + offset;
  PWM0_MATCHDA2 = cycle * 2/3 + offset;
  PWM0_MATCHDA3 = cycle * 1/2 + offset;
  PWM0_MATCHDA4 = cycle * 1/3 + offset;
  PWM0_MATCHDA5 = cycle * 1/6 + offset;

  PWM0_MODECTL |= UPD_ENA;
  PWM0_MODECTL |= TRANS_ENA;		/* Transfer and update shadow registers */
  return;
}

/******************************************************************************
** Function name:		PWM_Start
**
** Descriptions:		Start PWM
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM0_Start( void )
{
  PWM0_MODECTL |= CNT_RESET;	/* Counter Reset */
  PWM0_MODECTL &= ~CNT_RESET;

  /* start from reset value, enable update and transfer */
  PWM0_MODECTL = UPD_ENA|TRANS_ENA;	
  PWM0_MODECTL |= CNT_ENA;		/* counter enable, PWM enable */
  PWM0_MODECTL |= UPD_ENA;
  return;
}

/******************************************************************************
** Function name:		PWM_Stop
**
** Descriptions:		Stop PWM
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM0_Stop( void )
{
  PWM0_MODECTL |= CNT_RESET;	/* Counter Reset */
  PWM0_MODECTL &= ~CNT_RESET;

  /* All disabled UPD, TRANS, SYNC_OUT and COUNTER */
  PWM0_MODECTL &= ~(CNT_ENA|SYNC_OUT_ENA|TRANS_ENA);	/* Stop PWM */
  PWM0_MODECTL |= UPD_ENA;
  return;
}

/******************************************************************************
** Function name:		PWM_Reset
**
** Descriptions:		Reset PWM
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM0_Reset( void )
{
  PWM0_MODECTL |= CNT_RESET;	/* Counter Reset */
  PWM0_MODECTL &= ~CNT_RESET;

  /* Clear pending interrpt */
  PWM0_INT_CLR_STATUS = 0x0F;	/* Clear EMGY, UD, TD, and CO */
  PWM0_INTM_CLR_STATUS = 0xFFF;	/* Clear match act 0~5, match deact 0-5 */
  PWM0_INTC_CLR_STATUS = 0x0F;	/* Clear capture 0~3 */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
