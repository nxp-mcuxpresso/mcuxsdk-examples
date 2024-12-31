/*
** ###################################################################
**     Processors:          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Compiler:            GNU C Compiler
**     Reference manual:    RW61X User manual Rev. 0.95, June 2022
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b231201
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2025 NXP
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
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000280 : 0;

/* Specify the memory areas */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = 0x08000000, LENGTH = 0x00001000
  m_interrupts          (RX)  : ORIGIN = 0x08001000, LENGTH = 0x00000280
  m_text                (RX)  : ORIGIN = 0x08001280, LENGTH = 0x001FED80
  m_data                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00130000
}

/* Define output sections */
SECTIONS
{
  .flash_config :
  {
    . = ALIGN(4);
    FILL(0xFF)
    . = 0x400;
    __FLASH_BASE = .;
    KEEP(* (.flash_conf))     /* flash config section */
    . = 0x1000;
  } > m_flash_config

  /* The startup code goes first into internal ram */
  .interrupts :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __Vectors = .;
    KEEP(*(.isr_vector))     /* Startup code */
    FILL(0x00)
    . = 0x280;
  } > m_interrupts

  /* The program code and other data goes into internal ram */
  .text :
  {
    . = ALIGN(4);
    *(EXCLUDE_FILE(
      /* Exclude flash and frequently executed functions from XIP */
      */flexspi_nor_flash_ops.c.obj
      */fsl_flexspi.c.obj
      */fsl_cache.c.obj
      *flash_mcux_flexspi_nor.c.obj
      *jesd216.c.obj
      *memc_mcux_flexspi.c.obj
      *device.c.obj
      *zephyr_mcux_helper.c.obj
      *clock_control_mcux_syscon.c.obj
      *fsl_clock.c.obj
      *flexspi_clock_setup.c.obj
    ) .text)                 /* .text sections (code) */
    *(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        */flexspi_nor_flash_ops.c.obj
        */fsl_flexspi.c.obj
        */fsl_cache.c.obj
        *flash_mcux_flexspi_nor.c.obj
        *jesd216.c.obj
        *memc_mcux_flexspi.c.obj
        *device.c.obj
        *zephyr_mcux_helper.c.obj
        *clock_control_mcux_syscon.c.obj
        *fsl_clock.c.obj
        *flexspi_clock_setup.c.obj
    ) .text*)                /* .text* sections (code) */
    *(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        */flexspi_nor_flash_ops.c.obj
        */fsl_flexspi.c.obj
        */fsl_cache.c.obj
        *flash_mcux_flexspi_nor.c.obj
        *jesd216.c.obj
        *memc_mcux_flexspi.c.obj
        *device.c.obj
        *zephyr_mcux_helper.c.obj
        *clock_control_mcux_syscon.c.obj
        *fsl_clock.c.obj
        *flexspi_clock_setup.c.obj
    ) .rodata)               /* .rodata sections (constants, strings, etc.) */
    *(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        */flexspi_nor_flash_ops.c.obj
        */fsl_flexspi.c.obj
        */fsl_cache.c.obj
        *flash_mcux_flexspi_nor.c.obj
        *jesd216.c.obj
        *memc_mcux_flexspi.c.obj
        *device.c.obj
        *zephyr_mcux_helper.c.obj
        *clock_control_mcux_syscon.c.obj
        *fsl_clock.c.obj
        *flexspi_clock_setup.c.obj
    ) .rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

  initlevel :
  {
     __init_start = .;
     __init_EARLY_start = .; KEEP(*(SORT(.z_init_EARLY?_*))); KEEP(*(SORT(.z_init_EARLY??_*)));
     __init_PRE_KERNEL_1_start = .; KEEP(*(SORT(.z_init_PRE_KERNEL_1?_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_1??_*)));
     __init_PRE_KERNEL_2_start = .; KEEP(*(SORT(.z_init_PRE_KERNEL_2?_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_2??_*)));
     __init_POST_KERNEL_start = .; KEEP(*(SORT(.z_init_POST_KERNEL?_*))); KEEP(*(SORT(.z_init_POST_KERNEL??_*)));
     __init_APPLICATION_start = .; KEEP(*(SORT(.z_init_APPLICATION?_*))); KEEP(*(SORT(.z_init_APPLICATION??_*)));
     __init_SMP_start = .; KEEP(*(SORT(.z_init_SMP?_*))); KEEP(*(SORT(.z_init_SMP??_*)));
     __init_end = .;
     __deferred_init_list_start = .;
     KEEP(*(.z_deferred_init*))
     __deferred_init_list_end = .;
  } > m_text
  device_area : SUBALIGN(4) { _device_list_start = .; KEEP(*(SORT(._device.static.*_?_*))); KEEP(*(SORT(._device.static.*_??_*))); _device_list_end = .; } > m_text


  device_deps : ALIGN_WITH_INPUT
  {
     __device_deps_start = .;
     KEEP(*(SORT(.__device_deps_pass2*)));
     __device_deps_end = .;
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
    *flash_mcux_flexspi_nor.c.obj(.text .text* .rodata .rodata*)
    *fsl_flexspi.c.obj(.text .text* .rodata .rodata*)
    *fsl_cache.c.obj(.text .text* .rodata .rodata*)
    *fsl_clock.c.obj(.text .text* .rodata .rodata*)
    *jesd216.c.obj(.text .text* .rodata .rodata*)
    *memc_mcux_flexspi.c.obj(.text .text* .rodata .rodata*)
    *device.c.obj(.text .text* .rodata .rodata*)
    *zephyr_mcux_helper.c.obj(.text .text* .rodata .rodata*)
    *clock_control_mcux_syscon.c.obj(.text .text* .rodata .rodata* RamFunction)
    *flexspi_clock_setup.c.obj(.text .text* .rodata .rodata* RamFunction)
    *(EXCLUDE_FILE(
      */flash_mcux_flexspi_nor.c.obj
      */fsl_flexspi.c.obj
      */fsl_cache.c.obj
      */fsl_clock.c.obj
      */jesd216.c.obj
      */memc_mcux_flexspi.c.obj
      */device.c.obj
      */zephyr_mcux_helper.c.obj
      */clock_control_mcux_syscon.c.obj
    ) .z_init_EARLY?_* .z_init_EARLY?_* .z_init_PRE_KERNEL_1?_* .z_init_PRE_KERNEL_1??_* ._device.static.*_?_* ._device.static.*_??_* .z_init_POST_KERNEL?_* .z_init_POST_KERNEL??_* .z_init_EARLY?_* .z_init_EARLY??_* .z_init_PRE_KERNEL_2?_* .z_init_PRE_KERNEL_2??_*)
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    __device_states_start = .;
    _image_ram_start = .;
    KEEP(*(".z_devstate"));
    KEEP(*(".z_devstate.*"));
    _image_ram_end = .;
    __device_states_end = .;
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);

  /* Place holder for RAM function.
   * By default CodeQuickAccess is placed in .data section, but if there's dedicated mem region
   * allocated on code bus (SRAM address starts from 0x0), CodeQuickAccess can be inserted in
   * .ram_function and the .ram_function can be located in the code bus region instead of m_data
   * to get better performance. Remember to define __STARTUP_INITIALIZE_RAMFUNCTION macro to
   * build startup_xx.S to ensure the RAM function copy from flash to RAM. */
  .ram_function : AT(__DATA_END)
  {
    . = ALIGN(4);
    __ram_function_start__ = .;
    /* RAM function sections */
    . = ALIGN(4);
    __ram_function_end__ = .;
  } > m_data

  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")
  _image_size = __DATA_END - __VECTOR_TABLE;

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

