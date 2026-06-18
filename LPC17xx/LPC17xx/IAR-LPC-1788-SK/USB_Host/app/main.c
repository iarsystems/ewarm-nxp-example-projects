/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : IAR-LPC1788-SK USB Host example
 *
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-1788-SK board.  It shows basic use of I/O,
 * timer, interrupt, LCD controllers and USB Host Mass storage class framework.
 *
 *  This example demonstrates how to access files on "USB Flash drive"
 * connected to USB Host port on IAR-LPC-1788-SK board.
 *
 * Jumpers:
 *  PWR_SEL    - depending on the power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 1543 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define  FILENAME_R       "MSREAD.TXT"
#define  FILENAME_W       "MSWRITE.TXT"

#define  MAX_BUFFER_SIZE  (4000)
#define  WRITE_SIZE       (10 * 1000000)

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/** external data **/
#pragma section=".intvec"
/** public functions **/
/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL0 and clocks' dividers. PLL = 96MHz,
 *               CPU - 96MHz, PCLK - 48 MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /*Sys Clock Select as CPU clock
    divider 1:1*/
  CLK_SetCpuClk(CPUSEL_CLKSYS,1);
  /*Select IRC oscilator as Sys clock*/
  CLK_SetSysClk(SYSSEL_IRCOSC);
  /*Enable Main oscilator*/
  CLK_MainOscSet(CLK_ENABLE,MOSCRNG_1_20MHZ);
  /*Select Main oscilator as Sys clock*/
  CLK_SetSysClk(SYSSEL_MOSC);
  /*Enable set PLL 96Hz (M = 8; P = 1)*/
  CLK_SetMainPll(CLK_ENABLE, 8-1, 0);
  /*Set peripheral divider 1:2. Peripheral clock 48MHz*/
  CLK_SetPeriphClk(2);
  /*PLL0 out is used as the input to the CPU clock divider
  divider 1:1. CPU runs at 96MHz*/
  CLK_SetCpuClk(CPUSEL_CLKPLL,1);
}

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: void
 * Return:
 *
 * Description: Low level system init (clock, flash accelerator,
 *              SDRAM and vector table address)
 *
 *
 *************************************************************************/
int __low_level_init (void)
{
  /*if debug is in SDRAM
    clock and SDRAM initialization is made
    in mac file*/
#ifndef SDRAM_DEBUG
  int cpuclk;
  /* Flash accelerator safe value*/
  FLASHCFG = (5UL<<12) | 0x3AUL;
  /*Clock Init*/
  InitClock();
  /*Flash accelerator init*/
  cpuclk = CLK_GetClock(CLK_CPU);

  if( cpuclk < 20000000 ){
    FLASHCFG = (0x0UL<<12) | 0x3AUL;
  } else if( cpuclk < 40000000 ){
    FLASHCFG = (0x1UL<<12) | 0x3AUL;
  } else if( cpuclk< 60000000 ){
    FLASHCFG = (0x2UL<<12) | 0x3AUL;
  } else if( cpuclk < 80000000 ){
    FLASHCFG = (0x3UL<<12) | 0x3AUL;
  } else if( cpuclk < 100000000 ){
    FLASHCFG = (0x4UL<<12) | 0x3AUL;
  }
  /*SDRAM init*/
  SDRAM_Init();
#endif
  /*Set vector table location*/
  VTOR  = (unsigned int)__segment_begin(".intvec");

  return  1;
}

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
        fdw = FILE_Open(FILENAME_W, RDWR);
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
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) {
            GLCD_print("Copying from %s to %s...\n\r", FILENAME_R, FILENAME_W);
            do {
                bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
                FILE_Write(fdw, UserBuffer, bytes_read);
            } while (bytes_read);
            FILE_Close(fdw);
        } else {
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


  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Init LCD and copy picture in video RAM*/
  GLCD_Init (IarLogoPic.pPicStream, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);

  /*Enable interrupts*/
  __enable_interrupt();

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetFont(&Terminal_6_8_6,0x0000FF,0x000cd4ff);

  GLCD_SetWindow(10,180,320-1,240-1);
  GLCD_TextSetPos(0,0);


  /* Init IO cache */
  IOC_Init();

  /* Initialize the lpc178x host controller */
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
          /* Flush IO cache */
          IOC_Flush();
        }
      }
      else
      {
        GLCD_print("Not a Mass Storage device\n\r");
      }
    }
  }

  return 0;
}
/** private functions **/
