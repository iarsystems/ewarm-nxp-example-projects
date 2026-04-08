/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmcau_sha1_test.c
 * performs SHA1 hash algorithm test using MMCAU
 ******************************************************************************/
#include "drivers.h"
#include "mmcau.h"

/******************************************************************************
 * SHA1 static function prototypes                                            *
 ******************************************************************************/
static void sha1(unsigned char in[], unsigned int out[], int nbytes);

/******************************************************************************
 * SHA1 variables                                                             *
 ******************************************************************************/
/* SHA1 padding constant                                                      */
static unsigned char pad[] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* SHA1 input message                                                         */
unsigned char sha1_string[]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

/* SHA1 output to be calculated                                               */
unsigned int sha1_out[5];

/******************************************************************************
 * sha1 function: takes care of SHA1 padding                                  *
 * so that its length is divisible by 512 bits (64 bytes)                     *
 * Function limited up to 2 SHA1 blocks: 128 bytes                            *
 ******************************************************************************/
static void sha1(unsigned char in[], unsigned int out[], int nbytes)
{
    int i, j, k, n, npad;

    /* variable to hold padded input from original message                    */
    unsigned char padded_input[192];

  /* 1: insert SHA1 padding to handle input data strings of 128 chars or less */
  if (nbytes < 64)
    k = 0;
  else
  {
    k = 1;
    /* copy the 1st 64 chars of the input string to padded_input array  */
    for (i = 0; i < 64; i++)
    padded_input[i] = in[i];
  }

    npad = nbytes & 63;
    if (npad)
    {
        j = nbytes - npad;
        for (i = 0; i < npad; i++)
            padded_input[k+i] = in[i + j];
    }
    j = ((npad < 56) ? 56 : 120) - npad;
    for (i = 0; i < j; i++)
        padded_input[k + i + npad] = pad[i];
    j += npad;
    n = nbytes << 3;
    padded_input[k+j] = 0;
    padded_input[k+j+1] = 0;
    padded_input[k+j+2] = 0;
    padded_input[k+j+3] = 0;
    padded_input[k+j+4] = n>>24 & 0xff;
    padded_input[k+j+5] = n>>16 & 0xff;
    padded_input[k+j+6] = n>>8 & 0xff;
    padded_input[k+j+7] = n & 0xff;

    /* calculate number of 512-bit blocks to process by MMCAU                 */
    n = (nbytes >= 56) ? 2 : 1;

    /* 2: execute SHA1 hash                                                   */
    mmcau_sha1_update(&padded_input[0], n,(unsigned int *)&out[0]);
}

/******************************************************************************
 * sha1 main function: performs SHA1 test using MMCAU                         *
 ******************************************************************************/
void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* Updates SHA1 state variables for one or more input message blocks        */
  mmcau_sha1_initialize_output((unsigned int *)sha1_out);

  /* calculate SHA1 with padding support, hash stored in sha1_out             */
  sha1(sha1_string, sha1_out, sizeof(sha1_string)-1);

  while(1);
}
