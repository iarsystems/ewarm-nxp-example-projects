/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : IAR-LPC1788-SK USB Audio Device
 *
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-1788-SK board.  It implements a usb audio device
 * with one output, touch screen, sound effects loudness and wide stereo.
 *
 * Jumpers:
 *  PWR_SEL    - depending on the power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 1543 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define TIMER0_TICK_PER_SEC   2
#define UPDATE_SHOW_DLY       1
#define DLY_100US             500

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

volatile Int32U Tick;

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

/** external data **/
#pragma section=".intvec"
/** public functions **/
/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL0 and clocks' dividers. PLL = 96MHz,
 *               CPU - 96MHz, PCLK - 48 MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /*Sys Clock Select as CPU clock
    divider 1:1*/
  CLK_SetCpuClk(CPUSEL_CLKSYS,1);
  /*Select IRC oscilator as Sys clock*/
  CLK_SetSysClk(SYSSEL_IRCOSC);
  /*Enable Main oscilator*/
  CLK_MainOscSet(CLK_ENABLE,MOSCRNG_1_20MHZ);
  /*Select Main oscilator as Sys clock*/
  CLK_SetSysClk(SYSSEL_MOSC);
  /*Enable set PLL 96Hz (M = 8; P = 1)*/
  CLK_SetMainPll(CLK_ENABLE, 8-1, 0);
  /*Set peripheral divider 1:2. Peripheral clock 48MHz*/
  CLK_SetPeriphClk(2);
  /*PLL0 out is used as the input to the CPU clock divider
  divider 1:1. CPU runs at 96MHz*/
  CLK_SetCpuClk(CPUSEL_CLKPLL,1);
}

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: void
 * Return:
 *
 * Description: Low level system init (clock, flash accelerator,
 *              SDRAM and vector table address)
 *
 *
 *************************************************************************/
int __low_level_init (void)
{
  /*if debug is in SDRAM
    clock and SDRAM initialization is made
    in mac file*/
#ifndef SDRAM_DEBUG
  int cpuclk;
  /* Flash accelerator safe value*/
  FLASHCFG = (5UL<<12) | 0x3AUL;
  /*Clock Init*/
  InitClock();
  /*Flash accelerator init*/
  cpuclk = CLK_GetClock(CLK_CPU);

  if( cpuclk < 20000000 ){
    FLASHCFG = (0x0UL<<12) | 0x3AUL;
  } else if( cpuclk < 40000000 ){
    FLASHCFG = (0x1UL<<12) | 0x3AUL;
  } else if( cpuclk< 60000000 ){
    FLASHCFG = (0x2UL<<12) | 0x3AUL;
  } else if( cpuclk < 80000000 ){
    FLASHCFG = (0x3UL<<12) | 0x3AUL;
  } else if( cpuclk < 100000000 ){
    FLASHCFG = (0x4UL<<12) | 0x3AUL;
  }
  /*SDRAM init*/
  SDRAM_Init();
#endif
  /*Set vector table location*/
  VTOR  = (unsigned int)__segment_begin(".intvec");

  return  1;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
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

  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Init LCD and copy picture in video RAM*/
  GLCD_Init (IarLogoPic.pPicStream, NULL);

  GLCD_LoadPic(0  ,180,&LoudnessPic, 0);
  GLCD_LoadPic(269,180,&WideStereoPic, 0);

  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Configure USB Clock*/
  CLK_SetUsbClk(USBSEL_CLKPLL,USB_CLK_DIV);

  /* Init Audio */
  UsbAudioClassInit();

  /* Init touch screen */
  TouchScrInit();

  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;

  __enable_interrupt();

  /* Soft connection enable */
  USB_ConnectRes(TRUE);


  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetWindow(55,195,268,218);
  GLCD_TextSetPos(0,0);

  VS1002_SendCmd(VS1002_CmdPowerUp,(pInt32U)&VS1002_Status);
  if(VS1002_Pass == VS1002_Status)
  {
    GLCD_print("\fAudio class device");
  }
  else
  {
    GLCD_print("\fVS1002 init fault");
  }

  Volume = 0xFFFF;
  VS1002_SendCmd(VS1002_CmdSetVol,&Volume);
  VS1002_SendCmd(VS1002_CmdWideStereo,(pInt32U)&WideStereo);
  VS1002_SendCmd(VS1002_CmdLoudness,(pInt32U)&Loudness);
  VS1002_SendCmd(VS1002_CmdPlayStop,&Tmp);

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
/** private functions **/
