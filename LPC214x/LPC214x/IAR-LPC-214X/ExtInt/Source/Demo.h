

#include <NXP/iolpc2148.h>
#include <intrinsics.h>

#define TICKS_PER_5_SECONDS             500
#define FIVE_SEC_TIMER                    0
#define FIVE_SEC_TIMER_INSTALLED         20
#define TICKS_PER_SECOND                100
#define ONE_SEC_TIMER                     1
#define ONE_SEC_TIMER_INSTALLED          21
#define TICKS_PER_100mSEC                10
#define ONE_HUNDRED_mSEC_TIMER            2
#define ONE_HUNDRED_mSEC_TIMER_INSTALLED 22
#define TIMER_EXPIRED                    50
#define INT_ON_MATCH                    0x1
#define RESET                           0x2

#define XTALFREQ 12000000         //XTAL frequency in Hz
#define PCLKFREQ (XTALFREQ/4)     //pclk must always be XTALFREQ/4?

#define FALSE 0
#define TRUE !(FALSE)


#define DOWN                1000
#define UP                  1001
#define TIMED_OUT           0x7FFFFFFF
#define EVENT_UNDEFINED     0XFFFFFFFF
#define MAX_BUTTONS         3
#define MAX_SOFTWARE_TIMERS 3

typedef struct
{
  unsigned int us_buttonState;
  unsigned int us_pressCount;
} ButtonIntFlags_t;

typedef struct
{
  unsigned int us_Ticks;
  unsigned int us_Event;
} SoftwareTimer_t;

unsigned char bl_flag;
unsigned char bl_100msecFlag;
unsigned char bl_Btn1DownFlag;
unsigned char bl_Btn2DownFlag;
ButtonIntFlags_t buttonData[MAX_BUTTONS];
SoftwareTimer_t  softTimer[MAX_SOFTWARE_TIMERS];


int main(void);
void initializeInterruptDataStructs(void);
void installTimer(SoftwareTimer_t *, unsigned int, unsigned int , unsigned int);
__irq __arm void IRQ_ISR_Handler (void);
void MM_TIMER0_ISR();
void Btn1DownISR(void);
void Btn2DownISR(void);
void NonVectISR(void);
void feed (void);
