mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    components/wifi_bt_module/incl/wifi_bt_module_config.h
    examples/_boards/${board}/FreeRTOSConfigBoard.h
    examples/_boards/${board}/sdmmc_config.c
    examples/_boards/${board}/sdmmc_config.h
    examples/_boards/${board}/wifi_bt_config.c
    examples/_boards/${board}/wifi_bt_config.h
    examples/aws_examples/common/aws_mbedtls_config.h
    examples/aws_examples/common/config_files_wifi_nxp/wifi_config.h
    examples/aws_examples/common/dev_mode_key_provisioning/include/aws_dev_mode_key_provisioning.h
    examples/aws_examples/common/dev_mode_key_provisioning/src/aws_dev_mode_key_provisioning.c
    examples/aws_examples/common/lwipopts_wifi_nxp/lwipopts.h
    examples/aws_examples/common/pkcs11/iot_pkcs11_pal.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/hardware_init.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/pin_mux.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/pin_mux.h
    middleware/edgefast_bluetooth/output/templates/config/app_config.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    components/wifi_bt_module/incl
    examples/_boards/${board}
    examples/aws_examples/common
    examples/aws_examples/common/config_files_wifi_nxp
    examples/aws_examples/common/dev_mode_key_provisioning/include
    examples/aws_examples/common/dev_mode_key_provisioning/src
    examples/aws_examples/common/lwipopts_wifi_nxp
    examples/aws_examples/common/pkcs11
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile
    middleware/edgefast_bluetooth/output/templates/config
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DAUDIO_SINK_BUFFER_NUM=2\
      -DCONFIG_ARM=1\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_AUDIO_DMA_INIT_ENABLE=0\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DHAL_UART_DMA_ENABLE=1\
      -DHAL_UART_ISR_PRIORITY=configLIBRARY_LOWEST_INTERRUPT_PRIORITY\
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DSDIO_ENABLED=1\
      -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1\
      -DgMemManagerLight=0"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=1\
      -DSKIP_SYSCLK_INIT\
      -DXIP_BOOT_HEADER_DCD_ENABLE=1\
      -DXIP_BOOT_HEADER_ENABLE=1\
      -DXIP_EXTERNAL_FLASH=1"
  TARGETS flexspi_nor_sdram_debug
  TOOLCHAINS iar
)

mcux_add_macro(
  AS "-DNDEBUG"
  CC "-DDATA_SECTION_IS_CACHEABLE=1\
      -DNDEBUG\
      -DSKIP_SYSCLK_INIT\
      -DXIP_BOOT_HEADER_DCD_ENABLE=1\
      -DXIP_BOOT_HEADER_ENABLE=1\
      -DXIP_EXTERNAL_FLASH=1"
  CX "-DNDEBUG"
  TARGETS flexspi_nor_sdram_release
  TOOLCHAINS iar
)

mcux_add_macro(
  CC "-DLFS_NO_ASSERT"
  TARGETS flexspi_nor_release
  TOOLCHAINS iar
)

mcux_add_macro(
  AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
  TOOLCHAINS armgcc
)

mcux_add_macro(
  AS "-D__STARTUP_CLEAR_BSS"
  CC "-DDATA_SECTION_IS_CACHEABLE=1\
      -DSKIP_SYSCLK_INIT\
      -DXIP_BOOT_HEADER_DCD_ENABLE=1\
      -DXIP_BOOT_HEADER_ENABLE=1\
      -DXIP_EXTERNAL_FLASH=1"
  TARGETS flexspi_nor_sdram_debug
  TOOLCHAINS armgcc
)

mcux_add_macro(
  AS "-DNDEBUG\
      -D__STARTUP_CLEAR_BSS"
  CC "-DDATA_SECTION_IS_CACHEABLE=1\
      -DNDEBUG\
      -DSKIP_SYSCLK_INIT\
      -DXIP_BOOT_HEADER_DCD_ENABLE=1\
      -DXIP_BOOT_HEADER_ENABLE=1\
      -DXIP_EXTERNAL_FLASH=1"
  CX "-DNDEBUG"
  TARGETS flexspi_nor_sdram_release
  TOOLCHAINS armgcc
)

mcux_add_macro(
  CC "-DLFS_NO_ASSERT"
  TARGETS flexspi_nor_release
  TOOLCHAINS armgcc
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=1\
      -DSKIP_SYSCLK_INIT\
      -DXIP_BOOT_HEADER_DCD_ENABLE=1\
      -DXIP_BOOT_HEADER_ENABLE=1\
      -DXIP_EXTERNAL_FLASH=1"
  TARGETS flexspi_nor_sdram_debug
  TOOLCHAINS mdk
)

mcux_add_macro(
  AS "-DNDEBUG"
  CC "-DDATA_SECTION_IS_CACHEABLE=1\
      -DNDEBUG\
      -DSKIP_SYSCLK_INIT\
      -DXIP_BOOT_HEADER_DCD_ENABLE=1\
      -DXIP_BOOT_HEADER_ENABLE=1\
      -DXIP_EXTERNAL_FLASH=1"
  CX "-DNDEBUG"
  TARGETS flexspi_nor_sdram_release
  TOOLCHAINS mdk
)
mcux_add_iar_configuration(
    CX "--diag_suppress Pa050,Pa082,Pe186,Pe815,Pa167,Pe177,Pe546"
    LD "--semihosting"
)
mcux_add_iar_configuration(
    TARGETS flexspi_nor_sdram_debug
    AS "-r"
    CC "-Om\
       --debug\
       --no_cse\
       --no_unroll\
       --no_inline\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
    CX "--debug\
       --no_cse\
       --no_unroll\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
)
mcux_add_iar_configuration(
    TARGETS flexspi_nor_sdram_release
    CC "-Om"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_sdram_debug
    CC "-g\
       -O0"
    CX "-g\
       -O0"
    LD "-g"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_sdram_release
    CC "-Os"
    CX "-Os"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=6329\
       --diag_suppress=6319\
       --diag_suppress=6675\
       --diag_suppress=6775\
       --legacyalign\
       --diag_suppress=3912"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_sdram_debug
    CC "-O1\
       -g"
    CX "-O1\
       -g"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_sdram_release
    CC "-Oz"
    CX "-Oz"
)
mcux_remove_iar_configuration(
  CX --diag_suppress
)
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT1040/MIMXRT1042/iar/MIMXRT1042xxxxx_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT1040/MIMXRT1042/arm/MIMXRT1042xxxxx_flexspi_nor.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT1040/MIMXRT1042/gcc/MIMXRT1042xxxxx_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/linker/iar/MIMXRT1042xxxxx_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/linker/arm/MIMXRT1042xxxxx_flexspi_nor.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/linker/gcc/MIMXRT1042xxxxx_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
