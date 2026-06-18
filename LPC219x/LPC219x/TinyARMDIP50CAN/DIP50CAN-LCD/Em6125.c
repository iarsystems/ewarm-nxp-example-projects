// -------------------------------------------------------------------
// -------------------------EM6125 LCD driver-------------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

#include "ma_i2c.h"     /* Module driver header file */

#include "em_f6x8.h"
#include "em6125.h"

#include "string.h"
#include "stdio.h"

#define LCD_SLAVEADR_SEND 0x00

//----------------------------------------------------------
// I2C routines first
//----------------------------------------------------------

//----------------------------------------------------------
// EM_Fill_WithByte
// Sends LCD slave address,control byte and fills area of
// screen with specified byte (usually zero).
//
// Note: After this function is run, you may need a bus free time 
//       before a new data transfer can be initiated.
//
//  Parameters:
//      SlaveAdr    Slave Address+R/W  (usually 00H)
//      CtrlByte    ControlByte    (usually 40H)
//      ByteVal     Data Byte to transmit to LCD 
//      Length      The number of same data bytes to transmit
//
//  Returns:
//      MA_ERROR    If an error occured
//      MA_OK       Otherwise
//
//----------------------------------------------------------
S8 EM_Fill_WithByte(U8 SlaveAdr, U8 CtrlByte, U8 ByteVal, U16 Length ) 
{
    S8 ReturnCode;
    U8 Status, Flag, ii;

    /*--- Handle user code on function entry ---*/
    ENTER_MA_MASTERWRITE_I2C;

//	Info=0;
//	StatusI2C=0;

    /*--- Generate Start condition ---*/
    ReturnCode = MA_Start_I2C();

    /*--- Transmit address ---*/
    if( ReturnCode == MA_OK )
    {
        /*--- Write SLA+W ---*/
        ReturnCode = MA_MasterPutChar_I2C( SlaveAdr );
        while( ReturnCode == MA_BUSY )
        {
            ReturnCode = MA_MasterPutChar_I2C( SlaveAdr );
        }
    }

    if( ReturnCode == MA_OK )
    {
        /*--- Wait until address transmitted and transmit data ---*/
        // This was changed for( ii = 1; ii<Length; ii++ )
        for( ii = 0; ii<Length; ii++ )
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
                    ReturnCode = MA_MasterPutChar_I2C( ByteVal );
                    while( ReturnCode == MA_BUSY )
                    {
                        ReturnCode = MA_MasterPutChar_I2C( ByteVal );
                    }
                }
                else if( Status != 0xf8 )
                {
                    /*--- ERROR ---*/
                    Flag = FALSE;
                    ii = Length;
                    ReturnCode = MA_ERROR;
//					Info|=1;
//					StatusI2C=Status;
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

        if( (Status == 0x18) || (Status == 0x28) )
        {
            /*--- Data transmitted and ACK received ---*/
            Flag = FALSE;
        }
        else if( Status != 0xf8 )
        {
            /*--- ERROR ---*/
            Flag = FALSE;
            ii = Length;
//			Info|=2;
            ReturnCode = MA_ERROR;
        }
    }

    /*--- Generate Stop condition ---*/
    MA_Stop_I2C();

    /*--- Handle user code on function exit ---*/
    EXIT_MA_MASTERWRITE_I2C;
  
    return ReturnCode;
    
} /* EM_Fill_WithBytee */

//----------------------------------------------------------
// Initialise EM6125 LCD panel
//----------------------------------------------------------
short EM_Initialise_LCD(void)
{
	S8 Result;
	unsigned char LocalBuffer[16];
	short i;

	LocalBuffer[0] = LCD_SLAVEADR_SEND;
	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=000
	LocalBuffer[1] = 0x80;
	// Data Byte - Initialisation0: 
	// - Mux=1, TC1:0=00, INvRow=0, MX=0, Blank=0, Check=0, InvVideo=0
	LocalBuffer[2] = 0x80;

	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=001
	LocalBuffer[3] = 0x81;
	// Data Byte - Initialisation1: 
	// - X6:X0=0000000 , V=0 
	LocalBuffer[4] = 0x00;


	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=010
	LocalBuffer[5] = 0x82;
	// Data Byte - Initialisation2: 
	// - Y3:Y0=0000, bit3=0, VlcdDisch=0, DEC=0, LSB=0 
	LocalBuffer[6] = 0x00;

	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=011
	LocalBuffer[7] = 0x83;
	// Data Byte - Initialisation3: 
	// - VlcdLevel7:0 = (Vlcd_9V - 3.02V)/0.0352 = 170
	LocalBuffer[8] = 0xAA;

	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=100
	LocalBuffer[9] = 0x84;
	// Data Byte - Initialisation4: 
	// - Mult1:0=01 (3*), PartDisp=0, FirstRowPD3:0=0000, Sleep=0
	LocalBuffer[10] = 0x40;

	Result = MA_MasterWrite_I2C(LocalBuffer,11);
    if (Result!=MA_OK) return Result;

	// Is some dead time needed ? Some I2C status testing ?
       
    //Result = EM_Fill_WithByte(LCD_SLAVEADR_SEND,0x40,0,12*102 ); 
    //if (Result!=MA_OK) return Result;
	for (i=0; i<17*11;i++)
		EM_Display_Char(' ');

	LocalBuffer[0] = LCD_SLAVEADR_SEND;
	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=001
	LocalBuffer[1] = 0x81;
	// Data Byte - Initialisation1: 
	// - X6:X0=0000000 , V=0 
	LocalBuffer[2] = 0x00;


	// Control Byte: C0=1, DC=0, Test2:0=000, Ini2:0=010
	LocalBuffer[3] = 0x82;
	// Data Byte - Initialisation2: 
	// - Y3:Y0=0001, bit3=0, VlcdDisch=0, DEC=0, LSB=0 
	LocalBuffer[4] = 0x10;

	Result = MA_MasterWrite_I2C(LocalBuffer,5);
    if (Result!=MA_OK) return Result;

	return Result;
}


unsigned char * EM_Get_CharacterAdr(U8 Val8)
{
	Val8 &= 0x7F;
	return (unsigned char*)(&Font6x8[Val8]);
}

S8  EM_Display_Char(unsigned char Val8)
{
	U8	LocalBuffer[16],*Ptr;
	S8 Result;

	LocalBuffer[0] = LCD_SLAVEADR_SEND;
	LocalBuffer[1] = 0x40;					//C0=0, DC=1, rest=0
	Ptr = EM_Get_CharacterAdr(Val8);
	memcpy(&LocalBuffer[2],Ptr,6);

	Result = MA_MasterWrite_I2C(LocalBuffer,8);
    //if (Result!=MA_OK) return Result;
	return Result;
}

S8  EM_Display_InvChar(unsigned char Val8)
{
	U8	LocalBuffer[16],*Ptr;
	S8 Result;
	short i;

	LocalBuffer[0] = LCD_SLAVEADR_SEND;
	LocalBuffer[1] = 0x40;					//C0=0, DC=1, rest=0
	Ptr = EM_Get_CharacterAdr(Val8);
	//memcpy(&LocalBuffer[2],Ptr,6);
	for (i=0;i<6;i++)
		LocalBuffer[2+i]=~(*Ptr++);

	Result = MA_MasterWrite_I2C(LocalBuffer,8);
    //if (Result!=MA_OK) return Result;
	return Result;
}

S8  EM_Send_Cmd(unsigned char Adr8,unsigned char Val8)
{
	U8	LocalBuffer[16];
	S8 Result;

	Adr8 &= 0x7;
	LocalBuffer[0] = LCD_SLAVEADR_SEND;
	LocalBuffer[1] = 0x80+Adr8;				//C0=1, DC=0, rest=0
	LocalBuffer[2] = Val8;

	Result = MA_MasterWrite_I2C(LocalBuffer,3);
    //if (Result!=MA_OK) return Result;
	return Result;
}

S8  EM_Set_YPosition(unsigned char Y8)
{
	S8 Result;

	Y8 &= 0xF;
	Y8 <<= 4;
	Result=EM_Send_Cmd(2,Y8);	
	return Result;
}

S8  EM_Set_XPosition(unsigned char X8)
{
	S8 Result;

    // Note: Vertical is assumed to be zero
	X8 &= 0x7F;
	X8 <<= 1;
	Result=EM_Send_Cmd(1,X8);	
	return Result;
}

