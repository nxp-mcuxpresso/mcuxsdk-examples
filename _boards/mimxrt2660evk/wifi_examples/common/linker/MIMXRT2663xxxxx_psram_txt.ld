/*
** ###################################################################
**     Processors:          MIMXRT2663CVJ5A
**                          MIMXRT2663CVJ5B
**                          MIMXRT2663CVL5A
**                          MIMXRT2663CVL5B
**                          MIMXRT2663DVJ6A
**                          MIMXRT2663DVJ6B
**                          MIMXRT2663DVL6A
**                          MIMXRT2663DVL6B
**                          MIMXRT2663DVN6B
**                          MIMXRT2663XVN5B
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT2660RM Rev.3, 07/2021 | IMXRT266XSRM Rev.0
**     Version:             rev. 0.2, 2022-03-25
**     Build:               b241024
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2026 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

/*
 * RAM Macros
 */
PSRAM_SIZE             = 0x2000000;      /*32MB*/
PSRAM_TEXT_SPLIT_SIZE  = 0x800000;       /*8MB*/
PSRAM_CACHE_SPLIT_SIZE = 0x1800000;      /*24MB for cacheable and noncacheable*/
PSRAM_START            = 0x88000000;     /*PSRAM LLC Address*/
CTCM_START             = 0x0;
CTCM_SIZE              = 0x20000;
VTABLE_SIZE            = 0x800;
DTCM_START             = 0x20000000;
DTCM_DATA_SPLIT_SIZE   = 0x18000;
DTCM_SIZE              = 0x20000;
OCRAM_START            = 0x22000000;
OCRAM_CACHE_SPLIT_SIZE = 0x80000;        /*512KB*/
OCRAM_SIZE             = 0xC0000;        /*768KB*/

/*
 * Deduced Macros
 */
HEAP_SIZE        = DEFINED(__heap_size__)  ? __heap_size__  : 0x0400;
STACK_SIZE       = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;

/* cache data is in OCRAM*/
DATA_START       = OCRAM_START;
CACHE_DATA_SIZE  = DEFINED(__heap_noncacheable__) ? OCRAM_CACHE_SPLIT_SIZE : OCRAM_CACHE_SPLIT_SIZE - HEAP_SIZE;

/* Option 1, NCACHE in PSRAM*/
/* NCACHE_DATA_START = PSRAM_START + PSRAM_CACHE_SPLIT_SIZE*/
/* NCACHE_DATA_SIZE  = DEFINED(__heap_noncacheable__) ? PSRAM_SIZE - PSRAM_CACHE_SPLIT_SIZE - HEAP_SIZE : PSRAM_SIZE - PSRAM_CACHE_SPLIT_SIZE;*/
/* HEAP_START        = DEFINED(__heap_noncacheable__) ? PSRAM_START + PSRAM_SIZE - HEAP_SIZE : OCRAM_START + OCRAM_CACHE_SPLIT_SIZE - HEAP_SIZE;*/

/* Option 2, NCACHE to OCRAM*/
NCACHE_DATA_START = OCRAM_START + OCRAM_CACHE_SPLIT_SIZE;
NCACHE_DATA_SIZE  = DEFINED(__heap_noncacheable__) ? OCRAM_SIZE - OCRAM_CACHE_SPLIT_SIZE - HEAP_SIZE : OCRAM_SIZE - OCRAM_CACHE_SPLIT_SIZE;
HEAP_START        = DEFINED(__heap_noncacheable__) ? OCRAM_START + OCRAM_SIZE - HEAP_SIZE : OCRAM_START + OCRAM_CACHE_SPLIT_SIZE - HEAP_SIZE;
/*
 * Sceneario 1 (heap non-cacheable, put to PSRAM):
 *
 *  +------------------------------------------------------------------------------------------------------------------------+--------------+
 *  | CTCM (QACode) | DTCM (DATA2) | OCRAM (DATA)                | PSRAM (interrupt + text) | PSRAM (data3) | PSRAM (ncache) | PSRAM (heap) |
 *  +------------------------------------------------------------------------------------------------------------------------+--------------+
 *                                                                                                          \_______________________________/
 *
 * Sceneario 2 (heap cacheable, put to OCRAM):
 *
 *  +---------------------------------------------------------------------------------------------------------------------------------------+
 *  | CTCM (QACode) | DTCM (DATA2) | OCRAM (DATA) | OCRAM (heap) | PSRAM (interrupt + text) | PSRAM (date3) | PSRAM (ncache)                |
 *  +---------------------------------------------------------------------------------------------------------------------------------------+
 *                                                                                                          \_______________________________/
 *
 */

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = PSRAM_START,                          LENGTH = VTABLE_SIZE
  m_text                (RX)  : ORIGIN = PSRAM_START + VTABLE_SIZE,            LENGTH = PSRAM_TEXT_SPLIT_SIZE - VTABLE_SIZE
  m_qacode              (RX)  : ORIGIN = CTCM_START,                           LENGTH = CTCM_SIZE
  m_data                (RW)  : ORIGIN = DATA_START,                           LENGTH = CACHE_DATA_SIZE
  m_data2               (RW)  : ORIGIN = DTCM_START,                           LENGTH = DTCM_SIZE
  m_data3               (RW)  : ORIGIN = PSRAM_START + PSRAM_TEXT_SPLIT_SIZE,  LENGTH = PSRAM_CACHE_SPLIT_SIZE - PSRAM_TEXT_SPLIT_SIZE
  m_ncache              (RW)  : ORIGIN = NCACHE_DATA_START,                    LENGTH = NCACHE_DATA_SIZE
  m_heap                (RW)  : ORIGIN = HEAP_START,                           LENGTH = HEAP_SIZE
}

/* Define output sections */
SECTIONS
{
  __NCACHE_REGION_START = ORIGIN(m_ncache);
  __NCACHE_REGION_SIZE  = NCACHE_DATA_SIZE;

  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal RAM */
  .text :
  {
    . = ALIGN(4);
    *(EXCLUDE_FILE(
        */mflash_drv.c.obj
        */fsl_flexspi.c.obj
        *fsl_memcpy.S.obj
    ) .text)                 /* .text sections (code) */
    *(EXCLUDE_FILE(
        *queue.c.obj
        *tcp_out.c.obj
        *mlan_sdio.c.obj
        *mlan_11n_aggr.c.obj
        *mlan_11n_rxreorder.c.obj
        *tcp_in.c.obj
        *inet_chksum.c.obj
        *timers.c.obj
        *fsl_usdhc.c.obj
        *port.c.obj
        *wifi-sdio.c.obj
        *fsl_sdio.c.obj
        *pbuf.c.obj
        *tcp.c.obj
        *udp.c.obj
        *tcpip.c.obj
        *tasks.c.obj
        *lib_a-memset.o
        *fsl_cache.c.obj
        *wifi.c.obj
        *wifi_netif.c.obj
        *fsl_os_abstraction_free_rtos.c.obj
        *fsl_memcpy.S.obj
        */mflash_drv.c.obj
        */fsl_flexspi.c.obj
      ) .text*)              /* .text* sections (code) */
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

  __VECTOR_RAM = ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = 0x0;

  __etext = .;    /* define a global symbol at end of code */

  __data_flash_start = .; /* Symbol is used by startup for data initialization */

  .data : AT(__data_flash_start)
  {
    . = ALIGN(4);
    __data_start__ = .;      /* create a global symbol at data start */
    *(CacheLineData.init)
    . = ALIGN(32);           /* cache line size = 32 bytes */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    *(DataQuickAccess)       /* quick access data section */
    *(.wlan_data .wlan_data.*)
    */mflash_drv.c.obj(.text .text* .rodata .rodata*)
    */fsl_flexspi.c.obj(.text .text* .rodata .rodata*)
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data3

  __ram_function_flash_start = __data_flash_start + (__data_end__ - __data_start__); /* Symbol is used by startup for TCM data initialization */

  .ram_function : AT(__ram_function_flash_start)
  {
    . = ALIGN(32);
    __ram_function_start__ = .;
    *(CodeQuickAccess)
    *queue.c.obj(.text*)
    *tcp_out.c.obj(.text*)
    *mlan_sdio.c.obj(.text*)
    *mlan_11n_aggr.c.obj(.text*)
    *mlan_11n_rxreorder.c.obj(.text*)
    *tcp_in.c.obj(.text*)
    *inet_chksum.c.obj(.text*)
    *timers.c.obj(.text*)
    *fsl_usdhc.c.obj(.text*)
    *port.c.obj(.text*)
    *wifi-sdio.c.obj(.text*)
    *fsl_sdio.c.obj(.text*)
    *pbuf.c.obj(.text*)
    *tcp.c.obj(.text*)
    *udp.c.obj(.text*)
    *tcpip.c.obj(.text*)
    *tasks.c.obj(.text*)
    *lib_a-memset.o(.text*)
    *fsl_cache.c.obj(.text*)
    *wifi.c.obj(.text*)
    *wifi_netif.c.obj(.text*)
    *fsl_os_abstraction_free_rtos.c.obj(.text*)
    *fsl_memcpy.S.obj(.text)
    . = ALIGN(128);
    __ram_function_end__ = .;
  } > m_qacode

  __NDATA_ROM = __ram_function_flash_start + (__ram_function_end__ - __ram_function_start__);
  .ncache.init : AT(__NDATA_ROM)
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_ncache

  .ncache :
  {
    . = ALIGN(4);
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ncache

  __qadata_flash_start = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);
  .qadata : AT(__qadata_flash_start)
  {
    . = ALIGN(4);
    __qadata_start__ = .;
    *(DataQuickAccess)       /* quick access data section */
    . = ALIGN(4);
    __qadata_end__ = .;
  } > m_data2

  text_data_end = __qadata_flash_start + (__qadata_end__ - __qadata_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(text_data_end < text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(CacheLineData)
    . = ALIGN(32);           /* cache line size = 32 bytes */
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
  } > m_heap

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
}
