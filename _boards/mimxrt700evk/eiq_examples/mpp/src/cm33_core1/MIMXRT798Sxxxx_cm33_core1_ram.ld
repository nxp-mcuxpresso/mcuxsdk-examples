/*
** ###################################################################
**     Processors:          MIMXRT798SGAWBR_cm33_core1
**                          MIMXRT798SGFOB_cm33_core1
**
**     Compiler:            GNU C Compiler
**     Reference manual:    iMXRT700RM Rev.2 DraftA, 05/2024
**     Version:             rev. 3.0, 2025-06-06
**     Build:               b250606
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2026 NXP
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

/* Total SRAM SIZE that does include core 0 private data and shared memory regions */
/* If __use_shmem__ is defined, use SRAM P0-P29, otherwise use SRAM P0-P27 */
SRAM_START_ADDRESS = 0x20000000;
TOTAL_USABLE_SRAM =  DEFINED(__use_shmem__) ? 0x0074A000 : 0x00700000;

/* Start address of text (code) memory region for core1 */
TEXT_START = DEFINED(__core1_text_start__) ? __core1_text_start__ : 0x20600000;

/* Default SRAM size for core 1: 1MB when shared memory is enabled, 0 otherwise */
/* Total SRAM allocated to core 1, can be overridden by defining __core1_total_mem__ */
DEFAULT_CORE1_SRAM = DEFINED(__use_shmem__) ? 0x100000 : 0;
TOTAL_CORE1_SRAM = DEFINED(__core1_total_mem__) ? __core1_total_mem__ : DEFAULT_CORE1_SRAM;

/* Size needed by RPMSG */
RPMSG_SHMEM_SIZE = DEFINED(__use_shmem__) ? 0x2000 : 0;

/* Size needed for shared camera buffers when running USB camera on core 1 */
CAMERA_BUFFERS_SHMEM_SIZE = DEFINED(__camera_buffers_shmem_size__) ? __camera_buffers_shmem_size__ : 0;

/* Total SRAM allocated to core 0 after subtracting core 1 memory, RPMSG shared memory, and camera buffers */
TOTAL_CORE0_SRAM = TOTAL_USABLE_SRAM - TOTAL_CORE1_SRAM - RPMSG_SHMEM_SIZE - CAMERA_BUFFERS_SHMEM_SIZE;

/* Size of the text (code) section for core1 */
TEXT_SIZE = (SRAM_START_ADDRESS + TOTAL_CORE0_SRAM + TOTAL_CORE1_SRAM - TEXT_START);

/* data memory size */
DATA_SIZE = TOTAL_CORE1_SRAM - TEXT_SIZE;

/* Start address of data memory region for core1 (after core0's memory allocation) */
/* End address of data memory region for core1 */
DATA_START = SRAM_START_ADDRESS + TOTAL_CORE0_SRAM;
DATA_END = DATA_START + DATA_SIZE - 1;

/* Size of the interrupt vector table */
/* Start address of the main text section (after interrupt vectors) */
/* Size of the main text section (excluding interrupt vectors) */
/* End address of the text (code) memory region for core1 */
INTERRUPTS_SIZE = 0x00000200;
M_TEXT_START = TEXT_START +  INTERRUPTS_SIZE;
M_TEXT_SIZE = TEXT_SIZE - INTERRUPTS_SIZE;
TEXT_END = TEXT_START + TEXT_SIZE - 1;

ASSERT(DATA_END < TEXT_START, "text memory overlaps with data memory")
 
/* RPMSG shared memory start address in SRAM */
RPMSG_SHMEM_START = SRAM_START_ADDRESS + TOTAL_CORE0_SRAM + TOTAL_CORE1_SRAM;

/* Shared camera buffers start address in SRAM */
CAMERA_BUFFERS_SHMEM_START = RPMSG_SHMEM_START + RPMSG_SHMEM_SIZE;

/* Verify that the text (code) section does not overlap with RPMSG shared memory region */
ASSERT(TEXT_END < RPMSG_SHMEM_START, "text memory overlaps with RPMSG shared memory")

MEMORY
{
  m_interrupts          (RX)  : ORIGIN = TEXT_START, LENGTH = INTERRUPTS_SIZE
  m_text                (RX)  : ORIGIN = M_TEXT_START, LENGTH = M_TEXT_SIZE
  m_data                (RW)  : ORIGIN = DATA_START, LENGTH = DATA_SIZE
  m_core0_mem           (RW)  : ORIGIN = SRAM_START_ADDRESS, LENGTH = TOTAL_CORE0_SRAM
  rpmsg_sh_mem          (RW)  : ORIGIN = RPMSG_SHMEM_START, LENGTH = RPMSG_SHMEM_SIZE
  cam_buff_sh_mem       (RW)  : ORIGIN = CAMERA_BUFFERS_SHMEM_START, LENGTH = CAMERA_BUFFERS_SHMEM_SIZE
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal ram */
  .interrupts :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __Vectors = .;
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal ram */
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

  __VECTOR_RAM = ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = 0x0;
  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")
  _image_size = __DATA_END - __VECTOR_TABLE;

  /* Uninitialized data section */
  /* NOINIT section for rpmsg_sh_mem */
  .noinit_rpmsg_sh_mem (NOLOAD) : ALIGN(4)
  {
     __RPMSG_SH_MEM_START__ = .;
     *(.noinit.$rpmsg_sh_mem*)
     . = ALIGN(4) ;
     __RPMSG_SH_MEM_END__ = .;
  } > rpmsg_sh_mem

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
