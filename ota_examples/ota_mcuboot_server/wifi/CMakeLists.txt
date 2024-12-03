cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ota_mcuboot_server_wifi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ota_examples/ota_mcuboot_server/wifi)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/ota_mcuboot_server/ota_mcuboot_server.c
            examples/ota_examples/ota_mcuboot_server/httpsrv_multipart.c
            examples/ota_examples/ota_mcuboot_server/httpsrv_multipart.h
            examples/_boards/${board}/ota_examples/ota_mcuboot_server/wifi/network_cfg.h
            examples/ota_examples/ota_mcuboot_server/network_wifi_nxp/init_wifi.c
            examples/ota_examples/ota_mcuboot_server/network_wifi_nxp/lwipopts.h
            examples/ota_examples/ota_mcuboot_client/network_wifi_nxp/wifi_config.h
            examples/ota_examples/ota_mcuboot_server/httpsrv_fs_data.c
            examples/ota_examples/ota_mcuboot_server/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/ota_examples/ota_mcuboot_server/mbedtls_config.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_mcuboot_server
             examples/ota_examples/ota_mcuboot_server/network_wifi_nxp
             examples/_boards/${board}/ota_examples/ota_mcuboot_server
             examples/_boards/${board}/ota_examples/ota_mcuboot_server/wifi
             examples/_boards/${board}/wifi_examples/common/app_config/
             examples/_boards/${board}/ota_examples/mcuboot_opensource
             examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning
             examples/_boards/${board}
             examples/_boards/${board}/ota_examples/ota_mcuboot_server/wifi/${core_id}

)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.c
            examples/_boards/${board}/ota_examples/mcuboot_opensource/sblconfig.h
            examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
)


mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-g1"
    CX "-g1"
)
mcux_add_mcux_configuration(
    TARGETS debug
    CC "-Og"
    CX "-g3"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
)

mcux_add_macro(
    CC "-DMCUBOOT_APPLICATION=1\
       -DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DHTTPSRV_CFG_WEBSOCKET_ENABLED=0"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-D_POSIX_SOURCE"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)
mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)

include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/ota_mcuboot_server/wifi/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
