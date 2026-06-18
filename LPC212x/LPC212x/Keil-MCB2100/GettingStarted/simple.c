#include <intrinsics.h>
#include "timer.h"
#include "lpc212x.h"

void txstr(char*);
/******************************************************************************
*/
int main(void)
{
    unsigned int leds = 0x0010000;

    // System initialization, this will map the exception vectors.
    LPC212xSystemInit();

    // Set up peripheral registers.
    LPC212xInitPIO();

    // First disable interrupts.
    __disable_interrupt();
    // Setup interrupt controller.
    LPC212xInitVIC();

    LPC212xInitTimerInterrupt(TimerBeat);
    LPC212xInitTimer();

    LPC212xInitUART1();

    __enable_interrupt();

    // Start periodic timer.
    LPC212xStartTimer();

    // test UART output
    txstr("\r\nHello world\r\n");

    // Loop forever
    for (;;)
    {
        LPC212xLedSet(leds,0xff0000);
        Sleep(65); // Display for 65 ms.
        leds <<= 1;
        leds |= leds>>8;
        leds &= 0xff0000;
    }
}
