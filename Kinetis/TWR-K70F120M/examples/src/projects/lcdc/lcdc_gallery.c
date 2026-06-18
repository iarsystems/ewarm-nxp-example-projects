/*
 * File:		lcdc_color.c
 * Purpose:		LCD Color Demo
 *                      Display RGB colors to the screen
 *
 */

#include "lcdc.h"

//Image files
#include "fsl_logo.h"
#include "rome.h"
#include "prague.h"


/*
 * Display different images on the LCD screen. Hit any key to scroll through
 *  the images.
 *
 * The LSSAR register should point to the beginning of the last horizontal line
 *   of the image. 
 * The compiler will provide the location of the start of the image. Then the 
 *  end of the frame buffer for that image should be found: 
 *  (fsl_logo+FRAME_BUFFER_SIZE)
 * Then find the beginning of the last horizontal line. This is calculated by 
 *   subtracting the number horizontal pixels times the bytes per pixel from 
 *   that previously calculated value (-(SCREEN_XSIZE*4) for 24bpp). 
 * Finally because the compiler puts all "code" at 0x08xx_xxxx. However 
 *   the LCDC can't read from that address range, so the FRAME_DDR_OFFSET value 
 *   must be added to point to the alias at 0x80xx_xxxx. This final value is
 *   the put into LSSAR.
 */
void lcdc_gallery_demo()
{ 
  printf("Location fsl_logo: 0x%08X\n",(unsigned int)fsl_logo);
  printf("Location rome: 0x%08X\n",(unsigned int)rome);  
  printf("Location prague: 0x%08X\n",(unsigned int)prague);  
  
  
  //Since displaying an BMP image, set vertical reverse scan bit
  LCDC_LPCR |= LCDC_LPCR_REV_VS_MASK;

  SIM_MCR|=SIM_MCR_LCDSTART_MASK;  //Start LCDC      
  
  while(1)
  {
    //See top of file for explanation of how to set LSSAR address
    LCDC_LSSAR = (unsigned int)fsl_logo+FRAME_BUFFER_SIZE-(SCREEN_XSIZE*4)+FRAME_DDR_OFFSET;   
    in_char();
    
    LCDC_LSSAR = (unsigned int)rome+FRAME_BUFFER_SIZE-(SCREEN_XSIZE*4)+FRAME_DDR_OFFSET;   
    in_char();    

    LCDC_LSSAR = (unsigned int)prague+FRAME_BUFFER_SIZE-(SCREEN_XSIZE*4)+FRAME_DDR_OFFSET;   
    in_char();       
  }
}
