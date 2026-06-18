/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : gpt.c
 *    Description : General Purpose Timer driver
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 7084 $
 **************************************************************************/
#include <stdint.h>

/*************************************************************************
 * Function Name: GPTimer_Init
 * Parameters: GPT_MemMapPtr GPT_BASE, uint32_t ticks_per_sec
 *
 * Return: none
 *
 * Description: General Purpose Timer Initialization
 *************************************************************************/
void GPTimer_Init(GPT_MemMapPtr GPT_BASE, uint32_t ticks_per_sec);

/*************************************************************************
 * Function Name: GPTimer_Start
 * Parameters: GPT_MemMapPtr GPT_BASE
 *
 * Return: none
 *
 * Description: Starts the General Purpose Timer.
 *************************************************************************/
void GPTimer_Start(GPT_MemMapPtr GPT_BASE);

/*************************************************************************
 * Function Name: GPTimer_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: General Purpose Timer interrupt handler
 *************************************************************************/
void GPTimer_ClearOF1Flag(GPT_MemMapPtr GPT_BASE);
