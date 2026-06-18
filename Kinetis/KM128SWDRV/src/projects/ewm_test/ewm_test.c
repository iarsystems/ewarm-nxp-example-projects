#include "drivers.h"

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* upper watchdog window timeout is 200 ms                                  */
  EWM_Init(EWM_MODULE_IN_EN_CONFIG(0,200) ,PRI_LVL0, NULL);

  while(1)
  {
    EWM_WriteRefreshSeq();   /* EWM counter refresh                           */
  }
}
