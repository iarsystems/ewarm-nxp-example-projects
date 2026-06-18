/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1



extern void lptmr_app_isr(void);
extern void llwu_isr(void);



#undef VECTOR_074
#define VECTOR_074 lptmr_app_isr

#undef VECTOR_037
#define VECTOR_037 llwu_isr

/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr


// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/


#endif  //__ISR_H

/* End of "isr.h" */
