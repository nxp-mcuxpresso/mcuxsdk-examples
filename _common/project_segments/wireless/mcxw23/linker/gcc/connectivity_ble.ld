/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* Entry Point */
ENTRY(Reset_Handler)
FLASH_START                  = 0x0;
FLASH_SIZE                   = DEFINED(MCXW235) ? 0x80000 : 0xFE000;
FLASH_END                    = FLASH_START + FLASH_SIZE - 1;
FLASH_SECTOR_SIZE            = 0x2000; /* 8KB */

NV_STORAGE_END_ADDRESS       = FLASH_END;
NV_STORAGE_MAX_SECTORS       = DEFINED(MCXW235) ? 2 : 4;
NV_STORAGE_SECTOR_SIZE       = FLASH_SECTOR_SIZE;
NV_STORAGE_SIZE              = FLASH_SECTOR_SIZE * NV_STORAGE_MAX_SECTORS;
NV_STORAGE_START_ADDRESS     = NV_STORAGE_END_ADDRESS - NV_STORAGE_SIZE + 1;

DATASTORE_SIZE               = DEFINED(gUseDatastoreLink_d) ? FLASH_SECTOR_SIZE : 0;
DATASTORE_START_ADDRESS      = NV_STORAGE_START_ADDRESS - DATASTORE_SIZE;
DATASTORE_END_ADDRESS        = DATASTORE_START_ADDRESS + DATASTORE_SIZE -1;

EVENTSTORE_SIZE              = DEFINED(gUseEventstoreLink_d) ? FLASH_SECTOR_SIZE*2 : 0;
EVENTSTORE_START_ADDRESS     = DATASTORE_START_ADDRESS - EVENTSTORE_SIZE;
EVENTSTORE_END_ADDRESS       = EVENTSTORE_START_ADDRESS + EVENTSTORE_SIZE -1;

INT_STORAGE_END              = EVENTSTORE_START_ADDRESS - 1;
INT_STORAGE_SIZE             = DEFINED(gUseInternalStorageLink_d) ? FLASH_SECTOR_SIZE * ( ( (INT_STORAGE_END + 1 - FLASH_START) / 2 ) / FLASH_SECTOR_SIZE + 1) : 0;
INT_STORAGE_START            = INT_STORAGE_END - INT_STORAGE_SIZE + 1;
INT_STORAGE_SECTOR_SIZE      = FLASH_SECTOR_SIZE;

m_text_end                   = INT_STORAGE_START - 1;
m_text_start                 = 0x200;
m_text_size                  = m_text_end - m_text_start + 1;

m_data_start                 = 0x20004000;
m_data_size                  = DEFINED(MCXW235) ? 0x14000 : 0x1C000;
m_data_end                   = m_data_start + m_data_size - 1;

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0800;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0D00;
M_VECTOR_RAM_SIZE = 0x00000140;
LAST_HEAP_ADDRESS = m_data_end; /* Needs to be known to calculate __HEAP_end__ below */

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000200
  m_text                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  NVM_region            (RW)  : ORIGIN = NV_STORAGE_START_ADDRESS, LENGTH = NV_STORAGE_SIZE
  NVM_region_datastore  (RW)  : ORIGIN = DATASTORE_START_ADDRESS, LENGTH = DATASTORE_SIZE
  NVM_region_eventstore (RW)  : ORIGIN = EVENTSTORE_START_ADDRESS, LENGTH = EVENTSTORE_SIZE
  m_data                (RW)  : ORIGIN = m_data_start, LENGTH = m_data_size
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
		/* Global Section Table */
		. = ALIGN(4) ;
		__section_table_start = .;
		__data_section_table = .;
		LONG(LOADADDR(.data));
		LONG(    ADDR(.data)) ;
		LONG(  SIZEOF(.data));
		__data_section_table_end = .;
		__bss_section_table = .;
		LONG(    ADDR(.bss));
		LONG(  SIZEOF(.bss));
		__bss_section_table_end = .;
		__section_table_end = . ;
		/* End of Global Section Table */
		. = ALIGN(4) ;
  } > m_interrupts

  .interrupts_ram :
  {
    . = ALIGN(4);
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data
  __RAM_VECTOR_TABLE_SIZE_BYTES = (__interrupts_ram_end__ - __interrupts_ram_start__);

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(EXCLUDE_FILE(*fsl_spifi.c.obj) .text .rodata)
    *(EXCLUDE_FILE(*fsl_spifi.c.obj) .text* .rodata*)
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

 .VERSION_TAGS :
  {
    . = ALIGN(4);
    PROVIDE(__start_VERSION_TAGS = .);
    KEEP(*(.VERSION_TAGS));
    PROVIDE(__stop_VERSION_TAGS = .);
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

  .NVM_TABLE :
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.ramfunc*)             /* for functions in ram */
    *(RamFunction)           /* for functions in ram */
    *fsl_spifi.c.obj(.text)
    *fsl_spifi.c.obj(.text*)
    *fsl_spifi.c.obj(.rodata)
    *fsl_spifi.c.obj(.rodata*)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE_RW = .);
    KEEP(*(.NVM_TABLE_RW));
    PROVIDE(__stop_NVM_TABLE_RW = .);
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    PROVIDE(__HeapBase = .);
    . += HEAP_SIZE;
    PROVIDE(__HeapLimit = .);
    *(.heap)
  } > m_data

  .stack :
  {
    _vStackBase = .;
    . = ALIGN(8);
    . += STACK_SIZE;
    _vStackTop = . + STACK_SIZE;
  } > m_data

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* Symbol needed by mem_manager_light to know where theoretical end of heap is*/
  __HEAP_end__             = LAST_HEAP_ADDRESS - STACK_SIZE;

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

