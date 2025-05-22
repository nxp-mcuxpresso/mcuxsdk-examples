/*
** ###################################################################
**     Processors:          MIMX94398AVKE_cm33_core1
**                          MIMX94398AVKJ_cm33_core1
**                          MIMX94398AVKM_cm33_core1
**                          MIMX94398AVME_cm33_core1
**                          MIMX94398AVMJ_cm33_core1
**                          MIMX94398AVMM_cm33_core1
**                          MIMX94398CVKE_cm33_core1
**                          MIMX94398CVKJ_cm33_core1
**                          MIMX94398CVKM_cm33_core1
**                          MIMX94398CVME_cm33_core1
**                          MIMX94398CVMJ_cm33_core1
**                          MIMX94398CVMM_cm33_core1
**                          MIMX94398DVKE_cm33_core1
**                          MIMX94398DVKJ_cm33_core1
**                          MIMX94398DVKM_cm33_core1
**                          MIMX94398DVME_cm33_core1
**                          MIMX94398DVMJ_cm33_core1
**                          MIMX94398DVMM_cm33_core1
**                          MIMX94398XVKE_cm33_core1
**                          MIMX94398XVKJ_cm33_core1
**                          MIMX94398XVKM_cm33_core1
**                          MIMX94398XVME_cm33_core1
**                          MIMX94398XVMJ_cm33_core1
**                          MIMX94398XVMM_cm33_core1
**
**     Compiler:            GNU C Compiler
**     Reference manual:    iMX943RM rev1 draftK
**     Version:             rev. 1.0, 2023-11-01
**     Build:               b241111
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2024 NXP
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
  m_interrupts              (RX)  : ORIGIN = 0x0FFC0000, LENGTH = 0x00002000
  m_text                    (RX)  : ORIGIN = 0x0FFC2000, LENGTH = 0x0003E000
  m_data                    (RW)  : ORIGIN = 0x20003000, LENGTH = 0x0002B000
  m_ncache                  (RW)  : ORIGIN = 0x80000000, LENGTH = 0x01000000
  m_rpmsg_shmem_m70_m33s    (RW)  : ORIGIN = 0x20039000, LENGTH = 0x00001800
  m_rpmsg_shmem_m71_m33s    (RW)  : ORIGIN = 0x2003A800, LENGTH = 0x00001800
  m_rsc_tbl                 (RW)  : ORIGIN = 0x2003C000, LENGTH = 0x00004000
  m_rsc_tbl_share_with_mpu  (RW)  : ORIGIN = 0x208A0000, LENGTH = 0x00001000
}

/* Define output sections */
SECTIONS
{
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

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

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
  } > m_data

  __NCACHE_REGION_START = ORIGIN(m_ncache);
  __NCACHE_REGION_SIZE  = LENGTH(m_ncache);
  __NDATA_ROM = __DATA_ROM + SIZEOF(.data); /* Symbol is used by startup for ncache data initialization */
  .ncache.init : AT(__NDATA_ROM)
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_ncache

  . = __noncachedata_init_end__;
  .ncache :
  {
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ncache

  __DATA_END = __NDATA_ROM + SIZEOF(.ncache.init);
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


  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  /* For Resource table region */
  __RscTblStart = ORIGIN(m_rsc_tbl);
  __RscTblEnd = ORIGIN(m_rsc_tbl) + LENGTH(m_rsc_tbl);
  /* For Resource table region in dram, Cortex-A read the resource table from here */
  __RscTblShareWithMpuStart = ORIGIN(m_rsc_tbl_share_with_mpu);
  __RscTblShareWithMpuEnd = ORIGIN(m_rsc_tbl_share_with_mpu) + LENGTH(m_rsc_tbl_share_with_mpu);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

