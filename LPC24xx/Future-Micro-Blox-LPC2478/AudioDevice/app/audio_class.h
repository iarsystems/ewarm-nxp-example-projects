/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : audio_class.h
 *    Description : AUDIO CLASS definitions
 *
 *    History :
 *    1. Date        : November 29, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#ifndef __AUDIO_CLASS_H
#define __AUDIO_CLASS_H

#ifdef AUDIO_CLASS_GLOBAL
#define AUDIO_CLASS_EXTERN
#else
#define AUDIO_CLASS_EXTERN  extern
#endif

#define MinVol        0x8000

#define Feat1MinVol   0xC400UL
#define Feat1MaxVol   0x0000UL
#define Feat1ResVol   0x0001UL

typedef enum
{
  FeatUnit1Id = 1,
  SpkInTermID, SpkOutTermID,
  SelUnit1ID,
} AudioID_t;

extern Int32U SYS_GetFpclk(Int32U Periphery);

/*************************************************************************
 * Function Name: UsbAudioClassInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB Class Audio Init
 *
 *************************************************************************/
void UsbAudioClassInit (void);

/*************************************************************************
 * Function Name: UsbClassAudioConfigure
 * Parameters:  pUsbDevCtrl_t pDev
 *
 * Return: none
 *
 * Description: USB Class Audio configure
 *
 *************************************************************************/
void UsbClassAudioConfigure (pUsbDevCtrl_t pDev);

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
void AudioInHadler (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: AudioFeatureGetReg
 * Parameters:  Int32U CS, Int32U Id
 *
 * Return: Boolean
 *
 * Description:
 *
 *************************************************************************/
static Boolean AudioFeatureGetReg (Int32U CS, Int32U Id);

/*************************************************************************
 * Function Name: UsbClassAudioRequest
 * Parameters:  pUsbSetupPacket_t pSetup
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Class Audio Requests
 *
 *************************************************************************/
UsbCommStatus_t UsbClassAudioRequest (pUsbSetupPacket_t pSetup);

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
void UsbClassAudioData (USB_Endpoint_t EP);

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
void UsbClassAudioVolCtrl (Int8U Db);

#endif //__AUDIO_CLASS_H
