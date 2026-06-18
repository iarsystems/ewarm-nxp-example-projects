/***********************************************************************
 * $Id: usbd_cdc.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC18xx Validation
 *
 * Description: USB mass storage functions for composite device
 *	example project.
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
#include <stdint.h>
#include <string.h>
#include "usbd/usbd_rom_api.h"
#include "usbd/usbd_core.h"
#include "usbd/usbd_cdc.h"
#include "usbd/usbd_cdcuser.h"
#include "usbd/usbd_rom_api.h"
#include "LPC18xx.h"            
#include "config.h"


//#define UART_BRIDGE 1
//#define LOOP_BRIDGE 1

/* extern functions */
extern void vcom_copy_descriptors(USB_CORE_DESCS_T* pDesc, uint32_t mem_base, USBD_CDC_INIT_PARAM_T* pCdc);

/* UART defines */
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

/* VCOM defines */
#define VCOM_BUFFERS    4
#define VCOM_BUF_EMPTY_INDEX  (0xFF)
#define VCOM_BUF_FREE   0
#define VCOM_BUF_ALLOC  1
#define VCOM_BUF_USBTXQ  2
#define VCOM_BUF_UARTTXQ  3
#define VCOM_BUF_ALLOCU  4

struct VCOM_DATA;
typedef void (*VCOM_SEND_T) (struct VCOM_DATA* pVcom, int8_t idx);

typedef struct VCOM_DATA {
  USBD_HANDLE_T hUsb;
  USBD_HANDLE_T hCdc;
  uint8_t* buff[4];
  volatile uint8_t used[4];
  volatile uint8_t len[4];
  volatile uint8_t usb_rx;
  volatile uint8_t usb_tx;
  volatile uint8_t ser_rx;
  volatile uint8_t ser_tx;
  volatile uint8_t ser_tx_pos[4];
  VCOM_SEND_T send_fn;
  uint32_t total_usbrx;
  uint32_t total_usbtx;
  uint32_t total_serrx;
  uint32_t total_sertx;
  volatile uint32_t sof_counter;
  volatile uint32_t last_ser_rx;
  volatile uint16_t break_time;

} VCOM_DATA_T; 

VCOM_DATA_T g_vCOM;
/**********************************************************************
 ** Function prototyping 
**********************************************************************/

int8_t get_buf(VCOM_DATA_T* pVcom, uint8_t alloc)
{
  int i = VCOM_BUFFERS;

  while(i--) {
     if( pVcom->used[i] == VCOM_BUF_FREE) {
      pVcom->used[i] = alloc; //VCOM_BUF_ALLOC;
      pVcom->ser_tx_pos[i] = 0;
      pVcom->len[i] = 0;
      return i;
     }
  }
  return VCOM_BUF_EMPTY_INDEX;
}


void VCOM_usb_send(VCOM_DATA_T* pVcom, int8_t idx)
{
  pVcom->used[idx] = VCOM_BUF_USBTXQ;
  if ( pVcom->usb_tx == VCOM_BUF_EMPTY_INDEX) {
    pVcom->usb_tx = idx;
    /* data received send it back */
    USBD_API->hw->WriteEP (pVcom->hUsb, CDC_BULK_EP_IN, 
      pVcom->buff[pVcom->usb_tx], pVcom->len[pVcom->usb_tx]);   
    pVcom->total_usbtx += pVcom->len[pVcom->usb_tx];
  }
}

#if defined(UART_BRIDGE)
void init_uart1_bridge(VCOM_DATA_T* pVcom, CDC_LINE_CODING* line_coding)
{
  uint32_t Fdiv, baud = 9600;
  uint8_t  lcr = 0x3;    	/* 8 bits, no Parity, 1 Stop bit */

  if ( line_coding) {
    if ( line_coding->bCharFormat)
      lcr |= (1 << 2);                 /* Number of stop bits */
  
    if ( line_coding->bParityType) { /* Parity bit type */
      lcr |= (1 << 3);
      lcr |=  (((line_coding->bParityType - 1) & 0x3) << 4);
    }
    if ( line_coding->bDataBits) {      /* Number of data bits */
      lcr |= ((line_coding->bDataBits - 5) & 0x3);
    }
    else {
      lcr |= 0x3;
    }
    baud = line_coding->dwDTERate;
    /* enable SOF after we are connected */
    USBD_API->hw->EnableEvent(pVcom->hUsb, 0, USB_EVT_SOF, 1);
  }
  else {
  	SetClock(BASE_UART1_CLK, SRC_PL160M_0, DIV1);
  	LPC_SCU->SFSPC_13 = (0x3<<2) | 0x2;	/*	PC_13 U1_TXD */
  	LPC_SCU->SFSPC_14 = (0x3<<2) | 0x2;	/*	PC_14 U1_RXD */
//    scu_pinmux(0xC,13,MD_PDN,FUNC2);    /* PC.13, U1_TXD */
//    scu_pinmux(0xC,14,MD_PDN,FUNC2);    /* PC.14, U1_RXD */
  } 

  Fdiv = ( UART1Frequency / 16 ) / baud ;	/*baud rate */
  LPC_UART1->IER = 0;
  LPC_UART1->LCR = lcr | 0x80; /* DLAB = 1 */		
  LPC_UART1->DLM = Fdiv / 256;							
  LPC_UART1->DLL = Fdiv % 256;
  LPC_UART1->LCR = lcr;		/* DLAB = 0 */
  LPC_UART1->FCR = 0x07;/* Enable and reset TX and RX FIFO. 
                                      Rx trigger level 4 chars*/
  pVcom->ser_tx = VCOM_BUF_EMPTY_INDEX;
  pVcom->ser_rx = get_buf(pVcom, VCOM_BUF_ALLOCU);
  LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
}
static void uart_write(VCOM_DATA_T* pVcom)
{
  int8_t idx = pVcom->ser_tx;
  uint8_t *pbuf = pVcom->buff[idx];
  uint8_t pos = pVcom->ser_tx_pos[idx];
  uint32_t tx_cnt =  (LPC_UART1->FIFOLVL >> 8);
  /* find space in TX fifo */
  tx_cnt = 0xF - (tx_cnt & 0xF);

  if (tx_cnt > (pVcom->len[idx] - pos)) {
    tx_cnt = (pVcom->len[idx] - pos);
  }
    
  while(tx_cnt--) { 
    LPC_UART1->THR = pbuf[pos++];
    pVcom->total_sertx++;
  }
  pVcom->ser_tx_pos[idx] = pos;

  /* if done check anything pending */
  if (pos == pVcom->len[idx]) {
    /* Tx complete free the buffer */
    pVcom->used[idx] = VCOM_BUF_FREE;
    pVcom->ser_tx_pos[idx] = 0;
    pVcom->ser_tx = VCOM_BUF_EMPTY_INDEX;
    /* is there anything else to be transmitted */
    idx = VCOM_BUFFERS;
    while(idx--) {
      if( pVcom->used[idx] == VCOM_BUF_UARTTXQ) {
        pVcom->ser_tx = idx;
        uart_write(pVcom);
        break;
      }
    }
  }

  return ;
}

static void uart_read(VCOM_DATA_T* pVcom)
{
  int8_t idx = pVcom->ser_rx;
  uint8_t *pbuf;
  uint8_t pos;

  if (idx == VCOM_BUF_EMPTY_INDEX) {
    idx = pVcom->ser_rx = get_buf(pVcom, VCOM_BUF_ALLOCU);
    if (idx == VCOM_BUF_EMPTY_INDEX)
      return;
  }

  pbuf = pVcom->buff[idx];
  pos = pVcom->len[idx];
  
  if(LPC_UART1->LSR & LSR_RDR) { 
    pbuf[pos++] = LPC_UART1->RBR;
    pVcom->total_serrx++;
  } 

  pVcom->len[idx] = pos;
  if (pos == USB_HS_MAX_BULK_PACKET) {
    VCOM_usb_send(pVcom, idx);
    pVcom->ser_rx = get_buf(pVcom, VCOM_BUF_ALLOCU);
  }
  pVcom->last_ser_rx = pVcom->sof_counter;
}

void VCOM_uart_send(VCOM_DATA_T* pVcom, int8_t idx)
{

  pVcom->used[idx] = VCOM_BUF_UARTTXQ;
  if ( pVcom->ser_tx == VCOM_BUF_EMPTY_INDEX) {
    pVcom->ser_tx = idx;
    /* data received on USB send it to UART */
    uart_write(pVcom);  
  }
}

/*****************************************************************************
** Function name:		UART1_IRQHandler
**
** Descriptions:		UART1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART1_IRQHandler (void) 
{
  VCOM_DATA_T* pVcom = &g_vCOM;
  uint8_t intId, lsr;
  uint16_t serial_state = 0; 
	
  intId = (LPC_UART1->IIR >> 1) & 0x07; /* check bit 1~3, interrupt identification */
    
  switch (intId)
  {
    case  IIR_RLS:    /* Receive Line Status */
   	  /* Read LSR will clear the interrupt */
      lsr = LPC_UART1->LSR;
     	  /* There are errors or break interrupt update serial_state */
      if (lsr & LSR_OE)
        serial_state |= CDC_SERIAL_STATE_OVERRUN;

      if (lsr & LSR_PE)
        serial_state |= CDC_SERIAL_STATE_PARITY;

      if (lsr & LSR_FE)
        serial_state |= CDC_SERIAL_STATE_FRAMING;
        
      if (lsr & LSR_BI)
        serial_state |= CDC_SERIAL_STATE_BREAK;

      USBD_API->cdc->SendNotification (pVcom->hCdc, CDC_NOTIFICATION_SERIAL_STATE, 
        serial_state);  
      break;
    case IIR_CTI: 	/* Character timeout indicator */
      /* send packet to USB */
      /* read chars until FIFO empty */
      uart_read(pVcom);
      break;
    case IIR_RDA:  /* Receive Data Available */
      /* 4 chars are available in FIFO */
      uart_read(pVcom);
      break;
    case  IIR_THRE: 	/* THRE, transmit holding register empty */
      if (pVcom->ser_tx != VCOM_BUF_EMPTY_INDEX) {
        /* transmit */
        uart_write(pVcom); 
      }
      break;
  }
      

  return;
}

ErrorCode_t VCOM_SetLineCode (USBD_HANDLE_T hCDC, CDC_LINE_CODING* line_coding)
{
  VCOM_DATA_T* pVcom = &g_vCOM;
  int i;
  /* baud rate change reset buffers */
  for ( i= 0; i < VCOM_BUFFERS; i++) {
    pVcom->used[i] = VCOM_BUF_FREE;  
    pVcom->ser_tx_pos[i] = 0;
    pVcom->usb_tx = VCOM_BUF_EMPTY_INDEX;
    pVcom->ser_rx = pVcom->ser_tx = VCOM_BUF_EMPTY_INDEX;
  }
  init_uart1_bridge(pVcom, line_coding);

  return LPC_OK;
}

ErrorCode_t VCOM_sof_event(USBD_HANDLE_T hUsb)
{
  VCOM_DATA_T* pVcom = &g_vCOM;
  uint8_t lcr;
  uint32_t diff = pVcom->sof_counter - pVcom->last_ser_rx;

  pVcom->sof_counter++;

  if (pVcom->break_time) {
    pVcom->break_time--;
    if (pVcom->break_time == 0) {
      lcr = LPC_UART1->LCR;
      if (lcr & (1 << 6)) {
        lcr &= ~(1 << 6);
        LPC_UART1->LCR = lcr;
      }
    }
  }

  if ( pVcom->last_ser_rx && (diff > 5)) {
    VCOM_usb_send(pVcom, pVcom->ser_rx);
    pVcom->ser_rx = get_buf(pVcom, VCOM_BUF_ALLOCU);
    pVcom->last_ser_rx = 0;
  }
    
  return LPC_OK;
}

#endif

ErrorCode_t VCOM_SendBreak (USBD_HANDLE_T hCDC, uint16_t mstime)
{
  VCOM_DATA_T* pVcom = &g_vCOM;
  uint8_t lcr = LPC_UART1->LCR;
  
  if ( mstime) {
    lcr |= (1 << 6);
  } else {
    lcr &= ~(1 << 6);
  }

  pVcom->break_time = mstime;
  LPC_UART1->LCR = lcr;

  return LPC_OK;
}



ErrorCode_t VCOM_bulk_in_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event) 
{
  VCOM_DATA_T* pVcom = (VCOM_DATA_T*) data;
  int i = VCOM_BUFFERS;

  if (event == USB_EVT_IN) {
    /* Tx complete free the buffer */
    pVcom->used[pVcom->usb_tx] = VCOM_BUF_FREE;
    pVcom->ser_tx_pos[pVcom->usb_tx] = 0;
    pVcom->usb_tx = VCOM_BUF_EMPTY_INDEX;

    /* is there anything else to be transmitted */
    while(i--) {
      if( pVcom->used[i] == VCOM_BUF_USBTXQ) {
        VCOM_usb_send(pVcom, i);
        break;
      }
    }
  }
  return LPC_OK;
}
ErrorCode_t VCOM_bulk_out_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event) 
{
  VCOM_DATA_T* pVcom = (VCOM_DATA_T*) data;

  switch (event) {
    case USB_EVT_OUT:
      pVcom->len[pVcom->usb_rx] = USBD_API->hw->ReadEP(hUsb, CDC_BULK_EP_OUT, pVcom->buff[pVcom->usb_rx]);
      pVcom->total_usbrx += pVcom->len[pVcom->usb_rx];
      pVcom->send_fn(pVcom, pVcom->usb_rx);
      pVcom->usb_rx = VCOM_BUF_EMPTY_INDEX;
      /* fall through */
    case USB_EVT_OUT_NAK:
      /* queue free buffer for RX */
      if (pVcom->usb_rx == VCOM_BUF_EMPTY_INDEX)
        pVcom->usb_rx = get_buf(pVcom, VCOM_BUF_ALLOC);

      if (pVcom->usb_rx != VCOM_BUF_EMPTY_INDEX) {
        USBD_API->hw->ReadReqEP(hUsb, CDC_BULK_EP_OUT, pVcom->buff[pVcom->usb_rx], USB_HS_MAX_BULK_PACKET);
      }
      break;
    default: 
      break;
  }
  return LPC_OK;
}

/* Main Program */

ErrorCode_t usb_cdc_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
  USBD_CDC_INIT_PARAM_T cdc_param;
  USB_CORE_DESCS_T desc;
  USBD_HANDLE_T hCdc;
  ErrorCode_t ret = LPC_OK;
  uint32_t ep_indx;
	static USB_INTERFACE_DESCRIPTOR *pCifIntfDesc = NULL;
	static USB_INTERFACE_DESCRIPTOR *pDifIntfDesc = NULL;

  // This function will be called twice, one call for each interface type.
	// Save the interface descriptor pointers with each call and return
	// until both are saved.
	if (pIntfDesc->bInterfaceClass == USB_DEVICE_CLASS_COMMUNICATIONS)
		pCifIntfDesc = pIntfDesc;

	if (pIntfDesc->bInterfaceClass == USB_DEVICE_CLASS_CDC_DATA_INTERFACE)
		pDifIntfDesc = pIntfDesc;

	if (!pCifIntfDesc || !pDifIntfDesc)
		return LPC_OK;


  /* init CDC params */
  memset((void*)&cdc_param, 0, sizeof(USBD_CDC_INIT_PARAM_T));
  memset((void*)&g_vCOM, 0, sizeof(VCOM_DATA_T));

	/* user defined functions */
#if defined(UART_BRIDGE)
	cdc_param.SetLineCode = VCOM_SetLineCode; 
#endif
	cdc_param.SendBreak = VCOM_SendBreak;

	// init CDC params
	cdc_param.mem_base = *mem_base;
	cdc_param.mem_size = *mem_size;
	cdc_param.cif_intf_desc = (uint8_t*)pCifIntfDesc;
	cdc_param.dif_intf_desc = (uint8_t*)pDifIntfDesc;
	
	ret = USBD_API->cdc->init(hUsb, &cdc_param, &hCdc);
  /* update memory variables */
  *mem_base = cdc_param.mem_base;
  *mem_size = cdc_param.mem_size;

	if (ret == LPC_OK) {
		/* store USB handle */
		g_vCOM.hUsb = hUsb;
		g_vCOM.hCdc = hCdc;
		g_vCOM.usb_rx = g_vCOM.usb_tx = VCOM_BUF_EMPTY_INDEX;
		g_vCOM.send_fn = VCOM_usb_send;

		/* allocate transfer buffers */
		g_vCOM.buff[0] = (uint8_t*)(cdc_param.mem_base + (0 * USB_HS_MAX_BULK_PACKET));
		g_vCOM.buff[1] = (uint8_t*)(cdc_param.mem_base + (1 * USB_HS_MAX_BULK_PACKET));
		g_vCOM.buff[2] = (uint8_t*)(cdc_param.mem_base + (2 * USB_HS_MAX_BULK_PACKET));
		g_vCOM.buff[3] = (uint8_t*)(cdc_param.mem_base + (3 * USB_HS_MAX_BULK_PACKET));
		cdc_param.mem_size -= (4 * USB_HS_MAX_BULK_PACKET);

		/* register endpoint interrupt handler */
		ep_indx = (((CDC_BULK_EP_IN & 0x0F) << 1) + 1);
		ret = USBD_API->core->RegisterEpHandler (hUsb, ep_indx, VCOM_bulk_in_hdlr, &g_vCOM);
		if (ret == LPC_OK) {
			/* register endpoint interrupt handler */
			ep_indx = ((CDC_BULK_EP_OUT & 0x0F) << 1);
			ret = USBD_API->core->RegisterEpHandler (hUsb, ep_indx, VCOM_bulk_out_hdlr, &g_vCOM);
			if (ret == LPC_OK) {
				
#if defined(UART_BRIDGE)
				g_vCOM.send_fn = VCOM_uart_send;
				/* init UART for bridge */
				init_uart1_bridge(&g_vCOM, 0);
				/* enable IRQ */
				NVIC_EnableIRQ(UART1_IRQn); //  enable USB0 interrrupts 
#endif
			}
		}
  }


return ret;
}

