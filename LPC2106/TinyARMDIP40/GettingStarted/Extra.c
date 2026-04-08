//---------------------------------------------------------------------------
// Filename: Extra.c
//---------------------------------------------------------------------------
// Various routines
// V0.0 02-06-2004 Initial release
//---------------------------------------------------------------------------
// Created by Ivan Paulik for TinyARM DIP40
// Tools used: IAR EWARM v4.10B and IAR MakeApp for LPC2106 v4.00C
// Copyright PASAT 2004
//---------------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

//#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "ma_uart1.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */

#include "string.h"
#include "easypgm.h"

S8 MA_WritePoll_I2C(void);
unsigned char MA_WaitforChar_UART1( U8 *pChar );
void SPI_Reset_SPIF(void);
S8 I2C_StartRead_24C256( U8 *pTxData );
S8 I2C_PageRead_24C256( U8 *pRxData, U16 Length );

extern char Info;
extern char StatusI2C;

//---------------------------------------------------------------------------
//
//      Waits till I2C returns ACK (after BURN cycle)
//
//      Note: After this function is run, you may need a bus free time 
//            before a new data transfer can be initiated.
//  Parameters: None
//  Returns: MA_OK or MA_ERROR
//---------------------------------------------------------------------------
S8 MA_WritePoll_I2C(void) 
{
    S8 ReturnCode;
    U8 Status,BurnEnd;

    /*--- Handle user code on function entry ---*/
    ENTER_MA_MASTERWRITE_I2C;

	BurnEnd=FALSE;
	while (BurnEnd==FALSE)
	{
    	/*--- Generate Start condition ---*/
    	ReturnCode = MA_Start_I2C();

    	/*--- Transmit SLA+W ---*/
    	if( ReturnCode == MA_OK )
    	{
        	/*--- Write SLA+W ---*/
        	ReturnCode = MA_MasterPutChar_I2C( I2C_SLAVEADR_SEND );
        	while( ReturnCode == MA_BUSY )
        	{
            	ReturnCode = MA_MasterPutChar_I2C( I2C_SLAVEADR_SEND );
        	}
    	}

    	if( ReturnCode == MA_OK )
    	{
        	/*--- Wait until SLA+W transmitted ---*/
            /*--- Get new status ---*/
	        Status = MA_CheckStatus_I2C();

            if (Status == 0x18)
            {
            	/*--- Data transmitted and ACK received ---*/
				BurnEnd=TRUE;
           	}
           	else if( Status == 0x20 )
		 	{
           		/*--- Data transmitted and ACK not received ---*/
				// send start bit, start again
				BurnEnd=FALSE;
		 	}
           	else if( Status != 0xf8 )
           	{
                /*--- ERROR ---*/
				BurnEnd=TRUE;
                ReturnCode = MA_ERROR;
           	}
    	}

	    /*--- Generate Stop condition ---*/
	    MA_Stop_I2C();

    } // end of while (BurnEnd==FALSE)

    /*--- Handle user code on function exit ---*/
    EXIT_MA_MASTERWRITE_I2C;
  
    return ReturnCode;
    
} /* MA_WritePoll_I2C */

//-------------------------------------------------------------
// Wait for byte from UART1
//-------------------------------------------------------------
unsigned char MA_WaitforChar_UART1( U8 *pChar )
{
	S8 Result8;

    Result8=MA_GetChar_UART1(pChar);
	if (Result8==MA_ERROR) return 1;
	do
	{
	    Result8=MA_GetChar_UART1(pChar);
		if (Result8==MA_ERROR) return 1;
	}
	while (Result8 == MA_EMPTY);
	return 0;

}

//-------------------------------------------------------------
// Write 4 bytes (Random Read), do not issue STOP
//-------------------------------------------------------------
// Buffer:
// Byte 00: DeviceAddress R/Wn=0 (WRITE)
// Byte 01: EEPROM Address MSB   (A14..A08)
// Byte 02: EEPROM Address LSB   (A07..A00)
// Byte 03: DeviceAddress R/Wn=1 (READ)
//-------------------------------------------------------------
//  Parameters:
//      pRxData     A pointer to the receive buffer
//
//  Returns:
//      MA_ERROR    If an error occured
//      MA_OK       Otherwise
//
//-------------------------------------------------------------
S8 I2C_StartRead_24C256( U8 *pTxData ) 
{
    S8 ReturnCode;
    U8 Status, Flag, ii;

    /*--- Handle user code on function entry ---*/
    ENTER_MA_MASTERWRITE_I2C;

	// Trying to locate error
	//Info=0;
	//StatusI2C=0;

    /*--- Generate Start condition ---*/
    ReturnCode = MA_Start_I2C();

    /*--- Transmit address ---*/
    if( ReturnCode == MA_OK )
    {
        /*--- Write SLA+W ---*/
        ReturnCode = MA_MasterPutChar_I2C( *pTxData );
        while( ReturnCode == MA_BUSY )
        {
            ReturnCode = MA_MasterPutChar_I2C( *pTxData );
        }
        pTxData++;
    }

    if( ReturnCode == MA_OK )
    {
        /*--- Wait until address transmitted and transmit data ---*/
        for( ii = 1; ii<3; ii++ )
        {
            /*--- Wait until data transmitted ---*/
            Flag = TRUE;
            while( Flag )
            {
                /*--- Get new status ---*/
                Status = MA_CheckStatus_I2C();

                if( (Status == 0x18) || (Status == 0x28) )
                {
                    /*--- Data transmitted and ACK received ---*/
                    Flag = FALSE;

                    /*--- Write data ---*/
                    ReturnCode = MA_MasterPutChar_I2C( *pTxData );
                    while( ReturnCode == MA_BUSY )
                    {
                        ReturnCode = MA_MasterPutChar_I2C( *pTxData );
                    }
                    pTxData++;
                }
                else if( Status != 0xf8 )
                {
                    /*--- ERROR ---*/
                    Flag = FALSE;
                    ii = 4;
					//Info |= 1;
					//StatusI2C=Status;
                    ReturnCode = MA_ERROR;
                }
            }
        }
    }

    /*--- Wait until data transmitted ---*/
    Flag = TRUE;
    while( Flag )
    {
        /*--- Get new status ---*/
        Status = MA_CheckStatus_I2C();

        if (Status == 0x28)
        {
            /*--- Data transmitted and ACK received ---*/
            Flag = FALSE;
        }
        else if( Status != 0xf8 )
        {
            /*--- ERROR ---*/
            Flag = FALSE;
            ii = 4;
			//Info |= 2;
			//StatusI2C=Status;
            ReturnCode = MA_ERROR;
        }
    }

    /*--- Generate Start condition ---*/
    ReturnCode = MA_RepeatStart_I2C();

    /*--- Transmit device address ---*/
    if( ReturnCode == MA_OK )
    {
        /*--- Write SLA+R ---*/
        ReturnCode = MA_MasterPutChar_I2C( *pTxData );
        while( ReturnCode == MA_BUSY )
        {
            ReturnCode = MA_MasterPutChar_I2C( *pTxData );
        }
    }
//	else
//	{
//		Info |= 4;
//		StatusI2C=ReturnCode;
//	}

    /*--- Wait until SLA+R transmitted ---*/
    Flag = TRUE;
    while( Flag )
    {
        /*--- Get new status ---*/
        Status = MA_CheckStatus_I2C();

        if (Status == 0x40)
        {
            /*--- Data transmitted and ACK received ---*/
            Flag = FALSE;
        }
        else if( Status != 0xf8 )
        {
            /*--- ERROR ---*/
            Flag = FALSE;
            ii = 4;
            ReturnCode = MA_ERROR;
        }
    }

	// Do not even think of STOP bit here !!!

    /*--- Handle user code on function exit ---*/
    EXIT_MA_MASTERWRITE_I2C;
  
    return ReturnCode;
    
} // I2C_StartRead_24C256()

//-------------------------------------------------------------
// Random Read followed by sequential read
//-------------------------------------------------------------
// Receive buffer:
// Byte 00: DeviceAddress R/Wn=0 (WRITE)
// Byte 01: EEPROM Address MSB   (A14..A08)
// Byte 02: EEPROM Address LSB   (A07..A00)
// Byte 03: DeviceAddress R/Wn=1 (READ)
// Byte 04 to 67: Buffer for data read from EEPROM
//-------------------------------------------------------------
//  Parameters:
//      pRxData     A pointer to the receive buffer
//      Length      The number of data bytes to receive
//
//  Returns:
//      MA_OK or I2C status code
//
//-------------------------------------------------------------
S8 I2C_PageRead_24C256( U8 *pRxData, U16 Length ) 

{
    S8 ReturnCode;
    U8 Status, Flag, ii;

    /*--- Handle user code on function entry ---*/
    ENTER_MA_MASTERREAD_I2C;

    /*--- Write 4 bytes, see 24C256 Random Read ---*/
    ReturnCode = I2C_StartRead_24C256(pRxData);
	pRxData+=4;
	Length-=4;

	// Trying to locate error
	//Info=0;
	//StatusI2C=0;

    if( ReturnCode == MA_OK )
    {
        /*--- wait until address transmitted and receive data ---*/
        for( ii = 1; ii<= Length; ii++ )
        {
            /*--- wait until data transmitted ---*/
            Flag = TRUE;
            while( Flag )
            {
                /*--- Get new status ---*/
                Status = MA_CheckStatus_I2C();

                if(( Status == 0x40 ) || ( Status == 0x48 ) || ( Status == 0x50 ))
                {
                    /*--- Data received ---*/
                    Flag = FALSE;

                    if( ii == Length )
                    {
                        /*--- Set generate NACK ---*/
                        ReturnCode = MA_MasterGetChar_I2C( MA_I2C_ACK1, pRxData );
                    }
                    else
                    {
                        ReturnCode = MA_MasterGetChar_I2C( MA_I2C_ACK0, pRxData );
                    }

                    /*--- Read data ---*/
                    ReturnCode = MA_MasterGetChar_I2C( MA_I2C_READ, pRxData );
                    while( ReturnCode == MA_EMPTY )
                    {
               	        ReturnCode = MA_MasterGetChar_I2C( MA_I2C_READ, pRxData );
                    }
                    pRxData++;
                }
                else if( Status != 0xf8 )
                {
                    /*--- ERROR ---*/
                    Flag = FALSE;
                    ii = Length;
                    ReturnCode = MA_ERROR;
                }
            }
        }
    }
//	else
//	{
//		Info |= 4;
//		StatusI2C=ReturnCode;
//	}

    /*--- Generate Stop condition ---*/
    MA_Stop_I2C();

    /*--- Handle user code on function exit ---*/
    EXIT_MA_MASTERREAD_I2C;

    return ReturnCode;
    
} /* MA_MasterRead_I2C */
