/*******************************************************************************
    This module implements a linux character device driver for the QVGA chip.
    Copyright (C) 2006  Embedded Artists AB (www.embeddedartists.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/
#include "lcd_hw.h"
#include "lpc313x_timer_driver.h"


unsigned char activeController;


#define delayMs(ms) timer_wait_ms(NULL, ms);


/******************************************************************************
** Function name:		lcd_hw_init
**
** Descriptions:		Initialize hardware for LCD controller, it includes
**						both EMC and I/O initialization. External CS2 is used 
**						for LCD controller.		
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void lcd_hw_init(void)
{
#if (QVGA_LCD_IF == PARALLEL_16_LCD_IF) || (QVGA_LCD_IF == PARALLEL_8_LCD_IF)
  /****************************************************************
   * Initialize EMC for CS2
   ****************************************************************/
  //for NAND FLASH
//  EMC_STA_CFG0      = 0x00000080;
//  EMC_STA_WAITWEN0  = 0x2;
//  EMC_STA_WAITOEN0  = 0x2;
//  EMC_STA_WAITRD0   = 0x1f;
//  EMC_STA_WAITPAGE0 = 0x1f;
//  EMC_STA_WAITWR0   = 0x1f;
//  EMC_STA_WAITTURN0 = 0xf;

#endif

  /****************************************************************
   * Setup control of backlight
   ****************************************************************/
  return;
}

void
writeToDisp(unsigned short data)
{
  LCD_DATA_16 = data;
}

unsigned short
readFromDisp(void)
{
  volatile unsigned short value;

  value = LCD_DATA_16;  //dummy data
  value = LCD_DATA_16;
  return value;
}

void
writeToReg(unsigned short addr, unsigned short data)
{
  LCD_COMMAND_16 = addr;
  LCD_DATA_16    = data;
  LCD_COMMAND_16 = 0x22;  //restore index register to GRAM
}

//Note: Does not work on V2 controllers when using serial interface
unsigned short
readFromReg(unsigned char addr)
{
  LCD_COMMAND_16 = addr;
  return LCD_DATA_16;
}

/******************************************************************************
** Function name:		writeLcdCommand
**
** Descriptions:		
**
** parameters:			Command (16-bits)
** Returned value:	None
** 
******************************************************************************/
void
writeLcdCommand(unsigned short command)
{
  writeToReg(command, 0);
  switch ((command >> 8) & 0xff)
  {
    case 3:
    case 24:
    case 25:
    case 26:
    case 27:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 40:
    case 55:
      delayMs(1);
    default:
      break;
  }
}



/******************************************************************************
** Function name:		lcd_init
**
** Descriptions:		Read LCD controller R49 and R50 to make sure
**						the controller has been initialized correctly,
**						then, turn on system, set display, adjust GAMMA,
**						finally, display.		
**
** parameters:			None
** Returned value:		TRUE or FALSE
** 
******************************************************************************/
unsigned int lcd_init(void)
{
//  unsigned int result;
  
  /****************************************************************
   * Check if contact with V2 Lcd controller (read register R00 = 0x8989)
   ****************************************************************/
  activeController = V2_CONTROLLER;
if(1)//  if (readFromReg(0) == 0x8989)
  {
    activeController = V2_CONTROLLER;
    
    /****************************************************************
     * Initialize Lcd controller (long sequence) 
     ****************************************************************/
#if 1 /*  only to change here */

   /*Display Control Setting*/
   
   writeToReg (0x00,0x0001);    //Driver output control register
   delayMs(2);
   writeToReg (0x03,0xA2A4);    //LCD Driving waveform control register
   writeToReg (0x0C,0x0004);    //BGR=0,
   writeToReg (0x0D,0x0308);    //16-bit compare register
   writeToReg (0x0E,0x3000);    //display control register 2
   delayMs(10);
   
   writeToReg (0x1E,0x00EA);
   
   writeToReg (0x01,0x2B3F);    //Frame cycle control register
   writeToReg (0x02,0x0600);    //gate scan position register
   writeToReg (0x10,0x0000);    //CP/WM 18/16-bit selection register 16bit
   writeToReg (0x11,0x6030);    //vertical scroll control
   writeToReg (0x07,0x0233);
   writeToReg (0x0B,0x0039);
   writeToReg (0x0F,0x0000);    //HEA=128,HSA=0
   
   delayMs(5);
   writeToReg (0x30,0x0707);    //VEA=160,VSA=0
   writeToReg (0x31,0x0204);
   writeToReg (0x32,0x0204);
   writeToReg (0x33,0x0502);
   writeToReg (0x34,0x0507);
   writeToReg (0x35,0x0204);
   writeToReg (0x36,0x0204);
   writeToReg (0x37,0x0502);
   writeToReg (0x3A,0x0302);
   writeToReg (0x3B,0x0302);

   writeToReg (0x23,0x0000);    //start oscillation
   writeToReg (0x24,0x0000);    //
   writeToReg (0x25,0xA000);

   writeToReg (0x48,0x0000);    //VCOM,VCOMH set
   writeToReg (0x49,0x013F);    //REGP=vci
   writeToReg (0x4A,0x0000);    //
   writeToReg (0x4B,0x0000);    //

   writeToReg (0x41,0x0000);    //VCOM,VCOMH set
   writeToReg (0x42,0x0000);    //VREG1OUT=1.9*REGP

   writeToReg (0x44,0xEF00);    //
   writeToReg (0x45,0x0000);    //fosc/8,
   writeToReg (0x46,0x013F);    //VGH=6*VCI1,VGL=-4*VCI1,fosc/8,
   writeToReg (0x22,0x0000);	//GON=1 DTE=1 D1-0=11

   delayMs(5);
   writeToReg (0x44,0xEF00);	//GON=0 DTE=0 D1-0=01
   writeToReg (0x45,0x0000);	//GON=1 DTE=0 D1-0=01
   writeToReg (0x4E,0x0000);	//GON=1 DTE=0 D1-0=11
   writeToReg (0x4F,0x0000);	//GON=1 DTE=1 D1-0=11
   writeToReg (0x46,0x013F);	//GON=1 DTE=1 D1-0=11
   writeToReg (0x22,0x0000);	//GON=1 DTE=1 D1-0=11

  
   
 #if 0  
   writeToReg (0x00,0x0001);     //Driver output control register
   mdelay(20);
   writeToReg (0x03,0xA2A4);     //LCD Driving waveform control register
   writeToReg (0x0C,0x0004);	    //BGR=0,
   writeToReg (0x0D,0x0308);     //16-bit compare register
   writeToReg (0x0E,0x3000);     //display control register 2
   mdelay(50);
   writeToReg (0x1E,0x00AF);
   writeToReg (0x01,0x2B3F);     //Frame cycle control register
   writeToReg (0x02,0x0600);	    //gate scan position register
//   writeToReg (0x10,0x0000);     //CP/WM 18/16-bit selection register    16bit
//   writeToReg (0x11,0x6830);     //vertical scroll control
   writeToReg (0x07,0x0233);
   writeToReg (0x0B,0x0039);
   writeToReg (0x0F,0x0000);	    //HEA=128,HSA=0
   mdelay(50);


   writeToReg (0x30,0x0707);	    //VEA=160,VSA=0
   writeToReg (0x31,0x0204);
   writeToReg (0x32,0x0204);
   writeToReg (0x33,0x0502);
   writeToReg (0x34,0x0507);
   writeToReg (0x35,0x0204);
   writeToReg (0x36,0x0204);
   writeToReg (0x37,0x0502);
   writeToReg (0x3A,0x0302);
   writeToReg (0x3B,0x0302);

   writeToReg (0x23,0x0000);            //start oscillation
   writeToReg (0x24,0x0000);            //

   writeToReg (0x48,0x0000);            //VCOM,VCOMH set
   writeToReg (0x49,0x013F);             //REGP=vci
   writeToReg (0x4A,0x0000);             //
   writeToReg (0x4B,0x0000);//

   writeToReg (0x41,0x0000);//VCOM,VCOMH set
   writeToReg (0x42,0x0000);//VREG1OUT=1.9*REGP

   writeToReg (0x44,0xEF00);//
   writeToReg (0x45,0x0000);              //fosc/8,
   writeToReg (0x46,0x013F);              //VGH=6*VCI1,VGL=-4*VCI1,fosc/8,
   mdelay(50);

   //Display ON Flow
   writeToReg (0x44,0xEF00);	//GON=0 DTE=0 D1-0=01
   writeToReg (0x45,0x0000);	//GON=1 DTE=0 D1-0=01
   writeToReg (0x4E,0x0000);	//GON=1 DTE=0 D1-0=11
   writeToReg (0x4F,0x0000);	//GON=1 DTE=1 D1-0=11
   writeToReg (0x46,0x013F);	//GON=1 DTE=1 D1-0=11
//   writeToReg (0x22,);	//GON=1 DTE=1 D1-0=11

  #endif 
   
#else


  writeToReg(0x07, 0x0021);
  writeToReg(0x00, 0x0001);
  writeToReg(0x07, 0x0723);
  writeToReg(0x10, 0x0000);
if(0)
{
  writeToReg(0x03, 0xa8a6);
  writeToReg(0x0c, 0x0005);
  writeToReg(0x0d, 0x300b);
  writeToReg(0x0e, 0x2000);
}
  delayMs(50);
if(1)
{
  writeToReg(0x1e, 0x00AF);  
}
  writeToReg(0x07, 0x0033);
  writeToReg(0x11, 0x6830);
//writeToReg(0x11, 0x7330);
  writeToReg(0x02, 0x0600);
  writeToReg(0x0f, 0x0000);
  writeToReg(0x01, 0x2b3f);
  writeToReg(0x0b, 0x5308);
  writeToReg(0x25, 0xa000);
//  writeToReg(0x03, 0xa0ae);  

#endif
  
    return( 1 );
  }

//  return( 0 );
}

