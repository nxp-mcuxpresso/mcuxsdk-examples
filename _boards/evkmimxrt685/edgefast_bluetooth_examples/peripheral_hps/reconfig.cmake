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
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps/hardware_init.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps/pin_mux.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps/pin_mux.h
    middleware/edgefast_bluetooth/output/templates/config/app_config.h
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls/mbedtls_config_client.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    components/wifi_bt_module/incl
    examples/_boards/${board}
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps
    middleware/edgefast_bluetooth/output/templates/config
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DMBEDTLS_CONFIG_FILE=\\\"mbedtls_config_client.h\\\"\
      -DSDIO_ENABLED=1\
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
       --diag_suppress=6775\
       --legacyalign\
       --diag_suppress=3912"
)
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
  TARGETS
    flash_debug
    flash_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps/linker/iar/MIMXRT685Sxxxx_cm33_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps/linker/arm/MIMXRT685Sxxxx_cm33_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/peripheral_hps/linker/gcc/MIMXRT685Sxxxx_cm33_flash.ld
  TARGETS
    flash_debug
    flash_release
)
