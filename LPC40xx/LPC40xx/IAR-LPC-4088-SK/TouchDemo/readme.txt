########################################################################
#
#                           TouchDemo
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC-4088-SK board. It shows basic use of I/O,
 timer, interrupt, LCD controllers and LCD touch screen.
 
  A cursor is shown and moves when the screen is touched. You can change the
 pictures on the LCD by touching the "Next" and the "Prev" buttons.
  
COMPATIBILITY
=============

   The example project is compatible with IAR-LPC-4088-SK board. 
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.
   
 Jumpers:
   PWR_SEL    - depending of power source
   ISP_E      - unfilled
   RST_E      - unfilled
  
 Note:
    The LCD shares pins with Trace port. If ETM is enabled the LCD will not
    work.
  
GETTING STARTED
===============

  1) Open the TouchDemo work space.
  
  2) Power the board and connect the debugger probe.

  3) If you previously haven't run any of the IAR-LPC4088-SK examples
     you must program the SPI flash memory with the bmp images used by the
     demo projects. You should do this also if you have erased or programmed 
     the SPI flash with your own data.

     - Select img_loader project.
     
     - Use the "Make" button or press F7 to build it.
     
     - To program the images select "Project > Download > Download active application"
       A warning will appear. Ignore it by clicking the O.K. button. The flash
       loader will write the images to the SPI memory.
       
     - Switch back to the TouchDemo project.

  4) Build the TouchDemo example. Press CTRL+D or use Download and
     Debug button to start a debug session.
 
  5) Press F5 or use the GO button to run the example.