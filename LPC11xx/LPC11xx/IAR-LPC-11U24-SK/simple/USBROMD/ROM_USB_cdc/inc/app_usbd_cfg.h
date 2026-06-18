#ifndef __APP_USBD_CFG_H__
#define __APP_USBD_CFG_H__

#define USB_MAX_IF_NUM  8
#define USB_MAX_EP_NUM  5
#define USB_MAX_PACKET0 64

/* Max In/Out Packet Size */
#define USB_FS_MAX_BULK_PACKET  64
#define USB_HS_MAX_BULK_PACKET  USB_FS_MAX_BULK_PACKET

#define USB_CDC_CIF_NUM   0
#define USB_CDC_DIF_NUM   1

#define USB_CDC_EP_INT_IN    USB_ENDPOINT_IN(1)
#define USB_CDC_EP_BULK_IN   USB_ENDPOINT_IN(2)
#define USB_CDC_EP_BULK_OUT  USB_ENDPOINT_OUT(2)

#endif  /* __APP_USBD_CFG_H__ */
