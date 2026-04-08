/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/



/*
 * Include the platform specific header file
 */
#if (defined(FARADAY_EVB))
  #include "faraday_evm.h"
#else
  #error "No valid platform defined"
#endif

/*
 * Include the cpu specific header file
 */
#include "MVF50GS10MK50.h"
#include "CA5INTD.h"

/*
 * Include macros
 */
#include "macros.h"

/*
 * Inlude printfs
 */
#include "serial.h"
//#include "printf.h"



/********************************************************************/

#endif /* _COMMON_H_ */
