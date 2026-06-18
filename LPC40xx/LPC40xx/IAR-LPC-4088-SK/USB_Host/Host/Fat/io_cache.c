/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : io_cache.c
 *    Description : IO cache
 *
 *    History :
 *    1. Date        : 8, December 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <string.h>
#include "io_cache.h"
#include "usbhost_fat.h"

#define IO_CACHE_BASE       0x20002000
#define IO_CACHE_BLOCK_SIZE 512
#define IO_CACHE_BLOCKS     8

typedef struct _CacheDesc_t
{
  uint8_t *  pBuffer;
  Boolean Dirty;
  uint32_t  Sector;
  struct _CacheDesc_t * pPrev;
  struct _CacheDesc_t * pNext;
} CacheDesc_t, *pCacheDesc_t;

CacheDesc_t CacheDesc[IO_CACHE_BLOCKS];
pCacheDesc_t pHead, pTail;

/*************************************************************************
 * Function Name: IOC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize Cache module
 *
 *************************************************************************/
void IOC_Init (void)
{
uint8_t * pBuffer;
pCacheDesc_t pPrev = NULL;

  pBuffer = (uint8_t *)IO_CACHE_BASE;
  pTail   =\
  pHead   = CacheDesc;

  for(uint32_t i = 0; i < IO_CACHE_BLOCKS; i++)
  {
    pTail->pBuffer = pBuffer;
    pTail->Dirty   = FALSE;
    pTail->pPrev   = pPrev;
    pTail->Sector  = -1UL;

    pPrev = pTail;
    ++pTail;
    pPrev->pNext = pTail;
    pBuffer += IO_CACHE_BLOCK_SIZE;
  }
  pTail = pPrev;
  pTail->pNext = NULL;
}

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
int32_t IOC_Flush (void)
{
int32_t rs;

  for(pCacheDesc_t pCacheDesc = pHead;
      pCacheDesc != NULL;
      pCacheDesc = pCacheDesc->pNext)
  {
    if(pCacheDesc->Dirty)
    {
      if(0 > (rs = MS_BulkSend(pCacheDesc->Sector, 1, pCacheDesc->pBuffer)))
      {
        return(rs);
      }
      pCacheDesc->Dirty = FALSE;
    }
  }
  return(rs);
}

/*************************************************************************
 * Function Name: IOC_Read
 * Parameters: uint32_t Sector, uint32_t Numb, uint8_t * pBuffer
 *
 * Return: 0 >= - pass
 *         0 <  - IO error
 *
 * Description: Read sector
 *
 *************************************************************************/
int32_t IOC_Read (uint32_t Sector, uint32_t Numb, uint8_t * pBuffer)
{
int32_t rs = 0;
pCacheDesc_t pCacheDesc;

  if(1 < Numb)
  {
    return(MS_BulkRecv(Sector, Numb, (volatile USB_INT08U *)pBuffer));
  }

  for(pCacheDesc  = pHead;
      pCacheDesc != NULL;
      pCacheDesc  = pCacheDesc->pNext)
  {
    if(Sector == pCacheDesc->Sector)
    {
      break;
    }
  }

  if(NULL == pCacheDesc)
  {
    // cache miss
    pCacheDesc = pTail;
    if(pCacheDesc->Dirty)
    {
      if(0 > (rs = MS_BulkSend(pCacheDesc->Sector, 1, pCacheDesc->pBuffer)))
      {
        return(rs);
      }
      pCacheDesc->Dirty = FALSE;
    }
    pCacheDesc->Sector = -1UL;
    rs = MS_BulkRecv(Sector, 1, pCacheDesc->pBuffer);
  }

  if(rs >= 0)
  {
    pCacheDesc->Sector = Sector;
    memcpy(pBuffer,pCacheDesc->pBuffer,IO_CACHE_BLOCK_SIZE);
    if(NULL != pCacheDesc->pPrev)
    {
      pCacheDesc->pPrev->pNext = pCacheDesc->pNext;
      if(NULL == pCacheDesc->pNext)
      {
        pTail = pCacheDesc->pPrev;
      }
      else
      {
        pCacheDesc->pNext->pPrev = pCacheDesc->pPrev;
      }
      pCacheDesc->pPrev = pHead->pPrev;
      pHead->pPrev      = pCacheDesc;
      pCacheDesc->pNext = pHead;
      pHead             = pCacheDesc;
    }
  }
  return(rs);
}

/*************************************************************************
 * Function Name: IOC_Write
 * Parameters: uint32_t Sector, uint32_t Numb, const uint8_t * pBuffer
 *
 * Return: 0 >= - pass
 *         0 <  - IO error
 *
 * Description: Write sector
 *
 *************************************************************************/
int32_t IOC_Write (uint32_t Sector, uint32_t Numb, const uint8_t * pBuffer)
{
int32_t rs = 0;
pCacheDesc_t pCacheDesc;

  if(1 < Numb)
  {
    return(MS_BulkSend(Sector, Numb, (volatile USB_INT08U *)pBuffer));
  }


  for(pCacheDesc  = pHead;
      pCacheDesc != NULL;
      pCacheDesc  = pCacheDesc->pNext)
  {
    if(Sector == pCacheDesc->Sector)
    {
      break;
    }
  }

  if(NULL == pCacheDesc)
  {
    // cache miss
    pCacheDesc = pTail;
    if(pCacheDesc->Dirty)
    {
      rs = MS_BulkSend(pCacheDesc->Sector, 1, pCacheDesc->pBuffer);
    }
  }

  if(rs >= 0)
  {
    pCacheDesc->Sector = Sector;
    pCacheDesc->Dirty  = TRUE;
    memcpy(pCacheDesc->pBuffer,pBuffer,IO_CACHE_BLOCK_SIZE);
    if(NULL != pCacheDesc->pPrev)
    {
      pCacheDesc->pPrev->pNext = pCacheDesc->pNext;
      if(NULL == pCacheDesc->pNext)
      {
        pTail = pCacheDesc->pPrev;
      }
      else
      {
        pCacheDesc->pNext->pPrev = pCacheDesc->pPrev;
      }
      pCacheDesc->pPrev = pHead->pPrev;
      pHead->pPrev      = pCacheDesc;
      pCacheDesc->pNext = pHead;
      pHead             = pCacheDesc;
    }
  }
  return(rs);
}
