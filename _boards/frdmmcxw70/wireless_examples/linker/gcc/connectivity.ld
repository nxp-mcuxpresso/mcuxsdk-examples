/*
** ###################################################################
**     Processors:          MCXW70ACMFT
**                          MCXW70ACMMP
**
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2025-2026 NXP
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

/*******************************************  core0/core1 common defines **********************************************/

/* flash */
m_flash_start = 0x0;
m_flash_size  = 0x180000;
m_flash_end   = m_flash_start + m_flash_size - 1;
m_sector_size = 0x2000;

/* FLW - the application flash region */
m_flw_start = 0x00800000;
m_flw_size  = 0x180000;
m_flw_end   = m_flw_start + m_flw_size - 1;

/* 352K reserved flash space for the Radio core at the beginning of the flash */
m_flash_radio_size    = 0x58000;
m_flash_radio_start   = m_flash_start;
m_flash_radio_end     = m_flash_radio_start + m_flash_radio_size - 1;

/* RAM */
m_ram_size    = 0x10000;
m_ram0_start  = 0x20000000;
m_ram0_end    = m_ram0_start + m_ram_size -1;
m_ram1_start  = 0x20010000;
m_ram1_end    = m_ram1_start + m_ram_size - 1;
m_ram2_start  = 0x20020000;
m_ram2_end    = m_ram2_start + m_ram_size - 1;
/* ram3 is mapped to 0x04030000 as TCM-CODE but also remapped to 0x20030000 as TCM-SYSR (data)
 * For connectivity applications, this bank is reserved to the NBU core, core0 shall not use it */
m_ram3_start  = 0x20030000;
m_ram3_end    = m_ram3_start + m_ram_size - 1;

/* 8k shared ram between core0 and core1 - located at the end of the third ram bank */
m_shared_ram_size     = 0x2000;
m_shared_ram_end      = m_ram2_end;
m_shared_ram_start    = m_shared_ram_end - m_shared_ram_size + 1;

/* core1 reserved RAM (64K) */
m_core1_ram_start = m_ram3_start;
m_core1_ram_end   = m_ram3_end;
m_core1_ram_size  = m_core1_ram_end - m_core1_ram_start + 1;

/* Shared memory placements */

/* core0 and core1 RAM shared bank
 * core0 : 0x8E00 (35.5K)
 * core1 : 0x5200 (20.5K)
 * shared between both : 8k
*/
m_core1_shared_ram_end   = m_shared_ram_start - 1;
m_core1_shared_ram_size  = 0x5200;
m_core1_shared_ram_start = m_core1_shared_ram_end - m_core1_shared_ram_size + 1;

/* core0 reserved RAM (128K + 0x8E00 = 163.5K) */
m_core0_ram_start        = m_ram0_start;
m_core0_ram_end          = m_core1_shared_ram_start - 1;


/* Section allowing to share debug info between core0 and core1 */
m_sqram_debug_start         = m_shared_ram_start;
m_sqram_debug_size          = 0x200;
m_sqram_debug_end           = m_sqram_debug_start + m_sqram_debug_size - 1;

/* Section used to share platform/services (SFC, Lowpower) information */
framework_info_sh_mem_start = m_sqram_debug_end + 1;
framework_info_sh_mem_size  = 0x100;
framework_info_sh_mem_end   = framework_info_sh_mem_start + framework_info_sh_mem_size - 1;
m_sfc_log_start             = framework_info_sh_mem_start;
m_sfc_log_size              = 0x04;
m_lowpower_flag_start       = framework_info_sh_mem_start + m_sfc_log_size;
m_lowpower_flag_size        = 0x04;
m_lowpower_flag_end         = m_lowpower_flag_start + m_lowpower_flag_size - 1;
m_shared_data_start         = m_lowpower_flag_end + 1;
m_shared_data_size          = 0xF8;
m_shared_data_end           = m_shared_data_start + m_shared_data_size - 1;
ASSERT(m_shared_data_end <= framework_info_sh_mem_end, "region framework_info_sh_mem overflowed")

rpmsg_sh_mem_start          = framework_info_sh_mem_end + 1;
rpmsg_sh_mem_size           = 0xA00;
rpmsg_sh_mem_end            = rpmsg_sh_mem_start + rpmsg_sh_mem_size - 1;

/* fill the remaining space with the logging buffer */
dbg_ext_logging_start       = rpmsg_sh_mem_end + 1;
dbg_ext_logging_size        = m_shared_ram_size - (dbg_ext_logging_start - m_shared_ram_start);
dbg_ext_logging_end         = dbg_ext_logging_start + dbg_ext_logging_size - 1;

ASSERT(dbg_ext_logging_end <= m_shared_ram_end, "region m_shared_ram overflowed")

/********************************************** FLASH **********************************************/

_flash_start = m_flash_start;

m_vector_table_size = 0x400;

m_interrupts_start  = m_flw_start;
m_interrupts_end    = m_interrupts_start + m_vector_table_size - 1;
__VECTOR_TABLE      = m_interrupts_start;

/* ProdInfos, ot_settings, NVM and INT_STORAGE will use the physical flash address and not the FLW range
 * This adds some complexity because we have to translate between physical address and FLW address
 */

/* This section is not enabled by default but you need it if gHwParamsProdDataPlacement_c is defined to
 * gHwParamsProdDataMainFlashMode_c or gHwParamsProdDataMainFlash2IfrMode_c in your project */
/*
 * m_fsl_prodInfo_size                 = DEFINED(gUseProdInfoMainFlashMode_d) ? m_sector_size : 0;
 */
m_fsl_prodInfo_size                 = m_sector_size;
m_fsl_prodInfo_end                  = m_flash_end; /* MUST use physical flash address and not FLW */
m_fsl_prodInfo_start                = m_fsl_prodInfo_end - m_fsl_prodInfo_size + 1;

PROD_DATA_BASE_ADDR                 = m_fsl_prodInfo_start;

m_ot_settings_page_size             = m_sector_size;
m_ot_settings_page_count            = DEFINED(gUseOtSettingsStorage_d) ? 2 : 0;
m_ot_settings_size                  = DEFINED(gUseOtSettingsStorage_d) ? m_ot_settings_page_size * m_ot_settings_page_count : 0;
m_ot_settings_start                 = m_fsl_prodInfo_start - m_ot_settings_size;
m_ot_settings_end                   = m_fsl_prodInfo_start - 1;

NV_STORAGE_MAX_SECTORS              = DEFINED(gUseNVMLink_d) ? (DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 4) : 0;
NV_STORAGE_SECTOR_SIZE              = m_sector_size;
NV_STORAGE_END_ADDRESS              = m_ot_settings_start - 1;
NV_STORAGE_SIZE                     = (NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE);
NV_STORAGE_START_ADDRESS            = NV_STORAGE_END_ADDRESS - NV_STORAGE_SIZE + 1;
NV_VIRTUAL_PAGE_SZ                  = NV_STORAGE_SIZE / 2;
NV_F_VIRTUAL_PAGE_START             = NV_STORAGE_START_ADDRESS;
NV_S_VIRTUAL_PAGE_START             = NV_F_VIRTUAL_PAGE_START + NV_VIRTUAL_PAGE_SZ;
NV_VIRTUAL_PAGE_LAST_OFFSET         = DEFINED(gUseNVMLink_d) ? NV_VIRTUAL_PAGE_SZ - 5 : 0;
INT_STORAGE_END                     = NV_STORAGE_START_ADDRESS - 1;
/* The NBU firmware is located at the start of the flash so we have to compute the physical start address of the
 * application, meaning m_flash_start + m_flash_radio_size
 */
m_app_flash_start                   = m_flash_start + m_flash_radio_size;
INT_STORAGE_SIZE                    = DEFINED(gUseInternalStorageLink_d) ? m_sector_size * ( ( (INT_STORAGE_END + 1 - m_app_flash_start) / 2 ) / m_sector_size + 1) : 0;
INT_STORAGE_START                   = INT_STORAGE_END - INT_STORAGE_SIZE + 1;
INT_STORAGE_SECTOR_SIZE             = m_sector_size;

__MATTER_FACTORY_DATA_END           = INT_STORAGE_START - 1;
__MATTER_FACTORY_DATA_SIZE          = DEFINED(gUseFactoryData_d) ? m_sector_size : 0;
__MATTER_FACTORY_DATA_START         = __MATTER_FACTORY_DATA_END - __MATTER_FACTORY_DATA_SIZE + 1;

/* This m_image_length is used to define the image length field in the vector table for boot ROM - it is mandatory.
 * We set it to the total available space for the application code minus the reserved areas.
 * We could simplify and use the whole FLW space but we wouldn't be able to detect if the application overflows
 * in the reserved areas since they are referring to the physical flash address instead of FLW.
 * This way, we are able to limit the size of the application code to the available space.
 */
m_reserved_size = m_fsl_prodInfo_end - __MATTER_FACTORY_DATA_START + 1;
m_image_length = m_flw_size - m_flash_radio_size - m_reserved_size;

/* code is placed within FLW range */
m_text_start                        = m_interrupts_end + 1;
m_text_end                          = m_flw_start + m_image_length - 1;
m_text_size                         = m_text_end - m_text_start + 1;

/********************************************** RAM **********************************************/
m_ram_vector_table_size             = m_vector_table_size;
HEAP_SIZE                           = DEFINED(__heap_size__)  ? __heap_size__  : 0x0800;
STACK_SIZE                          = DEFINED(__stack_size__) ? __stack_size__ : 0x01000;

/* On A0 revisions of this chip, the CPU boot ROM does NOT correctly start the
 * application if the initial Main Stack Pointer (MSP) is located in SRAM1.
 * When the application’s SP is placed in SRAM1, the boot ROM refuses to jump
 * to the application entry point, resulting in a silent boot failure.
 *
 * Do NOT relocate the cstack region on A0 silicon.
 */
m_cstack_start   = m_core0_ram_start;
m_cstack_end     = m_cstack_start + STACK_SIZE - 1;

m_interrupts_ram_start              = m_cstack_end + 1;
m_interrupts_ram_end                = m_interrupts_ram_start + m_ram_vector_table_size - 1;
__VECTOR_RAM                        = m_interrupts_ram_start;
__RAM_VECTOR_TABLE_SIZE_BYTES       = m_ram_vector_table_size;

/* data0 region */
m_data0_start = m_interrupts_ram_end + 1;
m_data0_end   = m_core0_ram_end;
m_data0_size  = m_data0_end - m_data0_start + 1;

__HEAP_start__  = m_data0_end - HEAP_SIZE;
__HEAP_end__    = m_data0_end;

firmware_ram_lower_limit            = m_core0_ram_start;
firmware_ram_upper_limit            = m_core0_ram_end;

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = m_interrupts_start, LENGTH = m_vector_table_size
  m_text                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  m_factory_data        (RX)  : ORIGIN = __MATTER_FACTORY_DATA_START, LENGTH = __MATTER_FACTORY_DATA_SIZE
  m_internal_storage    (RX)  : ORIGIN = INT_STORAGE_START, LENGTH = INT_STORAGE_SIZE
  fNVM_region           (RX)  : ORIGIN = NV_F_VIRTUAL_PAGE_START, LENGTH = NV_VIRTUAL_PAGE_SZ
  sNVM_region           (RX)  : ORIGIN = NV_S_VIRTUAL_PAGE_START, LENGTH = NV_VIRTUAL_PAGE_SZ
  m_ot_settings         (RX)  : ORIGIN = m_ot_settings_start, LENGTH = m_ot_settings_size
  m_fsl_prod_info       (RX)  : ORIGIN = m_fsl_prodInfo_start, LENGTH = m_fsl_prodInfo_size
  m_cstack              (RW)  : ORIGIN = m_cstack_start, LENGTH = STACK_SIZE
  m_ram_interrupts      (RW)  : ORIGIN = m_interrupts_ram_start, LENGTH = m_ram_vector_table_size
  m_data0               (RW)  : ORIGIN = m_data0_start, LENGTH = m_data0_size
  rpmsg_sh_mem          (RW)  : ORIGIN = rpmsg_sh_mem_start, LENGTH = rpmsg_sh_mem_size
  m_shared_data         (RW)  : ORIGIN = m_shared_data_start, LENGTH = m_shared_data_size
  m_lowpower_flag       (RW)  : ORIGIN = m_lowpower_flag_start, LENGTH = m_lowpower_flag_size
}

/* Define output sections */
SECTIONS
{
  /* NOINIT section for rpmsg_sh_mem */
  .noinit_rpmsg_sh_mem (NOLOAD) : ALIGN(4)
  {
     __RPMSG_SH_MEM_START__ = .;
     *(.noinit.$rpmsg_sh_mem*)
     . = ALIGN(4) ;
     __RPMSG_SH_MEM_END__ = .;
  } > rpmsg_sh_mem

  .m_shared_data : ALIGN(4)
  {
    *(.m_shared_data)
    . = ALIGN(4);
  } > m_shared_data

  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* ZB OTA related sections, should not consume any memory if .ro_nonce/.ro_ota_header/.ro_se_lnkkey not used */
  .ro_nonce : ALIGN(0x10)
  {
    _FlsNonceStart = ABSOLUTE(.);
    *(.ro_nonce) /* nonce value is 16 bytes.*/
    FILL(0xff)
    . = ALIGN (0x10);
  } > m_text

  .ro_ota_header : ALIGN(0x10)
  {
    _enc_start = ABSOLUTE(.);
    _enc_offset = (_enc_start & 0x0000000F);
    _FlsOtaHeader = ABSOLUTE(.);
    *(.ro_ota_header) /* Ota Header 69 bytes*/
    FILL(0xff)
    . = ALIGN (0x10);
  } > m_text

  .ro_se_lnkKey (ALIGN((. - _enc_offset), 16) + _enc_offset):
  {
    _FlsLinkKey = ABSOLUTE(.);
    *(.ro_se_lnkKey)  /* Link Key 16 bytes*/
    FILL(0xff)
    . = ALIGN (0x10);
  } > m_text

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

  .NVM_TABLE :
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
  } > m_text

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

  ._settings_handler_static :
  {
    . = ALIGN(4);
    _settings_handler_static_list_start = .;
    KEEP(*(SORT(._settings_handler_static.static.*)))
    _settings_handler_static_list_end = .;
    . = ALIGN(4);
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

  .fsl_prod_info :
  {
    . = ALIGN(m_sector_size);
    *(.PROD_DATA*)
  } > m_fsl_prod_info

/* Note that fEraseNVM and sEraseNVM sections are only there to be able to erase an NVM storage content when flashing a new firmware that might be incompatible.
   * This may raise an issue if the final output of the build is a binary image. Indeed, in this case, the linker pads the binary between the top of the text section
   * and fills the space with 0xff till the top of the NVM section, causing an increase in size.
   * With IAR linker, undefining gEraseNVMLink_d linker symbol allows to remove the sections and thus prevent a binary image size increase.
   * Unfortunately, gcc linker script has a limitation that does not allow conditional linkage.
   * If a slim image is wanted, one can comment the .fEraseNVM and .sEraseNVM section definitions below.
   * Alternately it is possible to leave the linker script as is with both EraseNVM sections, but modify the mcux_convert_binary
   *  post-build rule in the application CMakeLists.txt as follows:
   * mcux_convert_binary(
   *  TOOLCHAINS armgcc
   *  EXTRA_ARGS --remove-section .fEraseNVM --remove-section .sEraseNVM
   *  BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}_slim.bin)
   * This results in removing the unwanted sections from the binary output.
   */

  .fEraseNVM :
  {
    . = ALIGN(m_sector_size);
    KEEP(*(fEraseNVM))
    FILL(0xFFFFFFFF)
  } > fNVM_region

  .sEraseNVM :
  {
    . = ALIGN(m_sector_size);
    KEEP(*(sEraseNVM))
    FILL(0xFFFFFFFF)
  } > sNVM_region

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(.ramfunc*)             /* for functions in ram */
    EXCLUDE_FILE (*k32w1_nbu_ble_15_4_dyn*.bin) *(.data)                 /* .data sections */
    EXCLUDE_FILE (*k32w1_nbu_ble_15_4_dyn*.bin) *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end0__ = .;        /* define an intermediate symbol marking data end */
  } > m_data0

  .NVM_TABLE_RW : AT(__DATA_ROM + (__data_end0__ - __data_start__))
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE_RW = .);
    KEEP(*(.NVM_TABLE_RW));
    PROVIDE(__stop_NVM_TABLE_RW = .);
    . = ALIGN(4);
    __data_end__ = .;         /* define global symbol at data end for C bootstrap data initialization*/
  } > m_data0

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);

  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

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

  .interrupts_ram :
  {
    . = ALIGN(4);
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_ram_interrupts

  /* Uninitialized data section */
  .bss (NOLOAD) :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    *(g_u32NwkFrameCounter)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data0

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    PROVIDE(__HeapBase = .);
    . += HEAP_SIZE;
    PROVIDE(__HeapLimit = .);
    *(.heap)
  } > m_data0

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* ASSERT(__StackLimit >= __HeapLimit, "region m_data1 overflowed with stack and heap") */
}

