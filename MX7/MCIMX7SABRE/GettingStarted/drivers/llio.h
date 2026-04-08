/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : llio.c
 *    Description : Low Level IO redirection driver
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
 * Parameters: UART_MemMapPtr UART_BASE
 *
 * Return: void
 *
 * Description: UART instance configuration for low level IO functions.
 *************************************************************************/
void LLIO_Init(UART_MemMapPtr UART_BASE);
