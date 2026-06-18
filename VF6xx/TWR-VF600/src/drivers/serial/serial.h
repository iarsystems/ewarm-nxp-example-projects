#ifndef _SERIAL_H
#define _SERIAL_H

#include "common.h"


#ifndef APPS_CODE
  //#define ThirdEye  // Used for logging of test results in IDC
#endif

/********************************************************************/

void uart_init(unsigned long int clkspeed, unsigned long int baud);
unsigned char uart_getchar (void);
void uart_putchar (char ch);
int uart_getchar_present (void);

char	
in_char(void);

void
out_char(char);

int
char_present(void);

int		
printf(const char *, ... );

int
sprintf(char *, const char *, ... );

/********************************************************************/
#endif

