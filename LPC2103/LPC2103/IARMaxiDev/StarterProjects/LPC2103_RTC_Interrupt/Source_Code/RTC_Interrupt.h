
#include "NXP/iolpc2101.h"
#include <intrinsics.h>
#include "lpc2103.h"

#define PIN0_4             0x04
#define FALSE              0
#define TRUE               !(FALSE)
#define ALL_PERIPHERALS_ON 0x30081793

int main(void);
__irq  __arm void RTC_INT_isr (void);
__fiq __arm void fiq_handler(void);
__irq __arm void undef_handler(void);
__irq __arm void prefetch_handler(void);
__irq __arm void data_handler(void);

unsigned int us_secondsCount;
unsigned int bl_TimerFlag;

