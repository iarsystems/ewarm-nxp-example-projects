/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 27, November 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC2478-SK board. It shows basic use of I/O,
 * timer, interrupt, LCD controllers and USB Host Mass storage class framework.
 *
 *  This example demonstrates how to access the files on "USB Flash drive"
 * connected to USB Host port on IAR-LPC-2478 board.
 *
 * Jumpers:
 *  EXT/JLINK  - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *  BDS_E      - unfilled
 *  C/SC       - SC
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LCD shares pins with Trace port. If ETM is enabled the LCD will not work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#ifndef SDRAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)

#define  FILENAME_R       "MSREAD.TXT"
#define  FILENAME_W       "MSWRITE.TXT"

#define  MAX_BUFFER_SIZE  (4000)
#define  WRITE_SIZE       (10 * 1000000)

extern Int32U SDRAM_BASE_ADDR;
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/*
**************************************************************************************************************
*                                      READ DATA FROM DISK
*
* Description: This function is used by the user to read data from the disk
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Read (void)
{
    USB_INT32U  fdr;
    USB_INT32U  bytes_read;


    fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        GLCD_print("Reading from %s...\n\r", FILENAME_R);
        do {
            bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        } while (bytes_read);

        FILE_Close(fdr);
        GLCD_print("Read Complete\n\r");
    } else {
        GLCD_print("Could not open file %s\n\r", FILENAME_R);
        return;
    }
}

/*
**************************************************************************************************************
*                                      WRITE DATA TO DISK
*
* Description: This function is used by the user to write data to disk
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Write (void)
{
    USB_INT32U  fdw;
    USB_INT32U  fdr;
    USB_INT32U  tot_bytes_written;
    USB_INT32U  bytes_written;


    fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        fdw = FILE_Open(FILENAME_W, WRITE);
        if (fdw > 0) {
            tot_bytes_written = 0;
            GLCD_print("Writing to %s...\n\r", FILENAME_W);
            do {
                bytes_written = FILE_Write(fdw, UserBuffer, MAX_BUFFER_SIZE);
                tot_bytes_written += bytes_written;
            } while (tot_bytes_written < WRITE_SIZE);
            FILE_Close(fdw);
            GLCD_print("Write completed\n\r");
        } else {
            GLCD_print("Could not open file %s\n\r", FILENAME_W);
            return;
        }
        FILE_Close(fdr);

    } else {
        GLCD_print("Could not open file %s\n\r", FILENAME_R);
        return;
    }
}

/*
**************************************************************************************************************
*                                    COPYING A FILE
*
* Description: This function is used by the user to copy a file
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Copy (void)
{
    USB_INT32S  rs;
    USB_INT08U  attr;

    USB_INT32U  fdr;
    USB_INT32U  fdw;
    USB_INT32U  bytes_read;



    fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        fdw = FILE_Open(FILENAME_W, WRITE);
        if (fdw > 0) {
            GLCD_print("Copying from %s to %s...\n\r", FILENAME_R, FILENAME_W);
            do {
                bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
                FILE_Write(fdw, UserBuffer, bytes_read);
            } while (bytes_read);
            FILE_Close(fdw);
        } else {
            FILE_Close(fdr);
            GLCD_print("Could not open file %s\n\r", FILENAME_W);
            return;
        }
        FILE_Close(fdr);
        GLCD_print("Copy completed\n\r");
    } else {
        GLCD_print("Could not open file %s\n\r", FILENAME_R);
        return;
    }
}

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
Int32S rc = ERR_TD_FAIL;

  GLCD_Ctrl (FALSE);
  // Init GPIO
  GpioInit();
#ifndef SDRAM_DEBUG
  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
  // Init clock
  InitClock();
  // SDRAM Init
  SDRAM_Init();
#endif // SDRAM_DEBUG
  // Init VIC
  VIC_Init();
  // GLCD init
  GLCD_Init (IarLogoPic.pPicStream, NULL);
  GLCD_Cursor_Dis(0);

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetFont(&Terminal_6_8_6,0x0000FF,0x000cd4ff);

  GLCD_SetWindow(10,180,320-1,240-1);
  GLCD_TextSetPos(0,0);

  GLCD_Ctrl (TRUE);
  __enable_interrupt();

  // Init IO cache
  IOC_Init();

  // Initialize the lpc2468 host controller
  GLCD_print("Initializing Host Stack\n\r");
  Host_Init();
  GLCD_print("Host Initialized\n\r");

  if(rc != OK)
  {
    GLCD_print("Connect a Mass Storage device\n\r");
    rc = Host_EnumDev();  /* Enumerate the device connected                     */
    if (rc == OK)
    {
      rc = MS_Init();     /* Initialize the mass storage and scsi interfaces    */
      if (rc == OK)
      {
        GLCD_print("Mass Storage device connected\n\r");
        rc = FAT_Init();  /* Initialize the FAT16 file system                   */
        if (rc == OK)
        {
          Main_Copy();    /* Call the application                               */
          // Flush IO cache
          IOC_Flush();
        }
      }
      else
      {
        GLCD_print("Not a Mass Storage device\n\r");
      }
    }
  }
}
