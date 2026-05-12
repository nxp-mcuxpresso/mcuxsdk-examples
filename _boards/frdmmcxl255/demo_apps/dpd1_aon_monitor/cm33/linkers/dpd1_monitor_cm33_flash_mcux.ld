/*
** ###################################################################
**     Processors:          MCXL255VDF_cm33
**                          MCXL255VLL_cm33
**
**     Compiler:            GNU C Compiler (MCUXpresso IDE)
**     Reference manual:    MCXL25xRM DraftH
**     Version:             rev. 1.0, 2025-06-13
**     Build:               b250616
**
**     Abstract:
**         Linker file for MCUXpresso IDE (mcux toolchain).
**         CM0+ slave image is NOT in __data_section_table so startup
**         does not auto-copy it to AON SRAM.  The application copies
**         explicitly via APP_CopyCore1Image() when CORE1_IMAGE_COPY_TO_RAM
**         is defined, which allows skipping the copy on DPD1 wakeup.
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

HEAP_SIZE  = DEFINED(_HeapSize)  ? _HeapSize  : 0x1000;
STACK_SIZE = DEFINED(_StackSize) ? _StackSize : 0x1000;

/* Specify the memory areas */
MEMORY
{
  PROGRAM_FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00060000 /* 384K */
  Ram0          (rwx) : ORIGIN = 0x28000000, LENGTH = 0x0001E000 /* 120K */
  AON_SRAM_ALIAS(rwx) : ORIGIN = 0xA1000000, LENGTH = 0x00007E00
  rpmsg_sh_mem  (rwx) : ORIGIN = 0xA1007E00, LENGTH = 0x00000200
}

/* Libraries — must be explicit since MCUXpresso uses -nostdlib */
GROUP (
  "libcr_nohost_nf.a"
  "libcr_c.a"
  "libcr_eabihelpers.a"
  "libgcc.a"
)

/* Symbol aliases expected by MCUXpresso IDE */
__base_PROGRAM_FLASH = ORIGIN(PROGRAM_FLASH);
__base_Flash         = ORIGIN(PROGRAM_FLASH);
__top_PROGRAM_FLASH  = ORIGIN(PROGRAM_FLASH) + LENGTH(PROGRAM_FLASH);
__top_Flash          = ORIGIN(PROGRAM_FLASH) + LENGTH(PROGRAM_FLASH);
__base_Ram0          = ORIGIN(Ram0);
__base_RAM           = ORIGIN(Ram0);
__top_Ram0           = ORIGIN(Ram0) + LENGTH(Ram0);
__top_RAM            = ORIGIN(Ram0) + LENGTH(Ram0);
__base_AON_SRAM_ALIAS = ORIGIN(AON_SRAM_ALIAS);
__base_RAM2          = ORIGIN(AON_SRAM_ALIAS);
__top_AON_SRAM_ALIAS = ORIGIN(AON_SRAM_ALIAS) + LENGTH(AON_SRAM_ALIAS);
__top_RAM2           = ORIGIN(AON_SRAM_ALIAS) + LENGTH(AON_SRAM_ALIAS);
__base_rpmsg_sh_mem  = ORIGIN(rpmsg_sh_mem);
__base_RAM3          = ORIGIN(rpmsg_sh_mem);
__top_rpmsg_sh_mem   = ORIGIN(rpmsg_sh_mem) + LENGTH(rpmsg_sh_mem);
__top_RAM3           = ORIGIN(rpmsg_sh_mem) + LENGTH(rpmsg_sh_mem);

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
        /* .data for Ram0 */
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        /* .data_RAM2 for AON_SRAM_ALIAS (user data only, NOT CM0+ slave) */
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        /* .data_RAM3 for rpmsg_sh_mem */
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        __data_section_table_end = .;

        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        __bss_section_table_end = .;
        __section_table_end = .;
        /* ---- End of Global Section Table ---- */

        *(.after_vectors*)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > PROGRAM_FLASH

    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > PROGRAM_FLASH

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > PROGRAM_FLASH

    _etext = .;

    /* -----------------------------------------------------------------
     * CM0+ slave image: VMA in AON_SRAM_ALIAS, LMA in PROGRAM_FLASH.
     * Intentionally NOT listed in __data_section_table so the startup
     * code does not auto-copy it.  The application copies explicitly
     * via APP_CopyCore1Image() (guarded by CORE1_IMAGE_COPY_TO_RAM).
     * ----------------------------------------------------------------- */
    .data_RAM2_core_m0slave_text :
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .);
        __core_m0slave_START__ = .;
        KEEP(*(.core_m0slave))
    } > AON_SRAM_ALIAS AT>PROGRAM_FLASH

    .data_RAM2_core_m0slave_ARM_extab :
    {
        FILL(0xff)
        KEEP(*(.core_m0slave.ARM.extab))
    } > AON_SRAM_ALIAS AT>PROGRAM_FLASH

    .data_RAM2_core_m0slave_ARM_exidx :
    {
        FILL(0xff)
        KEEP(*(.core_m0slave.ARM.exidx))
    } > AON_SRAM_ALIAS AT>PROGRAM_FLASH

    .data_RAM2_core_m0slave_data :
    {
        FILL(0xff)
        KEEP(*(.core_m0slave.data_*)) KEEP(*(.core_m0slave.data))
        __core_m0slave_END__ = .;

        ASSERT(!(__core_m0slave_START__ == __core_m0slave_END__), "No slave code for _core_m0slave");
    } > AON_SRAM_ALIAS AT>PROGRAM_FLASH

    /* Non-slave data in AON_SRAM_ALIAS (IS in data section table) */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        *(.ramfunc.$RAM2)
        *(.ramfunc.$AON_SRAM_ALIAS)
        *(.data.$RAM2)
        *(.data.$AON_SRAM_ALIAS)
        *(.data.$RAM2.*)
        *(.data.$AON_SRAM_ALIAS.*)
        . = ALIGN(4);
        PROVIDE(__end_data_RAM2 = .);
        PROVIDE(__end_data_AON_SRAM_ALIAS = .);
    } > AON_SRAM_ALIAS AT>PROGRAM_FLASH

    /* DATA section for rpmsg_sh_mem */
    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .);
        PROVIDE(__start_data_rpmsg_sh_mem = .);
        *(.ramfunc.$RAM3)
        *(.ramfunc.$rpmsg_sh_mem)
        *(.data.$RAM3)
        *(.data.$rpmsg_sh_mem)
        *(.data.$RAM3.*)
        *(.data.$rpmsg_sh_mem.*)
        . = ALIGN(4);
        PROVIDE(__end_data_RAM3 = .);
        PROVIDE(__end_data_rpmsg_sh_mem = .);
    } > rpmsg_sh_mem AT>PROGRAM_FLASH

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4);
        _end_uninit_RESERVED = .;
    } > Ram0 AT> Ram0

    .data : ALIGN(4)
    {
        FILL(0xff)
        _data = .;
        PROVIDE(__start_data_RAM = .);
        PROVIDE(__start_data_Ram0 = .);
        *(vtable)
        *(.ramfunc*)
        KEEP(*(CodeQuickAccess))
        KEEP(*(DataQuickAccess))
        *(RamFunction)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
        PROVIDE(__end_data_RAM = .);
        PROVIDE(__end_data_Ram0 = .);
    } > Ram0 AT>PROGRAM_FLASH

    /* BSS section for AON_SRAM_ALIAS */
    .bss_RAM2 (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__start_bss_RAM2 = .);
        PROVIDE(__start_bss_AON_SRAM_ALIAS = .);
        *(.bss.$RAM2)
        *(.bss.$AON_SRAM_ALIAS)
        *(.bss.$RAM2.*)
        *(.bss.$AON_SRAM_ALIAS.*)
        . = ALIGN(. != 0 ? 4 : 1);
        PROVIDE(__end_bss_RAM2 = .);
        PROVIDE(__end_bss_AON_SRAM_ALIAS = .);
    } > AON_SRAM_ALIAS AT> AON_SRAM_ALIAS

    /* BSS section for rpmsg_sh_mem */
    .bss_RAM3 (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__start_bss_RAM3 = .);
        PROVIDE(__start_bss_rpmsg_sh_mem = .);
        *(.bss.$RAM3)
        *(.bss.$rpmsg_sh_mem)
        *(.bss.$RAM3.*)
        *(.bss.$rpmsg_sh_mem.*)
        . = ALIGN(. != 0 ? 4 : 1);
        PROVIDE(__end_bss_RAM3 = .);
        PROVIDE(__end_bss_rpmsg_sh_mem = .);
    } > rpmsg_sh_mem AT> rpmsg_sh_mem

    /* MAIN BSS SECTION */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .);
        PROVIDE(__start_bss_Ram0 = .);
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        PROVIDE(__end_bss_RAM = .);
        PROVIDE(__end_bss_Ram0 = .);
        PROVIDE(end = .);
    } > Ram0 AT> Ram0

    /* NOINIT section for AON_SRAM_ALIAS */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__start_noinit_RAM2 = .);
        PROVIDE(__start_noinit_AON_SRAM_ALIAS = .);
        *(.noinit.$RAM2)
        *(.noinit.$AON_SRAM_ALIAS)
        *(.noinit.$RAM2.*)
        *(.noinit.$AON_SRAM_ALIAS.*)
        . = ALIGN(4);
        PROVIDE(__end_noinit_RAM2 = .);
        PROVIDE(__end_noinit_AON_SRAM_ALIAS = .);
    } > AON_SRAM_ALIAS AT> AON_SRAM_ALIAS

    /* NOINIT section for rpmsg_sh_mem */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
        PROVIDE(__start_noinit_RAM3 = .);
        PROVIDE(__start_noinit_rpmsg_sh_mem = .);
        *(.noinit.$RAM3)
        *(.noinit.$rpmsg_sh_mem)
        *(.noinit.$RAM3.*)
        *(.noinit.$rpmsg_sh_mem.*)
        . = ALIGN(4);
        PROVIDE(__end_noinit_RAM3 = .);
        PROVIDE(__end_noinit_rpmsg_sh_mem = .);
    } > rpmsg_sh_mem AT> rpmsg_sh_mem

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD) : ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .);
        PROVIDE(__start_noinit_Ram0 = .);
        *(.noinit*)
        . = ALIGN(4);
        _end_noinit = .;
        PROVIDE(__end_noinit_RAM = .);
        PROVIDE(__end_noinit_Ram0 = .);
    } > Ram0 AT> Ram0

    /* Heap */
    .heap (NOLOAD) : ALIGN(4)
    {
        _pvHeapStart = .;
        . += HEAP_SIZE;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > Ram0

    /* Reserve space for stack */
    .heap2stackfill (NOLOAD) :
    {
        . += STACK_SIZE;
    } > Ram0

    /* Stack at end of Ram0 */
    .stack ORIGIN(Ram0) + LENGTH(Ram0) - STACK_SIZE - 0 (NOLOAD) : ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + STACK_SIZE;
    } > Ram0

    _image_start = LOADADDR(.text);
    _image_end   = LOADADDR(.data) + SIZEOF(.data);
    _image_size  = _image_end - _image_start;

    /* Symbols for explicit CM0+ image copy (used by APP_CopyCore1Image).
     * core1_image_start is an absolute symbol = flash (LMA) address.
     * core1_image_size is stored as a LONG in flash so that
     *   "extern int core1_image_size; (uint32_t)core1_image_size"
     * reads the value, matching the fsl_incbin.S convention used by armgcc.
     */
    core1_image_start = LOADADDR(.data_RAM2_core_m0slave_text);

    .core1_image_meta : ALIGN(4)
    {
        core1_image_size = .;
        LONG(__core_m0slave_END__ - __core_m0slave_START__);
    } > PROGRAM_FLASH
}
