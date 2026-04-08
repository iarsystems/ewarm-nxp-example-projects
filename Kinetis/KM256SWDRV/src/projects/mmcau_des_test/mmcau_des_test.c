/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmcau_des_test.c
 * performs DES cipher test using MMCAU
 * Note: going from input to output and back is possible, DES is symmetric
 ******************************************************************************/
#include "mmcau.h"
#include "drivers.h"

/******************************************************************************
 * DES constants                                                              *
 ******************************************************************************/
/* First 8 bytes are the key to be used for DES                               */
const unsigned char des_key[]=
{
 0x0c, 0xdb, 0x25, 0xe3, 0xba, 0x3c, 0x6d, 0x79
};

/* Second 8 bytes are the input for DES                                       */
const unsigned char des_plain[]=
{
 0x0c, 0xdb, 0x25, 0xe3, 0xba, 0x3c, 0x6d, 0x79
};

/* Input and output for DES algorithm                                         */
unsigned char des_out[8];


void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* encrypts a single 8-byte block                                           */
  mmcau_des_encrypt(des_plain, des_key, des_out);

  /* Decrypts a single 8-byte block taken from previous step                  */
  mmcau_des_decrypt(des_out, des_key, des_out);

  while(1);
}
