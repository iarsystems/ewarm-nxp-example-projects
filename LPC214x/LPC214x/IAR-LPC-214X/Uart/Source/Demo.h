

#include <NXP/iolpc2148.h>
#include <intrinsics.h>


#define TICKS_PER_100mSEC                10
#define ONE_HUNDRED_mSEC_TIMER            2
#define TICKS_PER_50mSEC                  5
#define FIFTY_mSEC_TIMER                  1
#define FIFTY_mSEC_TIMER_INSTALLED       12
#define ONE_HUNDRED_mSEC_TIMER_INSTALLED 22
#define TIMER_EXPIRED                    50
#define INT_ON_MATCH                    0x1
#define RESET                           0x2
#define EVENT_UNDEFINED                 0xFFFFFFFF

#define XTALFREQ 12000000        //XTAL frequency in Hz
#define PCLKFREQ (XTALFREQ/4)     //pclk must always be XTALFREQ/4?

#define FALSE 0
#define TRUE !(FALSE)

#define EIGHT_BIT_CHARS          3
#define DIVISOR_LATCH_SET_bit  0x7
#define PARITY_ENABLE_bit      0x3   // Parity enable bit in UxLCR is bit 3
#define STOP_BIT_SELECT_bit    0x2   // Stop bit control in UxLCR is bit 2

#define ENABLE_UART1_RX     0x12
#define ENABLE_UART1_TX     0x10

#define MAX_SOFTWARE_TIMERS 3

#define SetBit(x,y);       ( (x) |= (1<<y) )
#define ClrBit(x,y);       ( (x) &= (~(1<<y)) )

typedef struct
{
  unsigned int us_Ticks;
  unsigned int us_Event;
} SoftwareTimer_t;

SoftwareTimer_t  softTimer[MAX_SOFTWARE_TIMERS];
char textString[] = "For cool ARM C/EC++development tools call Mark at IAR 508 485 2692";

int main(void);
void initializeInterruptDataStructs(void);
void installTimer(SoftwareTimer_t *, unsigned int, unsigned int , unsigned int);

__irq __arm void IRQ_ISR_Handler (void);
void MM_TIMER0_ISR();
void NonVectISR(void);
void sendByte(char byte);
void feed(void);




