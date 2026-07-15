mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    ${board_root}/${board}/FreeRTOSConfigBoard.h
    ${board_root}/${board}/sdmmc_config.c
    ${board_root}/${board}/sdmmc_config.h
    ${board_root}/${board}/wifi_bt_config.c
    ${board_root}/${board}/wifi_bt_config.h
    ${board_root}/${board}/wifi_examples/common/${core_id}/app.h
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/freertos_config/FreeRTOSConfig.h
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/${core_id}/hardware_init.c
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/${core_id}/pin_mux.c
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/${core_id}/pin_mux.h
    middleware/edgefast_open/source/bluetooth/hci/configs/mbedtls/mbedtls_config_client.h
    middleware/edgefast_open/examples/_boards/${board}/configs/mbedtls/mbedtls_user_config.h
    middleware/wifi_nxp/example/wifi_cli/lwip_config/lwiphooks.h
    middleware/wifi_nxp/example/wifi_cli/lwip_config/lwipopts.h
    middleware/wifi_nxp/example/wifi_cli/lwip_config/lwippools.h
    middleware/wifi_nxp/example/wifi_cli/wifi_config/wifi_config.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    ${board_root}/${board}
    ${board_root}/${board}/wifi_examples/common/${core_id}
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/freertos_config
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open
    ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/${core_id}
    middleware/edgefast_open/examples/_boards/${board}/configs/mbedtls
    middleware/wifi_nxp/example/wifi_cli/wifi_config
    middleware/wifi_nxp/example/wifi_cli/lwip_config
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
  LINKER middleware/edgefast_open/examples/_boards/${board}/shell/linker/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
  TARGETS
    flash_debug
    flash_release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_open/examples/_boards/${board}/shell/linker/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
  TARGETS
    flash_debug
    flash_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_open/examples/_boards/${board}/shell/linker/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
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
      -DCONFIG_BT_GATT_DYNAMIC_DB=1\
      -DDATA_SECTION_IS_CACHEABLE=1\
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
      -DSDIO_ENABLED=1\
      -DgMemManagerLight=0\
      -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\""
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
