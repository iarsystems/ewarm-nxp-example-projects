
#include "NXP/iolpc2101.h"
#include "lpc2103.h"
#include <intrinsics.h>

#define TICKS_PER_FIVEHUNDRED_MSEC 50   // 50 Ticks at 10msec rate = 0.5sec
#define TICKS_PER_ONEHUNDRED_MSEC  10   // 10 Ticks at 10msec rate = 0.1sec
#define FALSE 0
#define TRUE !(FALSE)
#define PIN0_0 0
#define PIN0_1 1
#define PIN0_2 2
#define PIN0_3 3
#define PIN0_4 4
#define PIN0_5 5
#define PIN0_6 6
#define PIN0_7 7
#define PIN0_8 8
#define PIN0_9 9
#define PIN0_A 10
#define PIN0_B 11
#define PIN0_C 12
#define PIN0_D 13
#define PIN0_E 14
#define PIN0_F 15

#define SetBit(x,y);  ( (x) |= (1<<y) )
#define ClrBit(x,y);  ( (x) &= (~(1<<y)) )

int main(void);
__irq __arm void Tmr0_IntOnMR0_isr(void);
__irq __arm void EINT1BtnDown_isr(void);
__irq __arm void EINT2BtnDown_isr(void);
__fiq __arm void fiq_handler(void);
__arm void undef_handler(void);
__arm void prefetch_handler(void);
__arm void data_handler(void);
void TogglePortPin(unsigned int );
unsigned int bl_Tmr0Flag;
unsigned int us_EINT1Count;
unsigned int us_EINT2Count;
