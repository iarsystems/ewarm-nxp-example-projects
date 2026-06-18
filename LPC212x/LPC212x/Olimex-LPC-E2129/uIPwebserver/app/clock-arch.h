/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : clock-arch.h
 *    Description : Implementation of architecture-specific clock
 *                  functionality include file
 *
 *    History :
 *    1. Date        : June 15, 2006
 *       Author      : Stanimir Bonev
 *       Description : Created
 *
 *    $Revision: 28 $
**************************************************************************/
#include "includes.h"

#ifndef __CLOCK_ARCH_H__
#define __CLOCK_ARCH_H__

// Time Precision time [us]
#define TIMER_PRECISION     1
// Sys timer tick per seconds
#define TICK_PER_SECOND     100

typedef Int32U clock_time_t;

#define CLOCK_CONF_SECOND TICK_PER_SECOND


/*************************************************************************
 * Function Name: TIM0_IntrHandler
 * Parameters: void *arg
 *
 * Return: none
 *
 * Description: TIM0 interrupt handler
 *
 *************************************************************************/
void TIM0_IntrHandler (void *);

/*************************************************************************
 * Function Name: clock_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer init
 *
 *************************************************************************/
void clock_init(void);

/*************************************************************************
 * Function Name: clock_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: The current clock time, measured in system ticks
 *
 *************************************************************************/
clock_time_t clock_time(void);

#endif /* __CLOCK_ARCH_H__ */
