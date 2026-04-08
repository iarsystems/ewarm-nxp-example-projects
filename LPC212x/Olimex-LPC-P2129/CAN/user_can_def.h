
/*
*****************************************************************************
**
**      Project     : EW CAN Library for NXP LPC2129
**
**      Modulename  : CAN
**
**      Filename    : user_can_def.h
**
**      Abstract    : This is the application specific CAN definition 
**                    header file used by the <ew_can.c> module.
**
**                    Update the CAN module settings and definitions 
**                    according to application requirements.
**
**      Date        : 2005-12-05   
**
**      Version     : 1.00A
**      
**      (c) Copyright IAR Systems 2005
**      
**      THE SOFTWARE IS PROVIDED "AS IS"  WITHOUT ANY WARRANTY OF ANY KIND,
**      IAR SYSTEMS DOES NOT WARRANT THAT THE SOFTWARE WILL BE ERROR-FREE 
**      OR THAT SUCH ERRORS WILL BE CORRECTED, AS A RESULT, IAR SYSTEMS 
**      SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL 
**      DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH 
**      SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION 
**      CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**
*****************************************************************************
*/

/*--- Avoid including this file more than once ---*/
#ifndef _IS_INCLUDED_EW_CAN_DEF_H
#define _IS_INCLUDED_EW_CAN_DEF_H


/*
**===========================================================================
**  CAN module settings
**===========================================================================
*/

/*
**------------------------------------------------------------------------
** According to 'NXP ERRATA SHEET´, August 5, 2005 - Version 1.8
** it is not recommended to use more than 1 TX buffer. 
** Therefor normally the CAN driver only uses 1 of the 3 TX buffers. 
**
** Add the definition USE_THREE_TX_BUFFERS only if all three TX buffers 
** shall be used by the EW_SendData_CAN() device driver. 
**
** INFO: The application <usercode.c> has been able to run also using all
** three TX buffers. 
**------------------------------------------------------------------------
*/
//  define USE_THREE_TX_BUFFERS

/*
**------------------------------------------------------
** CAN pin usage
** 0x00054000 gives RD2, TD2, and RD1 are enabled 
** Note! TD1 is a unique pin (#10) in LPC2119/2129/2194 
**------------------------------------------------------
*/
#define EW_CAN_PINSEL1  0x00054000

/*
**--------------------------------------------------------------------------
** CAN mode used
** bit 0  RMt =0  Not reset mode
** bit 1  LOM =0  Can controller acknowledges successfully received message
** bit 2  STM =0  Transmitted message needs to be acknowledged
** bit 3  TPM =1  Priority depends on Tx Priority field
** bit 4  SM  =0  Normal operation
** bit 5  RPM =0  RX and TX pins are low for a dominant bit
** bit 7  TM  =0  Normal operation
**
** Setting gives C1MOD = C2MOD = 0x08 
**--------------------------------------------------------------------------
*/
#define EW_C1MOD_CAN  0x08  
#define EW_C2MOD_CAN  0x08  

/*
**---------------------------------------------------------
** CAN interrupt handling
** CAN1 interrupt disabled (will be polled by application)
** CAN2 RX interrupt enabled 
**---------------------------------------------------------
*/
#define EW_C1IER_CAN  0x0000
#define EW_C2IER_CAN  0x0001

/*
**-------------------------------------------------------------
** BITRATE calculation  
** Example IAR Kickstart kit LPC2129-KS  Frequency 14.7456 MHz 
**
** CAN clock setting for 100 usec
** CLK = 1/14745600
** PCLK = 1/(4*CLK)  (VPBDIV=0) -> PCLK = CLK/4
**
** Example
** BRP = 0x16 =22       SJW = 0
** TSEG1 = 0xC=12       TSEG2 = 1
**
**         (BRP+1) * ( (SJW+1)+(TSEG1+1)+(TSEG2+1) )
** BIT =  -------------------------------------------
**                          PCLK
**
**         23 * ( 1+13+2 )             23*16
** BIT =  --------------------- =  ------------- = 100 usec
**            14745600 / 4         14745600 / 4
**
**
** CANBTR register setting (C1BTR and C1BTR)
** SAM=0 Bus is sampled once
** TSEG1 = 0xC=12
** TSEG2 = 1
** SJW = 0
** BRP = 0x16 =22
**
**-------------------------------------------------------------
*/
#define EW_C1BTR_CAN  0x001C0016
#define EW_C2BTR_CAN  0x001C0016

/*
**----------------------------------------------------------------------------
** Acceptance Filter Modes 
** AcceptanceFilterOff    =1  All Rx messages on all CAN buses are ignored
** AcceptanceFilterOn     =0  Acceptance filter is used to screen Rx messages
** AcceptanceFilterBypass =3  Filter bypass
** AcceptanceFilterFullCAN=4  FullCAN messages automatically stored
**----------------------------------------------------------------------------
*/
/*--- Don't change these values as they maps into AFMR bits ---*/
#define AcceptanceFilterOff      0x01
#define AcceptanceFilterOn       0x00
#define AcceptanceFilterBypass   0x03
#define AcceptanceFilterFullCAN  0x04

/*--- Select the filter mode to be used by the CAN driver for your application ---*/
#define EW_AFMR_CAN  AcceptanceFilterFullCAN 


/*
**===========================================================================
**  Application specific code
**===========================================================================
*/

/*
**-----------------------------------------------------------------------------
**  User specific code to be handled by the ew_can module can be included
**  in this header file. 
**      E.g 
**      1. Declare global variables. 
**      2. Update the user code macros. 
**
**  As this header file and user code macros are included in the <ew_can.c> 
**  file we do not need to change the library source code. 
**
**  EXAMPLE:
**  The application needs global message variables and counters to be 
**  incremented/decremented in the receive CAN interrupt function using macros.
** 
**-----------------------------------------------------------------------------
*/
/*--- Declare global variables ---*/
extern CAN_MSG RxMsg0;
extern CAN_MSG RxMsg1;
extern CAN_MSG RxMsg2;

extern U32 CANInterruptCount1;
extern U32 CANInterruptCount2;
extern U32 CAN1ii;
extern U32 CAN2ii;

/*--- Define user code macros ---*/
#define ENTER_EW_INIT_CAN
#define EXIT_EW_INIT_CAN

#define ENTER_EW_ADDFILTER_CAN
#define EXIT_EW_ADDFILTER_CAN

#define ENTER_EW_MANAGE_CAN
#define EXIT_EW_MANAGE_CAN

#define ENTER_EW_RECEIVEDATA_CAN
#define EXIT_EW_RECEIVEDATA_CAN

#define ENTER_EW_SENDDATA_CAN
#define EXIT_EW_SENDDATA_CAN

#define ENTER_EW_INTHANDLER_RX_CAN1     
#define INSIDE_EW_INTHANDLER_RX_CAN1    
#define EXIT_EW_INTHANDLER_RX_CAN1      

#define ENTER_EW_INTHANDLER_RX_CAN2     CANInterruptCount2++;
#define INSIDE_EW_INTHANDLER_RX_CAN2    CAN2ii--;
#define EXIT_EW_INTHANDLER_RX_CAN2      EW_ReceiveData_CAN( 2, &RxMsg2 );

#define ENTER_EW_INTHANDLER_TX_CAN1
#define INSIDE_EW_INTHANDLER_TX_CAN1
#define EXIT_EW_INTHANDLER_TX_CAN1

#define ENTER_EW_INTHANDLER_TX_CAN2
#define INSIDE_EW_INTHANDLER_TX_CAN2
#define EXIT_EW_INTHANDLER_TX_CAN2

#define ENTER_EW_INTHANDLER_OTHER_CAN
#define INSIDE_EW_INTHANDLER_OTHER_WARNING_CAN
#define INSIDE_EW_INTHANDLER_OTHER_OVERRUN_CAN
#define INSIDE_EW_INTHANDLER_OTHER_WAKEUP_CAN
#define INSIDE_EW_INTHANDLER_OTHER_PASSIVE_CAN
#define INSIDE_EW_INTHANDLER_OTHER_ARBITRATION_CAN
#define INSIDE_EW_INTHANDLER_OTHER_BUSERROR_CAN
#define INSIDE_EW_INTHANDLER_OTHER_IDREADY_CAN
#define EXIT_EW_INTHANDLER_OTHER_CAN

#endif /* Match the re-definition guard */

/*
**===========================================================================
** END OF FILE
**===========================================================================
*/
