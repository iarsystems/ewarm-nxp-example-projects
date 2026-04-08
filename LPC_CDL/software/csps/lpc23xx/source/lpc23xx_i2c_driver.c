/***********************************************************************
 * $Id: lpc23xx_i2c_driver.c 28 2014-01-27 14:45:13Z danielru $
 * 
 * Project: LPC23xx I2C driver
 *
 * Description:
 *     This file contains driver support for the I2C modules on the
 *     LPC23xx
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
#include "lpc23xx_i2c_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_pinsel_driver.h"

/***********************************************************************
 * Private I2C data type and functions
***********************************************************************/

/***********************************************************************
 * I2C device configuration structure type
***********************************************************************/
typedef struct
{
	I2C_Type  	*regptr;    			/* Pointer to I2C registers */
	U8			slaveaddr;				/* current slave address used in master mode */
	BOOL32		mastermode;				/* Mode in use, 0 = slave, 1 = master */
    U8*			datptr;					/* Data pointer */
    U32			length;					/* Length of current data */
    U32			idx;					/* Current index data */
    void		(*handler)(S32 I2Cdev); /* Transmission interrupt handler pointer*/
    void		(*cbs)(void);			/* Call back function pointer */
    BOOL32     	init;       			/* Device initialized flag */
    U32			status;					/* Current status of device on I2C bus */
} I2C_CFG_T;

/* I2C driver data in structure for I2C0, I2C1 and I2C2 controller */
STATIC I2C_CFG_T i2ccfgdat[3] = {
	{(I2C_Type *) 0, 0, TRUE, NULL, 0, 0, NULL, NULL, FALSE, 0},
	{(I2C_Type *) 0, 0, TRUE, NULL, 0, 0, NULL, NULL, FALSE, 0},
	{(I2C_Type *) 0, 0, TRUE, NULL, 0, 0, NULL, NULL, FALSE, 0}
	};

/***********************************************************************
 * Private I2C function declaration
***********************************************************************/

/* Generate a start condition on I2C bus (in master mode only) */
U32 I2C_Start (I2C_Type *i2cdev);

/* Generate a stop condition on I2C bus (in master mode only) */
void I2C_Stop (I2C_Type *i2cdev);

/* I2C send byte subroutine */
U32 I2C_SendByte (I2C_Type *i2cdev, U8 databyte);

/* I2C get byte subroutine */
U32 I2C_GetByte (I2C_Type *i2cdev, U8 *retdat, BOOL32 ack);

/* I2C interrupt master transmit handler */ 
void I2C_MTXHandler (S32 I2Cdev);

/* I2C interrupt master receive handler */ 
void I2C_MRXHandler(S32 I2Cdev);

/* I2C interrupt slave transmit handler */ 
void I2C_STXHandler(S32 I2Cdev);

/* I2C interrupt slave receive handler */ 
void I2C_SRXHandler(S32 I2Cdev);

/* Enable interrupt for I2C device */
void I2C_EnableInt (S32 I2Cdev);

/* Disable interrupt for I2C device */
void I2C_DisableInt (S32 I2Cdev);


/***********************************************************************
 * Function: I2C_Start
 * Purpose: Generate a start condition on I2C bus (in master mode only)
 * Parameters:
 *     i2cdev: Pointer to I2C register
 * Returns: value of I2C status register after generate a start condition
 **********************************************************************/
U32 I2C_Start (I2C_Type *i2cdev)
{
	i2cdev->I2CONCLR = I2CON_SI;
	i2cdev->I2CONSET = I2CON_STA;
	while (!(i2cdev->I2CONSET & I2CON_SI));
	i2cdev->I2CONCLR = I2CON_STA;
	return i2cdev->I2STAT;
}


/***********************************************************************
 * Function: I2C_Stop
 * Purpose: Generate a stop condition on I2C bus (in master mode only)
 * Parameters:
 *     i2cdev: Pointer to I2C register
 * Returns: None
 **********************************************************************/
void I2C_Stop (I2C_Type * i2cdev)
{
	i2cdev->I2CONSET = I2CON_STO;
	i2cdev->I2CONCLR = I2CON_SI;
}


/***********************************************************************
 * Function: I2C_SendByte
 * Purpose: Send a byte
 * Parameters:
 *     i2cdev: Pointer to I2C register
 * Returns: value of I2C status register after sending
 **********************************************************************/
U32 I2C_SendByte (I2C_Type *i2cdev, U8 databyte)
{
	i2cdev->I2DAT = (U32) databyte;
	i2cdev->I2CONCLR = I2CON_SI;
	while (!(i2cdev->I2CONSET & I2CON_SI));
	return i2cdev->I2STAT;
}


/***********************************************************************
 * Function: I2C_GetByte
 * Purpose: Get a byte
 * Parameters:
 *     i2cdev: Pointer to I2C register
 * Returns: value of I2C status register after receiving
 **********************************************************************/
U32 I2C_GetByte (I2C_Type *i2cdev, U8 *retdat, BOOL32 ack)
{
	if (ack == TRUE)
	{
		i2cdev->I2CONSET = I2CON_AA;		
	}
	else 
	{
		i2cdev->I2CONCLR = I2CON_AA;
	}
	i2cdev->I2CONCLR = I2CON_SI;
	while (!(i2cdev->I2CONSET & I2CON_SI));
	*retdat = (U8) i2cdev->I2DAT;
	return i2cdev->I2STAT;
}


/***********************************************************************
 * Function: I2C_MTXHandler
 * Purpose: I2C interrupt master transmit handler
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * Returns: None
 **********************************************************************/
void I2C_MTXHandler (S32 I2Cdev)
{
    I2C_CFG_T *device = (I2C_CFG_T *) I2Cdev;
    
    // Handle next operation according to current I2S status
    switch (device->regptr->I2STAT)
    {
    /* A start condition has been transmitted */
	case I2STAT_MSTR_TRSMT_START:
		/*Busy = 1, Done = 0*/
		/*Send slave addr + RD bit*/
		device->status = I2C_STATUS_BUSY | I2STAT_MSTR_TRSMT_START;
		device->regptr->I2CONCLR = I2CON_STA;
		device->regptr->I2DAT = (U32)(device->slaveaddr);
		device->regptr->I2CONCLR = I2CON_SI;
		break;
		
	/* A repeat start condition has been transmitted */
	case I2STAT_MSTR_TRSMT_RESTART:
		/*Busy = 1, Done = 0*/
		/*Send slave addr + RD bit*/
		device->status = I2C_STATUS_BUSY | I2STAT_MSTR_TRSMT_RESTART;
		device->regptr->I2CONCLR = I2CON_STA;
		device->regptr->I2DAT = (U32)(device->slaveaddr);
		device->regptr->I2CONCLR = I2CON_SI;
		break;
		
	/* SLA+W has been transmitted, ACK has been received */
	case I2STAT_MSTR_TRSMT_SLAW_ACK:
		if (device->idx < device->length)
		{
			/*Busy = 1, Done = 0*/
			/*Send more data*/
			device->status = I2C_STATUS_BUSY | I2STAT_MSTR_TRSMT_SLAW_ACK;
			device->regptr->I2DAT = (U32) *(device->datptr + device->idx);
			device->idx++;
			device->regptr->I2CONCLR = I2CON_SI;
		}	
		else
		{
			/*Busy = 0, Done = 1*/
			/*Last data has been sent, STOP will be generated*/
			device->status = I2C_STATUS_DONE | I2STAT_MSTR_TRSMT_SLAW_ACK;
			device->regptr->I2CONSET = I2CON_STO;
			device->regptr->I2CONCLR = I2CON_SI;
			/* Call callback function */
			device->cbs();
			I2C_DisableInt (I2Cdev);
		}
		break;
		
	/* SLA+W has been transmitted, NACK has been received */
	case I2STAT_MSTR_TRSMT_SLAW_NACK:
		/*Busy = 0, Done = 0*/
		/*STOP will be generated*/
		device->status = I2STAT_MSTR_TRSMT_SLAW_NACK | I2C_STATUS_ERROR;
		device->regptr->I2CONSET = I2CON_STO;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		I2C_DisableInt (I2Cdev);
		break;
		
	/* Data has been transmitted, ACK has been received */
	case I2STAT_MSTR_TRSMT_DAT_ACK:
		if (device->idx < device->length)
		{
			/*Busy = 1, Done = 0*/
			/*Send more data*/
			device->status = I2C_STATUS_BUSY | I2STAT_MSTR_TRSMT_DAT_ACK;
			device->regptr->I2DAT = (U32) *(device->datptr + device->idx);
			device->idx++;
			device->regptr->I2CONCLR = I2CON_SI;
		}	
		else
		{
			/*Busy = 0, Done = 1*/
			/*Last data has been sent, STOP will be generated*/
			device->status = I2C_STATUS_DONE | I2STAT_MSTR_TRSMT_DAT_ACK;
			device->regptr->I2CONSET = I2CON_STO;
			device->regptr->I2CONCLR = I2CON_SI;
			/* Call callback function */
			device->cbs();
			I2C_DisableInt (I2Cdev);
		}
		break;
		
	/* Data has been transmitted, NACK has been received */
	case I2STAT_MSTR_TRSMT_DAT_NACK:
		/*Busy = 0, Done = 0*/
		/*NACK has been received, STOP will be generated*/
		device->status = I2STAT_MSTR_TRSMT_DAT_NACK | I2C_STATUS_ERROR;
		device->regptr->I2CONSET = I2CON_STO;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		I2C_DisableInt (I2Cdev);
		break;
		
	/* Arbitration lost in SLA+R/W or Data bytes */
	case I2STAT_MSTR_TRSMT_ARB_LOST:
		/*Busy = 0, Done = 0, ARBF = 1*/
		/*Release I2C bus, enter not addressed slave mode*/
		device->status = I2STAT_MSTR_TRSMT_ARB_LOST | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
		
	// Other status must be captured
	default:
		/* Else... */
		device->status = device->regptr->I2STAT | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		I2C_DisableInt (I2Cdev);
		break;
    }
    return;
}


/***********************************************************************
 * Function: I2C_MRXHandler
 * Purpose: I2C interrupt master receive handler
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * Returns: None
 **********************************************************************/
void I2C_MRXHandler(S32 I2Cdev)
{
	I2C_CFG_T *device = (I2C_CFG_T *) I2Cdev;

	// Handle next operation according to current I2S status
    switch (device->regptr->I2STAT)
    {
    /* A start condition has been transmitted */
    case I2STAT_MSTR_RCV_START:
		/*Busy = 1, Done = 0*/
		/*Send slave addr + WR bit*/
		device->status = I2C_STATUS_BUSY | I2STAT_MSTR_RCV_START;
		device->regptr->I2DAT = device->slaveaddr | 0x01;
		device->regptr->I2CONCLR = I2CON_STA | I2CON_SI;
		break;
		
	/* A repeat start condition has been transmitted */
	case I2STAT_MSTR_RCV_RESTART:
		/*Busy = 1, Done = 0*/
		/*Send slave addr + WR bit*/
		device->status = I2C_STATUS_BUSY | I2STAT_MSTR_RCV_RESTART;
		device->regptr->I2DAT = device->slaveaddr | 0x01;
		device->regptr->I2CONCLR = I2CON_STA | I2CON_SI;		
		break;
		
	/* Arbitration lost */
	case I2STAT_MSTR_RCV_ARB_LOST:
		/*Arbitration lost, enter not addressed slave mode*/
		/*Busy = 0, Done = 0*/
		device->status = I2STAT_MSTR_RCV_ARB_LOST | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = I2CON_STA | I2CON_SI;
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
		
	/* SLA+R has been transmitted, ACK has been received */
	case I2STAT_MSTR_RCV_SLAR_ACK:
		if ((device->idx) < (device->length - 1))
		{
			/*Data will be received,  ACK will be return*/
			device->regptr->I2CONSET = I2CON_AA;
		}
		else
		{
			/*Last data will be received,  NACK will be return*/
			device->regptr->I2CONCLR = I2CON_AA;
		}
		/*Busy = 1, Done = 0*/
		device->status = I2C_STATUS_BUSY | I2STAT_MSTR_RCV_SLAR_ACK;
		device->regptr->I2CONCLR = I2CON_SI;
		break;
		
	/* SLA+R has been transmitted, NACK has been received */
	case I2STAT_MSTR_RCV_SLAR_NACK:
		/*Busy = 0, Done = 0*/
		device->status = I2STAT_MSTR_RCV_SLAR_NACK | I2C_STATUS_ERROR;
		/*STOP will be generated*/
		device->regptr->I2CONSET = I2CON_STO;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		I2C_DisableInt (I2Cdev);
		break;
		
	/* Data has been received, ACK has been returned */
	case I2STAT_MSTR_RCV_DAT_ACK:
		/* Save data from bufreg */
		*(device->datptr + device->idx) = (U8) device->regptr->I2DAT;
		device->idx++;
		if ((device->idx) < (device->length - 1))
		{
			/*Data will be received,  ACK will be return*/
			device->regptr->I2CONSET = I2CON_AA;
		}
		else
		{
			/*Last data will be received,  NACK will be return*/
			device->regptr->I2CONCLR = I2CON_AA;
		}
		/*Busy = 1, Done = 0*/
		device->status = I2C_STATUS_BUSY | I2STAT_MSTR_RCV_DAT_ACK;
		device->regptr->I2CONCLR = I2CON_SI;
		break;
		
	/*Data has been received, NACK has been return */
	case I2STAT_MSTR_RCV_DAT_NACK:
		/* Save data from bufreg */
		*(device->datptr + device->idx) = (U8) device->regptr->I2DAT;
		/*Busy = 0, Done = 1*/
		device->status = I2C_STATUS_DONE | I2STAT_MSTR_RCV_DAT_NACK;
		/* STOP will be generated */
		device->regptr->I2CONSET = I2CON_STO;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		I2C_DisableInt (I2Cdev);
		break;
	
	// Other status must be captured
	default:
		/* Else... */
		device->status = device->regptr->I2STAT | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = I2CON_SI;
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
    }
}


/***********************************************************************
 * Function: I2C_STXHandler
 * Purpose: I2C interrupt slave transmit handler
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * Returns: None
 **********************************************************************/
void I2C_STXHandler(S32 I2Cdev)
{
	I2C_CFG_T *device = (I2C_CFG_T *) I2Cdev;
	
	switch (device->regptr->I2STAT)
	{
	/* Own SLA+R has been received, ACK has been returned */
	case I2STAT_SLV_TRSMT_SLAR_ACK:	
		/* BUSY = 1, DONE = 0 */
		device->status = I2C_STATUS_BUSY | I2STAT_SLV_TRSMT_SLAR_ACK;
		if (device->idx < device->length)
		{
			device->regptr->I2DAT = (U32) *(device->datptr + device->idx);
			device->idx++;
		}
		device->regptr->I2CONSET = I2CON_AA;
		device->regptr->I2CONCLR = I2CON_SI;
		break;	 
	
	/* Data has been transmitted, ACK has been received */
	case I2STAT_SLV_TRSMT_DAT_ACK:
		/* BUSY = 1, DONE = 0 */
		device->status = I2C_STATUS_BUSY | I2STAT_SLV_TRSMT_DAT_ACK;
		if (device->idx < device->length)
		{
			device->regptr->I2DAT = (U32) *(device->datptr + device->idx);
			device->idx++;
		}	    			
		device->regptr->I2CONSET = I2CON_AA;
		device->regptr->I2CONCLR = I2CON_SI;
		break;	
		
	/* Data has been transmitted, NACK has been received */	
	case I2STAT_SLV_TRSMT_DAT_NACK:
		/* BUSY = 0, DONE = 1 */
		device->status = I2C_STATUS_DONE | I2STAT_SLV_TRSMT_DAT_NACK;
		device->regptr->I2CONCLR = (I2CON_SI | I2CON_AA);
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
		
	/* Else... */	
	default:		
		/* BUSY = 0, DONE = 0 */
		device->status = device->regptr->I2STAT | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = (I2CON_SI | I2CON_AA);
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
	}
}


/***********************************************************************
 * Function: I2C_SRXHandler
 * Purpose: I2C interrupt slave receive handler
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * Returns: None
 **********************************************************************/
void I2C_SRXHandler(S32 I2Cdev)
{
	I2C_CFG_T *device = (I2C_CFG_T *) I2Cdev;
	
	switch (device->regptr->I2STAT)
	{
	/* Own slave address has been received, ACK has been returned */
	case I2STAT_SLV_RCV_SLAW_ACK:
		/* BUSY = 1, DONE = 0 */
		device->status = I2C_STATUS_BUSY | I2STAT_SLV_RCV_SLAW_ACK;
		device->regptr->I2CONSET = I2CON_AA;
		device->regptr->I2CONCLR = I2CON_SI;
		break;
	
	/* Data has been received, ACK has been return */
	case I2STAT_SLV_RCV_PRE_SLA_DAT_ACK: 
		/* BUSY = 1, DONE = 0 */
		device->status = I2C_STATUS_BUSY | I2STAT_SLV_RCV_PRE_SLA_DAT_ACK;
		if (device->idx < device->length)
		{
			*(device->datptr + device->idx) = (U8) device->regptr->I2DAT;
			device->idx++;
		}
		/* if received data length is greater than specified length
		 * all next data will be lost */ 
		device->regptr->I2CONSET = I2CON_AA;
		device->regptr->I2CONCLR = I2CON_SI;
		break;
		
	/* Data has been received and NOT ACK has been return */
	case I2STAT_SLV_RCV_PRE_SLA_DAT_NACK:
		/* BUSY = 0, DONE = 0 */
		device->status = I2STAT_SLV_RCV_PRE_SLA_DAT_NACK | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = (I2CON_SI | I2CON_AA);
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;	 		

	/* General call address has been received, ACK has been returned */
	case I2STAT_SLV_RCV_GENCALL_ACK:
		/* BUSY = 1, DONE = 0 */
		device->status = I2C_STATUS_BUSY | I2STAT_SLV_RCV_GENCALL_ACK;
		device->regptr->I2CONSET = I2CON_AA;
		device->regptr->I2CONCLR = I2CON_SI;
		break;
	
	/* Data has been received and ACK has been return
	 * as general call address */
	case I2STAT_SLV_RCV_PRE_GENCALL_DAT_ACK:
		/* BUSY = 1, DONE = 0 */
		device->status = I2C_STATUS_BUSY | I2STAT_SLV_RCV_PRE_GENCALL_DAT_ACK;
		if (device->idx < device->length)
		{
			*(device->datptr + device->idx) = (U8) device->regptr->I2DAT;
			device->idx++;
		}
		/* if received data length is greater than specified length
		 * all next data will be lost */ 
		device->regptr->I2CONSET = I2CON_AA;
		device->regptr->I2CONCLR = I2CON_SI;
		break;
		
	/* Data has been received and NOT ACK has been return
	 * as general call address */
	case I2STAT_SLV_RCV_PRE_GENCALL_DAT_NACK:
		/* BUSY = 0, DONE = 0 */
		device->status = I2STAT_SLV_RCV_PRE_GENCALL_DAT_NACK | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = (I2CON_SI | I2CON_AA);
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
	
	/* A stop condition has been received */
	case I2STAT_SLV_RCV_STASTO_SLVREC_SLVTRX:
		/* BUSY = 0, DONE = 1 */
		device->status = I2C_STATUS_DONE | I2STAT_SLV_RCV_STASTO_SLVREC_SLVTRX;
		device->regptr->I2CONCLR = (I2CON_SI | I2CON_AA);
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;
	
		
	// Other status must be captured
	default:
		/* Else... */
		/* BUSY = 0, DONE = 0 */
		device->status = device->regptr->I2STAT | I2C_STATUS_ERROR;
		device->regptr->I2CONCLR = (I2CON_SI | I2CON_AA);
		/* Call callback function */
		device->cbs();
		/* Disable interrupt */
		I2C_DisableInt (I2Cdev);
		break;	  
	}
}


/***********************************************************************
 * Function: I2C_EnableInt
 * Purpose: Enable interrupt for I2C device
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * Returns: None
 **********************************************************************/
void I2C_EnableInt (S32 I2Cdev)
{	
	if(I2Cdev == (S32) (&i2ccfgdat[0]))
	{
		VIC_EnableInt (I2C0_INT);
	}
	else if (I2Cdev == (S32) (&i2ccfgdat[1]))
	{
		VIC_EnableInt (I2C1_INT);
	}
	else if (I2Cdev == (S32) (&i2ccfgdat[2]))
	{
		VIC_EnableInt (I2C2_INT);
	}
    return;
}


/***********************************************************************
 * Function: I2C_DisableInt
 * Purpose: Disable interrupt for I2C device
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * Returns: None
 **********************************************************************/
void I2C_DisableInt (S32 I2Cdev)
{	
	if (I2Cdev == (S32) (&i2ccfgdat[0]))
	{
		VIC_DisableInt (I2C0_INT);
	}
	else if (I2Cdev == (S32) (&i2ccfgdat[1]))
	{
		VIC_DisableInt (I2C1_INT);
	}
	else if (I2Cdev == (S32) (&i2ccfgdat[2]))
	{
		VIC_DisableInt (I2C2_INT);
	}
}


/**********************************************************************
 * I2C driver public functions
 *********************************************************************/

/***********************************************************************
 * Function: I2C_Init
 * Purpose: Init I2C device 
 * Parameters:
 *     I2CPortNum: I2C port number (0, 1 or 2)
 *     I2CMode: I2C mode (0 or 1)
 * 	   OwnSlvAdr: Own I2C slave address in SLAVE mode 
 * (8 bit address ORed with General Call enable bit - 0x01)
 *	   ClkRate: I2C Clock rate (Hz)
 * Returns: The pointer to a I2C config structure or NULL
 **********************************************************************/
S32 I2C_Init (U32 I2CPortNum, U32 I2CMode, U8 OwnSlvAdr, U32 ClkRate)
{
	I2C_CFG_T *i2ccfgptr;
	
	// I2C numport selection 
	if ((I2CPortNum >= 0) && (I2CPortNum <= 2))
	{
		i2ccfgptr = &i2ccfgdat[I2CPortNum];
	}
	else
	{
		return 0;
	}

    /* has the selected I2C been previously initialized? */
    if (i2ccfgptr->init == FALSE)
    {
        /* Device not initialized and it is usable, so set it to
           used */
    	i2ccfgptr->init = TRUE;
        
    	 /* Save address of register block */
        switch (I2CPortNum)
        {
        case I2C_PORT_0:
        	i2ccfgptr->regptr = I2C0;
        	// Enable power
        	SCB_EnablePPWR (PCI2C0);
        	// Set peripheral clock divider to 2 from CCLK
        	SCB_SetPCLKDiv (PCLK_I2C0, CCLK_DIV_2);
        	// Enable function pins according
//        	PINSEL_Set (PORT_0, 27, FUNC_1);
//        	PINSEL_Set (PORT_0, 28, FUNC_1);
        	break;
        	
        case I2C_PORT_1:
        	i2ccfgptr->regptr = I2C1;
        	// Enable power
	    	SCB_EnablePPWR (PCI2C1);
	    	// Set peripheral clock divider to 2 from CCLK
	    	SCB_SetPCLKDiv (PCLK_I2C1, CCLK_DIV_2);
        	// Enable function pins according
//        	PINSEL_Set (PORT_0, 0, FUNC_3);
//        	PINSEL_Set (PORT_0, 1, FUNC_3);
        	break;
        	
        case I2C_PORT_2:
        	i2ccfgptr->regptr = I2C2;
        	// Enable power
        	SCB_EnablePPWR (PCI2C2);
        	// Set peripheral clock divider to 2 from CCLK
        	SCB_SetPCLKDiv (PCLK_I2C2, CCLK_DIV_2);
        	// Enable function pins according
//        	PINSEL_Set (PORT_0, 10, FUNC_2);
//        	PINSEL_Set (PORT_0, 11, FUNC_2);
        	break;
        }

        if (I2C_Config ((S32) i2ccfgptr, I2CMode, OwnSlvAdr, ClkRate) 
        		== _ERROR)
        {
        	return 0;
        }

        // Set I2C operation to default
        i2ccfgptr->regptr->I2CONCLR = (I2CON_AA | I2CON_STA | 
									I2CON_STO | I2CON_SI | I2CON_EN);
        i2ccfgptr->regptr->I2CONSET = I2CON_EN;
        
        // Set Handler and CBS function used in ASYNC mode to NULL
        i2ccfgptr->handler = NULL;
        i2ccfgptr->cbs = NULL;
    }
    // The device has been already init 
    else
    {
    	return 0;
    }
    
    return (S32) i2ccfgptr;
}


/***********************************************************************
 * Function: I2C_Config
 * Purpose: Init I2C device 
 * Parameters:
 *     I2Cdev: Pointer to I2C device 
 *     I2CMode: I2C mode (0 or 1)
 * 	   OwnSlvAdr: Own I2C slave address in SLAVE mode 
 * (8 bit address ORed with General Call enable bit - 0x01)
 *	   ClkRate: I2C Clock rate (Hz)
 * Returns: Status of Progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 I2C_Config (S32 I2Cdev, U32 I2CMode, U8 OwnSlvAdr, U32 ClkRate)
{
	I2C_CFG_T *i2ccfgptr = (I2C_CFG_T *) I2Cdev;
	U32 temp;
	
	if (i2ccfgptr->init == FALSE)
	{
		return _ERROR;
	}
	
	// Set I2C mode with its address if device is in slave mode
	switch (I2CMode)
	{
	case I2C_MASTER_MODE:
		i2ccfgptr->mastermode = TRUE;
		break;
		
	case I2C_SLAVE_MODE:
		i2ccfgptr->mastermode = FALSE;
		i2ccfgptr->regptr->I2ADR = (U32) OwnSlvAdr;
		break;
		
	default:
		return _ERROR;
		break;
	}
	
	// Get PCLK of I2C controller
	if (I2Cdev == (S32) (&i2ccfgdat[0]))
	{
		temp = (SCB_GetPCLK (PCLK_I2C0)) / ClkRate;
	}
	else if (I2Cdev == (S32) (&i2ccfgdat[1]))
	{
		temp = (SCB_GetPCLK (PCLK_I2C1)) / ClkRate;
	}
	else if (I2Cdev == (S32) (&i2ccfgdat[2]))
	{
		temp = (SCB_GetPCLK (PCLK_I2C2)) / ClkRate;
	}
	else
	{
		return _ERROR;
	}
		
	/* Set the I2C clock value to register */
	i2ccfgptr->regptr->I2SCLH = (U32)(temp / 2);
	i2ccfgptr->regptr->I2SCLL = (U32)(temp - i2ccfgptr->regptr->I2SCLH);
	
	return _NO_ERROR;
}

/***********************************************************************
 * Function: I2C_Close
 * Purpose: Close the I2C device 
 * Parameters:
 *     I2Cdev: Pointer to I2C device
 * Returns: Status of Progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 I2C_Close (S32 I2Cdev)
{
    I2C_CFG_T *i2ccfgptr = (I2C_CFG_T *) I2Cdev;
    
    if (i2ccfgptr->init == TRUE)
	{
    	if (I2Cdev == (S32) (&i2ccfgdat[0]))
    	{
	    	// Turn-off power
	    	SCB_DisablePPWR (PCI2C0);
    	}
    	else if (I2Cdev == (S32) (&i2ccfgdat[1]))
    	{
	    	// Turn-off power
	    	SCB_DisablePPWR (PCI2C1);
    	}
    	else if (I2Cdev == (S32) (&i2ccfgdat[2]))
    	{
	    	// Turn-off power
	    	SCB_DisablePPWR (PCI2C2);
    	}
    	else
    	{
    		return _ERROR;
    	}
    	
        /* Set I2C as uninitialized */
    	i2ccfgptr->init = FALSE;
        
        /* disable I2C interrupt */
    	I2C_DisableInt(I2Cdev);
	    
	    /* Disable device */
	    i2ccfgptr->regptr->I2CONCLR = (I2CON_AA | I2CON_STA | 
				I2CON_STO | I2CON_SI | I2CON_EN);
	}

    return _NO_ERROR;
}


/***********************************************************************
 * Function: I2C_WritePoll
 * Purpose: Write a number of bytes in polling mode
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * 		SlvAdr: I2C slave address to send (available in master)
 * 		buff: pointer to buffer data.
 * 		length: length of data to be sent (in byte) 
 * Returns: count of actual data sent
 **********************************************************************/
S32 I2C_WritePoll (S32 I2Cdev,
				U8 SlvAdr,
				void* buff,
				U32 length)
{
	I2C_CFG_T* device = (I2C_CFG_T *) I2Cdev;
	I2C_Type* i2cregs = device->regptr;
	U8* dataptr = (U8 *) buff;
	U32 count = 0;
	U32 errcode = 0;
	S32 volatile exit_flag = 0;
	
	if (device->init == TRUE)
	{
		/* if master mode selected */
		if (device->mastermode == TRUE)
		{
			/* Start condition */
			if ((errcode = I2C_Start(i2cregs)) 
					!= I2STAT_MSTR_TRSMT_START)
	    	{
				// Save current I2C operation status
				device->status = errcode | I2C_STATUS_ERROR;
	    		goto exit_0;
	    	}
			
			/* Send slave address + WR direction bit = 0 */
	    	if ((errcode = I2C_SendByte(i2cregs, SlvAdr))
	    			!= I2STAT_MSTR_TRSMT_SLAW_ACK)
	    	{
				// Save current I2C operation status
				device->status = errcode | I2C_STATUS_ERROR;
	    		goto exit_0;
	    	}
	    	
	    	/* Send a number of data bytes */
	    	while (count < length)
	    	{
	    		if ((errcode = I2C_SendByte(i2cregs, *dataptr))
	    				!= I2STAT_MSTR_TRSMT_DAT_ACK)
	    		{
	    			// Save current I2C operation status
	    			device->status = errcode | I2C_STATUS_ERROR;
	    			break;
	    		}
	    		dataptr++;
	    		count++;
	    	}
	    	
	    	if (count == length)
	    	{
	    		// I2C Operation done!
	    		device->status = I2C_STATUS_DONE;
	    	}
	    	
	    	exit_0:
	    	/* Stop condition */
	    	I2C_Stop (i2cregs);
		}
		
		/* if slave mode selected */
		else
		{
			/* Set AA bit to ack command on I2C bus */
			i2cregs->I2CONSET = I2CON_AA;
			/* Clear Si bit to be ready ... */
			i2cregs->I2CONCLR = (I2CON_SI | I2CON_STA | I2CON_STO);
			
			/* Check for exit flag == 1 */
			while (exit_flag == 0)
			{
				/* Check SI flag ready */
				if (i2cregs->I2CONSET & I2CON_SI)
				{					
					switch (i2cregs->I2STAT)
					{
					/* Own SLA+R has been received, ACK has been returned */
					case I2STAT_SLV_TRSMT_SLAR_ACK:	
		    			if (count < length)
		    			{
		    				i2cregs->I2DAT = (U32) *dataptr;
			    			dataptr++;
			    			count++;
		    			}	    			
		    			i2cregs->I2CONSET = I2CON_AA;
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    			
		    		/* Data has been transmitted, ACK has been received */
		    		case I2STAT_SLV_TRSMT_DAT_ACK:
		    			if (count < length)
		    			{
		    				i2cregs->I2DAT = (U32) *dataptr;	
		    				dataptr++;
		    				count++;
		    			}    				
		    			i2cregs->I2CONSET = I2CON_AA;
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    			
		    		/* Data has been transmitted, NACK has been received */
		    		case I2STAT_SLV_TRSMT_DAT_NACK:
		    			// Save current I2C status 
		    			device->status = i2cregs->I2STAT | I2C_STATUS_DONE;
		    			i2cregs->I2CONSET = I2CON_AA;
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			exit_flag = 1;
		    			break;
		    			
		    		// Other status must be captured
		    		default:
		    			// Save current I2C operation status
		    			device->status = i2cregs->I2STAT | I2C_STATUS_ERROR;
		    			// Exit loop
		    			exit_flag = 1;
		    			break;
					}
				}
			}
			
			/* Clear AA bit to disable ack on I2C bus */
			i2cregs->I2CONCLR = I2CON_AA;
		}
	}
	
	return (S32) count;
}


/***********************************************************************
 * Function: I2C_WriteAsync
 * Purpose: Write a number of bytes in ASYNC (interrupt) mode
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * 		SlvAdr: I2C slave address to send (available in master)
 * 		buff: pointer to buffer data.
 * 		length: length of data to be sent (in byte) 
 * Returns: status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 I2C_WriteAsync (S32 I2Cdev,
					U8 SlvAdr,
					void* buff,
					U32 length)
{
	I2C_CFG_T* device = (I2C_CFG_T *) I2Cdev;
	I2C_Type* i2cregs = device->regptr;
	
	if (device->init == TRUE)
	{		
		device->datptr = (U8 *) buff;
		device->idx = 0;
		device->length = length;
		
		/* if master mode selected */
		if (device->mastermode == TRUE)
		{
			device->slaveaddr = SlvAdr;
			device->handler = I2C_MTXHandler;
			device->status = I2C_STATUS_BUSY;
			i2cregs->I2CONCLR = (I2CON_AA | I2CON_SI | I2CON_STO);
			i2cregs->I2CONSET = I2CON_STA;
		}
		/* if slave mode selected */
		else
		{
			device->handler = I2C_STXHandler;
			device->status = I2C_STATUS_BUSY;
			i2cregs->I2CONCLR = (I2CON_STA | I2CON_SI | I2CON_STO);
			i2cregs->I2CONSET = I2CON_AA;
		}
		
		/* Enable I2C interrupt for next necessary step */
		I2C_EnableInt (I2Cdev);
	}
	else
	{
		return _ERROR;
	}
	
	return _NO_ERROR;
}

/***********************************************************************
 * Function: I2C_ReadPoll
 * Purpose: Read a number of bytes in polling mode
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * 		SlvAdr: I2C slave address to send (available in master)
 * 		buff: pointer to buffer data.
 * 		length: length of data to be read (in byte) 
 * Returns: count of actual data sent
 **********************************************************************/
S32 I2C_ReadPoll (S32 I2Cdev,
				U8 SlvAdr,
				void* buff,
				U32 length)
{
	I2C_CFG_T* device = (I2C_CFG_T *) I2Cdev;
	I2C_Type* i2cregs = device->regptr;
	U8* dataptr = (U8 *) buff;
	U32 count = 0;
	U32 errcode = 0;
	U8 tmp;
	S32 volatile exit_flag = 0;
	
	if (device->init == TRUE)
	{
		/* if master mode selected */
		if (device->mastermode == TRUE)
		{
			/* Start condition */
			if ((errcode = I2C_Start(i2cregs)) 
					!= I2STAT_MSTR_RCV_START)
	    	{
				// Save current I2C operation status
				device->status = errcode | I2C_STATUS_ERROR;
	    		goto exit_1;
	    	}
			
			/* Send slave address + RD direction bit = 1*/
	    	if ((errcode = I2C_SendByte(i2cregs, (SlvAdr | 0x01)))
	    			!= I2STAT_MSTR_RCV_SLAR_ACK)
	    	{
				// Save current I2C operation status
				device->status = errcode | I2C_STATUS_ERROR;
	    		goto exit_1;
	    	}
	    	
	    	/* Read a number of data bytes */
	    	while (count < length)
	    	{
	    		if (count == (length - 1))
	    		{
	    			if ((errcode = I2C_GetByte(i2cregs, &tmp, FALSE)) 
	    					!= I2STAT_MSTR_RCV_DAT_NACK)
	    			{
	    				// Save current I2C operation status
	    				device->status = errcode | I2C_STATUS_ERROR;
	    				goto exit_1;
	    			} 
	    		}
	    		else
	    		{
	    			if ((errcode = I2C_GetByte(i2cregs, &tmp, TRUE)) 
	    					!= I2STAT_MSTR_RCV_DAT_ACK)
	    			{
	    				// Save current I2C operation status
	    				device->status = errcode | I2C_STATUS_ERROR;
	    				goto exit_1;
	    			}
	    		}
	    		*dataptr = tmp;
	    		dataptr++;
	    		count++;
	    	}
	    	
	    	if (count == length)
	    	{
	    		// I2C Operation done!
	    		device->status = I2C_STATUS_DONE;
	    	}
	    	
	    	exit_1:
	    	/* Stop condition */
	    	I2C_Stop(i2cregs);
		}
		
		/* if slave mode selected */
		else
		{
			/* Set AA bit to ack command on I2C bus */
			i2cregs->I2CONSET = I2CON_AA;
			/* Clear Si bit to be ready ... */
			i2cregs->I2CONCLR = (I2CON_SI | I2CON_STA | I2CON_STO);
			
			/* Check for exit flag == 1 */
			while (exit_flag == 0)
			{
				/* Check SI flag ready */
				if (i2cregs->I2CONSET & I2CON_SI)
				{
					switch (i2cregs->I2STAT)
					{
					/* Own slave address has been received, 
					 * ACK has been returned */
					case I2STAT_SLV_RCV_SLAW_ACK:
		    			i2cregs->I2CONSET = I2CON_AA;
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    			
			    	/* Previously addressed with own SLA; 
			    	 * DATA byte has been received; 
			    	 * ACK has been returned */
		    		case I2STAT_SLV_RCV_PRE_SLA_DAT_ACK: 
		    			if (count < length)
		    			{
		    				*dataptr = (U8) i2cregs->I2DAT;
							dataptr++;
							count++;
		    			}
						i2cregs->I2CONSET = I2CON_AA;					
						i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    			
		    		/* Previously addressed with own SLA; 
		    		 * DATA byte has been received; 
		    		 * NOT ACK has been returned */
		    		case I2STAT_SLV_RCV_PRE_SLA_DAT_NACK:
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			break;	
		    			
		    		/* General call ack */	
		    		case I2STAT_SLV_RCV_GENCALL_ACK:
		    			i2cregs->I2CONSET = I2CON_AA;
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    		
		    		/* DATA has been received, ACK hasn been return */
		    		case I2STAT_SLV_RCV_PRE_GENCALL_DAT_ACK:
		    			if (count < length)
		    			{
		    				*dataptr = (U8) i2cregs->I2DAT;
							dataptr++;
							count++;
		    			}
						i2cregs->I2CONSET = I2CON_AA;					
						i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    		
		    		/* DATA has been received, ACK hasn been return */
		    		case I2STAT_SLV_RCV_PRE_GENCALL_DAT_NACK:
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			break;
		    		
		    		/* A Stop condition */
		    		case I2STAT_SLV_RCV_STASTO_SLVREC_SLVTRX:
	    				// Save current I2C operation status
	    				device->status = i2cregs->I2STAT | I2C_STATUS_DONE;
		    			i2cregs->I2CONCLR = I2CON_SI;
		    			exit_flag = 1;
		    			break;
		    			
	    			// Other status must be captured
		    		default:
	    				// Save current I2C operation status
	    				device->status = i2cregs->I2STAT | I2C_STATUS_ERROR;
	    				i2cregs->I2CONCLR = I2CON_SI;
		    			exit_flag = 1;
		    			break;
					}
				}
			}
			
			/* Clear AA bit to disable ack on I2C bus */
			i2cregs->I2CONCLR = I2CON_AA;
		}
	}

	return (S32) count;
}


/***********************************************************************
 * Function: I2C_ReadAsync
 * Purpose: Read a number of bytes in ASYNC (interrupt) mode
 * Parameters:
 *     	I2Cdev: Pointer to I2C device
 * 		SlvAdr: I2C slave address to send (available in master)
 * 		buff: pointer to buffer data.
 * 		length: length of data to be read (in byte) 
 * Returns: status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 I2C_ReadAsync (S32 I2Cdev,
					U8 SlvAdr,
					void* buff,
					U32 length)
{
	I2C_CFG_T* device = (I2C_CFG_T *) I2Cdev;
	I2C_Type* i2cregs = device->regptr;
	
	if (device->init == TRUE)
	{
		device->datptr = (U8 *) buff;
		device->idx = 0;
		device->length = length;
		
		/* if master mode selected */
		if (device->mastermode == TRUE)
		{
			device->slaveaddr = SlvAdr;
			device->handler = I2C_MRXHandler;
			device->status = I2C_STATUS_BUSY;
			i2cregs->I2CONCLR = (I2CON_AA | I2CON_SI | I2CON_STO);
			i2cregs->I2CONSET = I2CON_STA;
		}
		/* if slave mode selected */
		else
		{
			device->handler = I2C_SRXHandler;
			device->status = I2C_STATUS_BUSY;
			i2cregs->I2CONCLR = (I2CON_STA | I2CON_SI | I2CON_STO);
			i2cregs->I2CONSET = I2CON_AA;
		}
		
		/* Enable I2C interrupt for next necessary step */
		I2C_EnableInt (I2Cdev);
	}
	else
	{
		return _ERROR;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: I2C_SetCBS
 * Purpose: Configure Call back function for I2C device used in SYNC mode
 * Parameters: 
 * 		I2Cdev: pointer to I2C device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 I2C_SetCBS (S32 I2Cdev, void *pCBS)
{
	I2C_CFG_T* device = (I2C_CFG_T *) I2Cdev;
	
	if (device->init == FALSE)
	{
		return _ERROR;
	}
	
	device->cbs = pCBS;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: I2C_GetStatus
 * Purpose: Get current status of I2C device operation 
 * Parameters: 
 * 		I2Cdev: pointer to I2C device
 * Returns: None
 **********************************************************************/
U32 I2C_GetStatus (S32 I2Cdev)
{
	I2C_CFG_T* device = (I2C_CFG_T *) I2Cdev;
	return device->status;
}


/***********************************************************************
 * Function: I2C0_ISR
 * Purpose: Common interrupt handler for I2C0 device
 * Parameters: None
 * Returns: None
 **********************************************************************/
void I2C0_ISR (void)
{
    i2ccfgdat[0].handler((S32) (&i2ccfgdat[0]));
    // update VIC addr
    VIC_Ack ();
}


/***********************************************************************
 * Function: I2C1_ISR
 * Purpose: Common interrupt handler for I2C1 device
 * Parameters: None
 * Returns: None
 **********************************************************************/
void I2C1_ISR (void)
{
    i2ccfgdat[1].handler((S32) (&i2ccfgdat[1]));
    // update VIC addr
    VIC_Ack ();
}


/***********************************************************************
 * Function: I2C2_ISR
 * Purpose: Common interrupt handler for I2C2 device
 * Parameters: None
 * Returns: None
 **********************************************************************/
void I2C2_ISR (void)
{
    i2ccfgdat[2].handler((S32) (&i2ccfgdat[2]));
    // update VIC addr
    VIC_Ack ();
}
