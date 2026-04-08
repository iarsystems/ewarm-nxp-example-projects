/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1

#ifdef SDRAM
#undef  VECTOR_113
#define VECTOR_113 lcdc_demo_isr

// ISR(s) are defined in your project directory.
extern void lcdc_demo_isr();
#endif

#endif  //__ISR_H

/* End of "isr.h" */
