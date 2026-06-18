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
 *    $Revision: 7859 $
 **************************************************************************/
/** include files **/
#include <NXP/iomcimx31.h>
#include "arm_comm.h"

/** local definitions **/
#define SDRAM_BASE_ADDR 0x80000000

//flash functions return value
#define FLASH_OK            ( 0 )
#define FLASH_ERROR         ( 1 )

#define FLASH_BASE_ADDR     0xB8000000

#define NAND_MAIN_SIZE        (512)     /*The NFC reads/writes 512 bytes of the main array*/
#define NAND_SPARE_SIZE       (16)      /*The NFC reads/writes 16 bytes of the spare array*/
#define NAND_PAGE_SIZE        (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_BLK_SIZE         (131072)  /*128K(only main arrays)*/
#define NAND_PG_PER_BLK       (NAND_BLK_SIZE/NAND_MAIN_SIZE)  /**/
#define NAND_BLK_NUM          (2048)
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
void NandGetStatus(void);
unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index);
unsigned int NandCheckBlock(unsigned int Block);
unsigned int NandFindValidBlock(unsigned int Block);
/** public data **/

/** private data **/

/** public functions **/
#pragma section =".bootloader"
/*************************************************************************
 * Function Name: _Init_Clock
 * Parameters: None
 *
 * Return: None
 *
 * Description: Clock Controller Module initialization.
 *
 *************************************************************************/
void Init_Clock (void) @ ".bootloader"
{
  /*Disable MCU PLL*/
  CCM_CCMR_bit.MPE = 0;
  /*Disable Serial PLL*/
  CCM_CCMR_bit.SPE = 0;
  /*Select CKIH for PLL reference*/
  CCM_CCMR_bit.PRCS = 2;
  /*Set MCU PLL 532MHz*/
  CCM_MPCTL = 0x00192806;
  /*Set Serial PLL 292MHz*/
  CCM_SPCTL = 0x00331420;
  /*Set Clock Deviders*/
  CCM_PDR0_bit.MCU_PODF = 0; /*MCU devider 1. Clock 532MHz*/
  CCM_PDR0_bit.MAX_PODF = 3; /*MAX devider 4. Clock 133MHz*/
  CCM_PDR0_bit.IPG_PODF = 1; /*IPG devider 2. Clock 66,5MHz*/

  IPU_CONF_bit.DI_EN = 1;    /*Enable IPU Module before changing the HSP Devider*/
  CCM_PDR0_bit.HSP_PODF = 3; /*HSP devider 4. Clock 133MHz*/
  CCM_PDR0_bit.CSI_PODF = 3; /*CSI devider 4. Clock 73MHz*/
  /*Enable MCU PLL*/
  CCM_CCMR_bit.MPE = 1;
  CCM_CCMR_bit.SPE = 1;
}
/*************************************************************************
 * Function Name: _Init_SDRAM
 * Parameters: None
 *
 * Return: None
 *
 * Description: SDRAM Controller Initialization
 *
 *************************************************************************/
void Init_SDRAM (void) @ ".bootloader"
{
volatile Int32U Dummy;
  // DDR RAM initialization
  // DDR 4x8Mx32
  // Row Address = 13
  // Col Address = 9
  // ESDMISC: enable DDR mode
  ESDMISC = 0x00000004;
  // ESDCFG0: timing config
  ESDCFG0 = 0x0255B29;
  // ESDCTL0: Precharge Mode
  ESDCTL0 = 0x92100000;
  // issue Precharge All command (MA10 = 1)
  Dummy = *((volatile unsigned int *)(SDRAM_BASE_ADDR + 0x400));
  // ESDCTL0: AutoRefresh Mode
  ESDCTL0 = 0xA2100000;
  // issue Autorefresh command
  Dummy = *((volatile unsigned int *)SDRAM_BASE_ADDR);
  Dummy = *((volatile unsigned int *)SDRAM_BASE_ADDR);
  // ESDCTL0: Load Mode register Mode
  ESDCTL0 = 0xB2100000;
  // issue Load Mode Register command
  Dummy = *((volatile unsigned char *)(SDRAM_BASE_ADDR + 0x33));
  Dummy = *((volatile unsigned char *)(SDRAM_BASE_ADDR + 0x1000000));
  // ESDCTL0: Normal Mode
  // Row Address  = 13
  // Col Address  = 10
  // Data width   = 32-bit
  // Refresh Rate = 16384/64ms
  // Burst Length = 8
  // /- 2 : 32 bit mode
  // /- 1 : 16 bit - D[15:00]
  // /- 0 : 16 bit - D[31:16]
  ESDCTL0 = 0x82226080;
  // issue normal access
  *((volatile unsigned int *)SDRAM_BASE_ADDR) = 0;
  // ESDMISC: Reset Delay Line Measurement
  ESDMISC = 0x0000000C;
}

/*************************************************************************
 * Function Name: Init_NAND
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init NFC
 *
 *************************************************************************/
void Init_NAND(void) @ ".bootloader"
{
  /*Select 2K page size*/
  CCM_RCSR_bit.NFMS = 1;
  /*Reset NAND Flash*/
  CmdInOperation(0xFF);
  /*Unlock RAM Buffer*/
  NFC_IBLC = 0x2;
}

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

  /*Init Clock*/
  Init_Clock();
  /*Init SDRAM*/
  Init_SDRAM();
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

    if(0xFFFFFFFF == (Int32U)pDest) break;/*No more data*/
    /*Invalid Block marker correction*/
    if(3 == (page%4))
    {
      *((unsigned int *)(FLASH_BASE_ADDR+NAND_MAIN_SIZE-48)) &=((*((unsigned int *)(FLASH_BASE_ADDR+NAND_SPARE_OFFSET+4))>>8) | 0xFFFFFF00);
    }
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
void CmdInOperation(unsigned short cmd) @ ".bootloader"
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
void AddrInOperation(unsigned int Column, unsigned int Row) @ ".bootloader"
{
  /*Column*/
  for(int i = 0 ; 2 > i ; i++)
  {
    /*Load Column*/
    NAND_FLASH_ADD = Column & 0xFF;
    /*Start Address Operation*/
    NAND_FC2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FC2_bit.INT);
    /*Shift Column*/
     Column>>=8;
  }
  /*Row*/
  for(int i = 0 ; 3 > i ; i++)
  {
    /*Load Row*/
    NAND_FLASH_ADD = Row & 0xFF;
    /*Start Address Operation*/
    NAND_FC2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FC2_bit.INT);
    /*Shift Row*/
     Row>>=8;
  }
}

/*************************************************************************
 * Function name: DataOutOperation
 * Parameters:  PageSize - Main + Spare of Spare Only
 *              Buffer_Index - Index to NFC Buffer (0-3)
 * Return: None
 *
 * Description: Perform Data Out Operation(main+spare or spare only)
 *              from NFC Buffer
 *************************************************************************/
void DataOutOperation(t_DataOutOp PageSize, unsigned int Buff_Index) @ ".bootloader"
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
unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index) @ ".bootloader"
{
    /*Read*/
    CmdInOperation(0x00);
    /*Set Address*/
    AddrInOperation(528*(Page%4),Page/4);
    /**/
    CmdInOperation(0x30);
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
unsigned int NandCheckBlock(unsigned int Block) @ ".bootloader"
{
unsigned int Page;
  /**/
  for(int cntr = 0; 2 > cntr; cntr++)
  {
    Page = cntr*4+3; /**/
    NandReadPage(Block*NAND_PG_PER_BLK+Page,0);
    if(0xFF != (*((volatile unsigned int * )(FLASH_BASE_ADDR+NAND_MAIN_SIZE-48))&0xFF)) return FLASH_ERROR;
  }
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
