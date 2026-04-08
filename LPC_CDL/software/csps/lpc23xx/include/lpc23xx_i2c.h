/***********************************************************************
* $Id: lpc23xx_i2c.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx I2C controller definitions
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component:
*     I2C controller
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
#ifndef LPC23xx_I2C_H_
#define LPC23xx_I2C_H_

#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/**********************************************************************
* I2C controller register structures
**********************************************************************/
typedef struct{
	__IO U32 I2CONSET;		/* I2C control set register */
	__IO U32 I2STAT; 		/* I2C status register */
	__IO U32 I2DAT; 		/* I2C data register */
	__IO U32 I2ADR; 		/* I2C slave addr register */
	__IO U32 I2SCLH; 		/* I2C SCH duty cycle register high half word */
	__IO U32 I2SCLL; 		/* I2C SCH duty cycle register low half word */
	__IO U32 I2CONCLR; 		/* I2C control clear register */
}I2C_Type;

/**********************************************************************
 * I2C control regs definition
 *********************************************************************/
#define I2CON_AA		0x04 /* Assert acknowledge flag */
#define I2CON_SI		0x08 /* I2C interrupt flag */
#define I2CON_STO		0x10 /* STOP flag */
#define I2CON_STA		0x20 /* START flag */
#define I2CON_EN		0x40 /* I2C interface enable */

/**********************************************************************
 * I2C status regs definition
 *********************************************************************/
/* Master transmit mode */
/* A start condition has been transmitted */
#define I2STAT_MSTR_TRSMT_START			0x08 
/* A repeat start condition has been transmitted */
#define I2STAT_MSTR_TRSMT_RESTART		0x10
/* SLA+W has been transmitted, ACK has been received */
#define I2STAT_MSTR_TRSMT_SLAW_ACK		0x18
/* SLA+W has been transmitted, NACK has been received */
#define I2STAT_MSTR_TRSMT_SLAW_NACK		0x20
/* Data has been transmitted, ACK has been received */
#define I2STAT_MSTR_TRSMT_DAT_ACK		0x28
/* Data has been transmitted, NACK has been received */
#define I2STAT_MSTR_TRSMT_DAT_NACK		0x30
/* Arbitration lost in SLA+R/W or Data bytes */
#define I2STAT_MSTR_TRSMT_ARB_LOST		0x38

/* Master receive mode */
/* A start condition has been transmitted */
#define I2STAT_MSTR_RCV_START			0x08
/* A repeat start condition has been transmitted */
#define I2STAT_MSTR_RCV_RESTART			0x10
/* Arbitration lost */
#define I2STAT_MSTR_RCV_ARB_LOST		0x38
/* SLA+R has been transmitted, ACK has been received */
#define I2STAT_MSTR_RCV_SLAR_ACK		0x40
/* SLA+R has been transmitted, NACK has been received */
#define I2STAT_MSTR_RCV_SLAR_NACK		0x48
/* Data has been received, ACK has been returned */
#define I2STAT_MSTR_RCV_DAT_ACK			0x50
/*Data has been received, NACK has been return */
#define I2STAT_MSTR_RCV_DAT_NACK		0x58

/* Slave receive mode */
/* Own slave address has been received, ACK has been returned */
#define I2STAT_SLV_RCV_SLAW_ACK					0x60
/* Arbitration lost in SLA+R/W as master; 
 * Own SLA+W has been received, ACK returned */
#define I2STAT_SLV_RCV_ARB_LOST_SLAW_ACK		0x68
/* General call address has been received, ACK has been returned */
#define I2STAT_SLV_RCV_GENCALL_ACK				0x70
/* Arbitration lost in SLA+R/W as master; General call address
has been received, ACK has been returned*/
#define I2STAT_SLV_RCV_ARB_LOST_GENCALL_ACK		0x78
/* Data has been received, ACK has been return */
#define I2STAT_SLV_RCV_PRE_SLA_DAT_ACK			0x80
/* Data has been received and NOT ACK has been return */
#define I2STAT_SLV_RCV_PRE_SLA_DAT_NACK			0x88
/* Data has been received and ACK has been return
 * as general call address */
#define I2STAT_SLV_RCV_PRE_GENCALL_DAT_ACK		0x90
/* Data has been received and NOT ACK has been return
 * as general call address */
#define I2STAT_SLV_RCV_PRE_GENCALL_DAT_NACK		0x98
/* A stop condition has been received */
#define I2STAT_SLV_RCV_STASTO_SLVREC_SLVTRX		0xA0

/* Slave transmit mode */
/* Own SLA+R has been received, ACK has been returned */
#define I2STAT_SLV_TRSMT_SLAR_ACK				0xA8
/* Arbitration lost in SLA+R/W as master;
Own SLA+R has been received, ACK has been returned */
#define I2STAT_SLV_TRSMT_ARB_LOST_SLAR_ACK		0xB0
/* Data has been transmitted, ACK has been received */
#define I2STAT_SLV_TRSMT_DAT_ACK				0xB8
/* Data has been transmitted, NACK has been received */	
#define I2STAT_SLV_TRSMT_DAT_NACK				0xC0
/* Last data byte in I2DAT has been transmitted (AA = 0);
ACK has been received */
#define I2STAT_SLV_TRSMT_LAST_DAT_ACK			0xC8

/**********************************************************************
 * Macro pointing to I2C register 
 *********************************************************************/
/* Macro pointing to I2C0 register */
#define I2C0	((I2C_Type *)(I2C0_BASE))
/* Macro pointing to I2C1 register */
#define I2C1	((I2C_Type *)(I2C1_BASE))
/* Macro pointing to I2C2 register */
#define I2C2	((I2C_Type *)(I2C2_BASE))

#ifdef __cplusplus
}
#endif

#endif /*LPC23xx_I2C_H_*/
