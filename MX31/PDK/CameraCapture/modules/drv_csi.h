/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_csi.h
 *    Description : CSI driver include file
 *
 *    History :
 *    1. Date        : May, 3 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "arm_comm.h"

#ifndef __DRV_CSI_H
#define __DRV_CSI_H

#define CAM_WINDOW_X        20
#define CAM_WINDOW_Y        24

#define CAM_SIZE_X          (640UL)
#define CAM_SIZE_Y          (480UL)
#define CAM_P_SIZE_X        (200UL)
#define CAM_P_SIZE_Y        (151UL)
#define CAM_BPP             (16UL)

#define CSI_SLAVE_ADDR      (0x90)

extern Int8U CamOutBuff[];

/*************************************************************************
 * Function Name: Csi_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init CSI module
 *
 *************************************************************************/
void Csi_init (void);
void OV2640_Init(void);

/*************************************************************************
 * Function Name: Csi_Ctrl
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable PrP controller
 *
 *************************************************************************/
void Csi_Ctrl(Boolean Enable);

#endif // __DRV_CSI_H
