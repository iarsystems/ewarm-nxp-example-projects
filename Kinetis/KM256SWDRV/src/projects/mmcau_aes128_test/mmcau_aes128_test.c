/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmcau_aes128_test.c
 * performs AES128 cipher test using MMCAU
 * Note: going from input to output and back is possible, AES is symmetric
 ******************************************************************************/
#include "mmcau.h"
#include "drivers.h"

/******************************************************************************
 * AES128 constants                                                           *
 ******************************************************************************/
/* First 16 bytes are the key to be used for AES128                           */
const unsigned char aes_key[]=
{
 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
};

/* Second 16 bytes are the input for AES128                                   */
const unsigned char aes_plain[]=
{
 0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34
};

/* pointer to 16-byte block of output ciphertext                              */
unsigned char aes_out[16];
/* key schedule for 44 longwords for AES128                                   */
unsigned char w[44*4];
/* key_size in bits for AES128                                                */
int bits = 128;
/* number of AES rounds: f(key_schedule)                                      */
int nr = 10;


void main(void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* performs an AES key expansion                                            */
  mmcau_aes_set_key(aes_key, bits, w);

  /* encrypts a single 16-byte block                                          */
  mmcau_aes_encrypt(aes_plain, w, nr, (unsigned char *)aes_out);

  /* Decrypts a single 16-byte block taken from previous step                 */
  mmcau_aes_decrypt(aes_out, w, nr, aes_out);

  while(1);
}
