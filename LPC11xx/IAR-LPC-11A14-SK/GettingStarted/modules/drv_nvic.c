/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : drv_nvic.c
 *    Description : NXP LPC11A14 Nested Vectored Interrupt Controller
 *
 *    History :
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    2. Date        : September 2011
 *       Author      : Atanas Uzunov
 *       Description : Changed function names to be identical with CMSIS.
 *                     Ported to LPC11A14   
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include <NXP\iolpc11a1x.h>
#include <assert.h>
/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/
uint32_t CriticalSecCntr;
/** private data **/

/** public functions **/
/*************************************************************************
 * Function Name: NVIC_EnableIRQ
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Enable interrupt at NVIC
 *
 *************************************************************************/
void NVIC_EnableIRQ(uint32_t IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &ISER;

  assert((NVIC_GPIO0 <= IntNumber) && (NVIC_FLASH >= IntNumber));
  IntNumber -= NVIC_GPIO0;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));
}

/*************************************************************************
 * Function Name: NVIC_DisableIRQ
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Disable interrupt at NVIC
 *
 *************************************************************************/
void NVIC_DisableIRQ(uint32_t IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &ICER;

  assert((NVIC_GPIO0 <= IntNumber) && (NVIC_FLASH >= IntNumber));
  IntNumber -= NVIC_GPIO0;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));
}

/*************************************************************************
 * Function Name: NVIC_ClearPendingIRQ
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Clear pending interrupt at NVIC
 *
 *************************************************************************/
void NVIC_ClearPendingIRQ(uint32_t IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &ICPR;

  assert((NVIC_GPIO0 <= IntNumber) && (NVIC_FLASH >= IntNumber));
  IntNumber -= NVIC_GPIO0;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));
}


/*************************************************************************
 * Function Name: NVIC_SetPriority
 * Parameters: IntNumber - Interrupt number, Priority - Interrupt priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *************************************************************************/
void NVIC_SetPriority(uint32_t IntNumber, uint8_t Priority)
{
volatile uint8_t * pNVIC_IntPri = (uint8_t *)&IPR0;

  assert((NVIC_GPIO0 <= IntNumber) && (NVIC_FLASH >= IntNumber));
  IntNumber -= NVIC_GPIO0;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

/** private functions **/
