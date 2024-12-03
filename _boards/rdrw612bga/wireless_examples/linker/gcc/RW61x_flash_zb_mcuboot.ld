/*
** ###################################################################
**     Processors:          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
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

/************************ Flash Layout with OTA enabled *********************

             0x0800_0000    m_flash_start
    - - - +---------------+ - - - - - - - - - - - - - - - - - - - -
          |               |
          |    MCUBoot    |
   128k   |   bootloader  |
          |               |
    - - - +---------------+ m_slot0_partition_start = FW_ACTIVE_APP_START
   4k     |  Header_slot0 |
          +---------------+ m_app_start - - - - - - - - - - - - - -
          |               |                                 '
 m_app    |               |                                 '
_size     |     m_app     |                                 '
          |               |                                 '   ACTIVE APP (slot 0)
          |               |                                 '        (4.4 MB)
          |               |                                 '
          +---------------+                                 '
   4k     | Trailer_slot0 |                                 '
          +---------------+                                 '
          |    padding    |                                 '
    - - - +---------------+ m_slot1_partition_start = FW_UPDATE_STORAGE_START
          |               |                                 '
          |               |                                 '
          |               |                                 '
          |               |                                 '
          |               |                                 '
          |      OTA      |                                 '
          |     Update    |                                 '   CANDIDATE_APP (slot 1)
          |     Image     |                                 '        (4.4 MB)
          |               |                                 '
          |               |                                 '
          |               |                                 '
          |               |                                 '
          |               |                                 '
    - - - +---------------+ - - - - - - - - - - - - - - - - - - - -
          +               +
          +               +
    - - - +---------------+ - - - - - - - - - - - - - - - - - - - -
          |               | NV_STORAGE_START_ADDRESS
          |      NVS      |
          |               | NV_STORAGE_END_ADDRESS
    - - - +---------------+ - - - - - - - - - - - - - - - - - - - -
    4k    |  Reserved     |
    - - - +---------------+ - - - - - - - - - - - - - - - - - - - -
             0x0C00_0000    m_flash_end

/* Entry Point */
ENTRY(Reset_Handler)

/* 64MB Flash */
m_flash_start                       = 0x08000000;
m_flash_size                        = 0x4000000;
m_flash_end                         = m_flash_start + m_flash_size - 1;
m_flash_sector_size                 = 0x1000;

/* We reserve 128K for MCUBoot, which includes the flash configuration */
m_boot_partition_start = m_flash_start;
m_boot_partition_size  = 0x20000;
m_boot_partition_end   = m_boot_partition_start + m_boot_partition_size - 1;

/* the slot size is the same no matter the slot */
m_slot_partition_size = 0x440000;

/* slot0 is the partition used for the active image
 * This MUST match MCUBoot partition configuration (fsl_partitioning.c/h) */
m_slot0_partition_start = 0x08020000;
m_slot0_partition_end   = m_slot0_partition_start + m_slot_partition_size - 1;

/* slot1 is the partition used for the image upgrade
 * This MUST match MCUBoot partition configuration (fsl_partitioning.c/h) */
m_slot1_partition_start = 0x08460000;
m_slot1_partition_end   = m_slot1_partition_start + m_slot_partition_size - 1;

/* MCUBoot header size is set to 1 sector size and is placed before the
 * active image, so we have to take into account to offset the image */
m_mcuboot_img_hdr_sz = m_flash_sector_size;

/* We reserve 1 sector for MCUBoot's trailer
 * The image size is at most the slot partition size minus 1 sector for the trailer */
m_app_start = m_slot0_partition_start + m_mcuboot_img_hdr_sz;
m_app_size  = m_slot_partition_size - m_flash_sector_size;

/* This symbol is used by zigbee OTA and should be set to the beginning of the image
 * It is used to compute offsets to OTA header, link key and nonce */
_flash_start = m_slot0_partition_start;

/* The active image section contains the MCUBoot header + application image + MCUBoot trailer */
FW_ACTIVE_APP_START                 = m_slot0_partition_start;
FW_ACTIVE_APP_OFFSET                = FW_ACTIVE_APP_START - m_flash_start;
FW_ACTIVE_APP_SIZE                  = m_app_size;

/* Candidate image storage - same size than the active image */
FW_UPDATE_STORAGE_START             = m_slot1_partition_start;
FW_UPDATE_STORAGE_SIZE              = FW_ACTIVE_APP_SIZE;
FW_UPDATE_STORAGE_OFFSET            = FW_UPDATE_STORAGE_START - m_flash_start;

/* Used for NVS - the last flash sector is usually reserved for factory data section */
NV_STORAGE_END_ADDRESS              = m_flash_end - m_flash_sector_size;
NV_STORAGE_MAX_SECTORS              = DEFINED(gUseNVMLink_d) ? (DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 8) : 0;
NV_STORAGE_SECTOR_SIZE              = m_flash_sector_size;
NV_STORAGE_SIZE                     = NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE;
NV_STORAGE_START_ADDRESS            = NV_STORAGE_END_ADDRESS - NV_STORAGE_SIZE + 1;
NV_STORAGE_START_ADDRESS_OFFSET     = NV_STORAGE_START_ADDRESS - m_flash_start;

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x1000;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x1000;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000280 : 0;

/* Specify the memory areas */
MEMORY
{
  m_app                (RX)  : ORIGIN = m_app_start, LENGTH = m_app_size
  /* 0x2012_3000 - 0x2012_FFFF is reserved for BootROM usage */
  m_data                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00123000
  m_mbox1               (RW)  : ORIGIN = 0x41380000, LENGTH = 0x00000488
  m_txq1                (RW)  : ORIGIN = 0x41380488, LENGTH = 0x00001000
  m_mbox2               (RW)  : ORIGIN = 0x443C0000, LENGTH = 0x00000488
  m_txq23               (RW)  : ORIGIN = 0x443C0488, LENGTH = 0x00001080
  m_txq32               (RW)  : ORIGIN = 0x443C1508, LENGTH = 0x00001080
}

/* Define output sections */
SECTIONS
{
  /* We need the ISR vector table at the very beginning of the active image so MCUBoot can jump into it */
  .interrupts :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __Vectors = .;
    KEEP(*(.isr_vector))     /* Startup code */
    FILL(0x00)
    . = 0x280;
  } > m_app

  /* ZB OTA related sections, should not consume any memory if .ro_nonce/.ro_ota_header/.ro_se_lnkkey not used */
  .ro_nonce : ALIGN(0x10)
  {
    _FlsNonceStart = ABSOLUTE(.);
    *(.ro_nonce) /* nonce value is 16 bytes.*/
    FILL(0xff)
    . = ALIGN (0x10);
  } > m_app

  .ro_ota_header : ALIGN(0x10)
  {
    _enc_start = ABSOLUTE(.);
    _enc_offset = (_enc_start & 0x0000000F);
    _FlsOtaHeader = ABSOLUTE(.);
    *(.ro_ota_header) /* Ota Header 69 bytes*/
    FILL(0xff)
    . = ALIGN (0x10);
  } > m_app

  .ro_se_lnkKey (ALIGN((. - _enc_offset), 16) + _enc_offset):
  {
    _FlsLinkKey = ABSOLUTE(.);
    *(.ro_se_lnkKey)  /* Link Key 16 bytes*/
    FILL(0xff)
    . = ALIGN (0x10);
  } > m_app

  /* The program code and other data goes into internal ram */
  .text :
  {
    . = ALIGN(4);
    EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        *fsl_adapter_flexspi_nor_flash.c.obj
        *NV_Flash.c.obj
        *mflash_drv.c.obj
        *fsl_flexspi.c.obj
        *port.c.obj
        *portasm.c.obj
        *queue.c.obj
        *timers.c.obj
        *tasks.c.obj
        *inet_chksum.c.obj
        *ip4.c.obj
        *ip6.c.obj
        *pbuf.c.obj
        *sys_arch.c.obj
        *tcp_out.c.obj
        *tcp_in.c.obj
        *mlan_11n_rxreorder.c.obj
        *wifi.c.obj
        *wifi-imu.c.obj
        *fsl_adapter_imu.c.obj
        *fsl_cache.c.obj
        *fsl_os_abstraction_free_rtos.c.obj
        *libethermind_ble_lc3_ndsp_common.a:*
        *libethermind_ble_lc3_ndsp_decoder.a:*
        *libethermind_ble_lc3_ndsp_encoder.a:*
    )
    *(.text* .rodata*)
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    KEEP(*(.fw_cpu2_combo))
    . = ALIGN(4);
    /* When using MCUBoot, the flash_config is not necessary but for convenience
    * we can still try to place it without actually filling the empty space like
    * done in a standalone image */
    . = ALIGN(4);
    KEEP(* (.flash_conf))
    . = ALIGN(4);
  } > m_app

  ._bt_settings_area :
  {
    . = ALIGN(4);
    _bt_settings_start = .;
    KEEP(*(SORT(._bt_settings.static.*)))
    _bt_settings_end = .;
    . = ALIGN(4);
  } > m_app

  ._settings_handler_static :
  {
    . = ALIGN(4);
    _settings_handler_static_list_start = .;
    KEEP(*(SORT(._settings_handler_static.static.*)))
    _settings_handler_static_list_end = .;
    . = ALIGN(4);
  } > m_app

  ._bt_conn_cb :
  {
    . = ALIGN(4);
    _bt_conn_cb_list_start = .;
    KEEP(*(SORT(._bt_conn_cb.static.*)))
    _bt_conn_cb_list_end = .;
    . = ALIGN(4);
  } > m_app

  ._bt_gatt_service_static :
  {
    . = ALIGN(4);
    _bt_gatt_service_static_list_start = .;
    KEEP(*(SORT(._bt_gatt_service_static.static.*)))
    _bt_gatt_service_static_list_end = .;
    . = ALIGN(4);
  } > m_app

  ._bt_l2cap_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_fixed_chan.static.*)))
    _bt_l2cap_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > m_app

  ._bt_l2cap_br_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_br_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_br_fixed_chan.static.*)))
    _bt_l2cap_br_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > m_app

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_app

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_app

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
  } > m_app

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_app

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_app

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_app

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_app

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

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_app);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(m_usb_dma_init_data)
    /* Explicit placement of flash and frequently executed functions in RAM */
    *fsl_adapter_flexspi_nor_flash.c.obj(.text* .rodata*)
    *mflash_drv.c.obj(.text* .rodata*)
    *fsl_flexspi.c.obj(.text* .rodata*)
    *port.c.obj(.text* .rodata*)
    *portasm.c.obj(.text* .rodata*)
    *queue.c.obj(.text* .rodata*)
    *timers.c.obj(.text* .rodata*)
    *tasks.c.obj(.text* .rodata*)
    *inet_chksum.c.obj(.text* .rodata*)
    *ip4.c.obj(.text* .rodata*)
    *ip6.c.obj(.text* .rodata*)
    *pbuf.c.obj(.text* .rodata*)
    *sys_arch.c.obj(.text* .rodata*)
    *tcp_out.c.obj(.text* .rodata*)
    *tcp_in.c.obj(.text* .rodata*)
    *mlan_11n_rxreorder.c.obj(.text* .rodata*)
    *wifi.c.obj(.text* .rodata*)
    *wifi-imu.c.obj(.text* .rodata*)
    *fsl_adapter_imu.c.obj(.text* .rodata*)
    *fsl_cache.c.obj(.text* .rodata*)
    *fsl_os_abstraction_free_rtos.c.obj(.text* .rodata*)
    *libethermind_ble_lc3_ndsp_common.a:*(.text* .rodata*)
    *libethermind_ble_lc3_ndsp_decoder.a:*(.text* .rodata*)
    *libethermind_ble_lc3_ndsp_encoder.a:*(.text* .rodata*)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    _k_mem_slab_list_start = .;
    KEEP(*(SORT(._k_mem_slab*)))
    _k_mem_slab_list_end = .;
    . = ALIGN(4);
    _net_buf_pool_list = .;
    KEEP(*(SORT(._net_buf_pool*)))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
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

  .smu_cpu13_mbox (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu13_mbox)
    KEEP (*(.smu_cpu13_mbox))
    . = ALIGN(4);
  } > m_mbox1

  .smu_cpu31_txq (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu31_txq)
    KEEP (*(.smu_cpu31_txq))
    . = ALIGN(4);
  } > m_txq1

  .smu_cpu23_mbox (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu23_mbox)
    KEEP (*(.smu_cpu23_mbox))
    . = ALIGN(4);
  } > m_mbox2

  .smu_cpu32_txq (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu32_txq)
    KEEP (*(.smu_cpu32_txq))
    . = ALIGN(4);
  } > m_txq32

}

