#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    components/wifi_bt_module/incl/wifi_bt_module_config.h
    ${board_root}/${board}/FreeRTOSConfigBoard.h
    ${board_root}/${board}/sdmmc_config.c
    ${board_root}/${board}/sdmmc_config.h
    ${board_root}/${board}/wifi_bt_config.c
    ${board_root}/${board}/wifi_bt_config.h
    middleware/edgefast_open/examples/_boards/${board}/unicast_media_sender/${core_id}/hardware_init.c
    ${board_root}/${board}/wifi_examples/common/${core_id}/app.h
    ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config/lwippools.h
    ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config/lwiphooks.h
    ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config/lwipopts.h
    middleware/wifi_nxp/example/wifi_cli/wifi_config/wifi_config.h
    # BT sources (from edgefast_open unicast_media_sender board reconfig)
    middleware/edgefast_open/examples/_boards/${board}/configs/freertos/FreeRTOSConfig.h
    middleware/edgefast_open/examples/_boards/${board}/unicast_media_sender/${core_id}/pin_mux.c
    middleware/edgefast_open/examples/_boards/${board}/unicast_media_sender/${core_id}/pin_mux.h
    middleware/edgefast_open/source/bluetooth/hci/configs/mbedtls/mbedtls_config_client.h
    middleware/edgefast_open/examples/_boards/${board}/configs/mbedtls/mbedtls_user_config.h
    middleware/wireless/framework/zephyr/lib/crc/crc8_sw.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    CONFIG True
    PREINCLUDE TRUE
    SOURCES ${board_root}/${board}/coex_examples/coex_wifi_ums_open/cm7/app_config.h
)

# wifi
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\""
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/coex_examples/coex_wifi_ums_open/cm7
             ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config
             ${board_root}/${board}/wifi_examples/common/${core_id}
             middleware/wifi_nxp/example/wifi_cli/wifi_config
             components/wifi_bt_module/incl
             # BT includes (from edgefast_open unicast_media_sender board reconfig)
             middleware/edgefast_open/examples/_boards/${board}/configs/freertos
             middleware/edgefast_open/examples/_boards/${board}/unicast_media_sender/${core_id}
             middleware/edgefast_open/examples/_boards/${board}/configs/mbedtls
             middleware/wireless/framework/zephyr/lib/crc
             middleware/edgefast_open/include
             middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller
)

# edgefast / BT macros
mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U      -DPRINTF_ADVANCED_ENABLE=1      -DSDK_DEBUGCONSOLE_UART=1      -DSDK_OS_FREE_RTOS      -DSHELL_TASK_STACK_SIZE=5120      -DUSE_RTOS=1"
)

mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO      -DCOEX_APP_SUPPORT=1      -DCONFIG_ARM=1      -DEDGEFAST_BT_LITTLEFS_MFLASH      -DXIP_EXTERNAL_FLASH=1      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1      -DGATT_CLIENT      -DSHELL_ADVANCE=1      -DGATT_DB      -DHAL_AUDIO_DMA_INIT_ENABLE=0      -DHAL_UART_ADAPTER_FIFO=1      -DHAL_UART_DMA_ENABLE=1      -DIOT_WIFI_ENABLE_SAVE_NETWORK=1      -DLFS_NO_ERROR=1      -DLFS_NO_INTRINSICS=1      -DSDIO_ENABLED=1      -DCONFIG_BLE_SLIM=0      -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1      -DgMemManagerLight=0      -DCONTROLLER_INIT_ESCAPE=1      -DCONFIG_WIFI_BTDM=1"
)

mcux_add_macro(
  CC "-DLFS_NO_ASSERT"
  TARGETS flexspi_nor_release
  TOOLCHAINS armgcc
)

mcux_add_macro(
  AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
  TOOLCHAINS armgcc
)

mcux_add_armgcc_configuration(
    CC "-Wno-error=implicit-function-declaration -Wno-error"
)

# Linker scripts
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

mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/coex_examples/coex_wifi_ums_open/linker/gcc/${core_id}.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)

mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_open/examples/_boards/${board}/unicast_media_sender/linker/iar/MIMXRT1176xxxxx_cm7_flexspi_nor.icf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/edgefast_open/examples/_boards/${board}/unicast_media_sender/linker/arm/${core_id}_flexspi.scf
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400        -Xlinker --defsym=__heap_size__=0x400"
)
