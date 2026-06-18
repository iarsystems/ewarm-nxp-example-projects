#ifndef __CircBuffer__
#define __CircBuffer__

/* Includes */
#include "arm_cm4.h"


/* Error Codes */
#define     OK                  0
#define     NOT_ENOUGH_SPACE    1


/* Extern variables */
extern volatile uint8 *OUT_StartAddress;        
extern volatile uint8 *OUT_EndAddress;          
extern volatile uint8 *OUT_UsbPointer;          
extern volatile uint8 *OUT_SciPointer;          
extern volatile uint8 gu8BufferOverFlow;


/* Prototypes */
void Buffer_Init(uint8* ,uint8);
uint8 Buffer_Request(uint8* ,uint16);


#endif /* __CircBuffer__*/
