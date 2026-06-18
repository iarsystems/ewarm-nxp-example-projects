
#ifndef __DEVICE_REGISTERS_H__
#define __DEVICE_REGISTERS_H__

/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */

#if defined(CPU_MCIMX7D_A7)

#include <NXP/MCIMX7D7_A7.h>

#elif defined(CPU_MCIMX7D_M4)

#include <NXP/MCIMX7D7_M4.h>
#define IRQn_Type IRQInterruptIndex
// "#define USE_CMSIS 1" if CMSIS is used
#define USE_CMSIS 0
#if defined(USE_CMSIS) && USE_CMSIS == 1
#define CMSIS_NVIC_VIRTUAL
//Number of Bits used for Priority Levels
#define __NVIC_PRIO_BITS               4
#include "core_cm4.h"
#endif
#else

#error "No valid CPU defined!"

#endif

#endif /* __DEVICE_REGISTERS_H__ */
