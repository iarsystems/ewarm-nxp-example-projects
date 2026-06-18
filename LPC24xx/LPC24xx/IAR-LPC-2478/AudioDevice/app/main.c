/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : August 5, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC2478-SK board.
 *  It implements a usb audio device with one output, touch screen, sound
 * effects loudness and wide stereo.
 *
 * Jumpers:
 *  EXT/JLINK  - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *  BDS_E      - unfilled
 *  C/SC       - SC
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LCD shares pins with Trace port. If ETM is enabled the LCD will not work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#ifndef SDRAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)
#define DLY_100US             500

extern Int32U SDRAM_BASE_ADDR;
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

#pragma data_alignment=4
const Int8U RIFF_Header[] =
{
  'R','I','F','F',
  58,0,0,0,
  'W','A','V','E',
  'f','m','t',' ',
  0x12,0,0,0,
  1,0,
  1,0,
  0x80,0xBB,0x00,0x00,
  0x00,0x77,0x01,0x00,
  2,0,0x10,0,0,0,
  'f','a','c','t',
  4,0,0,0,
  0xFF,0xFF,0xFF,0xFF,
  'd','a','t','a',
  0xFF,0xFF,0xFF,0xFF,
};

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}

/*************************************************************************
 * Function Name: CheckRectangle
 * Parameters: Int32U X_up, Int32U Y_up - rectangle coordinate
 *             Int32U X_size, Int32U Y_size - rectangle size
 *             Int32U X_poin, Int32U Y_point - point coordinate
 *
 * Return: Boolean
 *    TRUE  - the point is inside  from the rectangle
 *    FALSE - the point is outside from the rectangle
 *
 * Description: Check whether the coordinate of point is inside from a rectangle
 *
 *************************************************************************/
Boolean CheckRectangle (Int32U X_up, Int32U Y_up,
                        Int32U X_size, Int32U Y_size,
                        Int32U X_point, Int32U Y_point )
{
  if((X_up <= X_point) && (X_point <= X_up+X_size) &&
     (Y_up <= Y_point) && (Y_point <= Y_up+Y_size))
  {
    return (TRUE);
  }
  return (FALSE);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
VS1002_Status_t VS1002_Status;
Int32U Volume,Tmp;
VS1002_Stream_t VS1002_Stream;
VS1002_Stream_t VS1002_Stream_RIFF_Header;
Boolean Play = FALSE;
ToushRes_t XY_Touch;
Boolean Touch = FALSE;
Int32U WideStereo = 0;
Int32U Loudness = 0;

  VS1002_Stream_RIFF_Header.pStream = (pInt8U)RIFF_Header;
  VS1002_Stream_RIFF_Header.Size = sizeof(RIFF_Header);

  GLCD_Ctrl (FALSE);
  // Init GPIO
  GpioInit();
#ifndef SDRAM_DEBUG
  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
  // Init clock
  InitClock();
  // SDRAM Init
  SDRAM_Init();
#endif
  // Init VIC
  VIC_Init();
  // GLCD init
  GLCD_Init (IarLogoPic.pPicStream, NULL);
  // Disable Hardware cursor
  GLCD_Cursor_Dis(0);

  // Init Audio
  UsbAudioClassInit();

  // Init touch screen
  TouchScrInit();
  // Touched indication LED
  USB_H_LINK_LED_SEL = 0; // GPIO
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;

  __enable_interrupt();

  VS1002_SendCmd(VS1002_CmdPowerUp,(pInt32U)&VS1002_Status);
  Volume = 0xFFFF;
  VS1002_SendCmd(VS1002_CmdSetVol,&Volume);

  // Soft connection enable
  USB_ConnectRes(TRUE);

  GLCD_LoadPic(0  ,180,&LoudnessPic, 0);
  GLCD_LoadPic(269,180,&WideStereoPic, 0);

  // Enable GLCD
  GLCD_Ctrl (TRUE);

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetWindow(55,195,268,218);
  GLCD_TextSetPos(0,0);
  if(VS1002_Pass == VS1002_Status)
  {
    GLCD_print("\fAudio class device");
  }
  else
  {
    GLCD_print("\fVS1002 init fault");
  }

  VS1002_SendCmd(VS1002_CmdWideStereo,(pInt32U)&WideStereo);
  VS1002_SendCmd(VS1002_CmdLoudness,(pInt32U)&Loudness);

  while(1)
  {
    if (UsbAudioGetPlayStatus())
    {
      if (FALSE == Play)
      {
        Play = TRUE;
        VS1002_SendCmd(VS1002_CmdPlay,(pInt32U)&VS1002_Stream_RIFF_Header);
      }
      if( 0 != (VS1002_Stream.Size = UsbAudioGetLinBuffer(&VS1002_Stream.pStream)))
      {
        VS1002_SendCmd(VS1002_CmdPlay,(pInt32U)&VS1002_Stream);
      }
    }
    else if (Play)
    {
      Play = FALSE;
      VS1002_SendCmd(VS1002_CmdPlayStop,&Tmp);
    }

    if(UsbAudioVolumeGetVolume(&Volume))
    {
      if(0x8000 == Volume)
      {
        // mute
        Volume = 0xFF;
      }
      else
      {
        Volume >>= 7;
        Volume   = 12 - Volume;
        Volume  &= 0xFF;
      }
      Volume  *= 0x101;
      VS1002_SendCmd(VS1002_CmdSetVol,&Volume);
    }

    if(TouchGet(&XY_Touch))
    {
      if(FALSE == Touch)
      {
        Touch = TRUE;
        USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
        if(CheckRectangle(269, 180, 50, 50,XY_Touch.X,XY_Touch.Y))
        {
          WideStereo ^= 1;
          VS1002_SendCmd(VS1002_CmdWideStereo,(pInt32U)&WideStereo);
          GLCD_LoadPic(269,180,&WideStereoPic, WideStereo?0x00FF00:0);
        }
        else if (CheckRectangle(0, 180, 52, 52, XY_Touch.X,XY_Touch.Y))
        {
          Loudness ^= 1;
          VS1002_SendCmd(VS1002_CmdLoudness,(pInt32U)&Loudness);
          GLCD_LoadPic(0  ,180,&LoudnessPic, Loudness?0x00FF00:0);
        }
      }
    }
    else if(Touch)
    {
      USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
      Touch = FALSE;
    }
  }
}
