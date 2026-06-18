/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : llio.h
 *    Description : Low level IO initialization header
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 7084 $
 **************************************************************************/

/*************************************************************************
 * Function Name: LLIO_Init
 * Parameters: LPUART_MemMapPtr LPUART_Base
 *
 * Return: void
 *
 * Description: LPUART instance configuration for low level IO functions.
 *************************************************************************/
void LLIO_Init(LPUART_MemMapPtr LPUART_Base);
