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
 * to develop code for the IAR-LPC-2478 board. It shows basic use of I/O,
 * timer, interrupt and LCD controllers and USB Host Masstorage class framework.

 *  This example demonstrates how to access the files on "USB Flash drive"
 * connected to USB Host port on IAR-LPC-2478 board.
 *
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
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)

#define  FILENAME_R       "MSREAD.TXT"
#define  FILENAME_W       "MSWRITE.TXT"

#define  MAX_BUFFER_SIZE  (4000)
#define  WRITE_SIZE       (10 * 1000000)

#define DLY_100US          500

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
        PRINT_Log("Reading from %s...\n\r", FILENAME_R);
        do {
            bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        } while (bytes_read);

        FILE_Close(fdr);
        PRINT_Log("Read Complete\n\r");
    } else {
        PRINT_Log("Could not open file %s\n\r", FILENAME_R);
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
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) {
            tot_bytes_written = 0;
            PRINT_Log("Writing to %s...\n\r", FILENAME_W);
            do {
                bytes_written = FILE_Write(fdw, UserBuffer, MAX_BUFFER_SIZE);
                tot_bytes_written += bytes_written;
            } while (tot_bytes_written < WRITE_SIZE);
            FILE_Close(fdw);
            PRINT_Log("Write completed\n\r");
        } else {
            PRINT_Log("Could not open file %s\n\r", FILENAME_W);
            return;
        }
        FILE_Close(fdr);

    } else {
        PRINT_Log("Could not open file %s\n\r", FILENAME_R);
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
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) {
            PRINT_Log("Copying from %s to %s...\n\r", FILENAME_R, FILENAME_W);
            do {
                bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
                FILE_Write(fdw, UserBuffer, bytes_read);
            } while (bytes_read);
            FILE_Close(fdw);
        } else {
            PRINT_Log("Could not open file %s\n\r", FILENAME_W);
            return;
        }
        FILE_Close(fdr);
        PRINT_Log("Copy completed\n\r");
    } else {
        PRINT_Log("Could not open file %s\n\r", FILENAME_R);
        return;
    }
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
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
  // Flash accelerator init
  FLASHCFG = (0x5UL<<12) | 0x3AUL;
  // Init clock
  InitClock();
#if FCCLK_FREQ < 20000000
  FLASHCFG = (0x0UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 40000000
  FLASHCFG = (0x1UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 60000000
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 80000000
  FLASHCFG = (0x3UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 100000000
  FLASHCFG = (0x4UL<<12) | 0x3AUL;
#endif

  // Init GPIO
  GpioInit();
  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);

  GLCD_Backlight(BACKLIGHT_ON);

  GLCD_SetFont(&Terminal_6_8_6,0x000F00,0x00FF0);
  GLCD_SetWindow(5,110,131,131);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  __enable_interrupt();

  // Init IO cache
  IOC_Init();

  // Initialize the lpc2468 host controller
  Host_Init();

  if(rc != OK)
  {
    rc = Host_EnumDev();  /* Enumerate the device connected                     */
    if (rc == OK)
    {
      rc = MS_Init();     /* Initialize the mass storage and scsi interfaces    */
      if (rc == OK)
      {
        rc = FAT_Init();  /* Initialize the FAT16 file system                   */
        if (rc == OK)
        {
          Main_Copy();    /* Call the application                               */
          // Flush IO cache
          IOC_Flush();
        }
      }
    }
  }
}
