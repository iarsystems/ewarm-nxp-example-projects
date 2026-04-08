
/*
 * $Revision: 28 $
 */

//XTAL frequency in Hz
#define XTALFREQ 14745600

//pclk must always be XTALFREQ/4 when using the ROM monitor.
#define PCLKFREQ (XTALFREQ/4)

void LPC210xSystemInit();

void LPC210xInitVIC();
void LPC210xInitTimerInterrupt(void(*timer_func)());
void LPC210xInitExternalInterrupt0(void(*eint0_func)());
void LPC210xInitExternalInterrupt1(void(*eint1_func)());
void LPC210xInitExternalInterrupt2(void(*eint2_func)());
void LPC210xInitUART1Interrupt(void(*uart1rx_func)(unsigned char),
                               void(*uart1tx_func)());
                               
void LPC210xInitTimer();
void LPC210xStartTimer();

void LPC210xInitEINT0();
void LPC210xInitEINT1();
void LPC210xInitEINT2();
unsigned char EINT0PinIsLow();
unsigned char EINT1PinIsLow();
unsigned char EINT2PinIsLow();

void LPC210xInitPIO();
void LPC210xLedSet(unsigned short setmask, unsigned short clearmask);

void LPC210xInitUART1();

//Transmits one byte via UART1
//Note: The UART1 THRE register must be empty before the call to this function.
void LPC210xUART1TxByte(unsigned char byte);
