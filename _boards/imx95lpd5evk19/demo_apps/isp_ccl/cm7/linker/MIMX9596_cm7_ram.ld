/*
** ###################################################################
**     Processors:          MIMX9596CVTxLAA_cm7
**                          MIMX9596DVTxMAA_cm7
**                          MIMX9596XVTxKAA_cm7
**
**     Compiler:            GNU C Compiler
**     Reference manual:    iMX95RM rev1 draftM
**     Version:             rev. 1.0, 2021-11-16
**     Build:               b230506
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**     All rights reserved.
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
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0620 : 0x0;


/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00001000
  m_text                (RX)  : ORIGIN = 0x00001000, LENGTH = 0x0003F000
  m_m33_suspend_ram     (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00002000
  m_a55_suspend_ram     (RW)  : ORIGIN = 0x20002000, LENGTH = 0x00002000
  m_data                (RW)  : ORIGIN = 0x20004000, LENGTH = 0x0002a000
  m_reserved            (RW)  : ORIGIN = 0x2002E000, LENGTH = 0x0000e000
  m_rsc_tbl             (RW)  : ORIGIN = 0x2003c000, LENGTH = 0x00004000
  m_rsc_tbl_in_dram     (RW)  : ORIGIN = 0x80000000, LENGTH = 0x00004000
  m_data_ddr            (RW)  : ORIGIN = 0x80004000, LENGTH = 0x57FFC000
  m_ncache              (RW)  : ORIGIN = 0xD8000000, LENGTH = 0x08000000
  m_arm_m7_reserved     (RW)  : ORIGIN = 0xE0000000, LENGTH = 0x20000000
}

/* Define output sections */
SECTIONS
{
  /* MPU setup is:
  1) Define main DDR-MEM region as shared, cached.
  2) In DDR-MEM region we setup 2 sub-regions:
    a) DDR-MEM-NONCHACHED - shared, non cached
    b) ARM-M7-RESERVED - shared, non cached
  */
  __DDR_MEM_REGION_START = ORIGIN(m_rsc_tbl_in_dram);
  __DDR_MEM_REGION_SIZE  = LENGTH(m_rsc_tbl_in_dram) + LENGTH(m_data_ddr) + LENGTH(m_ncache) + LENGTH(m_arm_m7_reserved);
  __NCACHE_REGION_START = ORIGIN(m_ncache);
  __NCACHE_REGION_SIZE  = LENGTH(m_ncache);
  __M7_RESERVED_REGION_START = ORIGIN(m_arm_m7_reserved);
  __M7_RESERVED_REGION_SIZE  = LENGTH(m_arm_m7_reserved);


  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  .resource_table :
  {
    . = ALIGN(8);
    KEEP(*(.resource_table)) /* Resource table */
    . = ALIGN(8);
  } > m_text

  .ddr_code.init :
  {
    . = ALIGN(4);
    */lwip/*.obj(.text*)
    */debug_console_lite/*.obj(.text*)
    . = ALIGN(4);
  } > m_data_ddr

  /* initialized DDR data section */
  .ddr_data.init :
  {
    . = ALIGN(4);
    __bss_ddr_start__ = .;
    */lwip/*.obj(.bss*)
    . = ALIGN(4);
    __bss_ddr_end__ = .;
  } > m_data_ddr

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
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
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


  .m33_suspend :
  {
    *(M33SuspendRam)
    . = ALIGN(4);
  } > m_m33_suspend_ram

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data :
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
  } > m_data  AT > m_data_ddr
  /* NOTE: __etext is a symbol used by the startup along with __data_start__ and __data_end__ */
  __etext = LOADADDR(.data);

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

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data


  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data

  /* NOTE: .ncache.init must be placed directly after .data init block as
  startup code only initialize destination from __noncachedata_start__ to __noncachedata_init_end__
  but doesn't initialize source, that is taken as a leftover of previous .data section initialization copy. */
  .ncache.init :
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    KEEP(*(NonCacheable.init))
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_ncache AT > m_data_ddr

  . = __noncachedata_init_end__;
  .ncache (NOLOAD):
  {
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ncache

  /* initialized DDR data section */
  .ddr_data.init :
  {
    . = ALIGN(4);
    *(.ddr_data.init)
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } > m_data_ddr
  .gcov_info :
  {
    . = ALIGN(4);
    PROVIDE (__gcov_info_start = .);
    KEEP (*(.gcov_info))
    PROVIDE (__gcov_info_end = .);
    . = ALIGN(4);
  } > m_data_ddr
  /* NOTE: m33 can only reach 3 segments of DDR, each 256MB size (offset 0G, 0.5G, 1G)
   we only initialize first segment which gives us limit of 256MB for init constants */
  ASSERT((ADDR(.ddr_data.init) + SIZEOF(.ddr_data.init)) < 0x90000000, "DDR init block too big. M33 mcu can't access ddr beyond 0x90000000")

  /* Uninitialized DDR data section */
  .ddr_data (NOLOAD):
  {
    . = ALIGN(4);
    *(.ddr_data)
    *(.ddr_data*)
    . = ALIGN(4);
    __ddr_data_end__ = .;
  } > m_data_ddr

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  /* For Resource table region(store table in tcm) */
  __RscTblStart = ORIGIN(m_rsc_tbl);
  __RscTblEnd = ORIGIN(m_rsc_tbl) + LENGTH(m_rsc_tbl);
  /* For Resource table region(store table in dram) */
  __RscTblInDramStart = ORIGIN(m_rsc_tbl_in_dram);
  __RscTblInDramEnd = ORIGIN(m_rsc_tbl_in_dram) + LENGTH(m_rsc_tbl_in_dram);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

