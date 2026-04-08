/*
 * File:		lcdc.c
 * Purpose:		LCD Controller Example
 *
 * This is an example of using the LCD controller (LCDC) to drive the
 *   TWR-LCD-RGB display.
 *
 * If running the fsl_demo or gallery_demo, you must use the DDR2 target.
 *   Else you will get a linker compile error as the image will not fit in Flash
 *
 * If using the DDR2 target, make sure either the lcdc_demo.c file is excluded
 *  from the project (right click on file->Options->Exclude from Build)or the
 *  lcdc_gallery.c file is excluded. This is to save code size and download
 *  time since they are two completely seperate demos.
 */
#include "common.h"
#include "lcdc.h"
#include "lptmr.h"
#include "sdram.h"

/********************************************************************/
void main (void)
{
  printf("LCD Controller Example\n");

  //Enable Port clocks
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

  /* Enable LCDC and DDR Modules */
  SIM_SCGC3 |= SIM_SCGC3_LCDC_MASK;


#ifndef SDRAM //If running out of Flash or SRAM
  //Initialize DDR2 on Tower Board
  twr_ddr2_script_init();
#endif

  /* Disable MPU */
  MPU_CESR &= ~MPU_CESR_VLD_MASK;

  /* Setup LCD pin muxing */
  lcdc_init_pins();

  /* LCDC configuration for TWR-LCD-RGB */
  lcdc_init_screen();

//If running out of Flash, do a simple demo
#ifndef SDRAM
  /* Demo displaying colors and the graphic window */
  lcdc_color_demo();
#endif

//If running out of SDRAM, then do a more complex demo
#ifdef SDRAM
  /* Demo displaying FSL logo with fade in and panning */
  lcdc_fsl_demo();  //See instructions at top of file

  /* Demo displaying a gallery of photos */
  //lcdc_gallery_demo(); //See instructions at top of file
#endif
}


void lcdc_init_pins()
{
  #define ALT2 			(0|PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK)
  #define ALT5 			(0|PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK)
  #define ALT7 			(0|PORT_PCR_MUX(7)|PORT_PCR_DSE_MASK)


  PORTF_PCR4 =ALT7; // Graphic LCD D[0],  Schematic PTF4
  PORTF_PCR5 =ALT7; // Graphic LCD D[1],  Schematic PTF5
  PORTF_PCR6 =ALT7; // Graphic LCD D[2],  Schematic PTF6
  PORTF_PCR7 =ALT7; // Graphic LCD D[3],  Schematic PTF7
  PORTF_PCR8 =ALT7; // Graphic LCD D[4],  Schematic PTF8
  PORTF_PCR9 =ALT7; // Graphic LCD D[5],  Schematic PTF9
  PORTF_PCR10=ALT7; // Graphic LCD D[6],  Schematic PTF10
  PORTF_PCR11=ALT7; // Graphic LCD D[7],  Schematic PTF11
  PORTF_PCR12=ALT7; // Graphic LCD D[8],  Schematic PTF12
  PORTF_PCR13=ALT7; // Graphic LCD D[9],  Schematic PTF13
  PORTF_PCR14=ALT7; // Graphic LCD D[10], Schematic PTF14
  PORTF_PCR15=ALT7; // Graphic LCD D[11], Schematic PTF15

  PORTF_PCR16=ALT5; // Graphic LCD D[12], Schematic PTF16
  PORTF_PCR17=ALT5; // Graphic LCD D[13], Schematic PTF17
  PORTF_PCR18=ALT5; // Graphic LCD D[14], Schematic PTF18
  PORTF_PCR19=ALT5; // Graphic LCD D[15], Schematic PTF19
  PORTF_PCR20=ALT5; // Graphic LCD D[16], Schematic PTF20

  PORTF_PCR21=ALT7; // Graphic LCD D[17], Schematic PTF21
  PORTF_PCR22=ALT7; // Graphic LCD D[18], Schematic PTF22
  PORTF_PCR23=ALT7; // Graphic LCD D[19], Schematic PTF23
  PORTF_PCR24=ALT7; // Graphic LCD D[20], Schematic PTF24
  PORTF_PCR25=ALT7; // Graphic LCD D[21], Schematic PTF25
  PORTF_PCR26=ALT7; // Graphic LCD D[22], Schematic PTF26
  PORTF_PCR27=ALT7; // Graphic LCD D[23], Schematic PTF27

  PORTF_PCR0=ALT7;  // Graphic LCD PCLK,  Schematic PTF0
  PORTF_PCR1=ALT7;  // Graphic LCD DE,    Schematic PTF1
  PORTF_PCR2=ALT7;  // Graphic LCD HSYNC, Schematic PTF2
  PORTF_PCR3=ALT7;  // Graphic LCD VSYNC, Schematic PTF3


  // set LCD_CONTRAST
  //PORTB_PCR4=ALT2;  // Graphic LCD CONTRAST,  Schematic PTB4
}

/********************************************************************/

/*
 * Set up the LCDC for the LCD screen on the TWR-LCD-RGB
 */
void lcdc_init_screen()
{
  // set LCD Screen Start Address
  LCDC_LSSAR = FRAME_START_ADDRESS;

  // set LCD Size. The XMAX bitfield is the screen x-size/16.
  LCDC_LSR = LCDC_LSR_XMAX( (SCREEN_XSIZE / 16) ) | LCDC_LSR_YMAX( SCREEN_YSIZE );

  // set LCD virtual page width
  LCDC_LVPWR = LCDC_LVPWR_VPW( SCREEN_XSIZE );

  // set LCD cursor positon & settings (turn off)
  LCDC_LCPR = 0;
  LCDC_LCWHB = 0;

  // set LCD panel configuration. Use endianess to work with TWR-LCD-RGB lines.
  LCDC_LPCR =
#if LSCLK_POSITIVE
    LCDC_LPCR_CLKPOL_MASK   |
#endif
    LCDC_LPCR_TFT_MASK      |       //TFT Screen
    LCDC_LPCR_COLOR_MASK    |       //Color
    LCDC_LPCR_BPIX(0x7)     |       //24 bpp
    LCDC_LPCR_FLMPOL_MASK   |       //first line marker active low (VSYNC)
    LCDC_LPCR_LPPOL_MASK    |       //line pulse active low (HSYNC)
    LCDC_LPCR_END_SEL_MASK  |       //Use big-endian mode (0xFFAA5500 means R=AA,G=55,B=00).
    //LCDC_LPCR_SWAP_SEL_MASK |       //Set if needed for LCD data lines match up correctly with the LCD
    LCDC_LPCR_SCLKIDLE_MASK |       //Enalbe LSCLK when vsync is idle
    LCDC_LPCR_SCLKSEL_MASK  |       //Always enable clock
    LCDC_LPCR_PCD(11);              //Divide 120 PLL0 clock (default clock) by (11+1)=12 to get 10MHz clock

  // set LCD horizontal configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LHCR =
    LCDC_LHCR_H_WIDTH(40)   |    //(40+1)=41 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(1)  |    //(1+1)=2 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(0);     //(0+3)=3 SCLK periods between end of HSYNC and beginning of OE

  // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
  LCDC_LVCR =
    LCDC_LVCR_V_WIDTH(10)  |   //10 lines period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(2) |    //2 line period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(2);    //2 line periods between end of VSYNC and beginning of OE

  // set the LCD panning offset (not used in 24bpp mode)
  LCDC_LPOR = 0;

  // set LCD interrupt configuration register
  LCDC_LICR = 0;

  //Disable LCDC interrupts
  LCDC_LIER = 0;

  //Disable the graphic window. See the "color" and "fsl" demos for examples of
  //  using the graphic window feature
  LCDC_LGWCR &=~LCDC_LGWCR_GWE_MASK;

  //Set background plane DMA to burst mode
  LCDC_LDCR&=~LCDC_LDCR_BURST_MASK;

  //Set graphic window DMA to burst mode
  LCDC_LGWDCR&=~LCDC_LGWDCR_GWBT_MASK;
}
