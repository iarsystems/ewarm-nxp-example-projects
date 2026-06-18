/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * ftfa_test.c
 ******************************************************************************/
#include "drivers.h"

/* this variables is stored in parameter section of the flash  addr=0x001fc00 */
#if defined (__ICCARM__)
#pragma location = ".config"
  static const uint32 nvm_data[] = {1l, 2l, 3l, 4l};
#elif defined(__GNUC__) || defined(__CC_ARM)
  static const uint32 __attribute__((section (".config"))) nvm_data[] = {1l, 2l, 3l, 4l};
#endif

void main (void)
{
  uint32 ram_data[] = { 4l, 3l, 2l, 1l };

  FTFA_EraseSector (nvm_data);      /* erases flash sector; sets all 0s to 1s */
  /* write data in reverse order                                              */
  FTFA_WriteArray  (nvm_data,ram_data,sizeof(nvm_data));

  while(1);
}
