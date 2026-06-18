/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : EMC_drv.c
 *    Description : SDRAM driver HY57V561620FTP-H 16MBx16bit
 *                  and NAND Flash k9F1208U0B  driver
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : June 2008
 *       Author      : Stoyan Choynev
 *       Description : SDRAM Driver ported HY57V561620FTP-H device
 *                     added NAND Flash Driver
 *
 *    $Revision: 4288 $
 **************************************************************************/

#include "includes.h"

#if   SYS_FREQ == (72MHZ)
#define EMC_PERIOD          13.8  // 72MHz
#elif   SYS_FREQ == (57MHZ)
#define EMC_PERIOD          17.4  // 57.6MHz
#elif SYS_FREQ == (48MHZ)
#define EMC_PERIOD          20.8  // 48MHz
#elif SYS_FREQ == (36MHZ)
#define EMC_PERIOD          27.8  // 36MHz
#elif SYS_FREQ == (24MHZ)
#define EMC_PERIOD          41.7  // 24MHz
#else
#error Frequency not defined
#endif

#define P2C(Period)           (((Period<EMC_PERIOD)?0:(Int32U)((Flo32)Period/EMC_PERIOD))+1)

/*************************************************************************
* SDRAM definitions
**************************************************************************/
#define SDRAM_BASE_ADDR *(volatile Int32U*)0xA0000000

#define SDRAM_REFRESH         7813
#define SDRAM_TRP             20
#define SDRAM_TRAS            42
#define SDRAM_TAPR            1
#define SDRAM_TDAL            2
#define SDRAM_TWR             2
#define SDRAM_TRC             63
#define SDRAM_TRFC            63
#define SDRAM_TXSR            1
#define SDRAM_TRRD            15
#define SDRAM_TMRD            2

/*************************************************************************
* NAND Flash
**************************************************************************/
#define NAND_BASE_ADDR  *(volatile Int8U*)0x81000000
#define NAND_CMD_LATCH  *(volatile Int8U*)0x81200000
#define NAND_ADDR_LATCH *(volatile Int8U*)0x81400000

#define NAND_tCSWE            0
#define NAND_tCSOW            0
#define NAND_tREA             60
#define NAND_tRC              120
#define NAND_tWP              60
#define NAND_tTURNE           150

#define NAND_W_TO_B_DLY       2       //

#define NAND_MAIN_SIZE        (512)
#define NAND_SPARE_SIZE       (16)
#define NAND_PAGE_SIZE        (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_BLK_NUM          (4096)
#define NAND_PG_PER_BLK       (32)
#define NAND_PG_NUM           (NAND_BLK_NUM*NAND_PG_PER_BLK)

#define NAND_PASS (1<<0)
#define NAND_WP   (1<<7)

#define NandChipEn()   FIO0CLR |= (1<<11);
#define NandChipDis()  FIO0SET |= (1<<11);

//Wait until NAND gets ready
void NandWaitReady(void)
{
  for(int i = 0; i < NAND_W_TO_B_DLY; i++);

  while(!(FIO0PIN & (1<<10)));
}

//Read device status
char NandGetStatus(void)
{
  NAND_CMD_LATCH = 0x70;

  NandWaitReady();

  return NAND_BASE_ADDR;
}

/*************************************************************************
 * Function Name: NandReset
 * Parameters: None
 *
 * Return: None
 *
 * Description:Reset the NAND Flash
 *
 *************************************************************************/
void NandReset(void)
{
  NandChipEn();

  NAND_CMD_LATCH = 0xFF;

  NandWaitReady();

  NandChipDis();
}

/*************************************************************************
 * Function Name: NandGetID
 * Parameters: pID - Pointer to char array. The array should be at least
 *                   4 bytes long
 *
 * Return: None
 *
 * Description:Reads the Device ID.
 *
 *************************************************************************/
void NandGetID(char * pID)
{
  NandChipEn();
  //ID Command
  NAND_CMD_LATCH = 0x90;
  //
  NAND_ADDR_LATCH = 0x00;

  NandWaitReady();

  for(int i = 0 ; i < 4; i++) *pID++ = NAND_BASE_ADDR;

  NandChipDis();
}
/*************************************************************************
 * Function Name: NandRead
 * Parameters: pDest - pointer to char buffer.
 *             PageIndex - Index to the Page
 *             Column - Column Index
 *                      from 0 to 255 Main Array 1st Half
 *                      from 256 to 511 Main Array 2nd Half
 *                      from 511 to 527 SPare Array
 *             Size - Bytes to be read
 *
 * Return: none
 *
 * Description: Reads one page from NAND Flash
 *
 *************************************************************************/
void NandRead(char * pDest,Int32U PageIndex, Int32U Column, Int32U Size)
{
  assert(NAND_PG_NUM > PageIndex);
  assert(NAND_PAGE_SIZE >= (Size+Column));

  NandChipEn();
  //Read
  //Spare array
  if(NAND_MAIN_SIZE < Column)
  {
    NAND_CMD_LATCH = 0x50;
    Column -= NAND_MAIN_SIZE;
  }
  else
  {
    //main 2nd half
    if(NAND_MAIN_SIZE/2 < Column)
    {
      NAND_CMD_LATCH = 0x01;
    }
    else  //main 1st half
    {
      NAND_CMD_LATCH = 0x00;
    }
  }
  //column address
  NAND_ADDR_LATCH = Column & 0xFF;
  //Page Address
  NAND_ADDR_LATCH = (PageIndex>>0) & 0xFF;
  NAND_ADDR_LATCH = (PageIndex>>8) & 0xFF;
  NAND_ADDR_LATCH = (PageIndex>>16)& 0x01;

  NandWaitReady();

  for(Int32U cnt=0; Size > cnt; cnt++) *pDest++ = NAND_BASE_ADDR;

  NandChipDis();
}
/*************************************************************************
 * Function Name: NandReadPage
 * Parameters: pDest - pointer to char buffer. The Buffer's size should be at least
 *                     NAND_PAGE_SIZE.
 *             Page - Index to the Page
 * Return: none
 *
 * Description: Reads one page from NAND Flash
 *
 *************************************************************************/
void NandReadPage(char * pDest,Int32U Page)
{
  assert(NAND_PG_NUM > Page);

  NandChipEn();
  //Read 1 First page half
  NAND_CMD_LATCH = 0x00;
  //column 0
  NAND_ADDR_LATCH = 0;
  //Page Address
  NAND_ADDR_LATCH = (Page>>0) & 0xFF;
  NAND_ADDR_LATCH = (Page>>8) & 0xFF;
  NAND_ADDR_LATCH = (Page>>16)& 0x01;

  NandWaitReady();

  for(Int32U cnt=0; NAND_PAGE_SIZE > cnt; cnt++) *pDest++ = NAND_BASE_ADDR;

  NandChipDis();
}

/*************************************************************************
 * Function Name: NandSeqRead
 * Parameters: pDest - pointer to char buffer. The Buffer's size should be at least
 *                     NAND_PG_SIZE * NAND_PGS_PER_BLK.
 *             PageIndex - Index to the Page
 * Return: none
 *
 * Description: Reads All pages to the end of the Block
 *
 *************************************************************************/
void NandSeqRead(char * pDest,Int32U PageIndex)
{
  assert(NAND_PG_NUM > PageIndex);

  NandChipEn();
  //Read from first page half
  NAND_CMD_LATCH = 0x00;
  //column 0
  NAND_ADDR_LATCH = 0;
  //Page Address
  NAND_ADDR_LATCH = (PageIndex>>0) & 0xFF;
  NAND_ADDR_LATCH = (PageIndex>>8) & 0xFF;
  NAND_ADDR_LATCH = (PageIndex>>16)& 0x01;

  do
  {
    NandWaitReady();

    for(Int32U cnt=0; NAND_PAGE_SIZE > cnt; cnt++) *pDest++ = NAND_BASE_ADDR;

  }while((++PageIndex) & (NAND_PG_PER_BLK-1));

  NandChipDis();
}

/*************************************************************************
 * Function Name: NandProgramPage
 * Parameters: pSrc - pointer to char buffer.
 *
 *             PageIndex - Index to the Page
 * Return: Boolean TRUE - Write O.K.
 *                 FALSE - WRITE Error
 * Description: Writes one page to NAND Flash
 *
 *************************************************************************/
Int8U NandProgramPage(char * pSrc,Int32U Page)
{
Int8U NandStatus;
  assert(NAND_PG_NUM > Page);

  NandChipEn();
  //Read
  NAND_CMD_LATCH = 0x00;
  //
  NAND_CMD_LATCH = 0x80;
  //column 0
  NAND_ADDR_LATCH = 0;
  //Page Address
  NAND_ADDR_LATCH = (Page>>0) & 0xFF;
  NAND_ADDR_LATCH = (Page>>8) & 0xFF;
  NAND_ADDR_LATCH = (Page>>16)& 0x01;

  NandWaitReady();

  for( Int32U cnt=0; NAND_PAGE_SIZE > cnt; cnt++) NAND_BASE_ADDR = *pSrc++;

  NAND_CMD_LATCH = 0x10;

  NandWaitReady();

  NandStatus = NandGetStatus();

  NandChipDis();

  return NandStatus;
}


/*************************************************************************
 * Function Name: NandEraseBlock
 * Parameters: Block.
 *
 *
 * Return: Boolean TRUE - Erase O.K.
 *                 FALSE - Erase Error
 * Description: Erase a block.
 *
 *************************************************************************/
Int8U NandEraseBlock(Int32U Block)
{
Int32U Page = Block*32;
Int8U NandStatus;

  assert(NAND_BLK_NUM > Block);

  NandChipEn();
  //
  NAND_CMD_LATCH = 0x60;
  //
  NAND_ADDR_LATCH = (Page>>0) & 0xFF;
  NAND_ADDR_LATCH = (Page>>8) & 0xFF;
  NAND_ADDR_LATCH = (Page>>16)& 0x01;

  NAND_CMD_LATCH = 0xD0;

  NandWaitReady();

  NandStatus = NandGetStatus();

  NandChipDis();

  return NandStatus;
}

Boolean NAND_Test(void);


Boolean NandCheckBlock(Int32U Block)
{
Int32U Page;
char Byte;
  //For the first and second page in the Block
  for(Page = 0; 2 > Page; Page++)
  {
    //the 6th Byte in the Spare array shoud be 0xFF
    NandRead(&Byte,Block*NAND_PG_PER_BLK+Page,517,1);
    if(0xFF != Byte) return (FALSE);
  }
  return (TRUE);
}
/*************************************************************************
 * Function Name: SDRAM_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: SDRAM controller and memory init
 *
 *************************************************************************/
void EMC_Init (void)
{
  // Assign pins to EMC controller(SDRAM & NAND Flash)
  PINSEL0 &= BIN32(11111111,00001111,11111111,11111111);
  PINSEL5 &= BIN32(11110000,11111100,11111100,11000000);
  PINSEL5 |= BIN32(00000101,00000001,00000001,00010101);
  PINMODE5&= BIN32(11110000,11111100,11111100,11000000);
  PINMODE5|= BIN32(00001010,00000010,00000010,00101010);
  PINSEL6  = BIN32(01010101,01010101,01010101,01010101);
  PINMODE6 = BIN32(10101010,10101010,10101010,10101010);
  PINSEL8 &= BIN32(11000000,00000000,00000000,00000000);
  PINSEL8 |= BIN32(00010101,01010101,01010101,01010101);
  PINMODE8&= BIN32(11000000,00000000,00000000,00000000);
  PINMODE8|= BIN32(00101010,10101010,10101010,10101010);
  PINSEL9 &= BIN32(00111111,11110000,11000011,11111111);
  PINSEL9 |= BIN32(01000000,00000101,00010100,00000000);
  PINMODE9&= BIN32(00111111,11110000,11000011,11111111);
  PINMODE9|= BIN32(10000000,00001010,00101000,00000000);

  //
  FIO0DIR |= (1<<11);
  NandChipDis();

  // Init SDRAM controller
  // Enable EMC clock
  PCONP_bit.PCEMC = 1;
  EMCCONTROL      = 1;         // enable EMC

  EMCDINAMICRDCFG = 1;
  EMCDYNAMICRASCAS0_bit.CAS = 3;
  EMCDYNAMICRASCAS0_bit.RAS = 3;
  EMCDYNAMICRP = P2C(SDRAM_TRP);
  EMCDYNAMICRAS = P2C(SDRAM_TRAS);
  EMCDYNAMICSREX = SDRAM_TXSR;
  EMCDYNAMICAPR = SDRAM_TAPR;
  EMCDYNAMICDAL = SDRAM_TDAL+P2C(SDRAM_TRP);
  EMCDYNAMICWR = SDRAM_TWR;
  EMCDYNAMICRC = P2C(SDRAM_TRC);
  EMCDYNAMICRFC = P2C(SDRAM_TRFC);
  EMCDYNAMICXSR = SDRAM_TXSR;
  EMCDYNAMICRRD = P2C(SDRAM_TRRD);
  EMCDYNAMICMRD = SDRAM_TMRD;
  EMCDYNAMICCFG0 = 0x0000680;        // 13 row, 9 - col, SDRAM
  // JEDEC General SDRAM Initialization Sequence
  // DELAY to allow power and clocks to stabilize ~100 us
  // NOP
  EMCDINAMICCTRL = 0x0183;
  for(volatile Int32U i = 200*30; i;i--);
  // PALL
  EMCDINAMICCTRL_bit.I = 2;
  EMCDINAMICRFR = 1;
  for(volatile Int32U i= 128; i; --i); // > 128 clk
  EMCDINAMICRFR = P2C(SDRAM_REFRESH) >> 4;
  // COMM
  EMCDINAMICCTRL_bit.I = 1;
  // Burst 8, Sequential, CAS-2
  volatile unsigned long Dummy = *(volatile unsigned short *)
                                  ((Int32U)&SDRAM_BASE_ADDR + (0x33UL << (12)));
  // NORM
  EMCDINAMICCTRL = 0x0000;
  EMCDYNAMICCFG0_bit.B = 1;
  for(volatile Int32U i = 10000; i;i--);
  //8Bit Bus
  EMCSTATICCNFG1_bit.MW = 0;
  //Disable Page mode
  EMCSTATICCNFG1_bit.PM = 0;
  //Chip select active low
  EMCSTATICCNFG1_bit.PC = 0;
  //No Extended wait
  EMCSTATICCNFG1_bit.EW = 0;
  //Enable byte line state bit
  EMCSTATICCNFG1_bit.PB = 1;
  EMCSTATICCNFG1_bit.B = 0;
  EMCSTATICCNFG1_bit.P = 0;
  //Set delays
  EMCSTATICWAITWEN1 = NAND_tCSWE;
  EMCSTATICWAITOEN1 = NAND_tCSOW;
  EMCSTATICWAITRD1 = P2C(NAND_tREA);
  EMCSTATICWAITPG1 = 0;
  EMCSTATICWAITWR1 = P2C(NAND_tWP);
  //Reset
  NandReset();
}

Boolean SDRAM_Test (void)
{
  printf("SDRAM Test Start\n");
  // 32 bits access
  for (Int32U i = 0; i < 0x2000000; i+=sizeof(Int32U))
  {
    *(Int32U*)((Int32U)&SDRAM_BASE_ADDR+i) = i;
  }

  for (Int32U i = 0; i < 0x2000000; i+=sizeof(Int32U))
  {
    if (*(Int32U*)((Int32U)&SDRAM_BASE_ADDR+i) != i)
    {
      printf("SDRAM Verification error on address : 0x%x\n",(Int32U)&SDRAM_BASE_ADDR+i);
      return(FALSE);
    }
  }

  // 16 bits access
  for (Int32U i = 0; i < 0x10000; i+=sizeof(Int16U))
  {
    *(Int16U*)((Int32U)&SDRAM_BASE_ADDR+i) = i;
  }

  for (Int32U i = 0; i < 0x10000; i+=sizeof(Int16U))
  {
    if (*(Int16U*)((Int32U)&SDRAM_BASE_ADDR+i) != i)
    {
      printf("SDRAM Verification error on address : 0x%x\n",(Int32U)&SDRAM_BASE_ADDR+i);
      return(FALSE);
    }
  }

  // 8 bits access
  for (Int32U i = 0; i < 0x100; i+=sizeof(Int8U))
  {
    *(Int8U*)((Int32U)&SDRAM_BASE_ADDR+i) = i;
  }

  for (Int32U i = 0; i < 0x100; i+=sizeof(Int8U))
  {
    if (*(Int8U*)((Int32U)&SDRAM_BASE_ADDR+i) != i)
    {
      printf("SDRAM Verification error on address : 0x%x\n",(Int32U)&SDRAM_BASE_ADDR+i);
      return(FALSE);
    }
  }
  printf("SDRAM Test Pass\n");
  return(TRUE);
}

Boolean NAND_Test(void)
{
Int8U OperationStatus;
Int32U Block;
char * pBuffer;
  //Get Buffer for NAND Read/Write
  pBuffer = malloc(NAND_PG_PER_BLK*NAND_PAGE_SIZE);

  assert(pBuffer);

  NandGetID(pBuffer);

  if((0xEC != pBuffer[0]) || ((0x76 != pBuffer[1]) && (0x36 != pBuffer[1])))
  {
    printf("NAND Flash not supported or not present\n");
    free(pBuffer);
    return (FALSE);
  }

  printf("NAND Test Start\n");

  for(Block = 0; NAND_BLK_NUM > Block; Block++)
  {
Boolean Error = FALSE;
    //Check for invalid Block
    if((FALSE) == NandCheckBlock(Block))
    {//Do not erase invalid Block
      printf("NAND Invalid Block: %u\n",Block);
      continue;
    }

    OperationStatus = NandEraseBlock(Block);
    
    if(!(NAND_WP & OperationStatus))
    {
      printf("NAND is Write Protected\n");
      free(pBuffer);
      return(FALSE);
    }
    
    if(NAND_PASS & OperationStatus)
    {
      printf("NAND Erase Failed Block: %u\n",Block);
      continue;
    }

    for(Int32U Page = 0; NAND_PG_PER_BLK > Page; Page++)
    {
      for(Int32U i = 0; NAND_PAGE_SIZE > i; i++)
      {
        pBuffer[i] = ((Block^Page) + i) & 0xFF;
        //Valid Block Mark
        if(((0 == Page) || (1 == Page)) && 517 == i )
        pBuffer[i] = 0xFF;
      }

      OperationStatus = NandProgramPage(pBuffer, Block*NAND_PG_PER_BLK + Page);

      if(!(NAND_WP & OperationStatus))
      {
        printf("NAND is Write Protected\n");
        free(pBuffer);
        return(FALSE);
      }
    
      if(NAND_PASS & OperationStatus)
      {
        printf("NAND Program Error Page: %u Block: %u\n", Page, Block);
        Error= TRUE;
        break;
      }
    }

    if((FALSE) == Error)
    {//Read whole Block
      NandSeqRead(pBuffer,Block*NAND_PG_PER_BLK);

      for(Int32U Page = 0; NAND_PG_PER_BLK > Page; Page++)
      {
        for(Int32U i = 0; NAND_PAGE_SIZE > i; i++)
        {
        char Test = ((Block^Page) + i) & 0xFF;
          if(((0 == Page) || (1 == Page)) && 517 == i ) Test = 0xFF;

          if( pBuffer[Page*NAND_PAGE_SIZE+i] != Test)
          {
            printf("NAND Read Error Page %u Block %u\n", Page, Block);
            Error = TRUE;
            break;
          }
        }
      }
    }

    if((FALSE) == Error)
    {
      printf("NAND Block: %u O.K.\n", Block);
    }
  }

  printf("NAND Test End\n");
  free(pBuffer);
  return (TRUE);
}
