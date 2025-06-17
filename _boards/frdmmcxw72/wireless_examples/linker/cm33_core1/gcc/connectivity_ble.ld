/*
** ###################################################################
**     Processors:          KW47B42Z83AFTA_cm33_core1
**                          KW47B42Z96FTA_cm33_core1
**                          KW47B42Z97AFTA_cm33_core1
**                          KW47B42ZB2AFTA_cm33_core1
**                          KW47B42ZB3AFTA_cm33_core1
**                          KW47B42ZB6AFTA_cm33_core1
**                          KW47B42ZB7AFTA_cm33_core1
**                          MCXW727CAFTA_cm33_core1
**                          MCXW727AAFTA_cm33_core1
**                          MCXW727DAFTA_cm33_core1
**
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2024 NXP
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

GROUP (
  libm.a
)

/* Entry Point */
ENTRY(Reset_Handler)
/* Stack and Heap Sizes */
__size_cstack__        = DEFINED(__stack_size__)?__stack_size__:0x0400;
__size_heap__          = DEFINED(__heap_size__)?__heap_size__:0x000;
m_flash_start                       = 0x00000000;
m_flash_end                         = 0x0007FFFF;
m_flash_size                        = m_flash_end - m_flash_start +1;
m_sector_size                       = 0x2000;
m_vector_table_size                 = 0x400;
m_interrupts_start                  = m_flash_start;
m_interrupts_end                    = m_interrupts_start + m_vector_table_size - 1;
__VECTOR_TABLE                      = m_interrupts_start;

m_text_start                        = m_interrupts_end  + 1;
m_text_end                          = m_flash_end;
m_text_size                         = m_text_end - m_text_start + 1;

/* The addition of SMU size and the DMEM size is a constant but the sharing can change (see SMU_MEM_SEL)*/
smu_and_dmem_size                   = 0x00028000;
m_dmem_size                         = 0x00018000;  /*96 KB*/
m_ram1_start                        = 0x00140000;
m_ram1_end                          = m_ram1_start + m_dmem_size - 1;

/* Used by the fwk_platform_lowpower module */
gLowpowerPowerDownEnable_d          = DEFINED(gLowpowerPowerDownEnable_d) ? 1 : 0;
m_ram_vector_table_size             = m_vector_table_size;
HEAP_SIZE                           = DEFINED(__heap_size__)  ? __heap_size__  : 0x0000;
STACK_SIZE                          = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;

m_interrupts_ram_start              = m_cstack_end + 1;
m_interrupts_ram_end                = m_interrupts_ram_start + m_ram_vector_table_size - 1;
__VECTOR_RAM                        = m_interrupts_ram_start;
__RAM_VECTOR_TABLE_SIZE_BYTES       = m_ram_vector_table_size;

m_cstack_start                      = m_ram1_start;
m_cstack_end                        = m_cstack_start + __size_cstack__ - 1;
m_data1_size                        = m_dmem_size - __size_cstack__;
m_data1_start                       = m_cstack_end + 1;
m_data1_end                         = m_ram1_end;

firmware_ram_lower_limit            = DEFINED(lp_ram_lower_limit) ? lp_ram_lower_limit : m_cstack_start;
firmware_ram_upper_limit            = DEFINED(lp_ram_upper_limit) ? lp_ram_upper_limit : m_data1_end;

/*m_sqram_debug_size                  = 0x7C;*/
/*m_sqram_btblebuf_size               = 0x5400;*/


/********************************************** Computeur Engine (CE) Memory **********************************************/
/*New section allowing to share debug info to CM33 (LL error/warnings).*/
/* This size can be changed depending on the dmem shared mem muxing chosen (see SMU_MEM_SEL)*/
m_shared_ram_size                 = smu_and_dmem_size - m_dmem_size;
m_shared_ram_start                = 0xB0000000;
m_shared_ram_end                  = m_shared_ram_start + m_shared_ram_size - 1;

m_sqram_debug_start               = 0xB0000000;
m_sqram_debug_size                = 0x7C;           /*31 x 4 bytes*/
m_sqram_debug_end                 = m_sqram_debug_start + m_sqram_debug_size - 1;


m_lowpower_flag_size              = 0x04;
m_lowpower_flag_start    = m_sqram_debug_end + 1;
m_lowpower_flag_end               = m_lowpower_flag_start + m_lowpower_flag_size - 1;

m_sqram_btblesqbuf_start          = m_lowpower_flag_end + 1;
m_sqram_btblesqbuf_size           = 0x5500;        /* was 0x7700 but was reduced to give space to sh_mem and ext_log - can be readjusted if needed*/
                                                   /*Was 0x5000, increased to 0x5500 to hold 4 advertising set with max adv length = 800 bytes in "sqram_btbleaesqbuf" region and increase tx buffer to 20.*/
m_sqram_btblesqbuf_end            = m_sqram_btblesqbuf_start + m_sqram_btblesqbuf_size - 1;

/* Last reserved word of the debug section used for the sfc module */
m_sfc_log_start = 0xB0000078;


m_sqram_coverage_start               = m_sqram_btblesqbuf_end + 1;
m_sqram_coverage_size                = 0x4;
m_sqram_coverage_end                 = m_sqram_coverage_start + m_sqram_coverage_size -1;

/*if (gEnableCoverage_d) {
  define exported symbol dbg_ext_logging_start    = m_sqram_coverage_end + 1;
  define exported symbol dbg_ext_logging_size     = 0x00;      
} else {*/
dbg_ext_logging_start    = m_sqram_btblesqbuf_end + 1;
dbg_ext_logging_size     = 0x200;       /* was 0xB00 but was reduced to 0x200 to increase btbleaesqbuf section. */

dbg_ext_logging_end      = dbg_ext_logging_start + dbg_ext_logging_size - 1;

m_sqram_btbuf_start               = dbg_ext_logging_end + 1;
/* Classic BT buffers, should not be needed, reduce it to 1k for now */
m_sqram_btbuf_size                = 0x00;       /* was 0x200 but was reduced to 0x0 as not needed. */
m_sqram_btbuf_end                 = m_sqram_btbuf_start + m_sqram_btbuf_size -1;

m_sqram_btbleaesqbuf_start        = m_sqram_btbuf_end + 1;
m_sqram_btbleaesqbuf_size         = DEFINED(gEnableCoverage_d)?0xF20:0x2700;       /* was 0xF00 but was increased to hold 2 x 3 x 1650 bytes for advertising. */
                                                                                   /*Increased to hold 4 advertising set with max adv length = 800 bytes*/

m_sqram_btbleaesqbuf_end          = m_sqram_btbleaesqbuf_start + m_sqram_btbleaesqbuf_size -1;

m_sqram_start                     = m_sqram_btbleaesqbuf_end + 1;
/* Nothing placed in sqram region, reduce it to 1k for now */
m_sqram_size                      = 0x00;       /* was 0x400 but was reduced to 0x0 as not needed. */
m_sqram_end                       = m_sqram_start + m_sqram_size -1;

m_sqram_bkup_start                = m_sqram_end + 1;
/* Nothing placed in sqram bkup region, reduce it to 1k for now */
m_sqram_bkup_size                 = 0x00;       /* was 0x400 but was reduced to 0x0 as not needed. */
m_sqram_bkup_end                  = m_sqram_bkup_start + m_sqram_bkup_size -1;

m_sqram_rldat_ae_start            = m_sqram_bkup_end + 1;
m_sqram_rldat_ae_size             = DEFINED(gEnableCoverage_d)?0x00158:0x00258;

m_sqram_rldat_ae_end              = m_sqram_rldat_ae_start + m_sqram_rldat_ae_size -1;

m_sqram_rldat_start               = m_sqram_rldat_ae_end + 1;
m_sqram_rldat_size                = DEFINED(gEnableCoverage_d)?0x00100:0x005dc;  /*to hold 24 connection contexts*/

m_sqram_rldat_end                 = m_sqram_rldat_start + m_sqram_rldat_size -1;

rpmsg_sh_mem_end         = 0xB0009FFF;
rpmsg_sh_mem_size        = 0x1800;
rpmsg_sh_mem_start       = rpmsg_sh_mem_end - rpmsg_sh_mem_size + 1;



m_free_data_start                 = m_sqram_rldat_end + 1;
m_free_data_size                  = m_shared_ram_end - m_sqram_rldat_end;
m_free_data_end                   = rpmsg_sh_mem_start - 1;
m_extra_data_start                = rpmsg_sh_mem_end + 1;
m_extra_data_end                  = m_shared_ram_end;
__HEAP_start__                      = m_extra_data_end - HEAP_SIZE + 1;
__HEAP_end__                        = m_extra_data_end;

/* Last reserved word of the debug section used for the sfc module */
m_sfc_log_start                     = 0x489C0078;
/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = m_interrupts_start, LENGTH = m_vector_table_size
  m_text                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  m_cstack              (RW)  : ORIGIN = m_cstack_start, LENGTH = STACK_SIZE
  m_data1               (RW)  : ORIGIN = m_data1_start, LENGTH = m_data1_size
  rpmsg_sh_mem          (RW)  : ORIGIN = rpmsg_sh_mem_start, LENGTH = rpmsg_sh_mem_size
  m_lowpower_flag       (RW)  : ORIGIN = m_lowpower_flag_start, LENGTH = m_lowpower_flag_size
  sqram_debug_region (RX)  : ORIGIN = m_sqram_debug_start, LENGTH = m_sqram_debug_size
  lowpower_flag_region (RW): ORIGIN = m_lowpower_flag_start, LENGTH = m_lowpower_flag_size
  sqram_btblesqbuf_region (RW): ORIGIN = m_sqram_btblesqbuf_start, LENGTH = m_sqram_btblesqbuf_size
  sqram_btbuf_region (RW): ORIGIN = m_sqram_btbuf_start, LENGTH = m_sqram_btbuf_size
  sqram_btbleaesqbuf_region (RW): ORIGIN = m_sqram_btbleaesqbuf_start, LENGTH = m_sqram_btbleaesqbuf_size
  sqram_region (RW): ORIGIN = m_sqram_start, LENGTH = m_sqram_size
  sqram_bkup_region (RW): ORIGIN = m_sqram_bkup_start, LENGTH = m_sqram_bkup_size
  sqram_rldat_ae_region (RW): ORIGIN = m_sqram_rldat_ae_start, LENGTH = m_sqram_rldat_ae_size
  sqram_rldat_region (RW): ORIGIN = m_sqram_rldat_start, LENGTH = m_sqram_rldat_size
  rpmsg_sh_mem_region (RW): ORIGIN = rpmsg_sh_mem_start, LENGTH = rpmsg_sh_mem_size
  sqram_dbg_ext_logging_region (RW):ORIGIN = dbg_ext_logging_start, LENGTH = dbg_ext_logging_size
  extra_data (RW):ORIGIN = m_extra_data_start, LENGTH = m_extra_data_end-m_extra_data_start+1
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
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

  .sqram_debug_region (NOLOAD) : ALIGN(4)
  {
     *(.debugbuf)
  } > sqram_debug_region
  .lowpower_flag_region  (NOLOAD) : ALIGN(4)
  {
     *(.lpflag)
  }> lowpower_flag_region
  .sqram_btblesqbuf_region (NOLOAD) : ALIGN(4)
  {
     *(.btblesqbuf)
  }> sqram_btblesqbuf_region
  .sqram_btbuf_region (NOLOAD) : ALIGN(4)
  {
      *(.btrxsqbuf)
      *(.btscosqbuf)
  }>sqram_btbuf_region
  .sqram_btbleaesqbuf_region (NOLOAD) : ALIGN(4)
  {
      *(.btbleaesqbuf)
  }>sqram_btbleaesqbuf_region
  .sqram_region (NOLOAD) : ALIGN(4)
  {
      *(.sqbuf)
  }>sqram_region
  .sqram_rldat_ae_region (NOLOAD) : ALIGN(4)
  {
      *(.rl_ae)
  }>sqram_rldat_ae_region
  .sqram_rldat_region (NOLOAD) : ALIGN(4)
  {
      __rldat_start__ = .;    /* create a global symbol at rl data start for INIT_BLE_RL_DAT section in startup file */
      *(.rl_conn)
      __rldat_end__ = .;      /* create a global symbol at rl data end */
  }>sqram_rldat_region
  /*.logging (NOLOAD) : ALIGN(4)
  {
      *(.logging)
  }>logging*/
  .noinit_rpmsg_sh_mem (NOLOAD) : ALIGN(4)
  {
     __RPMSG_SH_MEM_START__ = .;
     *(.noinit.$rpmsg_sh_mem*)
     . = ALIGN(4) ;
     __RPMSG_SH_MEM_END__ = .;
  } > rpmsg_sh_mem_region
  .sqram_dbg_ext_logging_region (NOLOAD) : ALIGN(4)
  {
      *(.trackbuf)
  }>sqram_dbg_ext_logging_region

  .VERSION_TAGS :
  {
    . = ALIGN(4);
    PROVIDE(__start_VERSION_TAGS = .);
    KEEP(*(.VERSION_TAGS));
    PROVIDE(__stop_VERSION_TAGS = .);
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

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.ramfunc*)             /* for functions in ram */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define intermediate symbol at data end */
  } > m_data1


  __DATA_END = __DATA_RAM + (__data_end__ - __data_start__);

  text_end = ORIGIN(m_text) + LENGTH(m_text);
  /*ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")*/

  .stack :
  {
    . = ALIGN(8);
    /* STACK_SIZE is assumed to be multiple of 8 bytes,
     * thus __StackTop which is pointing to the end of
     * this section should be 8 byte aligned.
     */
    . += STACK_SIZE;
    __StackTop = .;
  } > m_cstack

  /* Initializes stack on the end of block */

  /* m_cstack_end is manually calculated and in the end it's not 8 byte aligned
   * breaking the AAPCS standard: 5.2.1.2 Stack constraints at a public interface
   * thus breaking va_args for 64 bit values.
   */
  /* __StackTop   = m_cstack_end; */
  __StackLimit = m_cstack_start;
  PROVIDE(__stack = __StackTop);

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
  } > m_data1

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    PROVIDE(__HeapBase = .);
    . += HEAP_SIZE;
    PROVIDE(__HeapLimit = .);
    *(.heap)
  } > extra_data

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* ASSERT(__StackLimit >= __HeapLimit, "region m_data1 overflowed with stack and heap") */
}

