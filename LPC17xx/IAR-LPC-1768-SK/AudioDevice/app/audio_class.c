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
 *    $Revision: 28 $
 **************************************************************************/
#define AUDIO_CLASS_GLOBAL
#include "audio_class.h"

extern void NVIC_IntEnable(Int32U IntNumber);
extern void NVIC_IntDisable(Int32U IntNumber);
extern void NVIC_ClrPend(Int32U IntNumber);
extern void NVIC_IntPri(Int32U IntNumber, Int8U Priority);

volatile Boolean SampEna,MicEna;
Int32U  SampCount,SampPeriod,Delta,MicCurrBuffer;
pInt16S pSpkData,pMicData;
Int32U  SampPerCurrHold,DeltaPer;

const Int32U VolumeMul [] =
{
  // 256 - 0
  //-48dB- 0x0
  261,
  //-47dB- 0x1
  293,
  //-46dB- 0x2
  328,
  //-45dB- 0x3
  369,
  //-44dB- 0x4
  414,
  //-43dB- 0x5
  464,
  //-42dB- 0x6
  521,
  //-41dB- 0x7
  584,
  //-40dB- 0x8
  655,
  //-39dB- 0x9
  735,
  //-38dB- 0xA
  825,
  //-37dB- 0xB
  926,
  //-36dB- 0xC
  1039,
  //-35dB- 0xD
  1165,
  //-34dB- 0xE
  1308,
  //-33dB- 0xF
  1467,
  //-32dB- 0x10
  1646,
  //-31dB- 0x11
  1847,
  //-30dB- 0x12
  2072,
  //-29dB- 0x13
  2325,
  //-28dB- 0x14
  2609,
  //-27dB- 0x15
  2927,
  //-26dB- 0x16
  3285,
  //-25dB- 0x17
  3685,
  //-24dB- 0x18
  4135,
  //-23dB- 0x19
  4640,
  //-22dB- 0x1A
  5206,
  //-21dB- 0x1B
  5841,
  //-20dB- 0x1C
  6554,
  //-19dB- 0x1D
  7353,
  //-18dB- 0x1E
  8250,
  //-17dB- 0x1F
  9257,
  //-16dB- 0x20
  10387,
  //-15dB- 0x21
  11654,
  //-14dB- 0x22
  13076,
  //-13dB- 0x23
  14672,
  //-12dB- 0x24
  16462,
  //-11dB- 0x25
  18471,
  //-10dB- 0x26
  20724,
  //-9dB - 0x27
  23253,
  //-8dB - 0x28
  26090,
  //-7dB - 0x29
  29274,
  //-6dB - 0x2A
  32846,
  //-5dB - 0x2B
  36854,
  //-4dB - 0x2C
  41350,
  //-3dB - 0x2D
  46396,
  //-2dB - 0x2E
  52057,
  //-1dB - 0x2F
  58409,
  // 0dB - 0x30
  65536,
  // 1dB - 0x31
  73533,
  // 2dB - 0x32
  82505,
  // 3dB - 0x33
  92572,
  // 4dB - 0x34
  103868,
  // 5dB - 0x35
  116541,
  // 6dB - 0x36
  130762,
};

#pragma segment="USB_DMA_RAM"
#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int16S AudioSpkData[SampRerFrame * 3];

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int16S AudioMicData1[SampRerFrame * 2];

#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int16S AudioMicData2[SampRerFrame * 2];

#pragma data_alignment=4
Int8U AudioBuf[2];

Int8U   AudioRequest,AudioCS,AudioCN,AudioId;
Int16U  AudioDataSize;

Int16S  AudioFeat1Vol;
Int32U  AudioSpkVolMul;
Boolean AudioFeat1Mute;

Int16S  AudioFeat2Vol;
Int32U  AudioMicVolMul;
Boolean AudioFeat2Mute;


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
  AudioFeat1Mute  =\
  AudioFeat2Mute  = FALSE;
  SampCount       =\
  AudioFeat1Vol   =\
  AudioFeat2Vol   = 0;
  pSpkData        = AudioSpkData;
  SampPerCurrHold = SampPeriod;

  // DAC Init
  PCONP_bit.PCAD = 1;   // Enable DAC/ADC clk
  PINSEL1_bit.P0_26 = 2;
  DACR_bit.VALUE = 0x200;
  // ADC 0.2 Init
  PINSEL1_bit.P0_25 = 1;
  // ADC clk = 18MHz/4, Ch2
  AD0CR = (1<<21) + (4 << 8) + (1<<2);

  // Init System Timer - Timer0
  // Init Time0
  PCONP_bit.PCTIM0 = 1; // Enable TMR0 clk
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T0PR = 1-1;
  SampPeriod = (SYS_GetFpclk(TIMER0_PCLK_OFFSET))/SampFreq;
  DeltaPer = SampPeriod/(SampRerFrame*2);
  T0MR0 = SampPeriod;
  // init timer 0 interrupt
  T0IR_bit.MR0INT = 1;  // clear pending interrupt
  // Timer 0 interrupt is assign to the FIQ interrupt line

  NVIC_IntEnable(NVIC_TIMER0);
  NVIC_IntPri(NVIC_TIMER0,0);
  T0TCR_bit.CE = 1;     // counting Enable

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
      SampEna = MicEna = FALSE;
    }
  }
  else
  {
    if((pDev->Configuration == 1))
    {
      if (pDev->AlternateSetting[USB_SPK_INTERFACE] == 1)
      {
        pSpkData = AudioSpkData;
        Delta = 0;
        USB_IO_Data( SpkEp,
                    (pInt8U)AudioSpkData,
                     SpkEpMaxSize,
                    (void*)AudioInHadler);
      }
      if (pDev->AlternateSetting[USB_MIC_INTERFACE] == 1)
      {
        MicEna = FALSE;
        pMicData = AudioMicData1;
        MicCurrBuffer = 0;
        USB_IO_Data( MicEp,
                     NULL,
                     0,
                    (void*)AudioOutHadler);
      }
      else
      {
        MicEna = FALSE;
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
#if __CORE__ < 7
  Int32U save;
#endif // __CORE__ < 7
  assert(SpkEp == EP);

  pSpkData += EpCnfg[EP].Size/sizeof(Int16U);
  if(pSpkData >= AudioSpkData + sizeof(AudioSpkData)/sizeof(Int16U))
  {
    pSpkData = AudioSpkData;
  }

  if(SampEna)
  {
    // Adapt sample rate
    if (Delta > (SampRerFrame * SubFrameSize))
    {
      SampPerCurrHold = SampPeriod - DeltaPer;
    }
    else if (Delta < (SampRerFrame * SubFrameSize))
    {
      SampPerCurrHold = SampPeriod + DeltaPer;
    }
    else
    {
      SampPerCurrHold = SampPeriod;
    }
  }
  else
  {
    if(pSpkData >= (AudioSpkData + (sizeof(AudioSpkData))/(2*sizeof(Int16U))))
    {
      // Enable output
      SampCount = 0;
      SampEna   = TRUE;
    }
  }

  #if __CORE__ < 7
    ENTR_CRT_SECTION_F(save);
  #else
    ENTR_CRT_SECTION();
  #endif // __CORE__ < 7
  Delta  += EpCnfg[EP].Size;
  #if __CORE__ < 7
    EXT_CRT_SECTION(Save);
  #else
    EXT_CRT_SECTION();
  #endif // __CORE__ < 7

  USB_IO_Data(SpkEp,(pInt8U)pSpkData,SpkEpMaxSize,(void*)AudioInHadler);
}

/*************************************************************************
 * Function Name: AudioOutHadler
 * Parameters:  USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: USB Class Audio In EP handler
 *
 *************************************************************************/
static
void AudioOutHadler (USB_Endpoint_t EP)
{
Int32U PacketLength;
#if __CORE__ < 7
  Int32U save;
#endif // __CORE__ < 7

  #if __CORE__ < 7
    ENTR_CRT_SECTION(Save);
  #else
    ENTR_CRT_SECTION();
  #endif // __CORE__ < 7
  MicEna = TRUE;
  // Set pointer to free buffer
  if(++MicCurrBuffer & 1)
  {
    PacketLength = (Int32U)pMicData - (Int32U)AudioMicData1;
    pMicData = AudioMicData2;
  }
  else
  {
    PacketLength = (Int32U)pMicData - (Int32U)AudioMicData2;
    pMicData = AudioMicData1;
  }
  #if __CORE__ < 7
    EXT_CRT_SECTION(Save);
  #else
    EXT_CRT_SECTION();
  #endif // __CORE__ < 7
  // Send microphone output buffer
  USB_IO_Data( MicEp,
              (pInt8U)((MicCurrBuffer & 1)?AudioMicData1:AudioMicData2),
              PacketLength,
              (void*)AudioOutHadler);
}

/*************************************************************************
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
void TMR0_IRQHandler (void)
{
static union ValUnion
{
  Int32S Data;
  struct
  {
    Int16U DataLo;
    Int16S DataHi;
  };
} Val = {0x02000000};

union ValUnion MicTemp;

  // Set DAC
  DACR_bit.VALUE = Val.DataHi;

  if (SampEna)
  {
    if(Delta > 1)
    {
      // Get next input sample
      Val.Data = AudioSpkData[SampCount++];
      Delta   -= 2;
      // Correct pointer to sample buffer
      if(SampCount >= sizeof(AudioSpkData)/sizeof(Int16U))
      {
        SampCount = 0;
      }
      if(!AudioFeat1Mute && AudioSpkVolMul)
      {
        // Apply volume
        Val.Data   *= AudioSpkVolMul;
        // Add offset
        Val.DataHi += 0x200;
        // Check for overflow and correct value
        if(Val.DataHi < 0)
        {
          Val.DataHi = 0;
        }
        else if(Val.DataHi > 0x3FF)
        {
          Val.DataHi = 0x3FF;
        }
      }
      else
      {
        // set middle of range
        Val.DataHi = 0x200;
      }
    }
    else
    {
      // Disable output stream after emptying the buffer
      SampEna = FALSE;
      SampPerCurrHold = SampPeriod;
    }
  }
  else
  {
    SampPerCurrHold = SampPeriod;
  }

  if(MicEna)
  {
    if(!AudioFeat2Mute && AudioMicVolMul)
    {
      // Get ADC sample and remove offset
      MicTemp.Data  = ADDR2_bit.RESULT - 0x800;
      // Start conversion
      AD0CR_bit.START = 1;
      // Add volume
      MicTemp.Data *= AudioMicVolMul;
      // check for overflow and correct the result
      if(MicTemp.DataHi < -1023)
      {
        MicTemp.Data = -1023 * 65536;
      }
      else if(MicTemp.DataHi > 1023)
      {
        MicTemp.Data = 1023 * 65536;
      }
      // Scale to 16bit
      MicTemp.Data <<= 4;
    }
    else
    {
      // set middle of range
      MicTemp.DataHi = 0;
    }
    // write to output buffer
    *pMicData++ = MicTemp.DataHi;
  }

  T0MR0 = SampPerCurrHold;
  T0IR  = T0IR;
  NVIC_ClrPend(NVIC_TIMER0);
}

/*************************************************************************
 * Function Name: AudioFeatureGetReg
 * Parameters:  Int32U CS, Int32U Id
 *
 * Return: Boolean
 *
 * Description:
 *
 *************************************************************************/
static
Boolean AudioFeatureGetReg (Int32U CS, Int32U Id)
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
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (AudioFeat2Vol>>8)& 0xFF;
      AudioBuf[0] =  AudioFeat2Vol    & 0xFF;
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
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (Feat2MinVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat2MinVol      & 0xFF;
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
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (Feat2MaxVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat2MaxVol      & 0xFF;
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
    else if (Id == FeatUnit2Id)
    {
      AudioBuf[1] = (Feat2ResVol>>8)  & 0xFF;
      AudioBuf[0] =  Feat2ResVol      & 0xFF;
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
    if ((pSetup->wIndex.Word == (FeatUnit1Id << 8)) ||
        (pSetup->wIndex.Word == (FeatUnit2Id << 8)))
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
          if(pSetup->wIndex.Hi == FeatUnit1Id)
          {
            AudioBuf[0]  = AudioFeat1Mute;
          }
          else
          {
            AudioBuf[0]  = AudioFeat2Mute;
          }
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
  if((AudioId == FeatUnit1Id) ||
     (AudioId == FeatUnit2Id))
  {
    switch (AudioCS)
    {
    case FU_MUTE_CONTROL:
      // Set mute flag
      if (AudioId == FeatUnit1Id)
      {
        AudioFeat1Mute = AudioBuf[0];
      }
      else if (AudioId == FeatUnit2Id)
      {
        AudioFeat2Mute = AudioBuf[0];
      }
      else
      {
        USB_StallCtrlEP();
        return;
      }
      break;
    case FU_VOLUME_CONTROL:
      // Set volume value
      if (AudioId == FeatUnit1Id)
      {
        AudioFeat1Vol = AudioBuf[0] + ((Int16U)AudioBuf[1]<<8);
        if((Int16U)AudioFeat1Vol == MinVol)
        {
          AudioSpkVolMul = 0;
        }
        else
        {
          if (AudioFeat1Vol < (Int16S)Feat1MinVol)
          {
            AudioFeat1Vol = Feat1MinVol;
          }
          else if(AudioFeat1Vol > (Int16S)Feat1MaxVol)
          {
            AudioFeat1Vol = Feat1MaxVol;
          }
          // -48dB - +6dB multiply
          AudioSpkVolMul = (VolumeMul[((AudioFeat1Vol>>8)+0x30) & 0x3F]) >> 6;
        }
      }
      else if (AudioId == FeatUnit2Id)
      {
        AudioFeat2Vol = AudioBuf[0] + ((Int16U)AudioBuf[1]<<8);
        if((Int16U)AudioFeat2Vol == MinVol)
        {
          AudioMicVolMul = 0;
        }
        else
        {
          if (AudioFeat2Vol < (Int16S)Feat2MinVol)
          {
            AudioFeat2Vol = Feat2MinVol;
          }
          else if(AudioFeat2Vol > (Int16S)Feat2MaxVol)
          {
            AudioFeat2Vol = Feat2MaxVol;
          }
          // -48dB - +6dB multiply
          AudioMicVolMul = VolumeMul[((AudioFeat2Vol>>8)+0x30) & 0x3F];
        }
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
