/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * dmam2p_test.c
 ******************************************************************************/
#include "drivers.h"

/* Imported symbols from linker                                               */
#if defined (__ICCARM__)
  #pragma section = ".intvec"
  #pragma section = ".text"
#elif defined(__GNUC__)
  extern char __CODE_BEGIN[];
  extern char __CODE_END[];
#elif defined(__CC_ARM)
  extern char Load$$ER_ROM$$Base[];
  extern char Load$$ER_ROM$$Length[];
#endif

static volatile uint32 result;
static uint8 *pCRC_DATA_REG;

#if defined (__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
void main (void)
{
#if defined (__ICCARM__)
  uint8* codeptr  = __section_begin(".intvec");
  uint32 codesize = (uint32)__section_end(".text")-(uint32)codeptr;
#elif defined(__GNUC__)
  uint8* codeptr  = (uint8*)__CODE_BEGIN;
  uint32 codesize = (uint32)__CODE_END-(uint32)codeptr;
#elif defined(__CC_ARM)
  uint8* codeptr  = (uint8*)Load$$ER_ROM$$Base;
  #pragma diag_suppress 1296
  uint32 codesize = (uint32)Load$$ER_ROM$$Length;
#endif

  /* Initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  while (1)
  {
    /* Initialize CRC module                                                  */
    pCRC_DATA_REG = (uint8*)CRC_Init (CRC_MODULE_CRC32_CONFIG);

    /* Initialize DMA for memory to peripheral continuous SW trigger transfer */
    DMA_Init (DMA0,
        DMA_CH_SWTRG_M2P_CN_CONFIG(codeptr,DMA_SIZE8BIT,pCRC_DATA_REG,codesize));
    while (!DMA_IsDone(DMA0));  /* Wait until transfer is completed           */

    /* Read computed CRC code                                                 */
    result = CRC_Read();
  }
}
