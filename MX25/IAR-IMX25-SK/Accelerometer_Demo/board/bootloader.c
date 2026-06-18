/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : bootloader.c
 *    Description : bootloader for iMX25 PDK
 *
 *
 *    History :
 *    1. Date        : 24.10.2008
 *       Author      : Stoyan Choynev
 *       Description : initial vestion
 *    2. Date        : 19.10.2009
 *       Author      : Stanimir Bonev
 *       Description : adapt for i.MX25
 *
 *    $Revision: 16 $
 **************************************************************************/
/** include files **/
#include "board.h"

/** local definitions **/

//flash functions return value
#define FLASH_OK             ( 0 )
#define FLASH_ERROR          ( 1 )

#define FLASH_BASE_ADDR       0xBB000000
#define NAND_SPARE_OFFSET3    0x10C0

#define NAND_MAIN_SIZE        (2048)    /*The NFC reads/writes 512 bytes of the main array*/
#define NAND_SPARE_SIZE       (16)      /*The NFC reads/writes 16 bytes of the spare array*/
#define NAND_PG_PER_BLK       (64)
#define NAND_BLK_NUM          (8192)

#define NAND_PAGE_SIZE        (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_BLK_SIZE         (NAND_MAIN_SIZE*NAND_PG_PER_BLK)  /* (only main arrays)*/
#define NAND_PG_NUM           (NAND_BLK_NUM  *NAND_PG_PER_BLK)

#define NAND_ID               *((volatile unsigned int *)FLASH_BASE_ADDR)
#define NAND_STATUS           *((volatile unsigned short *)FLASH_BASE_ADDR)

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
	// configure peripheral enables for CGCR0
  CGCR0 = 0x1FFFFFFF;
	// configure peripheral enables for CGCR1
  CGCR1 = 0xFFFFFFFF;
	// configure peripheral enables for CGCR2
  CGCR2 = 0x000FDFFF;
  // Set Peripheral Clock Divider Registers
  PCDR0 = 0x23C83403;
  PCDR1 = 0x03030303;
  PCDR2 = 0x01010103;
  PCDR3 = 0x01010101;

  // AIPS
  //*(unsigned long *)0x43f00000 = 0x77777777;
  //*(unsigned long *)0x43f00004 = 0x77777777;
  //*(unsigned long *)0x53f00000 = 0x77777777;
  //*(unsigned long *)0x53f00004 = 0x77777777;

  // MUX
  // init Slave General Purpose Control Register
  SGPCR0 = SGPCR1 = SGPCR2 = SGPCR3 = SGPCR4 = 0x00000010;
  // init Master Priority Register
  MPR0 = MPR1 = MPR2 =  MPR3 = MPR4 = 0x00043210;
  // init Master General Purpose Control Register
  MGPCR0 = MGPCR1 = MGPCR2 = MGPCR3 = MGPCR4 = 0;

  // Set maximum clock divistion
  CCTL_bit.ARMSRC     = 0x0;
  CCTL_bit.CGCTL      = 0x0;
  CCTL_bit.MPLLBYPASS = 0x0;
  // FCLK = 266Hz, HCLK = 133 MHz
  CCTL_bit.ARMCLKDIV = 0x1;
  CCTL_bit.AHBCLKDIV = 0x1;

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
  // Init SDRAM 0x80000000 - 0x87FFFFFF
  // SDRAM 4x16Mx16 x 1
  // Row Address = 13
  // Col Address = 9
  // ESDMISC: enable SDR mode
  ESDMISC = 0x00000002;
  ESDMISC = 0x00000000;
  // ESDCFG0: timing config
  ESDCFG0 = 0x006AC73A;
  // ESDCTL0: Precharge Mode
  ESDCTL0 = 0x92116080;
  // issue Precharge All command
  *(volatile unsigned int *)0x80000400 = 0;
  // ESDCTL0: AutoRefresh Mode
  ESDCTL0 = 0xA2116080;
  // issue Autorefresh command
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  *(volatile unsigned int *)0x80000000 = 0;
  // ESDCTL0: Load Mode register Mode
  ESDCTL0 = 0xB2116080;
  // issue Load Mode Register command
  *(volatile unsigned char *)0x80000033= 0;
  // ESDCTL0: Normal Mode
  // Row Address  = 13
  // Col Address  = 9
  // Data width   = 16-bit
  // Refresh Rate = 8192/64ms
  // Burst Length = 8
  // - 2 : 16 bit mode
  //     : 16 bit - D[15:00]
  ESDCTL0 = 0x82116080;
  // issue normal access
  *(volatile unsigned int *)0x80000000 = 0;
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
  // 16 bytes spare area
  NFC_CONFIGURATION = 2;  // Unlock
  UNLOCK_START_BLK_ADD = 0;
  UNLOCK_END_BLK_ADD = 0xFFFF;
  NF_WR_PROT = 4;
  NFC_SPAS = 0x20;
  NAND_FLASH_CONFIG1  =  (1UL << 0) | (1UL << 1) | (1UL << 3) | (1UL << 6) | (1UL << 9) | (1UL << 11);
  while(NAND_FLASH_CONFIG1_bit.NFC_RST);
  /*Reset NAND Flash*/
  CmdInOperation(0xFF);
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
    pDest = *((unsigned int* *)(FLASH_BASE_ADDR+NAND_SPARE_OFFSET3));

    if(0xFFFFFFFF == (Int32U)pDest) break;/*No more data*/
    /*Invalid Block marker correction*/
    *((unsigned int *)(FLASH_BASE_ADDR+NAND_MAIN_SIZE-48)) &=((*((unsigned int *)(FLASH_BASE_ADDR+NAND_SPARE_OFFSET3+4))>>8) | 0xFFFFFF00);
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
  NAND_FLASH_CONFIG2 = PERFORM_CMD;
  /*Wait operation to complete*/
  while(!NAND_FLASH_CONFIG2_bit.INT);

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
    NAND_FLASH_CONFIG2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FLASH_CONFIG2_bit.INT);
    /*Shift Column*/
    Column>>=8;
  }
  /*Row*/
  for(int i = 0 ; 3 > i ; i++)
  {
    /*Load Row*/
    NAND_FLASH_ADD = Row & 0xFF;
    /*Start Address Operation*/
    NAND_FLASH_CONFIG2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FLASH_CONFIG2_bit.INT);
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
  NAND_FLASH_CONFIG1_bit.ECC_EN = 1;
  /*Set Page Size*/
  if(MAIN_SPARE == PageSize)
  {
    NAND_FLASH_CONFIG1_bit.SP_EN = 0; /*Main + Spare arry*/
  }
  else
  {
    NAND_FLASH_CONFIG1_bit.SP_EN = 1; /*Spare arry only*/
  }
  /*Index to the Data Buffer*/
  RAM_BUFFER_ADDRESS = Buff_Index;
  /*Start Data Out Operation*/
  NAND_FLASH_CONFIG2 = PERFORM_DATAOUT;
  /*Wait operation to complete*/
  while(!NAND_FLASH_CONFIG2_bit.INT);

}

/*************************************************************************
 * Function Name: NandReadPage
 * Parameters: Page - NAND Flash Page Number. In is Block*NAND_PG_PER_BLK + Page_in_Block
 *             Buffer_Index - Index to NFC Buffer (0-3)
 * Return: FLASH_OK - No Read Error or 1bit Error
 *         FLASH_ERROR - Non Correcable Read Error
 * Description: Reads one page (512 main+16 spera)x4 from NAND Flash into RBA
 *
 *************************************************************************/
unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index) @ ".bootloader"
{
    /*Read*/
    CmdInOperation(0x00);
    /*Set Address*/
    AddrInOperation(0,Page);
    /**/
    CmdInOperation(0x30);
    /**/
    DataOutOperation(MAIN_SPARE,Buff_Index);
    /*Non Correctable error*/
    if(   0xF == ECC_STATUS_RESULT1_bit.NOSER1
       || 0xF == ECC_STATUS_RESULT1_bit.NOSER2
       || 0xF == ECC_STATUS_RESULT1_bit.NOSER3
       || 0xF == ECC_STATUS_RESULT1_bit.NOSER4) return FLASH_ERROR;

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
    Page = cntr; /**/
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
