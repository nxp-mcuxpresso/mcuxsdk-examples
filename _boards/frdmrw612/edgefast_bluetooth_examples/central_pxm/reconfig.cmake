mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/_boards/${board}/FreeRTOSConfigBoard.h
    examples/_boards/${board}/monolithic_config.h
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm/hardware_init.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm/pin_mux.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm/pin_mux.h
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs/FreeRTOSConfig.h
    middleware/edgefast_bluetooth/output/templates/config/app_config.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    examples/_boards/${board}
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs
    middleware/edgefast_bluetooth/output/templates/config
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DBT_PLATFORM\
      -DCONFIG_ARM=1\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_AUDIO_DMA_INIT_ENABLE=0\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DgMemManagerLight=0"
)

mcux_add_macro(
  CC "-DFILE=void"
  TOOLCHAINS iar
)
mcux_add_mdk_configuration(
    LD "--keep=*(._bt_*)\
       --keep=*(._net_buf_pool*)\
       --keep=*(._settings_handler_static*)\
       --keep=mflash_drv.o(*)\
       --diag_suppress=6329\
       --diag_suppress=6319\
       --diag_suppress=6675\
       --diag_suppress=6775"
)
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/Wireless/RW/RW612/arm/RW612_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
  TARGETS
    flash_debug
    flash_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm/linker/iar/RW610_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm/linker/arm/RW610_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_pxm/linker/gcc/RW610_flash.ld
  TARGETS
    flash_debug
    flash_release
)
