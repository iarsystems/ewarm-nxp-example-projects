
/*
 * $Revision: 28 $
 */

//XTAL frequency in Hz
//#define XTALFREQ 12000000 //14745600
#define XTALFREQ    1843200

//pclk must always be XTALFREQ/4 when using the ROM monitor.
#define PCLKFREQ (XTALFREQ/4)

void LPC212xSystemInit();

void LPC212xInitVIC();
void LPC212xInitTimerInterrupt(void(*timer_func)());
void LPC212xInitExternalInterrupt0(void(*eint0_func)());
void LPC212xInitExternalInterrupt1(void(*eint1_func)());
void LPC212xInitExternalInterrupt2(void(*eint2_func)());
void LPC212xInitUART1Interrupt(void(*uart1rx_func)(unsigned char),
                               void(*uart1tx_func)());

void LPC212xInitTimer();
void LPC212xStartTimer();

void LPC212xInitEINT0();
void LPC212xInitEINT1();
void LPC212xInitEINT2();
unsigned char EINT0PinIsLow();
unsigned char EINT1PinIsLow();
unsigned char EINT2PinIsLow();

void LPC212xInitPIO();
void LPC212xLedSet(unsigned long setmask, unsigned long clearmask);

void LPC212xInitUART1();

//Transmits one byte via UART1
//Note: The UART1 THRE register must be empty before the call to this function.
void LPC212xUART1TxByte(unsigned char byte);
