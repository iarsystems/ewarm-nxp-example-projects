// --------------------------------------------------------------------------
// COPYRIGHT (c) Freescale 2009
// Freescale Confidential Proprietary
//  All Rights Reserved
// --------------------------------------------------------------------------
//!\file    tprintf.h
//!\brief   This file contains declartions used with tprintf.c
//
// $Log: printf.h.rca $

// --------------------------------------------------------------------------


#ifndef _PRINTF_H_
#define _PRINTF_H_

#include <stdarg.h>

#define DEST_CONSOLE	(1)
#define DEST_STRING		(2)

typedef struct
{
	int	dest;
	void (*func)(char);
	char *loc;
} PRINTK_INFO;

int printf (const char *fmt, ...);
int printk (PRINTK_INFO *info, const char *fmt, va_list ap);



#endif
