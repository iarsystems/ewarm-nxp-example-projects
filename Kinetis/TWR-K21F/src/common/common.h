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
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/* 
 * Include the generic CPU header file 
 */
#include "arm_cm4.h"
//#include "Settings.h"

/* 
 * Include the platform specific header file 
 */
#if (defined(TOWER))
  #include "tower.h"
#else
  #error "No valid platform defined"
#endif

/* 
 * Include the cpu specific header file 
 */
#define MCU_MK21F120
#if(defined(MCU_MK21F120))
  #include "MK21F12.h"
#elif(defined(MCU_MK22F120))
  #include "MK22F12.h"
#else
  #error "No valid CPU defined"
#endif


/* 
 * Include any toolchain specfic header files 
 */
#if (defined(__MWERKS__))
  #include "mwerks.h"
#elif (defined(__DCC__))
  #include "build/wrs/diab.h"
#elif (defined(__ghs__))
  #include "build/ghs/ghs.h"
#elif (defined(__GNUC__))
  #include "build/gnu/gnu.h"
#elif (defined(IAR))
  #include "iar.h"
#else
#warning "No toolchain specific header included"
#endif

/* 
 * Include common utilities
 */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"

#if (defined(IAR))
  #include "intrinsics.h"
#endif
/********************************************************************/

#endif /* _COMMON_H_ */
