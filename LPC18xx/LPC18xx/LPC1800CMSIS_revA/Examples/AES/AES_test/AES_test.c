/**********************************************************************
* $Id: AES_test.c 28 2014-01-27 14:45:13Z danielru $		AES_test.c	2011-12-27
*//**
* @file		AES_test.c
* @brief	This example describes how to use AES Rom Library
* @version	1.0
* @date		27. Dec. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_libcfg.h"
#include "lpc18xx_cgu.h"
#include "otp.h"
#include "lpc18xx_aes_otp.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup AES_test	AES_test
 * @ingroup AES_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
/** Application Test Mode */
//#define FACTORY_MODE_TEST

#define OTP_BANK_0_ADDR		0x40045000UL
#define OTP_WORD_0_OFFSET	0x0
#define OTP_WORD_1_OFFSET	0x4
#define OTP_WORD_2_OFFSET	0x8
#define OTP_WORD_3_OFFSET	0xC

#define AES_KEY1_LOCK_MASK	(1UL << 30)
#define AES_KEY2_LOCK_MASK	(1UL << 29)

/* Simple AES Function Alias */
#define u32AES_Init					AESLib->aes_Init
#define u32AES_SetMode				AESLib->aes_SetMode
#define u32AES_LoadKey1				AESLib->aes_LoadKey1
#define u32AES_LoadKey2				AESLib->aes_LoadKey2
#define u32AES_LoadUserInitVector	AESLib->aes_LoadIV_SW
#define u32AES_Operate				AESLib->aes_Operate
#define u32AES_LoadUserKey			AESLib->aes_LoadKeySW
#define u32AES_ProgramKey1			AESLib->aes_ProgramKey1
#define u32AES_ProgramKey2			AESLib->aes_ProgramKey2

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" AES Rom library demo \n\r"
"\t - MCU: LPC18xx \n\r"
"\t - Core: ARM CORTEX-M3 \n\r"
"\t - Communicate via: UART0 - 115200 bps \n\r"
" Use AES to encrypt and decrypt data \n\r"
"********************************************************************************\n\r";

static volatile uint32_t u32Result;
/************************** PRIVATE FUNCTIONS *************************/
void ClockInit(void);
void vECBEncode(void);
void vECBDecode(void);
void vCBCEncode(void);
void vCBCDecode(void);
void vECBEncodeOTPKey1(void);
void vECBDecodeOTPKey1(void);
void vCBCEncodeOTPKey2(void);
void vCBCDecodeOTPKey2(void);
void vCatchError(uint8_t u8Error);

/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}



/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void) {                       /* Main Program                       */
	
	SystemInit();
	CGU_Init();
	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	print_menu();
	
#ifdef FACTORY_MODE_TEST
	/* Test encryption in ECB mode */
	vECBEncode();
	/* Test decryption in ECB mode */
	vECBDecode();
	/* Test encryption in CBC mode */
	vCBCEncode();
	/* Test decryption in CBC mode */
	vCBCDecode();

	lpc_printf("Ready\r\n");

	while(1);
#else
	/* Check if OTP configuration has been set*/
	if ((*(uint32_t *)(OTP_BANK_0_ADDR + OTP_WORD_1_OFFSET) & (1UL << 14)) == 0)
	{
		// OTP_WRMASK (0x080): This register masks APB write access to OTP Fuses protecting unintentional fuse programming by customer.
		// ‘0’ must be written to respective bit before initiating write operation to an OTP Fuse. Bootcode should set this
		// register to 0x07 before ending the booting sequence.
		/* Get access to OTP0 */
		LPC_OTP->WRTMASK &= ~(1<<0); 
		while(LPC_OTP->WRTMASK&(1<<0)) 
		{
			__NOP();
		}

		/* Configuration not set, make part AES capable and finalize configuration */		
		*(uint32_t *)(OTP_BANK_0_ADDR + OTP_WORD_0_OFFSET) = (6UL << 4);  /* Set to AES capable (AES encode/decode) */
		*(uint32_t *)(OTP_BANK_0_ADDR + OTP_WORD_1_OFFSET) = (1UL << 14); /* Set configured bit - can't set any more bits once this is done */    
		*(uint32_t *)(OTP_BANK_0_ADDR + OTP_WORD_2_OFFSET) = 0;                    
		*(uint32_t *)(OTP_BANK_0_ADDR + OTP_WORD_3_OFFSET) = 0;

		/* !!!MUST RESET BEFORE RUNNING FURTHER TESTS!!! */
		while(1);
	}
	else 
	{					
		static uint8_t au8Key1[16] = {0x0F,0x0E,0x0D,0x0C, 0x0B,0x0A,0x09,0x08, 0x07,0x06,0x05,0x04, 0x03,0x02,0x01,0x00};
		static uint8_t au8Key2[16] = {0x3c,0x4f,0xcf,0x09, 0x88,0x15,0xf7,0xab, 0xa6,0xd2,0xae,0x28, 0x16,0x15,0x7e,0x2b};
		
		/* Program test keys into the OTP fuse banks set aside for key storage */
		u32Result = u32AES_ProgramKey1(au8Key1);
		u32Result = u32AES_ProgramKey2(au8Key2);

		/* CAUTION: Once programmed, encoding/decoding tests are only possible in the remaining of this code. 
		   Otherwise, after reset/power up the JTAG will always be closed by bootrom. In this case, only
		   booting with an encrypted image is possible. */

		/* Test encryption in ECB mode */
		vECBEncode();
		/* Test decryption in ECB mode */
		vECBDecode();
		/* Test encryption in CBC mode */
		vCBCEncode();
		/* Test decryption in CBC mode */
		vCBCDecode();

		/* Test encryption in ECB mode using OTP key 0 */
		vECBEncodeOTPKey1();
		/* Test decryption in ECB mode using OTP key 0 */
		vECBDecodeOTPKey1();
		/* Test encryption in CBC mode using OTP key 1 */
		vCBCEncodeOTPKey2();
		/* Test decryption in CBC mode using OTP key 1 */
		vCBCDecodeOTPKey2();

		/* If we get here all tests have passed */
		while(1);		
	}	
#endif
}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}

/*********************************************************************//**
 * @brief		AES ECB Encryption
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vECBEncode(void)
{
	uint32_t i,fail=0;
	
	/* Test encryption (ECB mode) using the following test vectors taken from FIPS-197 (key loaded via application)  */
	/* http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-ecb-128  */
   	/* PLAINTEXT: 6bc1bee22e409f96e93d7e117393172a (127_0)*/
   	/* KEY:       2b7e151628aed2a6abf7158809cf4f3c (127_0)*/
   	/* RESULT:    3ad77bb40d7a3660a89ecaf32466ef97 (127_0)*/

	static uint8_t au8Key[16] =        {0x3c,0x4f,0xcf,0x09, 0x88,0x15,0xf7,0xab, 0xa6,0xd2,0xae,0x28, 0x16,0x15,0x7e,0x2b}; //key0, key1, key2, key3
	static uint8_t au8PlainText[16] =  {0x2a,0x17,0x93,0x73, 0x11,0x7e,0x3d,0xe9, 0x96,0x9f,0x40,0x2e, 0xe2,0xbe,0xc1,0x6b}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 
	static uint8_t au8CypherText[16] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8Result[16] =     {0x97,0xef,0x66,0x24, 0xf3,0xca,0x9e,0xa8, 0x60,0x36,0x7a,0x0d, 0xb4,0x7b,0xd7,0x3a}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadUserKey(au8Key); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_ENCODE_ECB);
	u32Result = u32AES_Operate(au8CypherText, au8PlainText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8CypherText[i] != au8Result[i])
		{
			lpc_printf("AES ECB encryption failed!\r\n");
			fail = 1;
//			vCatchError(0);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES ECB encryption passed\r\n");
}

/*********************************************************************//**
 * @brief		AES ECB Decryption
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vECBDecode(void)
{
	uint32_t i,fail=0;
	
	/* Test decryption (ECB mode) using the following test vectors taken from FIPS-197 (key loaded via application)  */
	/* http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-ecb-128  */
   	/* PLAINTEXT: 6bc1bee22e409f96e93d7e117393172a (127_0)*/
   	/* KEY:       2b7e151628aed2a6abf7158809cf4f3c (127_0)*/
   	/* RESULT:    3ad77bb40d7a3660a89ecaf32466ef97 (127_0)*/

	static uint8_t au8Key[16] =        {0x3c,0x4f,0xcf,0x09, 0x88,0x15,0xf7,0xab, 0xa6,0xd2,0xae,0x28, 0x16,0x15,0x7e,0x2b}; //key0, key1, key2, key3
	static uint8_t au8PlainText[16] =  {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8CypherText[16] = {0x97,0xef,0x66,0x24, 0xf3,0xca,0x9e,0xa8, 0x60,0x36,0x7a,0x0d, 0xb4,0x7b,0xd7,0x3a}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8Result[16] =     {0x2a,0x17,0x93,0x73, 0x11,0x7e,0x3d,0xe9, 0x96,0x9f,0x40,0x2e, 0xe2,0xbe,0xc1,0x6b}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadUserKey(au8Key); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_DECODE_ECB);
	u32Result = u32AES_Operate(au8PlainText, au8CypherText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8PlainText[i] != au8Result[i])
		{
			lpc_printf("AES ECB decryption failed!\r\n");
			fail = 1;
//			vCatchError(1);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES ECB decryption passed\r\n");
}

/*********************************************************************//**
 * @brief		AES CBC Encryption
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vCBCEncode(void)
{
	uint32_t i,fail=0;

	/* Test encryption (CBC mode) using the following test vectors taken from (key loaded via application):
	   http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-cbc-128  */
   	/* INIT VECTOR: 000102030405060708090a0b0c0d0e0f (127_0)*/
	/* PLAINTEXT:   6bc1bee22e409f96e93d7e117393172a (127_0)*/
   	/* KEY:         2b7e151628aed2a6abf7158809cf4f3c (127_0)*/
   	/* RESULT:      7649abac8119b246cee98e9b12e9197d (127_0)*/
              
	static uint8_t au8InitVector[16] = {0x0f,0x0e,0x0d,0x0c, 0x0b,0x0a,0x09,0x08, 0x07,0x06,0x05,0x04, 0x03,0x02,0x01,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 
	static uint8_t au8Key[16] =        {0x3c,0x4f,0xcf,0x09, 0x88,0x15,0xf7,0xab, 0xa6,0xd2,0xae,0x28, 0x16,0x15,0x7e,0x2b}; //key0, key1, key2, key3
	static uint8_t au8PlainText[16] =  {0x2a,0x17,0x93,0x73, 0x11,0x7e,0x3d,0xe9, 0x96,0x9f,0x40,0x2e, 0xe2,0xbe,0xc1,0x6b}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 
	static uint8_t au8CypherText[16] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8Result[16] =     {0x7d,0x19,0xe9,0x12, 0x9b,0x8e,0xe9,0xce, 0x46,0xb2,0x19,0x81, 0xac,0xab,0x49,0x76}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadUserInitVector(au8InitVector);
	u32Result = u32AES_LoadUserKey(au8Key); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_ENCODE_CBC);
	u32Result = u32AES_Operate(au8CypherText, au8PlainText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8CypherText[i] != au8Result[i])
		{
			lpc_printf("AES CBC encryption failed!\r\n");
			fail = 1;
//			vCatchError(2);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES CBC encryption passed\r\n");
}

/*********************************************************************//**
 * @brief		AES ECB Decryption
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vCBCDecode(void)
{
	uint32_t i,fail=0;

	/* Test encryption (CBC mode) using the following test vectors taken from (key loaded via application):
	   http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-cbc-128  */
   	/* INIT VECTOR: 000102030405060708090a0b0c0d0e0f (127_0)*/
	/* PLAINTEXT:   6bc1bee22e409f96e93d7e117393172a (127_0)*/
   	/* KEY:         2b7e151628aed2a6abf7158809cf4f3c (127_0)*/
   	/* RESULT:      7649abac8119b246cee98e9b12e9197d (127_0)*/

	static uint8_t au8InitVector[16] = {0x0f,0x0e,0x0d,0x0c, 0x0b,0x0a,0x09,0x08, 0x07,0x06,0x05,0x04, 0x03,0x02,0x01,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 
	static uint8_t au8Key[16] =        {0x3c,0x4f,0xcf,0x09, 0x88,0x15,0xf7,0xab, 0xa6,0xd2,0xae,0x28, 0x16,0x15,0x7e,0x2b}; //key0, key1, key2, key3
	static uint8_t au8PlainText[16] =  {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8CypherText[16] = {0x7d,0x19,0xe9,0x12, 0x9b,0x8e,0xe9,0xce, 0x46,0xb2,0x19,0x81, 0xac,0xab,0x49,0x76}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8Result[16] =     {0x2a,0x17,0x93,0x73, 0x11,0x7e,0x3d,0xe9, 0x96,0x9f,0x40,0x2e, 0xe2,0xbe,0xc1,0x6b}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadUserInitVector(au8InitVector);
	u32Result = u32AES_LoadUserKey(au8Key); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_DECODE_CBC);
	u32Result = u32AES_Operate(au8PlainText, au8CypherText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8PlainText[i] != au8Result[i])
		{
			lpc_printf("AES CBC decryption failed!\r\n");
			fail = 1;
//			vCatchError(3);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES CBC decryption passed\r\n");
}


/*********************************************************************//**
 * @brief		AES ECB Encrypt OTP key 1
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vECBEncodeOTPKey1(void)
{
	uint32_t i,fail=0;
	
	/* Test encryption (ECB mode) using the following test vectors taken from FIPS-197 (key loaded from OTP bank)  */
   	/* PLAINTEXT: 00112233445566778899aabbccddeeff */
   	/* KEY:       000102030405060708090a0b0c0d0e0f */
   	/* RESULT:    69c4e0d86a7b0430d8cdb78070b4c55a */	

	static uint8_t au8PlainText[16] =  {0xFF,0xEE,0xDD,0xCC, 0xBB,0xAA,0x99,0x88, 0x77,0x66,0x55,0x44, 0x33,0x22,0x11,0x00};
	static uint8_t au8CypherText[16] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
	static uint8_t au8Result[16] =     {0x5a,0xc5,0xb4,0x70, 0x80,0xb7,0xcd,0xd8, 0x30,0x04,0x7b,0x6a, 0xd8,0xe0,0xc4,0x69};

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadKey1(); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_ENCODE_ECB);
	u32Result = u32AES_Operate(au8CypherText, au8PlainText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8CypherText[i] != au8Result[i])
		{
			lpc_printf("AES ECB encryption with OTP key 1 failed!\r\n");
			fail = 1;
//			vCatchError(0);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES ECB encryption with OTP key 1 passed\r\n");
}

/*********************************************************************//**
 * @brief		AES ECB Decrypt OTP key 1
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vECBDecodeOTPKey1(void)
{
	uint32_t i,fail=0;
	
	/* Test decryption (ECB mode) using the following test vectors taken from FIPS-197 (key loaded from OTP bank)  */
   	/* PLAINTEXT: 69c4e0d86a7b0430d8cdb78070b4c55a */
   	/* KEY:       000102030405060708090a0b0c0d0e0f */
   	/* RESULT:    00112233445566778899aabbccddeeff */	

	static uint8_t au8PlainText[16] =  {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
	static uint8_t au8CypherText[16] = {0x5a,0xc5,0xb4,0x70, 0x80,0xb7,0xcd,0xd8, 0x30,0x04,0x7b,0x6a, 0xd8,0xe0,0xc4,0x69};		
	static uint8_t au8Result[16] =     {0xFF,0xEE,0xDD,0xCC, 0xBB,0xAA,0x99,0x88, 0x77,0x66,0x55,0x44, 0x33,0x22,0x11,0x00};

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadKey1(); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_DECODE_ECB);
	u32Result = u32AES_Operate(au8PlainText, au8CypherText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8PlainText[i] != au8Result[i])
		{
			lpc_printf("AES ECB decryption with OTP key 1 failed!\r\n");
			fail = 1;
//			vCatchError(1);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES ECB decryption with OTP key 1 passed\r\n");
}

/*********************************************************************//**
 * @brief		AES CBC Encrypt OTP key 2
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vCBCEncodeOTPKey2(void)
{
	uint32_t i,fail=0;

	/* Test encryption (CBC mode) using the following test vectors taken from (key loaded via application):
	   http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-cbc-128  */
   	/* INIT VECTOR: 000102030405060708090a0b0c0d0e0f (127_0)*/
	/* PLAINTEXT:   6bc1bee22e409f96e93d7e117393172a (127_0)*/
   	/* KEY:         2b7e151628aed2a6abf7158809cf4f3c (127_0)*/
   	/* RESULT:      7649abac8119b246cee98e9b12e9197d (127_0)*/
              
	static uint8_t au8InitVector[16] = {0x0f,0x0e,0x0d,0x0c, 0x0b,0x0a,0x09,0x08, 0x07,0x06,0x05,0x04, 0x03,0x02,0x01,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 
	static uint8_t au8PlainText[16] =  {0x2a,0x17,0x93,0x73, 0x11,0x7e,0x3d,0xe9, 0x96,0x9f,0x40,0x2e, 0xe2,0xbe,0xc1,0x6b}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120 
	static uint8_t au8CypherText[16] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120
	static uint8_t au8Result[16] =     {0x7d,0x19,0xe9,0x12, 0x9b,0x8e,0xe9,0xce, 0x46,0xb2,0x19,0x81, 0xac,0xab,0x49,0x76}; //7_0,15_8,23_16,31_24, 39_32,47_40,55_48,63_56, 71_64,79_72,87_80,95_88, 103_96,111_104,119_112,127_120

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadUserInitVector(au8InitVector);		
	u32Result = u32AES_LoadKey2(); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_ENCODE_CBC);
	u32Result = u32AES_Operate(au8CypherText, au8PlainText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8CypherText[i] != au8Result[i])
		{
			lpc_printf("AES CBC encryption with OTP key 2 failed!\r\n");
			fail = 1;
//			vCatchError(2);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES CBC encryption with OTP key 2 passed\r\n");
}

/*********************************************************************//**
 * @brief		AES CBC Decrypt OTP key 2
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vCBCDecodeOTPKey2(void)
{
	uint32_t i,fail=0;

	/* Test decryption (CBC mode) using the following test vectors taken from (key loaded from OTP bank):
	   http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-cbc-128  */
   	/* INIT VECTOR: 000102030405060708090a0b0c0d0e0f */
	/* PLAINTEXT:   7649abac8119b246cee98e9b12e9197d */	
   	/* KEY:         2b7e151628aed2a6abf7158809cf4f3c */
   	/* RESULT:      6bc1bee22e409f96e93d7e117393172a */
              	
	static uint8_t au8InitVector[16] = {0x0F,0x0E,0x0D,0x0C, 0x0B,0x0A,0x09,0x08, 0x07,0x06,0x05,0x04, 0x03,0x02,0x01,0x00};            
	static uint8_t au8PlainText[16] =  {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};              	
	static uint8_t au8CypherText[16] = {0x7d,0x19,0xe9,0x12, 0x9b,0x8e,0xe9,0xce, 0x46,0xb2,0x19,0x81, 0xac,0xab,0x49,0x76};	
	static uint8_t au8Result[16] =     {0x2a,0x17,0x93,0x73, 0x11,0x7e,0x3d,0xe9, 0x96,0x9f,0x40,0x2e, 0xe2,0xbe,0xc1,0x6b};	

	u32Result = u32AES_Init();
	u32Result = u32AES_LoadUserInitVector(au8InitVector);		
	u32Result = u32AES_LoadKey2(); /* do it firstly before setting a mode! */
	u32Result = u32AES_SetMode(AES_API_MODE_DECODE_CBC);
	u32Result = u32AES_Operate(au8PlainText, au8CypherText, 1);
		
	/* Check the result is as expected */
	for (i = 0; i < 16; i++)
	{
		if (au8PlainText[i] != au8Result[i])
		{
			lpc_printf("AES CBC decryption with OTP key 2 failed!\r\n");
			fail = 1;
//			vCatchError(3);
			break;
		}
	}	
	if (!fail)
		lpc_printf("AES CBC decryption with OTP key 2 passed\r\n");
}

/*********************************************************************//**
 * @brief		Catch Execution error
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void vCatchError(uint8_t u8Error)
{
	volatile uint8_t u8theError = u8Error;

	while(1);
}
#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
 */
