/*
 * File:		lcdc_gallery.c
 * Purpose:		LCD Color Demo
 *                      Display gallery of images to the screen
 *
 */

#include "lcdc.h"

//Image files
#include "images/fsl_logo.h"
#include "images/prague.h"
#include "images/rome.h"

/*
 * Display different images on the LCD screen. Hit any key to scroll through
 *  the images.
 */
void lcdc_gallery_demo()
{ 
  //printf("Location fsl: 0x%08X\n",(unsigned int)fsl_logo);
  //printf("Location rome: 0x%08X\n",(unsigned int)rome);  
  //printf("Location prague: 0x%08X\n",(unsigned int)prague);  
  
  
  //If displaying an BMP image, set vertical reverse scan bit
  LCDC_LPCR |= LCDC_LPCR_REV_VS_MASK;

  while(1)
  {
    LCDC_LSSAR = (unsigned int)fsl_logo+FRAME_BUFFER_SIZE+FRAME_DDR_OFFSET;   
    in_char();
    
    LCDC_LSSAR = (unsigned int)prague+FRAME_BUFFER_SIZE+FRAME_DDR_OFFSET;   
    in_char();    
          
  }
}
