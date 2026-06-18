/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : usb_cnfg.h
 *    Description : USB Configuration Parameters
 *
 *    History :
 *    1. Date        : July 12, 2012
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/

#ifndef __USB_CNFG_H
#define __USB_CNFG_H

/* Device power attributes  */
#define USB_SELF_POWERED                0
#define USB_REMOTE_WAKEUP               1

/* Endpoint definitions */
#define USB_MAX_PACKET0                 64
#define HID_EP_IN                       0x81
#define HID_EP_OUT                      0x01
#define USB_POOLING_PERIOD              2   /* resolution 1ms */

/* Class defenitions */
#define HID_INTERFACE_0                 0
#define HID_BOOT_DEVICE                 1

#define USB_MAX_IF_NUM                  8
#define USB_MAX_EP_NUM                  5

/* Max In/Out Packet Size */
#define USB_FS_MAX_BULK_PACKET          64

/* Full speed device only */
#define USB_HS_MAX_BULK_PACKET          USB_FS_MAX_BULK_PACKET

/* USB Speed defenitions */
#define FULL_SPEED_ONLY

#define LPC_USB_BASE                   (0x40080000)

#define LPC_USB_RAM_BEGIN               0x20004000
#define LPC_USB_RAM_SIZE                0x800

#endif /* __USB_CNFG_H */
