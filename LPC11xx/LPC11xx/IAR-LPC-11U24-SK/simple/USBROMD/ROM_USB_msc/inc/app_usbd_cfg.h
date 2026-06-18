#ifndef __APP_USBD_CFG_H__
#define __APP_USBD_CFG_H__

#define USB_COMPLIANCE_ENABLED 0

/* Mass Storage Class RAM Location and Size */
#define MSC_PARAM_MEM           0x20004000
#define MSC_RARAM_SIZE          0x00000800

/* Disk Image Location and Size*/
#define MSC_IMAGE_ADDR          0x10001000
#define MSC_IMAGE_SIZE          0x00001000

/* MSC Disk Image Definitions */

/* For WIN7 environment, the minimum FAT12 file system is 8K. Because of the small
   RAM size, you will see usb disk, bigger than the image size defined here */

#if USB_COMPLIANCE_ENABLED 
#define MSC_MemorySize          MSC_IMAGE_SIZE
#else
#define MSC_MemorySize         (1024 * 8)            /* = 0x2000 (8kB)*/
#endif

/* USB Specific Parameters */
#define MSC_EP_IN               USB_ENDPOINT_IN(1)
#define MSC_EP_OUT              USB_ENDPOINT_OUT(1)

#define USB_MAX_IF_NUM          1
#define USB_MAX_EP_NUM          2
#define USB_MAX_PACKET0         64

/* Max In/Out Packet Size */
#define USB_FS_MAX_BULK_PACKET  64
#define USB_HS_MAX_BULK_PACKET  USB_FS_MAX_BULK_PACKET

#endif  /* __APP_USBD_CFG_H__ */
