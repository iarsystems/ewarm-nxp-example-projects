/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * i2c_test.c
 ******************************************************************************/
#include "drivers.h"

#define BYTES (uint8)5                    /* number of bytes to read or write */

static uint8 volatile ui8_data[BYTES];
static uint8 i;

/* Send data to Slave, Wait on action complete, Clear Irq. flag               */
#define I2CWRITE(x)                                                            \
{                                                                              \
  I2C_WrData(I2C0, (x));                                                       \
  while(I2C_GetIsrFlag(I2C0) == 0);                                            \
  I2C_ClrIsrFlag(I2C0);                                                        \
}

/* Wait on action complete, Read data, Clear Irq. flag                        */
#define I2CREAD(x)                                                             \
{                                                                              \
  while(I2C_GetIsrFlag(I2C0) == 0);                                            \
  x = I2C_RdData(I2C0);                                                        \
  I2C_ClrIsrFlag(I2C0);                                                        \
}

void main (void)
{
  /*************************** INITIALIZING SECTION ***************************/
  /* initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* clock mode 2:1:1, 48MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_48MHZ_CONFIG);

  /* PORTs initialize, PTD7_SCL, PTE0_SDA                                     */
  PORT_Init (PORTD, PORT_MODULE_ALT2_OD_EN_MODE, PIN7);
  PORT_Init (PORTE, PORT_MODULE_ALT2_OD_EN_MODE, PIN0);

  /* I2C module initialization (100 kBps @24MHz)                              */
  I2C_Init  (I2C0, I2C_MODULE_IRQ_DI_CONFIG(0x1f));

  /****************************** WRITING SECTION *****************************/
  I2C_SetTxMode(I2C0);                 /* Set TX mode                         */
  I2C_StartSignal(I2C0);               /* Generate START on I2C bus           */

  I2CWRITE(0xA0);                      /* 0xA0= device (EEPROM) select byte   */
  I2CWRITE(0x00);                      /* 0x00= device starting address       */

  for (i=0; i<BYTES; i++)              /* Writing i-bytes to the memory       */
    I2CWRITE(2*i);                     /* Writing byte = address * 2          */

  I2C_StopSignal(I2C0);                /* Generate STOP signal on I2C bus     */
  arch_delay(1000000);                 /* Bus recovering between write/read   */

  /****************************** READING SECTION *****************************/
  I2C_SetTxMode(I2C0);                 /* Set TX mode                         */
  I2C_StartSignal(I2C0);               /* Generate START on I2C bus           */

  I2CWRITE(0xA0);                      /* 0xA0= device (EEPROM) select byte   */
  I2CWRITE(0x00);                      /* 0x00= device starting address       */

  I2C_RepeatStartSignal(I2C0);         /* Generate Repeat START on I2C bus    */
  while(!(I2C_GetTcFlag(I2C0)));       /* Waiting for bus releasing           */

  I2CWRITE(0xA1);                      /* 0xA1= activate device reading mode  */

  I2C_SetRxMode(I2C0);                 /* Set to RX Mode                      */
  (void) I2C_RdData(I2C0);             /* Dummy reading                       */

  for (i=0; i<BYTES; i++)              /* Reading i-bytes from the memory     */
  {
    if (i == (BYTES-1))                /* Last byte?                          */
    {
      I2C_SetNackMode(I2C0);           /* No acknowledge is needed now        */
      while(I2C_GetIsrFlag(I2C0) == 0);
      I2C_StopSignal(I2C0);            /* Generate STOP signal on I2C bus     */
      ui8_data[i] = I2C_RdData(I2C0);  /* Reading the last byte               */
      I2C_ClrIsrFlag(I2C0);
      I2C_ClrNackMode(I2C0);           /* next acknowledge is allowed         */
    }
    else
    {
      I2CREAD(ui8_data[i]);            /* Reading the i-th byte               */
    }
  }

  /* main loop - check the results                                            */
  while(1);
}
