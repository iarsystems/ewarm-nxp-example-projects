
/*
 * $Revision: 4271 $
 */
#include <stdint.h>

#ifndef __FLASH_H
#define __FLASH_H

#define CCLK 14746

#if __CORE__ < 7
#define kIAPentry 0x7ffffff1
#else
#define kIAPentry 0x1FFF1FF1
#endif

enum {
  CMD_PREPARE_SECTORS = 50,
  CMD_COPY_RAM_TO_FLASH,
  CMD_ERASE_SECTORS,
  CMD_BLANK_CHECK_SECTORS,
  CMD_READ_PART_ID,
  CMD_READ_BOOT_CODE_VERSION,
  CMD_COMPARE,
};

enum {
  STATUS_CMD_SUCCESS = 0,
  STATUS_INVALID_COMMAND,
  STATUS_SRC_ADDR_ERROR,
  STATUS_DST_ADDR_ERROR,
  STATUS_SRC_ADDR_NOT_MAPPED,
  STATUS_DST_ADDR_NOT_MAPPED,
  STATUS_COUNT_ERROR,
  STATUS_INVALID_SECTOR,
  STATUS_SECTOR_NOT_BLANK,
  STATUS_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION,
  STATUS_COMPARE_ERROR,
  STATUS_BUSY,
};

typedef void (__thumb *IAP)(void*, void*);

extern uint32_t FlashWrite(uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer);
extern uint32_t FlashErase(void *block_start,
                    uint32_t block_size);
extern uint32_t FlashBlankCheck(void * block_start,
                    uint32_t block_size);
#endif /*__FLASH_H*/
