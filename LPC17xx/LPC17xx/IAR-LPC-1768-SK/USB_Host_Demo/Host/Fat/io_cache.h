/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : io_cache.h
 *    Description : IO cache include file
 *
 *    History :
 *    1. Date        : 8, December 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "arm_comm.h"
#include "usbhost_fat.h"

#ifndef __IO_CACHE_H
#define __IO_CACHE_H

/*************************************************************************
 * Function Name: IOC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize Cache module
 *
 *************************************************************************/
void IOC_Init (void);

/*************************************************************************
 * Function Name: IOC_Flush
 * Parameters: none
 *
 * Return: 0 >= - pass
 *         0 <  - IO error
 *
 * Description: Flush dirty block from caches' buffers
 *
 *************************************************************************/
Int32S IOC_Flush (void);

/*************************************************************************
 * Function Name: IOC_Read
 * Parameters: Int32U Sector, Int32U Numb, Int8U * pBuffer
 *
 * Return: 0 >= - pass
 *         0 <  - IO error
 *
 * Description: Read sector
 *
 *************************************************************************/
Int32S IOC_Read (Int32U Sector, Int32U Numb, Int8U * pBuffer);

/*************************************************************************
 * Function Name: IOC_Write
 * Parameters: Int32U Sector, Int32U Numb, const Int8U * pBuffer
 *
 * Return: 0 >= - pass
 *         0 <  - IO error
 *
 * Description: Write sector
 *
 *************************************************************************/
Int32S IOC_Write (Int32U Sector, Int32U Numb, const Int8U * pBuffer);

#endif // __IO_CACHE_H

