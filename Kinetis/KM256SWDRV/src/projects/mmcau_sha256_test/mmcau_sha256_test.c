/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmcau_sha256_test.c
 * performs SHA256 hash algorithm test using MMCAU
 ******************************************************************************/
#include "drivers.h"
#include "mmcau.h"

/******************************************************************************
 * SHA256 static function prototypes                                          *
 ******************************************************************************/
void sha256(unsigned char in[], unsigned int out[], int nbytes);
/******************************************************************************
 * SHA256 variables                                                           *
 ******************************************************************************/

/* SHA1 padding constant                                                      */
static unsigned char pad[] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Sha256 input message                                                       */
static unsigned char sha256_string[] =
"The quick brown fox jumps over the lazy dog";

/* holds the SHA256 output                                                    */
static unsigned int sha256_out[8];

/******************************************************************************
 * sha256 function: takes care of SHA256 padding                              *
 * so that its length is divisible by 512 bits (64 bytes)                     *
 * Function limited up to 2 SHA256 blocks: 128 bytes                          *
 ******************************************************************************/
void sha256(unsigned char in[], unsigned int out[], int nbytes)
{
    int i, j, n, npad;
    /* variable to hold padded input from original message                    */
    unsigned char tail[128];

    /* perform the message padding and parsing                                */
    npad = nbytes & 63;
    if (npad)
    {
        j = nbytes - npad;
        for (i = 0; i < npad; i++)
            tail[i] = in[i + j];
    }
    j = ((npad < 56) ? 56 : 120) - npad;
    for (i = 0; i < j; i++)
        tail[i + npad] = pad[i];
    j += npad;
    n = nbytes << 3;
    tail[j] = 0;
    tail[j+1] = 0;
    tail[j+2] = 0;
    tail[j+3] = 0;
    tail[j+4] = n>>24 & 0xff;
    tail[j+5] = n>>16 & 0xff;
    tail[j+6] = n>>8 & 0xff;
    tail[j+7] = n & 0xff;

    /* calculate number of 512-bit blocks to process by MMCAU                 */
    n = nbytes >> 6;

    /* Initializes the hash output and checks the MMCAU hardware revision     */
    mmcau_sha256_initialize_output ((unsigned int *)out);

    /* Performs SHA256 hash algorithm for a single input message block        */
    for (i = 0; i < n; i++)
        mmcau_sha256_hash(&in[i*64], (unsigned int *)out);

    /* performs SHA256 hash algorithm for a single input message block        */
    mmcau_sha256_hash(&tail[0], (unsigned int *)out);
    /* Performs SHA256 hash algorithm for a single input message block        */
    if (npad > 55)
        mmcau_sha256_hash(&tail[64], (unsigned int *)out);
    /* at this point, the message digest is contained in out[]                */
}

/******************************************************************************
 * sha256 main function: performs SHA256 hash test using MMCAU                *
 ******************************************************************************/
void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* calculate SHA256 with padding support, hash stored in sha256_out         */
  sha256(sha256_string, sha256_out, sizeof(sha256_string)-1);

  while(1);
}
