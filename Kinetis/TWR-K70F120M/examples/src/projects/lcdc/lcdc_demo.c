/*
 * File:		lcdc_demo.c
 * Purpose:		LCD Demos
 *
 */

#include "lcdc.h"
#include "lptmr.h"

//Image
#include "fsl_logo.h"
#include "white.h"

/* Global Variables for lcdc_demo() function */
char change_flag;
int alpha_value;


/*
 * This demo fades in the FSL logo, and then scrolls it off to the right of the
 *   screen. The image must be placed at 0x8040_0000 to allow room for an extra
 *   frame buffer directly next to the image, which  enables the panning.
 *
 * Letting the compiler put the image where it may results in over-writing
 *   code/heap when creating the adjacent frame buffer.
 *
 * The demo works by putting a pure white graphic window over the image, and then
 *  adjusting the transparency to go from fully opaque to fully transparent. This
 *  occurs in the ISR function.
 *
 * Then the SSA register is adjust by (SCREEN_XSIZE*4)*num_of_lines_to_pan bytes
 *   to create the illusion of scrolling.
 */
void lcdc_fsl_demo()
{
  int *pointer32;
  int i;

  //Set maximum address
  unsigned int max_address=(unsigned int)fsl_logo+FRAME_BUFFER_SIZE+FRAME_BUFFER_SIZE+FRAME_DDR_OFFSET;

  //Set the start address for the background plane (which is vertical reverse mode)
  //  so the start address should point to the end of the image
  unsigned int bp_address=(unsigned int)fsl_logo+FRAME_BUFFER_SIZE+FRAME_DDR_OFFSET-(SCREEN_XSIZE*4);

  printf("LCDC Freescale Logo Demo\n");

  SIM_MCR&=~SIM_MCR_LCDSTART_MASK;  //Stop LCDC

  //Enable LCDC interrupt in NVIC
  enable_irq(97);

  //Configure interrupt to occur when sending out the last data to LCD
  LCDC_LICR |= LCDC_LICR_INTSYN_MASK;
  LCDC_LICR &= ~LCDC_LICR_INTCON_MASK;

 //Enable EOF interrupt
  LCDC_LIER=LCDC_LIER_EOF_EN_MASK;

  //Since displaying a BMP file, need to set the vertical reverse scan bit
  LCDC_LPCR |= LCDC_LPCR_REV_VS_MASK;

  //Point to SSA bits to the start of the image
  LCDC_LSSAR=bp_address;

  // set LCD graphic window start address to the second all white frame buffer
  // This buffer is directly behind the backplane buffer. The address for the bp
  // starts at the end of the image since doing a reverse vertical scan
  LCDC_LGWSAR = (unsigned int)white+FRAME_DDR_OFFSET;

  // set LCD graphic window size to be as big as the LCD display
  LCDC_LGWSR =
    LCDC_LGWSR_GWW(SCREEN_XSIZE/16) |
    LCDC_LGWSR_GWH(SCREEN_YSIZE);

  //  set LCD graphic window virtual page width
  LCDC_LGWVPWR = SCREEN_XSIZE;

  //  set LCD graphic window panning offset
  LCDC_LGWPOR = 0;

  //  set LCD graphic window position to start in the upper left corner
  LCDC_LGWPR =
    LCDC_LGWPR_GWXP(0) |
    LCDC_LGWPR_GWYP(0);

  //  set LCD graphic window control
  LCDC_LGWCR =
    LCDC_LGWCR_GWAV(0xFF)    | // alpha-transparent (0xFF means opaque)
    LCDC_LGWCR_GWE_MASK;       // enable

  SIM_MCR|=SIM_MCR_LCDSTART_MASK;  //Start LCDC

  while(1)
  {
    //Do the fade in by starting the graphic window as opaque, and gradually
    //  transition to completely transparent. See code in the ISR
    for(alpha_value=0xFF;alpha_value>0;alpha_value-=0x5)
    {
      change_flag=1;      //Set flag to signal the ISR to change the transparency
      time_delay_ms(100); //Wait 100ms between transparency changes
    }
    time_delay_ms(500);

    //Now scroll the image by changing the SSA address by the width of image
    while(bp_address<max_address)
    {
      time_delay_ms(10);               //Wait 10ms between shifts
      bp_address+=((SCREEN_XSIZE*4));  //Move over by one line
      LCDC_LSSAR=bp_address;
    }

    alpha_value = 0xFF;
    LCDC_LGWCR = LCDC_LGWCR_GWAV(0xFF)    | // alpha-transparent (0xFF means opaque)
                 LCDC_LGWCR_GWE_MASK;       // enable
    time_delay_ms(500);

    //Reset the variables
    bp_address=(unsigned int)fsl_logo+FRAME_BUFFER_SIZE+FRAME_DDR_OFFSET-(SCREEN_XSIZE*4);
    LCDC_LSSAR=bp_address;
  }
}

/*
 * LCDC ISR runs after LCDC finishes sending last data to the LCD. This prevents
 *  flickering while the LCDC is disabled
 */
void lcdc_demo_isr()
{
  int status=LCDC_LISR; //Clear the LISR register by reading it

  //If flag is set to 1, then change the transparency of the graphic window
  if(change_flag==1)
  {
    LCDC_LGWCR =LCDC_LGWCR_GWAV(alpha_value) | LCDC_LGWCR_GWE_MASK; //Adjust transparency value
    change_flag=0;  //Set flag to 0
  }

  return;
}
