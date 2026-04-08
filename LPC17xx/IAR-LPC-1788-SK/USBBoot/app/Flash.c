/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : Flash.c
 *    Description : LPC17xx flash programming low level functions
 *                 
 *    History :
 *    1. Date        : November, 2009.
 *       Author      : Stoyan Choynev
 *       Description : This is a modified versioin of FlashNXPLPC210x.c 
 *
 *    $Revision: 4280 $
 **************************************************************************/

/** include files **/
#include "includes.h"
#include "Flash.h"
/** local definitions **/
typedef struct SLayout {
  int sectors;
  int size;
} Layout;

//==============================================
//==============================================
//                32k flash, CortexM3
// LPC1751
//==============================================
//==============================================
//                64k flash, CortexM3
// LPC1752
//==============================================
//==============================================
//                128k flash, CortexM3
// LPC1754
// LPC1764
//==============================================
//==============================================
//                256k flash, CortexM3
// LPC1756
// LPC1765
// LPC1766
//==============================================
//==============================================
//                512k flash, CortexM3
// LPC1758
// LPC1768
//==============================================
//==============================================
//                32k flash, CortexM3/M0
// LPC1x13
// LPC1343
//==============================================
//==============================================
//                16k flash, CortexM3
// LPC1342
//==============================================
//==============================================
//                8k flash, CortexM3/M0
// LPC1x11
//==============================================
const Layout flashLayout[] =
{
  {16,  4096},
  {14, 32768},
  { 0,     0}
};

const int allowedWriteSizes[] =
{
  4096,
  1024,
  512,
  256,
  0
};


//__root int Dummy @ ".DUMMY";

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
static int ExecuteCommand(unsigned long* cmd, unsigned long* status);
static int CalculateSector(unsigned long addr);
/** public data **/

/** private data **/
static const IAP iap_entry = (IAP)kIAPentry; // MCU flash firmware interface function.
// The CPU clock speed (CCLK), the default value is used if no clock option is found.

static int ExecuteCommand(unsigned long* cmd, unsigned long* status)
{
  int ret;

  for (;;)
  {
    iap_entry(cmd, status);
    ret = status[0];
    if (ret != STATUS_BUSY)
    {
      return ret;
    }
    // Try again if busy.
  }
}

// Returns the flash sector number for a given flash address.
// Returns -1 if the address is outside the flash.
static int CalculateSector(unsigned long addr)
{
  int i;
  int j;
  int sector = 0;
  unsigned long current = 0;

  for (i = 0; flashLayout[i].sectors; i++)
  {
    for (j = 0; j < flashLayout[i].sectors; j++)
    {
      if (addr < current + flashLayout[i].size)
      {
        return sector;
      }
      sector++;
      current += flashLayout[i].size;
    }
  }
  return -1;
}


/*************************************************************************
 * Function Name: FlashWrite
 * Parameters: block base address, offet in block, data size, ram buffer
 *             pointer
 * Return:
 *
 * Description
 *
 *************************************************************************/
uint32_t FlashWrite(uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{
  int ret;
  unsigned int  Size ;
  unsigned long cmd[5];
  unsigned long status[3];

    do
    {
      for (unsigned int i = 0; allowedWriteSizes[i]; i++)
      {//take the max. allowed size
        Size = allowedWriteSizes[i];
        if (count >= allowedWriteSizes[i])
        {
          break;
        }
      }


      cmd[0] = CMD_PREPARE_SECTORS;
      cmd[1] = CalculateSector(offset_into_block);
      cmd[2] = CalculateSector(offset_into_block + Size-1);


      ret = ExecuteCommand(cmd, status);

      if (ret != STATUS_CMD_SUCCESS)
      {
        //message CMD_PREPARE_SECTORS failed.
        return ((uint32_t)-1);
      }

      cmd[0] = CMD_COPY_RAM_TO_FLASH;
      cmd[1] = offset_into_block;
      cmd[2] = (unsigned long)buffer;
      cmd[3] = Size;
      cmd[4] = CLK_GetClock(CLK_CPU)/1000;


      ret = ExecuteCommand(cmd, status);

      if (ret != STATUS_CMD_SUCCESS)
      {
        //message CMD_COPY_RAM_TO_FLASH failed.
        return ((uint32_t)-1);
      }

      if(count > cmd[3]) count -= cmd[3];
      else count = 0;

      offset_into_block += cmd[3];
      buffer += cmd[3];
    }
    while(count);

    return 0;
}
/*************************************************************************
 * Function Name: FlashErase
 * Parameters:  Block Address, Block Size
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
uint32_t FlashErase(void *block_start,
                    uint32_t block_size)
{
  int ret;
  int start_sector;
  int end_sector;
  unsigned long cmd[4];
  unsigned long status[3];

    start_sector = CalculateSector((unsigned long)block_start);
    end_sector = CalculateSector((unsigned long)block_start+(block_size-1));

    // Prepare sector for erase.
    cmd[0] = CMD_PREPARE_SECTORS;
    cmd[1] = start_sector;
    cmd[2] = end_sector;


    ret = ExecuteCommand(cmd, status);

   if (ret != STATUS_CMD_SUCCESS)
    {
      //message CMD_PREPARE_SECTORS failed.
      return((uint32_t)-1);
    }

    // Erase sector.
    cmd[0] = CMD_ERASE_SECTORS;
    cmd[1] = start_sector;
    cmd[2] = end_sector;
    cmd[3] = CLK_GetClock(CLK_CPU)/1000;


    ret = ExecuteCommand(cmd, status);

    if (ret != STATUS_CMD_SUCCESS)
    {
      //message CMD_ERASE_SECTORS failed.
      return((uint32_t)-1);
    }

    return 0;
}

/*************************************************************************
 * Function Name: Sector Blank Check
 * Parameters:  Block Address, Block Size
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
uint32_t FlashBlankCheck(void *block_start,
                    uint32_t block_size)
{
  int ret;
  int start_sector;
  int end_sector;
  unsigned long cmd[4];
  unsigned long status[3];

    start_sector = CalculateSector((unsigned long)block_start);
    end_sector = CalculateSector((unsigned long)block_start+(block_size-1));

    // Prepare sector for erase.
    cmd[0] = CMD_BLANK_CHECK_SECTORS;
    cmd[1] = start_sector;
    cmd[2] = end_sector;


    ret = ExecuteCommand(cmd, status);

   if (ret != STATUS_SECTOR_NOT_BLANK)
    {
      /*Sector is not Blank, Invalid Sector or
        param error*/
      return((uint32_t)-1);
    }

    return 0;
}
