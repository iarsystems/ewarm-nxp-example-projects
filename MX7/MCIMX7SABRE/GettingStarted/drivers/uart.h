/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : uart.c
 *    Description : MCIMX7D/MCIMX7S UART driver
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
 * Function Name: UART_Init
 * Parameters: UART_MemMapPtr UART_BASE
 *
 * Return: none
 *
 * Description: UART Initialization at 115200 baud.
 *
 *************************************************************************/
void UART_Init(UART_MemMapPtr UART_BASE);
