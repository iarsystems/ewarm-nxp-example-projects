// -------------------------------------------------------------------
// -----------------------LPC2106 PS/2 keyboard API-------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board by PASAT
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */
#include "my_files.h"    /* UART1 Module driver header file */
#include "lcd_api.h"

#include "mainmb40.h"
#include "key_api.h"
#include "key_scan.h"

#define BUFF_SIZE 64

U8 BitNr;
U8 Data8;

unsigned char Key_Buffer[BUFF_SIZE];
unsigned char *KeyIn_Ptr, *KeyOut_Ptr;
unsigned char Key_BufferCnt;

// Function prototypes
void Key_PutToBuffer(unsigned char c);


void Init_Keyboard(void)
{
    KeyIn_Ptr =  Key_Buffer; 	// Initialize buffer
    KeyOut_Ptr = Key_Buffer;
    Key_BufferCnt = 0;

    BitNr = 11;
}

void Key_Decode(unsigned char sc)
{
    static unsigned char shift = 0;
    //static unsigned char Scroll_Flag = 0;
    static unsigned char State_Flag=0;
    unsigned char i;

	switch (State_Flag)
	{     
	    case 0:		//anything
		{
			switch (sc)
        	{
          		case 0xF0:	        // Release mode 
	            	State_Flag = 1;
    	        	break;

				case 0xE0:			// Extended mode
        	    	State_Flag = 2;
            		break;

		    	case 0x12 :        // Left SHIFT
	            	shift = 1;
    	        	break;

        		case 0x59 :        // Right SHIFT
	            	shift = 1;
	            	break;
        		//case 0x7E :        // Scroll
        		// 	if (!Scroll_Flag) Scroll_Flag = 1;
				//	else              Scroll_Flag = 0;
	            //	break;
					
		        default:
	                if(!shift)                    // If shift not pressed,
    	            {                            // do a table look-up
        	            for(i = 0;  UnshiftedKey_Codes_Table[i][0]!=sc &&  UnshiftedKey_Codes_Table[i][0]; i++);
            	        if ( UnshiftedKey_Codes_Table[i][0] == sc)
            	        {
                        	Key_PutToBuffer( UnshiftedKey_Codes_Table[i][1]);
							if (UnshiftedKey_Codes_Table[i][2])
								Key_PutToBuffer(UnshiftedKey_Codes_Table[i][2]);
	                    }
    	            }
    	            else
    	            {                    // If shift pressed
                    	for(i = 0; ShiftedKey_Codes_Table[i][0]!=sc && ShiftedKey_Codes_Table[i][0]; i++);
                    	if (ShiftedKey_Codes_Table[i][0] == sc)
                    	{
            	            Key_PutToBuffer(ShiftedKey_Codes_Table[i][1]);
							if (ShiftedKey_Codes_Table[i][2])
								Key_PutToBuffer(ShiftedKey_Codes_Table[i][2]);
                    	}
                	}
                //put_kbbuff(' ');
                //put_kbbuff(' ');
            }
            break;
        }	// End of case 0

	    case 1:		//  Release
		{
			State_Flag = 0;
	        switch (sc)
    	    {
        		case 0x12 :                        // Left SHIFT
            	shift = 0;
            	break;
            
          		case 0x59 :                        // Right SHIFT
            	shift = 0;
            	break;
	        } 
            break;
    	}    


	    case 2:		//  Extended Make
		{
	        switch (sc)
    	    {
        		case 0xF0 :			// Release Extended
	            State_Flag = 1;
	            break;

		        default:			// Extended - find one
       	            for(i = 0; ExtKey_Codes_Table[i][0]!=sc && ExtKey_Codes_Table[i][0]; i++);
           	        if (ExtKey_Codes_Table[i][0] == sc)
           	        {
                       	Key_PutToBuffer(ExtKey_Codes_Table[i][1]);
                       	Key_PutToBuffer(ExtKey_Codes_Table[i][2]);
                    }
				State_Flag = 0;
				break;
			}
            break;
		}
	}
} 


void Key_PutToBuffer(unsigned char c)
{
    if (Key_BufferCnt<BUFF_SIZE)                        // If buffer not full
    {
        *KeyIn_Ptr = c;                                // Put character into buffer
        KeyIn_Ptr++;                                    // Increment pointer

        Key_BufferCnt++;

        if (KeyIn_Ptr >= Key_Buffer + BUFF_SIZE)        // Pointer wrapping
            KeyIn_Ptr = Key_Buffer;
    }
}

int Get_Char_Keyboard(void)
{
    int byte;
    while(Key_BufferCnt == 0);                        // Wait for data

    byte = *KeyOut_Ptr;                                // Get byte
    KeyOut_Ptr++;                                    // Increment pointer

    if (KeyOut_Ptr >= Key_Buffer + BUFF_SIZE)            // Pointer wrapping
        KeyOut_Ptr = Key_Buffer;
    
    Key_BufferCnt--;                                    // Decrement buffer count

    return byte;
}


int Test_Char_Keyboard(void)
{
    if (Key_BufferCnt == 0) return 0;				// Check if any keycodes
	return 1;
}

//----------------------------------------------------------------
// Exec_Keyboard_PC
//----------------------------------------------------------------
// If PS/2 key is pressed then send it (via UART) to PC
// Exited when ESC is pressed on remote terminal (PC)
//----------------------------------------------------------------
void Exec_Keyboard_PC(void)
{
	U8	pChar,Val8;
	S8 Result8;

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Press key(s) on PS/2 keyboard attached to TinyARM ");
	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" ------- Press ESC on PC's keyboard to exit -------");
	My_PutChar(0xD);
	My_PutChar(0xA);

	for (;;)
	{
		while (Test_Char_Keyboard()==0)
		{
		    Result8=My_WaitForChar(&pChar);
			if (Result8) pChar=0;
			if (pChar == KEY_ESC)
			{
				My_PutChar(0xD);
				My_PutChar(0xA);
			 	return;
			}

		}

		Val8 = Get_Char_Keyboard();
		My_PutChar(Val8);
	}
}

//----------------------------------------------------------------
// Exec_Keyboard_LCD
//----------------------------------------------------------------
// If PS/2 key is pressed then character is displayed on LCD
// Exited when ESC is pressed on remote terminal (PC)
//----------------------------------------------------------------
void Exec_Keyboard_LCD(void)
{
	U8	pChar,Val8;
	S8 Result8;

	//EM_Initialise_LCD();
	Lcd_Init_API();

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Press key(s) on PS/2 keyboard attached to TinyARM ");
	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" You should see the characters on your graphic LCD ");
	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" ------- Press ESC on PC's keyboard to exit -------");
	My_PutChar(0xD);
	My_PutChar(0xA);

	for (;;)
	{
		while (Test_Char_Keyboard()==0)
		{
		    Result8=My_WaitForChar(&pChar);
			if (Result8) pChar=0;
			if (pChar == KEY_ESC)
			{
				My_PutChar(0xD);
				My_PutChar(0xA);
			 	return;
			}

		}

		Val8 = Get_Char_Keyboard();

		// Handling extended keys
		if ((Val8==0xE0) | (Val8==0x00))
		{
			Val8 = Get_Char_Keyboard();
			Lcd_Put_Ctrl(Val8);
		}
		else
			Lcd_Putchar(Val8);
		// if <ENTER> add LF after CR
		if (Val8==0xD) Lcd_Putchar(0xA);

	}

}
