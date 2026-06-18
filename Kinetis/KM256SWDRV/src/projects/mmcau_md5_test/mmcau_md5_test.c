/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmcau_md5_test.c
 * performs MD5 hash algorithm test using MMCAU
 ******************************************************************************/
#include "drivers.h"
#include "mmcau.h"

/******************************************************************************
 * MD5 static function prototypes                                             *
 ******************************************************************************/
static void md5(unsigned char in[], unsigned char out[], int nbytes);

/******************************************************************************
 * MD5 variables                                                              *
 ******************************************************************************/
/* MD5 padding constant                                                       */
static unsigned char pad[] =
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* MD5 input string                                                           */
unsigned char md5_string[]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

/* holds the MD5 hash value output                                            */
unsigned char md5_out[16];

/******************************************************************************
 * md5 function: takes care of MD5 padding                                    *
 * so that its length is divisible by 512 bits (64 bytes)                     *
 * Function limited up to 2 MD5 blocks: 128 bytes                             *
 ******************************************************************************/
static void md5(unsigned char in[], unsigned char out[], int nbytes)
{
    int i, j, k, n, npad;
   /* variable to hold padded input from original message                     */
    unsigned char padded_input[192];

   /* 1: insert MD5 padding to handle input data strings of 128 chars or less */
    if (nbytes < 64)
      k = 0;
    else
    {
      k = 64;
      /* copy the 1st 64 chars of the input string to padded_input array */
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
    padded_input[k+j] = n & 0xff;
    padded_input[k+j+1] = n>>8 & 0xff;
    padded_input[k+j+2] = n>>16 & 0xff;
    padded_input[k+j+3] = n>>24 & 0xff;
    padded_input[k+j+4] = 0;
    padded_input[k+j+5] = 0;
    padded_input[k+j+6] = 0;
    padded_input[k+j+7] = 0;

    /* calculate number of 512-bit blocks to process by CAU                   */
    n = (nbytes > 56) ? 2 : 1;
    /* 2: execute MD5 hash                                                    */
    mmcau_md5_update(&padded_input[0], n, (unsigned char *)&out[0]);
}

/******************************************************************************
 * md5 main function: performs MD5 test using MMCAU                           *
 ******************************************************************************/
void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* FSL: calculate MD5 with padding support, hash stored in md5_out          */
  md5(md5_string, md5_out, sizeof(md5_string)-1);

  while(1);
}
