/***********************************************************************
* $Id: lpc23xx_i2c_driver.h 28 2014-01-27 14:45:13Z danielru $
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
#ifndef LPC23xx_I2C_DRIVER_H_
#define LPC23xx_I2C_DRIVER_H_

#include "lpc23xx_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif


/* I2C port number definition */
/* I2C port - controller 0 */
#define I2C_PORT_0		0x00
/* I2C port - controller 1 */
#define I2C_PORT_1		0x01
/* I2C port - controller 2 */
#define I2C_PORT_2		0x02

/* I2C Mode selection definition */
/* I2C in master mode */
#define I2C_MASTER_MODE		0x00
/* I2C in slave mode */
#define I2C_SLAVE_MODE		0x01

/* I2C status value definition */
/* I2C transmission is in progress*/
#define I2C_STATUS_BUSY		0x01
/* Last I2C transmission is done*/
#define I2C_STATUS_DONE		0x02
/* Last I2C transmission has error */
#define I2C_STATUS_ERROR	0x04

/***********************************************************************
 * I2C driver API functions
 **********************************************************************/
/* Init I2C controller */
S32 I2C_Init (U32 I2CPortNum, U32 I2CMode, U8 OwnSlvAdr, U32 ClkRate);

/* Config I2C controller */
S32 I2C_Config (S32 I2Cdev, U32 I2CMode, U8 OwnSlvAdr, U32 ClkRate);

/* Close I2C Device */
S32 I2C_Close (S32 I2Cdev);

/* Write a number of bytes in polling mode */
S32 I2C_WritePoll (S32 I2Cdev,
				U8 SlvAdr,
				void* buff,
				U32 length);

/* Read a number of bytes in polling mode */
S32 I2C_ReadPoll (S32 I2Cdev,
				U8 SlvAdr,
				void* buff,
				U32 length);

/* Write a number of bytes in asynchronous mode */
S32 I2C_WriteAsync(S32 I2Cdev,
				U8 SlvAdr,
				void* buff,
				U32 length);

/* Read a number of bytes in asynchronous mode */
S32 I2C_ReadAsync (S32 I2Cdev,
				U8 SlvAdr,
				void* buff,
				U32 length);

/* Get current status of I2C device operation */
U32 I2C_GetStatus (S32 I2Cdev);

/* Point Call back function for I2C device used in SYNC mode */
S32 I2C_SetCBS (S32 I2Cdev, void *pCBS);

/* i2c0 default interrupt subroutine */
void I2C0_ISR (void);

/* i2c1 default interrupt subroutine */
void I2C1_ISR(void);

/* i2c2 default interrupt subroutine */
void I2C2_ISR(void);


#ifdef __cplusplus
}
#endif

#endif /*LPC23xx_I2C_DRIVER_H_*/
