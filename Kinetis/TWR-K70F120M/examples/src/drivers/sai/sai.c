/*
 *	File: sai.c
 *	Purpose: SAI module driver
 *	Author: Wang Hao (B17539)
 * 
*/

#include "common.h"
#include "sai.h"

/* Global variable */
unsigned int tx_buffer[BUFFER_SIZE];
unsigned int rx_buffer[BUFFER_SIZE];
unsigned int tx_index;
unsigned int rx_index;

/*
 *	I2S0 port pins
 *	I2S0_TX_BCLK - 	PTC3 or PTB18 or PTE12 or PTA5
 *	I2S0_TX_FS - 	PTC2 or PTB19 or PTE11 or PTA13
 *	I2S0_RX_BCLK - 	PTC9	or PTC6 or PTE9 or PTA14
 *	I2S0_RX_FS - 	PTC10 or PTC7 or PTE8 or PTA16
 * 	I2S0_RXD0 -	PTC5 or PTE7 or PTA15
 *	I2S0_RXD1 - 	PTC11 or PTE8 or PTA16
 *	I2S0_TXD0 -	PTC1 or PTE10 or PTA12
 *	I2S0_TXD1 - 	PTC0 or PTE9 or PTA14
 * 	I2S0_MCLK - 	PTC6 or PTC8 or PTE6 or PTA17
 *
 *	I2S1 port pins
 *	I2S1_TX_BCLK - 	PTC4 or PTF6 or PTE25
 *	I2S1_TX_FS - 	PTC5 or PTF5 or PTE24
 *	I2S1_RX_BCLK - 	PTD3 or PTF1 or PTA7
 *	I2S1_RX_FS - 	PTD2 or PTF2 or PTA8
 * 	I2S1_RXD0 -	PTD1 or PTF3 or PTA6
 *	I2S1_RXD1 - 	PTD0 or PTF0 or PTE24
 *	I2S1_TXD0 -	PTF4 or PTE26
 *	I2S1_TXD1 - 	PTF7 or PTE25
 * 	I2S1_MCLK - 	PTF8 or PTC10 or PTE27
 *
*/ 
/*
*	Input parameter
*	port:		SAI module used
*	setting:	which pin mux setting used, please refer to above pin mux settings available on 256BGA package for K70
*/
void sai_pinmux_init(unsigned char port, unsigned char setting)
{
	if(port == 0)
	{
		if(setting == 0)	//using PORTC pins
		{
			PORTC_PCR(3) = PORT_PCR_MUX(6);		//PTC3(ALT6), I2S0_TX_BCLK	-	J53 pin11	
			PORTC_PCR(2) = PORT_PCR_MUX(6);		//PTC2(ALT6), I2S0_TX_FS	-	J8 pin8
			PORTC_PCR(9) = PORT_PCR_MUX(4);		//PTC9(ALT4), I2S0_RX_BCLK	-	J6 pin5
			PORTC_PCR(10) = PORT_PCR_MUX(4);	//PTC10(ALT4), I2S0_RX_FS	-	J6 pin2
			PORTC_PCR(5) = PORT_PCR_MUX(4);		//PTC5(ALT4), I2S0_RXD0	-	J8 pin2
			PORTC_PCR(11) = PORT_PCR_MUX(4);	//PTC11(ALT6), I2S0_RXD1	-	J14 pin8
			PORTC_PCR(1) = PORT_PCR_MUX(6);		//PTC1(ALT6), I2S0_TXD0	-	J8 pin11
			PORTC_PCR(0) = PORT_PCR_MUX(6);		//PTC0(ALT6), I2S0_TXD1	-	J8 pin14
			PORTC_PCR(6) = PORT_PCR_MUX(6);		//PTC6(ALT6), I2S0_MCLK	-	J6 pin14
		}
		else if(setting == 1)	//using PORTE pins
		{
			PORTE_PCR(12) = PORT_PCR_MUX(4);	//PTE12(ALT4), I2S0_TX_BCLK -	J3 pin8
			PORTE_PCR(11) = PORT_PCR_MUX(4);	//PTE11(ALT4), I2S0_TX_FS -		J3 pin5
			PORTE_PCR(9) = PORT_PCR_MUX(4);		//PTE9(ALT4), I2S0_RX_BCLK -	J9 pin14
			PORTE_PCR(8) = PORT_PCR_MUX(4);		//PTE8(ALT4), I2S0_RX_FS -		J9 pin11
			PORTE_PCR(7) = PORT_PCR_MUX(4);		//PTE7(ALT4), I2S0_RXD0 -		J9 pin8
			PORTC_PCR(11) = PORT_PCR_MUX(4);	//PTC11(ALT4), I2S0_RXD1 - 		J14 pin8
			PORTE_PCR(10) = PORT_PCR_MUX(4);	//PTE10(ALT4), I2S0_TXD0 -		J3 pin2
			PORTC_PCR(0) = PORT_PCR_MUX(6);		//PTC0(ALT6), I2S0_TXD1 -		J8 pin14
			PORTE_PCR(6) = PORT_PCR_MUX(4);		//PTE6(ALT4), I2S0_MCLK -		J9 pin5
		}
		else if(setting == 2)	//using PORTA pins
		{
			PORTA_PCR(5) = PORT_PCR_MUX(6);		//PTA5(ALT6), I2S0_TX_BCLK -	J27 pin11
			PORTA_PCR(13) = PORT_PCR_MUX(6);	//PTA13(ALT6), I2S0_TX_FS -		J24 pin5
			PORTA_PCR(14) = PORT_PCR_MUX(6);	//PTA14(ALT6), I2S0_RX_BCLK -	J24 pin8
			PORTA_PCR(16) = PORT_PCR_MUX(6);	//PTA16(ALT6), I2S0_RX_FS -		J24 pin14
			PORTA_PCR(15) = PORT_PCR_MUX(6);	//PTA15(ALT6), I2S0_RXD0 -		J24 pin11
			PORTC_PCR(11) = PORT_PCR_MUX(4);	//PTC11(ALT4), I2S0_RXD1 - 		J14 pin8
			PORTA_PCR(12) = PORT_PCR_MUX(6);	//PTA12(ALT6), I2S0_TXD0 -		J24 pin2
			PORTC_PCR(0) = PORT_PCR_MUX(6);		//PTC0(ALT6), I2S0_TXD1 -		J8 pin14
			PORTA_PCR(17) = PORT_PCR_MUX(6);	//PTA17(ALT6), I2S0_MCLK -		J25 pin2
		}
	}
	else if(port == 1)
	{
		if(setting == 0)	
		{
			PORTC_PCR(4) = PORT_PCR_MUX(7);		//PTC4(ALT7), I2S1_TX_BCLK	-	J8 pin5	
			PORTC_PCR(5) = PORT_PCR_MUX(7);		//PTC5(ALT7), I2S1_TX_FS	-	J8 pin2
			PORTD_PCR(3) = PORT_PCR_MUX(6);		//PTD3(ALT6), I2S1_RX_BCLK	-	J2 pin11
			PORTD_PCR(2) = PORT_PCR_MUX(6);		//PTD2(ALT6), I2S1_RX_FS	-	J2 pin14
			PORTD_PCR(1) = PORT_PCR_MUX(6);		//PTD1(ALT6), I2S1_RXD0	-	J14 pin14
			PORTD_PCR(0) = PORT_PCR_MUX(6);		//PTD0(ALT6), I2S1_RXD1	-	J43 pin11
			PORTF_PCR(4) = PORT_PCR_MUX(5);		//PTF4(ALT5), I2S1_TXD0	-	J62 pin9
			PORTF_PCR(7) = PORT_PCR_MUX(5);		//PTF7(ALT5), I2S1_TXD1	-	J63 pin1
			PORTC_PCR(10) = PORT_PCR_MUX(6);	//PTC10(ALT5), I2S1_MCLK	-	J6 pin2
		}
		else if(setting == 1)
		{
			PORTF_PCR(6) = PORT_PCR_MUX(5);		//PTF6(ALT5), I2S1_TX_BCLK -	J62 pin13
			PORTF_PCR(5) = PORT_PCR_MUX(5);		//PTF5(ALT5), I2S1_TX_FS -		J62 pin11
			PORTF_PCR(1) = PORT_PCR_MUX(5);		//PTF1(ALT5), I2S1_RX_BCLK -	J62 pin3
			PORTF_PCR(2) = PORT_PCR_MUX(5);		//PTF2(ALT5), I2S1_RX_FS -		J62 pin5
			PORTF_PCR(3) = PORT_PCR_MUX(5);		//PTF3(ALT5), I2S1_RXD0 -		J62 pin7
			PORTF_PCR(0) = PORT_PCR_MUX(5);		//PTF0(ALT5), I2S1_RXD1 - 		J62 pin1
			PORTF_PCR(4) = PORT_PCR_MUX(5);		//PTF4(ALT5), I2S1_TXD0 -		J62 pin9
			PORTF_PCR(7) = PORT_PCR_MUX(5);		//PTF7(ALT5), I2S1_TXD1 -		J63 pin1
			PORTF_PCR(8) = PORT_PCR_MUX(5);		//PTF8(ALT5), I2S1_MCLK -		J63 pin3
		}
		else if(setting == 2)	//using PORTA pins
		{
			/* Please note PTE24 and PTE25 is multiplexed with EXTAL1 and XTAL1, when testing this pin config, please use internal clock reference */
			PORTE_PCR(25) = PORT_PCR_MUX(4);	//PTE25(ALT4), I2S0_TX_BCLK -	J66 pin2
			PORTE_PCR(24) = PORT_PCR_MUX(4);	//PTE24(ALT4), I2S0_TX_FS -		J61 pin2
			PORTA_PCR(7) = PORT_PCR_MUX(4);		//PTA7(ALT4), I2S0_RX_BCLK -	J23 pin2
			PORTA_PCR(8) = PORT_PCR_MUX(4);		//PTA8(ALT4), I2S0_RX_FS -		J23 pin5
			PORTA_PCR(6) = PORT_PCR_MUX(4);		//PTA6(ALT4), I2S0_RXD0 -		R176 0ohm connecting to ULPI_CLK on motherboard
			PORTD_PCR(0) = PORT_PCR_MUX(6);		//PTD0(ALT6), I2S0_RXD1 - 		J43 pin11
			PORTE_PCR(26) = PORT_PCR_MUX(6);	//PTE26(ALT6), I2S0_TXD0 -		J53 pin13
			PORTF_PCR(7) = PORT_PCR_MUX(5);		//PTF7(ALT5), I2S0_TXD1 -		J63 pin1
			PORTA_PCR(27) = PORT_PCR_MUX(6);	//PTA27(ALT6), I2S0_MCLK -		J25 pin14
		}
	}
}
/*
*	Input Parameter:	
*	port:	SAI module used
*	tx:	whether configure tx or rx registers
*	stope:	stop enable or disable
*	dbge:		debug enable or disable
*/
void sai_stop_dbg_enable(unsigned char port, unsigned char tx, unsigned char stope, unsigned char dbge)
{
	if(port == 0)
	{
		if(tx)
		{
			if(stope)
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_STOPE_MASK;		//stop enable
			else
				I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_STOPE_MASK;	//stop disable
			if(dbge)
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_DBGE_MASK;		//debug enable
			else
				I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_DBGE_MASK;		//debug disable
		}
		else
		{
			if(stope)
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_STOPE_MASK;		//stop enable
			else
				I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_STOPE_MASK;	//stop disable
			if(dbge)
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_DBGE_MASK;		//debug enable
			else
				I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_DBGE_MASK;		//debug disable
		}
		
	}
	else if(port == 1)
	{
		if(tx)
		{
			if(stope)
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_STOPE_MASK;		//stop enable
			else
				I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_STOPE_MASK;	//stop disable
			if(dbge)
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_DBGE_MASK;		//debug enable
			else
				I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_DBGE_MASK;		//debug disable
		}
		else
		{
			if(stope)
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_STOPE_MASK;		//stop enable
			else
				I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_STOPE_MASK;	//stop disable
			if(dbge)
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_DBGE_MASK;		//debug enable
			else
				I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_DBGE_MASK;		//debug disable
		}
	}
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_fifo_reset(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FR_MASK;	//FIFO reset
		else
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FR_MASK;	//FIFO reset
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FR_MASK;	//FIFO reset
		else
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FR_MASK;	//FIFO reset
	}
	
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_soft_reset(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_SR_MASK;		//software reset
			I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_SR_MASK;		//clear software reset bit since it's sticky
		}
		else
		{
		  	I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_SR_MASK;		//software reset
			I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_SR_MASK;		//clear software reset bit since it's sticky
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_SR_MASK;		//software reset
			I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_SR_MASK;		//clear software reset bit since it's sticky
		}
		else
		{
		  	I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_SR_MASK;		//software reset
			I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_SR_MASK;		//clear software reset bit since it's sticky
		}
	}
	
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_interrupt_enable(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_WSIE_MASK	//word start interrupt enable
							| I2S_TCSR_SEIE_MASK	//sync error interrupt enable
							| I2S_TCSR_FEIE_MASK	//FIFO error interrupt enable
							| I2S_TCSR_FWIE_MASK	//FIFO warning interrupt enable
							| I2S_TCSR_FRIE_MASK	//FIFO request interrupt enable
							;
		}
		else
		{
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_WSIE_MASK	//word start interrupt enable
							| I2S_RCSR_SEIE_MASK	//sync error interrupt enable
							| I2S_RCSR_FEIE_MASK	//FIFO error interrupt enable
							| I2S_RCSR_FWIE_MASK	//FIFO warning interrupt enable
							| I2S_RCSR_FRIE_MASK	//FIFO request interrupt enable
							;					
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_WSIE_MASK	//word start interrupt enable
							| I2S_TCSR_SEIE_MASK	//sync error interrupt enable
							| I2S_TCSR_FEIE_MASK	//FIFO error interrupt enable
							| I2S_TCSR_FWIE_MASK	//FIFO warning interrupt enable
							| I2S_TCSR_FRIE_MASK	//FIFO request interrupt enable
							;
		}
		else
		{
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_WSIE_MASK	//word start interrupt enable
							| I2S_RCSR_SEIE_MASK	//sync error interrupt enable
							| I2S_RCSR_FEIE_MASK	//FIFO error interrupt enable
							| I2S_RCSR_FWIE_MASK	//FIFO warning interrupt enable
							| I2S_RCSR_FRIE_MASK	//FIFO request interrupt enable
							;					
		}
	}
	
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_clear_flags(unsigned char port, unsigned char tx)
{
	/* add code for checking if input parameter is valid */
	
	if(port == 0)
	{	/* SAI0 init */
		if(tx)
		{
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_WSF_MASK	//clear word start flag
								| I2S_TCSR_SEF_MASK		//clear sync error flag
								| I2S_TCSR_FEF_MASK		//clear FIFO error flag
								| I2S_TCSR_FWF_MASK		//clear FIFO warning flag
								| I2S_TCSR_FRF_MASK		//clear FIFO request flag
								;
		}
		else
		{
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_WSF_MASK	//clear word start flag
								| I2S_RCSR_SEF_MASK		//clear sync error flag
								| I2S_RCSR_FEF_MASK		//clear FIFO error flag
								| I2S_RCSR_FWF_MASK		//clear FIFO warning flag
								| I2S_RCSR_FRF_MASK		//clear FIFO request flag
								;
		}
	}
	else if(port == 1)
	{	/* SAI1 init */
		if(tx)
		{
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_WSF_MASK	//clear word start flag
								| I2S_TCSR_SEF_MASK		//clear sync error flag
								| I2S_TCSR_FEF_MASK		//clear FIFO error flag
								| I2S_TCSR_FWF_MASK		//clear FIFO warning flag
								| I2S_TCSR_FRF_MASK		//clear FIFO request flag
								;
		}
		else
		{
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_WSF_MASK	//clear word start flag
								| I2S_RCSR_SEF_MASK		//clear sync error flag
								| I2S_RCSR_FEF_MASK		//clear FIFO error flag
								| I2S_RCSR_FWF_MASK		//clear FIFO warning flag
								| I2S_RCSR_FRF_MASK		//clear FIFO request flag
								;
		}
	}	
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_fifo_request_dma_enable(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FRDE_MASK	//FIFO request DMA enable
								;
		}
		else
		{
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FRDE_MASK	//FIFO request DMA enable
								;
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FRDE_MASK	//FIFO request DMA enable
								;
		}
		else
		{
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FRDE_MASK	//FIFO request DMA enable
								;
		}
	}
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_fifo_warning_dma_enable(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FWDE_MASK	//FIFO warning DMA enable
								;
		}
		else
		{
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FWDE_MASK	//FIFO warning DMA enable
								;
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FWDE_MASK	//FIFO warning DMA enable
								;
		}
		else
		{
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FWDE_MASK	//FIFO warning DMA enable
								;
		}
	}
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_enable(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_TE_MASK;	//transmit enable
		else
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_RE_MASK;	//receive enable
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_TE_MASK;	//transmit enable
		else
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_RE_MASK;	//receive enable
	}
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_disable(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_TE_MASK;	//transmit disable
		else
			I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_RE_MASK;	//receive disable
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_TE_MASK;	//transmit disable
		else
			I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_RE_MASK;	//receive disable
	}
}


/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*	watermark:	FIFO watermark	
*/
void sai_watermark_config(unsigned char port, unsigned char tx, unsigned char watermark)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCR1_REG(I2S0_BASE_PTR) = watermark;	//tx FIFO watermark
		else
			I2S_RCR1_REG(I2S0_BASE_PTR)	= watermark;	//rx FIFO watermark
	}
	else if (port == 1)
	{
		if(tx)
			I2S_TCR1_REG(I2S1_BASE_PTR) = watermark;	//tx FIFO watermark
		else
			I2S_RCR1_REG(I2S1_BASE_PTR)	= watermark;	//rx FIFO watermark
	}
}

/*
*	Input parameter:
*	port:		SAI port used
*	tx:		transmit or receive
*	mode:		synchronous mode
*	bcs:		bit clock swap
*	bci:		bit clock input
*/
void sai_mode_config(unsigned char port, unsigned char tx, unsigned char mode, unsigned char bcs, unsigned char bci)
{
	if(port == 0)
	{
		if(tx)
		{
		  	I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_SYNC_MASK;
			I2S_TCR2_REG(I2S0_BASE_PTR) |= I2S_TCR2_SYNC(mode);	//configure SAI synchronous mode
			
			if(bcs)
				I2S_TCR2_REG(I2S0_BASE_PTR) |= I2S_TCR2_BCS_MASK;	//bit clock swap
			else 
				I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_BCS_MASK;	
				
			if(bci)
				I2S_TCR2_REG(I2S0_BASE_PTR) |= I2S_TCR2_BCI_MASK;	//internal logic clocked by external bit clock
			else 
				I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_BCI_MASK;	
		}
		else 
		{
			I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_SYNC_MASK;
			I2S_RCR2_REG(I2S0_BASE_PTR) |= I2S_RCR2_SYNC(mode);	//configure SAI synchronous mode
			
			if(bcs)
				I2S_RCR2_REG(I2S0_BASE_PTR) |= I2S_RCR2_BCS_MASK;	//bit clock swap
			else 
				I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_BCS_MASK;	
				
			if(bci)
				I2S_RCR2_REG(I2S0_BASE_PTR) |= I2S_RCR2_BCI_MASK;	//internal logic clocked by external bit clock
			else 
				I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_BCI_MASK;			
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_SYNC_MASK;
			I2S_TCR2_REG(I2S1_BASE_PTR) |= I2S_TCR2_SYNC(mode);	//configure SAI synchronous mode
			
			if(bcs)
				I2S_TCR2_REG(I2S1_BASE_PTR) |= I2S_TCR2_BCS_MASK;	//bit clock swap
			else 
				I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_BCS_MASK;	
				
			if(bci)
				I2S_TCR2_REG(I2S1_BASE_PTR) |= I2S_TCR2_BCI_MASK;	//internal logic clocked by external bit clock
			else 
				I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_BCI_MASK;	
		}
		else 
		{
			I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_SYNC_MASK;
			I2S_RCR2_REG(I2S1_BASE_PTR) |= I2S_RCR2_SYNC(mode);	//configure SAI synchronous mode
			
			if(bcs)
				I2S_RCR2_REG(I2S1_BASE_PTR) |= I2S_RCR2_BCS_MASK;	//bit clock swap
			else 
				I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_BCS_MASK;	
				
			if(bci)
				I2S_RCR2_REG(I2S1_BASE_PTR) |= I2S_RCR2_BCI_MASK;	//internal logic clocked by external bit clock
			else 
				I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_BCI_MASK;			
		}
	}
}

/*
*	Input parameter:	
*	port:		SAI module used
*	tx:		transmit or receive
*	mclksel:	select master clock to generate internal bit clock, ignored when configured for external generated bit clock
*	bclkpol:	bit clock polarity
*	bclkdir:	bit clock direction
*	bclkdiv:	bit clock divide, division value is (DIV+1)*2
*/
void sai_bclk_config(unsigned char port, unsigned char tx, unsigned char mclksel, unsigned char bclkpol, unsigned char bclkdir, unsigned char bclkdiv)
{
	if(port == 0)
	{	/* SAI0 init */
		if(tx)
		{
			I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_MSEL_MASK;	//clear master clock select
			I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_DIV_MASK;		//clear master clock divide

		  	I2S_TCR2_REG(I2S0_BASE_PTR) |= I2S_TCR2_MSEL(mclksel)	//master clock select
								| I2S_TCR2_DIV(bclkdiv)		//divide down master clock to generate bit clock
								;
			if(bclkpol)
				I2S_TCR2_REG(I2S0_BASE_PTR) |= I2S_TCR2_BCP_MASK;	//bit clock is active low
			else
				I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_BCP_MASK;	//bit clock is active high
				
			if(bclkdir)
				I2S_TCR2_REG(I2S0_BASE_PTR) |= I2S_TCR2_BCD_MASK;	//bit clock generated internally, master mode
			else
				I2S_TCR2_REG(I2S0_BASE_PTR) &= ~I2S_TCR2_BCD_MASK;	//bit clock generated externally, slave mode
		}
		else
		{
		  	I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_MSEL_MASK;	//clear master clock select
			I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_DIV_MASK;		//clear master clock divide
			
			I2S_RCR2_REG(I2S0_BASE_PTR) |= I2S_RCR2_MSEL(mclksel)	//master clock select
								| I2S_RCR2_DIV(bclkdiv)		//divide down master clock to generate bit clock
								;
			if(bclkpol)
				I2S_RCR2_REG(I2S0_BASE_PTR) |= I2S_RCR2_BCP_MASK;	//bit clock is active low
			else
				I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_BCP_MASK;	//bit clock is active high
				
			if(bclkdir)
				I2S_RCR2_REG(I2S0_BASE_PTR) |= I2S_RCR2_BCD_MASK;	//bit clock generated internally, master mode
			else
				I2S_RCR2_REG(I2S0_BASE_PTR) &= ~I2S_RCR2_BCD_MASK;	//bit clock generated externally, slave mode
		}
	}
	else if(port == 1)
	{	/* SAI1 init */
		if(tx)
		{
		  	I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_MSEL_MASK;	//clear master clock select
			I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_DIV_MASK;		//clear master clock divide
			
			I2S_TCR2_REG(I2S1_BASE_PTR) |= I2S_TCR2_MSEL(mclksel)	//master clock select
								| I2S_TCR2_DIV(bclkdiv)		//divide down master clock to generate bit clock
								;
			if(bclkpol)
				I2S_TCR2_REG(I2S1_BASE_PTR) |= I2S_TCR2_BCP_MASK;	//bit clock is active low
			else
				I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_BCP_MASK;	//bit clock is active high
				
			if(bclkdir)
				I2S_TCR2_REG(I2S1_BASE_PTR) |= I2S_TCR2_BCD_MASK;	//bit clock generated internally, master mode
			else
				I2S_TCR2_REG(I2S1_BASE_PTR) &= ~I2S_TCR2_BCD_MASK;	//bit clock generated externally, slave mode
		}
		else
		{
		  	I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_MSEL_MASK;	//clear master clock select
			I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_DIV_MASK;		//clear master clock divide
			
			I2S_RCR2_REG(I2S1_BASE_PTR) |= I2S_RCR2_MSEL(mclksel)	//master clock select
								| I2S_RCR2_DIV(bclkdiv)		//divide down master clock to generate bit clock
								;
			if(bclkpol)
				I2S_RCR2_REG(I2S1_BASE_PTR) |= I2S_RCR2_BCP_MASK;	//bit clock is active low
			else
				I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_BCP_MASK;	//bit clock is active high
				
			if(bclkdir)
				I2S_RCR2_REG(I2S1_BASE_PTR) |= I2S_RCR2_BCD_MASK;	//bit clock generated internally, master mode
			else
				I2S_RCR2_REG(I2S1_BASE_PTR) &= ~I2S_RCR2_BCD_MASK;	//bit clock generated externally, slave mode
		}
	}
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_bclk_en(unsigned char port, unsigned tx)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_BCE_MASK;	//transmit enable
		else
			I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_BCE_MASK;	//receive enable
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_BCE_MASK;	//transmit enable
		else
			I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_BCE_MASK;	//receive enable
	}
}

/*
*	Input parameter:
*	port: SAI module used
*	tx: 	whether to configure tx or rx registers
*/
void sai_bclk_dis(unsigned char port, unsigned tx)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_BCE_MASK;	//transmit enable
		else
			I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_BCE_MASK;	//receive enable
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_BCE_MASK;	//transmit enable
		else
			I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_BCE_MASK;	//receive enable
	}
}


/*
*	Input parameter:
*	port:		SAI module used
*	tx:		transmit or receive
*	chen:		channel enable, 0x01 enable channel one, 0x02 enable channel two, 0x03 enable both channel 
*/
void sai_channel_enable(unsigned char port, unsigned char tx, unsigned char chen)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCR3_REG(I2S0_BASE_PTR) |= I2S_TCR3_TCE(chen);		//tx channel enable
		else
			I2S_RCR3_REG(I2S0_BASE_PTR) |= I2S_RCR3_RCE(chen);		//rx channel enable
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCR3_REG(I2S1_BASE_PTR) |= I2S_TCR3_TCE(chen);		//tx channel enable
		else
			I2S_RCR3_REG(I2S1_BASE_PTR) |= I2S_RCR3_RCE(chen);		//rx channel enable
	}
}

/*
*	Input parameter:
*	port:		SAI module used
*	tx:		transmit or receive
*	chen:		channel disable, 0x01 disable channel one, 0x02 disable channel two, 0x03 disable both channel 
*/
void sai_channel_disable(unsigned char port, unsigned char tx, unsigned char chen)
{
	if(port == 0)
	{
		if(tx)
			I2S_TCR3_REG(I2S0_BASE_PTR) &= ~I2S_TCR3_TCE(chen);		//tx channel enable
		else
			I2S_RCR3_REG(I2S0_BASE_PTR) &= ~I2S_RCR3_RCE(chen);		//rx channel enable
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TCR3_REG(I2S1_BASE_PTR) &= ~I2S_TCR3_TCE(chen);		//tx channel enable
		else
			I2S_RCR3_REG(I2S1_BASE_PTR) &= ~I2S_RCR3_RCE(chen);		//rx channel enable
	}
}
/*
*	Input parameter
*	port:		SAI module used
*	tx:		transmit or receive
*	wdfl:		configure which word the start of flag is set, should be configured less than frame size
*/
void sai_word_flg_config(unsigned char port, unsigned char tx, unsigned char wdfl)
{
	if(port == 0)
	{
		if(tx)
		{
		  	I2S_TCR3_REG(I2S0_BASE_PTR) &= ~I2S_TCR3_WDFL_MASK;
			I2S_TCR3_REG(I2S0_BASE_PTR) |= I2S_TCR3_WDFL(wdfl);		//word flag config
		}
		else
		{
			I2S_RCR3_REG(I2S0_BASE_PTR) &= ~I2S_RCR3_WDFL_MASK;
			I2S_RCR3_REG(I2S0_BASE_PTR) |= I2S_RCR3_WDFL(wdfl);		//word flag config
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			I2S_TCR3_REG(I2S1_BASE_PTR) &= ~I2S_TCR3_WDFL_MASK;
			I2S_TCR3_REG(I2S1_BASE_PTR) |= I2S_TCR3_WDFL(wdfl);		//word flag config
		}
		else
		{
			I2S_RCR3_REG(I2S1_BASE_PTR) &= ~I2S_RCR3_WDFL_MASK;
			I2S_RCR3_REG(I2S1_BASE_PTR) |= I2S_RCR3_WDFL(wdfl);		//word flag config
		}
	}
	
}

/*
*	Input parameter:
*	port:			SAI module used
*	tx:			transmit or receive	
*	framesize:	 	configure number of words in each frame, maximum supported frame size is 32
*	sywd:			configure length of the frame sync in number of bit clocks
*	mf:			specify LSB or MSB first
*/
void sai_frame_config(unsigned char port, unsigned char tx, unsigned char framesize, unsigned char sywd, unsigned char mf)
{
	if(port == 0)
	{
		if(tx)
		{
		  	I2S_TCR4_REG(I2S0_BASE_PTR) &= ~I2S_TCR4_FRSZ_MASK;
			I2S_TCR4_REG(I2S0_BASE_PTR) &= ~I2S_TCR4_SYWD_MASK;
			
			I2S_TCR4_REG(I2S0_BASE_PTR) |= I2S_TCR4_FRSZ(framesize)	//configure number of words in each frame
								| I2S_TCR4_SYWD(sywd)	//configure length of the frame sync in number of bit clocks
								;
			if(mf)
				I2S_TCR4_REG(I2S0_BASE_PTR) |= I2S_TCR4_MF_MASK;	//MSB transmit first
			else
				I2S_TCR4_REG(I2S0_BASE_PTR) &= ~I2S_TCR4_MF_MASK;	//LSB transmit first					
		}
		else
		{
		  	I2S_RCR4_REG(I2S0_BASE_PTR) &= ~I2S_RCR4_FRSZ_MASK;
			I2S_RCR4_REG(I2S0_BASE_PTR) &= ~I2S_RCR4_SYWD_MASK;
			
			I2S_RCR4_REG(I2S0_BASE_PTR) |= I2S_RCR4_FRSZ(framesize)	//configure number of words in each frame
								| I2S_RCR4_SYWD(sywd)	//configure length of the frame sync in number of bit clocks
								;
			if(mf)
				I2S_RCR4_REG(I2S0_BASE_PTR) |= I2S_RCR4_MF_MASK;	//MSB transmit first
			else
				I2S_RCR4_REG(I2S0_BASE_PTR) &= ~I2S_RCR4_MF_MASK;	//LSB transmit first
		}	
	}
	else if(port == 1)
	{
		if(tx)
		{
		  	I2S_TCR4_REG(I2S1_BASE_PTR) &= ~I2S_TCR4_FRSZ_MASK;
			I2S_TCR4_REG(I2S1_BASE_PTR) &= ~I2S_TCR4_SYWD_MASK;
			
			I2S_TCR4_REG(I2S1_BASE_PTR) |= I2S_TCR4_FRSZ(framesize)	//configure number of words in each frame
								| I2S_TCR4_SYWD(sywd)	//configure length of the frame sync in number of bit clocks
								;
			if(mf)
				I2S_TCR4_REG(I2S1_BASE_PTR) |= I2S_TCR4_MF_MASK;	//MSB transmit first
			else
				I2S_TCR4_REG(I2S1_BASE_PTR) &= ~I2S_TCR4_MF_MASK;	//LSB transmit first					
		}
		else
		{
		  	I2S_RCR4_REG(I2S1_BASE_PTR) &= ~I2S_RCR4_FRSZ_MASK;
			I2S_RCR4_REG(I2S1_BASE_PTR) &= ~I2S_RCR4_SYWD_MASK;
			
			I2S_RCR4_REG(I2S1_BASE_PTR) |= I2S_RCR4_FRSZ(framesize)	//configure number of words in each frame
								| I2S_RCR4_SYWD(sywd)	//configure length of the frame sync in number of bit clocks
								;
			if(mf)
				I2S_RCR4_REG(I2S1_BASE_PTR) |= I2S_RCR4_MF_MASK;	//MSB transmit first
			else
				I2S_RCR4_REG(I2S1_BASE_PTR) &= ~I2S_RCR4_MF_MASK;	//LSB transmit first
		}	
	}
}

/*
*	Input parameter
*	port:		SAI module used
*	tx:		transmit or receive
*	fse:		frame sync early
*	fsp:		frame sync polarity
*	fsd:		frame sync direction
*/
void sai_frameclk_config(unsigned char port, unsigned char tx, unsigned char fse, unsigned char fsp, unsigned char fsd)
{
	if(port == 0)
	{
		if(tx)
		{
			if(fse)						
				I2S_TCR4_REG(I2S0_BASE_PTR) |= I2S_TCR4_FSE_MASK;	//frame sync early
			else
				I2S_TCR4_REG(I2S0_BASE_PTR) &= ~I2S_TCR4_FSE_MASK;
				
			if(fsp)						
				I2S_TCR4_REG(I2S0_BASE_PTR) |= I2S_TCR4_FSP_MASK;	//frame sync active low
			else
				I2S_TCR4_REG(I2S0_BASE_PTR) &= ~I2S_TCR4_FSP_MASK;	//frame sync active high
				
			if(fsd)						
				I2S_TCR4_REG(I2S0_BASE_PTR) |= I2S_TCR4_FSD_MASK;	//frame sync generated internally, master mode
			else
				I2S_TCR4_REG(I2S0_BASE_PTR) &= ~I2S_TCR4_FSD_MASK;	//frame sync generated externally, slave mode
		}
		else
		{
			if(fse)						
				I2S_RCR4_REG(I2S0_BASE_PTR) |= I2S_RCR4_FSE_MASK;	//frame sync early
			else
				I2S_RCR4_REG(I2S0_BASE_PTR) &= ~I2S_RCR4_FSE_MASK;
				
			if(fsp)						
				I2S_RCR4_REG(I2S0_BASE_PTR) |= I2S_RCR4_FSP_MASK;	//frame sync active low
			else
				I2S_RCR4_REG(I2S0_BASE_PTR) &= ~I2S_RCR4_FSP_MASK;	//frame sync active high
				
			if(fsd)						
				I2S_RCR4_REG(I2S0_BASE_PTR) |= I2S_RCR4_FSD_MASK;	//frame sync generated internally, master mode
			else
				I2S_RCR4_REG(I2S0_BASE_PTR) &= ~I2S_RCR4_FSD_MASK;	//frame sync generated externally, slave mode
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			if(fse)						
				I2S_TCR4_REG(I2S1_BASE_PTR) |= I2S_TCR4_FSE_MASK;	//frame sync early
			else
				I2S_TCR4_REG(I2S1_BASE_PTR) &= ~I2S_TCR4_FSE_MASK;
				
			if(fsp)						
				I2S_TCR4_REG(I2S1_BASE_PTR) |= I2S_TCR4_FSP_MASK;	//frame sync active low
			else
				I2S_TCR4_REG(I2S1_BASE_PTR) &= ~I2S_TCR4_FSP_MASK;	//frame sync active high
				
			if(fsd)						
				I2S_TCR4_REG(I2S1_BASE_PTR) |= I2S_TCR4_FSD_MASK;	//frame sync generated internally, master mode
			else
				I2S_TCR4_REG(I2S1_BASE_PTR) &= ~I2S_TCR4_FSD_MASK;	//frame sync generated externally, slave mode
		}
		else
		{
			if(fse)						
				I2S_RCR4_REG(I2S1_BASE_PTR) |= I2S_RCR4_FSE_MASK;	//frame sync early
			else
				I2S_RCR4_REG(I2S1_BASE_PTR) &= ~I2S_RCR4_FSE_MASK;
				
			if(fsp)						
				I2S_RCR4_REG(I2S1_BASE_PTR) |= I2S_RCR4_FSP_MASK;	//frame sync active low
			else
				I2S_RCR4_REG(I2S1_BASE_PTR) &= ~I2S_RCR4_FSP_MASK;	//frame sync active high
				
			if(fsd)						
				I2S_RCR4_REG(I2S1_BASE_PTR) |= I2S_RCR4_FSD_MASK;	//frame sync generated internally, master mode
			else
				I2S_RCR4_REG(I2S1_BASE_PTR) &= ~I2S_RCR4_FSD_MASK;	//frame sync generated externally, slave mode
		}
	}
}

/*
*	Input parameter
*	port:		SAI module used
*	tx:		transmit or receive
*	wnw:		word N width
*	w0w:		word 0 width
*	fbt:		configure the bit index of the first bit transmitted for each word in the frame
*/
void sai_wordlength_config(unsigned char port, unsigned char tx, unsigned char wnw, unsigned char w0w, unsigned char fbt)
{
	if(port == 0)
	{
		if(tx)
		{
		  	I2S_TCR5_REG(I2S0_BASE_PTR) &= ~I2S_TCR5_WNW_MASK;
			I2S_TCR5_REG(I2S0_BASE_PTR) &= ~I2S_TCR5_W0W_MASK;
			I2S_TCR5_REG(I2S0_BASE_PTR) &= ~I2S_TCR5_FBT_MASK;
			I2S_TCR5_REG(I2S0_BASE_PTR) |= I2S_TCR5_WNW(wnw)	//configure number of bits in each word except first in frame
						| I2S_TCR5_W0W(w0w)		//configure number of bits for first word in each frame
						| I2S_TCR5_FBT(fbt)		//configure bit index of first bit transmitted of each word
						;
		}
		else
		{
		  	I2S_RCR5_REG(I2S0_BASE_PTR) &= ~I2S_RCR5_WNW_MASK;
			I2S_RCR5_REG(I2S0_BASE_PTR) &= ~I2S_RCR5_W0W_MASK;
			I2S_RCR5_REG(I2S0_BASE_PTR) &= ~I2S_RCR5_FBT_MASK;
			I2S_RCR5_REG(I2S0_BASE_PTR) |= I2S_RCR5_WNW(wnw)	//configure number of bits in each word except first in frame
						| I2S_RCR5_W0W(w0w)		//configure number of bits for first word in each frame
						| I2S_RCR5_FBT(fbt)		//configure bit index of first bit transmitted of each word
						;
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
		  	I2S_TCR5_REG(I2S1_BASE_PTR) &= ~I2S_TCR5_WNW_MASK;
			I2S_TCR5_REG(I2S1_BASE_PTR) &= ~I2S_TCR5_W0W_MASK;
			I2S_TCR5_REG(I2S1_BASE_PTR) &= ~I2S_TCR5_FBT_MASK;
			I2S_TCR5_REG(I2S1_BASE_PTR) |= I2S_TCR5_WNW(wnw)	//configure number of bits in each word except first in frame
						| I2S_TCR5_W0W(w0w)		//configure number of bits for first word in each frame
						| I2S_TCR5_FBT(fbt)		//configure bit index of first bit transmitted of each word
						;
		}
		else
		{
		  	I2S_RCR5_REG(I2S1_BASE_PTR) &= ~I2S_RCR5_WNW_MASK;
			I2S_RCR5_REG(I2S1_BASE_PTR) &= ~I2S_RCR5_W0W_MASK;
			I2S_RCR5_REG(I2S1_BASE_PTR) &= ~I2S_RCR5_FBT_MASK;
			I2S_RCR5_REG(I2S1_BASE_PTR) |= I2S_RCR5_WNW(wnw)	//configure number of bits in each word except first in frame
						| I2S_RCR5_W0W(w0w)		//configure number of bits for first word in each frame
						| I2S_RCR5_FBT(fbt)		//configure bit index of first bit transmitted of each word
						;
		}
	}
}

/*
*	Input parameter:
*	port: 	SAI module used
*	channel:	which channel to fill in data, only two channels for SAI module on P3, corresponds to TX0 or TX1
*	txdata:	sending data
*/
unsigned char sai_tx(unsigned char port, unsigned char channel, unsigned int txdata)
{
	if(sai_tx_fifo_status(port, channel) == FIFO_FULL)
	{
		printf("tx FIFO already full\n");
		return ERROR;
	}
	if(port == 0)
	{
		I2S_TDR_REG(I2S0_BASE_PTR, channel) = txdata;
	}
	else if(port == 1)
	{
		I2S_TDR_REG(I2S1_BASE_PTR, channel) = txdata;
	}
	
	return OK;
}

/* 
*	Function:	
*	Direct write tdr, no checking on whether FIFO is full or not 
*	Input parameter:
*	port: 	SAI module used
*	channel:	which channel to fill in data, only two channels for SAI module on P3, corresponds to TX0 or TX1
*	txdata:	sending data
*/
void sai_wr_tdr(unsigned char port, unsigned char channel, unsigned int txdata)
{
	if(port == 0)
	{
		I2S_TDR_REG(I2S0_BASE_PTR, channel) = txdata;
	}
	else if(port == 1)
	{
		I2S_TDR_REG(I2S1_BASE_PTR, channel) = txdata;
	}
	
}

/*
*	Input parameter:
*	port: 	SAI module used
*	channel:	which channel to read data from, only two channels for SAI module on P3, corresponds to RX0 or RX1
*	rxdata:	pointer to received data
*/
unsigned char sai_rx(unsigned char port, unsigned char channel, unsigned int *rxdata)
{
	if(sai_rx_fifo_status(port, channel) == FIFO_EMPTY)
	{
		printf("rx FIFO is empty\n");
		return ERROR;
	}
	
	if(port == 0)
	{
		*rxdata = I2S_RDR_REG(I2S0_BASE_PTR, channel); 
	}
	else if(port == 1)
	{
		*rxdata = I2S_RDR_REG(I2S1_BASE_PTR, channel); 
	}
	
	return OK;
}

/* 
*	Function:
*	direct read from rdr, no checking on whether FIFO is empty or not 
*	Input parameter:
*	port: 	SAI module used
*	channel:	which channel to read data from, only two channels for SAI module on P3, corresponds to RX0 or RX1
*	rxdata:	pointer to received data
*/
void sai_rd_rdr(unsigned char port, unsigned char channel, unsigned int *rxdata)
{

	if(port == 0)
	{
		*rxdata = I2S_RDR_REG(I2S0_BASE_PTR, channel); 
	}
	else if(port == 1)
	{
		*rxdata = I2S_RDR_REG(I2S1_BASE_PTR, channel); 
	}
}

/*
*	Input parameter:
*	port: 	SAI module used
*	channel:	Channel for checking FIFO status, only two channels for SAI module on P3, corresponds to TX0 or TX1
*/
unsigned char sai_tx_fifo_status(unsigned char port, unsigned char channel)
{
	unsigned char wr_fp = 0;	//write FIFO pointer
	unsigned char rd_fp = 0;	//read FIFO pointer
	
	if(port == 0)
	{
		wr_fp = (I2S_TFR_REG(I2S0_BASE_PTR, channel)&I2S_TFR_WFP_MASK)>>I2S_TFR_WFP_SHIFT;
		rd_fp = (I2S_TFR_REG(I2S0_BASE_PTR, channel)&I2S_TFR_RFP_MASK)>>I2S_TFR_RFP_SHIFT;
		if(wr_fp == rd_fp)
			return FIFO_EMPTY;
		else if((wr_fp&0x08) != (rd_fp&0x08))	
			return FIFO_FULL;
	}
	else if(port == 1)
	{
		wr_fp = (I2S_TFR_REG(I2S1_BASE_PTR, channel)&I2S_TFR_WFP_MASK)>>I2S_TFR_WFP_SHIFT;
		rd_fp = (I2S_TFR_REG(I2S1_BASE_PTR, channel)&I2S_TFR_RFP_MASK)>>I2S_TFR_RFP_SHIFT;
		if(wr_fp == rd_fp)
			return FIFO_EMPTY;
		else if((wr_fp&0x08) != (rd_fp&0x08))	
			return FIFO_FULL;
	}
	
	return ERROR;
}

/*
*	Input parameter:
*	port: 	SAI module used
*	channel:	Channel for checking FIFO status, only two channels for SAI module on P3, corresponds to RX0 or RX1
*/
unsigned char sai_rx_fifo_status(unsigned char port, unsigned char channel)
{
	unsigned char wr_fp = 0;	//write FIFO pointer
	unsigned char rd_fp = 0;	//read FIFO pointer
	
	if(port == 0)
	{
		wr_fp = (I2S_RFR_REG(I2S0_BASE_PTR, channel)&I2S_RFR_WFP_MASK)>>I2S_RFR_WFP_SHIFT;
		rd_fp = (I2S_RFR_REG(I2S0_BASE_PTR, channel)&I2S_RFR_RFP_MASK)>>I2S_RFR_RFP_SHIFT;
		if(wr_fp == rd_fp)
			return FIFO_EMPTY;
		else if((wr_fp&0x08) != (rd_fp&0x08))	
			return FIFO_FULL;
	}
	else if(port == 1)
	{
		wr_fp = (I2S_RFR_REG(I2S1_BASE_PTR, channel)&I2S_RFR_WFP_MASK)>>I2S_RFR_WFP_SHIFT;
		rd_fp = (I2S_RFR_REG(I2S1_BASE_PTR, channel)&I2S_RFR_RFP_MASK)>>I2S_RFR_RFP_SHIFT;
		if(wr_fp == rd_fp)
			return FIFO_EMPTY;
		else if((wr_fp&0x08) != (rd_fp&0x08))	
			return FIFO_FULL;
	}
	
	return ERROR;
}

/*
*	Input parameter
*	port:		SAI module used
*	tx:		transmit or receive	
*	mask:		mask for each word in frame, if any bit in mask is set, then that word is tristated
*/
void sai_mask_config(unsigned char port, unsigned char tx, unsigned int mask)
{
	if(port == 0)
	{
		if(tx)
			I2S_TMR_REG(I2S0_BASE_PTR) = mask;	//tx word mask
		else
			I2S_RMR_REG(I2S0_BASE_PTR) = mask;	//rx word mask
	}
	else if(port == 1)
	{
		if(tx)
			I2S_TMR_REG(I2S1_BASE_PTR) = mask;	//tx word mask
		else
			I2S_RMR_REG(I2S1_BASE_PTR) = mask;	//rx word mask
	}
}

/*
*	Input parameter:
*	port: 	SAI module used
*	mics:		MCLK divide clock select
*	moe:		master clock enable, if moe=1, then I2Sx_MCLK pin outputs internally generated master clock, otherwise, it can be used to 
*			input externally generated master clock
*	fract:	MCLK fraction
*	divide:	MCLK divide
*/
void sai_mclk_config(unsigned char port, unsigned char mics, unsigned char moe, unsigned char fract, unsigned char divide)
{
	if(port == 0)
	{	
		/*	
		*	MICS		MCLK divider input clock
		*	00		system clock
		*	01		OSC0ERCLK
		*	10		OSC1ERCLK
		*	11		MCGPLLCLK
		*/
		I2S_MCR_REG(I2S0_BASE_PTR) &= ~I2S_MCR_MICS_MASK;
		I2S_MCR_REG(I2S0_BASE_PTR) |= I2S_MCR_MICS(mics);	//MCLK divider input clock select	
		
		/*
		*	MCLK output = MCLK input * ((FRACT+1)/(DIVIDE+1))
		*/
		I2S_MDR_REG(I2S0_BASE_PTR) &= ~I2S_MDR_FRACT_MASK;
		I2S_MDR_REG(I2S0_BASE_PTR) &= ~I2S_MDR_DIVIDE_MASK;
		I2S_MDR_REG(I2S0_BASE_PTR) |= I2S_MDR_FRACT(fract)	//MCLK fraction
						| I2S_MDR_DIVIDE(divide)		//MCLK divide
						;		
						
		/* 
		*	Please note that MICS field cannot be changed once MCLK divider enabled, because MOE bit will enable MCLK divider and configure MCLK pin as output,  
		*	so MOE bit needs to be set after setting MICS, otherwise the change to MICS bit will not be valid!!!
		*/				
		if(moe)
			I2S_MCR_REG(I2S0_BASE_PTR) |= I2S_MCR_MOE_MASK;		//SAI_MCLK configure as output from MCLK divider
		else
			I2S_MCR_REG(I2S0_BASE_PTR) &= ~I2S_MCR_MOE_MASK;	//SAI_MCLK configured as input which bypass MCLK divider				
	}
	else if(port == 1)
	{
		/*	
		*	MICS		MCLK divider input clock
		*	00		system clock
		*	01		OSC0ERCLK
		*	10		OSC1ERCLK
		*	11		MCGPLLCLK
		*/
		I2S_MCR_REG(I2S1_BASE_PTR) &= ~I2S_MCR_MICS_MASK;
		I2S_MCR_REG(I2S1_BASE_PTR) |= I2S_MCR_MICS(mics);	//MCLK divider input clock select	
		
		/*
		*	MCLK output = MCLK input * ((FRACT+1)/(DIVIDE+1))
		*/
		I2S_MDR_REG(I2S0_BASE_PTR) &= ~I2S_MDR_FRACT_MASK;
		I2S_MDR_REG(I2S0_BASE_PTR) &= ~I2S_MDR_DIVIDE_MASK;
		I2S_MDR_REG(I2S1_BASE_PTR) |= I2S_MDR_FRACT(fract)	//MCLK fraction
						| I2S_MDR_DIVIDE(divide)		//MCLK divide
						;		
						
		/* 
		*	Please note that MICS field cannot be changed once MCLK divider enabled, because MOE bit will enable MCLK divider and configure MCLK pin as output,  
		*	so MOE bit needs to be set after setting MICS, otherwise the change to MICS bit will not be valid!!!
		*/				
		if(moe)
			I2S_MCR_REG(I2S1_BASE_PTR) |= I2S_MCR_MOE_MASK;		//SAI_MCLK configure as output from MCLK divider
		else
			I2S_MCR_REG(I2S1_BASE_PTR) &= ~I2S_MCR_MOE_MASK;	//SAI_MCLK configured as input which bypass MCLK divider	
	}
}

/*
*	Input parameter
*	port:		SAI module used
*/
void sai_clear_reg(unsigned char port)
{
	
	if(port == 0)
	{	
		I2S_TCSR_REG(I2S0_BASE_PTR) = 0x001c0000;	//clear wsf, sef and fef flags
		I2S_TCR1_REG(I2S0_BASE_PTR) = 0x0;
		I2S_TCR2_REG(I2S0_BASE_PTR) = 0x0;
		I2S_TCR3_REG(I2S0_BASE_PTR) = 0x0;
		I2S_TCR4_REG(I2S0_BASE_PTR) = 0x0;
		I2S_TCR5_REG(I2S0_BASE_PTR) = 0x0;
		//soft reset, reset internal tranmit logic and FIFO pointers, this operation will clear TDR and TFR, don't try to write 0 to TDR for reg clear, any write to TDR will increment TFR
		sai_soft_reset(port, TX);	
		I2S_TMR_REG(I2S0_BASE_PTR) = 0x0;	
		
		/* Loop till TE bit is cleared */
		while(I2S_TCSR_REG(I2S0_BASE_PTR)&I2S_TCSR_TE_MASK)
			I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_TE_MASK;	//clear TE 
		
		/* Loop till BCE bit cleared */
		while(I2S_TCSR_REG(I2S0_BASE_PTR)&I2S_TCSR_BCE_MASK)
			I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_BCE_MASK;	//clear BE 
			
		I2S_RCSR_REG(I2S0_BASE_PTR) = 0x001c0000;	//clear wsf, sef and fef flags
		I2S_RCR1_REG(I2S0_BASE_PTR) = 0x0;
		I2S_RCR2_REG(I2S0_BASE_PTR) = 0x0;
		I2S_RCR3_REG(I2S0_BASE_PTR) = 0x0;
		I2S_RCR4_REG(I2S0_BASE_PTR) = 0x0;
		I2S_RCR5_REG(I2S0_BASE_PTR) = 0x0;
		//soft reset, reset internal receive logic and FIFO pointers, this operation will clear RDR and RFR, don't try to write 0 to RDR for reg clear, any write to RDR will increment RFR
		sai_soft_reset(port, RX);
		I2S_RMR_REG(I2S0_BASE_PTR) = 0x0;
		
		/* Loop till RE bit clear */
		while(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_RE_MASK)
			I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_RE_MASK;	//clear RE 

		/* Loop till BCE bit clear */
		while(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_BCE_MASK)
			I2S_RCSR_REG(I2S0_BASE_PTR) &= ~I2S_RCSR_BCE_MASK;	//clear BCE 
					
		//Clearing MCR register is a bit tricky, first disable MOE bit, then clear MDR, then change MICS to 0x0
		I2S_MCR_REG(I2S0_BASE_PTR) &= ~I2S_MCR_MOE_MASK;
		I2S_MDR_REG(I2S0_BASE_PTR) = 0x0;
		I2S_MCR_REG(I2S0_BASE_PTR) = I2S_MCR_MICS(0x0);		
		
	}
	else if(port == 1)
	{		
		I2S_TCSR_REG(I2S1_BASE_PTR) = 0x001c0000;	//clear wsf, sef and fef flags
		I2S_TCR1_REG(I2S1_BASE_PTR) = 0x0;
		I2S_TCR2_REG(I2S1_BASE_PTR) = 0x0;
		I2S_TCR3_REG(I2S1_BASE_PTR) = 0x0;
		I2S_TCR4_REG(I2S1_BASE_PTR) = 0x0;
		I2S_TCR5_REG(I2S1_BASE_PTR) = 0x0;
		//soft reset, reset internal tranmit logic and FIFO pointers, this operation will clear TDR and TFR, don't try to write 0 to TDR for reg clear, any write to TDR will increment TFR
		sai_soft_reset(port, TX);	
		I2S_TMR_REG(I2S1_BASE_PTR) = 0x0;
		
		/* Loop till TE bit is cleared */
		while(I2S_TCSR_REG(I2S1_BASE_PTR)&I2S_TCSR_TE_MASK)
			I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_TE_MASK;	//clear TE 
		
		/* Loop till BCE bit cleared */
		while(I2S_TCSR_REG(I2S1_BASE_PTR)&I2S_TCSR_BCE_MASK)
			I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_BCE_MASK;	//clear BE 
					
		I2S_RCSR_REG(I2S1_BASE_PTR) = 0x001c0000;	//clear wsf, sef and fef flags
		I2S_RCR1_REG(I2S1_BASE_PTR) = 0x0;
		I2S_RCR2_REG(I2S1_BASE_PTR) = 0x0;
		I2S_RCR3_REG(I2S1_BASE_PTR) = 0x0;
		I2S_RCR4_REG(I2S1_BASE_PTR) = 0x0;
		I2S_RCR5_REG(I2S1_BASE_PTR) = 0x0;
		//soft reset, reset internal receive logic and FIFO pointers, this operation will clear RDR and RFR, don't try to write 0 to RDR for reg clear, any write to RDR will increment RFR
		sai_soft_reset(port, RX);
		I2S_RMR_REG(I2S1_BASE_PTR) = 0x0;
		
		/* Loop till RE bit clear */
		while(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_RE_MASK)
			I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_RE_MASK;	//clear RE 

		/* Loop till BCE bit clear */
		while(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_BCE_MASK)
			I2S_RCSR_REG(I2S1_BASE_PTR) &= ~I2S_RCSR_BCE_MASK;	//clear BCE 
		
		//Clearing MCR register is a bit tricky, first disable MOE bit, then clear MDR, then change MICS to 0x0
		I2S_MCR_REG(I2S1_BASE_PTR) &= ~I2S_MCR_MOE_MASK;
		I2S_MDR_REG(I2S1_BASE_PTR) = 0x0;
		I2S_MCR_REG(I2S1_BASE_PTR) = I2S_MCR_MICS(0x0);	
	}
	
}

/*
*	Input parameter:
*	port:		SAI module used
*	channel:	select channel 0 or channel 1
*/
void sai_monitor_fifo_status(unsigned char port, unsigned char channel)
{
	if(port == 0)
	{
		printf("I2S_TFR_REG(I2S0_BASE_PTR,%d) is 0x%x\n", channel, I2S_TFR_REG(I2S0_BASE_PTR,channel));
		printf("I2S_RFR_REG(I2S0_BASE_PTR,%d) is 0x%x\n", channel, I2S_RFR_REG(I2S0_BASE_PTR,channel));
	}
	else if(port == 1)
	{
		printf("I2S_TFR_REG(I2S1_BASE_PTR,%d) is 0x%x\n", channel, I2S_TFR_REG(I2S1_BASE_PTR,channel));
		printf("I2S_RFR_REG(I2S1_BASE_PTR,%d) is 0x%x\n", channel, I2S_RFR_REG(I2S1_BASE_PTR,channel));
	}
}

/*
*	Input parameter:
*	port:		SAI module used
*	tx:		whether it's for tx or rx
*/
void sai_inteprete_flag(unsigned char port, unsigned char tx)
{
	if(port == 0)
	{
		if(tx)
		{
			printf("I2S_TCSR_REG(I2S0_BASE_PTR) is 0x%x\n", I2S_TCSR_REG(I2S0_BASE_PTR));
			if(I2S_TCSR_REG(I2S0_BASE_PTR) & I2S_TCSR_WSF_MASK)	
			{	//indicate start of configured word has been detected
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_WSF_MASK;	//clear flag
				printf("word start flag set\n");
			}
			else if(I2S_TCSR_REG(I2S0_BASE_PTR) & I2S_TCSR_SEF_MASK)	
			{	//indicate error in the externally generated frame sync has been detected
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_SEF_MASK;
				printf("sync error flag set\n");
			}
			else if(I2S_TCSR_REG(I2S0_BASE_PTR) & I2S_TCSR_FEF_MASK)
			{	//indicate an enabled transmit FIFO has underrun
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FEF_MASK;	//clear flag
				printf("fifo error flag set\n");
			}
			else if(I2S_TCSR_REG(I2S0_BASE_PTR) & I2S_TCSR_FWF_MASK)
			{	//indicate an enabled transmit FIFO is empty
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FWF_MASK;	//clear flag
				printf("fifo warning flag set\n");
			}
			else if(I2S_TCSR_REG(I2S0_BASE_PTR) & I2S_TCSR_FRF_MASK)
			{	//indicates number of words in an enabled transmit FIFO is less than or equal to the transmit FIFO watermark
				I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FRF_MASK;	//clear flag
				printf("fifo request flag set\n");		
			}
		}	
		else 
		{
			printf("I2S_RCSR_REG(I2S0_BASE_PTR) is 0x%x\n", I2S_RCSR_REG(I2S0_BASE_PTR));
			if(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_WSF_MASK)	
			{	//indicate start of configured word has been detected
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_WSF_MASK;	//clear flag
				printf("word start flag set\n");
			}
			else if(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_SEF_MASK)	
			{	//indicate error in the externally generated frame sync has been detected
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_SEF_MASK;
				printf("sync error flag set\n");
			}
			else if(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_FEF_MASK)
			{	//indicate an enabled receive FIFO has overflowed
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FEF_MASK;	//clear flag
				printf("fifo error flag set\n");
			}
			else if(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_FWF_MASK)
			{	//indicate an enabled receive FIFO is full
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FWF_MASK;	//clear flag
				printf("fifo warning flag set\n");
			}
			else if(I2S_RCSR_REG(I2S0_BASE_PTR) & I2S_RCSR_FRF_MASK)
			{	//indicates number of words in an enabled receive FIFO is greater than the receive FIFO watermark
				I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FRF_MASK;	//clear flag
				printf("fifo request flag set\n");		
			}
		}
	}
	else if(port == 1)
	{
		if(tx)
		{
			printf("I2S_TCSR_REG(I2S1_BASE_PTR) is 0x%x\n", I2S_TCSR_REG(I2S1_BASE_PTR));
			if(I2S_TCSR_REG(I2S1_BASE_PTR) & I2S_TCSR_WSF_MASK)	
			{	//indicate start of configured word has been detected
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_WSF_MASK;	//clear flag
				printf("word start flag set\n");
			}
			else if(I2S_TCSR_REG(I2S1_BASE_PTR) & I2S_TCSR_SEF_MASK)	
			{	//indicate error in the externally generated frame sync has been detected
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_SEF_MASK;
				printf("sync error flag set\n");
			}
			else if(I2S_TCSR_REG(I2S1_BASE_PTR) & I2S_TCSR_FEF_MASK)
			{	//indicate an enabled transmit FIFO has underrun
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FEF_MASK;	//clear flag
				printf("fifo error flag set\n");
			}
			else if(I2S_TCSR_REG(I2S1_BASE_PTR) & I2S_TCSR_FWF_MASK)
			{	//indicate an enabled transmit FIFO is empty
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FWF_MASK;	//clear flag
				printf("fifo warning flag set\n");
			}
			else if(I2S_TCSR_REG(I2S1_BASE_PTR) & I2S_TCSR_FRF_MASK)
			{	//indicates number of words in an enabled transmit FIFO is less than or equal to the transmit FIFO watermark
				I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FRF_MASK;	//clear flag
				printf("fifo request flag set\n");		
			}
		}	
		else 
		{
			printf("I2S_RCSR_REG(I2S1_BASE_PTR) is 0x%x\n", I2S_RCSR_REG(I2S1_BASE_PTR));
			if(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_WSF_MASK)	
			{	//indicate start of configured word has been detected
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_WSF_MASK;	//clear flag
				printf("word start flag set\n");
			}
			else if(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_SEF_MASK)	
			{	//indicate error in the externally generated frame sync has been detected
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_SEF_MASK;
				printf("sync error flag set\n");
			}
			else if(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_FEF_MASK)
			{	//indicate an enabled receive FIFO has overflowed
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FEF_MASK;	//clear flag
				printf("fifo error flag set\n");
			}
			else if(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_FWF_MASK)
			{	//indicate an enabled receive FIFO is full
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FWF_MASK;	//clear flag
				printf("fifo warning flag set\n");
			}
			else if(I2S_RCSR_REG(I2S1_BASE_PTR) & I2S_RCSR_FRF_MASK)
			{	//indicates number of words in an enabled receive FIFO is greater than the receive FIFO watermark
				I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FRF_MASK;	//clear flag
				printf("fifo request flag set\n");		
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Interrupt service routines */
void sai0_tx_isr(void)
{
	unsigned char cnt;
	unsigned int tcsr = I2S_TCSR_REG(I2S0_BASE_PTR);	//transmit control register
	unsigned int tfr0 = I2S_TFR_REG(I2S0_BASE_PTR, 0);	//channel 0 tx FIFO register
	unsigned int tfr1 = I2S_TFR_REG(I2S0_BASE_PTR, 1);	//channel 1 tx FIFO register
	
	unsigned char wfp;	//write FIFO pointer
	unsigned char rfp;	//read FIFO pointer
	
	unsigned char fifo0_cnt;	//current FIFO cnt for channel 0 
	unsigned char fifo1_cnt;	//current FIFO cnt for channel 1
	
	/* Calculate channel 0 FIFO cnt */
	wfp = (tfr0&I2S_TFR_WFP_MASK)>>I2S_TFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (tfr0&I2S_TFR_RFP_MASK)>>I2S_TFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo0_cnt = wfp - rfp;
	else
		fifo0_cnt = 16 - (rfp - wfp);
				
	/* Calculate channel 1 FIFO cnt */
	wfp = (tfr1&I2S_TFR_WFP_MASK)>>I2S_TFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (tfr1&I2S_TFR_RFP_MASK)>>I2S_TFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo1_cnt = wfp - rfp;
	else
		fifo1_cnt = 16 - (rfp - wfp);		
	
	if(tcsr & I2S_TCSR_WSF_MASK)	
	{	//indicate start of configured word has been detected
		I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_WSF_MASK;	//clear flag
		//printf("word start flag set\n");
		GPIOF_PTOR = 0x40;	//toggle PTF6 (J62 pin13)
	}
	else if(tcsr & I2S_TCSR_SEF_MASK)	
	{	//indicate error in the externally generated frame sync has been detected
		I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_SEF_MASK;
		//printf("sync error flag set\n");
	}
	else if(tcsr & I2S_TCSR_FEF_MASK)
	{	//indicate an enabled transmit FIFO has underrun
		I2S_TCSR_REG(I2S0_BASE_PTR) |= I2S_TCSR_FEF_MASK;	//clear flag
		//printf("fifo error flag set\n");
	}
	else if(tcsr & I2S_TCSR_FWF_MASK)
	{	//indicate an enabled transmit FIFO is empty
		I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_FWF_MASK;	//clear flag
		
		/* Since FIFO is empty, so we will fill all 8 entries */
		for(cnt = 0; cnt < 8; cnt++)
		{
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else
				I2S_TDR_REG(I2S0_BASE_PTR, 0) = tx_buffer[tx_index++];
			
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;	
			else	
				I2S_TDR_REG(I2S0_BASE_PTR, 1) = tx_buffer[tx_index++];
				
		}
		//printf("fifo warning flag set\n");
	}
	else if(tcsr & I2S_TCSR_FRF_MASK)
	{	//indicates number of words in an enabled transmit FIFO is less than or equal to the transmit FIFO watermark
		I2S_TCSR_REG(I2S0_BASE_PTR) &= ~I2S_TCSR_FRF_MASK;	//clear flag
		
		for(cnt = 0; cnt < 8 - fifo0_cnt; cnt++)
		{
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else
				I2S_TDR_REG(I2S0_BASE_PTR, 0) = tx_buffer[tx_index++];
			
		}
		
		for(cnt = 0; cnt < 8 - fifo1_cnt; cnt++)
		{
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else
				I2S_TDR_REG(I2S0_BASE_PTR, 1) = tx_buffer[tx_index++];
			
		}
		//printf("fifo request flag set\n");		
	}
}

void sai0_rx_isr(void)
{
	unsigned char cnt;
	unsigned int rcsr = I2S_RCSR_REG(I2S0_BASE_PTR);
	unsigned int rfr0 = I2S_RFR_REG(I2S0_BASE_PTR, 0);	//channel 0 rx FIFO register
	unsigned int rfr1 = I2S_RFR_REG(I2S0_BASE_PTR, 1);	//channel 1 rx FIFO register
	
	unsigned char wfp;	//write FIFO pointer
	unsigned char rfp;	//read FIFO pointer
	
	unsigned char fifo0_cnt;	//current FIFO cnt for channel 0 
	unsigned char fifo1_cnt;	//current FIFO cnt for channel 1
	
	/* Calculate channel 0 FIFO cnt */
	wfp = (rfr0&I2S_RFR_WFP_MASK)>>I2S_RFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (rfr0&I2S_RFR_RFP_MASK)>>I2S_RFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo0_cnt = wfp - rfp;
	else
		fifo0_cnt = 16 - (rfp - wfp);
				
	/* Calculate channel 1 FIFO cnt */
	wfp = (rfr1&I2S_RFR_WFP_MASK)>>I2S_RFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (rfr1&I2S_RFR_RFP_MASK)>>I2S_RFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo1_cnt = wfp - rfp;
	else
		fifo1_cnt = 16 - (rfp - wfp);		
		
	if(rcsr & I2S_RCSR_WSF_MASK)	
	{	//indicate start of configured word has been detected
		I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_WSF_MASK;	//clear flag
		//printf("word start flag set\n");
	}
	else if(rcsr & I2S_RCSR_SEF_MASK)	
	{	//indicate error in the externally generated frame sync has been detected
		I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_SEF_MASK;
		printf("sync error flag set\n");
		GPIOF_PTOR = 0x20;	//toggle PTF5 (J62 pin11)
	}
	else if(rcsr & I2S_RCSR_FEF_MASK)
	{	//indicate an enabled receive FIFO has overflowed
		I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FEF_MASK;	//clear flag
		//printf("fifo error flag set\n");
	}
	else if(rcsr & I2S_RCSR_FWF_MASK)
	{	//indicate an enabled receive FIFO is full
		I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FWF_MASK;	//clear flag
		/* Since receive FIFO is full, we read all 8 entries */
		for(cnt = 0; cnt < 8; cnt++)
		{
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S0_BASE_PTR, 0);
			
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;	
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S0_BASE_PTR, 1);
			
		}

		//printf("fifo warning flag set\n");
	}
	else if(rcsr & I2S_RCSR_FRF_MASK)
	{	//indicates number of words in an enabled receive FIFO is greater than the receive FIFO watermark
		I2S_RCSR_REG(I2S0_BASE_PTR) |= I2S_RCSR_FRF_MASK;	//clear flag
		for(cnt = 0; cnt < fifo0_cnt; cnt++)
		{
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S0_BASE_PTR, 0);
			
		}
		
		for(cnt = 0; cnt < fifo1_cnt; cnt++)
		{
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S0_BASE_PTR, 1);
			
		}
		
		//printf("fifo request flag set\n");		
	}
}

void sai1_tx_isr(void)
{
	unsigned char cnt;
	unsigned int tcsr = I2S_TCSR_REG(I2S1_BASE_PTR);	//transmit control register
	unsigned int tfr0 = I2S_TFR_REG(I2S1_BASE_PTR, 0);	//channel 0 tx FIFO register
	unsigned int tfr1 = I2S_TFR_REG(I2S1_BASE_PTR, 1);	//channel 1 tx FIFO register
	
	unsigned char wfp;	//write FIFO pointer
	unsigned char rfp;	//read FIFO pointer
	
	unsigned char fifo0_cnt;	//current FIFO cnt for channel 0 
	unsigned char fifo1_cnt;	//current FIFO cnt for channel 1
	
	/* Calculate channel 0 FIFO cnt */
	wfp = (tfr0&I2S_TFR_WFP_MASK)>>I2S_TFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (tfr0&I2S_TFR_RFP_MASK)>>I2S_TFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo0_cnt = wfp - rfp;
	else
		fifo0_cnt = 16 - (rfp - wfp);
				
	/* Calculate channel 1 FIFO cnt */
	wfp = (tfr1&I2S_TFR_WFP_MASK)>>I2S_TFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (tfr1&I2S_TFR_RFP_MASK)>>I2S_TFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo1_cnt = wfp - rfp;
	else
		fifo1_cnt = 16 - (rfp - wfp);		
	
	if(tcsr & I2S_TCSR_WSF_MASK)	
	{	//indicate start of configured word has been detected
		I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_WSF_MASK;	//clear flag
		//printf("word start flag set\n");
		GPIOC_PTOR = 0x08;	//toggle PTC3 (J53 pin11)
	}
	else if(tcsr & I2S_TCSR_SEF_MASK)	
	{	//indicate error in the externally generated frame sync has been detected
		I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_SEF_MASK;
		//printf("sync error flag set\n");
		GPIOC_PTOR = 0x04;	//toggle PTC2 (J8 pin8)
	}
	else if(tcsr & I2S_TCSR_FEF_MASK)
	{	//indicate an enabled transmit FIFO has underrun
		I2S_TCSR_REG(I2S1_BASE_PTR) |= I2S_TCSR_FEF_MASK;	//clear flag
		//printf("fifo error flag set\n");
	}
	else if(tcsr & I2S_TCSR_FWF_MASK)
	{	//indicate an enabled transmit FIFO is empty
		I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_FWF_MASK;	//clear flag
		
		/* Since FIFO is empty, so we will fill all 8 entries */
		for(cnt = 0; cnt < 8; cnt++)
		{
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else
				I2S_TDR_REG(I2S1_BASE_PTR, 0) = tx_buffer[tx_index++];
			
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else		
				I2S_TDR_REG(I2S1_BASE_PTR, 1) = tx_buffer[tx_index++];
				
		}
		//printf("fifo warning flag set\n");
	}
	else if(tcsr & I2S_TCSR_FRF_MASK)
	{	//indicates number of words in an enabled transmit FIFO is less than or equal to the transmit FIFO watermark
		I2S_TCSR_REG(I2S1_BASE_PTR) &= ~I2S_TCSR_FRF_MASK;	//clear flag
		
		for(cnt = 0; cnt < 8 - fifo0_cnt; cnt++)
		{
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else
				I2S_TDR_REG(I2S1_BASE_PTR, 0) = tx_buffer[tx_index++];
			
		}
		
		for(cnt = 0; cnt < 8 - fifo1_cnt; cnt++)
		{
			if(tx_index > BUFFER_SIZE)
				tx_index = 0;
			else
				I2S_TDR_REG(I2S1_BASE_PTR, 1) = tx_buffer[tx_index++];
			
		}
		//printf("fifo request flag set\n");		
	}
}

void sai1_rx_isr(void)
{
	unsigned char cnt;
	unsigned int rcsr = I2S_RCSR_REG(I2S1_BASE_PTR);
	unsigned int rfr0 = I2S_RFR_REG(I2S1_BASE_PTR, 0);	//channel 0 rx FIFO register
	unsigned int rfr1 = I2S_RFR_REG(I2S1_BASE_PTR, 1);	//channel 1 rx FIFO register
	
	unsigned char wfp;	//write FIFO pointer
	unsigned char rfp;	//read FIFO pointer
	
	unsigned char fifo0_cnt;	//current FIFO cnt for channel 0 
	unsigned char fifo1_cnt;	//current FIFO cnt for channel 1
	
	/* Calculate channel 0 FIFO cnt */
	wfp = (rfr0&I2S_RFR_WFP_MASK)>>I2S_RFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (rfr0&I2S_RFR_RFP_MASK)>>I2S_RFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo0_cnt = wfp - rfp;
	else
		fifo0_cnt = 16 - (rfp - wfp);
				
	/* Calculate channel 1 FIFO cnt */
	wfp = (rfr1&I2S_RFR_WFP_MASK)>>I2S_RFR_WFP_SHIFT;	//write FIFO pointer
	rfp = (rfr1&I2S_RFR_RFP_MASK)>>I2S_RFR_RFP_SHIFT;	//read FIFO pointer
	
	if(wfp >= rfp)
		fifo1_cnt = wfp - rfp;
	else
		fifo1_cnt = 16 - (rfp - wfp);		
		
	if(rcsr & I2S_RCSR_WSF_MASK)	
	{	//indicate start of configured word has been detected
		I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_WSF_MASK;	//clear flag
		//printf("word start flag set\n");
	}
	else if(rcsr & I2S_RCSR_SEF_MASK)	
	{	//indicate error in the externally generated frame sync has been detected
		I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_SEF_MASK;
		//printf("sync error flag set\n");
	}
	else if(rcsr & I2S_RCSR_FEF_MASK)
	{	//indicate an enabled receive FIFO has overflowed
		I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FEF_MASK;	//clear flag
		//printf("fifo error flag set\n");
	}
	else if(rcsr & I2S_RCSR_FWF_MASK)
	{	//indicate an enabled receive FIFO is full
		I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FWF_MASK;	//clear flag
		/* Since receive FIFO is full, we read all 8 entries */
		for(cnt = 0; cnt < 8; cnt++)
		{	
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S1_BASE_PTR, 0);
			
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;	
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S1_BASE_PTR, 1);
			
		}

		//printf("fifo warning flag set\n");
	}
	else if(rcsr & I2S_RCSR_FRF_MASK)
	{	//indicates number of words in an enabled receive FIFO is greater than the receive FIFO watermark
		I2S_RCSR_REG(I2S1_BASE_PTR) |= I2S_RCSR_FRF_MASK;	//clear flag
		for(cnt = 0; cnt < fifo0_cnt; cnt++)
		{
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S1_BASE_PTR, 0);
			
		}
		
		for(cnt = 0; cnt < fifo1_cnt; cnt++)
		{
			if(rx_index > BUFFER_SIZE)
				rx_index = 0;
			else
				rx_buffer[rx_index++] = I2S_RDR_REG(I2S1_BASE_PTR, 1);
			
		}
		
		//printf("fifo request flag set\n");		
	}
}
