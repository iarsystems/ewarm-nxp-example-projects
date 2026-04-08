
/*
*****************************************************************************
**
**      Project     : EW CAN Library for NXP LPC2129
**
**      Modulename  : Application code
**
**      Filename    : usercode.c
**
**      Abstract    : This file contain application source code for NXP
**                    LPC2129. The application uses the CAN module and
**                    send messages between the two CAN controllers CAN1
**                    and CAN2. The CAN library <ew_can.c> is used and
**                    the <user_can.h> contain application specific settings
**                    for the library.
**
**      Hardware    : IAR KickStart Kit for NXP LPC2129
**                    1. Power supply:
**                      The Kickstart kit needs to be supplied with an
**                      external power supply (7Volt has been used) if
**                      the external CAN bus shall be operable.
**
**                    2. Connection:
**                    Connect the CAN D-SUB connectors on the board:
**                        CAN1                CAN2
**                        pin2 ---- CANL ---- pin2
**                        pin7 ---- CANH ---- pin7
**
**      Software    : CAN module setting:
**                    These settings are made in the <user_can.h> file
**
**                    1. CAN pin usage - RD2, TD2, and RD1 are enabled
**                       (TD1 is by default enabled in LPC2129)
**
**                    2. CAN Mode is set as Priority depends on Tx
**                     priority field.
**
**                    3. CAN Interrupt
**                     CAN1 interrupt disabled (polled by application)
**                     CAN2 RX interrupt enabled
**
**                    4. CAN Bit timing
**                          Clock = 100 usec/bit
**                          SAM    =0 Bus is sampled once
**                          TSEG1 = 0xC=12
**                          TSEG2 = 1
**                          SJW   = 0
**                          BRP   = 0x16 =22
**
**                    5. Acceptance filter mode
**                    AcceptanceFilter Full CAN message is used.
**
**                    6. User code macros for CAN2 RX interrupts are defined
**
**      Overview    : 1. GPIO, and interrupt are initialized
**                    2. CAN module is initialized using EW_Init_CAN()
**                    3. Used CAN IDs are initialized using EW_AddFilter_CAN()
**                       The CAN filter uses the 5 different filter types
**                       A. Full standard ID messages
**                       B. Standard ID messages
**                       C. Standard Group ID messages
**                       D. Extended ID messages
**                       E. Extended Group ID messages
**                    4. CAN messages are sent between CAN1 and CAN2
**                       CAN1 is polled to detect received CAN message
**                       CAN2 uses receive interrupt to detect rx message
**                       The LEDs will toggle if CAN messages are running
**                       Button B1 can be used to stop CAN messages
**                       Button B2 can be used to start CAN messages
**
**      Functions   : main()
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

/*
**===========================================================================
**  1       GENERAL
**  1.1     Revisions
**===========================================================================
*/

/*
**===========================================================================
**  1.2     References
**===========================================================================
*/

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
#include "NXP/iolpc2129.h"      /* Defines Special function registers */
#include <intrinsics.h>     /* Intrinsic functions */
#include "ew_can.h"         /* EW CAN library header file */
#include "user_can_def.h"   /* Application specific CAN definitions */

/*
**===========================================================================
**  3.      DECLARATIONS
**  3.1     Internal constants
**===========================================================================
*/

/*
**===========================================================================
**  3.2     Internal macros
**===========================================================================
*/

/*
**===========================================================================
**  3.3     Internal type definitions
**===========================================================================
*/

/*
**===========================================================================
**  3.4     Global variables (declared as 'extern' in some header file)
**===========================================================================
*/

/*
**===========================================================================
**  3.5     Internal function prototypes (defined in Section 5)
**===========================================================================
*/
__irq __arm void IRQ_Handler(void);

/*
**===========================================================================
**  3.6     Internal variables
**===========================================================================
*/
/*--- CAN receive and transmitte messages */
CAN_MSG RxMsg0;
CAN_MSG RxMsg1;
CAN_MSG RxMsg2;
CAN_MSG TxMsg;

/*--- Counters ---*/
U32 CANInterruptCount1;
U32 CANInterruptCount2;
U32 CAN1ii;
U32 CAN2ii;

U32 CANRunFlag;     /* Flag used to enable/disable CAN messages */


/*
**----------------------------------------------------------------------------------------
** CAN Filter table
** These tables are installed into the CAN filter table by calling the
** EW_AddFilter_CAN() function.
**
** It is not required that the items in the Filter table are sorted as EW_AddFilter_CAN()
** function installes new items into the CAN filter at a sorted location.
**----------------------------------------------------------------------------------------
*/
/*--- CAN1 Filters ---*/
CAN_Filter CAN1Filter[] = {
/* Filter         Controller, Disable,  ID,        ID       */
/*  Type           1  2        1  2     1          2        */
{ EW_CAN_FULL,     1, 1,       0, 0,    0x14,      0x15  },    /* Standard ID 0x14, 0x15 */
{ EW_CAN_FULL,     1, 1,       0, 0,    0x16,      0x17  },    /* Standard ID 0x16, 0x17 */
{ EW_CAN_FULL,     1, 1,       0, 0,    0x18,      0x1E  },    /* Standard ID 0x18, 0x1E */
{ EW_CAN_FULL,     1, 1,       0, 0,    0x20,      0x22  },    /* Standard ID 0x20, 0x22 */
{ EW_CAN_STD,      1, 1,       0, 0,    0x140,     0x142 },    /* Standard ID 0x140, 0x142 */
{ EW_CAN_STD_GRP,  1, 1,       0, 0,    0x203,     0x206 },    /* Standard ID 0x203-0x206 */
{ EW_CAN_STD_GRP,  1, 1,       0, 0,    0x500,     0x580 },    /* Standard ID 0x500-0x580 */
{ EW_CAN_EXT,      1, 1,       0, 0,    0x123456,  0 },        /* Extended ID 0x123456 */
{ EW_CAN_EXT,      1, 1,       0, 0,    0x222333,  0 },        /* Extended ID 0x222333 */
{ EW_CAN_EXT_GRP,  1, 1,       0, 0,    0x300000,  0x400000 }, /* Extended ID 0x300000-0x400000 */
{ 0,            0, 0,       0, 0,    0,         0 },        /* End of table */
};

/*--- CAN2 Filters ---*/
CAN_Filter CAN2Filter[] = {
/* Filter         Controller, Disable,  ID,        ID       */
/*  Type           1  2        1  2     1          2        */
{ EW_CAN_FULL,     2, 2,       0, 0,    0x10,      0x12  },    /* Standard ID 0x10, 0x12 */
{ EW_CAN_FULL,     2, 2,       0, 0,    0x23,      0x24  },    /* Standard ID 0x23, 0x24 */
{ EW_CAN_FULL,     2, 2,       0, 0,    0x26,      0x27  },    /* Standard ID 0x26, 0x27 */
{ EW_CAN_FULL,     2, 2,       0, 0,    0x29,      0x2A  },    /* Standard ID 0x29, 0x2A */
{ EW_CAN_STD,      2, 2,       0, 0,    0x150,     0x151 },    /* Standard ID 0x150, 0x151 */
{ EW_CAN_STD_GRP,  2, 2,       0, 0,    0x600,     0x620 },    /* Standard ID 0x600-0x620 */
{ EW_CAN_STD_GRP,  2, 2,       0, 0,    0x700,     0x780 },    /* Standard ID 0x700-0x780 */
{ EW_CAN_EXT,      2, 2,       0, 0,    0x334455,  0 },        /* Extended ID 0x334455 */
{ EW_CAN_EXT,      2, 2,       0, 0,    0x444666,  0 },        /* Extended ID 0x444555 */
{ EW_CAN_EXT_GRP,  2, 2,       0, 0,    0x500000,  0x600000 }, /* Extended ID 0x500000-0x600000 */
{ 0,            0, 0,       0, 0,    0,         0 },        /* End of table */
};


/*--- CAN messages sent from CAN1 (to be received by CAN2 ---*/
CAN_MSG Can1Test[] = {
/* CTRL                                                   | ID         | Data1      | Data2       */
/* Priority,0-255, DLC bits, RTR,     Frame format        | ID         | Data1      | Data2       */
{   1            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x10,        0x11111111L, 0x22222222L },  // 0
{   2            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x12,        0x33333333L, 0x44444444L },  // 1
{   3            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x23,        0x11111111L, 0x22222222L },  // 2
{   4            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x24,        0x11111111L, 0x22222222L },  // 3
{   5            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x26,        0x33333333L, 0x44444444L },  // 4
{   6            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x27,        0x33333333L, 0x44444444L },  // 5
{   7            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x29,        0x33333333L, 0x44444444L },  // 6
{   0            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x2A,        0x33333333L, 0x44444444L },  // 7
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x150,       0x33333333L, 0x44444444L },  // 8
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x600,       0x33333333L, 0x44444444L },  // 9
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x620,       0x33333333L, 0x44444444L },  // 10
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x700,       0x33333333L, 0x44444444L },  // 11
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x780,       0x33333333L, 0x44444444L },  // 12
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x334455L,   0x12345678L, 0x12345678L },  // 13
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x444666L,   0x12345678L, 0x12345678L },  // 14
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x500000L,   0x12345678L, 0x12345678L },  // 15
};


/*--- CAN messages sent from CAN2 (to be received by CAN1 ---*/
CAN_MSG Can2Test[] = {
/* CTRL                                               | ID         | Data1      | Data2       */
/* Priority,0-255, DLC bits, RTR, Frame format        | ID         | Data1      | Data2       */
{   0            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x20,        0x55555555L, 0x66666666L },  // 0
{   1            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x22,        0x77777777L, 0x88888888L },  // 1
{   2            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x14,        0x77777777L, 0x88888888L },  // 2
{   3            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x15,        0x77777777L, 0x88888888L },  // 3
{   4            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x16,        0x77777777L, 0x88888888L },  // 4
{   5            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x17,        0x77777777L, 0x88888888L },  // 5
{   6            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x18,        0x77777777L, 0x88888888L },  // 6
{   7            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x1E,        0x77777777L, 0x88888888L },  // 7
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x140,       0x77777777L, 0x88888888L },  // 8
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x142,       0x77777777L, 0x88888888L },  // 9
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x204,       0x77777777L, 0x88888888L },  // 10
{   8            + (8<<16) +  0  +EW_CAN_CTRL_STANDARD_ID, 0x500,       0x77777777L, 0x88888888L },  // 11
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x123456L,   0x12345678L, 0x12345678L },  // 12
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x222333L,   0x12345678L, 0x12345678L },  // 13
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x300000L,   0x77777777L, 0x88888888L },  // 14
{   8            + (8<<16) +  0  +EW_CAN_CTRL_EXTENDED_ID, 0x400000L,   0x77777777L, 0x88888888L },  // 15
};


/*
**===========================================================================
**  4.      GLOBAL FUNCTIONS (declared as 'extern' in some header file)
**===========================================================================
*/

void feed (void)
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      Feed sequence for the PLL
**
**  Parameters:
**      None
**
**  Returns:
**      None
**
**---------------------------------------------------------------------------
*/
{
  PLLFEED=0xAA;
  PLLFEED=0x55;
}

int main( void )
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      User application
**
**  Parameters:
**      None
**
**  Returns:
**      None
**
**---------------------------------------------------------------------------
*/
{
    U32 ii, jj;
    U32 CAN1msg;
    U32 CAN2msg;
    U32 volatile *pAddrReg;
    U16 Status;

    // The MAMTIM value would depend upon the system clock desired
    // MAM is fully enabled below
    MAMCR_bit.MODECTRL=0x0;   // Disabel MAM
    MAMTIM_bit.CYCLES =0x1;   // Change MAM clock divider
    MAMCR_bit.MODECTRL=0x2;   // Enable MAM

    // Setup the system clock to the desired clock frequency
    // The PLLCFG value is provided for input frequency of 14.7456 MHz
    // Disabe PLL
    PLLCON = 0;
    feed();

    // Set the peripheral clock. Here we set it to 14.7456MHz/4
    APBDIV=0x0;

    /*
    **----------------
    ** Initialise CAN
    **----------------
    */
    EW_Init_CAN();

    /*
    **------------------------------------
    ** Initialise GPIO - Buttons and LEDs
    **------------------------------------
    */
    /*--- Buttons on P0.15 + P0.16, input pins ---*/
    PINSEL0_bit.P0_15 = 0;
    PINSEL1_bit.P0_16 = 0;

    IO0DIR_bit.P0_15 = 0;   /* B1 */
    IO0DIR_bit.P0_16 = 0;   /* B2 */

    /*--- LEDs on P0.12 + P0.13, output pins ---*/
    PINSEL0_bit.P0_12 = 0;
    PINSEL0_bit.P0_13 = 0;

    IO0DIR_bit.P0_12 = 1;
    IO0DIR_bit.P0_13 = 1;

    /*--- Handle LEDs ---*/
    IO0CLR_bit.P0_12 = 1;   /* Yellow led ON */
    IO0CLR_bit.P0_13 = 1;   /* Green led ON */

    /*
    **----------------------------
    ** Setup interrupt controller
    **----------------------------
    */
    /*--- Disable interrupts ---*/
    __disable_interrupt();

    /*--- Enable VIC registers access ---*/
    VICProtection = 0;

    /*--- Enable CAN1 Rx (=bit 26 0x04000000L ) ---*/
    /*--- Enable CAN2 Rx (=bit 27 0x08000000L ) ---*/
    VICIntEnable = 0x0C000000L;  /* CAN1 interrupt is enabled but not used */

    /*--- Select interrupt type, all are IRQ type, no FIQ type ---*/
    VICIntSelect = 0;

    /*
    **---------------------------------------------
    ** Enable vector IRQ slot
    ** Bit5    Enabled if =1 (0x20)
    ** Bit4:0  The number of the interrupt request
    **          CAN1 Rx 0x1A
    **          CAN2 Rx 0x1B
    **---------------------------------------------
    */
    /*--- Slot 1, Assign vector CAN1 Rx to this slot ---*/
    /*
    ** This vector is not used
    ** VICVectCntl1 = 0x3A;
    */

    /*--- Slot 2, Assign vector CAN2 Rx to this slot ---*/
    VICVectCntl2 = 0x3B;

    /*
    **--------------------------------------------
    ** Initialise used interrupt service routines
    ** Load registers with the ISR start address
    **--------------------------------------------
    */
    /*--- Slot 1 ---*/
    /*
    ** This slot is not used
    ** pAddrReg = &VICVectAddr0 + 1;
    ** *pAddrReg = (unsigned int)&EW_IntHandler_RX_CAN1;
    */

    /*--- Slot 2 ---*/
    pAddrReg = &VICVectAddr0 + 2;
    *pAddrReg = (unsigned int)&EW_IntHandler_RX_CAN2;

    /*--- Enable interrupts ---*/
    __enable_interrupt();

    /*
    **------------------
    ** Load CAN filters
    **------------------
    */
    ii= 0;
    while ( CAN1Filter[ii].FilterType )
    {
        /*--- Add new filter, new ID is sorted into filter table ---*/
        EW_AddFilter_CAN( &CAN1Filter[ii] );
        ii++;
    }
    ii= 0;
    while ( CAN2Filter[ii].FilterType )
    {
        /*--- Add new filter, new ID is sorted into filter table ---*/
        EW_AddFilter_CAN( &CAN2Filter[ii] );
        ii++;
    }

    /*
    **---------------------------------------
    ** Initialize and send first CAN message
    **---------------------------------------
    */
    TxMsg.CTRL = Can1Test[0].CTRL;
    TxMsg = Can1Test[0];
    EW_SendData_CAN( 1, &TxMsg );

    /*--- Initialize variables ---*/
    CANRunFlag = 1;
    CAN2ii = 1;
    CAN1msg = 0;
    CAN2msg = 0;

    /*
    **------------------
    ** Application code
    **------------------
    */
    while( 1 )
    {
        /*
        **------------------------
        ** Read button B1 and B2
        ** B1 stops CAN messages
        ** B2 starts CAN messages
        **------------------------
        */
        /*--- stop CAN? ---*/
        if(!IO0PIN_bit.P0_15)
        {
            /*--- B1 pressed - Bounce delay ---*/
            for (jj=0; jj<0x100; jj++)
            {
                ;
            }
            if(!IO0PIN_bit.P0_15)
            {
                /*--- B1 pressed - stop CAN message ---*/
                CANRunFlag = 0;
            }
        }

        /*--- start CAN? ---*/
        if(!IO0PIN_bit.P0_16)
        {
            /*--- B2 pressed - Bounce delay ---*/
            for (jj=0; jj<0x100; jj++)
            {
                ;
            }
            if(!IO0PIN_bit.P0_16)
            {
                /*--- B2 pressed - start CAN message ---*/
                CANRunFlag = 1;
            }
        }

        /*
        **-------------------------------------------------------
        ** Poll CAN controller 1 to find if new message received
        ** CAN controller 2 is handled via interrupt
        **-------------------------------------------------------
        */
        Status = EW_Manage_CAN(1);

        /*--- Error?  ---*/
        if( Status & ( EW_CAN_OVERRUN |
                       EW_CAN_ERROR |
                       EW_CAN_BUS_ERROR |
                       EW_CAN_FILTER_ERROR ))
        {
            /*--- Yes, some error, loop forever ---*/
            while (1)
            {
                ;
            }
        }
        /*--- CAN1 received message? ---*/
        if (Status & EW_CAN_RECEIVED )
        {
            /*--- CAN 1 has received data ---*/
            Status = EW_ReceiveData_CAN( 1, &RxMsg1 );
            if( Status == EW_DATA)
            {
                /*--- CAN1 Received data ---*/
                CAN1ii--;
            }
        }

        /*
        **----------------------------------
        ** Test if fullCAN message received
        ** via CAN1 or CAN2 filters
        **----------------------------------
        */
        Status = EW_ReceiveData_CAN( 0, &RxMsg0 );
        if( Status == EW_DATA)
        {
            /*--- Yes, new fullCAN message detected ---*/
            if(( RxMsg0.CTRL & 0x7000000L ) == 0x1000000L )
            {
                /*--- New message received and filtered by CAN1 ---*/
                __disable_interrupt();
                CAN1ii--;
                __enable_interrupt();
            }
            else if(( RxMsg0.CTRL & 0x7000000L ) == 0x2000000L )
            {
                /*--- New message received and filtered by CAN2 ---*/
                __disable_interrupt();
                CAN2ii--;
                __enable_interrupt();
            }
            else
            {
                /*--- ERROR - wrong Controller detected, loop forever ---*/
                while (1)
                {
                    ;
                }
            }
        }

        /*
        **----------------------------------
        ** Send new CAN messages?
        **----------------------------------
        */
        if( CANRunFlag )
        {
            if( CAN1ii == 0 )
            {
                /*
                **-------------------------------------
                ** CAN2 has received all messages
                ** Send new messages from CAN1 to CAN2
                **-------------------------------------
                */
                CAN1ii = 0xff;
                CAN2ii = 0;

                CAN1msg++;
                TxMsg = Can1Test[(CAN1msg & 0xf)];
                TxMsg.Data1 = CAN1msg;
                TxMsg.Data1 = CAN2ii-1;
                TxMsg.Data2 = CAN2ii++;
                EW_SendData_CAN( 1, &TxMsg );

#ifdef USE_THREE_TX_BUFFERS
                CAN1msg++;
                TxMsg = Can1Test[(CAN1msg & 0xf)];
                TxMsg.Data1 = CAN1msg;
                TxMsg.Data1 = CAN2ii-1;
                TxMsg.Data2 = CAN2ii++;
                EW_SendData_CAN( 1, &TxMsg );

                CAN1msg++;
                TxMsg = Can1Test[(CAN1msg & 0xf)];
                TxMsg.Data1 = CAN1msg;
                TxMsg.Data1 = CAN2ii-1;
                TxMsg.Data2 = CAN2ii++;
                EW_SendData_CAN( 1, &TxMsg );
#endif
                /*--- LED toggle ---*/
                if(CAN1msg & 0x1)
                {
                    /*--- YELLOW LED ON ---*/
                    IO0CLR_bit.P0_12 = 1;
                }
                else
                {
                    /*--- YELLOW LED OFF ---*/
                    IO0SET_bit.P0_12 = 1;
                }
            }

            if( CAN2ii == 0 )
            {
                /*
                **-------------------------------------
                ** CAN1 has received all messages
                ** Send new messages from CAN2 to CAN1
                **-------------------------------------
                */
                CAN2ii = 0xff;
                CAN1ii = 0;

                CAN2msg++;

                TxMsg = Can2Test[(CAN2msg & 0xf)];
                TxMsg.Data1 = CAN2msg;
                TxMsg.Data1 = CAN1ii-1;
                TxMsg.Data2 = CAN1ii++;
                EW_SendData_CAN( 2, &TxMsg );

#ifdef USE_THREE_TX_BUFFERS
                CAN2msg++;
                TxMsg = Can2Test[(CAN2msg & 0xf)];
                TxMsg.Data1 = CAN2msg;
                TxMsg.Data1 = CAN1ii-1;
                TxMsg.Data2 = CAN1ii++;
                EW_SendData_CAN( 2, &TxMsg );

                CAN2msg++;
                TxMsg = Can2Test[(CAN2msg & 0xf)];
                TxMsg.Data1 = CAN2msg;
                TxMsg.Data1 = CAN1ii-1;
                TxMsg.Data2 = CAN1ii++;
                EW_SendData_CAN( 2, &TxMsg );
#endif

                /*--- LED toggle ---*/
                if(CAN2msg & 0x1)
                {
                    /*--- GREEN LED ON ---*/
                    IO0CLR_bit.P0_13 = 1;
                }
                else
                {
                    /*--- GREEN LED OFF ---*/
                    IO0SET_bit.P0_13 = 1;
                }
            }
        }
    }

} /* main */


/*
**===========================================================================
**  5.      INTERNAL FUNCTIONS (declared in Section 3.5)
**===========================================================================
*/

__irq __arm void IRQ_Handler(void)
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      IRQ service interrupt handler.
**      Reads the VICVectAddr register and jump to the value read.
**
**  Parameters:
**      None
**
**  Returns:
**      None
**
**---------------------------------------------------------------------------
*/

{
  void (*interrupt_function)();
  unsigned int vector;

  vector = VICVectAddr; /* Get interrupt vector. */
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  /* Call vectored interrupt function. */

  VICVectAddr = 0; /* Clear interrupt in VIC. */
}

/*
**===========================================================================
** END OF FILE
**===========================================================================
*/
