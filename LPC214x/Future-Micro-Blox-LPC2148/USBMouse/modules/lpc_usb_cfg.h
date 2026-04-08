/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : lpc_usb_cfg.h
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : August 4, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    1. Date        : November 18, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *				Add DMA support
 *
 *    $Revision: 28 $
**************************************************************************/

#include "includes.h"

#ifndef __LPC_USB_CFG_H
#define __LPC_USB_CFG_H

/* USB Events */
#define USB_SOF_EVENT             0
#define USB_ERROR_EVENT           1
#define USB_HIGH_PRIORITY_EVENT   0

/* DMA transfer */
#define USB_DMA   								0
#define DMA_DD_MAX_NUMB						0
#define DMA_INT_ENABLE_MASK       0

/* USB Pll settings */
/* Fusb = Fosc*M */
#define USB_PLL_M         				4
#define USB_PLL_P         				2

/* Endpoint definitions */
#define Ep0MaxSize        				64
#define ReportEp          				UsbEp1In
#define ReportEpMaxSize   				3

/* Othe defenitions */

#endif //__LPC_USB_CFG_H
