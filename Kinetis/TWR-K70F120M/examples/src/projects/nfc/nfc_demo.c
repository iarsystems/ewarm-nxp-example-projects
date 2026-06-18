/*
 * File:		nfc.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "nfc.h"

/********************************************************************/
void main (void)
{
    uint32 flash_id0, flash_id1, i, temp, nfc_addr;
    char ch;
                
    printf("\nNAND flash controller demo!!\n");
    
    nfc_addr = (uint32) NFC_BASE_PTR;
        
    /* Initialize and reset the NAND flash */
    nand_init();
    nfc_reset_cmd();

    /* Read the NAND flash ID to test basic communication */
    nfc_read_flash_id(0, &flash_id0, &flash_id1);
    printf("\nNAND flash ID = 0x%08X%08X\n", flash_id0, flash_id1);
    
    /* Erase the first block of the NAND flash */
    nfc_block_erase(0);
    
    /* Write data to program into the first NAND page */
    for(i=0; i < PAGE_SIZE; i=i+4)
        *((uint32 *)(nfc_addr + i)) = i;   
    
    /* Program the first page of the NAND */
    nfc_page_program(0,0,0);
    
    /* Read the first page of the NAND */
    nfc_page_read(0,0,0);

    /* Test the data in the first page to make sure it matches the expected values */
    for(i=0; i < PAGE_SIZE; i=i+4)
    {
        temp = *((uint32 *)(nfc_addr + i));       
        if (temp != i)
            printf("NAND error detected! Read: 0x%08x Expected: 0x%08x\n",temp,i);
    }        
        
    /* Write data to program into the second NAND page */
    for(i=0; i < PAGE_SIZE; i=i+4)
        *((uint32 *)(nfc_addr + i)) = i + PAGE_SIZE;   
    
    /* Program the second page of the NAND */
    nfc_page_program(0,1,0);
    
    /* Read the second page of the NAND */
    nfc_page_read(0,1,0);
    
    /* Test the data in the second page to make sure it matches the expected values */
    for(i=0; i < PAGE_SIZE; i=i+4)
    {
        temp = *((uint32 *)(nfc_addr + i));       
        if (temp != (i + PAGE_SIZE))
            printf("NAND error detected! Read: 0x%08x Expected: 0x%08x\n",temp,(i + PAGE_SIZE));
    }        

    printf("\n\nNAND test complete!\n");
    
    while(1)
    {
        ch = in_char();
        out_char(ch);
    } 
}
/********************************************************************/
