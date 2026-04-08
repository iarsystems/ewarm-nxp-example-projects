/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * dmaaccess_test.c
 ******************************************************************************/
#include "drivers.h"

/* Imported symbols from linker                                               */
#if defined (__ICCARM__) || defined(__GNUC__)
  extern char __ICFEDIT_region_ROM_start__[];
  extern char __ICFEDIT_region_ROM_end__[];
  extern char __ICFEDIT_region_RAM_start__[];
  extern char __ICFEDIT_region_RAM_end__[];
  extern char __ICFEDIT_region_PRIVRAM_start__[];
  extern char __ICFEDIT_region_PRIVRAM_end__[];

  #define ROM_START_ADDR      __ICFEDIT_region_ROM_start__
  #define ROM_END_ADDR        __ICFEDIT_region_ROM_end__
  #define RAM_START_ADDR      __ICFEDIT_region_RAM_start__
  #define RAM_END_ADDR        __ICFEDIT_region_RAM_end__
  #define PRIVRAM_START_ADDR  __ICFEDIT_region_PRIVRAM_start__
  #define PRIVRAM_END_ADDR    __ICFEDIT_region_PRIVRAM_end__
#elif defined(__CC_ARM)
  #define ROM_START_ADDR      0x00000000
  #define ROM_END_ADDR        0x0001ffff
  #define RAM_START_ADDR      0x1ffff000
  #define RAM_END_ADDR        0x20001fff
  #define PRIVRAM_START_ADDR  0x20002000
  #define PRIVRAM_END_ADDR    0x20002fff
#endif

#if defined (__ICCARM__)
#pragma location = "PRIVDATA"
  static volatile uint8 dstbuff[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#elif defined(__GNUC__) || defined(__CC_ARM)
  static volatile uint8_t __attribute__((section (".PRIVDATA"))) dstbuff[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#endif

static volatile uint8 srcbuff[20] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};

void main (void)
{
  register uint16 i=0;

  /* Set bus masters attribute to be controlled internally by the core        */
  MCM_SetMasterAttr (MCM_CM0_MASTER|MCM_DMA_MASTER,
                     MCM_MASTER_EN_PRIV_OR_USER_SECURE_OR_NONSEC,
                     TRUE);

  /* Initialize RGD1 = Flash; RWX supervisor mode, RWX user mode              */
  MPU_RgdInit(RGD1,
              MPU_RGD_EN_CM0_PID_OFF_DMA_PID_OFF_CONFIG(MPU_SPVR_USER,/* CM0+ */
                                                        MPU_USER_RWX,
                                                        MPU_SPVR_USER,/* DMA  */
                                                        MPU_USER_RWX,
                                                        ROM_START_ADDR,
                                                        ROM_END_ADDR));

  /* Initialize RGD2 = RAM; RW supervisor mode, RW user mode                  */
  MPU_RgdInit(RGD2,
              MPU_RGD_EN_CM0_PID_OFF_DMA_PID_OFF_CONFIG(MPU_SPVR_RW,  /* CM0+ */
                                                        MPU_USER_RW,
                                                        MPU_SPVR_RW,  /* DMA  */
                                                        MPU_USER_RW,
                                                        RAM_START_ADDR,
                                                        RAM_END_ADDR));

  /* Initialize RGD3 = protected RAM                                          */
  /* CM0+ master:  RW supervisor mode, RW user mode                           */
  /* DMA  master:  RW supervisor mode, None user mode                         */
  MPU_RgdInit(RGD3,
              MPU_RGD_EN_CM0_PID_OFF_DMA_PID_OFF_CONFIG(MPU_SPVR_RW,  /* CM0+ */
                                                        MPU_USER_RW,
                                                        MPU_SPVR_RW,  /* DMA  */
                                                        MPU_USER_NONE,
                                                        PRIVRAM_START_ADDR,
                                                        PRIVRAM_END_ADDR));

  /* Invalidate overlapping RGD0                                              */
  MPU_DisableRGD0();

  /* Initialize DMA from privileged mode => channel access will be set to     */
  /* privileged mode; the transfer will complete successfully                 */
  DMA_AdvInit (DMA0,
               DMA_CH_SWTRG_M2M_CN_CONFIG(srcbuff,dstbuff,sizeof(srcbuff)),
               DMA_CHACR_RW_RW_RW,
               DMA_UMNSM_EN_PRIV_OR_USER_SECURE_OR_NONSEC);
  while (!DMA_IsDone(DMA0));  /* Wait until transfer is completed             */
  DMA_Ack(DMA0);              /* Acknowledge completed transfer               */

  /* Switch to user mode                                                      */
  UserMode();

  /* Note that software can clear destination buffer because user access is   */
  /* allowed for CM0 master                                                   */
  for (i=0; i<20; i++) { dstbuff[i]=0; }

  /* Initialize DMA from user mode => channel access will be set to user mode */
  /* Note that this transfer will not be completed because "DMA" master user  */
  /* secure and unsecure mode accesses to PRIVRAM region are prohibited; the  */
  /* transfer will terminate with a bus error during its write portion        */
  DMA_AdvInit (DMA0,
               DMA_CH_SWTRG_M2M_CN_CONFIG(srcbuff,dstbuff,sizeof(srcbuff)),
               DMA_CHACR_RW_RW_RW,
               DMA_UMNSM_EN_PRIV_OR_USER_SECURE_OR_NONSEC);
  /* Wait until transfer is completed or error condition is set               */
  while (!DMA_IsDone(DMA0));

  /* Software should hang in next loop due to bus error on destination (BED)  */
  /* flag being asserted during DMA transfer                                  */
  while (DMA_IsWrError(DMA0));
}
