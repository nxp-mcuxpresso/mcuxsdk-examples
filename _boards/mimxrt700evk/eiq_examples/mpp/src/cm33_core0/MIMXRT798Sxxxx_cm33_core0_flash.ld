/*
** ###################################################################
**     Processors:          MIMXRT798SGAWAR_cm33_core0
**                          MIMXRT798SGFOA_cm33_core0
**
**     Compiler:            GNU C Compiler
**     Reference manual:    iMXRT700RM Rev.1, 08/2022
**     Version:             rev. 1.0, 2023-11-14
**     Build:               b240227
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

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x1000;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x1000;

M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000300 : 0;

/* Total SRAM SIZE that does include core 1 private data and shared memory regions */
/* If __use_shmem__ is defined, use SRAM P0-P29, otherwise use SRAM P0-P27 */
SRAM_START_ADDRESS = 0x20000000;
TOTAL_USABLE_SRAM =  DEFINED(__use_shmem__) ? 0x0074A000 : 0x00700000;

/* warning NPU see only SRAM P0-P17 */
NCACHE_SIZE = DEFINED(__ncache_size__) ? __ncache_size__ : 0x00300000;  /* default SRAM P14-P27,  */

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

/* DATA_SIZE: SRAM P0-P27 size - NCACHE_SIZE */
DATA_SIZE = TOTAL_CORE0_SRAM - NCACHE_SIZE;

m_ezhv_image_maximum_size = 0x00008000;
m_core1_image_maximum_size = 0x00080000;

DATA_START   = SRAM_START_ADDRESS; /* SRAM P0 */
DATA_END     = DATA_START + DATA_SIZE - 1 - (DEFINED(__heap_noncacheable__) ? 0 : HEAP_SIZE);

NCACHE_START = DATA_START + TOTAL_CORE0_SRAM - NCACHE_SIZE;
NCACHE_END   = NCACHE_START + NCACHE_SIZE - 1 - (DEFINED(__heap_noncacheable__) ? HEAP_SIZE : 0);

HEAP_START   = DEFINED(__heap_noncacheable__) ? NCACHE_END + 1 : DATA_END + 1;

/* Core 1 memory start address in SRAM */
CORE1_MEM_START = SRAM_START_ADDRESS + TOTAL_CORE0_SRAM;

/* RPMSG shared memory start address in SRAM */
RPMSG_SHMEM_START = SRAM_START_ADDRESS + TOTAL_CORE0_SRAM + TOTAL_CORE1_SRAM;

/* Shared camera buffers start address in SRAM */
CAMERA_BUFFERS_SHMEM_START = RPMSG_SHMEM_START + RPMSG_SHMEM_SIZE;

/* Specify the memory areas */
/* The SRAM region [0x000000-0x013FFF] is reserved for ROM bootloader execution. Can be reused after boot. */
/* The SRAM region [0x014000-0x07FFFF] is reserved for Non-cached shared memory or application data. */
/* The SRAM region [0x080000-0x3FFFFF] is reserved for CPU0 application, last 5.5MB non-cacheable data for NPU/GPU/Display etc. */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = 0x28000000, LENGTH = 0x00004000
  m_interrupts          (RX)  : ORIGIN = 0x28004000, LENGTH = 0x00000300
  m_text                (RX)  : ORIGIN = 0x28004300, LENGTH = 0x00400000
  m_data                (RW)  : ORIGIN = DATA_START, LENGTH = DATA_END - DATA_START + 1
  m_ncache              (RW)  : ORIGIN = NCACHE_START, LENGTH = NCACHE_END - NCACHE_START + 1
  m_heap                (RW)  : ORIGIN = HEAP_START, LENGTH = HEAP_SIZE
  m_core1_mem           (RW)  : ORIGIN = CORE1_MEM_START, LENGTH = TOTAL_CORE1_SRAM
  rpmsg_sh_mem          (RW)  : ORIGIN = RPMSG_SHMEM_START, LENGTH = RPMSG_SHMEM_SIZE
  cam_buff_sh_mem       (RW)  : ORIGIN = CAMERA_BUFFERS_SHMEM_START, LENGTH = CAMERA_BUFFERS_SHMEM_SIZE
}

/* Define output sections */
SECTIONS
{
  .flash_config :
  {
    . = ALIGN(4);
    __FLASH_BASE = .;
    KEEP(* (.flash_conf))     /* flash config section */
    . = ALIGN(4);
  } > m_flash_config

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
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(16);
    core1_image_start = .;
    KEEP (*(.core1_code))
    *(.core1_code*)
    core1_image_end = .;
    . = ALIGN(4);
    
    ezhv_image_start = .;
    KEEP (*(.ezhvcode))
    *(.ezhvcode*)
    ezhv_image_end = .;
    . = ALIGN(4);
  } > m_text

  ASSERT((core1_image_end - core1_image_start) <= m_core1_image_maximum_size, "Core1 image size exceeds the limit")
  ASSERT((ezhv_image_end - ezhv_image_start) <= m_ezhv_image_maximum_size, "ezhv image size exceeds the limit")

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
    *(CacheLineData.init)    /* CacheLineData.init sections */
    . = ALIGN(16);           /* cache line size = 16 bytes */
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    *(.modeldata)
    *(.modeldata*)
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __NCACHE_REGION_START = ORIGIN(m_ncache);
  __NCACHE_REGION_SIZE  = NCACHE_SIZE;
  __NDATA_ROM = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for ncache data initialization */

  .ncache.init : AT(__NDATA_ROM)
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_ncache
  . = __noncachedata_init_end__;
  .npu_ncache(NOLOAD) :
  {
    KEEP(*(.npu_ncache_data))
    KEEP(*(.npu_ncache_data*))
  } > m_ncache
  .ezhv_camera(NOLOAD):
  {
    . = ALIGN(4);
    __ezhv_camera_start__ = .;
    KEEP(*(.ezhv_camera))
    KEEP(*(.ezhv_camera*))
    . = ALIGN(4);
    __ezhv_camera_end__ = .;
  } > m_ncache
  /* Make sure camera buffers are not placed beyond P17. This is a hardware limitation of EZHV core */
  ASSERT((__ezhv_camera_end__ - __ezhv_camera_start__) == 0 || . < 0x20540000, "Error: .npu_ncache/ezhv_camera cannot go beyond SRAM P17. Increase the __ncache_size__ from reconfig.cmake")
  .ncache(NOLOAD) :
  {
    KEEP(*(NonCacheable))
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ncache

  __DATA_END = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);

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

  /* Uninitialized data section */
  /* NOINIT section for cam_buff_sh_mem */
  .noinit_cam_buff_sh_mem (NOLOAD) : ALIGN(4)
  {
     __CAM_BUFF_SH_MEM_START__ = .;
     *(.noinit.$cam_buff_sh_mem*)
     . = ALIGN(4) ;
     __CAM_BUFF_SH_MEM_END__ = .;
  } > cam_buff_sh_mem

  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(CacheLineData)
    . = ALIGN(16); /* cache line size = 16 bytes */
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
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
  } > m_heap

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }
}