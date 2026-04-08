
#include <iolpc2148.h>
#include <intrinsics.h>

#define XTALFREQ 14745600         //XTAL frequency in Hz
#define PCLKFREQ (XTALFREQ/4)     //pclk must always be XTALFREQ/4?

#define FALSE 0
#define TRUE !(FALSE)

void main(void);
__irq __arm void IRQ_ISR_Handler (void);
void MM_TIMER0_ISR();




