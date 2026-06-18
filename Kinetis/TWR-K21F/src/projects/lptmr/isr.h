/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */
#undef  VECTOR_074
#define VECTOR_074 lptmr_isr_example


// ISR(s) are defined in your project directory.
extern void lptmr_isr_example(void);



#endif  //__ISR_H

/* End of "isr.h" */
