/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : drv_nvic.c
 *    Description : NXP LPC1342 Nested Vectored Interrup Controller
 *
 *    History :
 *    1. Date        : June 2012
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include <NXP\iolpc1342.h>
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
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(uint32_t IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &SETENA0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(uint32_t IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &CLRENA0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(uint32_t IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &CLRPEND0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(uint32_t IntNumber, uint8_t Priority)
{
volatile uint8_t * pNVIC_IntPri = (uint8_t *)&IP0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}
/** private functions **/
