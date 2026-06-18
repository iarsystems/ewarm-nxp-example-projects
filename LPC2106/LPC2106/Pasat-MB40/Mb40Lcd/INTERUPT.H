#ifndef _INTERUPT_INCLUDED
#define _INTERUPT_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Install_IntHandlers(void);
MEMORY_ATTRIBUTE void MA_IntHandler_Timer1Capture( void );


#endif



