/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

/*
 * File:	startup.c
 * Purpose:	Generic Kinetis startup code
 *
 * Notes:
 */

#include "common.h"

#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"

void common_startup(void)
{
//	extern char __DATA_ROM[];
//	extern char __DATA_RAM[];
//	extern char __DATA_END[];

	/* Declare a counter we'll use in all of the copy loops */
	uint32 n;

	/* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */
	extern uint32 __VECTOR_TABLE[];
	extern uint32 __VECTOR_RAM[];

	/* Copy the vector table to RAM */
	if (__VECTOR_RAM != __VECTOR_TABLE) {
		for (n = 0; n < 0x410; n++)
			__VECTOR_RAM[n] = __VECTOR_TABLE[n];
	}
	/* Point the VTOR to the new copy of the vector table */
	write_vtor((uint32) __VECTOR_RAM);

	/* Get the addresses for the .data section (initialized data section) */
	uint8 *data_ram = __section_begin(".data");
	uint8 *data_rom = __section_begin(".data_init");
	uint8 *data_rom_end = __section_end(".data_init");

	/* Copy initialized data from ROM to RAM */
	n = data_rom_end - data_rom;
	while (n--)
		*data_ram++ = *data_rom++;

	/* Get the addresses for the .bss section (zero-initialized data) */
	uint8 *bss_start = __section_begin(".bss");
	uint8 *bss_end = __section_end(".bss");

	/* Clear the zero-initialized data section */
	n = bss_end - bss_start;
	while (n--)
		*bss_start++ = 0;

	/*
	 * Get addresses for any code sections that need to be copied from ROM to RAM.
	 * The IAR tools have a predefined keyword that can be used to mark individual
	 * functions for execution from RAM. Add "__ramfunc" before the return type in
	 * the function prototype for any routines you need to execute from RAM instead
	 * of ROM. ex: __ramfunc void foo(void);
	 */
	uint8 *code_relocate_ram = __section_begin("CodeRelocateRam");
	uint8 *code_relocate = __section_begin("CodeRelocate");
	uint8 *code_relocate_end = __section_end("CodeRelocate");

	/* Copy functions from ROM to RAM */
	n = code_relocate_end - code_relocate;
	while (n--)
		*code_relocate_ram++ = *code_relocate++;
}
