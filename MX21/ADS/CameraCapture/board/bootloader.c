/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : bootloader.c
 *    Description : bootloader for iMX31 PDK
 *
 *
 *    History :
 *    1. Date        : 24.10.2008
 *       Author      : Stoyan Choynev
 *       Description : initial vestion
 *
 *    $Revision: 5880 $
 **************************************************************************/
/** include files **/
#include <NXP/iomc9328mx21.h>
#include "arm_comm.h"

/** local definitions **/
//flash functions return value
#define FLASH_OK            ( 0 )
#define FLASH_ERROR         ( 1 )

#define FLASH_BASE_ADDR     0xDF003000

#define NAND_MAIN_SIZE        (512)     /*The NFC reads/writes 512 bytes of the main array*/
#define NAND_SPARE_SIZE       (16)      /*The NFC reads/writes 16 bytes of the spare array*/
#define NAND_PAGE_SIZE        (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_BLK_SIZE         (16384)  /*128K(only main arrays)*/
#define NAND_PG_PER_BLK       (NAND_BLK_SIZE/NAND_MAIN_SIZE)  /**/
#define NAND_BLK_NUM          (4096)
#define NAND_PG_NUM           (NAND_BLK_NUM*NAND_PG_PER_BLK)

#define NAND_ID               *((volatile unsigned int *)FLASH_BASE_ADDR)
#define NAND_STATUS           *((volatile unsigned short *)FLASH_BASE_ADDR)

#define NAND_SPARE_OFFSET     0x800     /**/

typedef enum
{
  MAIN_SPARE,
  SPARE
} t_DataOutOp ;

#define PERFORM_CMD     0x1
#define PERFORM_ADDR    0x2
#define PERFORM_DATAIN  0x4
#define PERFORM_DATAOUT 0x8
#define PERFORM_IDOUT   0x10
#define PERFORM_STATUS  0x20

extern __arm void __vector(void);

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
void CmdInOperation(unsigned short cmd);
void AddrInOperation(unsigned int Column, unsigned int Row);
void DataOutOperation(t_DataOutOp PageSize, unsigned int Buff_Index);
//void NandGetStatus(void);
unsigned int NandReadSpare(unsigned int Page,unsigned int Buff_Index);
unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index);
unsigned int NandCheckBlock(unsigned int Block);
unsigned int NandFindValidBlock(unsigned int Block);
/** public data **/

/** private data **/

/** public functions **/
#pragma section =".bootloader"
/*************************************************************************
 * Function Name: Init_NAND
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init NFC
 *
 *************************************************************************/
__arm void Init_NAND(void) @ ".bootloader"
{
  PCDR0_bit.NFCDIV =15;
  PCCR0_bit.NFC_EN = 1;
  FMCR_bit.NF_FMS = 0;
  /* 8-bit NAND Flash*/
  FMCR_bit.NF_16BIT_SEL = 0;
  /*Reset NAND Flash*/
  //CmdInOperation(0xFF);
  /*Unlock RAM Buffer*/
  NFC_IBLC = 0x2;
}
extern __arm void low_level_init (void);

/*************************************************************************
 * Function Name: bootload
 * Parameters: none
 *
 * Return: none
 *
 * Description: bootloader for iMX31 PDK. Inits Clock and SDRAM controller
 *              Copies a program from NAND flash to SDRAM
 *
 *************************************************************************/
void bootload(void) @ ".bootloader"
{
Int32U * pSrc;
Int32U * pDest;
Int32U block,page;

  low_level_init();
  /*Init NFC*/
  Init_NAND();
  /*Start from Block 1*/
  block = NandFindValidBlock(1);
  page = 0;  /*Start from page 1 of the block*/

  while((NAND_BLK_NUM) > (block))
  {
    if(FLASH_ERROR == NandReadPage(block*NAND_PG_PER_BLK + page,0)) while(1); /*Read Error*/
    /*Source address =  NFC RAM buffer address*/
    pSrc = (unsigned int *)(FLASH_BASE_ADDR);
    /*Dest address = Last Word of the Spare Array*/
    pDest = *((unsigned int* *)(FLASH_BASE_ADDR+NAND_SPARE_OFFSET+12));

    if(0xFFFFFFFF == (Int32U)pDest)
        break;/*No more data*/
    /*Copy Data*/
    for(Int32U cntr = 0;NAND_MAIN_SIZE > cntr; cntr+=sizeof(Int32U))
    {
      *pDest++ = *pSrc++;
    }

    if(NAND_PG_PER_BLK <= ++page)
    {/*It was last page of the block*/
      page = 0;
      block = NandFindValidBlock(block+1);
    }
  }
  /*Jump To pogram Start*/
  __vector();
}
/** private functions **/
/*************************************************************************
 * Function name: CmdInOperation
 * Parameters:  cmd - the Command to send
 *
 * Return: None
 *
 * Description: Perform  Command Operaion
 *
 *************************************************************************/
void CmdInOperation(unsigned short cmd)
{
  /*Load the Command*/
  NAND_FLASH_CMD = cmd;
  /*Start Command Operation*/
  NAND_FC2 = PERFORM_CMD;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);

}

/*************************************************************************
 * Function name: AddrInOperation
 * Parameters:  Column - NAND Colum Address
 *              Row    - NAND Row Address
 * Return: None
 *
 * Description: Perform Address Operation
 *
 *************************************************************************/
void AddrInOperation(unsigned int Column, unsigned int Row)
{
  /*Load Column*/
  NAND_FLASH_ADD = Column & 0xFF;
  /*Start Address Operation*/
  NAND_FC2 = PERFORM_ADDR;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);

  /*Row*/
  for(int i = 0 ; 3 > i ; i++)
  {
    /*Load Row*/
    NAND_FLASH_ADD = Row & 0xFF;
    /*Start Address Operation*/
    NAND_FC2 = PERFORM_ADDR;
    /*Shift Row*/
    Row>>=8;
    /*Wait operation to complete*/
    while(!NAND_FC2_bit.INT);
  }
}

/*************************************************************************
 * Function name: DataOutOperation
 * Parameters:  PageSize - Main + Spare or Spare Only
 *              Buffer_Index - Index to NFC Buffer (0-3)
 * Return: None
 *
 * Description: Perform Data Out Operation(main+spare or spare only)
 *              from NFC Buffer
 *************************************************************************/
void DataOutOperation(t_DataOutOp PageSize, unsigned int Buff_Index)
{
  /*Enable ECC*/
  NAND_FC1_bit.ECC_EN = 1;
  /*Set Page Size*/
  if(MAIN_SPARE == PageSize)
  {
    NAND_FC1_bit.SP_EN = 0; /*Main + Spare arry*/
  }
  else
  {
    NAND_FC1_bit.SP_EN = 1; /*Spare arry only*/
  }
  /*Index to the Data Buffer*/
  NFC_RBA = Buff_Index;
  /*Start Data Out Operation*/
  NAND_FC2 = PERFORM_DATAOUT;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);
}

/*************************************************************************
 * Function Name: NandReadPage
 * Parameters: Page - NAND Flash Page Number. In is Block*NAND_PG_PER_BLK + Page_in_Block
 *             Buffer_Index - Index to NFC Buffer (0-3)
 * Return: FLASH_OK - No Read Error or 1bit Error
 *         FLASH_ERROR - Non Correcable Read Error
 * Description: Reads one page (512 main+16 spera) from NAND Flash into RBA
 *
 *************************************************************************/
unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index)
{
    /*Read*/
    CmdInOperation(0x00);
    /*Set Address*/
    AddrInOperation(0,Page);
    /**/
    DataOutOperation(MAIN_SPARE,Buff_Index);
    /*Non Correctable error*/
    if((2 == ECC_SRR_bit.ERM) || (2 == ECC_SRR_bit.ERS)) return FLASH_ERROR;

    return FLASH_OK;
}

/*************************************************************************
 * Function Name: NandCheckBlock
 * Parameters: Block - NAND Block Number.
 *
 * Return: FLASH_OK -Valid Block
 *         FLASH_ERROR - Invalid Block
 * Description: Checks The Invalid Bytes in the Block
 *
 *************************************************************************/
unsigned int NandCheckBlock(unsigned int Block)
{
unsigned int Page;
  /* For the first and second page in the Block */
  for(Page = 0; 2 > Page; Page++)
  {
    NandReadSpare(Block*NAND_PG_PER_BLK+Page,0);
    if(0xFF00 != (*((unsigned int * )(FLASH_BASE_ADDR+NAND_SPARE_OFFSET+4))&0xFF00)) return FLASH_ERROR;
  }
  return FLASH_OK;
}

/*************************************************************************
 * Function Name: NandReadSpare
 * Parameters: Page - NAND Flash Page Number. In is Block*NAND_PG_PER_BLK + Page_in_Block
 *             Buffer_Index - Index to NFC Buffer (0-3)
 * Return: FLASH_OK - No Read Error or 1bit Error
 *         FLASH_ERROR - Non Correcable Read Error
 * Description: Reads the Spare arrey of the page (16 Bytes) into RBA
 *
 *************************************************************************/
unsigned int NandReadSpare(unsigned int Page,unsigned int Buff_Index)
{
    /*Read*/
    CmdInOperation(0x50);
    /*Set Address*/
    AddrInOperation(0,Page);
    /**/
    DataOutOperation(SPARE,Buff_Index);
    /*Non Correctable error*/
    if((2 == ECC_SRR_bit.ERS)) return FLASH_ERROR;

    return FLASH_OK;
}
/*************************************************************************
 * Function Name: NandGetValidBlock
 * Parameters: Block - NAND Block Number. Start from this block
 *
 * Return: Block < NAND_BLK_NUM NAND -  Valid Block Number.
 *         NAND_BLK_NUM - No Valid Block
 * Description: Finds a vlaid Block.
 *
 *************************************************************************/
unsigned int NandFindValidBlock(unsigned int Block) @ ".bootloader"
{
  while(NAND_BLK_NUM > Block)
  {/**/
    if(FLASH_OK == NandCheckBlock(Block)) break;
    Block++;
  }
  return Block;
}
