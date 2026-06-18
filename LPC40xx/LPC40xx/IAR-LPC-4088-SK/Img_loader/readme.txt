########################################################################
#
#                           img_loader
#
# $Revision: 8209 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC-4088-SK board. 
 
  The example is intended to load the bmp images used by the other 
 IAR-LPC4088-SK demos into the SPI flash memory. You must program the SPI flash
 only the first time you run a IAR-LPC4088-SK example or if you have erased or
 programmed the SPI memory with your own data.
 
 The SPIFIIMG.BIN file was created using the command prompt "copy" command:
 "$PROJ_DIR$\bin>copy /b iar_logo_480x272.bmp+arena_480x272.bmp
  +squirrel_480x272.bmp+model_480x272.bmp+strawberry_480x272.bmp+Next_76x27.bmp
  +Prev_76x27.bmp+Loud_52x52.bmp+wide_stereo_52x52.bmp SPIFIIMG.BIN"

COMPATIBILITY
=============

   The example project is compatible with IAR-LPC-4088-SK board. 
  By default, the project is configured to use the I-Jet debugger.

CONFIGURATION
=============
  Download  - The startup code and a main function won't be loaded in memory.
              To program the SPI flash memory use "Project > Download > Download active application"
              or the "Download and Debug" IDE GUI button, but do not press the "Go" button.
              In the "Download and Debug" case the SPIFIMAP_LPC408x.mac file for verification
              of the SPIFI contents will be used.
 Jumpers:
   PWR_SEL    - depending of power source
   ISP_E      - unfilled
   RST_E      - unfilled
    
GETTING STARTED
===============

  1) Open the img_loader project.
  
  2) Power the board and connect the debugger probe.

  3) Use the "Make" button or press F7 to build it.
     
  4) To program the images select "Project > Download > Download active application"
     A warning will appear. Ignore it by clicking the O.K. button. The flash
     loader will write the images to the SPI memory.
