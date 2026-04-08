
/*
*****************************************************************************
**
**      Project     : EW CAN Library for NXP LPC2129
**
**      Modulename  : CAN
**
**      Filename    : ew_can.c
**
**      Abstract    : The module contain CAN library for the NXP LPC2129
**                    CAN controller.
**                    The software contain initalization, run-time and
**                    interrupt handling functions.
**
**                    The init function contain initialization code for the
**                    CAN peripheral and both CAN controller #1 and #2 are
**                    used.
**
**                    Run-time functions are normally called by the
**                    application software to
**                     1. Load CAN filters used by the application
**                     2. Manage the CAN peripheral to view if messages has
**                        been received or overrun or errors has occured.
**                     3. Read received CAN messages
**                     4. Transmit CAN messages
**
**                    Interrupt functions are used when interrupts has been
**                    enabled.
**
**                    Application software can change the behaviour of this
**                    CAN library by updating the <user_can.h> file.
**                    <user_can.h> file include user code macros definitions.
**
**      Functions   : EW_Init_CAN()             Initialize CAN
**
**                    EW_AddFilter_CAN()        Add filter
**                    EW_Manage_CAN()           Manage CAN operation
**                    EW_ReceiveData_CAN()      Receive data
**                    EW_SendData_CAN()         Send data
**
**                    EW_IntHandler_RX_CAN1()   RX message CAN1 interrupt
**                    EW_IntHandler_RX_CAN2()   RX message CAN2 interrupt
**                    EW_IntHandler_TX_CAN1()   TX message CAN1 interrupt
**                    EW_IntHandler_TX_CAN2()   TX message CAN2 interrupt
**                    EW_IntHandler_OTHER_CAN() Other CAN interrupts
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
#include "ew_can.h"         /* Module driver header file */
#include "user_can_def.h"   /* Application specific CAN definitions */



/*
**===========================================================================
**  3.      DECLARATIONS
**  3.1     Internal constants
**===========================================================================
*/
#define CAN_REG_BASE                    (0xE0000000)
#define ACCEPTANCE_FILTER_RAM_BASE      (CAN_REG_BASE + 0x00038000)
#define ACCEPTANCE_FILTER_REGISTER_BASE (CAN_REG_BASE + 0x0003C000)
#define CENTRAL_CAN_REGISTER_BASE       (CAN_REG_BASE + 0x00040000)
#define CAN_1_BASE                      (CAN_REG_BASE + 0x00044000)
#define CAN_2_BASE                      (CAN_REG_BASE + 0x00048000)


/*--- SFR register bits ---*/
#define C1GSR_RBS  0x01        /* Bit 0: RBS (Received buffer status)  */
#define C1CMR_RRB  0x04        /* Bit 2: RRB (Release receiver buffer) */


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
**------------------------------------
Counters used only during driver test
long SEM1count;
long SEM2count;
**------------------------------------
*/

/*
**===========================================================================
**  3.5     Internal function prototypes (defined in Section 5)
**===========================================================================
*/

/*
**===========================================================================
**  3.6     Internal variables
**===========================================================================
*/
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;

/*
**===========================================================================
**  4.      GLOBAL FUNCTIONS (declared as 'extern' in some header file)
**===========================================================================
*/

void EW_Init_CAN( void )
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**    Initializes the CAN module.
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
    /*--- Handle user code on function entry ---*/
    ENTER_EW_INIT_CAN;

    /*--- Enable Pins for CAN port 1 and 2 ---*/
    PINSEL1 |= EW_CAN_PINSEL1;

    /*--- Enable CAN modules clock ---*/
    PCONP |= (1<<13) | (1<<14);

    /*--- All Rx messages on all CAN buses are ignored ---*/
    AFMR = AcceptanceFilterOff;

    /*--- Enter reset mode ---*/
    C1MOD |= 0x01;
    C2MOD |= 0x01;

    /*
    **-----------------------------------
    ** Select the CAN interrupt handling
    **-----------------------------------
    */
    C1IER = EW_C1IER_CAN;
    C2IER = EW_C2IER_CAN;

    /*
    **-----------------------
    ** Clear status register
    **-----------------------
    */
    C1GSR = 0;
    C2GSR = 0;

    /*
    **-----------------------------------------------------
    ** Select the CAN bitrate, sample method, TSEG and SJW
    **-----------------------------------------------------
    */
    C1BTR = EW_C1BTR_CAN;
    C2BTR = EW_C2BTR_CAN;

    /*
    **--------------------------------------------------------------
    ** Acceptance filter and central CAN register updates. Note the
    ** filter table is updated with the function EW_AddFilter_CAN()
    **--------------------------------------------------------------
    */
    SFF_sa     = 0;
    SFF_GRP_sa = 0;
    EFF_sa     = 0;
    EFF_GRP_sa = 0;
    ENDofTable = 0;

    /*--- Acceptance filter mode ----*/
    AFMR = EW_AFMR_CAN;

    /*
    **---------------------------------------------------------------------------
    ** Select the CANMOD used by the CAN driver
    **---------------------------------------------------------------------------
    */
    /*--- Set mode register but do not release reset ---*/
    C1MOD = ( EW_C1MOD_CAN | 0x01 );
    C2MOD = ( EW_C2MOD_CAN | 0x01 );

    /*--- Release CAN reset ---*/
    C1MOD = ( C1MOD & ~0x01 );
    C2MOD = ( C2MOD & ~0x01 );

    /*--- Handle user code on function exit ---*/
    EXIT_EW_INIT_CAN;

} /* EW_Init_CAN */



S16 EW_AddFilter_CAN( CAN_Filter *pNewFilter )
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      The new filter is inserted into the look-up filter table RAM.
**
**      The CAN_Filter must contain the following information depending on
**      the added filter type.
**      CAN_FULL:   Two standard IDs with controller info.
**      STD:        Two standard IDs with controller info.
**      STD_GRP:    Lower and Upper standard ID bound with controller info.
**      EXT:        Extended ID with controller info.
**      EXT_GRP:    Lower and Upper extended id bound with controller info.
**
**      Please note; When CAN_FULL and STD messages shall be inserted
**                   two IDs will be inserted per call. The driver
**                   requires that these IDs can be inserted into table
**                   at the same word while keeping a sorted list.
**
**  Parameters:
**      pNewFilter  Pointer to the new Filter information
**
**  Returns:
**      EW_OK       if filter successfully added
**      EW_FULL     if filter table is full
**      EW_ERROR    if error
**
**---------------------------------------------------------------------------
*/
{
    U32 ii, Size;
    U32 FilterID;
    U32 Address;
    U32 *pFilter;
    U32 id0, id1, id3, id4;

    /*--- Handle user code on function entry ---*/
    ENTER_EW_ADDFILTER_CAN;

    /*--- Can we add new item? ---*/
    Size = ENDofTable + 4;
    Size += SFF_sa * 6;
    if( Size > 0x7ff )
    {
        /*--- Acceptance filter table full ---*/
        return EW_FULL;
    }

    /*--- Prepare filter values ---*/
    switch ( pNewFilter->FilterType )
    {
    case EW_CAN_FULL:
    case EW_CAN_STD:
    case EW_CAN_STD_GRP:
        /*--- Prepare new pair of IDs ---*/
        FilterID = pNewFilter->ID1;                    /* ID lower */
        FilterID |= ( pNewFilter->Disable1 << 12);     /* Disable bit */
        FilterID |= ( pNewFilter->Controller1 << 13);  /* Controller */
        FilterID <<= 16;                               /* shift to lower column */

        FilterID |= pNewFilter->ID2;                   /* ID upper */
        FilterID |= ( pNewFilter->Disable1 << 12);     /* Disable bit */
        FilterID |= ( pNewFilter->Controller2 << 13);  /* Controller */
        break;

    case EW_CAN_EXT:
    case EW_CAN_EXT_GRP:
        /*--- Prepare new extend ID ---*/
        FilterID = pNewFilter->ID1;                    /* ID */
        FilterID |= ( pNewFilter->Disable1 << 28);     /* Disable bit */
        FilterID |= ( pNewFilter->Controller1 << 29);  /* Controller */
        break;

    default:
        return EW_ERROR;
    }

    /*--- All Rx messages on all CAN buses are ignored ---*/
    AFMR = AcceptanceFilterOff;

    /*--- Enter reset mode ---*/
    C1MOD |= 0x01;
    C2MOD |= 0x01;

    /*--- Set pointer to first location of filter ---*/
    pFilter = (U32 *) ACCEPTANCE_FILTER_RAM_BASE;

    /*--- What kind of filter will be handled? ---*/
    switch ( pNewFilter->FilterType )
    {
    case EW_CAN_FULL:
        /*--- Find place to add new fullCAN filter ---*/
        ii = 0;

        /*--- Loop through all existing fullCAN filters ---*/
        while ( ii < SFF_sa )
        {
            /*--- New filter < Stored filter? ---*/
            if ( ( FilterID & 0xc7ffc7ff ) < ( *pFilter & 0xc7ffc7ff ) )
            {
                /*--- yes break ---*/
                break;
            }
            ii += 4;
            pFilter++;
        }

        /*--- Save current table ID ---*/
        id0 = *pFilter;

        /*--- Update start address registers ---*/
        SFF_sa += 4;
        SFF_GRP_sa += 4;
        EFF_sa += 4;
        EFF_GRP_sa += 4;
        break;

    case EW_CAN_STD:
        /*--- Find place to add new standard IDs filter ---*/
        ii = SFF_sa;
        pFilter += (ii / 4);

        /*--- Loop through all existing standard filters ---*/
        while ( ii < SFF_GRP_sa )
        {
            /*--- New filter < Stored filter? ---*/
            if ( ( FilterID & 0xc7ffc7ff ) < ( *pFilter & 0xc7ffc7ff ) )
            {
                /*--- yes break ---*/
                break;
            }
            ii += 4;
            pFilter++;
        }

        /*--- Save current table ID ---*/
        id0 = *pFilter;

        /*--- Add new Filter ID ---*/
        *pFilter = FilterID;

        /*--- Update start address registers ---*/
        SFF_GRP_sa += 4;
        EFF_sa += 4;
        EFF_GRP_sa += 4;
        break;

    case EW_CAN_STD_GRP:
        /*--- Find place to add new standard IDs group filter ---*/
        ii = SFF_GRP_sa;
        pFilter += (ii / 4);

        /*--- Loop through all existing standard filters ---*/
        while ( ii < EFF_sa )
        {
            /*--- New filter < Stored filter? ---*/
            if ( ( FilterID & 0xc7ffc7ff ) < ( *pFilter & 0xc7ffc7ff ) )
            {
                /*--- yes break ---*/
                break;
            }
            ii += 4;
            pFilter++;
        }

        /*--- Save current table ID ---*/
        id0 = *pFilter;

        /*--- Add new Filter ID ---*/
        *pFilter = FilterID;

        /*--- Update start address registers ---*/
        EFF_sa += 4;
        EFF_GRP_sa += 4;
        break;

    case EW_CAN_EXT:
        /*--- Find place to add new extended ID filter ---*/
        ii = EFF_sa;
        pFilter += (ii / 4);

        /*--- Loop through all existing standard filters ---*/
        while ( ii < EFF_GRP_sa )
        {
            /*--- New filter < Stored filter? ---*/
            if ( ( FilterID & 0xc7ffffff ) < ( *pFilter & 0xc7ffffff ) )
            {
                /*--- yes break ---*/
                break;
            }
            ii += 4;
            pFilter++;
        }

        /*--- Save current table ID ---*/
        id0 = *pFilter;

        /*--- Add new Filter ID ---*/
        *pFilter = FilterID;

        // Update start address registers
        EFF_GRP_sa += 4;
        break;

    case EW_CAN_EXT_GRP:
        /*--- Find place to add new extended IDs group filter ---*/
        ii = EFF_GRP_sa;
        pFilter += (ii / 4);

        /*--- Loop through all existing standard filters ---*/
        while ( ii < ENDofTable )
        {
            /*--- New filter < Stored filter? ---*/
            if ( ( FilterID & 0xc7ffffff ) < ( *pFilter & 0xc7ffffff ) )
            {
                /*--- yes break ---*/
                break;
            }
            ii += 8;
            pFilter += 2;
        }

        /*--- Save current table ID ---*/
        id0 = *pFilter;

        /*--- Add new Filter ID lower bound ---*/
        *pFilter = FilterID;

        /*--- Prepare new extend ID, higher bound ---*/
        FilterID = pNewFilter->ID2;                    /* ID */
        FilterID |= ( pNewFilter->Controller2 << 29);  /* Controller */
        pFilter++;

        /*--- Save current table ID ---*/
        id1 = *pFilter;

        /*--- Add new Filter ID higher bound ---*/
        *pFilter = FilterID;

        /*--- Move all existing filter bounds two steps forward ---*/
        pFilter++;
        while ( ii < ENDofTable )
        {
            /*--- lower bound ---*/
            id3 = *pFilter;  /* get filter */
            *pFilter = id0;  /* restore filter */
            id0 = id3;       /* move filter */
            pFilter++;

            /*--- higher bound ---*/
            id4 = *pFilter;  /* get filter */
            *pFilter = id1;  /* restore filter */
            id1 = id4;       /* move filter */
            pFilter++;
            ii += 8;
        }

        /*--- Update end of table register ---*/
        ENDofTable += 8;
        break;

    }

    if ( pNewFilter->FilterType != EW_CAN_EXT_GRP )
    {
      /*--- Add new Filter ID ---*/
      *pFilter = FilterID;

      /*--- Move all existing filters one step forward ---*/
      pFilter++;
      while ( ii < ENDofTable )
      {
          id1 = *pFilter;  /* get filter */
          *pFilter = id0;  /* restore filter */
          id0 = id1;       /* move filter */
          pFilter++;
          ii += 4;
      }

        /*--- Update end of table register ---*/
      ENDofTable += 4;

    }

    /*--- Acceptance filter mode ----*/
    AFMR = EW_AFMR_CAN;

    /*--- Release reset! ---*/
    C1MOD  = ( C1MOD & ~0x01 );
    C2MOD  = ( C2MOD & ~0x01 );

    /*--- List has been full? ---*/
    Address = ENDofTable;
    Address += ( SFF_sa * 6 );
    if( Address > 0x800 )
    {
        /*--- Acceptance filter table full after new entries ---*/
        return EW_FULL;
    }

    /*--- Handle user code on function exit ---*/
    EXIT_EW_ADDFILTER_CAN;

    return EW_OK;

} /* EW_AddFilter_CAN */


S16 EW_Manage_CAN( U8 Controller )
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      Manage status information from the selected module.
**
**      This function can be polled by the application to get status
**      information from the CAN controller.
**      If interrupts not are used it shall be called (polled) as
**      often as possible.
**
**      The function scans the global controller status register.
**
**  Parameters:
**      Controller     The Controller number
**
**  Returns:
**      EW_CAN_RECEIVED        A message is available
**      EW_CAN_OVERRUN         Data overrun
**      EW_CAN_TRANSMITTED     Requested transmissions completed
**      EW_CAN_ERROR           Error status
**      EW_CAN_BUS_ERROR       Bus error status
**      EW_CAN_FILTER_ERROR    Acceptance Filter error
**
**---------------------------------------------------------------------------
*/
{
    S16 RetCode;
    U32 status;

    /*--- Handle user code on function entry ---*/
    ENTER_EW_MANAGE_CAN;

    RetCode = 0;

    /*--- New message received? ---*/
    if( Controller == 1)
    {
        status = C1GSR;
    }
    else if( Controller == 2)
    {
        status = C2GSR;
    }

    if( status & 0x01 )
    {
        /*--- A message is available ---*/
        RetCode |= EW_CAN_RECEIVED;
    }

    /*--- Data overrun? ---*/
    if( status & 0x02 )
    {
        /*--- A data overrun has occurred ---*/
        RetCode |= EW_CAN_OVERRUN;
    }

    /*--- Transmit complete status? ---*/
    if( status & 0x08 )
    {
        /*--- All requested transmissions has been successfully completed ---*/
        RetCode |= EW_CAN_TRANSMITTED;
    }

    /*--- Error status? ---*/
    if( status & 0x40 )
    {
        /*
        **------------------------------------------------------
        ** Error status
        ** RXERR or TXERR bitfields contain error counter value
        **------------------------------------------------------
        */
        RetCode |= EW_CAN_ERROR;
    }

    /*--- Bus error status? ---*/
    if( status & 0x80 )
    {
        /*--- Bus error status ---*/
        RetCode |= EW_CAN_BUS_ERROR;
    }

    /*--- Acceptance Filter error? ---*/
    if( LUTerr > 0 )
    {
        /*--- YES some problem detected ---*/
        RetCode |= EW_CAN_FILTER_ERROR;
    }

    /*--- Handle user code on function exit ---*/
    EXIT_EW_MANAGE_CAN;

    return RetCode;

} /* EW_Manage_CAN */


S16 EW_ReceiveData_CAN( U8 Controller, CAN_MSG *pRxMsg )
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      Reads data from full CAN message or the selected CAN controller.
**      The function EW_Manage_CAN can be called to see if a message are
**      available and if errors has occured.
**
**  Parameters:
**      Controller  Full CAN message or the CAN controller number
**                  0   reading Full CAN message
**                  1-2 reading CAN controller #1 or #2
**
**      pRxMsg      Pointer to the CAN_MSG struct were received information
**                  shall be stored.
**
**  Returns:
**      EW_DATA     if it was a data frame
**      EW_RTR      if it was a remote request frame
**      EW_EMPTY    if the receive buffer is empty
**      EW_BUSY     if message is receiving
**      EW_ERROR    if data overrun mail flag is set during read
**
**---------------------------------------------------------------------------
*/
{
    S16 ErrorCode;
    U32 ii, jj;
    U32 FilterID;
    U32 volatile *pFilter;

    /*--- Handle user code on function entry ---*/
    ENTER_EW_RECEIVEDATA_CAN;

    switch( Controller )
    {
    case 0:
        /*
        **------------------
        ** Full CAN message
        **------------------
        */
        /*--- First verify that CAN controller not is busy receiving a message ---*/
        /* if(( CANRxSR & 0xf ) > 0 ) test with 0xf if Four CAN controllers are available in the microcontroller */
        if(( CANRxSR & 0x3 ) > 0 )
        {
            /*--- Receiving CAN message ---*/
            ErrorCode = EW_BUSY;
        }

        else
        {
            /*--- Error code initialized ---*/
            ErrorCode = EW_ERROR;

            /*--- Test if new Full CAN message received ---*/

            /*--- Set pointer to first location of filter ---*/
            pFilter = (U32 *) ( ACCEPTANCE_FILTER_RAM_BASE+ ENDofTable );

            /*--- Number of items in table ---*/
            jj = (SFF_sa >> 1);

            for( ii = 0; ii < jj; ii++ )
            {
                if(( *pFilter & 0x3000000L ) == 0x1000000L )
                {
                    /*--- SEM == 1 ---*/
                    /*
                    **-------------------------------------------------
                    ** Increment variable used only during driver test
                    ** SEM1count++;
                    **-------------------------------------------------
                    */
                }

                if(( *pFilter & 0x3000000L ) == 0x3000000L )
                {
                    /*--- SEM == 3, Message updated, read information ---*/

                    /*--- Read FF, RTR, DLC info and ID ---*/
                    pRxMsg->CTRL = (*pFilter & 0xC00F0000L);
                    pRxMsg->ID = (*pFilter & 0x000007FFL);

                    /*--- Clear SEM ---*/
                    *pFilter &= ~0x3000000L;
                    pFilter++;

                    /*--- Read Data 2 ---*/
                    pRxMsg->Data1= *pFilter;
                    pFilter++;

                    /*--- Read Data 3 ---*/
                    pRxMsg->Data2= *pFilter;

                    /*--- Message still OK? ---*/
                    pFilter -= 2;
                    if(( *pFilter & 0x3000000L ) == 0)
                    {
                        /*--- Yes OK ---*/

                        /*--- Get CAN controller # info - contained in filter table ---*/
                        pFilter = (U32 *) ACCEPTANCE_FILTER_RAM_BASE;
                        pFilter += (ii>>1);
                        FilterID = *pFilter;
                        if( ii&1 )
                        {
                            /*--- Get info from position 13-15 ---*/
                            FilterID &= 0xE000;
                            /*--- Store info in bit position 24-26 ---*/
                            FilterID <<= 11;
                        }
                        else
                        {
                            /*--- Get info from position 29-31 ---*/
                            FilterID &= 0xE0000000L;
                            /*--- Store info in bit position 24-26 ---*/
                            FilterID >>= 5;
                        }

                        /*--- Add CAN controller # info to bit position 24-26 ---*/
                        pRxMsg->CTRL |= FilterID;

                        if(( pRxMsg->CTRL & EW_CAN_CTRL_RTR ) != 0)
                        {
                            /*--- RTR message received ---*/
                            ErrorCode = EW_RTR;
                        }
                        else
                        {
                            /*--- Data message received ---*/
                            ErrorCode = EW_DATA;
                        }
                        break;
                    }
                    else
                    {
                        /*--- Message has been updated during read ---*/
                        /*
                        **-------------------------------------------------
                        ** Increment variable used only during driver test
                        ** SEM2count++;
                        **-------------------------------------------------
                        */
                        return EW_EMPTY;
                    }
                }
                else
                {
                    /*--- Next message ---*/
                    pFilter += 3;
                }
            }

            // Neee this test ????
            /*--- Any new message found? ---*/
            if( ii == jj )
            {
                /*--- Receive buffer is empty ---*/
                ErrorCode = EW_EMPTY;
            }
        }
        break;

    case 1:
        /*
        **--------------------------
        ** CAN controller 1 message
        **--------------------------
        */
        if( C1GSR & C1GSR_RBS )
        {
            /*--- A received message is available ---*/

            /*--- Read FF, RTR, DLC info ---*/
            pRxMsg->CTRL = (C1RFS & 0xC00F0000L);

            /*--- Read ID ---*/
            pRxMsg->ID = C1RID;

            /*--- Read Data ---*/
            pRxMsg->Data1= C1RDA;
            pRxMsg->Data2= C1RDB;

            /*--- Bit 2: RRB (Release receiver buffer) ---*/
            C1CMR |= C1CMR_RRB;

            if(( pRxMsg->CTRL & EW_CAN_CTRL_RTR ) != 0)
            {
                /*--- RTR message received ---*/
                ErrorCode = EW_RTR;
            }
            else
            {
                /*--- Data message received ---*/
                ErrorCode = EW_DATA;
            }
            /*--- Data overrun? ---*/
            if( C1GSR & 0x02 )
            {
                /*--- A data overrun has occurred ---*/
                ErrorCode = EW_ERROR;
            }
        }
        else
        {
            /*--- Receive buffer is empty ---*/
            ErrorCode = EW_EMPTY;
        }
        break;

    case 2:
        /*
        **--------------------------
        ** CAN controller 2 message
        **--------------------------
        */
        if( C2GSR & C1GSR_RBS )
        {
            /*--- A received message is available ---*/
            pRxMsg->CTRL = (C2RFS & 0xC00F0000L);

            /*--- Read ID ---*/
            pRxMsg->ID = C2RID;

            /*--- Read Data ---*/
            pRxMsg->Data1= C2RDA;
            pRxMsg->Data2= C2RDB;

            /*--- Bit 2: RRB (Release receiver buffer) ---*/
            C2CMR |= C1CMR_RRB;

            if(( pRxMsg->CTRL & EW_CAN_CTRL_RTR ) != 0)
            {
                /*--- RTR message received ---*/
                ErrorCode = EW_RTR;
            }
            else
            {
                /*--- Data message received ---*/
                ErrorCode = EW_DATA;
            }
            /*--- Data overrun? ---*/
            if( C1GSR & 0x02 )
            {
                /*--- A data overrun has occurred ---*/
                ErrorCode = EW_ERROR;
            }
        }
        else
        {
            /*--- Receive buffer is empty ---*/
            ErrorCode = EW_EMPTY;
        }
        break;
    default:
        /*--- wrong controller parameter ---*/
        ErrorCode = EW_ERROR;
        break;
    }

    /*--- Handle user code on function exit ---*/
    EXIT_EW_RECEIVEDATA_CAN;

    return ErrorCode;

} /* EW_ReceiveData_CAN */


S16 EW_SendData_CAN( U8 Controller, CAN_MSG *pTxMsg )
/*
**---------------------------------------------------------------------------
**
**  Abstract:
**      Writes data to the selected CAN controller.
**
**      The CAN controller contains three transmission buffers for each CAN
**      controller. The function trys to find an empty Tx buffer to start
**      transmission from any empty buffer. Please note that NXP has
**      described a problem with the the transmit buffers and recommend that
**      only one of the buffers shall be used.
**
**      See NXP ERRATA SHEET´, August 5, 2005 - Version 1.8   LPC2129
**
**  Parameters:
**      Controller  The Controller number (1-2)
**
**      pTxMsg      Pointer to the CAN_MSG struct were data to be
**                  transmitted is located.
**
**  Returns:
**      EW_OK       on success
**      EW_BUSY     if no tx buffer available for transmission
**      EW_ERROR    if wrong module number
**
**---------------------------------------------------------------------------
*/
{
    S16  ErrorCode;

    U32 *pAddr;
    U32 offset;
    U32 tx_buf;

    /*--- Handle user code on function entry ---*/
    ENTER_EW_SENDDATA_CAN;

    ErrorCode = EW_OK;

    /*--- Valid controller number? ---*/
    if( Controller > CAN_CONTROLLERS )
    {
        return EW_ERROR;
    }

    /*--- Read status to find available Tx buffer ---*/
    pAddr = (U32 *) &C1SR;
    offset = (Controller - 1) * 0x1000L;
    pAddr += offset;

    if( *pAddr & 0x4L )
    {
        /*--- Buffer 0 ---*/
        tx_buf = 0;
    }

#ifdef USE_THREE_TX_BUFFERS
    /*
    **--------------------------------------------
    ** It is not recommended according to NXP
    ** to use more than 1 Tx buffer
    **--------------------------------------------
    */
    else if( *pAddr & 0x400L )
    {
        /*--- Buffer 1 ---*/
        tx_buf = 4;
    }
    else if( *pAddr & 0x40000L )
    {
        /*--- Buffer 2 ---*/
        tx_buf = 8;
    }
#endif

    else
    {
        return EW_BUSY;
    }

    /*
    **----------------------------
    ** Write message to Tx buffer
    **----------------------------
    */
    /*--- Calculate CAN controller SFR address ---*/
    pAddr = (U32 *) &C1TFI1;
    pAddr += offset;
    pAddr += tx_buf;

    /*--- Write transmit frame - Priority, DLC bits, RTR, Frame format ---*/
    *pAddr = pTxMsg->CTRL;

    /*--- Write ID ---*/
    pAddr++;
    *pAddr = pTxMsg->ID;

    /*--- Write data ---*/
    pAddr++;
    *pAddr = pTxMsg->Data1;
    pAddr++;
    *pAddr = pTxMsg->Data2;

    /*--- Transmission Request Buffer x ---*/
    if( tx_buf == 0 )
    {
        tx_buf = 2;
    }

    /*--- Set the STPx bit (Tx buffer transmission bit for Buffer x) ---*/
    pAddr = (U32 *) &C1CMR;
    pAddr += offset;
    *pAddr = (tx_buf << 4) + 1;

    /*--- Handle user code on function exit ---*/
    EXIT_EW_SENDDATA_CAN;

    return ErrorCode;

} /* EW_SendData_CAN */


void EW_IntHandler_RX_CAN1( void )
 /*
**---------------------------------------------------------------------------
**
**  Abstract:
**      High level interrupt handler for RX CAN1 interrupt.
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
    /*--- Handle user code on function entry ---*/
    ENTER_EW_INTHANDLER_RX_CAN1;

    /*--- Handle user code ---*/
    INSIDE_EW_INTHANDLER_RX_CAN1;

    /*--- Handle user code on function exit ---*/
    EXIT_EW_INTHANDLER_RX_CAN1;

}  /* EW_IntHandler_RX_CAN1 */


void EW_IntHandler_RX_CAN2( void )
 /*
**---------------------------------------------------------------------------
**
**  Abstract:
**      High level interrupt handler for RX CAN2 interrupt.
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
    /*--- Handle user code on function entry ---*/
    ENTER_EW_INTHANDLER_RX_CAN2;

    /*--- Handle user code ---*/
    INSIDE_EW_INTHANDLER_RX_CAN2;

    /*--- Handle user code on function exit ---*/
    EXIT_EW_INTHANDLER_RX_CAN2;

}  /* EW_IntHandler_RX_CAN2 */


void EW_IntHandler_TX_CAN1( void )
 /*
**---------------------------------------------------------------------------
**
**  Abstract:
**      High level interrupt handler for TX CAN1 interrupt.
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
    /*--- Handle user code on function entry ---*/
    ENTER_EW_INTHANDLER_TX_CAN1;

    /*--- Handle user code ---*/
    INSIDE_EW_INTHANDLER_TX_CAN1;

    /*--- Handle user code on function exit ---*/
    EXIT_EW_INTHANDLER_TX_CAN1;

}  /* EW_IntHandler_TX_CAN1 */


void EW_IntHandler_TX_CAN2( void )
 /*
**---------------------------------------------------------------------------
**
**  Abstract:
**      High level interrupt handler for TX CAN2 interrupt.
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
    /*--- Handle user code on function entry ---*/
    ENTER_EW_INTHANDLER_TX_CAN2;

    /*--- Handle user code ---*/
    INSIDE_EW_INTHANDLER_TX_CAN2;

    /*--- Handle user code on function exit ---*/
    EXIT_EW_INTHANDLER_TX_CAN2;

}  /* EW_IntHandler_TX_CAN2 */


void EW_IntHandler_OTHER_CAN( void )
 /*
**---------------------------------------------------------------------------
**
**  Abstract:
**      High level interrupt handler for OTHER CAN interrupt.
**      The interrupt is generated by:
**      - Error warning interrupt
**      - Data overrun interrupt
**      - Wake-up interrupt
**      - Error passive interrupt
**      - Arbitration lost interrupt
**      - Bus error interrupt
**      - ID ready interrupt
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
    U32 *pAddr;
    U32 status;
    U32 controller;

    /*--- Handle user code on function entry ---*/
    ENTER_EW_INTHANDLER_OTHER_CAN;

    /*--- Initialize pointer for interrupt controller register ---*/
    pAddr = (U32 *) &C1ICR;

    /*--- Find interrupt sources for all CAN modules ---*/
    for( controller = 1; controller < (CAN_CONTROLLERS+1); controller++ )
    {
        /*--- Read status to find avalable Tx buffer ---*/
        status = *pAddr;

        /*--- Error warning interrupt? ---*/
        if( status & CAN_ICR_EI )
        {
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_WARNING_CAN;
        }

        /*--- Data overrun interrupt? ---*/
        if( status & CAN_ICR_DOI )
        {
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_OVERRUN_CAN;
        }

        /*--- Wake-up interrupt? ---*/
        if( status & CAN_ICR_WUI )
        {
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_WAKEUP_CAN;
        }

        /*--- Error passive interrupt? ---*/
        if( status & CAN_ICR_EPI )
        {
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_PASSIVE_CAN;
        }

        /*--- Arbitration lost interrupt? ---*/
        if( status & CAN_ICR_ALI )
        {
            /*
            **----------------------------------------
            ** CAN_ICR_ALCBIT contain the bit# info
            **----------------------------------------
            */
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_ARBITRATION_CAN;

            /*--- Add error handling routine ---*/
            /* ALCInfo = ( status & CAN_ICR_ALCBIT ); */
            ;
        }

        /*--- Bus error interrupt? ---*/
        if( status & CAN_ICR_BEI )
        {
            /*
            **-----------------------------------------------------
            ** CAN_ICR_ERRBIT contain a 5-bit buserror info code
            **
            ** CAN_ICR_ERRDIR =1 if receiving, =0 if transmitting
            **
            ** CAN_ICR_ERRC =00 bit error,   =01 Form error
            **              =10 Stuff error  =11 other error
            **-----------------------------------------------------
            */
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_BUSERROR_CAN;

            /*--- Add error handling routine ---*/
            /* ErrorInfo = ( status & CAN_ICR_ERRBIT ); */
            /* ErrorInfo = ( status & CAN_ICR_ERRDIR ); */
            /* ErrorInfo = ( status & CAN_ICR_ERRC ); */
            ;

        }

        /*--- ID ready interrupt? ---*/
        if( status & CAN_ICR_IDI )
        {
            /*--- Handle user code ---*/
            INSIDE_EW_INTHANDLER_OTHER_IDREADY_CAN;
        }

        /*--- Update ptr to next CAN interrupt controller register ---*/
        pAddr += 0x1000L;

    }

    /*--- Handle user code on function exit ---*/
    EXIT_EW_INTHANDLER_OTHER_CAN;

}  /* EW_IntHandler_OTHER_CAN */


/*
**===========================================================================
** END OF FILE
**===========================================================================
*/

