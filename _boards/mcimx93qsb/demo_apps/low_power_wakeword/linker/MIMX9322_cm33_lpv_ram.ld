/*
** ###################################################################
**     Processors:          MIMX9322CVXXK_cm33
**                          MIMX9322CVXXM_cm33
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMX93RM, Internal, November. 2021
**     Version:             rev. 1.0, 2021-11-16
**     Build:               b230423
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0400;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0478 : 0x0;


/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x0FFE0000, LENGTH = 0x00000478
  m_text                (RX)  : ORIGIN = 0x0FFE0478, LENGTH = 0x0001FB88
/*  m_m33_suspend_ram     (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00002000 */
/*  m_a55_suspend_ram     (RW)  : ORIGIN = 0x20002000, LENGTH = 0x00001000 */
/*  m_data                (RW)  : ORIGIN = 0x20003000, LENGTH = 0x0001C000 */
  m_data                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x0001E000
  m_rsc_tbl             (RW)  : ORIGIN = 0x2001E000, LENGTH = 0x00001000
  m_ext_data            (RW)  : ORIGIN = 0xC5000000, LENGTH = 0x01000000
}

/* Specify program headers */
PHDRS
{
  phdr_interrupts PT_LOAD;
  phdr_text       PT_LOAD;
  phdr_data       PT_LOAD;
  phdr_test_mode  PT_LOAD;
  phdr_bss        PT_LOAD;
  phdr_ext_data   PT_NULL; /* Don't load anything or set to zero */
}


/* Define output sections */
SECTIONS
{

  /* Audio buffer in external memory. */
  .ext_data :
  {
    *(.bss.g_external_audio_buffer*)
  } > m_ext_data : phdr_ext_data


  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts : phdr_text

  .resource_table :
  {
    . = ALIGN(8);
    KEEP(*(.resource_table)) /* Resource table */
    . = ALIGN(8);
  } > m_text : phdr_text

  /* The program code and other data goes into internal RAM */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    __quickcodeaccess_start__ = .;
    . = ALIGN(32);
    *(CodeQuickAccess)
    . = ALIGN(32);
    __quickcodeaccess_end__ = .;
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text : phdr_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text : phdr_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text : phdr_text

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
  } > m_text : phdr_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text : phdr_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text : phdr_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text : phdr_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text : phdr_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  /* Place this at the beginnig of the RAM section so its address never changes. */
  .test_mode :
  {
    *(.bss.DataInject)
  } > m_data : phdr_test_mode

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data : phdr_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    __quickdataaccess_start__ = .;
    . = ALIGN(32);
    *(DataQuickAccess)
    . = ALIGN(32);
    __quickdataaccess_end__ = .;
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data : phdr_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss : AT(ORIGIN(m_data))
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
  } > m_data : phdr_bss

/*
  .m33_suspend :
  {
    *(M33SuspendRam)
    . = ALIGN(4);
  } > m_m33_suspend_ram
*/
  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data : phdr_bss

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data : phdr_bss

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  /* For Resource table region */
  __RscTblStart = ORIGIN(m_rsc_tbl);
  __RscTblEnd = ORIGIN(m_rsc_tbl) + LENGTH(m_rsc_tbl);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

