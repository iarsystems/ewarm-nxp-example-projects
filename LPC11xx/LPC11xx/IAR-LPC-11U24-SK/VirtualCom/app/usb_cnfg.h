/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : usb_cfg.h
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : January 7, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/

#include "includes.h"

#ifndef __USB_CNFG_H
#define __USB_CNFG_H

/* USB High Speed support*/
#define USB_HIGH_SPEED                  0

/* USB interrupt priority */
/* 1 - high priority (FIQ handler) */
/* 0 - normal priority */
#define USB_FRAME_PRIORITY              0
#define USB_DEV_PRIORITY                0
#define USB_EP_PRIORITY                 0x000

/* */
#define USB_FIQ_INTR_PRIORITY           0
#define USB_INTR_PRIORITY               0x40
#define USB_WAKEUP_INTR_PRIORITY        0x80

/* USB Events */
#define USB_SOF_EVENT                   0
#define USB_SOF_FRAME_NUMB              0   /* disable frame number */

/* USB Clock settings */
#define USB_CLK_DIV                     6

/* Device power atrb  */
#define USB_SELF_POWERED                0
#define USB_REMOTE_WAKEUP               1

/* Max Interfaces number*/
#define USB_MAX_INTERFACE               1

/* Endpoint definitions */
#define Ep0MaxSize                      8

#define ReportEp                        ENP1_IN
#define ReportEpMaxSize                 16

#define CommOutEp                       ENP2_OUT
#define CommOutEpMaxSize                64

#define CommInEp                        ENP2_IN
#define CommInEpMaxSize                 64

/* Class defenitions*/
#define CDC_DEVICE_SUPPORT_LINE_CODING  1
#define CDC_DEVICE_SUPPORT_LINE_STATE   0

#define CDC_DEVICE_SUPPORT_BREAK        0
#define CDC_BRK_TIMER_INTR_PRI          3

#define CDC_DATA_RATE                   CBR_9600
#define CDC_DATA_BITS                   8
#define CDC_PARITY                      NOPARITY
#define CDC_STOP_BITS                   ONESTOPBIT

#define CDC_LINE_DTR                    0
#define CDC_LINE_RTS                    0

/* Other defenitions */

#endif //__USB_CNFG_H
