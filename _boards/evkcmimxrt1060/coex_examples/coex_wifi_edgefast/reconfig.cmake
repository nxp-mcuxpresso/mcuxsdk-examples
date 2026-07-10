#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES components/wifi_bt_module/incl/wifi_bt_module_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
            middleware/wireless/coex/build/${board}/common/hardware_init.c
            middleware/wireless/coex/build/${board}/common/app.h
            ${board_root}/${board}/coex_examples/coex_wifi_edgefast/pin_mux.c
            ${board_root}/${board}/coex_examples/coex_wifi_edgefast/pin_mux.h
            middleware/wireless/coex/src/configs/mimxrt1062/mbedtls/mbedtls_config_client.h
            middleware/wireless/coex/src/configs/mimxrt1062/wifi/wifi_config.h
            middleware/wireless/coex/src/configs/mimxrt1062/lwip/lwippools.h
            middleware/wireless/coex/src/configs/mimxrt1062/lwip/lwiphooks.h
            middleware/wireless/coex/src/configs/mimxrt1062/lwip/lwipopts.h
            middleware/wireless/coex/src/common/controller_coex_nxp.c
            middleware/wireless/coex/src/common/coex_nb_uart_fw_download.c
            middleware/wireless/coex/src/common/coex_nb_uart_fw_download.h
            middleware/wireless/coex/src/edgefast/coex_shell.c
            middleware/wireless/coex/src/edgefast/coex_shell.h
            middleware/wireless/coex/third_party/platform/rt1060/ot_config.cmake
            examples/coex_examples/coex_wifi_edgefast/app_config.cmake
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    CONFIG True
    PREINCLUDE TRUE
    SOURCES ${board_root}/${board}/coex_examples/coex_wifi_edgefast/app_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/wifi_examples/common
             ${board_root}/${board}/coex_examples/coex_wifi_edgefast
             middleware/wireless/coex/src/configs/mimxrt1062/edgefast
             middleware/wireless/coex/src/configs/mimxrt1062/wifi
             middleware/wireless/coex/src/configs/mimxrt1062/lwip
             middleware/wireless/coex/src/configs/mimxrt1062/mbedtls
             middleware/edgefast_open/examples/_boards/${board}/configs/mbedtls
             middleware/wireless/coex/src/edgefast
             middleware/wireless/coex/src/common
             middleware/wireless/coex/build/${board}/common
             examples/coex_examples/coex_wifi_edgefast
             components/wifi_bt_module/incl
)

# edgefast
mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSHELL_TASK_STACK_SIZE=5120\
      -DUSE_RTOS=1"
)

mcux_add_macro(
  CC "-DAPPL_USE_STANDARD_IO\
      -DCOEX_APP_SUPPORT=1\
      -DCONFIG_ARM=1\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DXIP_EXTERNAL_FLASH=1\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DSHELL_ADVANCE=1\
      -DGATT_DB\
      -DRAM_DISK_ENABLE=1\
      -DHAL_AUDIO_DMA_INIT_ENABLE=0\
      -DHAL_UART_DMA_ENABLE=1\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DSDIO_ENABLED=1\
      -DCONFIG_BLE_SLIM=0\
      -DgMemManagerLight=0"
)

# wifi
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
       -DCONFIG_WPA_SUPP_CRYPTO_MBEDTLS_PSA\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_config_client.h\\\"\
       -DMBEDTLS_WPA_SUPPLICANT_CONFIG_FILE=\\\"wpa_supp_mbedtls_config.h\\\"\
       -DCONFIG_HOSTAPD=0"
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
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_release
    AS "-g"
    CC "-g -Wno-array-bounds"
    CX "-g"
)

mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)

mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER middleware/wireless/coex/build/${board}/linker/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
  TARGETS
    flexspi_nor_debug
    flexspi_nor_release
)
