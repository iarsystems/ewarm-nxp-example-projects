/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * i2cisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* program constants                                                          */
#define READBYTES (uint8)5       /* number of bytes to read from 24C16 memory */

/* i2c callback function prototype                                            */
static void i2c_callback (I2C_CALLBACK_SRC module, I2C_CALLBACK_TYPE type);

/* data type definitions                                                      */
static uint8 ui8_data[READBYTES+3] = {0xa0,0,0xa1};           /* WR/RD buffer */
static uint8 cnt = 0;                                   /* WR/RD byte counter */

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* clock mode 2:1:1, 48MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_48MHZ_CONFIG);

  /* PORTs initialize, PTD7_SCL, PTE0_SDA                                     */
  PORT_Init (PORTD, PORT_MODULE_ALT2_MODE, PIN7);
  PORT_Init (PORTE, PORT_MODULE_ALT2_MODE, PIN0);

  /* I2C module initialize 100 kBps (@24MHz), IRQ master mode                 */
  I2C_Init  (I2C0, I2C_MODULE_IRQ_EN_CONFIG(0x1f));
  I2C_InstallCallback  (PRI_LVL1, i2c_callback);

  /* Set TX mode and Generate START on I2C bus                                */
  I2C_SetTxMode(I2C0);
  I2C_StartSignal(I2C0);

  /* Enable all interrupts                                                    */
  EnableInterrupts();

  /* Writing the 1-st byte (device select byte)                               */
  I2C_WrData(I2C0, ui8_data[0]);

  /* main loop                                                                */
  while(1);
}

/* i2c callback function definition                                           */
static void i2c_callback (I2C_CALLBACK_SRC module, I2C_CALLBACK_TYPE type)
{
  uint16 wait=0;

  if (module == I2C0_CALLBACK)
  {
    if (type == I2C_MTX_CALLBACK)
    {
      cnt++;
      if (cnt == 3)                                /* end of transmitting?    */
      {
        I2C_SetRxMode(I2C0);                       /* set to the RX mode      */
        cnt--;                                     /* will be dummy reading   */
      }
      else
      {
        if (cnt == 2)
        {
          I2C_RepeatStartSignal(I2C0);             /* generate repeat start   */
          while(I2C_GetTcFlag(I2C0) && (++wait));  /* bus releasing           */
        }
        I2C_WrData(I2C0, ui8_data[cnt]);           /* writing the n-th byte   */
      }
    }
    else if (type == I2C_MRX_CALLBACK)
    {
      if (cnt == (READBYTES+2))
      {
        I2C_StopSignal(I2C0);                      /* end of data reading     */
        I2C_ClrNackMode(I2C0);                     /* enables acknowledge     */
      }
      else if (cnt == (READBYTES+1))
      {
        I2C_SetNackMode(I2C0);                     /* no acknowledge needed   */
      }
      ui8_data[cnt++] = I2C_RdData(I2C0);          /* reading the n-th byte   */
    }
  }
}
