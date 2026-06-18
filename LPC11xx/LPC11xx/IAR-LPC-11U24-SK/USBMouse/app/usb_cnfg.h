/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : usb_cfg.h
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : May 12, 2011
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
#define ReportEpHid                     ENP1_IN
#define ReportEpMaxSize                 3
#define ReportEpPollingPeriod           2   /* resolution 1ms */
#define MaxIndOfRealizeEp               ENP1_IN

/* Class defenitions*/
#define HID_INTERFACE_0                 0
#define HID_BOOT_DEVICE                 1
#define HID_IDLE_SUPP                   1
#define HID_ID_NUMB                     0

#define HID_MOUSE_ID                    0

/* Other defenitions */

#endif /* __USB_CNFG_H */

