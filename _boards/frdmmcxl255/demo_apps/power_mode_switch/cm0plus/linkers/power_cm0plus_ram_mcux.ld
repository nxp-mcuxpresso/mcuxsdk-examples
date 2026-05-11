/*
** ###################################################################
**     Processors:          MCXL255VDF_cm0plus
**                          MCXL255VLL_cm0plus
**
**     Compiler:            GNU C Compiler (MCUXpresso IDE)
**     Reference manual:    MCXL25xRM DraftH
**     Version:             rev. 0.9, 2025-06-13
**     Build:               b250616
**
**     Abstract:
**         Linker file for MCUXpresso IDE (mcux toolchain).
**         CM0+ secondary core runs entirely from AON SRAM.
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

/* Entry Point — MCUXpresso IDE startup uses ResetISR */
ENTRY(ResetISR)

HEAP_SIZE  = DEFINED(_HeapSize)  ? _HeapSize  : 0x0400;
STACK_SIZE = DEFINED(_StackSize) ? _StackSize : 0x0400;

/* Specify the memory areas */
MEMORY
{
  AON_SRAM     (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00007E00
  rpmsg_sh_mem (rwx) : ORIGIN = 0x00007E00, LENGTH = 0x00000200
}

/* Libraries — must be explicit since MCUXpresso uses -nostdlib */
GROUP (
  "libcr_nohost_nf.a"
  "libcr_c.a"
  "libcr_eabihelpers.a"
  "libgcc.a"
)

/* Symbol aliases expected by MCUXpresso IDE */
__base_AON_SRAM     = ORIGIN(AON_SRAM);
__base_RAM          = ORIGIN(AON_SRAM);
__top_AON_SRAM      = ORIGIN(AON_SRAM) + LENGTH(AON_SRAM);
__top_RAM           = ORIGIN(AON_SRAM) + LENGTH(AON_SRAM);
__base_rpmsg_sh_mem = ORIGIN(rpmsg_sh_mem);
__base_RAM2         = ORIGIN(rpmsg_sh_mem);
__top_rpmsg_sh_mem  = ORIGIN(rpmsg_sh_mem) + LENGTH(rpmsg_sh_mem);
__top_RAM2          = ORIGIN(rpmsg_sh_mem) + LENGTH(rpmsg_sh_mem);

SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.);
        KEEP(*(.isr_vector))

        /* ---- Global Section Table ---- */
        . = ALIGN(4);
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        __bss_section_table_end = .;
        __section_table_end = .;
        /* ---- End of Global Section Table ---- */

        *(.after_vectors*)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > AON_SRAM

    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > AON_SRAM

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > AON_SRAM

    _etext = .;

    /* DATA section for rpmsg_sh_mem */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .);
        PROVIDE(__start_data_rpmsg_sh_mem = .);
        *(.ramfunc.$RAM2)
        *(.ramfunc.$rpmsg_sh_mem)
        *(.data.$RAM2)
        *(.data.$rpmsg_sh_mem)
        *(.data.$RAM2.*)
        *(.data.$rpmsg_sh_mem.*)
        . = ALIGN(4);
        PROVIDE(__end_data_RAM2 = .);
        PROVIDE(__end_data_rpmsg_sh_mem = .);
    } > rpmsg_sh_mem AT>AON_SRAM

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4);
        _end_uninit_RESERVED = .;
    } > AON_SRAM AT> AON_SRAM

    .data : ALIGN(4)
    {
        FILL(0xff)
        _data = .;
        PROVIDE(__start_data_RAM = .);
        PROVIDE(__start_data_AON_SRAM = .);
        *(vtable)
        *(.ramfunc*)
        KEEP(*(CodeQuickAccess))
        KEEP(*(DataQuickAccess))
        *(RamFunction)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
        PROVIDE(__end_data_RAM = .);
        PROVIDE(__end_data_AON_SRAM = .);
    } > AON_SRAM AT>AON_SRAM

    /* BSS section for rpmsg_sh_mem */
    .bss_RAM2 (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__start_bss_RAM2 = .);
        PROVIDE(__start_bss_rpmsg_sh_mem = .);
        *(.bss.$RAM2)
        *(.bss.$rpmsg_sh_mem)
        *(.bss.$RAM2.*)
        *(.bss.$rpmsg_sh_mem.*)
        . = ALIGN(. != 0 ? 4 : 1);
        PROVIDE(__end_bss_RAM2 = .);
        PROVIDE(__end_bss_rpmsg_sh_mem = .);
    } > rpmsg_sh_mem AT> rpmsg_sh_mem

    /* MAIN BSS SECTION */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .);
        PROVIDE(__start_bss_AON_SRAM = .);
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        PROVIDE(__end_bss_RAM = .);
        PROVIDE(__end_bss_AON_SRAM = .);
        PROVIDE(end = .);
    } > AON_SRAM AT> AON_SRAM

    /* NOINIT section for rpmsg_sh_mem */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__start_noinit_RAM2 = .);
        PROVIDE(__start_noinit_rpmsg_sh_mem = .);
        *(.noinit.$RAM2)
        *(.noinit.$rpmsg_sh_mem)
        *(.noinit.$RAM2.*)
        *(.noinit.$rpmsg_sh_mem.*)
        . = ALIGN(4);
        PROVIDE(__end_noinit_RAM2 = .);
        PROVIDE(__end_noinit_rpmsg_sh_mem = .);
    } > rpmsg_sh_mem AT> rpmsg_sh_mem

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD) : ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .);
        PROVIDE(__start_noinit_AON_SRAM = .);
        *(.noinit*)
        . = ALIGN(4);
        _end_noinit = .;
        PROVIDE(__end_noinit_RAM = .);
        PROVIDE(__end_noinit_AON_SRAM = .);
    } > AON_SRAM AT> AON_SRAM

    /* Heap */
    .heap (NOLOAD) : ALIGN(4)
    {
        _pvHeapStart = .;
        . += HEAP_SIZE;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > AON_SRAM

    /* Reserve space for stack */
    .heap2stackfill (NOLOAD) :
    {
        . += STACK_SIZE;
    } > AON_SRAM

    /* Stack at end of AON_SRAM */
    .stack ORIGIN(AON_SRAM) + LENGTH(AON_SRAM) - STACK_SIZE - 0 (NOLOAD) : ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + STACK_SIZE;
    } > AON_SRAM

    _image_start = LOADADDR(.text);
    _image_end   = LOADADDR(.data) + SIZEOF(.data);
    _image_size  = _image_end - _image_start;
}
