/***********************************************************************/
/*  ldscript_rom_gnu.ld:  Linker Script File                                        */
/***********************************************************************/
/* This file has been heavily modified for the GNU-Toolchain by:
			Martin Thomas, Kaiserslautern, Germany
      				<mthomas@rhrk.uni-kl.de>
   		http://www.siwawi.arubi.uni-kl.de/avr_projects
--------------------- Thanks Martin Thomas --------------------------*/
/* Adding ---------------------------------------------------------- */
/* Define the supported output formats - elf32-littlearm is the
   default */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

/* Define the target architecture */
OUTPUT_ARCH(arm)

/* Define the system entry point */
ENTRY(__startup)
/* ---------------------------------------------------------- Adding */

/* Memory Definitions 
MEMORY
{
  ROM (rx) : ORIGIN = 0x00000000, LENGTH = 512k
  RAM (rw) : ORIGIN = 0x40000000, LENGTH = 32k
}
*/

/* Section Definitions */
SECTIONS
{
	. = 0x00000000;
	/* first section is .text which is used for code */
	/* Start of .text segment */
    . = ALIGN(4);
    __exidx_start = .;
    PROVIDE (__gnu_textstart = .);
    . = ALIGN(4);
    
  /* first section is .text which is used for code */
   .text :
  {
	/* *crt0.o (.text) */           /* Startup code */
	KEEP(*(.vectorg))
	. = ALIGN(4);
	KEEP(*(.init))             /* Startup code from .init-section */
	*(.text .text.*)           /* remaining code */
	*(.gnu.linkonce.t.*)
	*(.glue_7)
	*(.glue_7t)
	*(.gcc_except_table)
	*(.rodata)                 /* read-only data (constants) */
	*(.rodata*)
	*(.gnu.linkonce.r.*)
	. = ALIGN(4);
  }

	. = ALIGN(4);
	/* (Real) End of .text segment */
    __exidx_end = .;
    PROVIDE (__gnu_textend = .);
	. = ALIGN(4);
  
	/* .ctors .dtors are used for c++ constructors/destructors */
	PROVIDE(__ctors_start__ = .);
	. = ALIGN(4);
	
	/* .ctors .dtors are used for c++ constructors/destructors */
	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		. = ALIGN(4);
		PROVIDE(__ctors_end__ = .);
		. = ALIGN(4);
	} 
	
	. = ALIGN(4);
	PROVIDE(__ctors_end__ = .);
	. = ALIGN(4);
	
	. = ALIGN(4);
	PROVIDE(__dtors_start__ = .);
	. = ALIGN(4);
	
	.dtors :
	{
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		. = ALIGN(4);
		PROVIDE(__dtors_end__ = .);
		. = ALIGN(4);
	} 
  
   . = ALIGN(4);
	PROVIDE(__dtors_end__ = .);
	. = ALIGN(4);
	  
    . = ALIGN(4);   
	_etext = . ;
	PROVIDE (etext = .);
	. = ALIGN(4);

  /* .data section which is used for initialized data */

	. = __exidx_end;
	. = ALIGN(4);
	
  /* .data section which is used for initialized data */
  . = 0x40000000;
  . = ALIGN(4);
  
  .data : AT (_etext)
  {	
	_data = .;
	KEEP(*(.vectmapped))
	. = ALIGN(4);
	*(.fastrun)
	. = ALIGN(4);
	SORT(CONSTRUCTORS)
	. = ALIGN(4);
	*(.data)
	*(.data.*)
	*(.gnu.linkonce.d*)
	. = ALIGN(4);
  }
 
	. = ALIGN(4);
	_edata = . ;
	PROVIDE (edata = .);
	. = ALIGN(4);

  /* .bss section which is used for uninitialized data */
  .bss (NOLOAD) :
  {
    __bss_start = . ;
    __bss_start__ = . ;
    *(.bss) 
    *(.bss.*)
    *(.gnu.linkonce.b*)
    *(COMMON)
    . = ALIGN(4);
  } 

	. = ALIGN(4);
	__bss_end__ = . ;
	PROVIDE (__bss_end = .);
	. = ALIGN(4);

  .stack ALIGN(256) :
  {
    *(.stack)
    PROVIDE (_stack = .);
    . = ALIGN(4);
  } 

	. = ALIGN(4);
	_end = . ;
	PROVIDE (end = .);
	. = ALIGN(4);

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}
