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
    examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/${core_id}/hardware_init.c
    examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/${core_id}/pin_mux.c
    examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/${core_id}/pin_mux.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    components/wifi_bt_module/incl
    ${board_root}/${board}
    examples/_boards/${board}/edgefast_bluetooth_examples/configs
    examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/${core_id}
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
)
mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DCONFIG_ARM=1\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=8192\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_UART_ADAPTER_FIFO=1\
      -DHAL_UART_DMA_ENABLE=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DLPFLEXCOMM_INIT_NOT_USED_IN_DRIVER=1\
      -DMFLASH_BASE_ADDRESS=0\
      -DSDIO_ENABLED=1"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=6329\
       --diag_suppress=6319\
       --diag_suppress=6675\
       --diag_suppress=6775\
       --legacyalign\
       --diag_suppress=3912"
)
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/MCX/MCXN/MCXN547/iar/MCXN547_cm33_core0_flash.icf
  TARGETS
    debug
    release
)
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/MCX/MCXN/MCXN547/arm/MCXN547_cm33_core0_flash.scf
  TARGETS
    debug
    release
)
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/MCX/MCXN/MCXN547/gcc/MCXN547_cm33_core0_flash.ld
  TARGETS
    debug
    release
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/linker/iar/MCXN547_cm33_core0_flash.icf
  TARGETS
    debug
    release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/linker/arm/MCXN547_cm33_core0_flash.scf
  TARGETS
    debug
    release
)
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/peripheral_ht/linker/gcc/MCXN547_cm33_core0_flash.ld
  TARGETS
    debug
    release
)
