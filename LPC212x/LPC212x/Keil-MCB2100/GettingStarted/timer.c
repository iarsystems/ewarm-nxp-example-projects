
/*
 * $Revision: 28 $
 */

#include "timer.h"
#include <NXP/iolpc2129.h>

static volatile int ms_ctr = 0;

void TimerBeat(void)
{
    // Called at 1000 Hz rate.
    ms_ctr++; // Sleep counter.
}

unsigned long v3;
void Sleep(int milliseconds)
{
    ms_ctr = 0;
    
    while (ms_ctr < milliseconds) ;
}
