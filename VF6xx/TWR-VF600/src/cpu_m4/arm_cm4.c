/*
 * File:		arm_cm4.c
 * Purpose:		Generic high-level routines for ARM Cortex M4 processors
 *
 * Notes:
 */

#include "common.h"
#include "arm_cm4.h"

/***********************************************************************/
/*
 * Change the value of the vector table offset register to the specified value.
 *
 * Parameters:
 * vtor     new value to write to the VTOR
 */

void write_vtor (int vtor)
{
        /* Write the VTOR with the new value */
        SCB->VTOR = vtor;
}
/***********************************************************************/
/*
 * Initialize the NVIC to enable the specified IRQ.
 *
 * NOTE: The function only initializes the NVIC to enable a single IRQ.
 * Interrupts will also need to be enabled in the ARM core. This can be
 * done using the EnableInterrupts macro.
 *
 * Parameters:
 * irq    irq number to be enabled (the irq number NOT the vector number)
 */

void enable_irq (int irq)
{
    int div;

    /* Make sure that the IRQ is an allowable number. Right now up to 105 is
     * used.
     */
    if (irq > 111)
        printf("\nERR! Invalid IRQ value passed to enable irq function!\n");

    /* Determine which of the NVICISERs corresponds to the irq */
    div = irq/32;

    switch (div)
    {
    	case 0x0:
              NVIC->ICPR[0] = 1 << (irq%32);
              NVIC->ISER[0] = 1 << (irq%32);
              break;
    	case 0x1:
              NVIC->ICPR[1]= 1 << (irq%32);
              NVIC->ISER[1] = 1 << (irq%32);
              break;
    	case 0x2:
              NVIC->ICPR[2] = 1 << (irq%32);
              NVIC->ISER[2] = 1 << (irq%32);
              break;
      case 0x3:
              NVIC->ICPR[3] = 1 << (irq%32);
	      NVIC->ISER[3] = 1 << (irq%32);
	      break;
    }
}
/***********************************************************************/
/*
 * Initialize the NVIC to disable the specified IRQ.
 *
 * NOTE: The function only initializes the NVIC to disable a single IRQ.
 * If you want to disable all interrupts, then use the DisableInterrupts
 * macro instead.
 *
 * Parameters:
 * irq    irq number to be disabled (the irq number NOT the vector number)
 */

void disable_irq (int irq)
{
    int div;

    /* Make sure that the IRQ is an allowable number. Right now up to 105 is
     * used.
     */
    if (irq > 105)
        printf("\nERR! Invalid IRQ value passed to disable irq function!\n");

    /* Determine which of the NVICICERs corresponds to the irq */
    div = irq/32;

    switch (div)
    {
    	case 0x0:
              NVIC->ICER[0] = 1 << (irq%32);
              break;
    	case 0x1:
              NVIC->ICER[1] = 1 << (irq%32);
              break;
    	case 0x2:
              NVIC->ICER[2] = 1 << (irq%32);
              break;
    	case 0x3:
              NVIC->ICER[3] = 1 << (irq%32);
              break;
    }
}
/***********************************************************************/
/*
 * Initialize the NVIC to set specified IRQ priority.
 *
 * NOTE: The function only initializes the NVIC to set a single IRQ priority.
 * Interrupts will also need to be enabled in the ARM core. This can be
 * done using the EnableInterrupts macro.
 *
 * Parameters:
 * irq    irq number to be enabled (the irq number NOT the vector number)
 * prio   irq priority. 0-15 levels. 0 max priority
 */

void set_irq_priority (int irq, int prio)
{
    /*irq priority pointer*/
    uint8 *prio_reg;

    /* Make sure that the IRQ is an allowable number. Right now up to 105 is
     * used.
     */
    if (irq > 105)
        printf("\nERR! Invalid IRQ value passed to priority irq function!\n");

    if (prio > 15)
        printf("\nERR! Invalid priority value passed to priority irq function!\n");

    /* Determine which of the NVICIPx corresponds to the irq */
    prio_reg = (uint8 *)(((uint32)&NVIC->IP[0]) + irq);
    /* Assign priority to IRQ */
    *prio_reg = ( (prio&0xF) << (8 - ARM_INTERRUPT_LEVEL_BITS) );
}
/***********************************************************************/

