/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_ipu.h
 *    Description : imx31 IPU driver include file
 *
 *    History :
 *    1. Date        : March 2009
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *    $Revision: 4075 $
 **************************************************************************/
#ifndef __DRV_IPU_H
#define __DRV_IPU_H
/**/
/*************************************************************************
 * Function Name: IPU_Init
 * Parameters:None
 *
 * Return:None  
 *
 * Description:Gonfigure IPU
 *
 *************************************************************************/
void IPU_Init(void);

/*************************************************************************
 * Function Name: ShowMenu
 * Parameters: None
 *
 * Return: None
 *
 * Description: Start BG tastk
 *
 *************************************************************************/
void ShowMenu(void);

/*************************************************************************
 * Function Name: HideMenu
 * Parameters: None
 *
 * Return: None
 *
 * Description: Stop BG Task
 *
 *************************************************************************/
void HideMenu(void);

/*************************************************************************
 * Function Name: ShowMenu
 * Parameters: None
 *
 * Return: None
 *
 * Description: Start FG tastk
 *
 *************************************************************************/
void ShowPicture(void);

/*************************************************************************
 * Function Name: HidePicture
 * Parameters: None
 *
 * Return: None
 *
 * Description: Stop FG Task
 *
 *************************************************************************/
void HidePicture(void);

/*************************************************************************
 * Function Name: StartImgCapturet
 * Parameters:None
 *
 * Return:None
 *
 * Description:enable DMAIC channel 1 and start VF tasks
 *
 *************************************************************************/
void StartImgCapture(void);
/*************************************************************************
 * Function Name: StopImgCapture
 * Parameters:None
 *
 * Return:None
 *
 * Description:Init Stop VF task and disable DMAIC channel 1
 *
 *************************************************************************/
void StopImgCapture(void);

#endif /*__DRV_IPU_H*/
