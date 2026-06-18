/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : TouchDemo.c
 *    Description : IAR-LPC4088-SK Touch screen Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for IAR-LPC-4088-SK board. It shows basic use of I/O,
 *  timer, interrupt, LCD controllers and LCD touch screen.
 *
 *    A cursor is shown and moves when the screen is touched. You can change the
 *  pictures on the LCD by touching the "Next" and the "Prev" buttons.
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
#include <yfuns.h>
#include <stdint.h>

#include "LPC407x_8x.h"
#include "board.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"
#include "cursor_arrow.h"
#include "drv_touch_scr.h"
#include "spifi_rom_api.h"

/**/
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
#define IMG_NUM       7
#define IMG_NEXT      (IMG_NUM-2)
#define IMG_PREV      (IMG_NUM-1)

#define IMG_NEXT_X    (480-76)
#define IMG_NEXT_Y    (272-32)
#define IMG_PREV_X    (0)
#define IMG_PREV_Y    (272-32)
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
int cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
pPic_t pPic[IMG_NUM] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL}; 
ToushRes_t XY_Touch;
uint32_t Touch = FALSE;
bmp_t * bmp_file = (bmp_t *)BMP_IMG_ADDR;
uint32_t Pic_Inedx = 0;
  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Load Picture from bmp file to SDRAM*/ 
  for(uint32_t i = 0; i < IMG_NUM ;i ++)
  {
    if(NULL == (pPic[i] = GLCD_LoadPic(bmp_file))) break;
    bmp_file = (bmp_t *)((uint8_t *)bmp_file + bmp_file->bmp_size);
  }
  /*Init LCD and copy picture ot VRAM*/
  GLCD_Init (pPic[0], NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Show navigation pics*/
  if( NULL != pPic[IMG_NEXT])
  {
    GLCD_ShowPic(IMG_NEXT_X,IMG_NEXT_Y,pPic[IMG_NEXT],0);
  }
  /*Show navigation pics*/
  if( NULL != pPic[IMG_PREV])
  {
    GLCD_ShowPic(IMG_PREV_X,IMG_PREV_Y,pPic[IMG_PREV],0);
  }
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Copy cursor in curosor RAM*/
  GLCD_Copy_Cursor ((uint32_t *)Cursor, 0, sizeof(Cursor)/sizeof(uint32_t));
  /*Cursor coordinates are synchronized
    cursor size is 32/32 pix*/
  GLCD_Cursor_Cfg(CRSR_FRAME_SYNC | CRSR_PIX_32);
  /*Set initial cursor position*/
  GLCD_Move_Cursor(cursor_x, cursor_y);
  /*Enable Cursor*/
  GLCD_Cursor_En(0);
  /* Init touch screen */
  TouchScrInit(); 
  /*Enable interrupts*/
  __enable_interrupt();

  /*Init USB Host Link LED*/
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;

  while(1)
  {
    if(TouchGet(&XY_Touch))
    {
      cursor_x = XY_Touch.X;
      cursor_y = XY_Touch.Y;
      GLCD_Move_Cursor(cursor_x, cursor_y);
      if (FALSE == Touch)
      {
        Touch = TRUE;
        USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
        
        if((NULL != pPic[IMG_NEXT]) && (NULL != pPic[IMG_PREV]))
        { 
          if((0 < Pic_Inedx) && 
             ( TRUE == CheckRectangle(IMG_PREV_X,IMG_PREV_Y,76, 32, cursor_x, cursor_y)))
          {
            --Pic_Inedx ;
            GLCD_ShowPic(0,0,pPic[Pic_Inedx],0);
            GLCD_ShowPic(IMG_NEXT_X,IMG_NEXT_Y,pPic[IMG_NEXT],0);
            GLCD_ShowPic(IMG_PREV_X,IMG_PREV_Y,pPic[IMG_PREV],0);
          }
          else
          { 
            if(((IMG_NEXT-1) > Pic_Inedx) &&
               ( TRUE == CheckRectangle(IMG_NEXT_X, IMG_NEXT_Y,76, 32,cursor_x,cursor_y)))
            { 
              ++Pic_Inedx ;
              GLCD_ShowPic(0,0,pPic[Pic_Inedx],0);
              GLCD_ShowPic(IMG_NEXT_X,IMG_NEXT_Y,pPic[IMG_NEXT],0);
              GLCD_ShowPic(IMG_PREV_X,IMG_PREV_Y,pPic[IMG_PREV],0);
            }
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
