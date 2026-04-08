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
#undef  VECTOR_089
#define VECTOR_089 USB_ISR   

#undef  VECTOR_090
#define VECTOR_090 DCD_ISR

#undef  VECTOR_104
#define VECTOR_104 PortB_ISR

// ISR(s) are defined in your project directory.
extern void USB_ISR (void);
extern void DCD_ISR (void);   
extern void PortB_ISR (void);  



#endif  //__ISR_H

/* End of "isr.h" */
