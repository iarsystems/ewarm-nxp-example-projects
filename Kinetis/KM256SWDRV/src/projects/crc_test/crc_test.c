/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * crc_test.c
 ******************************************************************************/
#include "drivers.h"

static const uint8   crcdata[]={'h','e','l','l','o',' ','w','o','r','l','d','!'};
static volatile uint32 result;

void main (void)
{
  SIM_EnableModule (CRC);                   /* enable clock to CRC peripheral */

  /* CRC32 calculation                                                        */
  CRC_Init (CRC_MODULE_CRC32_CONFIG);       /* result = 0x027B6A48            */
  result = (uint32)CRC_Calc8 (crcdata, sizeof(crcdata)   );

  CRC_Init (CRC_MODULE_CRC32_CONFIG);       /* result = 0x027B6A48            */
  result = (uint32)CRC_Calc16(crcdata, sizeof(crcdata)>>1);

  CRC_Init (CRC_MODULE_CRC32_CONFIG);       /* result = 0x027B6A48            */
  result = (uint32)CRC_Calc32(crcdata, sizeof(crcdata)>>2);

  /* CRC16_CCIT calculation                                                   */
  CRC_Init (CRC_MODULE_CRC16_CCIT_CONFIG);  /* result = 0x0000D382            */
  result = (uint32)CRC_Calc8 (crcdata, sizeof(crcdata)   );

  CRC_Init (CRC_MODULE_CRC16_CCIT_CONFIG);  /* result = 0x0000D382            */
  result = (uint32)CRC_Calc16(crcdata, sizeof(crcdata)>>1);

  CRC_Init (CRC_MODULE_CRC16_CCIT_CONFIG);  /* result = 0x0000D382            */
  result = (uint32)CRC_Calc32(crcdata, sizeof(crcdata)>>2);

  /* CRC16_DNP calculation                                                    */
  CRC_Init (CRC_MODULE_CRC16_DNP_CONFIG);   /* result = 0x0000A7F3            */
  result = (uint32)CRC_Calc8 (crcdata, sizeof(crcdata)   );

  CRC_Init (CRC_MODULE_CRC16_DNP_CONFIG);   /* result = 0x0000A7F3            */
  result = (uint32)CRC_Calc16(crcdata, sizeof(crcdata)>>1);

  CRC_Init (CRC_MODULE_CRC16_DNP_CONFIG);   /* result = 0x0000A7F3            */
  result = (uint32)CRC_Calc32(crcdata, sizeof(crcdata)>>2);

  /* CRC16_ANSI calculation                                                   */
  CRC_Init (CRC_MODULE_CRC16_ANSI_CONFIG);  /* result = 0x00006B77            */
  result = (uint32)CRC_Calc8 (crcdata, sizeof(crcdata)   );

  CRC_Init (CRC_MODULE_CRC16_ANSI_CONFIG);  /* result = 0x00006B77            */
  result = (uint32)CRC_Calc16(crcdata, sizeof(crcdata)>>1);

  CRC_Init (CRC_MODULE_CRC16_ANSI_CONFIG);  /* result = 0x00006B77            */
  result = (uint32)CRC_Calc32(crcdata, sizeof(crcdata)>>2);

  while(1);
}

