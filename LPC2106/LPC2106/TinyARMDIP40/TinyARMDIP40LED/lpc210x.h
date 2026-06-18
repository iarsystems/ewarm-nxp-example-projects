
/*
 * $Revision: 31 $
 */

void LPC210xSystemInit();
void LPC210xInitInterrupt(void(*timer_func)());
void LPC210xInitTimer();
void LPC210xStartTimer();

void LPC210xInitPIO();
void LPC210xLedSet(unsigned long mask);
void LPC210xSevenSegmentDisplay(unsigned int value);

