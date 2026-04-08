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

//extern void adc0_isr(void);                                // ISR for ADC0
//extern void adc1_isr(void);                                // ISR for ADC1


//#undef  VECTOR_073   // as it was previously defined in vectors.h
//#undef  VECTOR_074   // as it was previously defined in vectors.h

                                     // address     vector irq    perihperal
//#define VECTOR_073      adc0_isr     // 0x0000_0124 73     57     ADC0
//#define VECTOR_074      adc1_isr     // 0x0000_0128 74     58     ADC1

extern void ftm2_isr(void); //isr for FTM2

#undef VECTOR_080
#define VECTOR_080 ftm2_isr

#endif  //__ISR_H

/* End of "isr.h" */
