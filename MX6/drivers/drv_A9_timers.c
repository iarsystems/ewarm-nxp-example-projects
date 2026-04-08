/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : drv_A9_timers.c
 *    Description : Cortex  A9 Timers driver.
 *
 *    History :
 *    1. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : Initial revision.  Only Private Timer functions are
 *                     implemented
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#define TIMERS_CLOBAL


/** include files **/
#include <stddef.h>
#include <stdint.h>
#include "A9_timers.h"
#include "drv_A9_timers.h"

/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/
/*************************************************************************
 * Function Name: PTMR_Init
 * Parameters: uint32_t load      - Timer Load Value.
 *             uint8_t  prescaler - Timer Prescaler Value
 *             uint8_t  flags     - Timer Auto Reload and Timer IRQ enable
 * Return: none
 *
 * Description: Private Timer Initialization.
 *              After calling this function the timer will be disabled.
 *
 *************************************************************************/
void PTMR_Init(uint32_t load, uint8_t prescaler, uint8_t flags)
{
  /*Stop Private Timer*/
  PTMR->CTLR &= ~PTMR_CTLR_TMR_ENABLE;

  /*Set timer load value. This also will write to the timer
    counter register*/
  PTMR->LDR = load;

  /*Set prescaler, auto reload and irq enable*/
  PTMR->CTLR = (prescaler<<8) | (flags & (PTMR_CTLR_AUTO_RELOAD | PTMR_CTLR_IRQ_ENABLE));
}

/*************************************************************************
 * Function Name: PTMR_Start
 * Parameters: none
 *
 * Return: none
 *
 * Description: Start Private Timer.
 *
 *************************************************************************/
void PTMR_Start(void)
{
  PTMR->CTLR |= PTMR_CTLR_TMR_ENABLE;
}

/*************************************************************************
 * Function Name: PTMR_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stop Private Timer.
 *
 *************************************************************************/
void PTMR_Stop(void)
{
  PTMR->CTLR &= ~PTMR_CTLR_TMR_ENABLE;
}

/*************************************************************************
 * Function Name: PTMR_GetCounter
 * Parameters: none
 *
 * Return: uint32_t  - Private Timer Counter Value
 *
 *
 * Description: Read the Private Timer Counter Value.
 *
 *************************************************************************/
uint32_t PTMR_GetCounter(void)
{
  /*Read the Private Timer Counter Value*/
  return (PTMR->CNTR);
}

/*************************************************************************
 * Function Name: PTMR_GetIsr
 * Parameters: none
 *
 * Return: uint32_t  0 - The event flag in the ISR register is not set
 *                   1 - The event flag in the ISR register is set
 *
 * Description: Read the event flag in the Private timer ISR register.
 *
 *************************************************************************/
uint32_t PTMR_GetIsr(void)
{
  /*Read The Private Timer ISR event flag*/
  return (PTMR->ISR & PTMR_ISR_EVENT);
}

/*************************************************************************
 * Function Name: PTMR_ClrIsr
 * Parameters: none
 *
 * Return: none
 *
 * Description: Clear the event flag in the Private timer ISR register.
 *
 *************************************************************************/
void PTMR_ClrIsr(void)
{
  PTMR->ISR = PTMR_ISR_EVENT;
}
/** private functions **/
