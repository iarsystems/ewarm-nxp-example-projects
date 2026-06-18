/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : audio_desc.h
 *    Description : AUDIO descriptor definitions
 *
 *    History :
 *    1. Date        : November 29, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __AUDIO_DESC_H
#define __AUDIO_DESC_H

typedef enum
{
  SUBCLASS_UNDEFINED = 0, AUDIOCONTROL, AUDIOSTREAMING,
  MIDISTREAMING,
} AudioInterfaceSubclass_t;

typedef enum
{
  PR_PROTOCOL_UNDEFINED = 0,
} AudioInterfaceProtocol_t;

typedef enum
{
  CS_UNDEFINED = 0x20,CS_DEVICE,CS_CONFIGURATION,CS_STRING,
  CS_INTERFACE,CS_ENDPOINT,
} AudioClassSpecificDescriptorTypes_t;

typedef enum
{
  AC_DESCRIPTOR_UNDEFINED = 0,HEADER,INPUT_TERMINAL,OUTPUT_TERMINAL,
  MIXER_UNIT,SELECTOR_UNIT,FEATURE_UNIT,PROCESSING_UNIT,EXTENSION_UNIT,
} AudioACInterfaceDescriptorSubtypes_t;

typedef enum
{
  AS_DESCRIPTOR_UNDEFINED = 0,AS_GENERAL,FORMAT_TYPE,FORMAT_SPECIFIC,
} AudioASInterfaceDescriptorSubtypes_t;

typedef enum
{
  DESCRIPTOR_UNDEFINED = 0,EP_GENERAL,
} AudioASEndpoinDescriptorSubtypes_t;

typedef enum
{
  FORMAT_TYPE_UNDEFINED = 0,FORMAT_TYPE_I,FORMAT_TYPE_II,FORMAT_TYPE_III,
} AudioFormatTypeCodes_t;

typedef enum
{
  TYPE_I_UNDEFINED = 0,PCM,PCM8,IEEE_FLOAT,ALAW,MULAW,
} AudioDataFormatTypeI_t;

typedef enum
{
  TYPE_II_UNDEFINED = 0x1000,MPEG,AC_3,
} AudioDataFormatTypeII_t;

typedef enum
{
  TYPE_III_UNDEFINED = 0x2000,IEC1937_AC_3,IEC1937_MPEG_1,
  IEC1937_MPEG_2_NOEXT, IEC1937_MPEG_2_EXT, IEC1937_MPEG_2_Layer1_LS,
  IEC1937_MPEG_2_Layer2_3_LS,
} AudioDataFormatTypeIII_t;

typedef enum
{
  Undefined = 0, Milliseconds, DecodedPCMsamples,
} ClassEpLockDelay_t;

typedef union
{
  uint16_t wChannelConfig;
  struct
  {
     uint16_t LF : 1;   // Left Front
     uint16_t RF : 1;   // Right Front
     uint16_t CF : 1;   // Center Front
     uint16_t LFE: 1;   // Low Frequency Enhancement
     uint16_t Ls : 1;   // Left Surround
     uint16_t Rs : 1;   // Right Surround
     uint16_t Lc : 1;   // Left of Center
     uint16_t Rc : 1;   // Right of Center

     uint16_t S  : 1;   // Surround
     uint16_t Sl : 1;   // Side Left
     uint16_t Sr : 1;   // Side Right
     uint16_t T  : 1;   // Top
     uint16_t    : 4;   // Reserved
  };
} ChannelConfig_t;

#define FeatUnitMute                        (1UL<<0)
#define FeatUnitVolume                      (1UL<<1)
#define FeatUnitBass                        (1UL<<2)
#define FeatUnitMid                         (1UL<<3)
#define FeatUnitTreble                      (1UL<<4)
#define FeatUnitGraphicEqualizer            (1UL<<5)
#define FeatUnitAutomaticGain               (1UL<<6)
#define FeatUnitDelay                       (1UL<<7)
#define FeatUnitBassBoost                   (1UL<<8)
#define FeatUnitLoudness                    (1UL<<9)

// Predefined Audio Channel Configuration Bits
#define AUDIO_CHANNEL_M                     0x0000  // Mono
#define AUDIO_CHANNEL_L                     0x0001  // Left Front
#define AUDIO_CHANNEL_R                     0x0002  // Right Front
#define AUDIO_CHANNEL_C                     0x0004  // Center Front
#define AUDIO_CHANNEL_LFE                   0x0008  // Low Frequency Effects
#define AUDIO_CHANNEL_LS                    0x0010  // Left Surround
#define AUDIO_CHANNEL_RS                    0x0020  // Right Surround
#define AUDIO_CHANNEL_LC                    0x0040  // Left of Center
#define AUDIO_CHANNEL_RC                    0x0080  // Right of Center
#define AUDIO_CHANNEL_S                     0x0100  // Surround
#define AUDIO_CHANNEL_SL                    0x0200  // Side Left
#define AUDIO_CHANNEL_SR                    0x0400  // Side Right
#define AUDIO_CHANNEL_T                     0x0800  // Top

//  USB Terminal Types
#define AudioUsbUndefined                   0x0100
#define AudioUsbStreaming                   0x0101
#define AudioUsbVendorSpecific              0x01FF

// Input Terminal Types
#define AudioInputUndefined                 0x0200
#define AudioInputMic                       0x0201
#define AudioInputDesktopMic                0x0202
#define AudioInputPersonalMic               0x0203
#define AudioInputOmniDirectionalMic        0x0204
#define AudioInputMicArray                  0x0205
#define AudioInputProcessingMicArray        0x0206

//  Output Terminal Types
#define AudioOutputUndefined                0x0300
#define AudioOutputSpk                      0x0301
#define AudioOutputHeadphones               0x0302
#define AudioOutputHeadMountedDisplayAudio  0x0303
#define AudioOutputDesktopSpk               0x0304
#define AudioOutputRoomSpk                  0x0305
#define AudioOutputCommunicationSpk         0x0306
#define AudioOutputLowFrequencyEffectsSpk   0x0307

// Audio Class-Specific Request Codes
#define REQUEST_CODE_UNDEFINED              0x00
#define REQUEST_SET_CUR                     0x01
#define REQUEST_GET_CUR                     0x81
#define REQUEST_SET_MIN                     0x02
#define REQUEST_GET_MIN                     0x82
#define REQUEST_SET_MAX                     0x03
#define REQUEST_GET_MAX                     0x83
#define REQUEST_SET_RES                     0x04
#define REQUEST_GET_RES                     0x84
#define REQUEST_SET_MEM                     0x05
#define REQUEST_GET_MEM                     0x85
#define REQUEST_GET_STAT                    0xFF

// Terminal Control Selectors
#define TE_CONTROL_UNDEFINED                0x00
#define TE_COPY_PROTECT_CONTROL             0x01

// Feature Unit Control Selectors
#define FU_CONTROL_UNDEFINED                0x00
#define FU_MUTE_CONTROL                     0x01
#define FU_VOLUME_CONTROL                   0x02
#define FU_BASS_CONTROL                     0x03
#define FU_MID_CONTROL                      0x04
#define FU_TREBLE_CONTROL                   0x05
#define FU_GRAPHIC_EQUALIZER_CONTROL        0x06
#define FU_AUTOMATIC_GAIN_CONTROL           0x07
#define FU_DELAY_CONTROL                    0x08
#define FU_BASS_BOOST_CONTROL               0x09
#define FU_LOUDNESS_CONTROL                 0x0A

// Endpoint Control Selectors
#define EP_CONTROL_UNDEFINED                0x00
#define SAMPLING_FREQ_CONTROL               0x01
#define PITCH_CONTROL                       0x02

#pragma pack(1)
typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint16_t            bcdADC;
  uint16_t            wTotalLength;
  uint8_t             bInCollection;
  uint8_t             baInterfaceNr1;
} UsbClassSpecificACInterfaceDescriptor_t, *pUsbClassSpecificACInterfaceDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bTerminalID;
  uint16_t            wTerminalType;
  uint8_t             bAssocTerminal;
  uint8_t             bNrChannels;
  ChannelConfig_t   wChannelConfig;
  uint8_t             iChannelNames;
  uint8_t             iTerminal;
} UsbInputTerminalDescriptor_t, *pUsbInputTerminalDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bTerminalLink;
  uint8_t             bDelay;
  uint16_t            wFormatTag;
} UsbClassSpecificASInterfaceDescriptor_t, *pUsbClassSpecificASInterfaceDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bTerminalID;
  uint16_t            wTerminalType;
  uint8_t             bAssocTerminal;
  uint8_t             bSourceID;
  uint8_t             iTerminal;
} UsbOutputTerminalDescriptor_t, *pUsbOutputTerminalDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bFormatType;
  uint8_t             bNrChannels;
  uint8_t             bSubframeSize;
  uint8_t             bBitResolution;
  uint8_t             bSamFreqType;
} UsbTypeIFormatTypeDescriptor_t, *pUsbTypeIFormatTypeDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bEndpointAddress;
  EpAttr_t          bmAttributes;
  uint16_t            wMaxPacketSize;
  uint8_t             bInterval;
  uint8_t             bRefresh;
  uint8_t             bSynchAddress;
} StandardASEndpointDescriptor_t, *pStandardASEndpointDescriptor_t;

typedef union
{
  uint8_t Attribute;
  struct
  {
    uint8_t SamplFreq     : 1;
    uint8_t Pitch         : 1;
    uint8_t               : 5;
    uint8_t MaxPacketsOnly: 1;
  };
} ClassEpAttr_t, *pClassEpAttr_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  ClassEpAttr_t     bmAttributes;
  uint8_t             bLockDelayUnits;
  uint16_t            wLockDelay;
} ClassSpecASEndpointDescriptor_t, *pClassSpecASEndpointDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bUnitID;
  uint8_t             bSourceID;
  uint8_t             bControlSize;
} ClassSpecFeatureUnitDescriptor_t, *pClassSpecFeatureUnitDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bUnitID;
  uint8_t             bNrInPins;
} ClassSpecSelectorUnitDescriptor_t, *pClassSpecSelectorUnitDescriptor_t;

typedef struct
{
  uint8_t             bLength;
  uint8_t             bDescriptorType;
  uint8_t             bDescriptorSubtype;
  uint8_t             bUnitID;
  uint8_t             bNrInPins;
} ClassSpecMixUnitDescriptor_t, *pClassSpecMixrUnitDescriptor_t;

#pragma pack()

#endif //__AUDIO_DESC_H
