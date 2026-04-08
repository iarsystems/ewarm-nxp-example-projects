/***********************************************************************
 * $Id: ea3250_nand_boot.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: Program the NAND flash with two applications.
 *
 * Description:
 *    First application written to block 0. This block can directly
 *    be used by the LPC3250 bootloader to load an application. 
 *    This application should typically be the kickstart 
 *    bootloader that can then be used to load a larger 
 *    application from block 1 and onwards.
 *
 *    The second application may occupy several blocks starting
 *    from block 1. This application could for example be a more
 *    advanced bootloader such as the u-boot.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/


#include <string.h>
#include <stdio.h>

#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_arm922t_cp15_driver.h"
#include "ea3250_board.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_uart_driver.h"
#include "lpc32xx_uart_driver.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc_string.h"

#include "lpc_nandflash_params.h"
#include "lpc32xx_clkpwr_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_dma_driver.h"
#include "lpc32xx_slcnand_driver.h"
#include "lpc32xx_mlcnand_driver.h"

/* Prototype for external IRQ handler */
void lpc32xx_irq_handler(void);


/* Device handler */
static INT_32 mlcnanddev;

/* UART buffer */
static char buff[512];

static const NANDFLASH_PARAM_T k9f1g08u0a_9 =
{
  BUS_WIDTH_8,                   /* Bus width */
  BLOCK_PAGE_LARGE,              /* Block page (small/large) */
  ADDR_CYCLES_4,                 /* Address cycles (3/4/5) */
};

/* NAND flash */
#define NAND_FLASH k9f1g08u0a_9

/* for LARGE page NAND the maximum size of boot app is 54K */
#define NAND_MAX_BOOT_SIZE_LARGE (54*1024)

/* 
 * For the K9F1G08U0A the number of blocks is 1024. 
 * 1024 * 128 K = 1Gbit (128 MB)
 */
#define NAND_NUMBER_BLOCKS 1024

/* good block marker */
#define NAND_GOOD_BLOCK_MARKER 0xff
/* bad block marker */
#define NAND_BAD_BLOCK_MARKER 0x77

/* number of pages per block */
#define NAND_PAGES_PER_BLOCK 64

/* the size of one block in bytes (the data portion of the block */
#define NAND_BLOCK_SIZE (LARGE_BLOCK_PAGE_MAIN_AREA_SIZE*NAND_PAGES_PER_BLOCK)

/* offset to bad block info*/
#define NAND_SAMSUNG_BAD_OFFSET 2048
#define NAND_MLC_BAD_OFFSET     2096

/* maximum amount of internal memory */
#define MCU_MAX_INT_MEMORY (256*1024)

/* maximum size of application 2 */
#define MAX_APP2_SIZE (MCU_MAX_INT_MEMORY-NAND_MAX_BOOT_SIZE_LARGE)

/* Sector buffer - data area plus spare area */
static UNS_16 f16page [LARGE_BLOCK_PAGE_SIZE / 2];
static UNS_8 app2Magic[] = {0xea, 0x34, 0xcb, 0x15};




/***********************************************************************
 *
 * Function: deadloop
 *
 * Purpose: Endless loop
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void deadloop(void)
{
//	uart_output("Program halted...\r\n");
	while (1);
}



/***********************************************************************
 *
 * Function: nand_write_pages
 *
 * Purpose: Write several pages in a block
 *
 * Parameters: 
 *    block:    block number
 *    page:     starting page number
 *    pBuffer:  data to write
 *    ptw:      number of pages to write
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS nand_write_pages(UNS_32 block, UNS_32 page, 
  UNS_8* pBuffer, UNS_32 ptw)
{
  MLC_BLOCKPAGE_T blockpage;
  UNS_8 *pagedat = (UNS_8 *) f16page;

  /* Store application in block 0, starting from page */ 
  blockpage.dma = 0;
  blockpage.block_num = block;
  blockpage.page_num = page;
  blockpage.buffer = pagedat;
  
  while(blockpage.page_num <= ptw)
  {
    int i = 0;
      
    memset(pagedat, 0xff, LARGE_BLOCK_PAGE_SIZE);

    for (i = 0; i < 4; i++)
    {
      memcpy((pagedat+(i*SMALL_BLOCK_PAGE_SIZE)), 
        pBuffer, SMALL_BLOCK_PAGE_MAIN_AREA_SIZE);

        pBuffer += (SMALL_BLOCK_PAGE_MAIN_AREA_SIZE); 
    }

    if (mlcnand_ioctl(mlcnanddev, MLC_WRITE_PAGE,
                      (INT_32) &blockpage) == _ERROR)
    {
//      sprintf(buff, "Error writing block %d, page %d\r\n", block, 
//        blockpage.page_num);
//      uart_output((UNS_8*)buff);
      return _ERROR;
    }

    blockpage.page_num++;
  }

  return _NO_ERROR;
}

/***********************************************************************
 *
 * Function: app1WriteIcr
 *
 * Purpose: Write Interface Configuration data (ICR)
 *
 * Parameters: 
 *    ptw:    Number of pages to write for the application stored in
 *            block 0
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS app1WriteIcr(UNS_32 ptw)
{
  UNS_16 tmp16, tmp16i;
  UNS_8 *pagedat = (UNS_8 *) f16page;
  MLC_BLOCKPAGE_T blockpage;
  int i;

  /* Generate first page data for LPC3250 loader */
  for (i = 0; i < LARGE_BLOCK_PAGE_SIZE/2; i++) 
  {
    f16page [i] = 0x0000;
  }

  /* 
   * Setup FLASH config for large block, 4 address cycles.
   * Described in section 2.2.3.2 in the User's Manual for LPC3250 
   */
  tmp16 = 0xB4;

  tmp16i = 0xFF - tmp16;
  f16page[0] = tmp16;
  f16page[2] = tmp16i;
  f16page[4] = tmp16;
  f16page[6] = tmp16i;

  /*
   * Application size info. Described in section 2.2.3.3
   * in the User's manual
   */
  tmp16 = (UNS_16) ptw;
  tmp16i = 0x00FF - tmp16;
  f16page[8] = tmp16;
  f16page[10] = tmp16i;
  f16page[12] = tmp16;
  f16page[14] = tmp16i;
  f16page[16] = tmp16;
  f16page[18] = tmp16i;
  f16page[20] = tmp16;
  f16page[22] = tmp16i;
  f16page[24] = 0x00AA; /* Good block marker for page #0 ICR only */
  
  /* write ICR to page 0 in block 0*/                  
  blockpage.dma = 0;
  blockpage.block_num = 0;
  blockpage.page_num = 0;
  blockpage.buffer = (UNS_8 *) pagedat;
  if (mlcnand_ioctl(mlcnanddev, MLC_WRITE_PAGE,
                    (INT_32) &blockpage) == _ERROR)
  {
//    uart_output("Error writing block 0, page 0\r\n");
    return _ERROR;
  }

  return _NO_ERROR;
}


/***********************************************************************
 *
 * Function: app2IsMagicNumberAvail
 *
 * Purpose: Check if magic number is stored in block 1/page 0 
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the magic number is available; otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
static BOOL_32 app2IsMagicNumberAvail(void)
{
  BOOL_32 result = FALSE;
  MLC_BLOCKPAGE_T blockpage;
  UNS_8* buffer = (UNS_8*) f16page;

  blockpage.dma = 0;

  /* 
   * Here we assume that block 1 is always valid, which doesn't have 
   * to be the case. Should be changed so that the first valid block
   * after block 0 is used
   */
  blockpage.block_num = 1;
  blockpage.page_num = 0;
  blockpage.buffer = (UNS_8 *) buffer;

  if (mlcnand_ioctl(mlcnanddev, MLC_READ_PAGE,
                    (INT_32) &blockpage) == _ERROR)
  {
//    uart_output("Error reading block 1, page 0\r\n");
    goto failed;
  }

  if (memcmp(app2Magic, buffer, sizeof(app2Magic)) == 0)
    result = TRUE;

failed:

  return result;
}

/***********************************************************************
 *
 * Function: app2SaveBadBlockInfo
 *
 * Purpose: Save the bad block info in a location not overwritten
 *          with data. 
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS app2SaveBadBlockInfo(void)
{
  int i = 0;
  MLC_BLOCKPAGE_T blockpage;
  UNS_8* buffer = (UNS_8*) f16page;
  UNS_8 bad_0 = 0, bad_1 = 0;
  STATUS status = _ERROR;

  /*
   * The bad block info is stored in index 2048 of page 0
   * or page 1 for a block in the Samsung NAND.
   *
   * The MLC controller treats a large page NAND as if
   * a page is divided into four 528 bytes sections
   * (512 data + 16 spare) which means that index 2048 
   * will be overwritten with data. 
   *
   * Index 2096 can be used instead (528*3 + 512)
   *
   * The spare area in each section (16 bytes) is divided
   * into 6 bytes for data and 10 bytes for ECC info. The
   * ECC info is written by the MLC controller.
   */

  blockpage.dma = 0;
  blockpage.buffer = (UNS_8 *) buffer;

  /* the size of app2 is limited by size of IRAM -> max 2 blocks */
  for (i = 1; i < 2/*NAND_NUMBER_BLOCKS*/; i++)
  {
    blockpage.block_num = i;

    blockpage.page_num = 1;
    mlcnand_ioctl(mlcnanddev, MLC_READ_PAGE, (INT_32) &blockpage);
    bad_1 = buffer[NAND_SAMSUNG_BAD_OFFSET];

    blockpage.page_num = 0;
    mlcnand_ioctl(mlcnanddev, MLC_READ_PAGE, (INT_32) &blockpage);
    bad_0 = buffer[NAND_SAMSUNG_BAD_OFFSET];

    if (bad_0 != NAND_GOOD_BLOCK_MARKER ||
        bad_1 != NAND_GOOD_BLOCK_MARKER)
    {

      //sprintf(buff, "Saving bad block info for block %d\r\n", i);
      //uart_output((UNS_8*)buff);

      if (mlcnand_ioctl(mlcnanddev, MLC_ERASE_BLOCK,
                        (INT_32) i) == _ERROR)
      {
        //sprintf(buff, "Error erasing block %d\r\n", i);
        //uart_output((UNS_8*)buff);        
      }

      buffer[NAND_MLC_BAD_OFFSET] = NAND_BAD_BLOCK_MARKER;

      /* write bad block info in page 0 */      
      if (mlcnand_ioctl(mlcnanddev, MLC_WRITE_PAGE,
                        (INT_32) &blockpage) == _ERROR)
      {
       // sprintf(buff, "Error writing block %d, page 0\r\n", i);
        //uart_output((UNS_8*)buff);
        goto failed;
      }
      
    }   
  }

  status = _NO_ERROR;

failed:
  
  return status;
}

/***********************************************************************
 *
 * Function: app2IsBlockInvalid
 *
 * Purpose: Check if a block is marked as invalid (bad)
 *
 * Parameters: 
 *    blockNum:  The block number to check
 *
 * Outputs: None
 *
 * Returns:
 *    TRUE if it is invalid; otherwise FALSE 
 *
 * Notes: None
 *
 **********************************************************************/
static BOOL_32 app2IsBlockInvalid(UNS_32 blockNum)
{
  BOOL_32 result = TRUE;
  MLC_BLOCKPAGE_T blockpage;
  UNS_8* buffer = (UNS_8*) f16page;

  blockpage.dma = 0;
  blockpage.buffer = (UNS_8 *) buffer;
  blockpage.block_num = blockNum;
  blockpage.page_num = 0;

  if (mlcnand_ioctl(mlcnanddev, MLC_READ_PAGE,
                        (INT_32) &blockpage) == _ERROR)
  {
//    sprintf(buff, "Error reading block %d, page 0\r\n", blockNum);
//    uart_output((UNS_8*)buff);    
    goto failed;
  }

  result = (buffer[NAND_MLC_BAD_OFFSET] == NAND_BAD_BLOCK_MARKER);

failed:

  return result;
}

/***********************************************************************
 *
 * Function: app2EraseBlocks
 *
 * Purpose: Erase thos blocks needed by the application
 *
 * Parameters: 
 *    size:  Size of application in bytes
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS app2EraseBlocks(UNS_32 size)
{
  UNS_32 numBlocks = 0;
  UNS_32 blocksErased = 0;
  UNS_32 blockNum = 1;
  STATUS status = _NO_ERROR;


  /* 
   * page 0 in block 0 is reserved for magic number 
   * and size information so add one page size to the
   * total size info
   */

  numBlocks = (size+LARGE_BLOCK_PAGE_MAIN_AREA_SIZE)/NAND_BLOCK_SIZE;
  if ((numBlocks*NAND_BLOCK_SIZE 
    < (size+LARGE_BLOCK_PAGE_MAIN_AREA_SIZE)) != 0)
    numBlocks++;
    
  while(blocksErased < numBlocks)
  {

    /* Check the boundary for number of blocks */ 
    if (blockNum >= NAND_NUMBER_BLOCKS)
    {
      //sprintf(buff, "Error cannot erase all needed blocks. BlockNum = %d, num=%d\r\n", 
      //  blockNum, numBlocks);
      //uart_output((UNS_8*)buff);

      status = _ERROR;
      break;
    }

    /* if a block is invalid skip it */
    if (app2IsBlockInvalid(blockNum))
    {
      blockNum++;
      continue;
    }

    /* erase the block */
    if (mlcnand_ioctl(mlcnanddev, MLC_ERASE_BLOCK,
                      (INT_32) blockNum) == _ERROR)
    {

      //sprintf(buff, "Error erasing block %d\r\n", blockNum);
      //uart_output((UNS_8*)buff);

      blockNum++;
      continue;
    }

    blocksErased++;
    blockNum++;
  }
  
//  sprintf(buff, "%d blocks erased. Last block=%d\r\n", 
//    blocksErased, blockNum-1);
//  uart_output((UNS_8*)buff);    

  return status;
}

/***********************************************************************
 *
 * Function: app2WriteMagicAndSize
 *
 * Purpose: Write magic number and size information in block 1, page 0
 *
 * Parameters: 
 *    size:    Size of the application
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS app2WriteMagicAndSize(UNS_32 size)
{
  MLC_BLOCKPAGE_T blockpage;
  UNS_8* buffer = (UNS_8*) f16page;
  UNS_32 sizes[2];
  
  memset(buffer, 0xff, LARGE_BLOCK_PAGE_SIZE); 

  memcpy(buffer, app2Magic, sizeof(app2Magic));

	sizes[0] = size;
	sizes[1] = 0xFFFFFFFF - size;
	memcpy(buffer+sizeof(app2Magic), sizes, sizeof(sizes));

  blockpage.dma = 0;
  blockpage.buffer = (UNS_8 *) buffer;
  /* 
   * Here we assume that block 1 is always valid, which doesn't have 
   * to be the case. Should be changed so that the first valid block
   * after block 0 is used
   */
  blockpage.block_num = 1;
  blockpage.page_num = 0;

  if (mlcnand_ioctl(mlcnanddev, MLC_WRITE_PAGE,
                    (INT_32) &blockpage) == _ERROR)
  {

//    uart_output("Error writing block 1, page 0\r\n");
    return _ERROR;
  }

  return _NO_ERROR;
}

/***********************************************************************
 *
 * Function: app2Prepare
 *
 * Purpose: Prepare NAND for a second application
 *
 * Parameters: 
 *    size:    Size of the application
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS app2Prepare(UNS_32 size)
{
  STATUS status = _ERROR;

  
  if (!app2IsMagicNumberAvail())
  {

     //uart_output("No magic number available, saving bad block info\r\n");

    /* 
     * if the magic number isn't available we need to 
     * make sure bad block info is saved in spare area
     * used by MLC controller.
     */
    if (app2SaveBadBlockInfo() != _NO_ERROR)
      goto failed;  
  }

  do {

    if (app2EraseBlocks(size) != _NO_ERROR)
      break;

    if (app2WriteMagicAndSize(size) != _NO_ERROR)
      break;

    status = _NO_ERROR;
  } while (FALSE);

failed:

  return status;
}

/***********************************************************************
 *
 * Function: app2WriteAppData
 *
 * Purpose: 
 *    Write the application data
 *
 * Parameters:
 *    pBuffer: Application data 
 *    size:    Size of the application
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
static STATUS app2WriteAppData(UNS_8* pBuffer, UNS_32 size)
{
  STATUS status = _ERROR;
  UNS_32 blockNum = 1;
  UNS_32 page = 1;
  UNS_32 ptw = 0;
  UNS_32 dataWritten = 0;

  while(dataWritten < size)
  {

    /* if a block is invalid skip it */
    if (app2IsBlockInvalid(blockNum))
    {
      blockNum++;
      continue;
    }

    ptw = (size-dataWritten) / LARGE_BLOCK_PAGE_MAIN_AREA_SIZE;
    if (ptw*LARGE_BLOCK_PAGE_MAIN_AREA_SIZE < (size-dataWritten))
      ptw++;

    if (ptw > (NAND_PAGES_PER_BLOCK-page))
      ptw = (NAND_PAGES_PER_BLOCK-page);  

    if (nand_write_pages(blockNum, page, 
      pBuffer+dataWritten, ptw) != _NO_ERROR)
    {
      //sprintf(buff, "Failed to write pages in block %d\r\n", blockNum);
      //uart_output((UNS_8*)buff);
      goto failed;    
    }

    dataWritten += (ptw*LARGE_BLOCK_PAGE_MAIN_AREA_SIZE);
    blockNum++;
    page = 0;    
  }
  status = _NO_ERROR;
  
        
failed:

  return status;
}

/***********************************************************************
 *
 * Function: nand_boot_store_app2
 *
 * Purpose: Program a second application to block 1 and onwards
 *
 * Parameters: 
 *    pBuffer:    Buffer containing the application
 *    size:       Size of the buffer
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
STATUS nand_boot_store_app2(UNS_8* pBuffer, UNS_32 size)
{
  STATUS status = _ERROR;

  /* 
   * App2 will be executed from internal RAM which then sets the
   * limit for the application size.
   */
  if (size > MAX_APP2_SIZE)
  {
    //sprintf(buff, "App2 too large to fit in NAND (%d > %d)\r\n", 
    //  size, MAX_APP2_SIZE);
    //uart_output((UNS_8*)buff);

    return _ERROR;
  }

  do {  

    if (app2Prepare(size) != _NO_ERROR)
      break;

    if (app2WriteAppData(pBuffer, size) != _NO_ERROR)
      break;

    status = _NO_ERROR;
  } while (FALSE);

  return status;
}

/***********************************************************************
 *
 * Function: nand_boot_store_app1
 *
 * Purpose: Program block 0 with a bootable application. The bootloader
 *          in the LPC3250 can boot an application stored in block 0.
 *
 * Parameters: 
 *    pBuffer:    Buffer containing the application
 *    size:       Size of the buffer
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
STATUS nand_boot_store_app1(UNS_8* pBuffer, UNS_32 size)
{
  STATUS status = _ERROR;
  UNS_32 ptw = 0;

  /* make sure the application fits in block 0 */
  if (size > NAND_MAX_BOOT_SIZE_LARGE)
  {
//    sprintf(buff, "App too large to fit in block 0 (%d > %d)\r\n", 
//      size, NAND_MAX_BOOT_SIZE_LARGE);
//    uart_output((UNS_8*)buff);

    return _ERROR;
  }

  /* Determine number of pages to write */
  ptw = size / LARGE_BLOCK_PAGE_MAIN_AREA_SIZE;
  if ((ptw * LARGE_BLOCK_PAGE_MAIN_AREA_SIZE) < size)
  {
  	ptw++;
  }
 	ptw++; /* Include non-used sector */

  /* erase block 0 */
  if (mlcnand_ioctl(mlcnanddev, MLC_ERASE_BLOCK,
                    (INT_32) 0) == _ERROR)
  {
//    uart_output("Failed to erase block 0\r\n");
    return _ERROR;
  }


  do {  

    if (app1WriteIcr(ptw) != _NO_ERROR)
      break;

    /* Store application in block 0, starting from page 1 */
    if (nand_write_pages(0, 1, pBuffer, ptw) != _NO_ERROR)
      break;

    status = _NO_ERROR;
  } while (FALSE);

  return status;
}

/***********************************************************************
 *
 * Function: nand_boot_init
 *
 * Purpose: Initialize NAND
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: STATUS
 *
 * Notes: None
 *
 **********************************************************************/
STATUS nand_boot_init(void)
{
  MLC_TIMING_T mt;
  UNS_32 flashid;
  UNS_32 clk;

  clkpwr_setup_nand_ctrlr(0, 0, 0);
  clkpwr_clk_en_dis(CLKPWR_NAND_MLC_CLK, 1);

  mlcnanddev = mlcnand_open(MLCNAND, (INT_32) &NAND_FLASH);
  if (mlcnanddev == 0)
  {
//    uart_output("Error opening NAND driver\r\n");
    deadloop();
  }

  /* Setup NAND controller timing */

  clk = clkpwr_get_clock_rate(CLKPWR_NAND_MLC_CLK);

  mt.tcea_delay = (1 + (clk / EA_NAND_TCEA_DELAY));
  mt.busy_delay = (1 + (clk / EA_NAND_BUSY_DELAY));
  mt.nand_ta    = (1 + (clk / EA_NAND_NAND_TA));
  mt.r_high     = (1 + (clk / EA_NAND_RD_HIGH));
  mt.r_low      = (1 + (clk / EA_NAND_RD_LOW));
  mt.wr_high    = (1 + (clk / EA_NAND_WR_HIGH));
  mt.wr_low     = (1 + (clk / EA_NAND_WR_LOW));


  if (mlcnand_ioctl(mlcnanddev, MLC_SET_TIMING, (INT_32) &mt)
                    == _ERROR)
  {
//    uart_output("Error setting NAND timing\r\n");
    return _ERROR;
  }

  /* Get NAND flash id */
  if (mlcnand_ioctl(mlcnanddev, MLC_READ_ID,
                    (INT_32) &flashid) == _ERROR)
  {
//    uart_output("Error reading NAND ID\r\n");
    return _ERROR;
  }
  flashid = flashid & 0xFFFF;

//  sprintf(buff, "Detected Flash ID is: %x\r\n", flashid);
//  uart_output((UNS_8*)buff);

  return _NO_ERROR;
}

/***********************************************************************
 *
 * Function: nand_boot_deinit
 *
 * Purpose: De-Initialize NAND
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void nand_boot_deinit(void)
{
  mlcnand_close(mlcnanddev);  
}


