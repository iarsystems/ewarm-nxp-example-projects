#ifndef _LED_INCLUDED
#define _LED_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes

MEMORY_ATTRIBUTE void Exec_BlinkLed(void);
MEMORY_ATTRIBUTE void Send_To_4Leds(unsigned char Var8);

#endif



