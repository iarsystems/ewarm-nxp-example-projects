#include "common.h"
#include "sysinit.h"
#include "dual_core.h"
#include "lpt.h"

/*
 * Please read the readme.docx in \src\projects\dual_core for details
 *  about this demo
 */
#pragma section=".noinit_m4"

int main()
{
  printf("A5 Main!\n");

  printf("Starting M4 core\n");
  //Set starting point for M4 code
  SRC->GPR[2] = (unsigned int)__section_begin(".noinit_m4") + 0x401; //must be odd address since M4 is thumb

  //Enable M4 core
  CCM->CCOWR = 0x15a5a;

  //Make the A5 wait forever now
  while(1)
  {}
}