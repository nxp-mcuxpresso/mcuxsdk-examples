/*
** ###################################################################
**     Processors:          MIMXRT2663AHP8A
**                          MIMXRT2663CHPAA
**                          MIMXRT2663CVVAA
**                          MIMXRT2663DHPAA
**                          MIMXRT2663DVVAA
**                          MIMXRT2663XHP8A
**                          MIMXRT2663XVV8A
**
**     Compiler:            GNU C Compiler
**     Reference manual:    iMXRT2660RM Rev.2 DraftA, 05/2024
**     Version:             rev. 1.0, 2024-11-05
**     Build:               b260603
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2026 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

ENTRY(Reset_Handler)

/* ========================== Hardware addresses ========================== */
/* ======================================================================== */
m_flash_start          = 0x60000000;
m_flash_size           = 0x04000000;
m_itcm_start           = 0x00000000;
m_itcm_size            = 0x00040000;
m_dtcm_start           = 0x20000000;
m_dtcm_size            = 0x00040000;
m_sram0_start          = 0x22000000;
m_sram0_size           = 0x00040000;
m_sram1_start          = 0x22040000;
m_sram1_size           = 0x00040000;
m_sram2_start          = 0x22080000;
m_sram2_size           = 0x00040000;
m_psram_start          = 0x88000000;
m_psram_size           = 0x02000000;

/* ========================== Offsets and sizes =========================== */
/* ======================================================================== */
m_fcb_offset           = 0x400;
m_fcb_size             = 0x300;
m_xmcd_offset          = 0xA00;
m_xmcd_size            = 0x300;
m_container_offset     = 0x1000;
m_container_size       = 0x9000;
m_vector_table_size    = 0x600;
m_app_image_offset     = 0xC000;
m_ncache_alloc_size    = 0x40000;

/* ====================== Region start/end addresses ====================== */
/* ======================================================================== */
m_fcb_start            = m_flash_start + m_fcb_offset;
m_xmcd_start           = m_flash_start + m_xmcd_offset;
m_container_start      = m_flash_start + m_container_offset;
m_text_start           = m_flash_start + m_app_image_offset;
m_text_size            = m_flash_size - m_app_image_offset;

m_ncache_start         = m_sram2_start + m_sram2_size - m_ncache_alloc_size;
m_dataext_start        = m_sram0_start;
m_dataext_size         = m_sram0_size + m_sram1_size;
/*
m_dataext_start        = m_sram2_start;
m_dataext_size         = m_sram2_size;
m_ncache_start         = m_sram0_start + m_sram0_size + m_sram1_size - m_ncache_alloc_size;
*/
_size_cstack_          = DEFINED(__stack_size__) ? __stack_size__ : 0x1000;
_size_heap_            = DEFINED(__heap_size__)  ? __heap_size__  : 0x1000;
m_rvt_offset           = DEFINED(__ram_vector_table__) ? m_vector_table_size : 0;

m_stack_start          = m_dtcm_start + m_dtcm_size - _size_cstack_;
m_stack_size           = _size_cstack_;
m_heap_size            = _size_heap_;
m_heap_start           = DEFINED(__heap_noncacheable__)
                         ? (m_ncache_start + m_ncache_alloc_size - _size_heap_)
                         : m_sram2_start;
m_ncache_size          = m_ncache_alloc_size
                         - (DEFINED(__heap_noncacheable__) ? _size_heap_ : 0);

m_qacode_start         = m_itcm_start + m_rvt_offset;
m_qacode_size          = m_itcm_size - m_rvt_offset;
m_qadata_start         = m_dtcm_start;
m_qadata_size          = m_dtcm_size - _size_cstack_;

/* ================ Target specific CODE / DATA addresses ================= */
/* ======================================================================== */
m_code_start           = m_itcm_start + m_rvt_offset;
m_code_size            = m_itcm_size - m_rvt_offset;
m_data_start           = m_dtcm_start;
m_data_size            = m_dtcm_size - _size_cstack_;

/* ====================== Provided symbol fallbacks ======================= */
/* ======================================================================== */
PROVIDE(__text_root_start__ = 0);
PROVIDE(__text_root_end__   = 0);
PROVIDE(__TEXT_ROM          = 0);
PROVIDE(__text_start__      = 0);
PROVIDE(__text_end__        = 0);

/* ============================ Memory regions ============================ */
/* ======================================================================== */
MEMORY
{
  m_fcb_data       (RX) : ORIGIN = m_fcb_start,        LENGTH = m_fcb_size
  m_xmcd_data      (RX) : ORIGIN = m_xmcd_start,       LENGTH = m_xmcd_size
  m_container_data (RX) : ORIGIN = m_container_start,  LENGTH = m_container_size
  m_interrupts     (RX) : ORIGIN = m_text_start,                       LENGTH = m_vector_table_size
  m_text           (RX) : ORIGIN = m_text_start + m_vector_table_size, LENGTH = m_text_size - m_vector_table_size
  m_rvt            (RX) : ORIGIN = m_itcm_start,                       LENGTH = m_rvt_offset
  m_code           (RWX): ORIGIN = m_code_start,                       LENGTH = m_code_size
  m_data           (RW) : ORIGIN = m_data_start,                       LENGTH = m_data_size
  m_ncache         (RW) : ORIGIN = m_ncache_start,                     LENGTH = m_ncache_size
  m_heap           (RW) : ORIGIN = m_heap_start,                       LENGTH = m_heap_size
  m_stack          (RW) : ORIGIN = m_stack_start,                      LENGTH = m_stack_size
  m_dataext        (RW) : ORIGIN = m_dataext_start,                    LENGTH = m_dataext_size
}

REGION_ALIAS("m_qacode", m_code)
REGION_ALIAS("m_qadata", m_data)

/* =============================== Sections =============================== */
/* ======================================================================== */
SECTIONS
{
  /* ------------ Boot headers (Flash: FCB / XMCD / Container) ------------ */
  .fcb       : { KEEP(*(.boot_hdr.fcb)) }        > m_fcb_data
  .xmcd      : { KEEP(*(.boot_hdr.xmcd_data)) }  > m_xmcd_data
  .container : { KEEP(*(.boot_hdr.container)) }  > m_container_data

  /* ------------------------ Vector table (Flash) ------------------------ */
  .interrupts :
  {
    . = ALIGN(4);
    __Vectors = .;
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > m_interrupts

  /*  RAM vector table (ITCM[0..0x5FF], reserved when __ram_vector_table__)  */
  .ram_vector_table (NOLOAD) :
  {
    . = ALIGN(4);
  } > m_rvt

  /* ---- Pre-scatter startup (Flash-resident, RO-copied targets only) ---- */
  /* SCOPE: .text_root bounds ONLY pre-scatter startup CODE.
   * It does NOT cover .ctors / .dtors / .preinit_array / .init_array /
   * .fini_array -- those are also Flash-resident but are function-pointer
   * ARRAYS (data, not code), placed as separate output sections below.
   * Data arrays are read by __libc_init_array *after* scatter completes,
   * so they don't need to be inside this bounded startup-code region.
   *
   * What lives here:
   *   Every function reachable from Reset_Handler_C before
   *   toolchain_manual_init() completes must be attributed
   *   section(".text.startup") -- Reset_Handler (naked ASM) is
   *   included via its STARTUP_RESET_ATTR macro. libc
   *   __libc_init_array / __libc_fini_array + .init / .fini are
   *   pinned here to save ITCM (one-time-use code).
   *
   * __text_root_start__ / __text_root_end__ bracket THIS region only
   * (code), not the entire Flash-resident footprint. */
  .text_root :
  {
    . = ALIGN(4);
    __text_root_start__ = .;
    KEEP(*(.text.startup))
    KEEP(*(.text.startup.*))
    KEEP(*(.text.__libc_init_array))
    KEEP(*(.text.__libc_fini_array))
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    __text_root_end__ = .;
  } > m_text

  /* --------------- Main code (.text + ARM unwind tables) ---------------- */
  .text : ALIGN(4)
  {
    __text_start__ = .;
    *(.text)
    *(.text*)
    *(.glue_7) *(.glue_7t) *(.eh_frame)
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
    __text_end__ = .;
  } > m_code AT > m_text
  __TEXT_ROM = LOADADDR(.text);

  /* ---- C runtime init/fini (Flash-resident function-pointer arrays) ---- */
  .ctors :
  {
    __CTOR_LIST__ = .;
    KEEP(*crtbegin.o(.ctors))
    KEEP(*crtbegin?.o(.ctors))
    KEEP(*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP(*(SORT(.ctors.*)))
    KEEP(*(.ctors))
    __CTOR_END__ = .;
  } > m_text
  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP(*crtbegin.o(.dtors))
    KEEP(*crtbegin?.o(.dtors))
    KEEP(*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP(*(SORT(.dtors.*)))
    KEEP(*(.dtors))
    __DTOR_END__ = .;
  } > m_text
  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text
  __etext = .;

  /* -------- QuickAccess: fast-execute code (VMA=ITCM, LMA=Flash) -------- */
  .qacode : ALIGN(4)
  {
    __qacode_start__ = .;
    *(CodeQuickAccess)
    . = ALIGN(4);
    __qacode_end__ = .;
  } > m_qacode AT > m_text
  __QACODE_ROM = LOADADDR(.qacode);

  .app_qacode : ALIGN(4)
  {
    __app_qacode_start__ = .;
    /* *libfoo.a:flash.o(.text .text*) */
    . = ALIGN(4);
    __app_qacode_end__ = .;
  } > m_qacode AT > m_text
  __APP_QACODE_ROM = LOADADDR(.app_qacode);

  /* -------- QuickAccess: fast-access data (VMA=DTCM, LMA=Flash) --------- */
  .qadata : ALIGN(4)
  {
    __qadata_start__ = .;
    *(DataQuickAccess)
    . = ALIGN(4);
    __qadata_end__ = .;
  } > m_qadata AT > m_text
  __QADATA_ROM = LOADADDR(.qadata);

  /* ---- Standard RW data (VMA=DTCM/PSRAM, LMA=Flash, scatter-copied) ---- */
  .data : ALIGN(4)
  {
    __data_start__ = .;
    *(.data)
    *(EXCLUDE_FILE(*NeutronDriverPrivate.c.o) .data*)
    *(.rodata)
    *(.rodata*)
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;
  } > m_data AT > m_text
  __DATA_ROM = LOADADDR(.data);

  /* ---- Cache-line-aligned data (64B aligned, for D-cache-safe DMA) ----- */
  .cacheline_data : ALIGN(64)
  {
    __cacheline_data_start__ = .;
    *(CacheLineData.init)
    . = ALIGN(64);
    __cacheline_data_end__ = .;
  } > m_data AT > m_text
  __CACHELINE_ROM = LOADADDR(.cacheline_data);

  .cacheline_bss (NOLOAD) : ALIGN(64)
  {
    __cacheline_bss_start__ = .;
    *(CacheLineData)
    . = ALIGN(64);
    __cacheline_bss_end__ = .;
  } > m_data

  /* -- Non-cacheable data (framebuffer / DMA / shared with peripherals) -- */
  .ncache.init : ALIGN(4)
  {
    __noncache_data_start__ = .;
    *(NonCacheable.init)
    . = ALIGN(4);
    *(.tensordata)
    *(.tensordata*)
    *(.data.neutron_driver)
    *(.data.neutron_driver*)
    __noncache_data_end__ = .;
  } > m_ncache AT > m_text
  __NDATA_ROM = LOADADDR(.ncache.init);

  .ncache (NOLOAD) :
  {
    . = ALIGN(4);
    __noncache_bss_start__ = .;
    *(NonCacheable)
    . = ALIGN(4);
    __noncache_bss_end__ = .;
  } > m_ncache

  /* -------- BSS (zero-init) and no-init (retained across reset) --------- */
  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > m_data

  .noinit (NOLOAD) :
  {
    . = ALIGN(4);
    __noinit_start__ = .;
    *(.noinit)
    *(.noinit*)
    . = ALIGN(4);
    __noinit_end__ = .;
  } > m_data

  /* --------- Heap and Stack (both in dedicated MEMORY regions) ---------- */
  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __HeapBase  = .;
    PROVIDE(end = .);
    . += _size_heap_;
    __HeapLimit = .;
  } > m_data

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . += _size_cstack_;
    __StackTop   = .;
  } > m_stack
  PROVIDE(__stack      = __StackTop);
  PROVIDE(__stack_size = _size_cstack_);

  /* ---- DATAEXT: user overflow region (SRAM0+SRAM1, 512 KB, opt-in) ----- */
  .dataext.init : ALIGN(4)
  {
    __dataext_data_start__ = .;
    /* user: add wildcards for initialized data going to m_dataext */
    . = ALIGN(4);
    *(.modeldata)
    *(.modeldata*)
    *(.elfdata)
    *(.elfdata*)
    __dataext_data_end__ = .;
  } > m_dataext AT > m_text
  __DATAEXT_ROM = LOADADDR(.dataext.init);

  .dataext (NOLOAD) :
  {
    . = ALIGN(4);
    __dataext_bss_start__ = .;
    /* user: add wildcards for zero-init data going to m_dataext */
    . = ALIGN(4);
    __dataext_bss_end__ = .;
  } > m_dataext

  /* ----------------- Image sentinel + overflow ASSERTs ------------------ */
  .image_end :
  {
    __image_end = .;
  } > m_text

  text_data_end = __image_end;
  text_end      = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(text_data_end <= text_end, "region m_text overflowed with text and data")

  ASSERT(_size_cstack_ > 0,
         "__stack_size__ must be > 0")
  ASSERT(_size_cstack_ < m_dtcm_size,
         "__stack_size__ must be less than DTCM size")
  ASSERT((_size_cstack_ & 7) == 0,
         "__stack_size__ must be 8-byte aligned")
  ASSERT(m_data_size > 0,
         "m_data region has no room (check __stack_size__ or PSRAM partition sizes)")
  ASSERT((_size_heap_ & 7) == 0,
         "__heap_size__ must be 8-byte aligned")
  ASSERT(!DEFINED(__heap_noncacheable__) || _size_heap_ < m_ncache_alloc_size,
         "__heap_size__ too large; heap carve-out would exhaust m_ncache")

  /* -------------------- ELF metadata (non-loadable) --------------------- */
  .ARM.attributes 0 : { *(.ARM.attributes) }
}

/* =========================== Exported symbols =========================== */
/* ======================================================================== */
__VECTOR_TABLE                = ORIGIN(m_interrupts);
__VECTOR_RAM                  = DEFINED(__ram_vector_table__) ? ORIGIN(m_rvt) : ORIGIN(m_interrupts);
__RAM_VECTOR_TABLE_SIZE_BYTES = LENGTH(m_rvt);
__NCACHE_REGION_START         = ORIGIN(m_ncache);
__NCACHE_REGION_SIZE          = m_ncache_alloc_size;
__CONTAINER_IMG_OFFSET        = m_app_image_offset - m_container_offset;
__CONTAINER_IMG_LOAD_ADDR     = ORIGIN(m_interrupts);
__CONTAINER_IMG_ENTRY_ADDR    = ORIGIN(m_interrupts);
__CONTAINER_IMG_SIZE          = text_data_end - __VECTOR_TABLE;

