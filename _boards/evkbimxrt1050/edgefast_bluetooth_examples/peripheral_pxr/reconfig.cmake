mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    components/wifi_bt_module/incl/wifi_bt_module_config.h
    examples/_boards/${board}/FreeRTOSConfigBoard.h
    examples/_boards/${board}/sdmmc_config.c
    examples/_boards/${board}/sdmmc_config.h
    examples/_boards/${board}/wifi_bt_config.c
    examples/_boards/${board}/wifi_bt_config.h
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs/FreeRTOSConfig.h
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr/hardware_init.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr/pin_mux.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr/pin_mux.h
    middleware/edgefast_bluetooth/output/templates/config/app_config.h
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls/mbedtls_config_client.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    components/wifi_bt_module/incl
    examples/_boards/${board}
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr
    middleware/edgefast_bluetooth/output/templates/config
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=262144\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_AUDIO_DMA_INIT_ENABLE=0\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DHAL_UART_DMA_ENABLE=1\
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DMBEDTLS_CONFIG_FILE=\\\"mbedtls_config_client.h\\\"\
      -DMFLASH_PAGE_SIZE=256\
      -DSDIO_ENABLED=1\
      -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1\
      -DgMemManagerLight=0"
)

mcux_add_macro(
  CC "-DFILE=void"
  TOOLCHAINS iar
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
mcux_add_mdk_configuration(
    LD "--keep=*(._bt_*)\
       --keep=*(._net_buf_pool*)\
       --keep=*(._settings_handler_static*)\
       --keep=mflash_drv.o(*)\
       --diag_suppress=6329\
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
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_sdram_debug
    CC "-g\
       -O0"
    CX "-g\
       -O0"
    AS "-g"
    LD "-g"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_sdram_release
    CC "-Os"
    CX "-Os"
)
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT1050/MIMXRT1052/iar/MIMXRT1052xxxxx_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT1050/MIMXRT1052/arm/MIMXRT1052xxxxx_flexspi_nor.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT1050/MIMXRT1052/gcc/MIMXRT1052xxxxx_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr/linker/iar/MIMXRT1052xxxxx_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr/linker/arm/MIMXRT1052xxxxx_flexspi_nor.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_pxr/linker/gcc/MIMXRT1052xxxxx_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
