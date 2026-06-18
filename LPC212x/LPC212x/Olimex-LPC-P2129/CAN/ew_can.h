
/*
*****************************************************************************
**
**      Project     : EW CAN Library for NXP LPC2129
**
**      Modulename  : CAN
**
**      Filename    : ew_can.h
**
**      Abstract    : This is the device driver header file for the CAN
**                    module.

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


/*
**===========================================================================
**  1       GENERAL
**  1.1     Revisions
**
**===========================================================================
*/

/*
**===========================================================================
**  1.2     References
**
**  No   Identification          Name or Description
**  ==   ===================     =======================================
**  1    May 03, 2004            NXP LPC2119/2129/2194/2292/2294
**                               User Manual, Preliminary Release
**  2    August 5, 2005          NXP ERRATA SHEET - Version 1.8 LPC2129
**
**===========================================================================
*/

/*
**===========================================================================
**  1.3     Re-definition guard
**===========================================================================
*/

/*--- Avoid including this file more than once ---*/
#ifndef _IS_INCLUDED_EW_CAN_H
#define _IS_INCLUDED_EW_CAN_H

/*
**===========================================================================
**  2.      INCLUDE FILES
**  2.1     Standard include files
**===========================================================================
*/

/*
**===========================================================================
**  2.2     Application include files
**===========================================================================
*/

/*
**===========================================================================
**  3.      DECLARATIONS
**  3.1     Global constants
**===========================================================================
*/

#define C8      char
#define U8      unsigned char
#define U16     unsigned short
#define U32     unsigned long
#define S8      signed char
#define S16     signed short
#define S32     signed long


/*--- Max number of CAN_CONTROLLERS ---*/
#define CAN_CONTROLLERS  2
/*---   CAN_CONTROLLERS  4 ---*/

/*--- Return codes ---*/
#define EW_OK            1
#define EW_DATA          2
#define EW_RTR           3

#define EW_ERROR        -1
#define EW_FULL         -2
#define EW_EMPTY        -3
#define EW_BUSY         -4

/*--- CAN status codes ---*/
#define EW_CAN_RECEIVED        1
#define EW_CAN_OVERRUN         2
#define EW_CAN_TRANSMITTED     4
#define EW_CAN_ERROR           8
#define EW_CAN_BUS_ERROR       16
#define EW_CAN_FILTER_ERROR    32

/*--- CAN message control information constants ---*/
#define EW_CAN_CTRL_STANDARD_ID  0
#define EW_CAN_CTRL_EXTENDED_ID  0x80000000L
#define EW_CAN_CTRL_RTR          0x40000000L

/*--- CAN Filter type ---*/
#define EW_CAN_FULL            1
#define EW_CAN_STD             2
#define EW_CAN_STD_GRP         3
#define EW_CAN_EXT             4
#define EW_CAN_EXT_GRP         5

/*
**---------------------------------------------------------------------------
**  CAN Interrupt and Capture Register bits (C1ICR, C2ICR, C3ICR, C4ICR)
**---------------------------------------------------------------------------
*/
#define CAN_ICR_ALCBIT  0x1f000000  /* Bit 28-24: ALCBIT (Arbitration lost count bit) */
#define CAN_ICR_ERRC    0xc00000    /* Bit 23-22: ERRC (Error type code) */
#define CAN_ICR_ERRDIR  0x200000    /* Bit 21: ERRDIR (Error direction) */
#define CAN_ICR_ERRBIT  0x1f0000    /* Bit 20-16: ERRBIT (Error code capture) */
#define CAN_ICR_TI3     0x400       /* Bit 10: TI3 (Transmit interrupt 3) */
#define CAN_ICR_TI2     0x200       /* Bit 9: TI2 (Transmit interrupt 2) */
#define CAN_ICR_IDI     0x100       /* Bit 8: IDI (ID Ready interrupt) */
#define CAN_ICR_BEI     0x80        /* Bit 7: BEI (Bus error interrupt) */
#define CAN_ICR_ALI     0x40        /* Bit 6: ALI (Arbitration lost interrupt) */
#define CAN_ICR_EPI     0x20        /* Bit 5: EPI (Error passive interrupt) */
#define CAN_ICR_WUI     0x10        /* Bit 4: WUI (Wake-up interrupt) */
#define CAN_ICR_DOI     0x08        /* Bit 3: DOI (Data overrun interrupt) */
#define CAN_ICR_EI      0x04        /* Bit 2: EI (Error warning interrupt) */
#define CAN_ICR_TI1     0x02        /* Bit 1: TI1 (Transmit interrupt 1) */
#define CAN_ICR_RI      0x01        /* Bit 0: RI (Receive interrupt) */

/*
**===========================================================================
**  3.2     Global macros
**===========================================================================
*/

/*
**===========================================================================
**  3.3     Global type definitions
**===========================================================================
*/

/*
**===========================================================================
**  3.4     Global variables (defined in some implementation file)
**===========================================================================
*/
typedef struct
{
    U32 CTRL;       /* bit 0-7=PRIO bits  (Priority,0-255) */
                    /* bit 16-19=DLC bits (Datalength,1-8) */
                    /* bit 24-26=CAN Controller (1-4) only used when fullCAN message received */
                    /* bit 30=RTR bit     (EW_CTRL_RTR)    */
                    /* bit 31=FF bit      (EW_CTRL_FF)     */
    U32 ID;         /* Extended ID bits [28:0]             */
                    /* Standard ID bits [10:0]             */
    U32 Data1;      /* Message data A                      */
    U32 Data2;      /* Message data B                      */
} CAN_MSG;


typedef struct
{
    U8 FilterType;  /* CAN_FULL, CAN_STD, CAN_STD_GRP, CAN_EXT, CAN_EXT_GRP */
    U8 Controller1; /* Controller ID1, 1=CAN1, 2=CAN2,3=CAN3,4=CAN4 */
    U8 Controller2; /* Controller ID2, 1=CAN1, 2=CAN2,3=CAN3,4=CAN4 */
    U8 Disable1;    /* 0 = Enabled, 1 = Disabled */
    U8 Disable2;    /* 0 = Enabled, 1 = Disabled */
    U32 ID1;        /* ID 1 (lower bound for standard/extended id group)  */
    U32 ID2;        /* ID 2 (upper bound for standard/extended id group)  */
} CAN_Filter;


/*
**===========================================================================
**  4.      GLOBAL FUNCTIONS (defined in some implementation file)
**===========================================================================
*/
extern void EW_Init_CAN( void );
extern S16  EW_AddFilter_CAN( CAN_Filter *pNewFilter );
extern S16  EW_Manage_CAN( U8 Controller );
extern S16  EW_ReceiveData_CAN( U8 Controller, CAN_MSG *pRxMsg );
extern S16  EW_SendData_CAN( U8 Controller, CAN_MSG *pTxMsg );

void EW_IntHandler_RX_CAN1( void );
void EW_IntHandler_RX_CAN2( void );
void EW_IntHandler_TX_CAN1( void );
void EW_IntHandler_TX_CAN2( void );
void EW_IntHandler_OTHER_CAN( void );

#endif /* Match the re-definition guard */

/*
**===========================================================================
** END OF FILE
**===========================================================================
*/
