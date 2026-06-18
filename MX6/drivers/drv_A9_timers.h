/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : drv_A9_timers.h
 *    Description : Cortex  A9 Timers driver header.
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
/** include files **/
#include <stdint.h>
#include "A9_timers.h"

/** definitions **/

/** public data **/

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
void PTMR_Init(uint32_t load, uint8_t prescaler, uint8_t flags);

/*************************************************************************
 * Function Name: PTMR_Start
 * Parameters: none
 *
 * Return: none
 *
 * Description: Start Private Timer.
 *
 *************************************************************************/
void PTMR_Start(void);

/*************************************************************************
 * Function Name: PTMR_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stop Private Timer.
 *
 *************************************************************************/
void PTMR_Stop(void);

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
uint32_t PTMR_GetCounter(void);

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
uint32_t PTMR_GetIsr(void);

/*************************************************************************
 * Function Name: PTMR_ClrIsr
 * Parameters: none
 *
 * Return: none
 *
 * Description: Clear the event flag in the Private timer ISR register.
 *
 *************************************************************************/
void PTMR_ClrIsr(void);
