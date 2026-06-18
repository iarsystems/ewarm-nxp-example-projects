/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */

#undef  VECTOR_077
#define VECTOR_077 portc_isr

// ISR(s) are defined in your project directory.
extern void portc_isr(void);


#endif  //__ISR_H

/* End of "isr.h" */
