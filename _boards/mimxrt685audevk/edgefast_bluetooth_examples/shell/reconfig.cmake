mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    components/wifi_bt_module/incl/wifi_bt_module_config.h
    ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
    ${board_root}/${board}/FreeRTOSConfigBoard.h
    ${board_root}/${board}/sdmmc_config.c
    ${board_root}/${board}/sdmmc_config.h
    ${board_root}/${board}/wifi_bt_config.c
    ${board_root}/${board}/wifi_bt_config.h
    examples/_boards/${board}/edgefast_bluetooth_examples/configs/FreeRTOSConfig.h
    examples/_boards/${board}/edgefast_bluetooth_examples/shell/hardware_init.c
    examples/_boards/${board}/edgefast_bluetooth_examples/shell/pin_mux.c
    examples/_boards/${board}/edgefast_bluetooth_examples/shell/pin_mux.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    components/wifi_bt_module/incl
    ${board_root}/${board}
    examples/_boards/${board}/edgefast_bluetooth_examples/configs
    examples/_boards/${board}/edgefast_bluetooth_examples/shell
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DNVM_NO_COMPONNET=1\
      -DSDIO_ENABLED=1\
      -DSDK_COMPONENT_INTEGRATION=1\
      -DgMemManagerLight=0\
      -DgNvStorageIncluded_d=1\
      -DgNvTableKeptInRam_d=1"
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
  LINKER ${device_root}/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
  TARGETS
    flash_debug
    flash_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/shell/linker/iar/MIMXRT685Sxxxx_cm33_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/shell/linker/arm/MIMXRT685Sxxxx_cm33_flash.scf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/shell/linker/gcc/MIMXRT685Sxxxx_cm33_flash.ld
  TARGETS
    flash_debug
    flash_release
)
