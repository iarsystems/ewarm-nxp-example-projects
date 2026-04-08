/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : lpc23xx_usb_cfg.h
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
#define USB_HIGH_SPEED                  1

/* USB interrupt priority */
#define USB_INTR_PRIORITY               15

/* USB Events */
#define USB_SOF_EVENT                   0
#define USB_ERROR_EVENT                 1   // for debug
#define USB_SOF_FRAME_NUMB              0   // disable frame number

//DMA Settings
#define USB_DMA_DD_MAX_NUMB             0   // number of DMA descriptors
#define USB_DMA_ID_MAX_NUMB             0   // number of Isochronous DMA descriptors
#define DMA_INT_ENABLE_MASK             0   // DMA interrupt enable (End of Transfer,
                                            // New DD request, System error interrupt)
/* Device power atrb  */
#define USB_SELF_POWERED                0
#define USB_REMOTE_WAKEUP               1

/* Max Interfaces number*/
#define USB_MAX_INTERFACE               1

/* Endpoint definitions */
#define Ep0MaxSize                      64

#define BulkInEp                        ENP1_IN
#if USB_HIGH_SPEED > 0
#define BulkInEpMaxSize                 512
#define BulkInEpMaxSize1                64
#else
#define BulkInEpMaxSize                 64
#endif

#define BulkOutEp                       ENP1_OUT
#if USB_HIGH_SPEED > 0
#define BulkOutEpMaxSize                512
#define BulkOutEpMaxSize1               64
#else
#define BulkOutEpMaxSize                64
#endif

/* Class definitions */
#define MSD_INTERFACE_ID                0

/* Other defenitions */

#endif //__USB_CNFG_H
