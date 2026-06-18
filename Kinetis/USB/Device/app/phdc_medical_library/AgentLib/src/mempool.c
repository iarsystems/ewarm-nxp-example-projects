/**************************************************************************
*
* Copyright 2005-2009 by Andrey Butok. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************//*!
*
* @file mempool.c
*
* @author
*
* @version 0.1.2.0
*
* @date Apr-16-2009
*
* @brief Memory pool functions.
*
******************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <mempool.h>
#include <stdlib.h>
/******************************************************************************
 * Local Types - None
 *****************************************************************************/
/*****************************************************************************
 * Local Variables - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes - None
 *****************************************************************************/

 /*****************************************************************************
 * Local Functions - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  mempool_free
 *
 * @brief This funtion releases Buffer allocated from Heap
 *
 * @param ap		:	Buffer Pointer
 *
 * @return None
 ******************************************************************************
 * This funtion releases Buffer allocated from Heap
 *****************************************************************************/
void mempool_free( 
	void *ap				/* [IN] Buffer Pointer */
)
{
    free(ap);
}

/**************************************************************************//*!
 *
 * @name  mempool_malloc
 *
 * @brief This funtion allocates Buffer from Heap
 *
 * @param mpool		:	Memory Pool Descriptor
 * @param nbytes	:	Buffer Size in bytes
 *
 * @return Pointer to allocated buffer otherwise NULL
 ******************************************************************************
 * This funtion allocates Buffer from Heap
 *****************************************************************************/
void *mempool_malloc(
	uint_32 nbytes			/* [IN] Buffer Size */
)
{
    return malloc((size_t)nbytes);  
}
