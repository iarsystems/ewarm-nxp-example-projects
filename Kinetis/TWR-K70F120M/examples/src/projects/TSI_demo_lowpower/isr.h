/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */



#undef  VECTOR_099
#define VECTOR_099 TSI_isr


#undef  VECTOR_037
#define VECTOR_037 llwu_isr

extern void llwu_isr(void);
extern void TSI_isr(void);


#endif  //__ISR_H

/* End of "isr.h" */
/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr


// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/


#endif  //__ISR_H

/* End of "isr.h" */
