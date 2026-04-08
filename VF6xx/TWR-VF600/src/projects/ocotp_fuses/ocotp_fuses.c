////////////////////////////////////////////////////////////////////////////////
//
// Filename: ocotp_fuses.c
//
// Description: Program to blow fuses on Vybrid.
//
////////////////////////////////////////////////////////////////////////////////

#include "ocotp_fuses.h"
#include "lpt.h"



int main()
{
  fusedata_t fuse[] = {
    0, 5, 0, 0x62,	/* Bank 0, Word 5, Byte 0: sd card, normal mode */
    0, 5, 1, 0x28,	/* Bank 0, Word 5, Byte 1: 4-bit, esdhc 1 */
    0, 6, 0, 0x10,  /* Bank 0, Word 6, Byte 0: Enable BT_FUSE_SEL bit to use fuses for boot */
    0, 0, 0, 0      /* End on value of 0 */
  };
  char proceed, bank_num=0, word_num=0, byte_pos=0, fuse_val=0;
  int i;

  printf("Vybrid Fuse Blowing\n");

  /* Fuse location and descriptions are in Chapter 36 in the
   * OCOTP_CFGx registers
   *
   * Fuses are 0 by default. Once blown to '1', the change is permanent.
   *  Be careful to ensure you are blowing the correct fuses. Ensure
   *  other fuses have not already been blown that would interfere with fuses
   *  you want to blow.
   *
   * The code below will set the fuses to boot from SD Card
   *
   * The shadow registers where fuse state can be viewed starts at 0x400a5400
   */


  //Enable OCOTP clock
  CCM->CCGR6 |= CCM_CCGR6_CG5(1); // OCOTP ungate

  OCOTP->TIMING=0x00461355; //Set up timing parameters for OCOTP.


  printf("The following fuses will be blown:\n");
  for(i=0;fuse[i].value!=0;i++)
  {
    printf("  bank %d word %d bytepos %d value 0x%x\n",
	  fuse[i].bankno, fuse[i].word,
          fuse[i].bytepos, fuse[i].value);
  }
  printf("\nTHIS IS A PERMANENT CHANGE\n");
  printf("Ensure no other fuses have been blown already that would interfere with fuses you want to blow\n");
  printf("Some boards were shipped out with SD card boot fuses blown already\n");
  printf("Current value of BOOT_CFG fuse registers is: %08X\n",OCOTP->CFG4);
  printf("Enter 'y' to proceed, any other key to exit\n");

  proceed=uart_getchar();
  if(proceed!='y')
  {
    printf("Leaving program\n");
    return 0;
  }

  printf("Will blow fuses\n");
  for(i=0;fuse[i].value!=0;i++)
  {
    printf("bank %d word %d bytepos %d value 0x%x\n",
	  fuse[i].bankno, fuse[i].word,
          fuse[i].bytepos, fuse[i].value);

    bank_num = fuse[i].bankno;
    word_num = fuse[i].word;
    byte_pos = fuse[i].bytepos;
    fuse_val = fuse[i].value;

    program_fuse_word((bank_num*8+word_num),(unsigned int)fuse_val<<(byte_pos*8));
    wait_for_fuse();
    read_fuse_word(bank_num*8+word_num);
    wait_for_fuse();

    //Reload cache
    OCOTP->CTRL|=OCOTP_CTRL_RELOAD_SHADOWS_MASK;
    //wait for fuse to return done
    wait_for_fuse();
    printf("Fuses blown\n\n");
  }

  printf("Finished blowing fuses\n");
  return 0;
}

void program_fuse_word(int address,int data_temp)
{
  //configure fuse word address with unlock key
  OCOTP->CTRL=(OCOTP_WR_UNLOCK_KEY_VALUE | (address & OCOTP_CTRL_ADDR_MASK));

  // now change the data
  OCOTP->DATA=data_temp;
}

void read_fuse_word(int address)
{
  //configure read fuse word address
  OCOTP->CTRL=(address & OCOTP_CTRL_ADDR_MASK);

  //Set read fuse bit
  OCOTP->READ_CTRL|=OCOTP_READ_CTRL_READ_FUSE_MASK;
}

void wait_for_fuse(void)
{
  //wait for busy is clear
   while (OCOTP->CTRL & OCOTP_CTRL_BUSY_MASK)
   {
   }

  //check if error bit is asserted
  if (OCOTP->CTRL & OCOTP_CTRL_ERROR_MASK)
  {
    // clear the error bit
    OCOTP->CTRL_CLR|=OCOTP_CTRL_CLR_ERROR_MASK;
  }
}




