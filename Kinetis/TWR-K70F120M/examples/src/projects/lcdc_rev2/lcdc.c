/*
 * File:		lcdc.c
 * Purpose:		LCD Controller Example
 *
 * This is an example of using the LCD controller (LCDC) to drive the
 *   TWR-LCD-RGB display.
 *
 * Included in this project is code to do the Background Look Up Table (BGLUT)
 *  and Graphic Window Look up Table (GW). These are not used in the 24bpp mode
 *  that the TWR-LCD-RGB uses, but are for example purposes only.
 *
 */
#include "common.h"
#include "lcdc.h"
#include "spi.h"
#include "lptmr.h"
#include "sdram.h"

/*
 * If running out of SDRAM, program in an image
 *   Else you will get a linker compile error as the image will not fit in Flash
 */
#ifdef SDRAM
  #include "fsl.h"
#endif

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

  /* Initialize TWR-LCD-RGB Screen over SPI */
 // lcdc_init_spi_lcd();  //42WQW3T states that no SPI config is needed

  /* Setup LCD pin muxing */
  lcdc_init_pins();

  /* LCDC configuration for TWR-LCD-RGB */
  lcdc_init_screen();

  /* Demo displaying colors and the graphic window */
  lcdc_color_demo();

#ifdef SDRAM
  /* Demo displaying FSL logo with fade in and panning */
  lcdc_fsl_demo();

  /* Demo displaying a gallery of photos */
  lcdc_gallery_demo();
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
  PORTB_PCR4=ALT2;  // Graphic LCD CONTRAST,  Schematic PTB4
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
    //LCDC_LPCR_SWAP_SEL_MASK |       //Set so that the LCD data lines match up correctly with the TWR-LCD-RGB
    LCDC_LPCR_SCLKIDLE_MASK |       //Enalbe LSCLK when vsync is idle
    LCDC_LPCR_SCLKSEL_MASK  |       //Always enable clock
    LCDC_LPCR_PCD(11);             //Divide 120 PLL0 clock (default clock) by (11+1)=12 to get 10MHz clock

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

/*
 * SPI configuration to initialize the LCD
 */
void lcdc_init_spi_lcd()
{
  spi_init();

  time_delay_ms(50);

  SpiRegSet(0x20);

  SpiRegSet(0x36);
  SpiSendDataWord(0x00);

  SpiRegSet(0x3A);
  SpiSendDataWord(0x70);

  printf("Value of reg before write at 0xB1 is 0x%04X\n",SpiReadDataWord(0xB1));
  SpiRegSet(0xB1);
  SpiSendDataWord(0x06);
  SpiSendDataWord(0x43);
  SpiSendDataWord(0x0A);
  printf("Value of reg after write at 0xB1 should be 0x0643. It is: 0x%04X\n",SpiReadDataWord(0xB1));

  SpiRegSet(0xB2);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0xC8);

  SpiRegSet(0xB3);
  SpiSendDataWord(0x00);

  SpiRegSet(0xB4);
  SpiSendDataWord(0x04);

  SpiRegSet(0xB5);
  SpiSendDataWord(0x42);
  SpiSendDataWord(0x10);
  SpiSendDataWord(0x10);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);

  SpiRegSet(0xB6);
  SpiSendDataWord(0x0B);
  SpiSendDataWord(0x18);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x40);
  SpiSendDataWord(0x10);
  SpiSendDataWord(0x30);

  SpiRegSet(0xC3);
  SpiSendDataWord(0x07);
  SpiSendDataWord(0x04);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x04);

  time_delay_ms(500);

  SpiRegSet(0xC4);
  SpiSendDataWord(0x12);
  SpiSendDataWord(0x24);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x49);

  time_delay_ms(500);

  SpiRegSet(0xC5);
  SpiSendDataWord(0x5B);

  time_delay_ms(500);

  SpiRegSet(0xC6);
  SpiSendDataWord(0x44);
  SpiSendDataWord(0x63);

  time_delay_ms(500);

  SpiRegSet(0xF9);
  SpiSendDataWord(0x40);

  SpiRegSet(0xD0);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD1);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD2);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD3);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD4);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0xD5);
  SpiSendDataWord(0x01);
  SpiSendDataWord(0x17);
  SpiSendDataWord(0x73);
  SpiSendDataWord(0x02);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x00);
  SpiSendDataWord(0x02);

  SpiRegSet(0x11);

  time_delay_ms(120);

  SpiRegSet(0x29);

  printf("Finished LCD SPI Init\n");
}
