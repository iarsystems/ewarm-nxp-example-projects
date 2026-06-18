
#ifndef UART_H
#define UART_H

#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif




/***********************************************************************
 * NAND support functions
 **********************************************************************/

void uart_output_init(void);
void uart_output(UNS_8 *buff);
INT_32 uart_input(void *buffer, INT_32 max_bytes);


#ifdef __cplusplus
}
#endif

#endif /* UART_H */
