/**********************************************************************
* $Id: FlashPrg.c 28 2014-01-27 14:45:13Z danielru $		FlashPrg.c			2011-06-02
*//**
* @file		FlashPrg.c
* @brief	Flash Programming Functions adapted for LHF00L28 (16-bit Bus)
* @version	1.0
* @date		02. June. 2011
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
**********************************************************************/
#define BOARD_HITEX_LPC1800
#include "FlashOS.H"        // FlashOS Structures
#include "lpc18xx.h"
#include "lpc_types.h"
#include "lpc18xx_scu.h"

#if !(defined(BOARD_HITEX_LPC1800))
#define M8(adr)  (*((volatile unsigned char  *) (adr)))
#define M16(adr) (*((volatile unsigned short *) (adr)))
#define M32(adr) (*((volatile unsigned long  *) (adr)))

unsigned long base_adr;

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
  base_adr = adr;

	/* Set up EMC pin */
	scu_pinmux(	1	,	0	,	2<<2	,	2	);
	scu_pinmux(	1	,	1	,	2<<2	,	2	);
	scu_pinmux(	1	,	2	,	2<<2	,	2	);
	scu_pinmux(	1	,	3	,	2<<2	,	3	);
	scu_pinmux(	1	,	5	,	2<<2	,	3	);
	scu_pinmux(	1	,	6	,	2<<2	,	3	);
	scu_pinmux(	1	,	7	,	2<<2	,	3	);
	scu_pinmux(	1	,	8	,	2<<2	,	3	);
	scu_pinmux(	1	,	9	,	2<<2	,	3	);
	scu_pinmux(	1	,	10	,	2<<2	,	3	);
	scu_pinmux(	1	,	11	,	2<<2	,	3	);
	scu_pinmux(	1	,	12	,	2<<2	,	3	);
	scu_pinmux(	1	,	13	,	2<<2	,	3	);
	scu_pinmux(	1	,	14	,	2<<2	,	3	);
	scu_pinmux(	2	,	0	,	2<<2	,	2	);
	scu_pinmux(	2	,	1	,	2<<2	,	2	);
	scu_pinmux(	2	,	2	,	2<<2	,	2	);
	scu_pinmux(	2	,	6	,	2<<2	,	2	);
	scu_pinmux(	2	,	7	,	2<<2	,	3	);
	scu_pinmux(	2	,	8	,	2<<2	,	3	);
	scu_pinmux(	2	,	9	,	2<<2	,	3	);
	scu_pinmux(	2	,	10	,	2<<2	,	3	);
	scu_pinmux(	2	,	11	,	2<<2	,	3	);
	scu_pinmux(	2	,	12	,	2<<2	,	3	);
	scu_pinmux(	2	,	13	,	2<<2	,	3	);
	scu_pinmux(	5	,	0	,	2<<2	,	2	);
	scu_pinmux(	5	,	1	,	2<<2	,	2	);
	scu_pinmux(	5	,	2	,	2<<2	,	2	);
	scu_pinmux(	5	,	3	,	2<<2	,	2	);
	scu_pinmux(	5	,	4	,	2<<2	,	2	);
	scu_pinmux(	5	,	5	,	2<<2	,	2	);
	scu_pinmux(	5	,	6	,	2<<2	,	2	);
	scu_pinmux(	5	,	7	,	2<<2	,	2	);
	scu_pinmux(	6	,	3	,	2<<2	,	3	);
	scu_pinmux(	6	,	7	,	2<<2	,	1	);
	scu_pinmux(	6	,	8	,	2<<2	,	1	);
	scu_pinmux(	10	,	4	,	2<<2	,	3	);
	scu_pinmux(	13	,	2	,	2<<2	,	2	);
	scu_pinmux(	13	,	3	,	2<<2	,	2	);
	scu_pinmux(	13	,	4	,	2<<2	,	2	);
	scu_pinmux(	13	,	5	,	2<<2	,	2	);
	scu_pinmux(	13	,	6	,	2<<2	,	2	);
	scu_pinmux(	13	,	7	,	2<<2	,	2	);
	scu_pinmux(	13	,	8	,	2<<2	,	2	);
	scu_pinmux(	13	,	9	,	2<<2	,	2	);
	scu_pinmux(	13	,	12	,	2<<2	,	2	);
	scu_pinmux(	13	,	15	,	2<<2	,	2	);
	scu_pinmux(	13	,	16	,	2<<2	,	2	);
	scu_pinmux(	14	,	0	,	2<<2	,	3	);
	scu_pinmux(	14	,	1	,	2<<2	,	3	);
	scu_pinmux(	14	,	2	,	2<<2	,	3	);
	scu_pinmux(	14	,	3	,	2<<2	,	3	);
	scu_pinmux(	14	,	4	,	2<<2	,	3	);
	scu_pinmux(	14	,	5	,	2<<2	,	3	);
	scu_pinmux(	14	,	6	,	2<<2	,	3	);
	scu_pinmux(	14	,	7	,	2<<2	,	3	);
	scu_pinmux(	14	,	8	,	2<<2	,	3	);
	scu_pinmux(	14	,	9	,	2<<2	,	3	);
	scu_pinmux(	14	,	10	,	2<<2	,	3	);
	scu_pinmux(	14	,	11	,	2<<2	,	3	);
	scu_pinmux(	14	,	12	,	2<<2	,	3	);

	/* Set up EMC Controller */
	LPC_EMC->STATICWAITRD0 = 0x06;
	LPC_EMC->CONTROL = 0x01;
	LPC_EMC->STATICCONFIG0 = 0x81;
	LPC_EMC->STATICWAITOEN0 = 0x01;
  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  return 0;
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
	unsigned short sr;
  M16(adr) = 0x60;              // Start Unlock Block Command
  M16(adr) = 0xD0;              // Confirm Command
  do {
    M16(adr) = 0x70;
    sr = M16(adr);              // Read Status Register
  } while (!(sr & (1 << 7)));   // until Flash is Ready
  M16(adr) = 0xFF;              // Revert to Read Mode

  M16(adr) = 0x20;              // Start Erase Block Command
  M16(adr) = 0xD0;              // Confirm Command
  do {
    M16(adr) = 0x70;
    sr = M16(adr);              // Read Status Register
  } while (!(sr & (1 << 7)));   // until Flash is Ready
  M16(adr) = 0x50;              // Clear Status Register
  M16(adr) = 0xFF;              // Revert to Read Mode
  if (sr & 0x3A) {              // Check for Errors
    return (1);                 // Failed
  } else {
    return (0);                 // OK
  }
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  unsigned long ba;
  unsigned char sr = 0;
  int  i, n;
  n = sz/2;
  ba = adr;                                     // Block Address
  for (i = 0; i < n; i++)  {
    M16(adr) = 0x40;							// Write Command
      M16(adr) = *((unsigned short *) buf);     // Write Data to Flash
      adr += 2;                                 // Next Address
      buf += 2;                                 // Next Data
    do {
      sr = M16(ba);                             // Read Status Register
    } while (!(sr & (1 << 7)));                 // until Flash is Ready
    if (sr & 0x1B) break;                       // Check for Errors
  }
  M16(ba) = 0x50;                               // Clear Status Register
  M16(ba) = 0xFF;                               // Revert to Read Mode
  if (sr & 0x1B) {                              // Check for Errors
    return (1);                                 // Failed
  } else {
    return (0);                                 // OK
  }
}

#else

#define M8(adr)  (*((volatile unsigned char  *) (adr)))
#define M16(adr) (*((volatile unsigned short *) (adr)))
#define M32(adr) (*((volatile unsigned long  *) (adr)))

#define STACK_SIZE   64        // Stack Size


union fsreg {                  // Flash Status Register
  struct b  {
    unsigned int q0:1;
    unsigned int q1:1;
    unsigned int q2:1;
    unsigned int q3:1;
    unsigned int q4:1;
    unsigned int q5:1;
    unsigned int q6:1;
    unsigned int q7:1;
  } b;
  unsigned int v;
} fsr;

unsigned long base_adr;


/*
 * Check if Program/Erase completed
 *    Parameter:      adr:  Block Start Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int Polling (unsigned long adr) {
  unsigned int q6;

  // Check Toggle Bit
  do {
    fsr.v = M16(adr);
    q6 = fsr.b.q6;
    fsr.v = M16(adr);
  } while (fsr.b.q6 != q6);
  return (0);                  // Done
}


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
#define _WDWORD(x,y) (*(unsigned long *)x)=y
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
	base_adr = adr;

 	/* Set up EMC pin */
	scu_pinmux(	1	,	0	,	2<<2	,	2	);
	scu_pinmux(	1	,	1	,	2<<2	,	2	);
	scu_pinmux(	1	,	2	,	2<<2	,	2	);
	scu_pinmux(	1	,	3	,	2<<2	,	3	);
	scu_pinmux(	1	,	5	,	2<<2	,	3	);
	scu_pinmux(	1	,	6	,	2<<2	,	3	);
	scu_pinmux(	1	,	7	,	2<<2	,	3	);
	scu_pinmux(	1	,	8	,	2<<2	,	3	);
	scu_pinmux(	1	,	9	,	2<<2	,	3	);
	scu_pinmux(	1	,	10	,	2<<2	,	3	);
	scu_pinmux(	1	,	11	,	2<<2	,	3	);
	scu_pinmux(	1	,	12	,	2<<2	,	3	);
	scu_pinmux(	1	,	13	,	2<<2	,	3	);
	scu_pinmux(	1	,	14	,	2<<2	,	3	);
	scu_pinmux(	2	,	0	,	2<<2	,	2	);
	scu_pinmux(	2	,	1	,	2<<2	,	2	);
	scu_pinmux(	2	,	2	,	2<<2	,	2	);
	scu_pinmux(	2	,	6	,	2<<2	,	2	);
	scu_pinmux(	2	,	7	,	2<<2	,	3	);
	scu_pinmux(	2	,	8	,	2<<2	,	3	);
	scu_pinmux(	2	,	9	,	2<<2	,	3	);
	scu_pinmux(	2	,	10	,	2<<2	,	3	);
	scu_pinmux(	2	,	11	,	2<<2	,	3	);
	scu_pinmux(	2	,	12	,	2<<2	,	3	);
	scu_pinmux(	2	,	13	,	2<<2	,	3	);
	scu_pinmux(	5	,	0	,	2<<2	,	2	);
	scu_pinmux(	5	,	1	,	2<<2	,	2	);
	scu_pinmux(	5	,	2	,	2<<2	,	2	);
	scu_pinmux(	5	,	3	,	2<<2	,	2	);
	scu_pinmux(	5	,	4	,	2<<2	,	2	);
	scu_pinmux(	5	,	5	,	2<<2	,	2	);
	scu_pinmux(	5	,	6	,	2<<2	,	2	);
	scu_pinmux(	5	,	7	,	2<<2	,	2	);
	scu_pinmux(	6	,	3	,	2<<2	,	3	);
	scu_pinmux(	6	,	7	,	2<<2	,	1	);
	scu_pinmux(	6	,	8	,	2<<2	,	1	);
	scu_pinmux(	10	,	4	,	2<<2	,	3	);
	scu_pinmux(	13	,	2	,	2<<2	,	2	);
	scu_pinmux(	13	,	3	,	2<<2	,	2	);
	scu_pinmux(	13	,	4	,	2<<2	,	2	);
	scu_pinmux(	13	,	5	,	2<<2	,	2	);
	scu_pinmux(	13	,	6	,	2<<2	,	2	);
	scu_pinmux(	13	,	7	,	2<<2	,	2	);
	scu_pinmux(	13	,	8	,	2<<2	,	2	);
	scu_pinmux(	13	,	9	,	2<<2	,	2	);
	scu_pinmux(	13	,	12	,	2<<2	,	2	);
	scu_pinmux(	13	,	15	,	2<<2	,	2	);
	scu_pinmux(	13	,	16	,	2<<2	,	2	);
	scu_pinmux(	14	,	0	,	2<<2	,	3	);
	scu_pinmux(	14	,	1	,	2<<2	,	3	);
	scu_pinmux(	14	,	2	,	2<<2	,	3	);
	scu_pinmux(	14	,	3	,	2<<2	,	3	);
	scu_pinmux(	14	,	4	,	2<<2	,	3	);
	scu_pinmux(	14	,	5	,	2<<2	,	3	);
	scu_pinmux(	14	,	6	,	2<<2	,	3	);
	scu_pinmux(	14	,	7	,	2<<2	,	3	);
	scu_pinmux(	14	,	8	,	2<<2	,	3	);
	scu_pinmux(	14	,	9	,	2<<2	,	3	);
	scu_pinmux(	14	,	10	,	2<<2	,	3	);
	scu_pinmux(	14	,	11	,	2<<2	,	3	);
	scu_pinmux(	14	,	12	,	2<<2	,	3	);

	/* Set up EMC Controller */
	LPC_EMC->STATICWAITRD0 = 0x06;
	LPC_EMC->CONTROL = 0x01;
	LPC_EMC->STATICCONFIG0 = 0x81;
	LPC_EMC->STATICWAITOEN0 = 0x01;

    M32(0x40086400) = 3;
    M32(0x40086404) = 3;
    M32(0x40086408) = 3;
    M32(0x4008640C) = 3;
    M32(0x40086410) = 3;
    M32(0x40086414) = 3;
    M32(0x40086418) = 3;
    M32(0x4008641C) = 3;

    M8(0x400F0080) = 0xFF;
    M8(0x400F009C) = 0xFF;
  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  // Start Chip Erase Command
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(base_adr + (0x5555 << 1)) = 0x80;
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(base_adr + (0x5555 << 1)) = 0x10;

  return (Polling(base_adr));  // Wait until Erase completed
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  // Start Erase Sector Command
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(base_adr + (0x5555 << 1)) = 0x80;
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(adr) = 0x30;

  return (Polling(adr));       // Wait until Erase completed
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int i;

  for (i = 0; i < ((sz+1)/2); i++)  {
    // Start Program Command
    M16(base_adr + (0x5555 << 1)) = 0xAA;
    M16(base_adr + (0x2AAA << 1)) = 0x55;
    M16(base_adr + (0x5555 << 1)) = 0xA0;
    M16(adr) = *((unsigned short *) buf);
    if (Polling(adr) != 0) return (1);
    buf += 2;
    adr += 2;
  }
  return (0);
}

#endif
