/*
** ###################################################################
**     Processors:          MIMXRT1062CVJ5A
**                          MIMXRT1062CVL5A
**                          MIMXRT1062DVJ6A
**                          MIMXRT1062DVL6A
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1060RM Rev.1, 12/2018 | IMXRT1060SRM Rev.3
**     Version:             rev. 0.1, 2017-01-10
**     Build:               b210709
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2021 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

MEMORY
{
  /* Define each memory region */
  BOARD_FLASH (rx) : ORIGIN = 0x60000000, LENGTH = 0x800000 /* 8M bytes (alias Flash) */  
  SRAM_OC (rwx) : ORIGIN = 0x20200000, LENGTH = 0x80000 /* 768K bytes (alias RAM) */  
  SRAM_DTC (rwx) : ORIGIN = 0x20000000, LENGTH = 0x20000 /* 188K bytes (alias RAM2) */  
  SRAM_ITC (rwx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128K bytes (alias RAM3) */  
  BOARD_SDRAM (rwx) : ORIGIN = 0x80000000, LENGTH = 0x1e00000 /* 30M bytes (alias RAM4) */  
  NCACHE_REGION (rwx) : ORIGIN = 0x81e00000, LENGTH = 0x200000 /* 2M bytes (alias RAM5) */  
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_FLASH = 0x60000000  ; /* BOARD_FLASH */  
  __base_Flash = 0x60000000 ; /* Flash */  
  __top_BOARD_FLASH = 0x60000000 + 0x800000 ; /* 8M bytes */  
  __top_Flash = 0x60000000 + 0x800000 ; /* 8M bytes */  
  __base_SRAM_OC = 0x20200000  ; /* SRAM_OC */  
  __base_RAM = 0x20200000 ; /* RAM */  
  __top_SRAM_OC = 0x20200000 + 0x80000 ; /* 768K bytes */  
  __top_RAM = 0x20200000 + 0x80000 ; /* 768K bytes */  
  __base_SRAM_DTC = 0x20000000  ; /* SRAM_DTC */  
  __base_RAM2 = 0x20000000 ; /* RAM2 */  
  __top_SRAM_DTC = 0x20000000 + 0x2f000 ; /* 188K bytes */  
  __top_RAM2 = 0x20000000 + 0x2f000 ; /* 188K bytes */  
  __base_SRAM_ITC = 0x0  ; /* SRAM_ITC */  
  __base_RAM3 = 0x0 ; /* RAM3 */  
  __top_SRAM_ITC = 0x0 + 0x20000 ; /* 128K bytes */  
  __top_RAM3 = 0x0 + 0x20000 ; /* 128K bytes */  
  __base_BOARD_SDRAM = 0x80000000  ; /* BOARD_SDRAM */  
  __base_RAM4 = 0x80000000 ; /* RAM4 */  
  __top_BOARD_SDRAM = 0x80000000 + 0x1e00000 ; /* 30M bytes */  
  __top_RAM4 = 0x80000000 + 0x1e00000 ; /* 30M bytes */  
  __base_NCACHE_REGION = 0x81e00000  ; /* NCACHE_REGION */  
  __base_RAM5 = 0x81e00000 ; /* RAM5 */  
  __top_NCACHE_REGION = 0x81e00000 + 0x200000 ; /* 2M bytes */  
  __top_RAM5 = 0x81e00000 + 0x200000 ; /* 2M bytes */

GROUP (
  "libgcc.a"
  "libc_nano.a"
  "libm.a"
  "libcr_newlib_nohost.a"
)

ENTRY(ResetISR)

SECTIONS
{
     __base_NCACHE_REGION = ORIGIN(SRAM_OC);
     __top_NCACHE_REGION  = ORIGIN(SRAM_OC) + 0x00080000;
     /* Image Vector Table and Boot Data for booting from external flash */
    .boot_hdr : ALIGN(4)
    {
        FILL(0xff)
        __boot_hdr_start__ = ABSOLUTE(.) ;
        KEEP(*(.boot_hdr.conf))
        . = 0x1000 ;
        KEEP(*(.boot_hdr.ivt))
        . = 0x1020 ;
        KEEP(*(.boot_hdr.boot_data))
        . = 0x1030 ;
        KEEP(*(.boot_hdr.dcd_data))
        __boot_hdr_end__ = ABSOLUTE(.) ;
        . = 0x2000 ;
    } >BOARD_FLASH

    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_RAM5));
        LONG(    ADDR(.data_RAM5));
        LONG(  SIZEOF(.data_RAM5));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_RAM5));
        LONG(  SIZEOF(.bss_RAM5));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

        *(EXCLUDE_FILE(
            *queue.o
            *lib_a-memcpy-stub.o
            *timers.o
            *port.o
            *startup_mimxrt1062.o
            *lib_a-memset.o
            *fsl_cache.o
            *fsl_memcpy.o
            *stream_buffer.o
            *ncp_inet.o
            *ncp_tlv_adapter.o
            *ncp_utils.o
            *ncp_crc_dcp.o
            *ncp_crc_sw.o
            *ncp_pm.o
            *ncp_pm_action.o
            *fsl_usdhc.o
            *fsl_sdio.o
            *ncp_intf_sdio_host.o
        ) .text*)
       KEEP(*freertos*/tasks.o(.rodata*)) /* FreeRTOS Debug Config */
       *(.rodata .rodata.* .constdata .constdata.*)
       . = ALIGN(4);
    } > BOARD_FLASH
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > BOARD_FLASH

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > BOARD_FLASH
 
    _etext = .;
        
    /* DATA section for SRAM_DTC */

    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        PROVIDE(__start_data_SRAM_DTC = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_DTC)
       *(DataQuickAccess)
       *(NonCacheable.init)
        *(.data.$RAM2)
        *(.data.$SRAM_DTC)
        *(.data.$RAM2.*)
        *(.data.$SRAM_DTC.*)

        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
        PROVIDE(__end_data_SRAM_DTC = .) ;
     } > SRAM_DTC AT>BOARD_FLASH

    /* DATA section for SRAM_ITC */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        PROVIDE(__start_data_SRAM_ITC = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_ITC)
       *(CodeQuickAccess)
        *(.data.$RAM3)
        *(.data.$SRAM_ITC)
        *(.data.$RAM3.*)
        *(.data.$SRAM_ITC.*)

        *queue.o(.text*)
        *lib_a-memcpy-stub.o(.text*)
        *timers.o(.text*)
        *port.o(.text*)
        *startup_mimxrt1062.o(.text*)
        *lib_a-memset.o(.text*)
        *fsl_cache.o(.text*)
        *fsl_memcpy.o(.text*)
        *stream_buffer.o(.text)
        *ncp_inet.o(.text)
        *ncp_tlv_adapter.o(.text)
        *ncp_utils.o(.text)
        *ncp_crc_dcp.o(.text)
        *ncp_crc_sw.o(.text)
        *ncp_pm.o(.text)
        *ncp_pm_action.o(.text)
        *fsl_usdhc.o(.text*)
        *fsl_sdio.o(.text*)
        *ncp_intf_sdio_host.o(.text)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
        PROVIDE(__end_data_SRAM_ITC = .) ;
     } > SRAM_ITC AT>BOARD_FLASH

    /* DATA section for BOARD_SDRAM */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        PROVIDE(__start_data_BOARD_SDRAM = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$BOARD_SDRAM)
        *(.data.$RAM4)
        *(.data.$BOARD_SDRAM)
        *(.data.$RAM4.*)
        *(.data.$BOARD_SDRAM.*)

        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
        PROVIDE(__end_data_BOARD_SDRAM = .) ;
     } > BOARD_SDRAM AT>BOARD_FLASH

    /* DATA section for NCACHE_REGION */

    .data_RAM5 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM5 = .) ;
        PROVIDE(__start_data_NCACHE_REGION = .) ;
        *(.ramfunc.$RAM5)
        *(.ramfunc.$NCACHE_REGION)
        *(.data.$RAM5)
        *(.data.$NCACHE_REGION)
        *(.data.$RAM5.*)
        *(.data.$NCACHE_REGION.*)

        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM5 = .) ;
        PROVIDE(__end_data_NCACHE_REGION = .) ;
     } > NCACHE_REGION AT>BOARD_FLASH

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_OC AT> SRAM_OC

    /* Main DATA section (SRAM_OC) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       PROVIDE(__start_data_RAM = .) ;
       PROVIDE(__start_data_SRAM_OC = .) ;
       *(vtable)
       *(.ramfunc*)
       KEEP(*(CodeQuickAccess))
       KEEP(*(DataQuickAccess))
       *(RamFunction)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_RAM = .) ;
       PROVIDE(__end_data_SRAM_OC = .) ;
    } > SRAM_OC AT>BOARD_FLASH

    /* BSS section for SRAM_DTC */
    .bss_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       PROVIDE(__start_bss_SRAM_DTC = .) ;
       *(NonCacheable)
               . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
       PROVIDE(__end_bss_SRAM_DTC = .) ;
    } > SRAM_DTC AT> SRAM_DTC

    /* BSS section for SRAM_ITC */
    .bss_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       PROVIDE(__start_bss_SRAM_ITC = .) ;
               . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
       PROVIDE(__end_bss_SRAM_ITC = .) ;
    } > SRAM_ITC AT> SRAM_ITC

    /* BSS section for BOARD_SDRAM */
    .bss_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       PROVIDE(__start_bss_BOARD_SDRAM = .) ;
               . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
       PROVIDE(__end_bss_BOARD_SDRAM = .) ;
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* BSS section for NCACHE_REGION */
    .bss_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM5 = .) ;
       PROVIDE(__start_bss_NCACHE_REGION = .) ;
               . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM5 = .) ;
       PROVIDE(__end_bss_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* MAIN BSS SECTION */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .) ;
        PROVIDE(__start_bss_SRAM_OC = .) ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end_bss_RAM = .) ;
        PROVIDE(__end_bss_SRAM_OC = .) ;
        PROVIDE(end = .);
    } > SRAM_OC AT> SRAM_OC

    /* NOINIT section for SRAM_DTC */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM2 = .) ;
       PROVIDE(__start_noinit_SRAM_DTC = .) ;
       *(.noinit.$RAM2)
       *(.noinit.$SRAM_DTC)
       *(.noinit.$RAM2.*)
       *(.noinit.$SRAM_DTC.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM2 = .) ;
       PROVIDE(__end_noinit_SRAM_DTC = .) ;
    } > SRAM_DTC AT> SRAM_DTC

    /* NOINIT section for SRAM_ITC */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM3 = .) ;
       PROVIDE(__start_noinit_SRAM_ITC = .) ;
       *(.noinit.$RAM3)
       *(.noinit.$SRAM_ITC)
       *(.noinit.$RAM3.*)
       *(.noinit.$SRAM_ITC.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM3 = .) ;
       PROVIDE(__end_noinit_SRAM_ITC = .) ;
    } > SRAM_ITC AT> SRAM_ITC

    /* NOINIT section for BOARD_SDRAM */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM4 = .) ;
       PROVIDE(__start_noinit_BOARD_SDRAM = .) ;
       *(.noinit.$RAM4)
       *(.noinit.$BOARD_SDRAM)
       *(.noinit.$RAM4.*)
       *(.noinit.$BOARD_SDRAM.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM4 = .) ;
       PROVIDE(__end_noinit_BOARD_SDRAM = .) ;
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* NOINIT section for NCACHE_REGION */
    .noinit_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM5 = .) ;
       PROVIDE(__start_noinit_NCACHE_REGION = .) ;
       *(.noinit.$RAM5)
       *(.noinit.$NCACHE_REGION)
       *(.noinit.$RAM5.*)
       *(.noinit.$NCACHE_REGION.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM5 = .) ;
       PROVIDE(__end_noinit_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .) ;
        PROVIDE(__start_noinit_SRAM_OC = .) ;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
       PROVIDE(__end_noinit_RAM = .) ;
       PROVIDE(__end_noinit_SRAM_OC = .) ;        
    } > SRAM_OC AT> SRAM_OC

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1000;
    .heap (NOLOAD) :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > SRAM_OC

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill (NOLOAD) :
    {
        . += _StackSize;
    } > SRAM_DTC
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM_DTC) + LENGTH(SRAM_DTC) - _StackSize - 0 (NOLOAD) :  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > SRAM_DTC

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
}
