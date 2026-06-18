
#include "NXP/iolpc2101.h"
#include <stdio.h>
#include <intrinsics.h>
#include "lpc2103.h"


#define TICKS_PER_FIVEHUNDRED_MSEC 50      // 50 Ticks at 10msec rate = 0.5sec

#define FALSE 0
#define TRUE !(FALSE)
#define PIN0_4             0x4
#define PIN0_5             0x5
#define RESET_ON_MR0         1
#define COUNTER_ENABLE 0
#define INT_ON_MR0     0
#define STOP_ON_MR0    2

#define EIGHT_BIT_CHARS          3
#define DIVISOR_LATCH_SET_bit  0x7
#define PARITY_ENABLE_bit      0x3   // Parity enable bit in UxLCR is bit 3
#define STOP_BIT_SELECT_bit    0x2   // Stop bit control in UxLCR is bit 2

#define ENABLE_UART1_RX       0x12
#define ENABLE_UART1_TX       0x10

#define MANDELAY              110000UL

typedef unsigned char UCHAR8;
typedef unsigned int  UINT32;

int main(void);
__arm void undef_handler(void);
__arm void prefetch_handler(void);
__arm void data_handler(void);
__irq __arm void Tmr0IntOnMR0_isr(void);
__fiq __arm void fiq_handler(void);

UCHAR8 string1[] = "  Interrupt on Match Register 0 interrupt handler \r\n";
UCHAR8 string2[] = "  ******** FIQ interrupt handler ******** \r\n";
