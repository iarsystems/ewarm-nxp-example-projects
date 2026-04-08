// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

/*
 * $Revision: 31 $
 */

#include "timer.h"

static volatile int ms_ctr = 0;


void TimerBeat(void)
{
  // Called at 1000 Hz rate.
  ms_ctr++; // Sleep counter.
}


void Sleep(int milliseconds)
{
  ms_ctr = 0;
  while (ms_ctr < milliseconds) ;
}
