/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : USB_Host.c
 *    Description : IAR-LPC4088-SK USB_Host Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board.  It shows basic use of the USB Host
 * controller.
 * 
 *  This example implements USB Host Mass storage class framework and demonstrates
 * how to access files on "USB Flash drive" connected to USB Host port on
 * IAR-LPC-4088-SK board.
 *
 * Jumpers:
 *  PWR_SEL    - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 * 
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <yfuns.h>
#include <stdint.h>

#include "LPC407x_8x.h"
#include "board.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"
#include "spifi_rom_api.h"

#include "usbhost_fat.h"
#include "usbhost_lpc408x.h"
#include "io_cache.h"

#define  FILENAME_R       "MSREAD.TXT"
#define  FILENAME_W       "MSWRITE.TXT"

#define  MAX_BUFFER_SIZE  (4000)
#define  WRITE_SIZE       (10 * 1000000)


extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/** public data **/
volatile uint32_t CriticalSecCntr;
SPIFIobj obj;
/** external data **/
#pragma section=".intvec"
/** public functions **/
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
  /*Here the clock is already set by
    SystemInit function*/
  
  /*SDRAM init*/
  SDRAM_Init();

  /*SPIFI initialization*/
  LPC_SC->PCONP |= (1<<16);     /* enable SPIFI clock */
  LPC_SC->PCONP |= (1<<15);     /* enable IOCON clock */

  /*SPIFI Configure Pins*/
  SPIFI_IO0_IOCON = 0x35;
  SPIFI_IO1_IOCON = 0x35;
  SPIFI_IO2_IOCON = 0x35;
  SPIFI_IO3_IOCON = 0x35;
  SPIFI_CLK_IOCON = 0x35;
  SPIFI_CS_IOCON = 0x35;
	/* Initialize SPIFI driver */
	spifi_init(&obj, 7, S_RCVCLK | S_FULLCLK, SPIFIClock/1000000);
  
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
        GLCD_print("Reading from %s...\r\n", FILENAME_R);
        do {
            bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        } while (bytes_read);

        FILE_Close(fdr);
        GLCD_print("Read Complete\r\n");
    } else {
        GLCD_print("Could not open file %s\r\n", FILENAME_R);
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
            GLCD_print("Writing to %s...\r\n", FILENAME_W);
            do {
                bytes_written = FILE_Write(fdw, UserBuffer, MAX_BUFFER_SIZE);
                tot_bytes_written += bytes_written;
            } while (tot_bytes_written < WRITE_SIZE);
            FILE_Close(fdw);
            GLCD_print("Write completed\r\n");
        } else {
            GLCD_print("Could not open file %s\r\n", FILENAME_W);
            return;
        }
        FILE_Close(fdr);

    } else {
        GLCD_print("Could not open file %s\r\n", FILENAME_R);
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
            GLCD_print("Copying from %s to %s...\r\n", FILENAME_R, FILENAME_W);
            do {
                bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
                FILE_Write(fdw, UserBuffer, bytes_read);
            } while (bytes_read);
            FILE_Close(fdw);
        } else {
            GLCD_print("Could not open file %s\r\n", FILENAME_W);
            return;
        }
        FILE_Close(fdr);
        GLCD_print("Copy completed\r\n");
    } else {
        GLCD_print("Could not open file %s\r\n", FILENAME_R);
        return;
    }
}

#define BMP_IMG_ADDR  0x28000000
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
USB_INT32S rc = ERR_TD_FAIL;
pPic_t pPic;
  /*Disable interrupts*/
  __disable_interrupt();
  /*Init LCD Controller*/
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Load Picture from bmp file to SDRAM*/
  pPic = GLCD_LoadPic((bmp_t *)BMP_IMG_ADDR);
  /*Init LCD and copy picture ot VRAM*/
  GLCD_Init (pPic, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Enable interrupts*/
  __enable_interrupt();

  
  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x0000CCFF);
  GLCD_SetWindow(162,124,318,148);
  GLCD_TextSetPos(0,0);
  GLCD_print("USB HOST DEMO");

  GLCD_SetFont(&Terminal_9_12_6,0x0000FF,0x0000CCFF);
  GLCD_SetWindow(115,173,365,243);


  /* Init IO cache */
  IOC_Init();

  /* Initialize the lpc408x host controller */
  GLCD_TextSetPos(0,0);
  GLCD_print("Initializing Host Stack\r\n");
  Host_Init();

  GLCD_print("Host Initialized\r\n");

  if(rc != OK)
  {

    GLCD_print("Connect a Mass Storage device\r\n");
    rc = Host_EnumDev();  /* Enumerate the device connected                     */
    if (rc == OK)
    {
      rc = MS_Init();     /* Initialize the mass storage and scsi interfaces    */
      if (rc == OK)
      {
        GLCD_print("Mass Storage device connected\r\n");
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
        GLCD_print("Not a Mass Storage device\r\n");
      }
    }
  }
  
  return 0;
}
/** private functions **/
