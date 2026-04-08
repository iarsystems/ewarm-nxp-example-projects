/*
 * File:		lcdc_color.c
 * Purpose:		LCD Color Demo
 *                      Display RGB colors to the screen
 *
 */

#include "lcdc.h"

/*
 * Display Red, Green, Blue, and White in each quadrant of the screen. Enable the 
 *  graphic window to be displayed or not displayed, which will be a black rectangle
 */
void lcdc_color_demo()
{ 
  color_demo_data();     //Create the frame buffer data for background plane and graphic window
  color_demo_gw_setup(); //Initialize the graphic window registers for this demo
  
  // set LCD Screen Start Address
  LCDC_LSSAR = FRAME_START_ADDRESS;    
  
  // set LCD graphic window start address
  LCDC_LGWSAR = GW_START_ADDRESS;

  SIM_MCR|=SIM_MCR_LCDSTART_MASK;  //Start LCDC      
  
  printf("Hit any key to turn on/off the graphic window\n");
  while(1)
  {
    in_char(); //Wait for input
    LCDC_LGWCR&=~LCDC_LGWCR_GWE_MASK;    //Turn off graphic window
    in_char(); //Wait for input
    LCDC_LGWCR|=LCDC_LGWCR_GWE_MASK;     //Turn on graphic window
  }
}

/*
 *  Create a frame buffer with the data to display on the LCD
 */
void color_demo_data()
{
  int *pointer32;
  int i,n;  

  
  /* 
   * Create the frame buffer so that the screen will be divided into 4 quadrants 
   *  of a different color:
   *   RG
   *   BW
   */
  pointer32=(int *)FRAME_START_ADDRESS;
  
   //Loop through top half of screen
  for(i=0;i<SCREEN_YSIZE/2;i++)
  {
    //One half will be red
    for(n=0;n<SCREEN_XSIZE/2;n++) 
    {
      *pointer32=0x00FF0000; //Red
      pointer32++;      
    }
    
    //One half will be green
    for(n=0;n<SCREEN_XSIZE/2;n++)
    {
      *pointer32=0x0000FF00; //Green
      pointer32++;      
    }    
  }
  
   //Loop through bottom half of screen  
  for(i=0;i<SCREEN_YSIZE/2;i++)
  {
    //One half will be blue
    for(n=0;n<SCREEN_XSIZE/2;n++)
    {
      *pointer32=0x000000FF; //Blue
      pointer32++;      
    }
    
    //One half will be white
    for(n=0;n<SCREEN_XSIZE/2;n++)
    {
      *pointer32=0x00FFFFFF; //White
      pointer32++;      
    }    
  }  
  
  
  
  /* Create a graphic window filled with the color black*/
  pointer32=(int *)GW_START_ADDRESS;
  for(i=0;i<GW_BUFFER_SIZE/4;i++)
  {
    *pointer32=0x00000000; //Black
    pointer32++;
  }    
}

/*
 * Initialize the graphic window registers to create a graphic window in the
 *   middle of the screen
 */
void color_demo_gw_setup()
{
  // set LCD graphic window size 
  LCDC_LGWSR =
    LCDC_LGWSR_GWW(GW_XSIZE/16) |
    LCDC_LGWSR_GWH(GW_YSIZE);

  //  set LCD graphic window virtual page width
  LCDC_LGWVPWR = GW_XSIZE;

  //  set LCD graphic window panning offset (not used in 24bpp mode)
  LCDC_LGWPOR = 0;

  //  set LCD graphic window position so that the GW will cover up the middle of screen
  LCDC_LGWPR =
    LCDC_LGWPR_GWXP(SCREEN_XSIZE/4) |
    LCDC_LGWPR_GWYP(SCREEN_YSIZE/4);

  //  set LCD graphic window control
  LCDC_LGWCR =
    LCDC_LGWCR_GWAV(0xff)   | // alpha-transparent value (0xFF is opaque)
    LCDC_LGWCR_GWE_MASK;
}
