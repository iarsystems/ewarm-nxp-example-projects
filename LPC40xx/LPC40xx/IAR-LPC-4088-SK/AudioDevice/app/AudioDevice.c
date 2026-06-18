/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : AudioDevice.c
 *    Description : IAR-LPC4088-SK AudioDevice Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for IAR-LPC-4088-SK board.It implements a usb audio device
 *  with one output, touch screen, sound effects loudness and wide stereo.
 * 
 *   Press "LOUD" and "WIDE STEREO" buttons on the screen to enable/disable
 *  the sound effects.
 *
 * Jumpers:
 *  PWR_SEL    - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 * 
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "LPC407x_8x.h"
#include "board.h"

#include "arm_comm.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"
#include "drv_touch_scr.h"
#include "spifi_rom_api.h"
#include "drv_vs1002.h"

#include "usb_desc.h"
#include "usb_cnfg.h"
#include "usb_hw.h"
#include "usb_t9.h"

#include "audio_class.h"

#define DLY_100US             500

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t Lun0Buffer[512];

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

#pragma data_alignment=4
const uint8_t RIFF_Header[] =
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

/** public data **/
volatile uint32_t CriticalSecCntr;
SPIFIobj obj;
/** external data **/
#pragma section=".intvec"
/** public functions **/
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
  /*Here the clock is already set by
    SystemInit function*/
  
  /*SDRAM init*/
  SDRAM_Init();

  /*SPIFI initialization*/
  LPC_SC->PCONP |= (1<<16);     /* enable SPIFI clock */
  LPC_SC->PCONP |= (1<<15);     /* enable IOCON clock */

  /*SPIFI Configure Pins*/
  SPIFI_IO0_IOCON = 0x35;
  SPIFI_IO1_IOCON = 0x35;
  SPIFI_IO2_IOCON = 0x35;
  SPIFI_IO3_IOCON = 0x35;
  SPIFI_CLK_IOCON = 0x35;
  SPIFI_CS_IOCON = 0x35;
	/* Initialize SPIFI driver */
	spifi_init(&obj, 7, S_RCVCLK | S_FULLCLK, SPIFIClock/1000000);
  
  return  1;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: uint32_t Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile uint32_t j = DLY_100US; j; j--)
    {
    }
  }
}

/*************************************************************************
 * Function Name: CheckRectangle
 * Parameters: uint32_t X_up, uint32_t Y_up - rectangle coordinate
 *             uint32_t X_size, uint32_t Y_size - rectangle size
 *             uint32_t X_point, uint32_t Y_point - point coordinate
 *
 * Return: Boolean
 *    TRUE  - the point is inside of the rectangle
 *    FALSE - the point is outside of the rectangle
 *
 * Description: Checks whether the coordinates of a point are inside of a 
 *              rectangle
 *
 *************************************************************************/
Boolean CheckRectangle (uint32_t X_up, uint32_t Y_up,
                        uint32_t X_size, uint32_t Y_size,
                        uint32_t X_point, uint32_t Y_point )
{
  if((X_up <= X_point) && (X_point <= X_up+X_size) &&
     (Y_up <= Y_point) && (Y_point <= Y_up+Y_size))
  {
    return (TRUE);
  }
  return (FALSE);
}

#define BMP_IMG_ADDR  0x28000000
#define IMG_NUM 9

pPic_t pPic[IMG_NUM] = { NULL, NULL, NULL, NULL, NULL,
                         NULL, NULL, NULL, NULL};

#define IMG_LOUD      (IMG_NUM-2)
#define IMG_STEREO    (IMG_NUM-1)

#define IMG_LOUD_X    (8)
#define IMG_LOUD_Y    (272-60)
#define IMG_STEREO_X  (72)
#define IMG_STEREO_Y  (272-60)

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
ToushRes_t XY_Touch;
uint32_t Touch = FALSE;
VS1002_Status_t VS1002_Status;
uint32_t Volume,Tmp;
VS1002_Stream_t VS1002_Stream;
VS1002_Stream_t VS1002_Stream_RIFF_Header;
Boolean Play = FALSE;
uint32_t WideStereo = 0;
uint32_t Loudness = 0;
  
bmp_t * bmp_file = (bmp_t *)BMP_IMG_ADDR;

  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Load Pictures from bmp files to SDRAM*/ 
  for(uint32_t i = 0; i < IMG_NUM ;i ++)
  {
    if(NULL == (pPic[i] = GLCD_LoadPic(bmp_file))) break;
    bmp_file = (bmp_t *)((uint8_t *)bmp_file + bmp_file->bmp_size);
  }
  /*Init LCD and copy picture ot VRAM*/
  GLCD_Init (pPic[0], NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Show loud pic*/
  if( NULL != pPic[IMG_LOUD])
  {
    GLCD_ShowPic(IMG_LOUD_X,IMG_LOUD_Y,pPic[IMG_LOUD],0);
  }
  /*Show loud stereo*/
  if( NULL != pPic[IMG_STEREO])
  {
    GLCD_ShowPic(IMG_STEREO_X,IMG_STEREO_Y,pPic[IMG_STEREO],0);
  }
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);

  /* Init touch screen */
  TouchScrInit();


  /* Init Audio */
  UsbAudioClassInit();
  
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  
  __enable_interrupt();

  /* Soft connection enable */
  USB_ConnectRes(TRUE);

  
  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000CCFF);
  GLCD_SetWindow(132,124,348,148);
  GLCD_TextSetPos(0,0);
  
  VS1002_SendCmd(VS1002_CmdPowerUp,(uint32_t *)&VS1002_Status);
  if(VS1002_Pass == VS1002_Status)
  {
    GLCD_print("\fAudio Class Device");
  }
  else
  {
    GLCD_print("\fVS1002 init fault");
  }

  Volume = 0xFFFF;
  VS1002_SendCmd(VS1002_CmdSetVol,&Volume);
  VS1002_SendCmd(VS1002_CmdWideStereo,(uint32_t *)&WideStereo);
  VS1002_SendCmd(VS1002_CmdLoudness,(uint32_t *)&Loudness);
  VS1002_SendCmd(VS1002_CmdPlayStop,&Tmp);

  VS1002_Stream_RIFF_Header.pStream = (uint8_t *)RIFF_Header;
  VS1002_Stream_RIFF_Header.Size = sizeof(RIFF_Header);
  
  while(1)
  {
    if (UsbAudioGetPlayStatus())
    {
      if (FALSE == Play)
      {
        Play = TRUE;
        VS1002_SendCmd(VS1002_CmdPlay,(uint32_t *)&VS1002_Stream_RIFF_Header);
      }
      if( 0 != (VS1002_Stream.Size = UsbAudioGetLinBuffer(&VS1002_Stream.pStream)))
      {
        VS1002_SendCmd(VS1002_CmdPlay,(uint32_t *)&VS1002_Stream);
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
        if(CheckRectangle(IMG_STEREO_X, IMG_STEREO_Y, 52, 52,XY_Touch.X,XY_Touch.Y))
        {
          WideStereo ^= 1;
          VS1002_SendCmd(VS1002_CmdWideStereo,(uint32_t *)&WideStereo);
          if( NULL != pPic[IMG_STEREO])
          {
            GLCD_ShowPic(IMG_STEREO_X,IMG_STEREO_Y,pPic[IMG_STEREO],WideStereo?0x00FF00:0);
          }
        }
        else if (CheckRectangle(IMG_LOUD_X, IMG_LOUD_Y, 52, 52, XY_Touch.X,XY_Touch.Y))
        {
          Loudness ^= 1;
          VS1002_SendCmd(VS1002_CmdLoudness,(uint32_t *)&Loudness);
           if( NULL != pPic[IMG_LOUD])
          {
            GLCD_ShowPic(IMG_LOUD_X,IMG_LOUD_Y,pPic[IMG_LOUD],Loudness?0x00FF00:0);
          }
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
