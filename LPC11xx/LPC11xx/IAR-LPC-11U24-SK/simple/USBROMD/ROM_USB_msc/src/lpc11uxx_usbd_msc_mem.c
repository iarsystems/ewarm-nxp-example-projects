#include <stdint.h>
#include <string.h>
#include "app_usbd_cfg.h"
#include "sbl_iap.h"
#include "mw_usbd_rom_api.h"

/* MSC Memory Layout */
#define MSC_BLOCK_SIZE        512
#define MSC_BLOCK_COUNT      (MSC_MemorySize / MSC_BLOCK_SIZE)

extern USBD_API_T* pUsbApi;
extern const unsigned char DiskImage[MSC_IMAGE_SIZE];

const uint8_t InquiryStr[] = {'N','X','P',' ',' ',' ',' ',' ','L','P','C',' ','M','e','m',' ',     \
                              'D','i','s','k',' ',' ',' ',' ','1','.','0',' ',};

uint8_t RAMDisk[MSC_IMAGE_SIZE] @ MSC_IMAGE_ADDR;

void MSC_Read( uint32_t offset, uint8_t** dst, uint32_t length)
{
  for (uint32_t i = 0; i<length; i++) *(*dst+i) = RAMDisk[offset+i];
}

void MSC_Write( uint32_t offset, uint8_t** src, uint32_t length)
{
  for (uint32_t i = 0; i<length; i++) RAMDisk[offset+i] = *(*src+i);
}

ErrorCode_t MSC_Verify( uint32_t offset, uint8_t src[], uint32_t length)
{
  if (memcmp((void*)&RAMDisk[offset], src, length))
    return ERR_FAILED;
  else
    return LPC_OK;
}

ErrorCode_t usb_msc_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
  USBD_MSC_INIT_PARAM_T msc_param;
  ErrorCode_t ret;

  /* copy disk image to ram */
  memcpy(RAMDisk, DiskImage, MSC_IMAGE_SIZE);

  /* clear msc_param structure */
  memset((void*)&msc_param, 0, sizeof(USBD_MSC_INIT_PARAM_T));

  /* init msc_param parameters */
  msc_param.mem_base = *mem_base;
  msc_param.mem_size = *mem_size;

  /* mass storage parameters */
  msc_param.InquiryStr = (uint8_t*)InquiryStr;
  msc_param.BlockCount = MSC_MemorySize / MSC_BLOCK_SIZE;
  msc_param.BlockSize  = MSC_BLOCK_SIZE;
  msc_param.MemorySize = MSC_MemorySize;

  if ((pIntfDesc == 0) ||
      (pIntfDesc->bInterfaceClass    != USB_DEVICE_CLASS_STORAGE) ||
      (pIntfDesc->bInterfaceSubClass != MSC_SUBCLASS_SCSI) )
    return ERR_FAILED;

  msc_param.intf_desc  = (uint8_t*)pIntfDesc;

  /* user defined functions */
  msc_param.MSC_Write  = MSC_Write;
  msc_param.MSC_Read   = MSC_Read;
  msc_param.MSC_Verify = MSC_Verify;

  /* init mass storage device */
  ret = pUsbApi->msc->init(hUsb, &msc_param);

  /* update memory variables */
  *mem_base = msc_param.mem_base;
  *mem_size = msc_param.mem_size;

  return ret;
}

