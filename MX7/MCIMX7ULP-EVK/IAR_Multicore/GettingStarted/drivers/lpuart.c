/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : lpuart.c
 *    Description : LPUART driver
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 7084 $
 **************************************************************************/
#include <intrinsics.h>
#include <yfuns.h>
#include "device_registers.h"
#include "lpuart.h"

#define LPUART_OSR                            15
#define LPUART_CALC_SBR_OSR(baudrate,clk,osr) (uint16_t)((double)clk/(((double)osr+1.0)*(double)baudrate))
#define LPUART_CALC_SBR(baudrate,clk)         LPUART_CALC_SBR_OSR(baudrate,clk,LPUART_OSR)

/*************************************************************************
 * Function Name: LPUART_Init
 * Parameters: LPUART_MemMapPtr LPUART_BASE, uint32_t Baud, uint32_t Clock, uint8_t Src
 *
 * Return: void
 *
 * Description: Configure the LPUART module.
 *************************************************************************/
void LPUART_Init(LPUART_MemMapPtr LPUART, uint32_t Baud, uint32_t Clock, uint8_t Src)
{
  /*Configure LPUART_TX and LPUART_RX pins*/
  if (LPUART0_BASE_PTR == LPUART)
  {
    IOMUXC0_SW_MUX_CTL_PAD_PTA18 = IOMUXC0_SW_MUX_CTL_PAD_MUX_MODE(4) | IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK;
    IOMUXC0_SW_MUX_CTL_PAD_PTA19 = IOMUXC0_SW_MUX_CTL_PAD_MUX_MODE(4) | IOMUXC0_SW_MUX_CTL_PAD_IBE_MASK;
    IOMUXC0_LPUART0_IPP_IND_LPUART_RXD_SELECT_INPUT = (2<<IOMUXC0_SELECT_INPUT_DAISY_SHIFT);
    IOMUXC0_LPUART0_IPP_IND_LPUART_TXD_SELECT_INPUT = (2<<IOMUXC0_SELECT_INPUT_DAISY_SHIFT);
    /*Set and enable LPUART Clocks*/
    PCC0_PCC_LPUART0 = (1<<PCC0_PCC_LPUART0_CGC_SHIFT) | (Src<<PCC0_PCC_LPUART0_PCS_SHIFT);
  }
  else if (LPUART4_BASE_PTR == LPUART)
  {
    IOMUXC1_SW_MUX_CTL_PAD_PTC2 = IOMUXC1_SW_MUX_CTL_PAD_MUX_MODE(4) | IOMUXC1_SW_MUX_CTL_PAD_OBE_MASK;
    IOMUXC1_SW_MUX_CTL_PAD_PTC3 = IOMUXC1_SW_MUX_CTL_PAD_MUX_MODE(4) | IOMUXC1_SW_MUX_CTL_PAD_IBE_MASK;
    IOMUXC1_LPUART4_IPP_IND_LPUART_RXD_SELECT_INPUT = (1<<IOMUXC1_SELECT_INPUT_DAISY_SHIFT);
    IOMUXC1_LPUART4_IPP_IND_LPUART_TXD_SELECT_INPUT = (1<<IOMUXC1_SELECT_INPUT_DAISY_SHIFT);
    /*Set and enable LPUART Clocks*/
    PCC2_PCC_LPUART4 = (1<<PCC2_PCC_LPUART4_CGC_SHIFT) | (Src<<PCC2_PCC_LPUART4_PCS_SHIFT);
  }

  /*Reset LPUART registers*/
  LPUART->GLOBAL  =  (1<<LPUART_GLOBAL_RST_SHIFT);
  LPUART->GLOBAL &= ~(1<<LPUART_GLOBAL_RST_SHIFT);

  /*LPUART Initialization*/
  LPUART->BAUD =   (0<<LPUART_BAUD_MAEN1_SHIFT)     /*Match Address Mode disabled 1*/\
                 | (0<<LPUART_BAUD_MAEN2_SHIFT)     /*Match Address Mode disabled 2*/\
                 | (0<<LPUART_BAUD_M10_SHIFT)       /*10-bit Mode disabled*/\
                 | LPUART_BAUD_OSR(LPUART_OSR)      /*Oversampling Ratio: 16x*/\
                 | (0<<LPUART_BAUD_TDMAE_SHIFT)     /*Transmitter DMA disabled*/\
                 | (0<<LPUART_BAUD_RDMAE_SHIFT)     /*Receiver Full DMA disabled*/\
                 | LPUART_BAUD_MATCFG(0x0)          /*Address Match Wakeup*/\
                 | (0<<LPUART_BAUD_BOTHEDGE_SHIFT)  /*Receiver samples input data using the rising edge of the baud rate clock.*/\
                 | (0<<LPUART_BAUD_RESYNCDIS_SHIFT) /*Resynchronization during received data word is supported*/\
                 | (0<<LPUART_BAUD_LBKDIE_SHIFT)    /*Hardware interrupts from LPUART_STAT[LBKDIF] disabled (use polling).*/\
                 | (0<<LPUART_BAUD_RXEDGIE_SHIFT)   /*Hardware interrupts from LPUART_STAT[RXEDGIF] disabled (use polling).*/\
                 | (0<<LPUART_BAUD_SBNS_SHIFT)      /*One stop bit*/\
                 | LPUART_CALC_SBR(Baud, Clock);    /*Baud Rate Modulo Divisor*/

  LPUART->STAT =   (0<<LPUART_STAT_MSBF_SHIFT)      /*LSB First*/\
                 | (0<<LPUART_STAT_RXINV_SHIFT)     /*Receive Data Inversion disabled*/\
                 | (0<<LPUART_STAT_RWUID_SHIFT)     /*Receive Wake Up Idle Detect disabled*/\
                 | (0<<LPUART_STAT_BRK13_SHIFT)     /*Break Character Generation Length 9-13bit times*/\
                 | (0<<LPUART_STAT_LBKDE_SHIFT);    /*LIN Break Detection Enable disabled*/\

  LPUART->CTRL =   (0<<LPUART_CTRL_R8T9_SHIFT)      /*Receive Bit 8 / Transmit Bit 9*/\
                 | (0<<LPUART_CTRL_R9T8_SHIFT)      /*Receive Bit 9 / Transmit Bit 8*/\
                 | (0<<LPUART_CTRL_TXDIR_SHIFT)     /*TXD pin is an input in single-wire mode*/\
                 | (0<<LPUART_CTRL_TXINV_SHIFT)     /*Transmit data not inverted*/\
                 | (0<<LPUART_CTRL_ORIE_SHIFT)      /*OR interrupts disabled; use polling*/\
                 | (0<<LPUART_CTRL_NEIE_SHIFT)      /*NF interrupts disabled; use polling*/\
                 | (0<<LPUART_CTRL_FEIE_SHIFT)      /*FE interrupts disabled; use polling*/\
                 | (0<<LPUART_CTRL_PEIE_SHIFT)      /*PF interrupts disabled; use polling*/\
                 | (0<<LPUART_CTRL_TIE_SHIFT)       /*Hardware interrupts from TDRE disabled; use polling*/\
                 | (0<<LPUART_CTRL_TCIE_SHIFT)      /*Hardware interrupts from TC disabled; use polling*/\
                 | (0<<LPUART_CTRL_RIE_SHIFT)       /*Hardware interrupts from RDRF disabled; use polling*/\
                 | (0<<LPUART_CTRL_ILIE_SHIFT)      /*Hardware interrupts from IDLE disabled; use polling*/\
                 | (1<<LPUART_CTRL_TE_SHIFT)        /*Transmitter enabled*/\
                 | (1<<LPUART_CTRL_RE_SHIFT)        /*Receiver enabled*/\
                 | (0<<LPUART_CTRL_RWU_SHIFT)       /*Receiver Wakeup Control disable*/\
                 | (0<<LPUART_CTRL_SBK_SHIFT)       /*Send Break disable*/\
                 | (0<<LPUART_CTRL_MA1IE_SHIFT)     /*MA1F interrupt disabled*/\
                 | (0<<LPUART_CTRL_MA2IE_SHIFT)     /*MA2F interrupt disabled*/\
                 | (0<<LPUART_CTRL_M7_SHIFT)        /*Receiver and transmitter use 8-bit to 10-bit data characters*/\
                 | LPUART_CTRL_IDLECFG(0x0)         /*1 idle character*/\
                 | (0<<LPUART_CTRL_LOOPS_SHIFT)     /*Normal operation - RXD and TXD use separate pins*/\
                 | (0<<LPUART_CTRL_DOZEEN_SHIFT)    /*LPUART is enabled in Doze mode*/\
                 | (0<<LPUART_CTRL_RSRC_SHIFT)      /*Provided LOOPS is set, sets internal loop back mode, RXD pin is not used*/\
                 | (0<<LPUART_CTRL_M_SHIFT)         /*Receiver and transmitter use 8-bit data characters*/\
                 | (0<<LPUART_CTRL_WAKE_SHIFT)      /*Configures RWU for idle-line wakeup*/\
                 | (0<<LPUART_CTRL_ILT_SHIFT )      /*Idle character bit count starts after start bit*/\
                 | (0<<LPUART_CTRL_PE_SHIFT )       /*No hardware parity generation or checking*/\
                 | (0<<LPUART_CTRL_PT_SHIFT );      /*Even parity if parity enabled*/\
}
