/* MAC7100InterruptHandler.h */


#ifndef __MAC7100INTERRUPTHANDLER_H
#define __MAC7100INTERRUPTHANDLER_H


/*
** module fuctions
*/
void MAC7100InstallIRQ( unsigned char INTC_Num, unsigned char INTC_Level, void ( *HandlerFunc )( ) ) ;
void MAC7100RemoveIRQ( unsigned char INTC_Num ) ;


#endif  /* __MAC7100INTERRUPTHANDLER_H */
