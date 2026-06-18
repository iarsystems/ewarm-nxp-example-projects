/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mpu_test.c
 ******************************************************************************/
#include "drivers.h"

/* Imported symbols from linker                                               */
#if defined (__ICCARM__) || defined(__GNUC__)
  extern char __ICFEDIT_region_ROM_start__[];
  extern char __ICFEDIT_region_ROM_end__[];
  extern char __ICFEDIT_region_RAM_start__[];
  extern char __ICFEDIT_region_RAM_end__[];
  extern char __ICFEDIT_region_USERRAM_start__[];
  extern char __ICFEDIT_region_USERRAM_end__[];
  #define ROM_START_ADDR      __ICFEDIT_region_ROM_start__
  #define ROM_END_ADDR        __ICFEDIT_region_ROM_end__
  #define RAM_START_ADDR      __ICFEDIT_region_RAM_start__
  #define RAM_END_ADDR        __ICFEDIT_region_RAM_end__
  #define USERRAM_START_ADDR  __ICFEDIT_region_USERRAM_start__
  #define USERRAM_END_ADDR    __ICFEDIT_region_USERRAM_end__
#elif defined(__CC_ARM)
  #define ROM_START_ADDR      0x00000000
  #define ROM_END_ADDR        0x0001ffff
  #define RAM_START_ADDR      0x1ffff000
  #define RAM_END_ADDR        0x20001fff
  #define USERRAM_START_ADDR  0x20002000
  #define USERRAM_END_ADDR    0x20002fff
#endif

#if defined (__ICCARM__)
  #pragma location = "USERDATA"
  static volatile uint8 user_data[100]; // data with user and supervisor R/W access
#elif defined(__GNUC__) || defined(__CC_ARM)
  static volatile uint8 __attribute__((section (".USERDATA"))) user_data[100];
#endif

static volatile uint8 spvr_data[100];   // data with supervisor R/W access

void main (void)
{
  register uint8 tmp=1, i=0;

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
  /* Initialize RGD2 = RAM; RW supervisor mode, W user mode                   */
  MPU_RgdInit(RGD2,
              MPU_RGD_EN_CM0_PID_OFF_DMA_PID_OFF_CONFIG(MPU_SPVR_RW,  /* CM0+ */
                                                        MPU_USER_W,
                                                        MPU_SPVR_RW,  /* DMA  */
                                                        MPU_USER_W,
                                                        RAM_START_ADDR,
                                                        RAM_END_ADDR));
  /* Initialize RGD3 = RAM RW supervisor mode, RW user mode                   */
  MPU_RgdInit(RGD3,
              MPU_RGD_EN_CM0_PID_OFF_DMA_PID_OFF_CONFIG(MPU_SPVR_RW,  /* CM0+ */
                                                        MPU_USER_RW,
                                                        MPU_SPVR_RW,  /* DMA  */
                                                        MPU_USER_RW,
                                                        USERRAM_START_ADDR,
                                                        USERRAM_END_ADDR));

  /* Invalidate overlapping RGD0                                              */
  MPU_DisableRGD0();

  /* Test supervisor access                                                   */
  SpvrMode();               /* device run in supervisor mode by default       */
  for (i=0; i<100; i++) { user_data[i] = tmp; }           /* W - passes       */
  for (i=0; i<100; i++) { tmp = user_data[i]; }           /* R - passes       */
  for (i=0; i<100; i++) { spvr_data[i] = tmp; }           /* W - passes       */
  for (i=0; i<100; i++) { tmp = spvr_data[i]; }           /* R - passes       */

  /* Test user access                                                         */
  UserMode();               /* device transitioning to user mode              */
  for (i=0; i<100; i++) { user_data[i] = tmp; }           /* W - passes       */
  for (i=0; i<100; i++) { tmp = user_data[i]; }           /* R - passes       */
  for (i=0; i<100; i++) { spvr_data[i] = tmp; }           /* W - passes       */
  for (i=0; i<100; i++) { tmp = spvr_data[i]; }           /* R - RGD2 hit     */

  while(1);
}
