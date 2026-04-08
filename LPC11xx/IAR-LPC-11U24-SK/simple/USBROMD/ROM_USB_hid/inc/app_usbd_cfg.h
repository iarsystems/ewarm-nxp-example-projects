#ifndef __APP_USBD_CFG_H__
#define __APP_USBD_CFG_H__

/* HID In/Out Endpoint Address */
#define HID_EP_IN       USB_ENDPOINT_IN(1)
#define HID_EP_OUT      USB_ENDPOINT_OUT(1)

#define USB_MAX_IF_NUM  8
#define USB_MAX_EP_NUM  5
#define USB_MAX_PACKET0 64

/* Max In/Out Packet Size */
#define USB_FS_MAX_BULK_PACKET  64
#define USB_HS_MAX_BULK_PACKET  USB_FS_MAX_BULK_PACKET

#endif  /* __APP_USBD_CFG_H__ */
