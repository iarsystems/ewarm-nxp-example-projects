/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : audio_class.c
 *    Description : AUDIO CLASS module
 *
 *    History :
 *    1. Date        : April 7, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4271 $
 **************************************************************************/
#include "assert.h"
#include "arm_comm.h"

#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_buffer.h"

#define AUDIO_CLASS_GLOBAL
#include "audio_class.h"
#include "audio_desc.h"

volatile Boolean SampEna;
uint32_t  Delta;
int16_t * pSpkData;
int16_t * pSpkDataLin;

static volatile Boolean VolUpdate = TRUE;

#pragma segment="USB_DMA_RAM"
#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init int16_t AudioSpkData[SampRerFrame * 3];

#pragma data_alignment=4
uint8_t AudioBuf[2];

uint8_t   AudioRequest,AudioCS,AudioCN,AudioId;
uint16_t  AudioDataSize;

int16_t  AudioFeat1Vol;
Boolean AudioFeat1Mute;

/*************************************************************************
 * Function Name: UsbAudioClassInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB Class Audio Init
 *
 *************************************************************************/
void UsbAudioClassInit (void)
{
  // Init Audio Class variables
  SampEna         =\
  AudioFeat1Mute  = FALSE;
  AudioFeat1Vol   = 0;
  pSpkDataLin = pSpkData = AudioSpkData;
  UsbClassAudioConfigure(NULL);
  UsbCoreInit();
}

/*************************************************************************
 * Function Name: UsbClassAudioConfigure
 * Parameters:  pUsbDevCtrl_t pDev
 *
 * Return: none
 *
 * Description: USB Class Audio configure
 *
 *************************************************************************/
void UsbClassAudioConfigure (pUsbDevCtrl_t pDev)
{
  if(pDev == NULL)
  {
    if(UsbCoreReq(UsbCoreReqConfiquration) != 0)
    {
      SampEna = FALSE;
    }
  }
  else
  {
    if((pDev->Configuration == 1))
    {
      if (pDev->AlternateSetting[USB_SPK_INTERFACE] == 1)
      {
        pSpkDataLin = pSpkData = AudioSpkData;
        Delta = 0;
        SampEna = TRUE;
        USB_IO_Data( SpkEp,
                    (uint8_t *)AudioSpkData,
                     SpkEpMaxSize,
                    (void*)AudioInHadler);
      }
      else
      {
        SampEna = FALSE;
      }
    }
  }
}

/*************************************************************************
 * Function Name: AudioInHadler
 * Parameters:  USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: USB Class Audio Out EP handler
 *
 *************************************************************************/
static
void AudioInHadler (USB_Endpoint_t EP)
{
  assert(SpkEp == EP);

  pSpkData += EpCnfg[EP].Size/sizeof(uint16_t);
  if(pSpkData >= AudioSpkData + sizeof(AudioSpkData)/sizeof(uint16_t))
  {
    pSpkData = AudioSpkData;
  }

  ENTR_CRT_SECTION();
  Delta  += EpCnfg[EP].Size;
  EXT_CRT_SECTION();

  USB_IO_Data(SpkEp,(uint8_t *)pSpkData,SpkEpMaxSize,(void*)AudioInHadler);
}

/*************************************************************************
 * Function Name: AudioFeatureGetReg
 * Parameters:  uint32_t CS, uint32_t Id
 *
 * Return: Boolean
 *
 * Description:
 *
 *************************************************************************/
static
Boolean AudioFeatureGetReg (uint32_t CS, uint32_t Id)
{
  switch (CS)
  {
  case REQUEST_GET_CUR:
    // Load current value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (AudioFeat1Vol>>8)& 0xFF;
      AudioBuf[0] =  AudioFeat1Vol    & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  case REQUEST_GET_MIN:
    // Load minimum value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (Feat1MinVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat1MinVol      & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  case REQUEST_GET_MAX:
    // Load maximum value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (Feat1MaxVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat1MaxVol      & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  case REQUEST_GET_RES:
    // Load resolution value of Volume in the transmit buffer
    if (Id == FeatUnit1Id)
    {
      AudioBuf[1] = (Feat1ResVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat1ResVol      & 0xFF;
    }
    else
    {
      return(FALSE);
    }
    break;
  default:
    return(FALSE);
  }
  return(TRUE);
}

/*************************************************************************
 * Function Name: UsbClassAudioRequest
 * Parameters:  pUsbSetupPacket_t pSetup
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Class Audio Requests
 *
 *************************************************************************/
UsbCommStatus_t UsbClassAudioRequest (pUsbSetupPacket_t pSetup)
{
  // Validate Request
  switch (pSetup->mRequestType.Recipient)
  {
  case UsbRecipientInterface:
    // Feature Unit requests only Imlement for interface 0
    if (pSetup->wIndex.Word == (FeatUnit1Id << 8))
    {
      // Request type
      switch (pSetup->wValue.Hi)
      {
      case FU_MUTE_CONTROL:
        if ((pSetup->bRequest == REQUEST_SET_CUR) &&
            (pSetup->wLength.Word == 1))
        {
          // Set mute flag
          AudioRequest  = pSetup->bRequest;
          AudioId       = pSetup->wIndex.Hi;
          AudioCS       = pSetup->wValue.Hi;
          AudioCN       = pSetup->wValue.Lo;
          AudioDataSize = pSetup->wLength.Word;
          USB_IO_Data(CTRL_ENP_OUT,
                      AudioBuf,
                      USB_T9_Size(1,pSetup->wLength.Word),
                     (void *)UsbClassAudioData);
          return(UsbPass);
        }
        else if ((pSetup->bRequest == REQUEST_GET_CUR) &&
                 (pSetup->wLength.Word == 1))
        {
          // Read mute flag
          AudioBuf[0]  = AudioFeat1Mute;
          USB_IO_Data(CTRL_ENP_IN,
                      AudioBuf,
                      USB_T9_Size(1,pSetup->wLength.Word),
                     (void *)USB_StatusHandler);
          return(UsbPass);
        }
        break;
      case FU_VOLUME_CONTROL:
        if(pSetup->bRequest & 0x80)
        {
          // Read different volume values
          if((pSetup->wLength.Word == 2) &&
              AudioFeatureGetReg(pSetup->bRequest,pSetup->wIndex.Hi))
          {
          USB_IO_Data(CTRL_ENP_IN,
                      AudioBuf,
                      USB_T9_Size(2,pSetup->wLength.Word),
                     (void *)USB_StatusHandler);
            return(UsbPass);
          }
        }
        else if((pSetup->bRequest == REQUEST_SET_CUR) &&
                (pSetup->wLength.Word  == 2))
        {
          // Set volume value
          AudioRequest  = pSetup->bRequest;
          AudioId       = pSetup->wIndex.Hi;
          AudioCS       = pSetup->wValue.Hi;
          AudioCN       = pSetup->wValue.Lo;
          AudioDataSize = pSetup->wLength.Word;
          USB_IO_Data(CTRL_ENP_OUT,
                      AudioBuf,
                      USB_T9_Size(2,pSetup->wLength.Word),
                     (void *)UsbClassAudioData);
          return(UsbPass);
        }
        break;
      }
    }
    // Selector Unit requests only Imlement for interface 0
    else if (pSetup->wIndex.Word == (SelUnit1ID << 8))
    {
      if(pSetup->wValue.Word == 0)
      {
        // Read different selector unit values
        if(pSetup->bRequest & 0x80)
        {
          if(pSetup->wLength.Word == 1)
          {
            AudioBuf[0] = 1;
            USB_IO_Data(CTRL_ENP_IN,
                        AudioBuf,
                        USB_T9_Size(1,pSetup->wLength.Word),
                       (void *)USB_StatusHandler);
            return(UsbPass);
          }
        }
        else
        {
          // Set channel
          AudioRequest  = pSetup->bRequest;
          AudioId       = pSetup->wIndex.Hi;
          AudioDataSize = pSetup->wLength.Word;
          USB_IO_Data(CTRL_ENP_OUT,
                      AudioBuf,
                      USB_T9_Size(1,pSetup->wLength.Word),
                     (void *)UsbClassAudioData);
          return(UsbPass);
        }
      }
    }
    return(UsbFault);
  case UsbRecipientEndpoint:
    return(UsbFault);
  }
  return(UsbFault);
}

/*************************************************************************
 * Function Name: UsbClassAudioData
 * Parameters:  USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: USB Class Audio Data receive
 *
 *************************************************************************/
static
void UsbClassAudioData (USB_Endpoint_t EP)
{
  if (EpCnfg[EP].Status != COMPLETE)
  {
    USB_StallCtrlEP();
    return;
  }
  if(AudioId == FeatUnit1Id)
  {
    switch (AudioCS)
    {
    case FU_MUTE_CONTROL:
      // Set mute flag
      if (AudioId == FeatUnit1Id)
      {
        AudioFeat1Mute = AudioBuf[0];
      }
      else
      {
        USB_StallCtrlEP();
        return;
      }
      VolUpdate = TRUE;
      break;
    case FU_VOLUME_CONTROL:
      // Set volume value
      if (AudioId == FeatUnit1Id)
      {
        AudioFeat1Vol = AudioBuf[0] + ((uint16_t)AudioBuf[1]<<8);
        VolUpdate = TRUE;
      }
      else
      {
        USB_StallCtrlEP();
        return;
      }
      break;
    default:
      USB_StallCtrlEP();
      return;
    }
  }
  else if (AudioId == SelUnit1ID)
  {
    // empty
  }
  else
  {
    USB_StallCtrlEP();
    return;
  }
  USB_StatusHandler(CTRL_ENP_IN);
}

/*************************************************************************
 * Function Name: UsbAudioGetLinBuffer
 * Parameters: *uint8_t * pStream
 *
 * Return: none
 *
 * Description: Return Linear buffer pointer and size
 *
 *************************************************************************/
uint32_t UsbAudioGetLinBuffer(uint8_t ** ppStream)
{
uint32_t DeltaHold = Delta;

  if(DeltaHold)
  {
    *ppStream = (uint8_t*)pSpkDataLin;
    if(  (pSpkDataLin + (DeltaHold/sizeof(AudioSpkData[0])))
       > (AudioSpkData + sizeof(AudioSpkData)/sizeof(AudioSpkData[0])))
    {
      DeltaHold = (AudioSpkData + sizeof(AudioSpkData)/sizeof(AudioSpkData[0])) - pSpkDataLin;
      pSpkDataLin += DeltaHold;
      DeltaHold *= sizeof(AudioSpkData[0]);
    }
    else
    {
      pSpkDataLin += DeltaHold/sizeof(AudioSpkData[0]);
    }
    if (pSpkDataLin >= (AudioSpkData + sizeof(AudioSpkData)/sizeof(AudioSpkData[0])))
    {
      pSpkDataLin = AudioSpkData;
    }
    ENTR_CRT_SECTION();
    Delta  -= DeltaHold;
    EXT_CRT_SECTION();
  }
  return(DeltaHold);
}

/*************************************************************************
 * Function Name: UsbAudioVolumeGetVolume
 * Parameters:  uint32_t * pVol
 *
 * Return: Boolean
 *
 * Description: Return Volume value and update status
 *
 *************************************************************************/
Boolean UsbAudioVolumeGetVolume(uint32_t * pVol)
{
Boolean Update;

  if(FALSE != (Update = AtomicExchange (0, (uint32_t *)&VolUpdate)))
  {
    *pVol = AudioFeat1Mute ? Feat1MinVol : AudioFeat1Vol;
  }
  return(Update);
}

/*************************************************************************
 * Function Name: UsbAudioGetPlayStatus
 * Parameters:  none
 *
 * Return: Boolean
 *
 * Description: Return current status (play/stop)
 *
 *************************************************************************/
Boolean UsbAudioGetPlayStatus(void)
{
  return(SampEna);
}
