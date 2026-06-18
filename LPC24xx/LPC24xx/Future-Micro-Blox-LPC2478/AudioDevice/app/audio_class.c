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
 *    1. Date        : October 3, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#define AUDIO_CLASS_GLOBAL
#include "audio_class.h"

#define L3_DATA_MODE() FIO0SET  = 1UL << 6
#define L3_CMD_MODE()  FIO0CLR  = 1UL << 6

#define L3_UDA1344_ADDR   0x28
#define L3_DATA           0x00
#define L3_STATUS         0x40

#define L3_256_FS         0x04
#define L3_DATA_I2S_16B   0x00
#define L3_DC_FILT        0x80

#define L3_VOL            0x00

#define L3_BASS_TREB      0x02
#define L3_BASS_2DB       0x20
#define L3_TREB_6DB       0xC0

#define L3_OTHER          0x01
#define L3_DE_EMPH_44KHZ  0x08
#define L3_MUTE_OFF       0x00
#define L3_MUTE_ON        0x20
#define L3_MODE_MAX       0xC0

#define L3_POWER_CTRL     0x03
#define L3_ADC_ON         0x40
#define L3_ADC_OFF        0x00
#define L3_DAC_ON         0x80
#define L3_DAC_OFF        0x00

volatile Boolean SampEna;
Int32U  SampCount,SampPeriod,Delta;
pInt16S pSpkData;
Int32U  SampPerCurrHold,DeltaPer;

#pragma segment="USB_DMA_RAM"
#pragma location="USB_DMA_RAM"
#pragma data_alignment=4
__no_init Int16S AudioSpkData[SampRerFrame * 3];

#pragma data_alignment=4
Int8U AudioBuf[2];

Int8U   AudioRequest,AudioCS,AudioCN,AudioId;
Int16U  AudioDataSize;

Int16S  AudioFeat1Vol;
Int32U  AudioSpkVolMul;
Boolean AudioFeat1Mute;

/*************************************************************************
 * Function Name: L3_Write
 * Parameters: Int8U Addr, Int8U Data
 *
 * Return: none
 *
 * Description: Write data in UDA1344
 *
 *************************************************************************/
void L3_Write(Int8U Addr, Int8U Data)
{
volatile Int32U Dumy;
Int32U Save;
  ENTR_CRT_SECTION(Save);
  L3_CMD_MODE();
  for(volatile Int32U i = 10; i; i--);
  while(!SSP1SR_bit.TNF);
  SSP1DR = Addr;
  while(!SSP1SR_bit.RNE);
  Dumy = SSP1DR;
  for(volatile Int32U i = 10; i; i--);
  L3_DATA_MODE();
  for(volatile Int32U i = 10; i; i--);
  L3_CMD_MODE();
  for(volatile Int32U i = 10; i; i--);
  L3_DATA_MODE();
  for(volatile Int32U i = 10; i; i--);
  while(!SSP1SR_bit.TNF);
  SSP1DR = Data;
  while(!SSP1SR_bit.RNE);
  Dumy = SSP1DR;
  for(volatile Int32U i = 10; i; i--);
  L3_CMD_MODE();
  for(volatile Int32U i = 10; i; i--);
  L3_DATA_MODE();
  EXT_CRT_SECTION(Save);
}

/*************************************************************************
 * Function Name: InitUDA1344
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init UDA1344
 *
 *************************************************************************/
void InitUDA1344(void)
{
  // Set Status part
  // system clock frequency = 256fs, DC fitering, I2S mode
  L3_Write(L3_UDA1344_ADDR | L3_STATUS, L3_DATA_I2S_16B | L3_256_FS | L3_DC_FILT);
  // Set Data part
  // set volume 0 db
  L3_Write(L3_UDA1344_ADDR | L3_DATA, L3_VOL | 0);
  // set bass boost 2 dB and treble 6dB
  L3_Write(L3_UDA1344_ADDR | L3_DATA, L3_BASS_TREB | L3_BASS_2DB | L3_TREB_6DB);
  // set other settings - De-emphasis 48kHz, Mute - on, Mode Max,
  L3_Write(L3_UDA1344_ADDR | L3_DATA, L3_OTHER | L3_DE_EMPH_44KHZ | L3_MUTE_OFF | L3_MODE_MAX);
  // set power control - DAC ON, ADC OFF
  L3_Write(L3_UDA1344_ADDR | L3_DATA, L3_POWER_CTRL | L3_ADC_ON | L3_DAC_ON);
}


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
  SampCount       =\
  AudioFeat1Vol   = 0;
  pSpkData        = AudioSpkData;
  SampPerCurrHold = SampPeriod;

  // Init I2S and HT82V731 I2S DAC
  // Set I2S CLK Fcckl/1
  PCLKSEL1_bit.PCLK_I2S = 1;
  // enable I2S in the PCONP register. I2S is disabled on reset*/
  PCONP |= (1 << 27);

  // Init UDA1344 clock
  // The hardware is little complex :(. TIMER1 CAP1.0 and MAT1.0 are used for BCKL signal generation
  // from external clock (11.2896MHz/256 = fs = 44100 fs*2*16 = bclk = 1.411HMz)
  // TIMER0 CAP0.0 and MAT0.0 are used for WS signal generation from the BCKL signal.
  PINSEL7_bit.P3_27 = 2;  // CAP1.0
  PINSEL7_bit.P3_29 = 2;  // MAT1.0
  PINSEL7_bit.P3_23 = 2;  // CAP0.0
  PINSEL7_bit.P3_25 = 2;  // MAT0.0
  // Init Time1 - BCLK
  PCLKSEL0_bit.PCLK_TIMER1 = 1;
  PCONP_bit.PCTIM1 = 1; // Enable TMR1 clk
  T1TCR_bit.CE = 0;     // counting  disable
  T1TCR_bit.CR = 1;     // set reset
  T1TCR_bit.CR = 0;     // release reset
  T1CTCR_bit.CTM = 2;   // TC is incremented on falling edges on the CAP input
  T1CTCR_bit.CIS = 0;   // CAP1.0
  T1MCR_bit.MR0I = 0;   // no interrupt Interrupt on MR0
  T1MCR_bit.MR0R = 1;   // Enable reset on MR0
  T1MCR_bit.MR0S = 0;   // Disable stop on MR0
  T1EMR_bit.EMC0 = 3;   // Toggle pin
  // set timer 1 period
  T1PR = 1-1;
  T1MR0 = 2;
  T1TCR_bit.CE = 1;     // counting Enable
  // Init Time0 - WS
  PCLKSEL0_bit.PCLK_TIMER0 = 1;
  PCONP_bit.PCTIM0 = 1; // Enable TMR0 clk
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 2;   // TC is incremented on falling edges on the CAP input
  T0CTCR_bit.CIS = 0;   // CAP1.0
  T0MCR_bit.MR0I = 0;   // no interrupt Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  T0EMR_bit.EMC0 = 3;   // Toggle pin
  // set timer 1 period
  T0PR = 1-1;
  T0MR0 =32;
  T0TCR_bit.CE = 1;     // counting Enable

  // connect the I2S sigals to port pins
  // P2.11 - TX_CLK, P2.12 - TX_WS, P2.13 - TX_SDA
  PINSEL4_bit.P2_11 = 3;
  PINSEL4_bit.P2_12 = 3;
  PINSEL4_bit.P2_13 = 3;

  /* Audio output is the master, audio input is the slave, */
  I2SDAO_bit.RESET      = 1;  // reset FIFIO
  I2SDAO_bit.WORS_WIDTH = 1;  // 16 bits
  I2SDAO_bit.MONO       = 0;  // Stereo
  I2SDAO_bit.WS_SEL     = 1;  // Slave mode
  I2SDAO_bit.WS_HALFPERIOD = 32-1;
  I2SDAO_bit.RESET      = 0;  // release reset
  I2SDAO_bit.MUTE       = 0;  // release mute
  I2SDAO_bit.STOP       = 0;  // start I2S

  // Init System Timer - Timer2
  // Init Time2
  PCLKSEL1_bit.PCLK_TIMER2 = 1;
  PCONP_bit.PCTIM2 = 1; // Enable TMR2 clk
  T2TCR_bit.CE = 0;     // counting  disable
  T2TCR_bit.CR = 1;     // set reset
  T2TCR_bit.CR = 0;     // release reset
  T2CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T2MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T2MCR_bit.MR0R = 1;   // Enable reset on MR0
  T2MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 2 period
  T2PR = 1-1;
  SampPeriod = (SYS_GetFpclk(TIMER2_PCLK_OFFSET))/SampFreq;
  DeltaPer = SampPeriod/(SampRerFrame*2);
  T2MR0 = SampPeriod;
  // init timer 2 interrupt
  T2IR_bit.MR0INT = 1;  // clear pending interrupt
  // Timer 2 interrupt is assign to the FIQ interrupt line
  VICINTSELECT = 1<<VIC_TIMER2;
  VICINTENABLE |= 1UL << VIC_TIMER2;
  T2TCR_bit.CE = 1;     // counting Enable

  // Init SSP1 (L3 interface)
  // connect the SSP1 sigals to port pins
  // P0.6 - SS, P0.7 - SCLK, P0.8 - MISO and P0.9 - MOSI
  PINSEL0_bit.P0_6 = 0;
  FIO0DIR_bit.P0_6 = 1;
  L3_DATA_MODE();
  PINSEL0_bit.P0_7 = 2;
  PINSEL0_bit.P0_8 = 2;
  PINSEL0_bit.P0_9 = 2;
  PCONP_bit.PCSSP1 = 1;     // enable clock of SSP1

  SSP1CR1  = 0;             // Disable SSP, master mode
  SSP1CR0  = 0xC7;          // 8 bits
  SSP1CPSR = 24;
  SSP1CR1_bit.SSE = 1;      // Enable SSP

  for (Int32U i = 0; i < 8; i++ )
  {
    volatile Int32U Dummy = SSP1DR; /* clear the RxFIFO */
  }

  // Init UDA1344
  InitUDA1344();
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
        pSpkData = AudioSpkData;
        Delta = 0;
        USB_IO_Data( SpkEp,
                    (pInt8U)AudioSpkData,
                     SpkEpMaxSize,
                    (void*)AudioInHadler);
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
Int32U save;
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

  ENTR_CRT_SECTION_F(save);
  Delta  += EpCnfg[EP].Size;
  EXT_CRT_SECTION_F(save);

  USB_IO_Data(SpkEp,(pInt8U)pSpkData,SpkEpMaxSize,(void*)AudioInHadler);
}

/*************************************************************************
 * Function Name: FIQ_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
__fiq __arm void FIQ_Handler (void)
{
static Int16U Data;

  // Set DAC
  I2STXFIFO = Data*0x10001;

  if (SampEna)
  {
    if(Delta > 1)
    {
      // Get next input sample
      Data = AudioSpkData[SampCount++];
      Delta   -= 2;
      // Correct pointer to sample buffer
      if(SampCount >= sizeof(AudioSpkData)/sizeof(Int16S))
      {
        SampCount = 0;
      }
      if(AudioFeat1Mute)
      {
        // set middle of range
        Data = 0;
      }
    }
    else
    {
      // Disable output stream after emptying the buffer
      SampEna = FALSE;
      SampPerCurrHold = SampPeriod;
      Data = 0;
    }
  }
  else
  {
    SampPerCurrHold = SampPeriod;
    Data = 0;
  }

  T2MR0 = SampPerCurrHold;
  T2IR  = T2IR;
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
          if (AudioFeat1Vol < ((Int16S)Feat1MinVol))
          {
            AudioFeat1Vol = Feat1MinVol;
          }
          else if(AudioFeat1Vol > (Int16S)Feat1MaxVol)
          {
            AudioFeat1Vol = Feat1MaxVol;
          }
          AudioFeat1Vol  = -AudioFeat1Vol;
          // -60dB - +0dB
          UsbClassAudioVolCtrl(AudioFeat1Vol>>8);
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

/*************************************************************************
 * Function Name: UsbClassAudioVolCtrl
 * Parameters:  Int8U Db
 *
 * Return: none
 *
 * Description: USB Class Audio set volume (0 - -60)
 *
 *************************************************************************/
static
void UsbClassAudioVolCtrl (Int8U Db)
{
Int8U tmp;
  if(Db > 0x3F)
  {
    Db = 0x3F;
  }
  tmp = 0;
  // revert bits order
  for(Int8U mask = 1; mask; mask <<= 1)
  {
    tmp <<= 1;
    if(Db & mask)
    {
      tmp |= 0x1;
    }
  }
  L3_Write(L3_UDA1344_ADDR | L3_DATA, L3_VOL | tmp);
}
