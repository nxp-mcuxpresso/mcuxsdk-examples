/*
** ###################################################################
**     Processors:          MIMXRT1152AHM6B
**                          MIMXRT1152AVM6B
**                          MIMXRT1152DHM8B
**                          MIMXRT1152DVM8B
**                          MIMXRT1152XHM8B
**                          MIMXRT1152XVM8B
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1150RM, Rev 1, 01/2026
**
**     Abstract:
**         Linker file for the GNU C Compiler,
**         openvg_tiger_freertos on FRDM-IMXRT1152, hyperram targets.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2026 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
**
** Memory layout notes
** -------------------
** Derived from the FRDM-IMXRT1152 lvgl_examples hyperram linker script, but
** re-shaped to resemble the openvg_tiger memory layout used on
** MIMXRT1176 (openvg_tiger_MIMXRT1176xxxxx_cm7_sdram_only.ld):
**   - a large dedicated heap in its own m_heap region placed above the
**     cached external RAM data region,
**   - a large call stack kept OUT of that region (DTCM) so a heap overflow
**     cannot clobber it,
**   - a large non-cacheable region for the VGLite contiguous memory pool and
**     the frame buffers.
**
** The 32 MB HyperRAM mapped by BOARD_MPU_REGION_HYPERRAM at 0x60000000 is
** split into the conventional FRDM-IMXRT1152 halves, with the cached half
** carved up to fit the mandatory 9 MB (0x900000) malloc heap:
**   0x60000000..0x600FFFFF   1 MB cached      vectors + .text
**   0x60100000..0x606FFFFF   6 MB cached      .data / .bss
**   0x60700000..0x60FFFFFF   9 MB cached      .heap (0x900000, as on RT1176)
**   0x61000000..0x61FFFFFF  16 MB non-cached  .ncache, frame buffers
** The non-cacheable base/size are left exactly as the board expects (16 MB
** at 0x61000000) so the power-of-two / natural alignment asserts in
** BOARD_ConfigMPU() keep holding.
**
** As on RT1176 (where openvg_tiger runs entirely out of SDRAM rather than
** the 256 KB ITCM), the non-XIP hyperram target places vectors and text at
** the bottom of the cached HyperRAM: the OpenVG + VGLite + FreeRTOS image
** does not fit in the 256 KB ITCM at -O0.
*/

/* Entry Point */
ENTRY(Reset_Handler)

/*
** Sizes, identical to openvg_tiger on MIMXRT1176. The heap takes the top
** 9 MB of the cached HyperRAM region, leaving 6 MB for .data / .bss. The
** FreeRTOS kernel heap (CONFIG_configTOTAL_HEAP_SIZE) is a .bss array and is
** sized in this example's prj.conf to fit that 6 MB.
** The aliases below are the ones the RT1176 openvg_tiger script exports.
*/
HEAP_SIZE     = 0x00900000;
__heap_size   = HEAP_SIZE;
_HeapSize     = HEAP_SIZE;
__heap_size__ = HEAP_SIZE;
STACK_SIZE    = 0x0000FA00;

/* Specify the memory areas */
MEMORY
{
  /* Vectors and text at the bottom of the cached HyperRAM (1 MB window). */
  m_interrupts          (RX)  : ORIGIN = 0x60000000, LENGTH = 0x00000400
  m_text                (RX)  : ORIGIN = 0x60000400, LENGTH = 0x000FFC00

  /* Cached HyperRAM above the text window, heap carved off the top. */
  m_data                (RW)  : ORIGIN = 0x60100000, LENGTH = 0x00F00000 - HEAP_SIZE
  m_heap                (RW)  : ORIGIN = ORIGIN(m_data) + LENGTH(m_data), LENGTH = HEAP_SIZE

  /* DTCM: quick-access data and the call stack. */
  m_data2               (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00040000

  /* OCRAM. */
  m_data3               (RW)  : ORIGIN = 0x202C0000, LENGTH = 0x00080000

  /* Non-cacheable HyperRAM. */
  m_ncache              (RW)  : ORIGIN = 0x61000000, LENGTH = 0x01000000
}

/* Define output sections */
SECTIONS
{
  __NCACHE_REGION_START = ORIGIN(m_ncache);
  __NCACHE_REGION_SIZE  = LENGTH(m_ncache);

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
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(CodeQuickAccess)       /* quick access code section */
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
    *(CacheLineData.init)
    . = ALIGN(32);           /* cache line size = 32 bytes */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __NDATA_ROM = __DATA_ROM + (__data_end__ - __data_start__);
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

  __DATA_END = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  .qadata :
  {
    . = ALIGN(4);
    *(DataQuickAccess)       /* quick access data section */
    . = ALIGN(4);
  } > m_data2

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

  /* Large malloc() heap, in its own region above the cached data region. */
  .heap ALIGN(ORIGIN(m_heap),8) (NOLOAD):
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __sheap = .;
    __HeapBase = .;
    . += HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_heap

  /* Call stack at the top of DTCM, away from the heap. */
  .stack ALIGN(ORIGIN(m_data2) + LENGTH(m_data2) - STACK_SIZE, 8) (NOLOAD):
  {
    __StackLimit = .;
    . += STACK_SIZE;
    . = ALIGN(8);
    _stack = .;
    _estack = .;
    __stack = .;
    __StackTop = .;
    __stack_base__ = .;
  } > m_data2

  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
