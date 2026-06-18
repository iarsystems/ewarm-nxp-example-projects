/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : glcd_ll.c
 *    Description : GLCD low level functions
 *
 *    History :
 *    1. Date        : December 2, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "glcd_ll.h"
#include "lpc17xx.h"

#define SSP_FIFO_SIZE   8

/*************************************************************************
 * Function Name: GLCD_SetReset
 * Parameters: int32_t State
 * Return: none
 *
 * Description: Set reset pin state
 *
 *************************************************************************/
void GLCD_SetReset (int32_t State)
{
	if(State)
	{
    /*Set LCD Reset pin*/
    LPC_GPIO3->FIOSET = (1UL<<25);
	}
	else
	{
    /*clear LCD Reset pin*/
    LPC_GPIO3->FIOCLR = (1UL<<25);
	}
}

/*************************************************************************
 * Function Name: GLCD_Backlight
 * Parameters: uint8_t Light
 * Return: none
 *
 * Description: Set backlight pin state
 *
 *************************************************************************/
void GLCD_Backlight (uint8_t Light)
{
  /*load Match Register*/
  LPC_PWM1->MR3 = BACKLIGHT_OFF + Light;
  LPC_PWM1->LER |= (1UL<<3);
}

/*************************************************************************
 * Function Name: GLCD_LLInit
 * Parameters: none
 * Return: none
 *
 * Description: Init Reset and Backlight control outputs
 *
 *************************************************************************/
void GLCD_LLInit (void)
{
	/* LCD Reset output */
  LPC_GPIO3->FIODIR |= (1UL<<25);

  GLCD_SetReset(0);

	/* LCD backlight PWM 8bit init */
  LPC_PINCON->PINSEL7 |= (3UL<<20);   /* assign P3.26 to PWM1 */
  LPC_SC->PCONP |= (1UL<<6);          /* enable PWM1 clock*/
  
  LPC_PWM1->TCR = (1UL<<1);           /* reset PWM1 */

  LPC_PWM1->CTCR = 0;                 /* Timer mode */
  LPC_PWM1->MCR = (1UL<<1);           /* Reset on PWMMR0 */
  LPC_PWM1->PCR = (0UL<<3)|           /* single edge controlled mode for PWM3 */
                  (1UL<<11);          /* enabled PWM3 output */
  LPC_PWM1->PR = 0;                   /* */
  LPC_PWM1->MR0 = 0xFF;               /* 8bit resolution */
  LPC_PWM1->MR3 = 0;
  LPC_PWM1->LER = (1UL<<0)|           /* Enable PWM Match 0 Latch*/
                  (1UL<<3);           /* Enable PWM Match 3 Latch*/

  LPC_PWM1->TCR = (1UL<<0)|           /* enable counting */
                  (0UL<<1)|           /* release reset */
                  (1UL<<3);           /* enable PWM function */
  
  GLCD_Backlight(0);
}

/*************************************************************************
 * Function Name: GLCD_SPI_ChipSelect
 * Parameters: int32_t Select
 * Return: none
 *
 * Description: SSP0 Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void GLCD_SPI_ChipSelect (int32_t Select)
{
  if (Select)
  {
    /*Clear SPI CS pin*/
    LPC_GPIO1->FIOCLR = (1UL<<21);
  }
  else
  {
    /*Set SPI CS pin*/
    LPC_GPIO1->FIOSET = (1UL<<21);
  }
}

/*************************************************************************
 * Function Name: GLCD_SPI_SetWordWidth
 * Parameters: uint32_t Width
 * Return: int32_t
 *
 * Description: Set SSP 0 word width
 *
 *************************************************************************/
int32_t GLCD_SPI_SetWordWidth (uint32_t Width)
{
uint32_t tmp;
  if(4 > Width || Width > 16)
  {
    return(FALSE);
  }

  tmp = LPC_SSP0->CR0 & 0xFFFFFFF0;
  LPC_SSP0->CR0 = tmp | (Width - 1);
  
  return(TRUE);
}

/*************************************************************************
 * Function Name: GLCD_SPI_SetClockFreq
 * Parameters: uint32_t Frequency
 * Return: uint32_t
 *
 * Description: Set SSP 0 clock
 *
 *************************************************************************/
uint32_t GLCD_SPI_SetClockFreq (uint32_t Frequency)
{
uint32_t Fspi;
uint32_t Div = 2;
  switch(((LPC_SC->PCLKSEL1)>>10) & 0x3)
  {
  case 0:
      Fspi = SystemCoreClock/4;
    break;
  case 1:
      Fspi = SystemCoreClock;
    break;
  case 2:
      Fspi = SystemCoreClock/2;
    break;
  case 3:
      Fspi = SystemCoreClock/8;
    break;
  }

  while((Div * Frequency) < Fspi)
  {
    if((Div += 2) == 254)
    {
      break;
    }
  }
  LPC_SSP0->CPSR = Div;
  return(Fspi/Div);
}

/*************************************************************************
 * Function Name: GLCD_SPI_Init
 * Parameters: uint32_t Clk, uint32_t Width
 * Return: none
 *
 * Description: Init SSP0
 *
 *************************************************************************/
void GLCD_SPI_Init(uint32_t Clk, uint32_t Width)
{
  /* Assign GPIO to SSP0 - SCK, MOSI, MISO */
  LPC_PINCON->PINSEL3 |= (3UL<<8)|      /*  SCK */
                         (3UL<<14)|     /*  MISO */
                         (3UL<<16);     /*  MOS1 */
  /* Chip select */
  LPC_GPIO1->FIODIR |= (1UL<<21);

  GLCD_SPI_ChipSelect(FALSE);

  /*  Spi init */
  LPC_SC->PCONP |= (1UL<<21);           /* enable SSP0 clock */

  LPC_SSP0->CR1 = 0;                    /* Disable module, Disable Loop Back Mode, 
                                           select Master mode */

  LPC_SSP0->CR0 = 0;	                  /*Frame Format SPI, bus clock low between
                                          frames, captures serial data on the
                                          first clock transition of the frame*/

  LPC_SSP0->IMSC = 0;                   /* disable all interrupts */
  LPC_SSP0->DMACR = 0;                  /* disable DMA */
  LPC_SSP0->CR1 |= (1UL<<1);            /*  Enable module */
  
  for (uint32_t i = 0; i < 8; i++ )
  {
    volatile uint32_t Dummy = LPC_SSP0->DR;   /* clear the RxFIFO */
  }
  /* Set SSP clock frequency */
  GLCD_SPI_SetClockFreq(Clk);
  /* Set data width */
  GLCD_SPI_SetWordWidth(Width);

}

/*************************************************************************
 * Function Name: GLCD_SPI_TranserByte
 * Parameters: uint32_t Data
 * Return: uint32_t
 *
 * Description: Transfer byte from SSP0
 *
 *************************************************************************/
uint32_t GLCD_SPI_TranserByte (uint32_t Data)
{
  while(! (LPC_SSP0->SR & (1UL<<1)));
  LPC_SSP0->DR = Data;
  while(LPC_SSP0->SR & (1UL<<4));
  return(LPC_SSP0->DR);
}

/*************************************************************************
 * Function Name: GLCD_SPI_SendBlock
 * Parameters: uint8_t * pData, uint32_t Size
 *
 * Return: void
 *
 * Description: Write block of data to SSP
 *
 *************************************************************************/
void GLCD_SPI_SendBlock (uint8_t * pData, uint32_t Size)
{
uint32_t OutCount = Size;
  while (OutCount)
  {
    while((LPC_SSP0->SR & (1UL<<1)) && OutCount)
    {
       LPC_SSP0->DR = *pData++ | 0x100;  // Data
      --OutCount;
    }
  }
  while(LPC_SSP0->SR & (1UL<<4));
  // draining RX Fifo
  while (LPC_SSP0->SR & (1UL<<2))
  {
    volatile uint32_t Dummy = LPC_SSP0->DR;
  }
}

/*************************************************************************
 * Function Name: GLCD_SPI_ReceiveBlock
 * Parameters: uint8_t * pData, uint32_t Size
 *
 * Return: void
 *
 * Description: Read block of data from SSP
 *
 *************************************************************************/
void GLCD_SPI_ReceiveBlock (uint8_t * pData, uint32_t Size)
{
uint32_t Delta = 0;
  while (Size || Delta)
  {
    while((LPC_SSP0->SR & (1UL<<1)) && (Delta < SSP_FIFO_SIZE) && Size)
    {
      LPC_SSP0->DR = 0xFFFF;
      --Size; ++Delta;
    }
    while (LPC_SSP0->SR & (1UL<<2))
    {
      *pData++ = LPC_SSP0->DR;
       --Delta;
    }
  }
}
