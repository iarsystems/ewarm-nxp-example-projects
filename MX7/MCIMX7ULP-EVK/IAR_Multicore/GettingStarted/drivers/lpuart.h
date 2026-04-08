/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : lpuart.h
 *    Description : LPUART driver header
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
 * Function Name: LPUART_Init
 * Parameters: LPUART_MemMapPtr LPUART_BASE, uint32_t Baud, uint32_t Clock, unit8_t Src
 *
 * Return: void
 *
 * Description: Configure the LPUART module.
 *************************************************************************/
void LPUART_Init(LPUART_MemMapPtr LPUART, uint32_t Baud, uint32_t Clock, uint8_t Src);
