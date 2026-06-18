/***********************************************************************
* $Id: lpc23xx_uart_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx UART driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: UART
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
#include "arm7tdmis_types.h"
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_pinsel_driver.h"

/***********************************************************************
 * Private UART driver data and types
 **********************************************************************/
/* UART baudrate parameter for register structure type */
typedef struct
{
    U32 Udlm;		/* divisor latch MSB */
    U32 Udll;		/* divisor latch LSB */
    U32 Umul;		/* Fractional Divider multiple value */
    U32 Udiv;		/* Fractional Divider divider value */
} UART_BAUDREGS_VAL_T;

/* UART Ring buffer structure */
typedef struct
{
    U8  tx[UART_RING_BUFSIZE];  /* UART Tx data ring buffer */
    U8  rx[UART_RING_BUFSIZE];  /* UART Rx data ring buffer */
    S32 tx_head;                /* UART Tx ring buffer head index */
    S32 tx_tail;                /* UART Tx ring buffer tail index */
    S32 rx_head;                /* UART Rx ring buffer head index */
    S32 rx_tail;                /* UART Rx ring buffer tail index */
} UART_RING_BUFFER_T;

/* UART device configuration structure type */
typedef struct
{
    BOOL32 init;          	/* Device initialized flag */
    UART_RING_BUFFER_T rb; 	/* Device ring buffer */
    UART_Type *regptr;   	/* Pointer to UART registers */
} UART_CFG_T;

/* UART device configuration structure for UART 0 */
#if UART_0_ENABLE == 1
STATIC UART_CFG_T uart0cfg;
#endif

/* UART device configuration structure for UART 1 */
#if UART_1_ENABLE == 1
STATIC UART_CFG_T uart1cfg;
#endif

/**********************************************************************
 * UART driver private functions
 *********************************************************************/

/**********************************************************************
 * Function: uart_standard_receive
 * Purpose: Common UART receive function
 * Parameter:
 * 		uartcfg: pointer to UART configuration structure
 * Return: 
 * 		None
 *********************************************************************/
STATIC void uart_standard_receive(UART_CFG_T *uartcfg)
{
    UART_Type *uartregs = uartcfg->regptr;
    /* Continue stuffing the receive ring FIFO until the receive FIFO
       is empty - RDR bit = 0 */
    while (uartregs->LSR & UART_LSR_RDR)
    {
        uartcfg->rb.rx[uartcfg->rb.rx_head] =
            (U8) (uartregs->RBTHDLR & UART_DATA_MASK);

        /* Increment receive ring buffer head pointer */
        uartcfg->rb.rx_head++;
        if (uartcfg->rb.rx_head >= UART_RING_BUFSIZE)
        {
            uartcfg->rb.rx_head = 0;
        }
    }
}

/**********************************************************************
 * Function: uart_standard_transmit
 * Purpose: Common UART transmit function
 * Parameter:
 * 		uartcfg: pointer to UART configuration structure
 * Return: 
 * 		None
 *********************************************************************/
STATIC void uart_standard_transmit(UART_CFG_T *uartcfg)
{
    U32 FIFO_buff_size = 16; /* reserve for FIFO mode */
	UART_Type *uartregs = uartcfg->regptr;
	
	/* Wait for FIFO buffer empty, transfer 16 bytes of data or 
	 break whenever ring buffers are empty*/
	/* Wait until THR empty */
	while (!(uartregs->LSR & UART_LSR_THRE));

    while ((uartcfg->rb.tx_head != uartcfg->rb.tx_tail)
        && (FIFO_buff_size > 0))
    {
        /* Move a piece of data into the transmit FIFO */
    	/* HieuNguyen: Note U32 */
        uartregs->RBTHDLR = uartcfg->rb.tx[uartcfg->rb.tx_tail];
        
        /* Update FIFO_buff_size */
        FIFO_buff_size--;
        /* Update transmit ring FIFO tail pointer */
        uartcfg->rb.tx_tail++;
        if (uartcfg->rb.tx_tail >= UART_RING_BUFSIZE)
        {
            uartcfg->rb.tx_tail = 0;
        }
    } 

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
    if (uartcfg->rb.tx_head == uartcfg->rb.tx_tail)
    {
        uartregs->DMIER &= ~UART_IER_THRE;
    }
    else
    {
        uartregs->DMIER |= UART_IER_THRE;
    }
}

/**********************************************************************
 * Function: uart_standard_interrupt
 * Purpose: Common UART interrupt function
 * Parameter:
 * 		uartcfg: pointer to UART configuration structure
 * Return: 
 * 		None
 *********************************************************************/
STATIC void uart_standard_interrupt(UART_CFG_T *uartcfg)
{	
	UART_Type *uartregs = uartcfg->regptr;
    U32 iir_temp = uartregs->IIFCR;
    /* Skip Pending bit and mask bits 3..1 for checking */
    iir_temp &= 0x0E;
    
    /* Interrupt was due to a receive data FIFO service request */
    if ((iir_temp == UART_IIR_RDA) || (iir_temp == UART_IIR_CTI))
    /* if (iir_temp == UART_IIR_RDA)
     * HieuNguyen: remain for spurious interrupt handling
     * causes by CTI interrupt... */
    {
    	uart_standard_receive(uartcfg);
    }
    
    /* Interrupt was due to a transmit data FIFO service request */
    /* Check interrupt pending and THRE bit */
    if (iir_temp == UART_IIR_THRE)
    {	
    	uart_standard_transmit(uartcfg);
    }
    
    /* Interrupt was due to receive line status */
    if (iir_temp == UART_IIR_RLS)
    {
    	/* Add your code here */
    }
    
    /* Interrupt was due to a modem service request */
    /* 
    if ((uartregs->iifcr & UART_INTR_MI) != 0)
    {
         Nothing to do here - add functionality as needed. For now,
           just disable the interrupt so it won't happen again 
        uartregs->inte &= ~UART_INTR_MI;
    }
    */
}

/**********************************************************************
 * Function: uart_get_free_tx_count
 * Purpose: Return the amount of free space in the transmit ring buffer
 * Parameter:
 * 		uartcfg: pointer to UART configuration structure
 * Return: 
 * 		The number of free bytes in the transmit ring buffer
 *********************************************************************/
STATIC S32 uart_get_free_tx_count(UART_CFG_T *uartcfg)
{
    S32 count = 0;

    count = uartcfg->rb.tx_head - uartcfg->rb.tx_tail;
    if (count < 0)
    {
        /* head pointer has flipped to start of ring */
        count = (UART_RING_BUFSIZE - uartcfg->rb.tx_tail) +
            uartcfg->rb.tx_head;
    }

    return (UART_RING_BUFSIZE - count - 1);
}



/**********************************************************************
 * UART driver public functions
 *********************************************************************/

/**********************************************************************
 * Function: UART_Init
 * Purpose: Open a UART
 * Parameter:
 * 		UARTPortNum: UART port number (0, 1, 2, 3)
 * 		Baudrate: Baudrate value 
 * 		Databit: Databit value
 * 		Parity: Parity value
 * 		Stopbit: Stopbit value
 * Return: 
 * 		The pointer to a UART config structure or 0
 *********************************************************************/
S32 UART_Init (S32 UARTPortNum, U32 Baudrate, U32 Databit, 
				U32 Parity, U32 Stopbit)
{
    UART_CFG_T *uartcfg;

    /* Invalid or disabled UART */
    uartcfg = (UART_CFG_T *) NULL;

#if UART_0_ENABLE == 1
    if (UARTPortNum == UART_PORT_0)
    {
        /* UART 0 selected */
        uartcfg = &uart0cfg;
    }
#endif

#if UART_1_ENABLE == 1
    if (UARTPortNum == UART_PORT_1)
    {
        /* UART 0 selected */
        uartcfg = &uart1cfg;
    }
#endif

    if (uartcfg != (UART_CFG_T *) NULL)
    {
        /* Valid UART selected, has it been previously initialized? */
        if (uartcfg->init == FALSE)
        {
            /* Device not initialized and it is usable, so set it to
               used */
            uartcfg->init = TRUE;

            /* Save address of register block */
            switch (UARTPortNum)
            {
            /* UART 0 selected */
            case UART_PORT_0:
            	uartcfg->regptr = UART0;
            	
                /* Enable UART function pin */
                PINSEL_Set(PORT_0, 2, FUNC_1);
                PINSEL_Set(PORT_0, 3, FUNC_1);
                
                /* Set up clock and power for UART module */
            	SCB_EnablePPWR (PCUART0);
                /* As default, peripheral clock for UART0 module 
                 * is set to FCCLK / 2 */
            	SCB_SetPCLKDiv(PCLK_UART0, CCLK_DIV_2);
            	
            	break;
            	
        	/* UART 1 selected */
            case UART_PORT_1:
            	uartcfg->regptr = UART1;
            	
                /* Enable UART function pin */
                PINSEL_Set(PORT_0, 15, FUNC_1);
                PINSEL_Set(PORT_0, 16, FUNC_1);
                
                /* Set up clock and power for UART module */
            	SCB_EnablePPWR (PCUART1);
                /* As default, peripheral clock for UART1 module 
                 * is set to FCCLK / 2 */
            	SCB_SetPCLKDiv(PCLK_UART1, CCLK_DIV_2);
            	
            	break;
            }
            
            /* Disable interrupts - the interrupts remain off until
                they are enabled */
            /* DLAB bit (lcr) must be reset */
            uartcfg->regptr->LCR = 0;
            uartcfg->regptr->DMIER = 0;

            /* Initialize the ring buffers */
            uartcfg->rb.tx_head = uartcfg->rb.tx_tail = 0;
            uartcfg->rb.rx_head = uartcfg->rb.rx_tail = 0;            
            
            UART_SetBaudrate ((S32) uartcfg, Baudrate);
            UART_SetDatabit ((S32) uartcfg, Databit);
            UART_SetParity ((S32) uartcfg, Parity);
            UART_SetStopbit ((S32) uartcfg, Stopbit);
 
            /* FIFOs are empty when it is re-enabled - 
             * FIFO length = 1 as default */
            uartcfg->regptr->IIFCR = (U32)( UART_FIFO_EN | 
            		UART_FIFO_RX_RS | UART_FIFO_TX_RS);
            
            /* Enable transmit bit */
            uartcfg->regptr->TER = UART_TER_TXEN;
        }
    }

    return (S32) uartcfg;
}

/**********************************************************************
 * Function: UART_SetBaudrate
 * Purpose: Set baurate for UART device
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		Baudrate: Baudrate value
 * Return: 
 * 		None
 *********************************************************************/
void UART_SetBaudrate (S32 UARTdev, U32 Baudrate)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	
	S32 temp, div, mul, udl1, udl1m, udl2, udls, last_udl, last_div, last_mul;
	float clock, baud, udl0, baud1, baud2, err1, err2, errs, last_err;
	U32 udll, udlm;
	
	if (uartcfg->init == FALSE)
	{
		return;
	}
	
	if (uartcfg->regptr == UART0)
	{
		clock = (float) SCB_GetPCLK (PCLK_UART0);
	}
	else if (uartcfg->regptr == UART1)
	{
		clock = (float) SCB_GetPCLK (PCLK_UART1);
	}
	
	baud = (float) Baudrate;
	div = 0;
	mul = 0;
	udl1 = 0;
	udl1m = 0;
	udl2 = 0;
	udls = 0;
	last_udl = 0;
	last_div = 0;
	last_mul = 0;
	
	udl0 = 0;
	baud1 = 0;
	baud2 = 0;
	err1 = 0;
	err2 = 0;
	errs = 0;
	last_err = 1000;
	
	for (div = 0; div < 15; div++)
	{		
		for (mul = div + 1; mul < 16; mul++)
		{
			udl0 = (float)clock / (float)(16 * baud * (1 + ((float)div / (float)mul)));
			temp = (S32) udl0;			
			
			if (temp == 0)
			{
				udl1 = 1;
			}
			else
			{
				udl1 = temp;
			}
			
			if (udl1 > 65535)
			{
				udl1m = 65535;
			}
			else
			{
				udl1m = udl1;
			}
			
			if ((udl0 == udl1) || (udl1m == 65535))
			{
				udl2 = udl1m;
			}
			else
			{
				udl2 = temp + 1;
			}
			
			baud1 = (float)clock / (float)(16 * udl1m * (1 + ((float)div / (float)mul)));
			err1 =  (((float)baud1 / (float)baud) - 1);
			
			if (err1 < 0)
			{
				err1 = -err1;
			}
			
			baud2 = (float)clock / (float)(16 * udl2 * (1 + ((float)div / (float)mul)));
			err2 = (((float)baud2 / (float)baud) - 1);
			
			if (err2 < 0)
			{
				err2 = -err2;
			}
			
			if (err1 < err2)
			{
				errs = err1;
				udls = udl1;
			}
			else
			{
				errs = err2;
				udls = udl2;  
			}
			if (errs < last_err)
			{
				last_err = errs;
				last_udl = udls;
				last_div = div;
				last_mul = mul;
			}
		}		
	}
	
	udlm = (U32) (last_udl / 256);
	udll = (U32) (last_udl - (256 * udlm));
	
	uartcfg->regptr->LCR |= UART_LCR_DLABEN;
	uartcfg->regptr->DMIER = udlm;							
	uartcfg->regptr->RBTHDLR = udll;
    /* Then reset DLAB bit */
	uartcfg->regptr->LCR&= ~UART_LCR_DLABEN;
	uartcfg->regptr->FDR = (U32)(((U32) last_mul << 4) | ((U32) last_div));
}

/**********************************************************************
 * Function: UART_SetDatabit
 * Purpose: Set databit for UART device
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		Databit: Databit value
 * Return: 
 * 		None
 *********************************************************************/
void UART_SetDatabit (S32 UARTdev, U32 Databit)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	U32 tmp1;
	
	if (uartcfg->init == FALSE)
	{
		return;
	}
	
	/* Set the data bit width */            	
	if ((Databit >= 5) && (Databit <= 8))
	{
	    tmp1 = uartcfg->regptr->LCR & ~UART_LCR_WLEN8;
	    switch (Databit)
	    {
    	case 5: uartcfg->regptr->LCR = 
    			(tmp1 | UART_LCR_WLEN5);
    			break;
    	case 6: uartcfg->regptr->LCR =
				(tmp1 | UART_LCR_WLEN6);
				break;
    	case 7: uartcfg->regptr->LCR = 
    			(tmp1 | UART_LCR_WLEN7);
    			break;
    	case 8: uartcfg->regptr->LCR = 
				(tmp1 | UART_LCR_WLEN8);
				break;
	    }
	}
	
	return;
}

/**********************************************************************
 * Function: UART_SetParity
 * Purpose: Set parity for UART device
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		Parity: Parity value
 * Return: 
 * 		None
 *********************************************************************/
void UART_SetParity (S32 UARTdev, U32 Parity)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	U32 tmp1 = 0;
	
	if (uartcfg->init == FALSE)
	{
		return;
	}
	
    switch (Parity)
    {
        case PARITY_NONE:
            /* Disable parity */
            uartcfg->regptr->LCR &= ~UART_LCR_PEN;
            break;

        case PARITY_ODD:
            /* Odd parity */
        	uartcfg->regptr->LCR |= UART_LCR_PEN;
            tmp1 = uartcfg->regptr->LCR & ~UART_LCR_SP_0;
            uartcfg->regptr->LCR =  tmp1 | UART_LCR_PODD;
            break;

        case PARITY_EVEN:
            /* Even parity */
        	uartcfg->regptr->LCR |= UART_LCR_PEN;
            tmp1 = uartcfg->regptr->LCR & ~UART_LCR_SP_0;
            uartcfg->regptr->LCR =  tmp1 | UART_LCR_PEVEN;
            break;
            
        case PARITY_SP_1:
            /* Forced "1" stick parity */
        	uartcfg->regptr->LCR |= UART_LCR_PEN;
            tmp1 = uartcfg->regptr->LCR & ~UART_LCR_SP_0;
            uartcfg->regptr->LCR =  tmp1 | UART_LCR_SP_1;
            break;
        
        case PARITY_SP_0:
            /* Forced "0" stick parity */
        	uartcfg->regptr->LCR |= UART_LCR_PEN;
        	uartcfg->regptr->LCR |= UART_LCR_SP_0;
            break;
    }
    
    return;
}

/**********************************************************************
 * Function: UART_SetStopbit
 * Purpose: Set stopbit for UART device
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		Stopbit: Stopbit value
 * Return: 
 * 		None
 *********************************************************************/
void UART_SetStopbit (S32 UARTdev, U32 Stopbit)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	
	if (uartcfg->init == FALSE)
	{
		return;
	}
	
    /* Set the number of stop bits */
    if (Stopbit == 1)
    {
        /* 1 stop bit */
    	uartcfg->regptr->LCR &= ~UART_LCR_STP2;
    }
    else if (Stopbit == 2)
    {
        /* 2 stop bits */
        uartcfg->regptr->LCR |= UART_LCR_STP2;
    }
}

/**********************************************************************
 * Function: UART_SetFIFO
 * Purpose: Set Rx FIFO trigger level for UART device
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		FIFOlevel: Rx FIFO trigger level value
 * Return: 
 * 		None
 *********************************************************************/
void UART_SetFIFO (S32 UARTdev, U32 FIFOlevel)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	
	if (uartcfg->init == FALSE)
	{
		return;
	}
	
	/* This argument used for disable FIFO */
	if (FIFOlevel == 4)
	{
        /* Disable FIFOs */
		uartcfg->regptr->IIFCR &= ~UART_FIFO_EN;
	}
	/* Enable FIFO with FIFO level */
	else
	{
		switch (FIFOlevel)
		{
		/* 1 character */
		case 0:
			uartcfg->regptr->IIFCR |= UART_FIFO_TRG_LEV0;
			break;
		/* 4 character */
		case 1:
			uartcfg->regptr->IIFCR |= UART_FIFO_TRG_LEV1;
			break;
		/* 8 character */
		case 2:
			uartcfg->regptr->IIFCR |= UART_FIFO_TRG_LEV2;
			break;
		/* 14 character */
		case 3:
			uartcfg->regptr->IIFCR |= UART_FIFO_TRG_LEV3;
			break;	
		}
		
        /* Enable FIFOs */
        uartcfg->regptr->IIFCR |= UART_FIFO_EN;
	}
}

/**********************************************************************
 * Function: UART_EnableInt
 * Purpose: Enable interrupt for each type for UART device 
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		UARTIntType: Type of UART interrupt
 * Return: 
 * 		None
 *********************************************************************/
void UART_EnableInt (S32 UARTdev, U32 UARTIntType)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	
    /* Enable selected interrupts */            	
	uartcfg->regptr->DMIER |= ((U32) UARTIntType & 
     		(UART_IER_RBR | UART_IER_THRE | UART_IER_RLS)); 
}

/**********************************************************************
 * Function: UART_DisableInt
 * Purpose: Disable interrupt for each type for UART device 
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * 		UARTIntType: Type of UART interrupt
 * Return: 
 * 		None
 *********************************************************************/
void UART_DisableInt (S32 UARTdev, U32 UARTIntType)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	
	/* Disable selected interrupts */  
	uartcfg->regptr->DMIER &= ~((U32) UARTIntType & 
    		(UART_IER_RBR | UART_IER_THRE | UART_IER_RLS));      
}

/**********************************************************************
 * Function: UART_GetStatus
 * Purpose: Get current status of UART line status register 
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * Return: 
 * 		Value of UART status register 
 *********************************************************************/
U32 UART_GetStatus (S32 UARTdev)
{
	UART_CFG_T *uartcfg = (UART_CFG_T *) UARTdev;
	
	if (uartcfg->init == FALSE) return 0;
	return uartcfg->regptr->LSR;
}

/**********************************************************************
 * Function: UART_Close
 * Purpose: Close the UART device
 * Parameter:
 * 		UARTdev: pointer to UART configuration structure
 * Return: 
 * 		_NO_ERROR if success or _ERROR
 *********************************************************************/
S32 UART_Close (S32 UARTdev)
{
    UART_CFG_T *uartcfgptr = (UART_CFG_T *) UARTdev;
    STATUS status = _ERROR;

    if (uartcfgptr->init == TRUE)
    {
        /* Disable UART */
        /* Do nothing since UART has not UART ENABLE bit */
        status = _NO_ERROR;
        uartcfgptr->init = FALSE;
    }

    return status;
}

/**********************************************************************
 * Function: UART_ReadPoll
 * Purpose: UART read function for polled mode
 * Parameter:
 * 	   UARTdev: Pointer to UART config structure
 *     buffer: Pointer to data buffer to copy to
 *     max_bytes: Number of bytes to read
 * Return: 
 * 		Number of bytes actually read from the FIFO
 *********************************************************************/
S32 UART_ReadPoll (S32 UARTdev,
                        void *buffer,
                        S32 max_bytes)
{
    UART_Type *uartregs;
    UART_CFG_T *uartcfgptr = (UART_CFG_T *) UARTdev;
    U8 *data = (U8 *) buffer;
    S32 bytes = 0;

    if (uartcfgptr->init == TRUE)
    {
        uartregs = uartcfgptr->regptr;

        /* Loop until receive FIFO is empty or until max_bytes
           expires */
        while ((max_bytes > 0) && (uartregs->LSR & UART_LSR_RDR))
        {
            /* Read data from FIFO into buffer */
            *data = (U8) uartregs->RBTHDLR;
            data++;

            /* Increment data count and decrement buffer size count */
            bytes++;
            max_bytes--;
        }
    }

    return bytes;
}

/**********************************************************************
 * Function: UART_WritePoll
 * Purpose: UART write function for polled mode
 * Parameter:
 * 	   UARTdev: Pointer to UART config structure
 *     buffer:  Pointer to data buffer to copy from
 *     n_bytes: Number of bytes to write
 * Return: 
 * 		 Number of bytes actually written to the FIFO
 *********************************************************************/
S32 UART_WritePoll (S32 UARTdev,
                         void *buffer,
                         S32 n_bytes)
{
    UART_Type *uartregs;
    UART_CFG_T *uartcfgptr = (UART_CFG_T *) UARTdev;
    U8 *data = (U8 *) buffer;
    S32 bytes = 0;

    if (uartcfgptr->init == TRUE)
    {
        uartregs = uartcfgptr->regptr;

        /* Loop until n_bytes expires to 0 */
        while (n_bytes > 0)
        {
            if (uartregs->LSR & UART_LSR_THRE)
            {
                /* Write data from buffer into FIFO */
                uartregs->RBTHDLR = *data;
                data++;

                /* Increment data count and decrement buffer size
                   count */
                bytes++;
                n_bytes--;
            }
        }
    }

    return bytes;
}

/**********************************************************************
 * Function: UART_ReadRing
 * Purpose: UART read function for interrupt mode (using ring buffers)
 * Parameter:
 * 	   UARTdev:  Pointer to UART config structure
 *     buffer:    Pointer to data buffer to copy to
 *     max_bytes: Number of bytes to read
 * Return: 
 * 		 Number of bytes actually read from the ring buffer
 *********************************************************************/
S32 UART_ReadRing (S32 UARTdev,
                      void *buffer,
                      S32 max_bytes)
{
    UART_Type *uartregs;
    U32 savedint;
    UART_CFG_T *uartcfgptr = (UART_CFG_T *) UARTdev;
    U8 *data = (U8 *) buffer;
    S32 bytes = 0;

    if (uartcfgptr->init == TRUE)
    {
        uartregs = uartcfgptr->regptr;
        
        /* Temporarily lock out UART receive interrupts during this
           read so the UART receive interrupt won't cause problems
           with the index values */ 
        savedint = uartregs->DMIER & (UART_IER_RBR | UART_IER_RLS);
        uartregs->DMIER &= ~(UART_IER_RBR | UART_IER_RLS);
        
        /* Loop until receive buffer ring is empty or 
        	until max_bytes expires */
        while ((max_bytes > 0) &&
            (uartcfgptr->rb.rx_tail != uartcfgptr->rb.rx_head))
        {
            /* Read data from ring buffer into user buffer */
            *data = uartcfgptr->rb.rx[uartcfgptr->rb.rx_tail];
            data++;

            /* Update tail pointer */
            uartcfgptr->rb.rx_tail++;

            /* Make sure tail didn't overflow */
            if (uartcfgptr->rb.rx_tail >= UART_RING_BUFSIZE)
            {
                uartcfgptr->rb.rx_tail = 0;
            }

            /* Increment data count and decrement buffer size count */
            bytes++;
            max_bytes--;
        }

        /* Re-enable UART interrupts */
        uartregs->DMIER |= savedint;
    }

    return bytes;
}

/**********************************************************************
 * Function: UART_WriteRing
 * Purpose: UART write function for interrupt mode (using ring buffers)
 * Parameter:
 * 	   UARTdev: Pointer to UART config structure
 *     buffer:  Pointer to data buffer to copy from
 *     n_bytes: Number of bytes to write
 * Return: 
 * 		 Number of bytes actually written to the ring buffer
 *********************************************************************/
S32 UART_WriteRing (S32 UARTdev,
                       void *buffer,
                       S32 n_bytes)
{
    UART_Type *uartregs;
    U32 savedint;
    UART_CFG_T *uartcfgptr = (UART_CFG_T *) UARTdev;
    U8 *data = (U8 *) buffer;
    S32 bytes = 0;

    if (uartcfgptr->init == TRUE)
    {
        uartregs = uartcfgptr->regptr;

        /* Temporarily lock out UART transmit interrupts during this
           read so the UART transmit interrupt won't cause problems
           with the index values */
        savedint = uartregs->DMIER & UART_IER_THRE;
        uartregs->DMIER &= ~UART_IER_THRE;
        
        /* Loop until transmit run buffer is full or until n_bytes
           expires */
        while ((n_bytes > 0) &&
            (uart_get_free_tx_count(uartcfgptr) > 0))
        {
            /* Write data from buffer into ring buffer */
            uartcfgptr->rb.tx[uartcfgptr->rb.tx_head] = *data;
            data++;

            /* Increment head pointer */
            uartcfgptr->rb.tx_head++;
            if (uartcfgptr->rb.tx_head >= UART_RING_BUFSIZE)
            {
                uartcfgptr->rb.tx_head = 0;
            }

            /* Increment data count and decrement buffer size count */
            bytes++;
            n_bytes--;
        }

        /* Now start sending the data - this will also re-enable the
           transmit interrupt */
        if (bytes > 0)
        {
            if (uartregs->LSR & UART_LSR_THRE)
            {
            	uart_standard_transmit(uartcfgptr);
            }
            else 
            {
            	uartregs->DMIER |= UART_IER_THRE;
            }
        }
        else
        {
        	/* Restore original interrupt state */
        	uartregs->DMIER |= savedint;
        }
    }
    return bytes;
}

#if UART_0_ENABLE == 1
/**********************************************************************
 * Function: uart0_isr
 * Purpose: UART 0 interrupt function
 * Parameter:
 * 	   	None
 * Return: 
 * 		None
 *********************************************************************/
void uart0_isr(void)
{
    uart_standard_interrupt(&uart0cfg);
    /* Update VIC Vector Addr */
    VIC_Ack();
}
#endif

#if UART_1_ENABLE == 1
/**********************************************************************
 * Function: uart1_isr
 * Purpose: UART 1 interrupt function
 * Parameter:
 * 	   	None
 * Return: 
 * 		None
 *********************************************************************/
void uart1_isr(void)
{
    uart_standard_interrupt(&uart1cfg);
    /* Update VIC Vector Addr */
    VIC_Ack();
}
#endif


/***********************************************************************
 * UART driver utilities
 **********************************************************************/

/********************************************************************
 * Function: UART_PutChar
 * Purpose: Put a character to UART port
 * Parameters: 
 * 		devid: Pointer to UART device
 * 		ch: character to send
 * Returns: 
 * 		Nothing
 *******************************************************************/
void UART_PutChar (S32 devid, CHAR ch)
{
	UART_Type *uartregs;
    UART_CFG_T *uartcfgptr = (UART_CFG_T *) devid;

    if (uartcfgptr->init == TRUE)
    {
        uartregs = uartcfgptr->regptr;
        while (!(uartregs->LSR & UART_LSR_THRE));
        uartregs->RBTHDLR = ch;
    }
}

/********************************************************************
 * Function: UART_Puts
 * Purpose: Put a string to UART port
 * Parameters: 
 * 		devid: Pointer to UART device
 * 		str: pointer to string to send
 * Returns: 
 * 		Nothing
 *******************************************************************/
void UART_Puts(S32 devid, const void *str)
{
	CHAR *s = (CHAR *) str;
	
	while (*s)
	{
		UART_PutChar(devid, *s++);
	}
}

/**********************************************************************
 * Function: UART_Puts_
 * Purpose: Put a string to UART port and print new line
 * Parameter:
 *		devid: Pointer to UART device
 * 		str: pointer to string to send
 * Return: 
 * 		None
 *********************************************************************/
void UART_Puts_(S32 devid, const void *str)
{
	UART_Puts (devid, str);
	UART_Puts (devid, "\n\r");
}

/**********************************************************************
 * Function: UART_PutDec
 * Purpose: Covert a decimal number (U64) to string and send to UART
 * Parameter:
 * 		devid: Pointer to UART device
 * 		decnum: U64 number 
 * Return: 
 * 		None
 *********************************************************************/
void UART_PutDec(S32 devid, U64 decnum)
{
	U64 tmp = 9, cnt = 1;
	
#if 1
	CHAR ch[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
	
	while (tmp < decnum)
	{
		tmp *= 10;
		tmp += 9;
		cnt ++;
	}
	
#if 0
	CHAR ch[cnt + 1];
	ch[cnt] = 0;
#endif
	
	while (cnt--)
	{
		ch[cnt] = '0' + (decnum % 10);
		decnum /= 10;
	}
	
	UART_Puts (devid, ch);
}

/**********************************************************************
 * Function: UART_PutHex
 * Purpose: Covert a hex number (U32) to string and send to UART
 * Parameter:
 * 		devid: Pointer to UART device
 * 		hexnum: U32 hex number 
 * Return: 
 * 		None
 *********************************************************************/
void UART_PutHex (S32 devid, U32 hexnum) 
{
	U8 nibble;
	U8 i;
	
	UART_Puts (devid, "0x");
	
	i = 7;
	do
	{
		nibble = (hexnum >> (4 * i)) & 0x0F;
		if (nibble > 9) 
		{
			UART_PutChar(devid, 'A' + (nibble - 10));
		}
		else
		{
			UART_PutChar(devid, '0' +  nibble);
		}
	} while (i--);
}
