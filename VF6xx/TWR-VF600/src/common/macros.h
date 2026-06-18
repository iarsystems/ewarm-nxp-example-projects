/*
 * Copyright (C) 2010, Freescale Semiconductor, Inc. All Rights Reserved
 * THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
 * BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
 * Freescale Semiconductor, Inc.
*/

/*!
 * @file functions.h
 * @brief This header file contains functions info.
 *
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
//#include "typedefs.h"
#include "common.h"

/***********************************************************************/
/*
 * The basic data types
 */
typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef char			    int8;   /*  8 bits */
typedef short int	        int16;  /* 16 bits */
typedef int		            int32;  /* 32 bits */

typedef volatile int8		vint8;  /*  8 bits */
typedef volatile int16		vint16; /* 16 bits */
typedef volatile int32		vint32; /* 32 bits */

typedef volatile uint8		vuint8;  /*  8 bits */
typedef volatile uint16		vuint16; /* 16 bits */
typedef volatile uint32		vuint32; /* 32 bits */

// function prototype for main function
int main(void);
/***********************************************************************/

/**************************************************************************
 *  Some Genaral Macros
 **************************************************************************/
#define FALSE 0
#define TRUE  1

#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7
#define BIT8 8
#define BIT9 9
#define BIT10 10
#define BIT11 11
#define BIT12 12
#define BIT13 13
#define BIT14 14
#define BIT15 15
#define BIT16 16
#define BIT17 17
#define BIT18 18
#define BIT19 19
#define BIT20 20
#define BIT21 21
#define BIT22 22
#define BIT23 23
#define BIT24 24
#define BIT25 25
#define BIT26 26
#define BIT27 27
#define BIT28 28
#define BIT29 29
#define BIT30 30
#define BIT31 31

#define PIN_MASK            0x1Fu
#define PIN(x)              (unsigned)(((1)<<(x & PIN_MASK)))

#define reg8_read(addr)       (*((vuint8_t *)(addr)))
#define reg16_read(addr)      (*((vuint16_t *)(addr)))
#define reg32_read(addr)      (*(vuint32_t *)((addr)))

#define reg8_write(addr,val)  (*((vuint8_t *)(addr)) = (val))
#define reg16_write(addr,val) (*((vuint16_t *)(addr)) = (val))
#define reg32_write(addr,val) (*((vuint32_t *)(addr)) = (val))

#define mem8_read(addr)       (*((vuint8_t *)(addr)))
#define mem16_read(addr)      (*((vuint16_t *)(addr)))
#define mem32_read(addr)      (*(vuint32_t *)((addr)))

#define mem8_write(addr,val)  (*((vuint8_t *)(addr)) = (val))
#define mem16_write(addr,val) (*((vuint16_t *)(addr)) = (val))
#define mem32_write(addr,val) (*((vuint32_t *)(addr)) = (val))

#define  reg8setbit(addr,bitpos) \
         reg8_write((addr),(reg8_read((addr)) | (1<<(bitpos))))

#define  reg16setbit(addr,bitpos) \
         reg16_write((addr),(reg16_read((addr)) | (1<<(bitpos))))

#define  reg32setbit(addr,bitpos) \
         reg32_write((addr),(reg32_read((addr)) | (1<<(bitpos))))

#define  reg8clrbit(addr,bitpos) \
         reg8_write((addr),(reg8_read((addr)) & (0xFF ^ (1<<(bitpos)))))

#define  reg16clrbit(addr,bitpos) \
         reg16_write((addr),(reg16_read((addr)) & (0xFFFF ^ (1<<(bitpos)))))

#define  reg32clrbit(addr,bitpos) \
         reg32_write((addr),(reg32_read((addr)) & (0xFFFFFFFF ^ (1<<(bitpos)))))

#define  reg8testbit(addr,bitpos) (((*((vuint8_t *)(addr))) & (1<<(bitpos))) >> bitpos)

#define  reg16testbit(addr,bitpos) (((*((vuint16_t *)(addr))) & (1<<(bitpos))) >> bitpos)

#define  reg32testbit(addr,bitpos) (((*((vuint32_t *)(addr))) & (1<<(bitpos))) >> bitpos)

#define byte_swap32(word) \
  (((word&0x000000FF) << 24)|			\
   ((word&0x0000FF00) <<  8)|			\
   ((word&0x00FF0000) >>  8)|			\
   ((word&0xFF000000) >> 24))

#define byte_swap16(word) \
  (((word&0x00FF) << 8) |			\
   ((word&0xFF00) >> 8) )

/**************************************************************************
 *  Write With Mask (write only certain bits)
 **************************************************************************/
void reg8_write_mask(uint32_t addr, uint8_t wdata, uint8_t mask);
void reg16_write_mask(uint32_t addr, uint16_t wdata, uint16_t mask);
void reg32_write_mask(uint32_t addr, uint32_t wdata, uint32_t mask);

#endif
