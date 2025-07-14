/*
** ###################################################################
**     Processors:          MCXE31BMPB
**
**     Compiler:            GNU C Compiler
**     Reference manual:    MCXE31RM, Rev. 1, Jan. 2025
**     Version:             rev. 1.0, 2023-03-01
**     Build:               b241119
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2025 NXP
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

m_itcm_start             = 0x00000000;
m_itcm_end               = 0x00007FFF;

m_dtcm_start             = 0x20000000;
m_dtcm_end               = 0x2000FFFF;

m_stdby_ram_start        = 0x20400000;
m_stdby_ram_end          = 0x20407FFF;

m_interrupts_start       = 0x00401000;
m_interrupts_end         = 0x004013FF;

m_boot_header_start      = 0x00400000;
m_boot_header_end        = 0x004000FF;

m_text_start             = 0x00401400;
m_text_end               = 0x007D3FFF;

m_data_start             = 0x20408000;
m_data_end               = 0x2044FFFF;

m_ncache_size            = 0x00004000;
m_ncache_start           = m_dtcm_start;
m_ncache_end             = m_ncache_start + m_ncache_size - 1;

m_qacode_start           = m_itcm_start;
m_qacode_end             = m_itcm_end;
m_qadata_start           = m_ncache_end + 1;
m_qadata_end             = m_dtcm_end;


/* Specify the memory areas */
/* The flash memory region [0x007D4000-0x7FFFFF] is reserved for HSE_B. */
MEMORY
{
  m_boot_header         (RX)  : ORIGIN = m_boot_header_start, LENGTH = m_boot_header_end - m_boot_header_start + 1
  m_interrupts          (RX)  : ORIGIN = m_interrupts_start,  LENGTH = m_interrupts_end - m_interrupts_start + 1
  m_text                (RX)  : ORIGIN = m_text_start,        LENGTH = m_text_end - m_text_start + 1
  m_data                (RW)  : ORIGIN = m_data_start,        LENGTH = m_data_end - m_data_start + 1
  m_ncache              (RW)  : ORIGIN = m_ncache_start,      LENGTH = m_ncache_size
  m_qacode              (RW)  : ORIGIN = m_qacode_start,      LENGTH = m_qacode_end - m_qacode_start + 1
  m_qadata              (RW)  : ORIGIN = m_qadata_start,      LENGTH = m_qadata_end - m_qadata_start + 1
}

/* Define output sections */
SECTIONS
{
  .boot_header :
  {
    . = ALIGN(4);
    KEEP(*(.boot_header))    /* IVT */
    . = ALIGN(4);
  } > m_boot_header

  /* The startup code goes first into internal flash */
  .interrupts :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
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

  /* reserve MTB memory at the beginning of m_data */
  .mtb : /* MTB buffer address as defined by the hardware */
  {
    . = ALIGN(8);
    _mtb_start = .;
    KEEP(*(.mtb_buf)) /* need to KEEP Micro Trace Buffer as not referenced by application */
    . = ALIGN(8);
    _mtb_end = .;
  } > m_data

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __ram_function_flash_start = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for initialization */
  .ram_function : AT(__ram_function_flash_start)
  {
    . = ALIGN(8);
    __ram_function_start__ = .;
    *(CodeQuickAccess)
    . = ALIGN(8);
    __ram_function_end__ = .;
  } > m_qacode

  __qadata_flash_start = __ram_function_flash_start + (__ram_function_end__ - __ram_function_start__);
  .qadata : AT(__qadata_flash_start)
  {
    . = ALIGN(4);
    __qadata_start__ = .;
    . = ALIGN(4);
    __qadata_end__ = .;
  } > m_qadata

  __noncache_data_flash_start = __qadata_flash_start + (__qadata_end__ - __qadata_start__);
  .ncache.init : AT(__noncache_data_flash_start)
  {
    . = ALIGN(8);
    __noncache_data_start__ = .;      /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(8);
    __noncache_data_end__ = .;   /* create a global symbol at ncache data end */
  } > m_ncache

  .ncache :
  {
    . = ALIGN(8);
    __noncache_bss_start__ = .;  /* define a global symbol at ncache bss start */
    *(NonCacheable)
    . = ALIGN(8);
    __noncache_bss_end__ = .;    /* define a global symbol at ncache bss end */
  } > m_ncache

  __DATA_END = __noncache_data_flash_start + (__noncache_data_end__ - __noncache_data_start__);
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

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

