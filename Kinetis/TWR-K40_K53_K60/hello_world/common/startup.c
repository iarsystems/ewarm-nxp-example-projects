/*
 * File:    startup.c
 * Purpose: Generic Kinetis startup code
 *
 * Notes:
 */

#include "common.h"

#if (defined(IAR))
extern void __iar_data_init3 (void);
#endif

/********************************************************************/
void
common_startup(void)
{

#if (defined(CW))	
    extern char __START_BSS[];
    extern char __END_BSS[];
    extern uint32 __DATA_ROM[];
    extern uint32 __DATA_RAM[];
    extern char __DATA_END[];

    /* Declare pointers for various data sections. These pointers
     * are initialized using values pulled in from the linker file
     */
    uint8 * data_ram, * data_rom, * data_rom_end;
    uint8 * bss_start, * bss_end;
#endif

    /* Declare a counter we'll use in all of the copy loops */
    uint32 n;


    /* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */
    extern uint32 __VECTOR_TABLE[];
    extern uint32 __VECTOR_RAM[];

    /* Copy the vector table to RAM */
    if (__VECTOR_RAM != __VECTOR_TABLE)
    {
        for (n = 0; n < (0x410/sizeof(__VECTOR_RAM[0])); n++)
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }
    /* Point the VTOR to the new copy of the vector table */
    write_vtor((uint32)__VECTOR_RAM);

    /* Get the addresses for the .data section (initialized data section) */
	#if (defined(CW))
    data_ram = (uint8 *)__DATA_RAM;
    data_rom = (uint8 *)__DATA_ROM;
    data_rom_end  = (uint8 *)__DATA_END; /* This is actually a RAM address in CodeWarrior */
    n = data_rom_end - data_ram;

    /* Copy initialized data from ROM to RAM */
    while (n--)
      *data_ram++ = *data_rom++;

		bss_start = (uint8 *)__START_BSS;
		bss_end = (uint8 *)__END_BSS;

    /* Clear the zero-initialized data section */
    n = bss_end - bss_start;
    while(n--)
      *bss_start++ = 0;

  #elif (defined(IAR))
    __iar_data_init3();
	#endif		
		
}
/********************************************************************/
