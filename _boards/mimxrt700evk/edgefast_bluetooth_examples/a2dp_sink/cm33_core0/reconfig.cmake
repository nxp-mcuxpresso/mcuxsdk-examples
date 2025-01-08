mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    ${board_root}/${board}/FreeRTOSConfigBoard.h
    ${board_root}/${board}/sdmmc_config.c
    ${board_root}/${board}/sdmmc_config.h
    ${board_root}/${board}/wifi_bt_config.c
    ${board_root}/${board}/wifi_bt_config.h
    examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/${core_id}/hardware_init.c
    examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/${core_id}/pin_mux.c
    examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/${core_id}/pin_mux.h
    examples/_boards/${board}/edgefast_bluetooth_examples/configs/FreeRTOSConfig.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    ${board_root}/${board}
    examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/${core_id}
    examples/_boards/${board}/edgefast_bluetooth_examples/configs
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
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/linker/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
  TARGETS
    flash_debug
    flash_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/linker/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/linker/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DCONFIG_ARM=1\
      -DDATA_SECTION_IS_CACHEABLE=1\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DI2S_NUM_BUFFERS=6\
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1\
      -DgMemManagerLight=0"
)

mcux_add_macro(
  CC "-DFILE=void"
  TOOLCHAINS iar
)

mcux_add_macro(
  CC "-DLFS_NO_ASSERT"
  TARGETS flash_release
  TOOLCHAINS iar
)

mcux_add_macro(
  AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
  TOOLCHAINS armgcc
)

mcux_add_macro(
  CC "-DLFS_NO_ASSERT"
  TARGETS flash_release
  TOOLCHAINS armgcc
)
