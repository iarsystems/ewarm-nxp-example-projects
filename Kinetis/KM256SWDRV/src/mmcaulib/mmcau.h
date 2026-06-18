/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      mmcau.h
 * @version   1.0.0.0
 * @date      April-07-2015
 * @brief     MMCAU header file.
 * @note      Tested with library mmcau_lib.a and lib_mmcau*.a (ver. 12/19/2013)
 ******************************************************************************/
#ifndef __MMCAU_H
#define __MMCAU_H

/******************************************************************************
 * exported function prototypes                                               *
 ******************************************************************************/
/******************************************************************************
* MMCAU function definitions
*
*//*! @addtogroup mmcau_function
* @{
*******************************************************************************/

/***************************************************************************//*!
 * @brief   AES: Performs an AES cipher key expansion
 * @details Sets AES secret key, used for encryption and decryption (symmetric)
 * @param   *key        pointer to input key (128, 192, 256 bits in length)
 * @param    key_size   key_size in bits (128, 192, 256)
 * @param   *key_sch    pointer to key schedule output (44, 52, 60 longwords)
 * @see     @ref mmcau_aes_encrypt, @ref mmcau_aes_decrypt
 ******************************************************************************/
void
mmcau_aes_set_key (const unsigned char *key, const int key_size,
                 unsigned char *key_sch);

/***************************************************************************//*!
 * @brief   AES: Encrypts a single 16-byte block
 * @details Encrypts a single 16-byte block
 * @param   *in         pointer to 16-byte block of input plaintext
 * @param   *key_sch    pointer to key schedule (44, 52, 60 longwords)
 * @param    nr         number of AES rounds (10, 12, 14 = f(key_schedule))
 * @param   *out        pointer to 16-byte block of output ciphertext
 * @note    Input and output blocks may overlap
 * @see     @ref mmcau_aes_set_key, @ref mmcau_aes_decrypt
 ******************************************************************************/
void
mmcau_aes_encrypt (const unsigned char *in, const unsigned char *key_sch,
                 const int nr, unsigned char *out);

/***************************************************************************//*!
 * @brief   AES: Decrypts a single 16-byte block
 * @details Decrypts a single 16-byte block
 * @param   *in         pointer to 16-byte block of input chiphertext
 * @param   *key_sch    pointer to key schedule (44, 52, 60 longwords)
 * @param    nr         number of AES rounds (10, 12, 14 = f(key_schedule))
 * @param   *out        pointer to 16-byte block of output plaintext
 * @note    Input and output blocks may overlap
 * @see     @ref mmcau_aes_set_key, @ref mmcau_aes_encrypt
 ******************************************************************************/
void
mmcau_aes_decrypt (const unsigned char *in, const unsigned char *key_sch,
                 const int nr, unsigned char *out);

/***************************************************************************//*!
 * @brief   DES: Checks key parity
 * @details Checks DES key parity
 * @param   *key        pointer to 64-bit DES key with parity bits
 * @return   0          no error
 * @return  -1          parity error
 * @see     @ref mmcau_des_encrypt, @ref mmcau_des_decrypt
 ******************************************************************************/
int
mmcau_des_chk_parity (const unsigned char *key);

/***************************************************************************//*!
 * @brief   DES: Encrypts a single 8-byte block
 * @details Encrypts a single 8-byte block using DES cipher
 * @param   *in         pointer to 8-byte block of input plaintext
 * @param   *key        pointer to 64-bit DES key with parity bits
 * @param   *out        pointer to 8-byte block of output ciphertext
 * @note    Input and output blocks may overlap
 * @see     @ref mmcau_des_chk_parity, @ref mmcau_des_decrypt
 ******************************************************************************/
void
mmcau_des_encrypt (const unsigned char *in, const unsigned char *key,
                 unsigned char *out);

/***************************************************************************//*!
 * @brief   DES: Decrypts a single 8-byte block
 * @details Decrypts a single 8-byte block using DES cipher
 * @param   *in         pointer to 8-byte block of input ciphertext
 * @param   *key        pointer to 64-bit DES key with parity bits
 * @param   *out        pointer to 8-byte block of output plaintext
 * @note    Input and output blocks may overlap
 * @see     @ref mmcau_des_chk_parity, @ref mmcau_des_encrypt
 ******************************************************************************/
void
mmcau_des_decrypt (const unsigned char *in, const unsigned char *key,
                 unsigned char *out);

/***************************************************************************//*!
 * @brief   MD5: Initializes the MD5 state variables
 * @details Initializes the MD5 state variables
 * @param   *md5_state  pointer to 120-bit block of md5 state variables: a,b,c,d
 * @see     @ref mmcau_md5_hash_n, @ref mmcau_md5_update,
 * @see     @ref mmcau_md5_hash
 ******************************************************************************/
void
mmcau_md5_initialize_output (const unsigned char *md5_state);


/***************************************************************************//*!
 * @brief  MD5: Updates MD5 state variables for one or more input message blocks
 * @details  Updates MD5 state variables for one or more input message blocks
 * @param   *msg_data   pointer to start of input message data
 * @param    num_blks   number of 512-bit blocks to process
 * @param   *md5_state  pointer to 128-bit block of MD5 state variables: a,b,c,d
 * @see     @ref mmcau_md5_initialize_output, @ref mmcau_md5_update,
 * @see     @ref mmcau_md5_hash
 ******************************************************************************/
void
mmcau_md5_hash_n (const unsigned char *msg_data, const int num_blks,
                unsigned char *md5_state);

/***************************************************************************//*!
 * @brief  MD5: Updates MD5 state variables for one or more input message blocks
 * @details  Updates MD5 state variables for one or more input message blocks
 * @param   *msg_data   pointer to start of input message data
 * @param    num_blks   number of 512-bit blocks to process
 * @param   *md5_state  pointer to 128-bit block of MD5 state variables: a,b,c,d
 * @see     @ref mmcau_md5_initialize_output, @ref mmcau_md5_hash_n,
 * @see     @ref mmcau_md5_hash
 ******************************************************************************/
void
mmcau_md5_update (const unsigned char *msg_data, const int num_blks,
                unsigned char *md5_state);

/***************************************************************************//*!
 * @brief   MD5: Performs MD5 hash algorithm for a single input message block
 * @details  Performs MD5 hash algorithm for a single input message block
 * @param   *msg_data   pointer to start of input message data
 * @param   *md5_state  pointer to 128-bit block of MD5 state variables: a,b,c,d
 * @see     @ref mmcau_md5_initialize_output, @ref mmcau_md5_hash_n,
 * @see     @ref mmcau_md5_update
 ******************************************************************************/
void
mmcau_md5_hash (const unsigned char *msg_data, unsigned char *md5_state);

/***************************************************************************//*!
 * @brief   SHA1: Initializes the SHA1 state variables
 * @details  Initializes the SHA1 hash algorithm state variables
 * @param   *sha1_state pointer to 160-bit block of SHA1 state variables:a,b,c,d,e
 * @see     @ref mmcau_sha1_hash_n, @ref mmcau_sha1_update, @ref mmcau_sha1_hash
 ******************************************************************************/
void
mmcau_sha1_initialize_output (const unsigned int *sha1_state);

/***************************************************************************//*!
 * @brief   SHA1: Perform the hash and generate SHA1 state variables
 * @brief   for one or more input message blocks
 * @details Perform the hash and generate SHA1 state variables
 * @param   *msg_data   pointer to start of input message data
 * @param    num_blks   number of 512-bit blocks to process
 * @param *sha1_state pointer to 160-bit block of SHA1 state variables:a,b,c,d,e
 * @note   Input message and state variable output blocks must not overlap
 * @see    @ref mmcau_sha1_initialize_output, @ref mmcau_sha1_update,
 * @see    @ref mmcau_sha1_hash
 ******************************************************************************/
void
mmcau_sha1_hash_n (const unsigned char *msg_data, const int num_blks,
                 unsigned int *sha1_state);

/***************************************************************************//*!
 * @brief   SHA1: Updates SHA1 state variables for one or more input message blocks
 * @details Updates SHA1 state variables for one or more input message blocks
 * @param   *msg_data   pointer to start of input message data
 * @param    num_blks   number of 512-bit blocks to process
 * @param *sha1_state pointer to 160-bit block of SHA1 state variables:a,b,c,d,e
 * @see    @ref mmcau_sha1_initialize_output, @ref mmcau_sha1_hash_n,
 * @see    @ref mmcau_sha1_hash
 ******************************************************************************/
void
mmcau_sha1_update (const unsigned char *msg_data, const int num_blks,
                 unsigned int *sha1_state);

/***************************************************************************//*!
 * @brief   SHA1: Performs SHA1 hash algorithm on a single input message block
 * @details Performs SHA1 hash algorithm on a single input message block arguments
 * @param   *msg_data   pointer to start of input message data
 * @param *sha1_state pointer to 160-bit block of SHA1 state variables:a,b,c,d,e
 * @see    @ref mmcau_sha1_initialize_output, @ref mmcau_sha1_hash_n,
 * @see    @ref mmcau_sha1_update
 ******************************************************************************/
void
mmcau_sha1_hash (const unsigned char *msg_data,
               unsigned int *sha1_state);

/***************************************************************************//*!
 * @brief   SHA256: Init the hash output and checks the HW revision arguments
 * @details Init the hash output and checks the HW revision arguments
 * @param   *output     pointer to 256-bit message digest output
 * @return   0          no error -> CAU2 hardware present
 * @return  -1          error -> incorrect CAU hardware revision
 * @see     @ref mmcau_sha256_hash_n, @ref mmcau_sha256_update,
 * @see     @ref mmcau_sha256_hash
 ******************************************************************************/
int
mmcau_sha256_initialize_output (const unsigned int *output);

/***************************************************************************//*!
 * @brief   SHA256: Updates digest output for one or more message blocks
 * @details Updates SHA256 digest output for one or more message block arguments
 * @param   *input      pointer to start of input message
 * @param    num_blks   number of 512-bit blocks to process
 * @param   *output     pointer to 256-bit message digest output
 * @note    Input message and digest output blocks must not overlap
 * @see     @ref mmcau_sha256_initialize_output, @ref mmcau_sha256_update,
 * @see     @ref mmcau_sha256_hash
 ******************************************************************************/
void
mmcau_sha256_hash_n (const unsigned char *input, const int num_blks,
                   unsigned int *output);

/***************************************************************************//*!
 * @brief   SHA256: Updates state variables for one or more input message blocks
 * @details Updates SHA256 state variables for one or more input message blocks
 * @param   *input      pointer to start of input message data
 * @param    num_blks   number of 512-bit blocks to process
 * @param   *output     pointer to 256-bit message digest output
 * @see     @ref mmcau_sha256_initialize_output, @ref mmcau_sha256_hash_n,
 * @see     @ref mmcau_sha256_hash
 ******************************************************************************/
void
mmcau_sha256_update (const unsigned char *input, const int num_blks,
                   unsigned int *output);

/***************************************************************************//*!
 * @brief   SHA256: Performs hash algorithm for a single input message block
 * @details Performs SHA256 hash algorithm for a single input message block
 * @param   *input      pointer to start of input message data
 * @param   *output     pointer to 256-bit message digest output
 * @see     @ref mmcau_sha256_initialize_output, @ref mmcau_sha256_hash_n,
 * @see     @ref mmcau_sha256_update
 ******************************************************************************/
void
mmcau_sha256_hash (const unsigned char *input, unsigned int *output);
/*! @} End of mmcau_function                                                 */

#endif /* __MMCAU_H */
