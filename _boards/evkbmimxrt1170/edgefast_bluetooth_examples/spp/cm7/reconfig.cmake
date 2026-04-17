mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    components/wifi_bt_module/incl/wifi_bt_module_config.h
    ${board_root}/${board}/FreeRTOSConfigBoard.h
    ${board_root}/${board}/sdmmc_config.c
    ${board_root}/${board}/sdmmc_config.h
    ${board_root}/${board}/wifi_bt_config.c
    ${board_root}/${board}/wifi_bt_config.h
    examples/_boards/${board}/edgefast_bluetooth_examples/configs/FreeRTOSConfig.h
    examples/_boards/${board}/edgefast_bluetooth_examples/spp/${core_id}/hardware_init.c
    examples/_boards/${board}/edgefast_bluetooth_examples/spp/${core_id}/pin_mux.c
    examples/_boards/${board}/edgefast_bluetooth_examples/spp/${core_id}/pin_mux.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    components/wifi_bt_module/incl
    ${board_root}/${board}
    examples/_boards/${board}/edgefast_bluetooth_examples/configs
    examples/_boards/${board}/edgefast_bluetooth_examples/spp/${core_id}
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
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
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DSDIO_ENABLED=1\
      -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1\
      -DgMemManagerLight=0"
)

mcux_add_macro(
  CC "-DFILE=void"
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
  CC "-DLFS_NO_ASSERT"
  TARGETS flexspi_nor_release
  TOOLCHAINS armgcc
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
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm7_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_cm7_flexspi_nor.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/spp/linker/iar/MIMXRT1176xxxxx_cm7_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/spp/linker/arm/MIMXRT1176xxxxx_cm7_flexspi_nor.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/spp/linker/gcc/MIMXRT1176xxxxx_cm7_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
