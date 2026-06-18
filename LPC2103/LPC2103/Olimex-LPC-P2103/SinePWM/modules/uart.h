/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : uart.h
 *    Description : uart header file
 *
 *    History :
 *    1. Datå        : 15.11.2007 ã.  
 *       Author      : Stoyan Choynev
 *       Description : initial version
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __UART_H
#define __UART_H
/** include files **/
/** definitions **/
#define UART0_BASE (Lpc2103_Uart *)0xE000C000
#define UART1_BASE (Lpc2103_Uart *)0xE0010000

#define FIFO_EN 0


#define TX_EN 7

typedef struct
{
  union
  {//Offset 0x00
    unsigned int  :32;
    unsigned char Rbr;  //Receive buffer register
    unsigned char Thr;  //Trasmite holding register
    unsigned char Dll;  //divisor latch LSB
  };
  
  union
  {//Offser 0x04struct
    struct
    {
      unsigned int RDAIE      : 1;
      unsigned int THREIE     : 1;
      unsigned int RXLSIE     : 1;
      unsigned int MSIE       : 1;
      unsigned int            : 3;
      unsigned int CTSIE      : 1;
      unsigned int ABTOIntEn  : 1;
      unsigned int ABEOIntEn  : 1;
      unsigned int            :22;
    } Ier_bit;
    unsigned int Ier;     //interrupt enable register
    unsigned char Dlm;    //divisor latch MSB
  };
    
  union
  {//Offser 0x08
    struct
    {
      unsigned int IP       : 1;
      unsigned int IID      : 3;
      unsigned int          : 2;
      unsigned int IIRFE    : 2;
      unsigned int ABEOInt  : 1;
      unsigned int ABTOInt  : 1;
      unsigned int          :22;
    } Iir_bit;             //Interrup ID register bits
    unsigned int Iir ;        //Interrup ID register      
    struct {
      unsigned char FCRFE    : 1;
      unsigned char RFR      : 1;
      unsigned char TFR      : 1;
      unsigned char          : 3;
      unsigned char RTLS     : 2;
    } Fcr_bit;             //Fifo Control Register bits 
    unsigned char Fcr;       //Fifo Control Register
  };
  
  union
  {//Offser 0x0C
    unsigned int       :32;
    struct
    {
      unsigned char  WLS   : 2;
      unsigned char  SBS   : 1;
      unsigned char  PE    : 1;
      unsigned char  PS    : 2;
      unsigned char  BC    : 1;
      unsigned char  DLAB  : 1;
    } Lcr_bit;   
    unsigned char Lcr ;   //Line Control Register
  };
  
  union
  {//Offset 0x10
    unsigned int       :32 ;
    struct
    {
      unsigned char  DTR    : 1;
      unsigned char  RTS    : 1;
      unsigned char         : 2;
      unsigned char  LMS    : 1;
      unsigned char         : 1;
      unsigned char  RTSen  : 1;
      unsigned char  CTSen  : 1;
    } Mcr_bit;
    unsigned char Mcr ;   //Mocem Control register
  };
  
  union
  {//Offset 0x14
    unsigned int       :32 ;
    struct _Lsr
    {
      unsigned char  RDR   : 1;
      unsigned char  OE    : 1;
      unsigned char  PE    : 1;
      unsigned char  FE    : 1;
      unsigned char  BI    : 1;
      unsigned char  THRE  : 1;
      unsigned char  TEMT  : 1;
      unsigned char  RXFE  : 1;
    } Lsr_bit;
    unsigned char Lsr;
  };
  
  union
  {//Offser 0x18
    unsigned int       :32 ;   
    union
    {
    //U1MSR
      struct
      {
        unsigned char  DCTS  : 1;
        unsigned char  DDSR  : 1;
        unsigned char  TERI  : 1;
        unsigned char  DDCD  : 1;
        unsigned char  CTS   : 1;
        unsigned char  DSR   : 1;
        unsigned char  RI    : 1;
        unsigned char  DCD   : 1;
      };
      //U1MSR
      struct 
      {
        unsigned char  MSR0  : 1;
        unsigned char  MSR1  : 1;
        unsigned char  MSR2  : 1;
        unsigned char  MSR3  : 1;
        unsigned char  MSR4  : 1;
        unsigned char  MSR5  : 1;
        unsigned char  MSR6  : 1;
        unsigned char  MSR7  : 1;
      };
    } Msr_bit;
    unsigned char Msr ;  
  };
  
  union
  {//Offser 0x1C
    unsigned int  :32 ;
    unsigned char Scr ;   //Scratch pad register
  };
  
  union
  {//Offset 0x20
    struct
    {
      unsigned int Start        : 1;
      unsigned int Mode         : 1;
      unsigned int AutoRestart  : 1;
      unsigned int              : 5;
      unsigned int ABEOIntClr   : 1;
      unsigned int ABTOIntClr   : 1;
      unsigned int              :22;
    } Acr_bit;
    unsigned char Acr ;   //Auto-Baud control Register
  };
  //Offser 0x24
  unsigned int   :32;        //Reserved 
  
  union
  {//Offser 0x28
    struct
    {
      unsigned int DIVADDVAL  : 4;
      unsigned int MULVAL     : 4;
      unsigned int            :24;
    } Fdr_bit ;
    unsigned int Fdr;    //Fractional Divider Register
  };
  //Offser 0x2C
  unsigned int   :32;        //Reserved
  //Offser 0x30
  union
  {
    unsigned int       :32 ;   
    struct
    {
      unsigned char        : 7;
      unsigned char  TXEN  : 1;
    } Ter_bit;
    
    unsigned char Ter;    //Transmite Enable Register
  };
} Lpc2103_Uart ;
/** default settings **/

/** external functions **/
//extern void UartIntIsr (void);
extern void UartInit (Lpc2103_Uart *);
extern void putchar (Lpc2103_Uart *, unsigned char );
extern void UartSendString (unsigned char *, Lpc2103_Uart *);
extern int getchar(Lpc2103_Uart *);
//extern unsigned char UartRead(void);

/** external data **/
//extern Uart_tIOFlags UartIOFlags;
#endif
