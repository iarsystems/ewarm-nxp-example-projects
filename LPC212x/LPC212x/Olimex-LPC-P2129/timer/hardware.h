#include <intrinsics.h>
#include <NXP/iolpc2129.h>

//Type Definitions -these are tailored to fit given bit sizes storage requests to the actual bit size preference of the target
typedef unsigned int            U8;             /*  8 bit unsigned UPGRADE*/
typedef signed int              S8;             /*  8 bit signed   UPGRADE*/
typedef unsigned int            U16;            /* 16 bit unsigned */
typedef signed int              S16;            /* 16 bit signed */
typedef unsigned long int       U32;            /* 32 bit unsigned */
typedef signed long int         S32;            /* 32 bit signed */

/*-------------------------------------------------------------------------------------------------------------------------------*/
//Declarations of functions coded in hardware.c
void CPUinit(void);
void INTERRUPTSinit(void);
__irq __arm void IRQ_Handler(void);
static void DefDummyInterrupt();
//static void ExternalInterrupt0();
//static void ExternalInterrupt1();
//static void ExternalInterrupt2();
static void UART1Interrupt();

void TIMER0initstart();
static void TimerInterrupt();
//
void Delay(U32 del);
//
void UART0Init(U16 baud);
void UART0Init_Int(U16 baud);
void UART1Init(U16 baud);
void UART1Init_Int(U16 baud);
void UART0WriteChar(U16 chr);
void UART1WriteChar(U16 chr);
U16 UART0ReadChar(void);
U16 UART1ReadChar(void);
void __putchar0(U8 ch0);
void __putchar1(U8 ch1);

/*-------------------------------------------------------------------------------------------------------------------------------*/
//Generic Constants
#define FALSE     0
#define FAIL      0
#define OFF       0
#define DOWN      0
#define LOW       0

#define TRUE      1
#define SUCCESS   1
#define ON        1
#define UP        1
#define HIGH      1

/*-------------------------------------------------------------------------------------------------------------------------------*/
//Generic Macros -BUT IO0CLR_bit.P0_12=1 type bit setting is preffered
#define BIT_SET(data,bit)     (data |= 2^bit)
#define BIT_CLEAR(data,bit)   (data &= ~2^bit)
#define BIT_TEST(data,bit)    (data & 2^bit)

#define MSB(word)       ((U8)((word)>>8))              /* used to strip MSB and LSB bytes from 16-bit Words */
#define LSB(word)       ((U8)(word))

/*-------------------------------------------------------------------------------------------------------------------------------*/
//Hardware Specific Macros -Note the use of structures from IAR Header files
//Actions (Prefix A_)
#define   A_PULL_DOWN   IO0CLR_bit.P0_2=1
#define   A_PULL_UP     IO0SET_bit.P0_2=1
#define   A_LED1_ON     IO0CLR_bit.P0_12=1
#define   A_LED1_OFF    IO0SET_bit.P0_12=1
#define   A_LED2_ON     IO0CLR_bit.P0_13=1
#define   A_LED2_OFF    IO0SET_bit.P0_13=1
#define   A_TD2_LOW     IO0CLR_bit.P0_24=1
#define   A_TD2_HIGH    IO0SET_bit.P0_24=1
//States  (Prefix S_)
#define   S_BUT1        IO0PIN_bit.P0_10
#define   S_BUT2        IO0PIN_bit.P0_11
#define   S_DALLAS      IO0PIN_bit.P0_15
#define   S_FREQ        IO0PIN_bit.P0_22
#define   S_RD2         IO0PIN_bit.P0_23
#define   S_RD1         IO0PIN_bit.P0_25
#define   S_ADC         IO0PIN_bit.P0_27

#define   OSCILLATOR_CLOCK_FREQUENCY  14745600  //in MHz
#define   PLOCK           0x0400
#define   DLS_FREQ_TIME   1000000
#define   SET_TIME        100
#define   LED_TIME        500000
#define   TD2_TIME        1250
